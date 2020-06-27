#include "KVF.hh"
//#include "KVFParticleEnergyKat.hh"
#include "KltRunbaseUI.hh"
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <string>

#include "TROOT.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"

using namespace std;

float eventRadius(KVFGeneralEvent* event);
float deltaR(KVFGeneralEvent* pro, KVFGeneralEvent* del);

int main(int argc, char* argv[])
{
  //  cerr << "ok1" << endl;
  KVFGeneralEvent::setECorrMode(false);
  KVFParticleEnergy::PositronExtrapolate();

  //string RunInfoFile  = "./table/run-info-11947.table";
  //string RunTrigFile  = "./table/trigger_conditions_11947.dat";

  string RunInfoFile  = "./table/run-info.table-160616";
  string RunTrigFile  = "./table/trigger_conditions.dat";
  
  if (KVFRunInfo::ReadTable(RunInfoFile.c_str()) == false) {
    cerr << "Cannot set run info table" << endl;
    abort();
  }
  
  if (KVFRunInfo::ReadRunInfoTable(RunTrigFile.c_str()) == false) {
    cerr << "Cannot set run info table" << endl;
    abort();
  }
  
  KltRunbaseUI deadtime_manager;

  vector<KVFGeneralEvent> muon; 
  muon.reserve(32000);

  KVFGeneralEvent* DelayedEvent = new KVFGeneralEvent[200000];
  
  for(int i=0;i<200000;i++){
    DelayedEvent[i].enablePositionDependentEnergyCorrection();
  }

  int deadtime_veto, full_volume_veto, track_veto, badness_veto;
  int run, pevent, devent, unixtime;
  float Ep, Ed, Rp, Rd, Xp,Yp,Zp, Xd,Yd,Zd, dR, Rxyd, energy;
  float xp, yp, zp, xd, yd, zd, rp, rd;
  double dT;

  KVFReader gvf(argv[2]);
    
  if(gvf.bad()){
    cerr<<"ERROR: Cannot open file"<<endl;
    return (-1);
  }    

  int RunNumber = 0;

  KVFGeneralEvent tmp_muon;
  muon.clear();
  for(gvf.begin(&tmp_muon); gvf.couldRead(); gvf.next(&tmp_muon)){

    // select muon event
    if (tmp_muon.isThisMuon() == false) continue;
      
    muon.push_back(tmp_muon);
    muon[muon.size()-1].TimeStamp = tmp_muon.TimeStamp;
    RunNumber = tmp_muon.RunNumber;
  }

  if( KVFRunInfo::HasReactorInfo(RunNumber) == false ){
    cout<<"read: "<<argv[2]<<endl;
    cout<<" --> no reactor info"<<endl;
    cout<<endl;
      
    return 0;
  }

  int run_grade = KVFRunInfo::Grade(RunNumber);
  if(run_grade<0 || run_grade>=10){
    cout<<"read: "<<argv[2]<<endl;
    cout<<" --> badrun"<<endl;
    cout<<endl;
      
    return 0;
  }
  
  TFile rootf(argv[1], "RECREATE");
  
  TROOT root("root", "test");
  
  TH1F* h_large_energy = new TH1F("h_large_energy", "", 3000, 0, 30.0);//10keV bin
  //TH1F* h_large_energy = new TH1F("h_large_energy", "", 1000, 0, 10.0);//10keV bin

  TH1F* h_large_dT = new TH1F("h_large_dT", "", 2000, 0, 20);//0.01sec bin

  static const double R_balloon = 650.0;
  static const double p0 = 55.18;
  static const double p1 = -0.0509;
  static const double p2 = 613.3;

  int muon_index=0;
    
  int NofDelayedEvent = 0;
    
  for (gvf.begin(&DelayedEvent[NofDelayedEvent]); gvf.couldRead(); gvf.next(&DelayedEvent[NofDelayedEvent])) {
    // select low energy event
    if (DelayedEvent[NofDelayedEvent].isThisMuon() == true) continue;
    if (DelayedEvent[NofDelayedEvent].isThisFlasher() == true) continue;      
      
    if(DelayedEvent[NofDelayedEvent].isNoise()) continue;

    //Ed       = DelayedEvent[NofDelayedEvent].EnergyA2;
    Ed       = DelayedEvent[NofDelayedEvent].Energy;
    Xd       = DelayedEvent[NofDelayedEvent].X;
    Yd       = DelayedEvent[NofDelayedEvent].Y;
    Zd       = DelayedEvent[NofDelayedEvent].Z;
    Rd       = sqrt(Xd*Xd+Yd*Yd+Zd*Zd);

    if(!(Ed>=1.8)) continue;
    if(!(Rd<600)) continue;
    //if(!(Rd<550)) continue;
      
    NofDelayedEvent++;
  }

  int last_event = 0;

  int NofAccidental_NoCylinderCut = 0;
  int NofAccidental_LAT = 0;
  int NofAccidental_HAT = 0;

  KVFGeneralEvent prompt;

  prompt.enablePositionDependentEnergyCorrection();

  int N=0;
  unsigned long long int TimeStampOfLastHighEnergyEvent=0;
  for (gvf.begin(&prompt); gvf.couldRead(); gvf.next(&prompt)) {
      
    // select low energy event
    if (prompt.isThisMuon() == true) continue;
    if (prompt.isThisFlasher() == true) continue;      
      
    if(prompt.isNoise()) continue;
      
    int NsumMaxThreshold;
      
    NsumMaxThreshold = KVFRunInfo::Prompt(RunNumber);
      
    if (prompt.NsumMax < NsumMaxThreshold) continue;  
      
    //if(prompt.EnergyA2 >20) TimeStampOfLastHighEnergyEvent=prompt.TimeStamp; 
    if(prompt.Energy >20) TimeStampOfLastHighEnergyEvent=prompt.TimeStamp;
   //energy   = KVFParticleEnergy::Positron(prompt.EnergyA2, RunNumber);
    //    energy   = KVFParticleEnergyKat::Positron(prompt.Energy);
    energy   = KVFParticleEnergy::Positron(prompt.Energy, RunNumber);

    double CorrectionFactor = 1.0;
    
    if(energy>7.5){
      CorrectionFactor = R_balloon / (p0 * exp(p1 * energy) + p2);
    }

    xp = prompt.X * CorrectionFactor;
    yp = prompt.Y * CorrectionFactor;
    zp = prompt.Z * CorrectionFactor;

    double Rp       = sqrt(xp*xp+yp*yp+zp*zp);
    
//     Xp       = prompt.X;
//     Yp       = prompt.Y;
//     Zp       = prompt.Z;
//     Rp       = sqrt(Xp*Xp+Yp*Yp+Zp*Zp);
    if(Rp>600) continue;//6m fiducial
    //if(Rp>550) continue;//6m fiducial

    //if(energy>20) continue;
    if(energy>30) continue;

    static const unsigned long long int TwoSec = 80000000; //2sec
    static const unsigned long long int OneSec = 40000000; //1sec
    static const unsigned long long int TwoMSec = 80000; //2msec

    //
    // muon veto (for prompt)
    //
    if(muon_index>=(int)muon.size())muon_index=(int)muon.size()-1;
    if(muon_index<0) muon_index=0;
    if(muon[muon_index].TimeStamp +TwoSec > prompt.TimeStamp && muon_index>0){
      for(muon_index=muon_index-1; muon_index>=0; muon_index--){
	if(muon[muon_index].TimeStamp + TwoSec < prompt.TimeStamp){
	  muon_index++;
	  break;
	}
      }

    } else if(muon[muon_index].TimeStamp + TwoSec < prompt.TimeStamp 
	      && muon_index < (int)muon.size()-1 ){
      for(muon_index=muon_index+1; muon_index<(int)muon.size(); muon_index++){
	if(muon[muon_index].TimeStamp + TwoSec >= prompt.TimeStamp) break;
      }
    }
    if(muon_index>=(int)muon.size())muon_index=(int)muon.size()-1;
    int muon_veto_prompt1=0; // within 2msec after muon
    int muon_veto_prompt2=0; // within 1sec after muon
    int muon_veto_prompt3=0; // bad muon fit
    int muon_veto_prompt4=0; // muon with shower -> veto 2sec whole detector
    //                // other       muon -> veto 2sec along muon-trak within 3m
    if(muon_index<0) muon_index=0;
    int event_number_of_first_near_muon=0;
    int index_of_nearest_muon=muon_index;
    int number_of_near_muon=0;
    if(muon.size()>0 && muon_index>=0){
      for(int j=muon_index; j<(int)muon.size(); j++){
	if(muon[j].TimeStamp > prompt.TimeStamp) break;

	number_of_near_muon++;
	index_of_nearest_muon=muon_index;
	if(event_number_of_first_near_muon==0) 
	  event_number_of_first_near_muon=muon[j].EventNumber;

	if(muon[j].TimeStamp + TwoMSec >= prompt.TimeStamp) muon_veto_prompt1++;
	    
	if(muon[j].TimeStamp + OneSec > prompt.TimeStamp) muon_veto_prompt2++;
      }
    }
    if( muon_veto_prompt1>0) continue;

    //
    // delayed coincidence selection
    //
    //static const unsigned long long int CoincidenceGateWidth= 100000; //2.5msec
    static const unsigned long long int CoincidenceGateWidth= 800000000; //20sec
    static const double CoincidenceSpace= 200.0; //2m

    //find first event after prompt event
    while(true){
      if(DelayedEvent[last_event].TimeStamp > prompt.TimeStamp) break;
      if(last_event+1>=NofDelayedEvent) break;
      last_event++;
    }      
      
    for(int k=last_event; k<NofDelayedEvent; k++){

      if(DelayedEvent[k].TimeStamp <= prompt.TimeStamp) continue;
	
      // select low energy event
      if (DelayedEvent[k].isThisMuon() == true) continue;
      if (DelayedEvent[k].isThisFlasher() == true) continue;      
	
      if(DelayedEvent[k].TimeStamp > (prompt.TimeStamp+CoincidenceGateWidth))break;
      if(DelayedEvent[k].isNoise()) continue;
      //if(DelayedEvent[k].EnergyA2<1) continue;
      if(DelayedEvent[k].Energy<1) continue;
      //if(deltaR(&prompt, &DelayedEvent[k]) > CoincidenceSpace) continue;

      xd = DelayedEvent[k].X;
      yd = DelayedEvent[k].Y;
      zd = DelayedEvent[k].Z;

      double Rd       = sqrt(xd*xd+yd*yd+zd*zd);

      double dl = sqrt( (xp-xd)*(xp-xd) + (yp-yd)*(yp-yd) + (zp-zd)*(zp-zd) );
      if(dl > CoincidenceSpace) continue;
    
      rp = Rp;
      rd = Rd;


      //
      // muon veto (for delayed)
      //
      if(muon_index>=(int)muon.size())muon_index=(int)muon.size()-1;
      if(muon_index<0) muon_index=0;
      if(muon[muon_index].TimeStamp +TwoSec > DelayedEvent[k].TimeStamp && muon_index>0){
	for(muon_index=muon_index-1; muon_index>=0; muon_index--){
	  if(muon[muon_index].TimeStamp + TwoSec < DelayedEvent[k].TimeStamp){
	    muon_index++;
	    break;
	  }
	}

      } else if(muon[muon_index].TimeStamp + TwoSec < DelayedEvent[k].TimeStamp 
		&& muon_index < (int)muon.size()-1 ){
	for(muon_index=muon_index+1; muon_index<(int)muon.size(); muon_index++){
	  if(muon[muon_index].TimeStamp + TwoSec >= DelayedEvent[k].TimeStamp) break;
	}
      }
      if(muon_index>=(int)muon.size())muon_index=(int)muon.size()-1;
      int muon_veto_delayed1=0; // within 2msec after muon
      int muon_veto_delayed2=0; // within 1sec after muon
      int muon_veto_delayed3=0; // bad muon fit
      int muon_veto_delayed4=0; // muon with shower -> veto 2sec whole detector
      //                // other       muon -> veto 2sec along muon-trak within 3m
      if(muon_index<0) muon_index=0;
      int event_number_of_first_near_muon=0;
      int index_of_nearest_muon=muon_index;
      int number_of_near_muon=0;
      if(muon.size()>0 && muon_index>=0){
	for(int j=muon_index; j<(int)muon.size(); j++){
	  if(muon[j].TimeStamp > DelayedEvent[k].TimeStamp) break;

	  number_of_near_muon++;
	  index_of_nearest_muon=muon_index;
	  if(event_number_of_first_near_muon==0) 
	    event_number_of_first_near_muon=muon[j].EventNumber;

	  if(muon[j].TimeStamp + TwoMSec >= DelayedEvent[k].TimeStamp) muon_veto_delayed1++;
	    
	  if(muon[j].TimeStamp + OneSec > DelayedEvent[k].TimeStamp) muon_veto_delayed2++;
	    
	  if( (int)(muon[j].Badness) >= 100 && muon[j].TotalCharge17>40000) muon_veto_delayed3++;
	    
	  float sciL,cheL;
	  double dQ, dL;
	  
	  muon[j].getTrackLength(&sciL,&cheL);

	  if(sciL<0 || cheL<0){
	    dQ = 0;
	  }
	  else{
	    dQ = KVFResidualCharge::dQ(muon[j].TotalCharge17, cheL, sciL, RunNumber);
	  }

	  dL = muon[j].getDistanceFrom(DelayedEvent[k].X,DelayedEvent[k].Y,DelayedEvent[k].Z);

// 	  float sciL,cheL;
// 	  muon[j].getTrackLength(&sciL,&cheL);

// 	  double dQ = KVFResidualCharge::dQ(muon[j].TotalCharge17, cheL, sciL, RunNumber);
// 	  double dL = muon[j].getDistanceFrom(DelayedEvent[k].X,DelayedEvent[k].Y,DelayedEvent[k].Z);

	  if( dL<=300 && dQ<1000000 
	      && muon[j].TotalCharge17>40000) muon_veto_delayed4++;
	  else if( dQ>=1000000) muon_veto_delayed4++;
	}
      }
      if( muon_veto_delayed1>0 || muon_veto_delayed3>0 || muon_veto_delayed4>0) continue;
	
      // ignore this delayed coincidence event because this is whithin 2msec after muon.
      // because muon produce many neutrons.

      //
      // deadtime veto check
      //
      if(deadtime_manager.changeRun(RunNumber) == false){
	cerr << "ERROR : cannot set deadtime manager" << endl;
	return (-1);
      }
	
      deadtime_veto=0;
      if( deadtime_manager.withinVeto(prompt.TimeStamp)
	  ||
	  deadtime_manager.withinVeto(DelayedEvent[k].TimeStamp)
	  )  deadtime_veto++;	

      //
      // fill into root tree
      //
      run      = prompt.RunNumber;
      unixtime = prompt.UnixTime;
      pevent   = prompt.EventNumber;
      devent   = DelayedEvent[k].EventNumber;
      Ep       = energy;
      //Ed       = DelayedEvent[k].EnergyA2;
      Ed       = DelayedEvent[k].Energy;
//       Xp       = prompt.X;
//       Yp       = prompt.Y;
//       Zp       = prompt.Z;
//       Xd       = DelayedEvent[k].X;
//       Yd       = DelayedEvent[k].Y;
//       Zd       = DelayedEvent[k].Z;
//       Rd       = sqrt(Xd*Xd+Yd*Yd+Zd*Zd);
//       dR       = sqrt( (Xp-Xd)*(Xp-Xd) + (Yp-Yd)*(Yp-Yd) + (Zp-Zd)*(Zp-Zd) );
      dR = dl;
      dT       = (double)( ( (double)DelayedEvent[k].TimeStamp/40.0 - (double)prompt.TimeStamp/40.0 ));
      Rxyd     = sqrt(Xd*Xd+Yd*Yd);

      //same cut with anti-neutrino except time window cut(10msec~20sec)
      //if(dT<10000 || dR>200 || dT>20000000) continue;
      //same cut with anti-neutrino except time window cut(200msec~1.2sec)
      //if(dT<200500 || dR>160 || dT>1200000) continue;
      if(dR>200) continue;
	
      //delayed energy cut 
      //if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.0 && Ed < 5.8))) continue;
      if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.4 && Ed < 5.6))) continue;
      //      if (!(Ed >= 1.8 && Ed < 2.6)) continue;
	
      //prompt energy cut
      //if(energy>20) continue;
      if(energy>30) continue;
      
      //dead time cut
      if(deadtime_veto>0) continue;

      //OD hit cut
      if( prompt.N200OD >= 5) continue;
	
      if(dT>=200500 && dT<1200000){
	h_large_energy->Fill(energy);
      }
		
      //      if(energy>=7.5 && energy<30.0){
      if(energy>=5.0 && energy<30.0){
	h_large_dT->Fill(dT);
      }
		
      N++;
    }
  }
  gvf.close();
    
  cout<<"read: "<<argv[2]<<endl;
  cout<<" --> "<<N<<endl;
  cout<<endl;

  h_large_energy->Write();
  h_large_dT->Write();
  
  rootf.Close();
   
  return 0;
}

float eventRadius(KVFGeneralEvent* event){
  return sqrt((event->X)*(event->X) + (event->Y)*(event->Y) + (event->Z)*(event->Z));
}
float deltaR(KVFGeneralEvent* pro,KVFGeneralEvent* del){
  return sqrt((pro->X-del->X)*(pro->X-del->X)
	      +(pro->Y-del->Y)*(pro->Y-del->Y)
	      +(pro->Z-del->Z)*(pro->Z-del->Z));
}
