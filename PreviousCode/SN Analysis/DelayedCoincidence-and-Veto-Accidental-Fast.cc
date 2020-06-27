#include "KVF.hh"
#include "KltUI.hh"
#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "TROOT.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TGraphErrors.h"


#include "TChain.h"
#include "TStyle.h"
#include "TColor.h"
#include "TSystem.h"
#include "TTree.h"
#include "TF1.h"
#include "TCut.h"
#include "TLine.h"
#include "TLatex.h"
#include "TGaxis.h"
#include "TGraphErrors.h"
#include "TPostScript.h"





#include "KVFLikelihoodRatio.hh"

using namespace std;

float eventRadius(KVFGeneralEvent* event);
float deltaR(KVFGeneralEvent* pro, KVFGeneralEvent* del);

bool Resolution_enable20inch = false;

int main(int argc, char* argv[])
{


  KVFLikelihoodRatio::setVersion(2);

  int runmin = atoi(argv[1]);
  int runmax = atoi(argv[2]);



  char ps_file[256];
  sprintf(ps_file,"./picture/DelayedCoincidence-and-Veto-Accidental-v3-%06d-%06d.ps",runmin,runmax);
  //  sprintf(ps_file,"./picture/DelayedCoincidence-and-Veto-Accidental-Rall2_%06d-%06d.ps",runmin,runmax);
  //  sprintf(ps_file,"./picture/test-%06d-%06d.ps",runmin,runmax);


  //    char *ps_file = "./picture/test5030-5035.ps";



  gROOT  -> SetStyle("Plain");
  gStyle -> SetStatFont(42);
  gStyle -> SetOptStat(0);
  gStyle -> SetLabelFont(42,"XY");
  gStyle -> SetLabelSize(0.055, "XY");
  gStyle -> SetLabelOffset(0.008, "X");
  gStyle -> SetLabelOffset(0.002, "Y");
  gStyle -> SetTitleFont(42, "XY");
  gStyle -> SetTitleSize(0.055, "XY");
  gStyle -> SetTitleOffset(1.0, "X");
  gStyle -> SetTitleOffset(0.9, "Y");

  TCanvas *c1 = new TCanvas("c1", "Title", 0, 0, 1000, 700);
  c1 -> SetFillStyle(4001);
  c1 -> SetBottomMargin(0.12);
  c1 -> SetLeftMargin(0.12);
  //  c1 -> Divide(2,2);


  // make picture
  int type = 112;
  TPostScript *ps = new TPostScript(ps_file, type);
  ps -> Range(25, 12);



  /*
  //set Energy MAX
  const float ENERGY_MIN = 4.0;
  const float ENERGY_MAX = 30.0;

  */

  /*
  if(argc < 3) {
    cerr << "Usage : " << argv[0] << " [root file] [gvf file]" << endl;
    exit(-1);
  }
  */

  char root_file[256];
  //  sprintf(root_file,"./result/DelayedCoincidence-and-Veto-Accidental-Rall2_%06d-%06d.root",runmin,runmax);
  sprintf(root_file,"./result/DelayedCoincidence-and-Veto-Accidental-v3-%06d-%06d.root",runmin,runmax);
  //  sprintf(root_file,"./result/test-%06d-%06d.root",runmin,runmax);
  cerr << root_file << endl;

  TFile rootf(root_file, "RECREATE");
  
  string RunInfoFile  = "./table/run-info-11947.table";
  string RunTrigFile  = "./table/trigger_conditions_11947.dat";

  char RunInfoFile_run[120000]  = "./table/run-info-11947.table";




  int RunInfoRun[120000];
  int RunGrade[120000];
  int RunInfoFlag[120000];
  for (int i = 0 ; i < 120000 ; i++) {
    RunInfoRun[i] = -1;
    RunGrade[i] = -1;
    RunInfoFlag[i] = -1;
    
  }



  string line;
  int info_run_number;
  int dummy;
  string str_dummy;
  float info_run_time;
  int info_unixtime;
  int info_flag;
  int info_run_grade;

  ifstream info(RunInfoFile_run);
  while (getline(info, line)) {

    istringstream iss(line);
    iss >> info_run_number >> str_dummy >> info_run_time >> info_unixtime >> info_flag >> info_run_grade  >> dummy >> dummy >> dummy;

    RunInfoRun[info_run_number] = 1;
    RunGrade[info_run_number] = info_run_grade;
    RunInfoFlag[info_run_number] = info_flag; 
  }
  





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
  TH1F* h_ln = new TH1F("h_ln", "", bin, XMIN, XMAX);//20keV bin
  TH1F* h_all = new TH1F("h_all", "", bin, XMIN, XMAX);//20keV bin
  TH1F* h_ln_root = new TH1F("h_ln_root", "", bin, XMIN, XMAX);//20keV bin
  TH1F* h_all_root = new TH1F("h_all_root", "", bin, XMIN, XMAX);//20keV bin

  //  TH1F* h      = new TH1F("h", "", bin, XMIN, XMAX);//20keV bin



  KltUI deadtime_manager;
  
  string dead_time_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/data/kamland/const/dead-time/dead-time-log");
  string missing_muon_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/data/kamland/const/dead-time/missing-muon-log");
  
  deadtime_manager.readDeadTimeLog(dead_time_filename.c_str(), ".dt");
  deadtime_manager.readMissingMuonLog(missing_muon_filename.c_str(), ".mm");

  vector<KVFGeneralEvent> muon; 
  muon.reserve(32000);

  KVFGeneralEvent* DelayedEvent = new KVFGeneralEvent[1000000];
  
  for(int i=0;i<1000000;i++){
    DelayedEvent[i].enablePositionDependentEnergyCorrection();
  }

  int deadtime_veto, full_volume_veto, track_veto, badness_veto, run_grade, RunNumber = 0, ln=0;
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
  nt -> Branch("ln", &ln, "ln/I");
    

  char vf_filename[256];



  //run selection





  for(int i=runmin; i<=runmax; i++){

       //   for(int i=5030; i<=5035; i++){





 
    if (6820 < i && i < 6890 ) continue;
    if (7930 < i && i < 8070)continue;
    if (10675 < i && i < 11000)continue;

    if (i == 11348 ) continue;

  

    cerr << "run_grade =  " << RunGrade[i] << " : " << i << endl; 

    if (RunGrade[i] >= 10)continue;


    //    cerr << "run_info_flag = " << RunInfoFlag[i] << endl;

    if (!(RunInfoFlag[i] == 1))
      {cerr << "no reactor file : " << i << endl;
	continue;
      }


    if (RunInfoRun[i] < 0) {
      //      cerr << "RunInfoRun = "<< RunInfoRun[i] << endl;
      cerr << "no vector file : " << i << endl;
      continue;
    }
  



   sprintf(vf_filename,"/data/datavf/v2-v1.04-solar-phase/run%06d.gvf",i);
   cerr << vf_filename << endl;
 


 
  KVFReader gvf(vf_filename);
  KVFReader gvf_prompt(vf_filename);


  //  for(int i=1;i<(argc-1);i++){
  //    KVFReader gvf(argv[i+1]);
    
    if(gvf.bad()){
     
      cerr<<"ERROR: Cannot open file!"<<endl;
      //           return (-1);
      continue;
    }    

    KVFGeneralEvent tmp_muon;
    KVFDelayedCoincidenceEvent event;
    muon.clear();
    for(gvf.begin(&tmp_muon); gvf.couldRead(); gvf.next(&tmp_muon)){
    //      for(gvf.begin(&tmp_muon); gvf.couldRead(); gvf.next(&tmp_muon); gvf.next(&event)){



      // select muon event
      if (tmp_muon.isThisMuon() == false) continue;
      
      muon.push_back(tmp_muon);
      muon[muon.size()-1].TimeStamp = tmp_muon.TimeStamp;
      RunNumber = tmp_muon.RunNumber;
    }



    if( KVFRunInfo::HasReactorInfo(RunNumber) == false ){
      //      cout<<"read: "<<argv[i+1]<<endl;
      //      cout<<"read: "<< vf_filename <<endl;
      cout<<" --> no reactor info"<<endl;
      cout<<endl;
      
      continue;
    }




   //insert
      int VersionOfKamLAND = KVFRunInfo::VersionOfKamLAND(RunNumber);
      
      if( VersionOfKamLAND == 1 || VersionOfKamLAND == 2 || VersionOfKamLAND == 4 ){
	Resolution_enable20inch = false;
	KVFLikelihoodRatio::disable20inch();
      }
      else if( VersionOfKamLAND == 3 || VersionOfKamLAND == 5 ){
	Resolution_enable20inch = true;
	KVFLikelihoodRatio::enable20inch();
      }
      else{
	cerr << "ERROR : set VersionOfKamLAND" << endl;
	abort();
      }
      
      KVFLikelihoodRatio::changeRun(RunNumber);
      






    run_grade = KVFRunInfo::Grade(RunNumber);

    //    cout << "run grade = " << run_grade << endl;
    if(run_grade<0 || run_grade>=10){
      //     cout<<"read: "<<argv[i+1]<<endl;
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
      


      //Xd = event.Delayed.X;
      //Yd = event.Delayed.Y;
      //Zd = event.Delayed.Z;


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



      //      for (gvf.begin(&prompt); gvf.couldRead(); gvf.next(&prompt)) 
      for (gvf_prompt.begin(&prompt); gvf_prompt.couldRead(); gvf_prompt.next(&prompt)) 
	{


	  
	  ln=0;
	  
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
      double      energyA2   = KVFParticleEnergy::Positron(prompt.EnergyA2, i);

      //      cerr << "Kat : " << energy << " A2 : " << energyA2 << endl;



      static const double R_balloon = 650.0;
      static const double p0 = 55.18;
      static const double p1 = -0.0509;
      static const double p2 = 613.3;


      double CorrectionFactor = 1.0;
    
      if(energy>7.5){
	CorrectionFactor = R_balloon / (p0 * exp(p1 * energy) + p2);
      }

	Xp = prompt.X * CorrectionFactor;
	Yp = prompt.Y * CorrectionFactor;
	Zp = prompt.Z * CorrectionFactor;

      double Rp       = sqrt(Xp*Xp+Yp*Yp+Zp*Zp);


      //     double Rd       = sqrt(Xd*Xd+Yd*Yd+Zd*Zd);

      double dl = sqrt( (Xp-Xd)*(Xp-Xd) + (Yp-Yd)*(Yp-Yd) + (Zp-Zd)*(Zp-Zd) );

      //      rp = Rp;
      //      rd = Rd;





      //     Xp       = prompt.X;
      //      Yp       = prompt.Y;
      //      Zp       = prompt.Z;
      //      Rp       = sqrt(Xp*Xp+Yp*Yp+Zp*Zp);

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

	//insert
	int isVeto = 0;	
	if ( event.Veto[0] > 0 || event.Veto[1] > 0 || event.Veto[3] > 0 || event.Veto[4] > 0) isVeto=1;


	//prompt energy cut
	if(energy > 30) continue;






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

      double CorrectionFactor2 = 1.0;
    
      if(energy>7.5){
	CorrectionFactor2 = R_balloon / (p0 * exp(p1 * energy) + p2);
      }

	Xp = prompt.X * CorrectionFactor2;
	Yp = prompt.Y * CorrectionFactor2;
	Zp = prompt.Z * CorrectionFactor2;

	Rp       = sqrt((Xp*Xp)+(Yp*Yp)+(Zp*Zp));


	//	Xp       = prompt.X;
	//	Yp       = prompt.Y;
	//	Zp       = prompt.Z;
	//	Rp       = sqrt(Xp*Xp+Yp*Yp+Zp*Zp);
	Xd       = DelayedEvent[k].X;
	Yd       = DelayedEvent[k].Y;
	Zd       = DelayedEvent[k].Z;
	Rd       = sqrt((Xd*Xd)+(Yd*Yd)+(Zd*Zd));
	dl = sqrt( (Xp-Xd)*(Xp-Xd) + (Yp-Yd)*(Yp-Yd) + (Zp-Zd)*(Zp-Zd) );
	dR       = dl;
	dT       = (double)( ( (double)DelayedEvent[k].TimeStamp/40.0 - (double)prompt.TimeStamp/40.0 ));
	Rxyd     = sqrt(Xd*Xd+Yd*Yd);
	NsumMax  = prompt.NsumMax;
	N200OD   = prompt.N200OD;
	chiQ     = prompt.VertexChi2T;
	chiT     = prompt.VertexChi2Q;
	VertexBadness = prompt.VertexBadness;
	dTmuon   = (double)((double)(prompt.TimeStamp)/40.0 - (double)(LastMuonTimeStamp)/40.0);
	deadtime_veto = deadtime_veto;

	if (Rp >= 600.0 || Rd >= 600.0) continue;



	c1 -> cd(1);


	//same cut with anti-neutrino except time window cut(0.2sec~1.2sec)

	//	if(dT<200000 || dT>1200000) continue;
	if(dT<200000 || dR>200 || dT>1200000) continue;

	//  if(dT<200000 || dR>160 || dT>1200000) continue;
	
	//delayed energy cut 
	if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.4 && Ed < 5.6))) continue;
	//	if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.0 && Ed < 5.8))) continue;


	//dead time cut
	if(deadtime_veto>0) continue;

	//NsumMax cut
	if(NsumMax < 200) continue;

      // spallation cut
	//	if (isVeto >= 1) continue;

	
	//N200OD cut
	if(N200OD > 5) continue;


	//fiducial volume cut XeLS cut

		
			
	if( run <= 10675){

	  if (Rp >= 600.0 || Rd >= 600.0) continue;

	}



	if(run >= 11000){

	if (Rp >= 600.0 || Rd >= 600.0 || Rd <= 250) continue;

	  // delayed; cylinder cut
	if (Zd > 0 && sqrt((Xd*Xd)+(Yd*Yd)) < 250) continue;
	  
	}
	
	


	if (run >= 11000 && Rd >= 250)
	  {
	    cer << "Hi" << endl;

	  } 
	
	//	cerr << run << " " << run_grade << " "<< pevent<< " " << devent<< " " << Ep<< " " << Ed<< " " << Xp<< " " << Yp<< " " << Zp<< " " << Rp<< " " << Xd<< " " << Yd<< " " << Zd<< " " << Rd<< " " << dR<< " " << dT<< " "  << NsumMax<< " " << N200OD<< " " << chiQ<< " " << chiT<< " " << endl;

	  h_all ->Fill(energy);
	  h_all_root->Fill(energy);
	
	  //	h -> Fill(energy);



	//insert

	  // ############## likelihood ratio selection ##################### //
	  //
	  if(!KVFLikelihoodRatio::set_Ep(Ep)){
	    cerr << "Cannot set Ep" << endl;
	    abort();
	  }


	double dT_Uniform = dT / 1000.0; // accidental : 0.2-1.2 sec, candidate : 0.5-1000 musec

	//	double Likelihood_Ratio = KVFLikelihoodRatio::Likelihood_Ratio(dR, Rp, Rd, Ed, dT);
	double Likelihood_Ratio = KVFLikelihoodRatio::Likelihood_Ratio(dR, Rp, Rd, Ed, dT_Uniform);


	//	  cerr << "Likelihood : " << Likelihood_Ratio << " : " << KVFLikelihoodRatio::Likelihood_Ratio_Cut() << endl;
	  
	  if (Likelihood_Ratio > KVFLikelihoodRatio::Likelihood_Ratio_Cut()) {




		  h_ln->Fill(energy);
		  h_ln_root->Fill(energy);
		  ln = 1;
		}
	

	nt -> Fill();
	//	cerr << run << " " << run_grade << " "<< pevent<< " " << devent<< " " << Ep<< " " << Ed<< " " << Xp<< " " << Yp<< " " << Zp<< " " << Rp<< " " << Xd<< " " << Yd<< " " << Zd<< " " << Rd<< " " << dR<< " " << dT<< " "  << NsumMax<< " " << N200OD<< " " << chiQ<< " " << chiT<< " " << endl;




	N++;
      }
	}
    gvf.close();
    
    //    cout<<"read: "<<argv[i+1]<<endl;
    cout<<" --> "<<N<<endl;
    cout << "Likelihood Event : "<< h_ln -> GetEntries() << endl;

    cout<<endl;

    //	  cerr << "ok5" <<  endl; 
 }


  nt -> Write();
  h_ln_root->Write();
  h_all_root->Write();


  h_all -> SetLineWidth(3);
  h_all -> SetLineColor(4);
  //  h_ln -> SetFillColor(TColor::GetColor("#33ccff")); 
  h_ln -> SetLineWidth(3);
  h_ln -> SetLineColor(3);

  h_all -> Draw();
  h_ln -> Draw("same");

  c1 -> Print(ps_file);

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
