/*

0. make list file
> ./perl_make_list.pl

1. check data file and compile
## root file
## input file

> gmake TARGET=example-Read-DelayedCoincidenceEvent-Muon-LikelihoodRatio

2. select candidate events 
w/o "cout"
> ( ./example-Read-DelayedCoincidenceEvent-Muon-LikelihoodRatio.Linux) >& ./log/DelayedCoincidence-Muon.log-110921 &
> ( ./example-Read-DelayedCoincidenceEvent-Muon-LikelihoodRatio.Linux) >& ./log/DelayedCoincidence-Muon.log-121218 &

with "cout"
> ( ./example-Read-DelayedCoincidenceEvent-Muon-LikelihoodRatio.Linux > DelayedCoincidence-Muon.candidate ) >& ./log/DelayedCoincidenceEvent-Muon.log &

*/

#include "KVF.hh"
#include "KVFDelayedCoincidenceEventMuon.hh"
#include <iostream>
#include <math.h>
#include <stdio.h>

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

bool setPDF();

int main(int argc, char* argv[])
{
  KVFGeneralEvent::setECorrMode(false);
  KVFParticleEnergy::PositronExtrapolate();
  KVFLikelihoodRatio::setVersion(2);

  // ------ File Name ------------------------------------------------------------------------ //
#if 0
  // dcvf list
  char dcvf_list_input_filename[256] = "./analysis_list/muon-dcvf-A2.list-110426";
  
  // output file
  char root_output_filename[256]     = "./result/DelayedCoincidence-Muon_110426.root";
#endif

#if 0
  // dcvf list
  char dcvf_list_input_filename[256] = "./analysis_list/muon-dcvf-A2.list-110921";
  
  // output file
  char root_output_filename[256]     = "./result/DelayedCoincidence-Muon_110921.root";
#endif

#if 0
  // dcvf list
  char dcvf_list_input_filename[256] = "./analysis_list/muon-dcvf-A2.list-Dthesis";
  
  // output file
  char root_output_filename[256]     = "./result/DelayedCoincidence-Muon_Dthesis.root";
#endif


#if 0
  // dcvf list
  char dcvf_list_input_filename[256] = "./analysis_list/muon-dcvf-A2.list-KamLAND";
  
  // output file
  char root_output_filename[256]     = "./result/DelayedCoincidence-Muon_KamLAND.root";
#endif


#if 0
  // dcvf list
  char dcvf_list_input_filename[256] = "./analysis_list/muon-dcvf-A2.list-KamLAND-9088-10676";
  
  // output file
  char root_output_filename[256]     = "./result/DelayedCoincidence-Muon_KamLAND-9088-10676.root";
#endif

#if 0
  // dcvf list
  char dcvf_list_input_filename[256] = "./analysis_list/muon-dcvf-A2.list-KamLAND-DS1";
  
  // output file
  char root_output_filename[256]     = "./result/DelayedCoincidence-Muon_KamLAND_DS1.root";
#endif

#if 0
  // dcvf list
  char dcvf_list_input_filename[256] = "./analysis_list/muon-dcvf-A2-2011Paper.list";
  
  // output file
  char root_output_filename[256]     = "./result/DelayedCoincidence-Muon-2011Paper.root";
#endif

#if 1
  // dcvf list
  char dcvf_list_input_filename[256] = "./analysis_list/muon-dcvf-A2-DS12.list";
  
  // output file
  char root_output_filename[256]     = "./result/DelayedCoincidence-Muon-DS12.root";
#endif


#if 1  
  // constant table <FIX>
  char trigger_filename[256]         = "./table/trigger_conditions_11947.dat";
  char run_info_filename[256]        = "./table/run-info-11947.table";
#endif


#if 0
  // constant table <FIX>
  char trigger_filename[256]         = "./table/trigger_conditions.dat";
  char run_info_filename[256]        = "./table/run-info.table-Dthesis";
#endif



  // cut conditions <FIX>
  double Fiducial  = 600;
  double dTCut     = 1000;
  double dRCut     = 200;
  double RxyCut    = 0;

  // ------------------------------------------------------------------------------------------ //

  
  // read constant table & set filename
  //
  // ### run info table & trigger info table
  string RunInfoFile      = run_info_filename;
  string TriggerInfoFile  = trigger_filename;
  if (KVFRunInfo::ReadTable(RunInfoFile.c_str()) == false
      || KVFRunInfo::ReadRunInfoTable(TriggerInfoFile.c_str())==false) {
    cerr << "Cannot set run info table" << endl;
    cerr << "Please check environmental variable: KAMLAND_CONST_DIR = " << getenv("KAMLAND_CONST_DIR") << endl;
    abort();
  }

  // ### read input list
  string ListFile  = dcvf_list_input_filename;
  
  // ### root file
  string RootFile  = root_output_filename;
  TFile rootf(RootFile.c_str(), "RECREATE");

  // set paramater
  int Ncoincidence = 0;


  // ------------------------------- 8He/9Li Event Selection ------------------------------- //

  // set paramaters
  TROOT root("root", "test");
  TNtuple nt("nt", "", "run:Ed:dR:dT:Ep:Q:dQ:dL:dTM:badness:Evis:rp:rd");

  int LastEvent = 0;
  int NofNeutrino = 0;
  bool IsRead = false;
  
  // read list file
  int run;
  char file[256];
  ifstream flist(ListFile.c_str());
  if (!flist) {
    cerr << "ERROR : list file -> " << ListFile << endl;
    exit(8);
  }
  
  while (flist >> run >> file) {

    if( KVFRunInfo::HasReactorInfo(run) == false ) continue;
    int run_grade = KVFRunInfo::Grade(run);
    if(run_grade<0 || run_grade>=10) continue;


    // ### high dark rate period ###
    if(6820<run&&run<6890) continue; 
    if(run==6942) continue;
    if(7930<run&&run<8070) continue;
    // -------------------------- //
    
    KVFReader dcf(file);
    KVFDelayedCoincidenceEventMuon event;
    
    int N = 0;

    if (dcf.bad()) {
      cerr << "ERROR: Cannot open file run" << run << endl;
      dcf.close();
      continue;
    }

    
    while(dcf.next(&event)){
      int VersionOfKamLAND = KVFRunInfo::VersionOfKamLAND(event.RunNumber);
      int RunNumber = event.RunNumber;

      
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


      static const double MicroSec = 40.0;
      double dt = (event.Delayed.TimeStamp - event.Prompt.TimeStamp)/MicroSec;
      //      double dl = GetVertexDiff( &(event.Prompt), &(event.Delayed) );
      //      double Rp = GetRadius( &(event.Prompt) );
      //      double Rd = GetRadius( &(event.Delayed) );
      double Rxyd = sqrt(event.Delayed.X*event.Delayed.X
			 + event.Delayed.Y*event.Delayed.Y
			 );


      //insert
      double energy = KVFParticleEnergy::Positron(event.Prompt.EnergyA2, RunNumber);

      static const double R_balloon = 650.0;
      static const double p0 = 55.18;
      static const double p1 = -0.0509;
      static const double p2 = 613.3;

      double CorrectionFactor = 1.0;
    
      if(energy>7.5){
	CorrectionFactor = R_balloon / (p0 * exp(p1 * energy) + p2);
      }

      double      xp = event.Prompt.X * CorrectionFactor;
      double      yp = event.Prompt.Y * CorrectionFactor;
      double      zp = event.Prompt.Z * CorrectionFactor;

      //      double Rp       = sqrt(xp*xp+yp*yp+zp*zp);
      double      Rp       = sqrt(xp*xp+yp*yp+zp*zp);
      double     rp = Rp;
      double      xd = event.Delayed.X;
      double      yd = event.Delayed.Y;
      double      zd = event.Delayed.Z;


      //      double Rd       = sqrt(xd*xd+yd*yd+zd*zd);
      double      Rd       = sqrt(xd*xd+yd*yd+zd*zd);
      double rd =Rd;
      double dl = sqrt( (xp-xd)*(xp-xd) + (yp-yd)*(yp-yd) + (zp-zd)*(zp-zd) );


 

      
      // nsum cut
      if(event.Prompt.NsumMax < KVFRunInfo::Prompt(RunNumber)) continue;
    
      if(dt>2500 || 
	 dl>300 ||
	 event.Delayed.EnergyA2 <1.0
	 ) continue;


      // Muon noise cut
      if(event.Muon.isNoise()==true) continue;

      Ncoincidence++;

      int isPrompt=1;
      if( !(event.Prompt.TriggerType & 0x02000000) ) isPrompt=0;
      //      double energy = KVFParticleEnergy::Positron(event.Prompt.EnergyA2, RunNumber);
      
      // fiducial, time, space and cylinder cut
      //      if(Rp>Fiducial || Rd>Fiducial || dt>dTCut || dl>dRCut || dt<0.5) continue;
      if(dt>dTCut || dl>dRCut || dt<0.5) continue;
    
      // delayed energy cut 
      if((event.Delayed.EnergyA2)<1.8 || (event.Delayed.EnergyA2)>2.6) continue;

      // prompt energy cut
      if(energy>30 || energy<0.9) continue;

      //muon info
      float sciL,cheL;
      event.Muon.getTrackLength(&sciL,&cheL);

      double Q  = event.Muon.TotalCharge17;
      double dQ = KVFResidualCharge::dQ(event.Muon.TotalCharge17, cheL, sciL, RunNumber);
      //    float dQ = event.Muon.TotalCharge17-31.45*cheL-629.4*sciL; // version3
      float dL  = event.Muon.getDistanceFrom(event.Delayed.X,event.Delayed.Y,event.Delayed.Z);
      float dTM = (event.Prompt.TimeStamp - event.Muon.TimeStamp) / 40000000.0;
      float badness = event.Muon.Badness;

      double Ep = energy;
      double dR = dl;
      double Ed = KVFDelayedEnergy::DelayedEnergy(event.Delayed.EnergyA2, energy, Rd, dt);
      double dT = dt;

      // likelihood ratio selection
      if(!KVFLikelihoodRatio::set_Ep(Ep)){
	cerr << "Cannot set Ep" << endl;
	abort();
      }
      
      double Likelihood_Ratio = KVFLikelihoodRatio::Likelihood_Ratio(dR, Rp, Rd, Ed, dT);
      
      if (Likelihood_Ratio > KVFLikelihoodRatio::Likelihood_Ratio_Cut()) {

	nt.Fill(RunNumber,
		Ed,
		dl, 
		dt,
		energy,
		event.Muon.TotalCharge17,
		dQ,
		dL,
		dTM,
		badness,
		energy,
		Rp,
		Rd);
      
	printf("Run: %06d\n", (int)RunNumber);
	printf("  TimeDiff = %8.3f[microsec]  VertexDiff = %8.3f[cm]\n", (float)dt, (float)dl);
	printf("  Prompt (%9d): energy=%8.3f[MeV]   trigger=0x%08x   R= %4d[cm]   Status=%d\n", 
	       event.Prompt.EventNumber,
	       (float)energy, 
	       (unsigned int)event.Prompt.TriggerType,
	       (int)Rp,
	       event.Prompt.EventStatus
	       );
	printf("                %8.3f  %8.3f  %8.3f\n",
	       event.Prompt.X,
	       event.Prompt.Y,
	       event.Prompt.Z);
      
      
	printf("  Delayed(%9d): energy=%8.3f[MeV]   trigger=0x%08x   R= %4d[cm]   Status=%d\n", 
	       event.Delayed.EventNumber,
	       (float)event.Delayed.EnergyA2,
	       (unsigned int)event.Delayed.TriggerType,
	       (int)Rd,
	       event.Delayed.EventStatus
	       );
	printf("                %8.3f  %8.3f  %8.3f\n",
	       event.Delayed.X,
	       event.Delayed.Y,
	       event.Delayed.Z);
      
	printf("\n");
      
	printf("  Muon(%9d): TotalCharge17=%8.3f[p.e.]   trigger=0x%08x\n", 
	       event.Muon.EventNumber,
	       (float)event.Muon.TotalCharge17,
	       (unsigned int)event.Muon.TriggerType
	       );
	printf("        dQ=%15.3f[p.e]  dL=%8.3f[cm]  dTM=%10.5f[sec]",
	       dQ,
	       dL,
	       dTM
	       );
	printf("\n");
      
	printf("        badness=%5d",
	       (int) badness
	       );
      
	printf("\n");
      
      }
      

#if 0
      //energitic or miss reconstructed muon
      if( ((Q>40000 && dQ>1000000) || (Q>40000 && badness>=100))){
	fMuon_Energetic << Ep << " " << dTM << endl;
      }
    
      //low charge or well reconstructed non-energetic muon (dL<=300cm)
      if( ((Q<40000) || (Q>40000 && badness<100 && dQ<1000000)) && dL<=300){
	fMuon_NonEnergetic << Ep << " " << dTM << endl;
      }
    
      //energitic or miss reconstructed muon --- NoDouble
      if( ((Q>40000 && dQ>1000000) || (Q>40000 && badness>=100)) && dTM<0.5 && LastEvent!=event.Prompt.EventNumber){
	fMuon_Energetic_NoDouble << RunNumber << " " << Ep << endl;
	LastEvent = event.Prompt.EventNumber;
      }
    
      //low charge or well reconstructed non-energetic muon (dL<=300cm) --- NoDouble
      if( ((Q<40000) || (Q>40000 && badness<100 && dQ<1000000)) && dL<=300 && dTM<0.5 && LastEvent!=event.Prompt.EventNumber){
	fMuon_NonEnergetic_NoDouble << RunNumber << " " << Ep << endl;
	LastEvent = event.Prompt.EventNumber;
      }
#endif      
    }

    dcf.close();
  }

  nt.Write();
  rootf.Close();
  
  return 0;
}


double GetVertexDiff(const KVFGeneralEvent* a, const KVFGeneralEvent* b){
  return sqrt( (a->X - b->X)*(a->X - b->X)
	       + (a->Y - b->Y)*(a->Y - b->Y)
	       + (a->Z - b->Z)*(a->Z - b->Z)
	       );
}
double GetRadius(const KVFGeneralEvent* a){
  return sqrt( (a->X)*(a->X) + (a->Y)*(a->Y) + (a->Z)*(a->Z) );
}
