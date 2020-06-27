#include "KVF.hh"
#include "KltRunbaseUI.hh"
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <string>

using namespace std;

float eventRadius(KVFGeneralEvent* event);
float deltaR(KVFGeneralEvent* pro, KVFGeneralEvent* del);

int main(int argc, char* argv[])
{
  KVFParticleEnergy::PositronExtrapolate(); // fix 10.4.21

    KVFDelayedCoincidenceEvent event;


  KVFGeneralEvent::setECorrMode(false);
  //KVFParticleEnergy::PositronExtrapolate();

//   string RunInfoFile  = "./run-info.table-9626";
//   string RunTrigFile  = "./trigger_conditions.dat";

//   string RunInfoFile  = "./table/run-info.table";
//   string RunTrigFile  = "./table/trigger_conditions.dat";

  
  //KltRunbaseUI deadtime_manager;
  
  vector<KVFGeneralEvent> muon; 
  muon.reserve(32000);

  KVFGeneralEvent* DelayedEvent = new KVFGeneralEvent[200000];

  for(int i=0;i<200000;i++){
    DelayedEvent[i].enablePositionDependentEnergyCorrection();
  }

  int deadtime_veto, full_volume_veto, track_veto, badness_veto;
  int run, pevent, devent, unixtime;
  float Ep, Ed, Rp, Rd, Xp, Yp, Zp, Xd, Yd, Zd, dR;
  double dT;

  KVFReader gvf_muon(argv[2]);
  KVFReader gvf_delayed(argv[2]);
  KVFReader gvf_prompt(argv[2]);
  
  if (gvf_muon.bad()) {
    cerr << "ERROR: Cannot open file" << endl;
    return (-1);
  }

  if (gvf_delayed.bad()) {
    cerr << "ERROR: Cannot open file" << endl;
    return (-1);
  }


  if (gvf_prompt.bad()) {
    cerr << "ERROR: Cannot open file" << endl;
    return (-1);
  }

  
  int RunNumber = 0;

  KVFGeneralEvent tmp_muon;
  muon.clear();
  for (gvf_muon.begin(&tmp_muon); gvf_muon.couldRead(); gvf_muon.next(&tmp_muon)) {

    // select muon event
    if (tmp_muon.isThisMuon() == false) continue;

    muon.push_back(tmp_muon);
    muon[muon.size() - 1].TimeStamp = tmp_muon.TimeStamp;
    RunNumber = tmp_muon.RunNumber;
  }

    
  KVFDelayedCoincidenceEvent dc_event;
  KVFWriter dcf(argv[1]);
  if (dcf.bad()) {
    cerr << "ERROR: Cannot open file" << endl;
    return (-1);
  }
  
  int muon_index = 0;

  int NofDelayedEvent = 0;
    
  for (gvf_delayed.begin(&DelayedEvent[NofDelayedEvent]); gvf_delayed.couldRead(); gvf_delayed.next(&DelayedEvent[NofDelayedEvent])) {
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
    if(!(Rd<600)) continue; // 6m fiducial
      
    NofDelayedEvent++;
  }

  int last_event = 0;

  KVFGeneralEvent prompt;

  prompt.enablePositionDependentEnergyCorrection();

  int N = 0;
  unsigned long long int TimeStampOfLastHighEnergyEvent = 0;
  unsigned long long int TimeStampOfLastMissingATWDEvent1 = 0;
  unsigned long long int TimeStampOfLastMissingATWDEvent2 = 0;
  int last_muon=0;
    
  for (gvf_prompt.begin(&prompt); gvf_prompt.couldRead(); gvf_prompt.next(&prompt)) {
      
    if (prompt.isThisFlasher() == true) continue;      
      
    Xp       = prompt.X;
    Yp       = prompt.Y;
    Zp       = prompt.Z;
    Rp       = sqrt(Xp * Xp + Yp * Yp + Zp * Zp);

    if(!(Rp<600)) continue; // 6m fiducial

    static const unsigned long long int TwoSec = 80000000; // 2sec
    static const unsigned long long int OneSec = 40000000; // 1sec
    static const unsigned long long int TwoMSec = 80000; // 2msec
      
    // find last muon since 2msec ago 
    while (true) {
      if (muon[last_muon].TimeStamp + TwoMSec > prompt.TimeStamp) break;
      if (last_muon + 1 >= (int)muon.size()) break;
      last_muon++;
    }

    // no muon within 2msec
    if (muon[last_muon].TimeStamp > prompt.TimeStamp) {
      if (prompt.Nhit17 < (unsigned int)prompt.NsumMax && prompt.NsumMax >= 200) TimeStampOfLastMissingATWDEvent1 = prompt.TimeStamp; 
      if (prompt.Nhit17 < (unsigned int)prompt.NsumMax && prompt.NsumMax >= 1200) TimeStampOfLastMissingATWDEvent2 = prompt.TimeStamp; 
    }
      
    if (prompt.isNoise()) continue;
      
    //if (prompt.EnergyA2 > 20) TimeStampOfLastHighEnergyEvent = prompt.TimeStamp;
    if (prompt.Energy > 20) TimeStampOfLastHighEnergyEvent = prompt.TimeStamp;

    //
    // delayed coincidence selection
    //
    static const unsigned long long int CoincidenceGateWidth = 100000; //2.5msec
    static const double CoincidenceSpace = 300.0; //2m

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
	
      if (DelayedEvent[k].TimeStamp > (prompt.TimeStamp + CoincidenceGateWidth))break;
      if (DelayedEvent[k].isNoise()) continue;
      //if (DelayedEvent[k].EnergyA2 < 1) continue;
      if (DelayedEvent[k].Energy < 1) continue;
      if (deltaR(&prompt, &DelayedEvent[k]) > CoincidenceSpace) continue;
		
      int muon_veto5=0;
      if (prompt.TimeStamp < TimeStampOfLastHighEnergyEvent + TwoMSec
	  ||
	  DelayedEvent[k].TimeStamp < TimeStampOfLastHighEnergyEvent + TwoMSec) muon_veto5++;


      //
      // muon veto
      //
      if (muon_index >= (int)muon.size())muon_index=(int)muon.size() - 1;
      if (muon_index < 0) muon_index = 0;
      if (muon[muon_index].TimeStamp + TwoSec > prompt.TimeStamp && muon_index > 0) {
	for (muon_index=muon_index - 1; muon_index >= 0; muon_index--) {
	  if (muon[muon_index].TimeStamp + TwoSec < prompt.TimeStamp) {
	    muon_index++;
	    break;
	  }
	}
      }
      else if (muon[muon_index].TimeStamp + TwoSec < prompt.TimeStamp 
	       && muon_index < (int)muon.size() - 1 ) {
	for (muon_index=muon_index + 1; muon_index<(int)muon.size(); muon_index++) {
	  if (muon[muon_index].TimeStamp + TwoSec >= prompt.TimeStamp) break;
	}
      }

      if (muon_index >= (int)muon.size()) muon_index = (int)muon.size() - 1;

      int muon_veto1 = 0; // within 2msec after muon
      int muon_veto2 = 0; // within 1sec after muon
      int muon_veto3 = 0; // bad muon fit
      int muon_veto4 = 0; // muon with shower -> veto 2sec whole detector
      //                // other       muon -> veto 2sec along muon-trak within 3m

      if (muon_index < 0) muon_index = 0;

      int event_number_of_first_near_muon = 0;
      int index_of_nearest_muon = muon_index;
      int number_of_near_muon = 0;

      if (muon.size() > 0 && muon_index >= 0) {
	for (int j = muon_index; j < (int)muon.size(); j++) {
	  if (muon[j].TimeStamp > DelayedEvent[k].TimeStamp) break;
	    
	  // itself cut
	  if (muon[j].TimeStamp == prompt.TimeStamp) continue;
	    
	  // fake muon cut
	  if (j > 0) {
	    if (muon[j].TotalCharge17 < 40000 && muon[j - 1].TimeStamp + 400 > muon[j].TimeStamp) continue;
	  }

	  number_of_near_muon++;
	  index_of_nearest_muon = muon_index;
	  if (event_number_of_first_near_muon == 0) 
	    event_number_of_first_near_muon=muon[j].EventNumber;

	  if (muon[j].TimeStamp + TwoMSec >= prompt.TimeStamp
	      ||
	      muon[j].TimeStamp + TwoMSec >= DelayedEvent[k].TimeStamp
	      ||
	      (muon[j].TimeStamp > prompt.TimeStamp
	       && muon[j].TimeStamp < DelayedEvent[k].TimeStamp)
	      ) muon_veto1++;
	    
	  if (muon[j].TimeStamp + OneSec > prompt.TimeStamp) muon_veto2++;
	    
	  if ((int)(muon[j].Badness) >= 100 && muon[j].TotalCharge17 >= 40000) muon_veto3++;
	    
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
	    
	  if (dL <= 300 && dQ < 1000000 
	      && muon[j].TotalCharge17 >= 40000) muon_veto4++;
	  else if (dQ >= 1000000) muon_veto4++;
	}
      }
      if (muon_veto1 > 0) continue;

      // ignore this delayed coincidence event because this is whithin 2msec after muon.
      // because muon produce many neutrons.

      //
      // deadtime veto check
      //

      deadtime_veto=0;

      //
      // fill into root tree
      //
      run      = prompt.RunNumber;
      unixtime = prompt.UnixTime;
      pevent   = prompt.EventNumber;
      devent   = DelayedEvent[k].EventNumber;
      //Ep       = prompt.EnergyA2;
      Ep       = prompt.Energy;
      //Ed       = DelayedEvent[k].EnergyA2;
      Ed       = DelayedEvent[k].Energy;
      Xp       = prompt.X;
      Yp       = prompt.Y;
      Zp       = prompt.Z;
      Xd       = DelayedEvent[k].X;
      Yd       = DelayedEvent[k].Y;
      Zd       = DelayedEvent[k].Z;
      Rp       = sqrt(Xp * Xp + Yp * Yp + Zp * Zp);
      Rd       = sqrt(Xd * Xd + Yd * Yd + Zd * Zd);
      dR       = sqrt((Xp - Xd) * (Xp - Xd) + (Yp - Yd) * (Yp - Yd) + (Zp - Zd) * (Zp - Zd));
      dT       = (double)(((double)DelayedEvent[k].TimeStamp / 40.0 - (double)prompt.TimeStamp / 40.0));
      full_volume_veto = muon_veto2;
      badness_veto = muon_veto3;
      track_veto = muon_veto4; 

      dc_event.Prompt = prompt;
      dc_event.Delayed = DelayedEvent[k];
      dc_event.Veto[0] = (unsigned int)deadtime_veto;
      dc_event.Veto[1] = (unsigned int)muon_veto1;
      dc_event.Veto[2] = (unsigned int)muon_veto2;
      dc_event.Veto[3] = (unsigned int)muon_veto3;
      dc_event.Veto[4] = (unsigned int)muon_veto4;
      dc_event.Veto[5] = (unsigned int)muon_veto5;
      int miss_atwd_veto=0;
      if (TimeStampOfLastMissingATWDEvent1 + TwoMSec > prompt.TimeStamp) miss_atwd_veto = 1;
      if (TimeStampOfLastMissingATWDEvent2 + TwoSec > prompt.TimeStamp) miss_atwd_veto = 1;
      dc_event.Veto[5] = (unsigned int)miss_atwd_veto;
      dc_event.FirstEventNumberOfNearMuon = event_number_of_first_near_muon;
      dc_event.NofNearMuon = (unsigned int)number_of_near_muon;
      dcf.write(&dc_event);

      double EpA2       = prompt.EnergyA2;
      double energy = KVFParticleEnergy::Positron(Ep, run);
      double energyA2 = KVFParticleEnergy::Positron(EpA2, run);

      if (run == 7567 && pevent == 18418630)
	{
	  cerr << run << " " << pevent << " " << Ep << " " << EpA2 << " " << energy << " " << energyA2 << endl; 
	}


      N++;
    }
  }
    
  gvf_muon.close();
  gvf_delayed.close();
  gvf_prompt.close();

    
  cerr << "read: " << argv[2] << endl;
  cerr << " --> " << N << endl;
  cerr << endl;
  
  dcf.close();
  
  return 0;
}

float eventRadius(KVFGeneralEvent* event)
{
  return sqrt((event->X) * (event->X) + (event->Y) * (event->Y) + (event->Z) * (event->Z));
}

float deltaR(KVFGeneralEvent* pro, KVFGeneralEvent* del)
{
  return sqrt((pro->X-del->X) * (pro->X-del->X)
	      + (pro->Y-del->Y) * (pro->Y-del->Y)
	      + (pro->Z-del->Z) * (pro->Z-del->Z));
}
