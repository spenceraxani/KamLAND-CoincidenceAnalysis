/*
  ./a.out output.dcvf `find /bb2/kamland/vf/ -name "run0*vf" -print | sort`
  or
  ./a.out output.dcvf runXXX.lvf runXXX.mvf

  or

  ./a.out output.root `find /bb2/kamland/vf/ -name "run0*vf" -print | sort`
  or
  ./a.out output.root runXXX.lvf runXXX.mvf
*/

#include "KVF.hh"
#include "KVFDelayedCoincidenceEventMuon.hh"
#include "KltUI.hh"
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
  
  //   KVFConstHandler::setVersion(6);
  //   KVFEnergyCorrection::setVersion(2);
  KVFGeneralEvent::setECorrMode(false);
  KVFParticleEnergy::PositronExtrapolate();

  KltUI deadtime_manager;

  // --- read constant table
  string dead_time_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/dead-time/dead-time-log");
  string missing_muon_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/dead-time/missing-muon-log");

  deadtime_manager.readDeadTimeLog(dead_time_filename.c_str(), ".dt");
  deadtime_manager.readMissingMuonLog(missing_muon_filename.c_str(), ".mm");

  if(!KVFRunInfo::ReadTable("./table/run-info-11947.table")){
    cerr<<"Error: wrong run info"<<endl;
    abort();
  }

  cerr << "ok1" << endl;

  // --- read vector file
  vector<KVFGeneralEvent> muon; 
  muon.reserve(32000);

  int deadtime_veto, full_volume_veto, track_veto, badness_veto;
  int run, pevent, devent, unixtime;
  float Ep, Ed, Rp, Rd, Xp, Yp, Zp, Xd, Yd, Zd, dR, Rxyd, energy;
  double dT;

  cerr << argc << endl;

  for (int i = 1; i < (argc - 1); i++) {
    KVFReader gvf_muon(argv[i + 1]);
    KVFReader gvf_prompt(argv[i + 1]);
    KVFReader gvf_delayed(argv[i + 1]);
    
    if (gvf_muon.bad()) {
      cerr<<"ERROR: Cannot open file"<<endl;
      return (-1);
    }

    if (gvf_prompt.bad()) {
      cerr<<"ERROR: Cannot open file"<<endl;
      return (-1);
    }

    if (gvf_delayed.bad()) {
      cerr<<"ERROR: Cannot open file"<<endl;
      return (-1);
    }

    cerr << "ok2" << endl;

    int RunNumber;
    
    KVFGeneralEvent tmp_muon;
    muon.clear();
    for(gvf_muon.begin(&tmp_muon); gvf_muon.couldRead(); gvf_muon.next(&tmp_muon)){
      RunNumber = tmp_muon.RunNumber;

      int grade=KVFRunInfo::Grade(RunNumber);

      if(grade==10) {
	cout << "read: " << argv[i + 1] << endl;
	cout << " --> badrun" << endl;
	cout << endl;

	cerr << "read: " << argv[i + 1] << endl;
	cerr << " --> badrun" << endl;
	cerr << endl;

	return 0;
      }

      //      cerr << "ok6" << endl;

      // select muon event
      if (tmp_muon.isThisMuon() == false) continue;

      muon.push_back(tmp_muon);
      muon[muon.size()-1].TimeStamp = tmp_muon.TimeStamp;
    }
    //gvf.close();

    cerr << "ok3" << endl;   

    KVFDelayedCoincidenceEventMuon dc_event;
    KVFWriter dcf(argv[1]);
    if(dcf.bad()){
      cerr<<"ERROR: Cannot open file"<<endl;
      return (-1);
    }
    
    int muon_index=0;

    const int n=200000;
    KVFGeneralEvent* DelayedEvent = new KVFGeneralEvent[n];
    
    for(int nm=0; nm<n; nm++){
      DelayedEvent[nm].enablePositionDependentEnergyCorrection();
    }

    int N=0;
    unsigned long long int TimeStampOfLastHighEnergyEvent=0;

    int NofDelayedEvent = 0;

    for (gvf_delayed.begin(&DelayedEvent[NofDelayedEvent]); gvf_delayed.couldRead(); gvf_delayed.next(&DelayedEvent[NofDelayedEvent])) {

      if (DelayedEvent[NofDelayedEvent].isThisMuon() == true) continue;
      if (DelayedEvent[NofDelayedEvent].isThisFlasher() == true) continue;      
      if (DelayedEvent[NofDelayedEvent].isNoise()) continue;
      
      Ed       = DelayedEvent[NofDelayedEvent].EnergyA2;
      Xd       = DelayedEvent[NofDelayedEvent].X;
      Yd       = DelayedEvent[NofDelayedEvent].Y;
      Zd       = DelayedEvent[NofDelayedEvent].Z;
      Rd       = sqrt(Xd*Xd+Yd*Yd+Zd*Zd);
      
      if(!(Ed>=1.7)) continue;
      
      // ######### fiducial cut
      if(!(Rd<600)) continue;

      NofDelayedEvent++;
    }

    cerr << "ok4" << endl;

    int last_event = 0;
    KVFGeneralEvent prompt;
    prompt.enablePositionDependentEnergyCorrection();

    for (gvf_prompt.begin(&prompt); gvf_prompt.couldRead(); gvf_prompt.next(&prompt)) {
      if (prompt.isThisMuon() == true) continue;
      if (prompt.isThisFlasher() == true) continue;      
      if (prompt.isNoise()) continue;
      
      energy   = KVFParticleEnergy::Positron(prompt.EnergyA2, RunNumber);
      //energy   = KVFParticleEnergy::Positron(prompt.EnergyA2);
      Xp       = prompt.X;
      Yp       = prompt.Y;
      Zp       = prompt.Z;
      Rp       = sqrt(Xp*Xp+Yp*Yp+Zp*Zp);

      // ######### fiducial cut
      if(Rp>600) continue;//6m fiducial
      
      static const unsigned long long int TenSec = 400000000; //10sec
      static const unsigned long long int TwoSec = 80000000; //2sec
      static const unsigned long long int OneSec = 40000000; //1sec
      static const unsigned long long int TwoMSec = 80000; //2msec
      //static const unsigned long long int TwoMSec = 160000; //4msec
      
      //
      // delayed coincidence selection
      //
      static const unsigned long long int CoincidenceGateWidth= 100000; //2.5msec
      static const double CoincidenceSpace = 300.0; //2m

      // select low energy event
      //find first event after prompt event
      while(true){
        if(DelayedEvent[last_event].TimeStamp > prompt.TimeStamp) break;
        if(last_event+1>=NofDelayedEvent) break;
        last_event++;
      }      


      cerr << "ok5" << endl;  
      
      for(int k=last_event; k<NofDelayedEvent; k++){
	if (DelayedEvent[k].isThisMuon() == true) continue;
	if (DelayedEvent[k].isThisFlasher() == true) continue;      
	
	if(DelayedEvent[k].TimeStamp > (prompt.TimeStamp+CoincidenceGateWidth))break;
	if(DelayedEvent[k].isNoise()) continue;
	//if(delayed.Energy<1 || delayed.Energy>3.5) continue;
	//if(DelayedEvent[k].Energy<1) continue;
	if(deltaR(&prompt, &DelayedEvent[k]) > CoincidenceSpace) continue;
	

	//
	// muon veto
	//
	if(muon_index>=(int)muon.size())muon_index=(int)muon.size()-1;
	if(muon_index<0) muon_index=0;
	if(muon[muon_index].TimeStamp +TenSec > prompt.TimeStamp && muon_index>0){
	  for(muon_index=muon_index-1; muon_index>=0; muon_index--){
	    if(muon[muon_index].TimeStamp + TenSec < prompt.TimeStamp){
	      muon_index++;
	      break;
	    }
	  }

	} else if(muon[muon_index].TimeStamp + TenSec < prompt.TimeStamp 
		  && muon_index < (int)muon.size()-1 ){
	  for(muon_index=muon_index+1; muon_index<(int)muon.size(); muon_index++){
	    if(muon[muon_index].TimeStamp + TenSec >= prompt.TimeStamp) break;
	  }
	}


    cerr << "ok6" << endl;

	if(muon_index>=(int)muon.size())muon_index=(int)muon.size()-1;
	int muon_veto1=0; // within 2msec after muon
//  	int muon_veto2=0; // within 1sec after muon
//  	int muon_veto3=0; // bad muon fit
//  	int muon_veto4=0; // muon with shower -> veto 2sec whole detector
//  	//                // other       muon -> veto 2sec along muon-trak within 3m
	if(muon_index<0) muon_index=0;
//  	int event_number_of_first_near_muon=0;
//  	int index_of_nearest_muon=muon_index;
//  	int number_of_near_muon=0;
	if(muon.size()>0 && muon_index>=0){
	  for(int j=muon_index; j<(int)muon.size(); j++){
	    //	    if(muon[j].TimeStamp > delayed.TimeStamp) break;
	    if(muon[j].TimeStamp > DelayedEvent[k].TimeStamp) break;

//  	    //fake muon cut
//  	    if(j>0){
//  	      if(muon[j-1].TimeStamp + 40 > muon[j].TimeStamp) continue;
//  	    }

//  	    number_of_near_muon++;
//  	    index_of_nearest_muon=muon_index;
//  	    if(event_number_of_first_near_muon==0) 
//  	      event_number_of_first_near_muon=muon[j].EventNumber;

	    if(muon[j].TimeStamp + TwoMSec >= prompt.TimeStamp//here
	       ||
	       muon[j].TimeStamp + TwoMSec >= DelayedEvent[k].TimeStamp//here
	       ||
	       (muon[j].TimeStamp > prompt.TimeStamp
		&& muon[j].TimeStamp < DelayedEvent[k].TimeStamp)
	       ) muon_veto1++;
	    
//  	    if(muon[j].TimeStamp + OneSec > prompt.TimeStamp) muon_veto2++;
	    
//  	    if( (int)(muon[j].Badness) >= 100 && muon[j].TotalChargeID>40000) muon_veto3++;
	    
//  	    float sciL,cheL;
//  	    muon[j].getTrackLength(&sciL,&cheL);

//  	    ///(UpDate)
//  	    double dQ = muon[j].TotalChargeID-26.47*cheL-604.8*sciL;
//  	    //double dQ = muon[j].TotalChargeID-32.45*cheL-738.4*sciL;
//  	    //double dQ = muon[j].TotalChargeID-32.1*cheL-638.3*sciL;
//  	    //double dL = muon[j].getDistanceFrom(prompt.X,prompt.Y,prompt.Z);
//  	    double dL2 = muon[j].getDistanceFrom(delayed.X,delayed.Y,delayed.Z);
//  	    //if(dL2<dL) dL=dL2;
//  	    double dL=dL2;

//  	    if( dL<=300 && dQ<1000000 
//  		&& muon[j].TotalChargeID>40000) muon_veto4++;
//  	    else if( dQ>=1000000) muon_veto4++;
	  }
	}
	if( muon_veto1>0 ) continue;

	cerr << "ok7" << endl;

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

	    cerr << "ok8" << endl;

	//
	// fill into root tree
	//
	run      = prompt.RunNumber;
	unixtime = prompt.UnixTime;
	pevent   = prompt.EventNumber;
	devent   = DelayedEvent[k].EventNumber;
	Ep       = energy;
	Ed       = DelayedEvent[k].EnergyA2;
	Xp       = prompt.X;
	Yp       = prompt.Y;
	Zp       = prompt.Z;
	Xd       = DelayedEvent[k].X;
	Yd       = DelayedEvent[k].Y;
	Zd       = DelayedEvent[k].Z;
	Rp       = sqrt(Xp*Xp+Yp*Yp+Zp*Zp);
	Rd       = sqrt(Xd*Xd+Yd*Yd+Zd*Zd);
	dR       = sqrt( (Xp-Xd)*(Xp-Xd) + (Yp-Yd)*(Yp-Yd) + (Zp-Zd)*(Zp-Zd) );
	dT       = (double)( ( (double)DelayedEvent[k].TimeStamp/40.0 - (double)prompt.TimeStamp/40.0 ));
	Rxyd     = sqrt(Xd*Xd+Yd*Yd);

//  	full_volume_veto = muon_veto2;
//  	badness_veto = muon_veto3;
//  	track_veto = muon_veto4; 
	
	//same cut with anti-neutrino except energy and spallation cut
	//5m fiducial
	//if(Rp>500 || Rd>500 || dT>660 || dR>160 || dT<0.5 || Rxyd<120) continue;

	//6.0m fiducial ( no cylinder cut)
	//	if(Rp>600 || Rd>600 || dT>1000 || dR>200 || dT<0.5) continue;

	//5.5m fiducial ( no cylinder cut)
	//if(Rp>550 || Rd>550 || dT>1000 || dR>200 || dT<0.5) continue;

	for(int j=muon_index; j<(int)muon.size(); j++){
	  if(muon[j].TimeStamp > DelayedEvent[k].TimeStamp) break;
	  //fake muon cut
	  if(j>0){
	    if(muon[j-1].TimeStamp + 40 > muon[j].TimeStamp) continue;
	  }
	  dc_event.Prompt = prompt;
	  dc_event.Delayed = DelayedEvent[k];
	  dc_event.Muon = muon[j];
	  
	  dcf.write(&dc_event);
	  N++;
	}

    cerr << "ok9" << endl;

//  	dc_event.Veto[0] = (unsigned int)deadtime_veto;
//  	dc_event.Veto[1] = (unsigned int)muon_veto1;
//  	dc_event.Veto[2] = (unsigned int)muon_veto2;
//  	dc_event.Veto[3] = (unsigned int)muon_veto3;
//  	dc_event.Veto[4] = (unsigned int)muon_veto4;
//  	dc_event.Veto[5] = (unsigned int)muon_veto5;
//  	int miss_tag_veto=0;
//  	if(TimeStampOfLastHighEnergyEvent+TwoSec > prompt.TimeStamp) miss_tag_veto=1;
//  	dc_event.Veto[5] = (unsigned int)miss_tag_veto;
//  	dc_event.FirstEventNumberOfNearMuon = event_number_of_first_near_muon;
//  	dc_event.NofNearMuon = (unsigned int)number_of_near_muon;
      }

      //      gvf.seek((-1*NofForward), prompt.getBufSize());
    }

    gvf_muon.close();
    gvf_prompt.close();
    gvf_delayed.close();

    dcf.close();

    cout << "read: " << argv[i + 1] << endl;
    cout << " --> " << N << endl;
    cout << endl;

    cerr << "read: " << argv[i + 1] << endl;
    cerr << " --> " << N << endl;
    cerr << endl;

  }
  
    cerr << "ok10" << endl;
  
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
