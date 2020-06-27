#include "KVF.hh"
#include "KltUI.hh"
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
  //set Energy MAX
  const float ENERGY_MIN = 0.0;
  const float ENERGY_MAX = 30.0;

  if(argc < 3) {
    cerr << "Usage : " << argv[0] << " [root file] [gvf file]" << endl;
    exit(-1);
  }

  TFile rootf(argv[1], "RECREATE");
  
  string RunInfoFile  = "./run-info.table";
  string RunTrigFile  = "./trigger_conditions.dat";
  
  if (KVFRunInfo::ReadTable(RunInfoFile.c_str()) == false) {
    cerr << "Cannot set run info table" << endl;
    abort();
  }
  
  if (KVFRunInfo::ReadRunInfoTable(RunTrigFile.c_str()) == false) {
    cerr << "Cannot set run info table" << endl;
    abort();
  }

  TROOT root("root", "test");
  TTree* nt = new TTree("nt", "");
    
  const float XMIN = 0.0;
  const float XMAX = 30.0;
  const float BINWIDTH = 0.02;
  int bin = (int)((XMAX - XMIN) / BINWIDTH);
  TH1F* h_R550 = new TH1F("h_R550", "", bin, XMIN, XMAX);//20keV bin
  TH1F* h_R600 = new TH1F("h_R600", "", bin, XMIN, XMAX);//20keV bin
  TH1F* h      = new TH1F("h", "", bin, XMIN, XMAX);//20keV bin

  KltUI deadtime_manager;
  
  string dead_time_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/dead-time/dead-time-log");
  string missing_muon_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/dead-time/missing-muon-log");
  
  deadtime_manager.readDeadTimeLog(dead_time_filename.c_str(), ".dt");
  deadtime_manager.readMissingMuonLog(missing_muon_filename.c_str(), ".mm");

  vector<KVFGeneralEvent> muon; 
  muon.reserve(32000);

  KVFGeneralEvent* DelayedEvent = new KVFGeneralEvent[1000000];
  
  for(int i=0;i<1000000;i++){
    DelayedEvent[i].enablePositionDependentEnergyCorrection();
  }

  int deadtime_veto, full_volume_veto, track_veto, badness_veto, run_grade, RunNumber = 0;
  int run, pevent, devent, unixtime, livetime = 0, ispps = 0, NsumMax, N200OD;
  float Ep, Ed, Rp, Rd, Xp,Yp,Zp, Xd,Yd,Zd, dR, Rxyd, energy, chiQ, chiT, VertexBadness;
  double dT, dTmuon;
  
  nt -> Branch("run", &run, "run/I");
  nt -> Branch("run_grade", &run_grade, "run_grade/I");
  nt -> Branch("unixtime", &unixtime, "unixtime/I");
  nt -> Branch("pevent", &pevent, "pevent/I");
  nt -> Branch("devent", &devent, "devent/I");
  nt -> Branch("Ep", &Ep, "Ep/F");
  nt -> Branch("Ed", &Ed, "Ed/F");
  nt -> Branch("Xp", &Xp, "Xp/F");
  nt -> Branch("Yp", &Yp, "Yp/F");
  nt -> Branch("Zp", &Zp, "Zp/F");
  nt -> Branch("Rp", &Rp, "Rp/F");
  nt -> Branch("Xd", &Xd, "Xd/F");
  nt -> Branch("Yd", &Yd, "Yd/F");
  nt -> Branch("Zd", &Zd, "Zd/F");
  nt -> Branch("Rd", &Rd, "Rd/F");
  nt -> Branch("dR", &dR, "dR/F");
  nt -> Branch("dT", &dT, "dT/D");
  nt -> Branch("Rxyd", &Rxyd, "Rxyd/F");
  nt -> Branch("NsumMax", &NsumMax, "NsumMax/I");
  nt -> Branch("N200OD", &N200OD, "N200OD/I");
  nt -> Branch("deadtime_veto", &deadtime_veto, "deadtime_veto/I");
  nt -> Branch("chiQ", &chiQ, "chiQ/F");
  nt -> Branch("chiT", &chiT, "chiT/F");
  nt -> Branch("VertexBadness", &VertexBadness, "VertexBadness/F");
  nt -> Branch("dTmuon", &dTmuon, "dTmuon/D");
    
  for(int i=1;i<(argc-1);i++){
    KVFReader gvf(argv[i+1]);
    
    if(gvf.bad()){
      cerr<<"ERROR: Cannot open file"<<endl;
      return (-1);
    }    

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
      cout<<"read: "<<argv[i+1]<<endl;
      cout<<" --> no reactor info"<<endl;
      cout<<endl;
      
      continue;
    }

    run_grade = KVFRunInfo::Grade(RunNumber);
    if(run_grade<0 || run_grade>=10){
      cout<<"read: "<<argv[i+1]<<endl;
      cout<<" --> badrun"<<endl;
      cout<<endl;
      
      continue;
    }

    int muon_index=0;
    
    int NofDelayedEvent = 0;
    
    for (gvf.begin(&DelayedEvent[NofDelayedEvent]); gvf.couldRead(); gvf.next(&DelayedEvent[NofDelayedEvent])) {
      // select low energy event
      if (DelayedEvent[NofDelayedEvent].isThisMuon() == true) continue;
      if (DelayedEvent[NofDelayedEvent].isThisFlasher() == true) continue;      
      
      if(DelayedEvent[NofDelayedEvent].isNoise()) continue;

      Ed       = DelayedEvent[NofDelayedEvent].Energy;
      Xd       = DelayedEvent[NofDelayedEvent].X;
      Yd       = DelayedEvent[NofDelayedEvent].Y;
      Zd       = DelayedEvent[NofDelayedEvent].Z;
      Rd       = sqrt(Xd*Xd+Yd*Yd+Zd*Zd);
      
      if(!(Ed>=1.8)) continue;
      
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
    unsigned long long int LastMuonTimeStamp;
    //    int ispps = 0;
    for (gvf.begin(&prompt); gvf.couldRead(); gvf.next(&prompt)) {
      
      // pps trigger events can be removed in analysis, count it before muon veto
      if (prompt.TriggerType & 0x08) {
	ispps++;
	continue;
      }

      // select low energy event
      if (prompt.isThisMuon() == true) {
	LastMuonTimeStamp = prompt.TimeStamp;
	continue;
      }
      if (prompt.isThisFlasher() == true) continue;      
      
      if(prompt.isNoise()) continue;
      
      int NsumMaxThreshold;
      
      NsumMaxThreshold = KVFRunInfo::Prompt(RunNumber);
      
      if (prompt.NsumMax < NsumMaxThreshold) continue;  
      
      if(prompt.Energy >20) TimeStampOfLastHighEnergyEvent=prompt.TimeStamp;
      energy   = KVFParticleEnergy::Positron(prompt.Energy);
      Xp       = prompt.X;
      Yp       = prompt.Y;
      Zp       = prompt.Z;
      Rp       = sqrt(Xp*Xp+Yp*Yp+Zp*Zp);

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
	if(DelayedEvent[k].Energy<1) continue;
	if(deltaR(&prompt, &DelayedEvent[k]) > CoincidenceSpace) continue;

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
	    muon[j].getTrackLength(&sciL,&cheL);

	    double dQ = muon[j].TotalCharge17-31.45*cheL-629.4*sciL;//version3
	    double dL = muon[j].getDistanceFrom(DelayedEvent[k].X,DelayedEvent[k].Y,DelayedEvent[k].Z);

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
	deadtime_veto=0;
	if( deadtime_manager.withinVeto((int)prompt.RunNumber, prompt.TimeStamp)
	    ||
	    deadtime_manager.withinVeto((int)DelayedEvent[k].RunNumber, DelayedEvent[k].TimeStamp)
	    )  deadtime_veto++;	

	//
	// fill into root tree
	//
	run      = prompt.RunNumber;
	run_grade= run_grade;
	unixtime = prompt.UnixTime;
	pevent   = prompt.EventNumber;
	devent   = DelayedEvent[k].EventNumber;
	Ep       = energy;
	Ed       = DelayedEvent[k].Energy;
	Xp       = prompt.X;
	Yp       = prompt.Y;
	Zp       = prompt.Z;
	Rp       = sqrt(Xp*Xp+Yp*Yp+Zp*Zp);
	Xd       = DelayedEvent[k].X;
	Yd       = DelayedEvent[k].Y;
	Zd       = DelayedEvent[k].Z;
	Rd       = sqrt(Xd*Xd+Yd*Yd+Zd*Zd);
	dR       = sqrt( (Xp-Xd)*(Xp-Xd) + (Yp-Yd)*(Yp-Yd) + (Zp-Zd)*(Zp-Zd) );
	dT       = (double)( ( (double)DelayedEvent[k].TimeStamp/40.0 - (double)prompt.TimeStamp/40.0 ));
	Rxyd     = sqrt(Xd*Xd+Yd*Yd);
	NsumMax  = prompt.NsumMax;
	N200OD   = prompt.N200OD;
	chiQ     = prompt.VertexChi2T;
	chiT     = prompt.VertexChi2Q;
	VertexBadness = prompt.VertexBadness;
	dTmuon   = (double)((double)(prompt.TimeStamp)/40.0 - (double)(LastMuonTimeStamp)/40.0);
	deadtime_veto = deadtime_veto;

	nt -> Fill();

	//same cut with anti-neutrino except time window cut(0.2sec~1.2sec)
	if(dT<200000 || dR>160 || dT>1200000) continue;
	
	//delayed energy cut 
	//	if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.0 && Ed < 5.8))) continue;
	if (!(Ed >= 1.8 && Ed < 2.6)) continue;
	
	//prompt energy cut
	if(energy>30) continue;
	
	//dead time cut
	if(deadtime_veto>0) continue;

	//NsumMax cut
	if(NsumMax < 200) continue;
	
	//N200OD cut
	if(N200OD > 5) continue;

	if(Rp < 550 && Rd < 550) {
	  h_R550->Fill(energy);
	}
	if(Rp < 600 && Rd < 600) {
	  h_R600->Fill(energy);
	}
	h -> Fill(energy);
	N++;
      }
    }
    gvf.close();
    
    cout<<"read: "<<argv[i+1]<<endl;
    cout<<" --> "<<N<<endl;
    cout<<endl;
  }


  nt -> Write();
  h_R550->Write();
  h_R600->Write();
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
