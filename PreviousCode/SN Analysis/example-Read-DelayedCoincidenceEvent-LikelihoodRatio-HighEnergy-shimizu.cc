/*
 make ascii file and show solar anti-neutrino with box selection

 (dcvf made by example-DelayedCoincidence-and-Veto is in /data/vf/v2-version2-solar-phase/)

> find /data/vf/dcvf/v2-version2-solar-phase/ -name "*.dcvf" | sort > ./dcvf.list

> ( ./example-Read-DelayedCoincidenceEvent-SolarAntiNeutrino.Linux > DelayedCoincidence-SolarAntiNeutrino.candidate ) >& DelayedCoincidence-SolarAntiNeutrino.log &
*/


#define MAKE_ROOT_FILE_ALL 1
#define MAKE_ROOT_FILE_LS  0


#include "KVF.hh"
//#include "KVFParticleEnergyKat.hh"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>

#include "TROOT.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"

#include "KVFLikelihoodRatio.hh"

using namespace std;

double GetVertexDiff(const KVFGeneralEvent* a, const KVFGeneralEvent* b);
double GetRadius(const KVFGeneralEvent* a);

bool Resolution_enable20inch = false;

int main(int argc, char* argv[])
{
  KVFGeneralEvent::setECorrMode(false);
  KVFParticleEnergy::PositronExtrapolate(); // fix 10.4.21
  KVFLikelihoodRatio::setVersion(2);

  // ------ File Name ------------------------------------------------------------------------ //

#if 0
  string ListFile  = "./analysis_list/dcvf-Kat.list-2013-9626";
  string Reactor = "./result/Observed-dcvf-Kat-shimikzu.dat-2013-9626";

  char root_output_filename[256]                       = "./result/DelayedCoincidence_shimizu_2013_9626.root";
  char root_output_filename_likelihood_selection[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_shimizu_2013_9626.root";
#endif

#if 0
  string ListFile  = "./analysis_list/dcvf-Kat.list-2011Paper";
  string Reactor = "./result/Observed-dcvf-Kat-shimikzu.dat-2011Paper";

  char root_output_filename[256]                       = "./result/DelayedCoincidence_shimizu_2011Paper.root";
  char root_output_filename_likelihood_selection[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_shimizu_2011Paper.root";
  string Rungrade = "./result/Observed-dcvf-Kat-rungrade-shimikzu.dat-2011Paper";
#endif

#if 0
  string ListFile  = "./analysis_list/dcvf-Kat.list-2013-179-11947";
  string Reactor = "./result/Observed-dcvf-Kat-shimikzu.dat-2013-11947";

  char root_output_filename[256]                       = "./result/DelayedCoincidence_shimizu_2013-11947.root";
  char root_output_filename_likelihood_selection[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_shimizu_2013-11947.root";
  string Rungrade = "./result/Observed-dcvf-Kat-rungrade-shimikzu.dat-2013-11947";
#endif


#if 1
  string ListFile  = "./analysis_list/dcvf-Kat.list-2013-11000-11947";
  string Reactor = "./result/Observed-dcvf-Kat-shimizu.dat-2013-11000-11947";

  char root_output_filename[256]                       = "./result/DelayedCoincidence_shimizu_2013-11000-11947.root";
  char root_output_filename_likelihood_selection[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_shimizu_2013-11000-11947.root";
  string Rungrade = "./result/Observed-dcvf-Kat-rungrade-shimizu.dat-2013-11000-11947";
#endif


  //  string ListFile  = "./analysis_list/dcvf-Kat.list-2011Paper";
  //  string ListFile  = "./analysis_list/dcvf-Kat.list-2013-9626";
  //  string ListFile  = "./dcvf.list-9626";
  //string ListFile  = "./dcvf.list-6801";
  

  //  string Reactor = "./result/Observed-dcvf-Kat-vertex.dat-2011Paper";
  //  string Reactor = "./result/Observed-dcvf-Kat.dat-shimizu-wo";
  //  string Reactor = "./SolarAntiNeutrino.dat";
  
    string RunInfoFile  = "./table/run-info-11947.table";
    //    string RunInfoFile  = "./table/run-info.table";
  //string RunInfoFile  = "./run-info.table-6801";
  


  // ------------------------------------------------------------------------------------------ //




   ofstream fReactor(Reactor.c_str());
   ofstream fRungrade(Rungrade.c_str());
  
  if (KVFRunInfo::ReadTable(RunInfoFile.c_str()) == false) {
    cerr << "Cannot set run info table" << endl;
    abort();
  }
  

#if MAKE_ROOT_FILE_ALL
  // ### root file
  string RootFile  = root_output_filename;
  TFile rootf(RootFile.c_str(), "RECREATE");
#endif


#if MAKE_ROOT_FILE_LS
  // ### root file
  string RootFile_ls  = root_output_filename_likelihood_selection;
  TFile rootf_ls(RootFile_ls.c_str(), "RECREATE");
#endif


  //  TFile rootf("./result/DelayedCoincidence-vertex-2011Paper.root", "RECREATE");
  //  TFile rootf("./result/DelayedCoincidence-shimizu-wo.root", "RECREATE");
  //  TFile rootf("DelayedCoincidence-SolarAntiNeutrino.root", "RECREATE");
  
  TROOT root("root", "test");

  TTree nt("nt", "");
  TTree nt_ls("nt_ls", "");

  int run, EventNumber, veto, prompt, NhitOD, N200OD, multiplicity;
  float Ed, dR, dT, dTp, Ep, xp, yp, zp, xd, yd, zd, rp, rd, Evis, unixtime;
  float vsTp, chiQp, chiTp, vsTd, chiQd, chiTd, VertexBadness, totalQ17, totalQOD; 

#if MAKE_ROOT_FILE_ALL
  nt.Branch("run", &run, "run/I");  
  nt.Branch("EventNumber", &EventNumber, "EventNumber/I");  
  nt.Branch("veto", &veto, "veto/I");  
  nt.Branch("prompt", &prompt, "prompt/I");  
  //  nt.Branch("unixtime", &unixtime, "unixtime/F");  
  nt.Branch("Ed", &Ed, "Ed/F");  
  nt.Branch("dR", &dR, "dR/F");  
  nt.Branch("dT", &dT, "dT/F");  
  nt.Branch("dTp", &dTp, "dTp/F");  
  nt.Branch("Ep", &Ep, "Ep/F");  
  nt.Branch("xp", &xp, "xp/F");  
  nt.Branch("yp", &yp, "yp/F");  
  nt.Branch("zp", &zp, "zp/F");  
  nt.Branch("xd", &xd, "xd/F");  
  nt.Branch("yd", &yd, "yd/F");  
  nt.Branch("zd", &zd, "zd/F");  
  nt.Branch("rp", &rp, "rp/F");  
  nt.Branch("rd", &rd, "rd/F");  
  nt.Branch("Evis", &Evis, "Evis/F");  
  nt.Branch("vsTp", &vsTp, "vsTp/F");  
  nt.Branch("chiQp", &chiQp, "chiQp/F");  
  nt.Branch("chiTp", &chiTp, "chiTp/F");  
  nt.Branch("vsTd", &vsTd, "vsTd/F");  
  nt.Branch("chiQd", &chiQd, "chiQd/F");  
  nt.Branch("chiTd", &chiTd, "chiTd/F");  
  nt.Branch("NhitOD", &NhitOD, "NhitOD/I");  
  nt.Branch("N200OD", &N200OD, "N200OD/I");  
  nt.Branch("totalQ17", &totalQ17, "totalQ17/F");  
  nt.Branch("totalQOD", &totalQOD, "totalQOD/F");  
  nt.Branch("VertexBadness", &VertexBadness, "VertexBadness/F");  
  nt.Branch("multiplicity", &multiplicity, "multiplicity/I");  
#endif

#if MAKE_ROOT_FILE_LS
  nt_ls.Branch("run", &run, "run/I");  
  nt_ls.Branch("EventNumber", &EventNumber, "EventNumber/I");  
  nt_ls.Branch("veto", &veto, "veto/I");  
  nt_ls.Branch("prompt", &prompt, "prompt/I");
  //  nt_ls.Branch("unixtime", &unixtime, "unixtime/F");    
  nt_ls.Branch("Ed", &Ed, "Ed/F");  
  nt_ls.Branch("dR", &dR, "dR/F");  
  nt_ls.Branch("dT", &dT, "dT/F");  
  nt_ls.Branch("Ep", &Ep, "Ep/F");  
  nt_ls.Branch("xp", &xp, "xp/F");  
  nt_ls.Branch("yp", &yp, "yp/F");  
  nt_ls.Branch("zp", &zp, "zp/F");  
  nt_ls.Branch("xd", &xd, "xd/F");  
  nt_ls.Branch("yd", &yd, "yd/F");  
  nt_ls.Branch("zd", &zd, "zd/F");  
  nt_ls.Branch("rp", &rp, "rp/F");  
  nt_ls.Branch("rd", &rd, "rd/F");  
  nt_ls.Branch("Evis", &Evis, "Evis/F");  
  nt_ls.Branch("vsTp", &vsTp, "vsTp/F");  
  nt_ls.Branch("chiQp", &chiQp, "chiQp/F");  
  nt_ls.Branch("chiTp", &chiTp, "chiTp/F");  
  nt_ls.Branch("vsTd", &vsTd, "vsTd/F");  
  nt_ls.Branch("chiQd", &chiQd, "chiQd/F");  
  nt_ls.Branch("chiTd", &chiTd, "chiTd/F");  
  nt_ls.Branch("NhitOD", &NhitOD, "NhitOD/I");  
  nt_ls.Branch("N200OD", &N200OD, "N200OD/I");  
  nt_ls.Branch("totalQ17", &totalQ17, "totalQ17/F");  
  nt_ls.Branch("totalQOD", &totalQOD, "totalQOD/F");  
  nt_ls.Branch("VertexBadness", &VertexBadness, "VertexBadness/F");  
  nt_ls.Branch("multiplicity", &multiplicity, "multiplicity/I");  
#endif
  


  int                    Last_NofNeutron = 0;
  unsigned long long int Last_TimeStamp_PromptEvent = 0;
  unsigned long long int Previous_TimeStamp_PromptEvent = 0;
  
  char file[256];
  ifstream flist(ListFile.c_str());
  
  int runnum;
 
  if (!flist) {
    cerr << "ERROR : don't exite such a list! -> " << ListFile << endl;
    exit(8);
  }
  while (flist >> runnum >> file) {

    cerr << file << endl;
    int RunNumber = runnum;
   
    KVFReader dcf(file);


    /*
 while (flist >> file) {
    KVFReader dcf(file);
    
    if (dcf.bad()) {
      cerr << "ERROR: Cannot open file1" << endl;
      return (-1);
    }
  
    */
    
    int N = 0;

    KVFDelayedCoincidenceEvent event;
    while (dcf.next(&event)) {


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
      




      if ( KVFRunInfo::HasReactorInfo(event.RunNumber) == false ) continue;    
      int run_grade = KVFRunInfo::Grade(event.RunNumber);
      if (run_grade < 0 || run_grade >= 10) continue;

      static const double MicroSec = 40.0;

    
      // set neutron multiplicity (0.5 < dT < 1000 usec, dR < 2 m)
      unsigned long long int TimeStamp_PromptEvent = event.Prompt.TimeStamp;
      double energy = KVFParticleEnergy::Positron(event.Prompt.Energy, event.Prompt.RunNumber);
      //      double energy = KVFParticleEnergyKat::Positron(event.Prompt.Energy);
      //double energy = KVFParticleEnergyKat::Positron(event.Prompt.EnergyA2);
      //double energy = KVFParticleEnergyKat::Positron(event.Prompt.EnergyA2, event.Prompt.RunNumber);
      double dt = (event.Delayed.TimeStamp - event.Prompt.TimeStamp) / MicroSec;

      if(TimeStamp_PromptEvent!=Last_TimeStamp_PromptEvent) Previous_TimeStamp_PromptEvent = Last_TimeStamp_PromptEvent;
      double dt_previous_prompt = (TimeStamp_PromptEvent - Previous_TimeStamp_PromptEvent) / MicroSec;

      static const double R_balloon = 650.0;
      static const double p0 = 55.18;
      static const double p1 = -0.0509;
      static const double p2 = 613.3;

//       double CorrectionFactor = R_balloon / (p0 * exp(p1 * energy) + p2);

      double CorrectionFactor = 1.0;
    
      if(energy>7.5){
	CorrectionFactor = R_balloon / (p0 * exp(p1 * energy) + p2);
      }

      xp = event.Prompt.X * CorrectionFactor;
      yp = event.Prompt.Y * CorrectionFactor;
      zp = event.Prompt.Z * CorrectionFactor;

      double Rp       = sqrt(xp*xp+yp*yp+zp*zp);

      xd = event.Delayed.X;
      yd = event.Delayed.Y;
      zd = event.Delayed.Z;

      double Rd       = sqrt(xd*xd+yd*yd+zd*zd);

      double dl = sqrt( (xp-xd)*(xp-xd) + (yp-yd)*(yp-yd) + (zp-zd)*(zp-zd) );

      rp = Rp;
      rd = Rd;

//       double dl = GetVertexDiff(&(event.Prompt), &(event.Delayed) );
//       double Rp = GetRadius(&(event.Prompt) );
//       double Rd = GetRadius(&(event.Delayed) );
//       double Rxyd = sqrt(event.Delayed.X * event.Delayed.X
// 			 + event.Delayed.Y * event.Delayed.Y
// 			 );

      double DelayedEnergy = KVFDelayedEnergy::DelayedEnergy(event.Delayed.Energy, energy, Rd, dt);
      //double DelayedEnergy = KVFDelayedEnergy::DelayedEnergy(event.Delayed.EnergyA2, energy, Rd, dt);
      
      int NofNeutron = 0;
      if (Last_TimeStamp_PromptEvent == TimeStamp_PromptEvent) {
	NofNeutron = Last_NofNeutron;
      }
      else {
	int NofForward = 0;
	do {
	  NofForward++;
	
	  if (event.Prompt.TimeStamp != TimeStamp_PromptEvent) break;
	  if (event.Delayed.TimeStamp >= (TimeStamp_PromptEvent + 40000)) break;
	
	  double dt0 = (event.Delayed.TimeStamp - event.Prompt.TimeStamp) / MicroSec;
	  double xp0 = event.Prompt.X * CorrectionFactor;
	  double yp0 = event.Prompt.Y * CorrectionFactor;
	  double zp0 = event.Prompt.Z * CorrectionFactor;

	  double Rp0       = sqrt(xp0*xp0+yp0*yp0+zp0*zp0);
	  
	  double xd0 = event.Delayed.X;
	  double yd0 = event.Delayed.Y;
	  double zd0 = event.Delayed.Z;

	  double Rd0       = sqrt(xd0*xd0+yd0*yd0+zd0*zd0);

	  double dl0 = sqrt( (xp0-xd0)*(xp0-xd0) + (yp0-yd0)*(yp0-yd0) + (zp0-zd0)*(zp0-zd0) );

	  double rp0 = Rp0;
	  double rd0 = Rd0;



          double DelayedEnergy0 = KVFDelayedEnergy::DelayedEnergy(event.Delayed.Energy, energy, Rd0, dt0);

	  //          if (dl0 < 160 && dt0 >= 0.5 && dt0 < 1000 && DelayedEnergy0 >= 1.8 && DelayedEnergy0 < 2.6) NofNeutron++;
          if (dl0 < 200 && dt0 >= 0.5 && dt0 < 1000 && DelayedEnergy0 >= 1.8 && DelayedEnergy0 < 2.6) NofNeutron++;

	} while (dcf.next(&event));    
	dcf.seek((-1 * NofForward), event.getBufSize());
	dcf.next(&event);
      }
      Last_NofNeutron            = NofNeutron;
      Last_TimeStamp_PromptEvent = TimeStamp_PromptEvent;    
    
      int isVeto = 0;
    
      if (event.Veto[0] > 0 || event.Veto[1] > 0 || event.Veto[3] > 0 || event.Veto[4] > 0) isVeto=1;
      //if (event.Veto[0] > 0 || event.Veto[1] > 0 || event.Veto[3] > 0 || event.Veto[4] > 0 || event.Veto[5] > 0) isVeto=1;
 
      int isPrompt = 1;
      if (!(event.Prompt.TriggerType & 0x02000000)) isPrompt = 0;
    

      // fill paramters
      run = event.RunNumber;
      EventNumber = event.Prompt.EventNumber;
      Ed = (DelayedEnergy);      dR = dl;
      dT = dt;
      dTp = dt_previous_prompt;
      Ep = energy;
//       xp = event.Prompt.X;
//       yp = event.Prompt.Y;
//       zp = event.Prompt.Z;
//       xd = event.Delayed.X;
//       yd = event.Delayed.Y;
//       zd = event.Delayed.Z;
//      unixtime = event.Prompt.UnixTime;
//       rp = Rp;
//       rd = Rd;
      Evis = event.Prompt.Energy;
      //Evis = event.Prompt.EnergyA2;
      veto = isVeto;
      prompt = isPrompt;
      vsTp = event.Prompt.VertexSigmaT;
      chiQp = event.Prompt.VertexChi2Q;
      chiTp = event.Prompt.VertexChi2T;
      vsTd = event.Delayed.VertexSigmaT;
      chiQd = event.Delayed.VertexChi2Q;
      chiTd = event.Delayed.VertexChi2T;
      NhitOD = event.Prompt.NhitOD;
      N200OD = event.Prompt.N200OD;
      totalQ17 = event.Prompt.TotalCharge17;
      totalQOD = event.Prompt.TotalChargeOD;
      VertexBadness = event.Prompt.VertexBadness;
      multiplicity = NofNeutron;



#if MAKE_ROOT_FILE_ALL
      nt.Fill();
#endif
      N++;

      /// delayed coincidence ///

      // space and time correlation cut
      if (dl >= 200 || dt < 0.5 || dt >= 1000) continue;
      //      if (dl >= 160 || dt < 0.5 || dt >= 1000) continue;
    
      // delayed energy cut
      //      if (!((DelayedEnergy >= 1.8 && DelayedEnergy < 2.6) || (DelayedEnergy >= 4.0 && DelayedEnergy < 5.8))) continue;
      if (!((DelayedEnergy >= 1.8 && DelayedEnergy < 2.6) || (DelayedEnergy >= 4.4 && DelayedEnergy < 5.6))) continue;
      //      if (!(DelayedEnergy >= 1.8 && DelayedEnergy < 2.6)) continue;
    
      // spallation cut
      if (isVeto >= 1) continue;

      // trigger threshold cut
      if (event.Prompt.NsumMax < 200) continue;  
    
      /// solar anti-neutrino selection ///
      //if (energy >= 7.5 && energy < 30.0 && N200OD < 5 && multiplicity <= 1) {

      //      if (energy >= 7.5 && energy < 30.0 && N200OD < 5) {

      if (energy >= 0.9 && energy < 30.0 && N200OD < 5) {

	//fiducial volume cut XeLS cut


	if( RunNumber <= 10675){

       	if (Rp >= 600.0 || Rd >= 600.0) continue;
	//       	if (Rp < 600.0 && Rd < 600.0) {
	}


	//	if (Rp < 600.0 && Rd < 600.0 && Rd > 250) {
	if(RunNumber >= 11000){

	if (Rp >= 600.0 || Rd >= 600.0 || Rd <= 250) continue;

	  // delayed; cylinder cut
	  if (zd > 0 && sqrt(xd*xd+yd*yd) < 250) continue;
	  
	}


	  // ############## likelihood ratio selection ##################### //
	  //
	  if(!KVFLikelihoodRatio::set_Ep(Ep)){
	    cerr << "Cannot set Ep" << endl;
	    abort();
	  }

	  double Likelihood_Ratio = KVFLikelihoodRatio::Likelihood_Ratio(dR, Rp, Rd, Ed, dT);
	  
	  if (Likelihood_Ratio > KVFLikelihoodRatio::Likelihood_Ratio_Cut()) {


	    /*
	  printf("Run: %06d\n", (int)event.RunNumber);
	  printf("  TimeDiff = %8.3f[microsec]  VertexDiff = %8.3f[cm] (TimeDiff_PreviousPrompt = %8.3f[microsec]  )\n", (float)dt, (float)dl, (float)dt_previous_prompt);
	  printf("  Prompt (%9d): energy=%8.3f[MeV]   trigger=0x%08x   R= %6.2f[cm]   Status=%d\n", 
		 event.Prompt.EventNumber,
		 (float)energy, 
		 (unsigned int)event.Prompt.TriggerType,
		 (float)Rp,
		 event.Prompt.EventStatus
		 );
	  printf("                %8.2f  %8.2f  %8.2f\n",
		 xp,
		 yp,
		 zp);
// 		 event.Prompt.X,
// 		 event.Prompt.Y,
// 		 event.Prompt.Z);
	
	
	  printf("  Delayed(%9d): energy=%8.3f[MeV]   trigger=0x%08x   R= %6.2f[cm]   Status=%d\n", 
		 event.Delayed.EventNumber,
		 (float)DelayedEnergy,
		 (unsigned int)event.Delayed.TriggerType,
		 (float)Rd,
		 event.Delayed.EventStatus
		 );
	  printf("                %8.2f  %8.2f  %8.2f\n",
		 xd,
		 yd,
		 zd);
// 		 event.Delayed.X,
// 		 event.Delayed.Y,
// 		 event.Delayed.Z);
	  printf("\n");
	    */
	
	  fReactor << event.RunNumber << " " << energy << " " << endl;
	  fRungrade << event.RunNumber <<  " " << run_grade << " " << endl;


#if MAKE_ROOT_FILE_LS
   	    nt_ls.Fill();
#endif
	
	  }
      }
    }

    dcf.close();
  
    cerr << "read: " << file << endl;
    cerr << " --> " << N << endl;
    cerr<<endl;
  }


  
#if MAKE_ROOT_FILE_ALL
  nt.Write();
  rootf.Close();
#endif

#if MAKE_ROOT_FILE_LS
  nt_ls.Write();
  rootf_ls.Close();
#endif





  return 0;
}

double GetVertexDiff(const KVFGeneralEvent* a, const KVFGeneralEvent* b)
{
  return sqrt((a->X - b->X) * (a->X - b->X)
	       + (a->Y - b->Y) * (a->Y - b->Y)
	       + (a->Z - b->Z) * (a->Z - b->Z)
	       );
}

double GetRadius(const KVFGeneralEvent* a)
{
  return sqrt((a->X) * (a->X) + (a->Y) * (a->Y) + (a->Z) * (a->Z));
}

