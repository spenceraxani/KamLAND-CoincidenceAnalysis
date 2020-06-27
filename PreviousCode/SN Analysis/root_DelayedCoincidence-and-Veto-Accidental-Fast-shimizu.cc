#ifndef __CINT__
#include "TROOT.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TColor.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TCut.h"
#include "TLine.h"
#include "TLatex.h"
#include "TGaxis.h"
#include "TGraphErrors.h"
#include "TPostScript.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
using namespace std;
#endif

Int_t root_DelayedCoincidence_and_Veto_Accidental_Fast()
{
  // -------------------------------------------------------------------------------------------------------- //



#if 1
  //  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_shimizu_2011Paper.root";
  //  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_shimizu_2011Paper.root";
  //  char *ps_file = "./picture/DelayedCoincidence-and-Veto-Accidental-all-v2.ps";
  //  char *ps_file = "./picture/DelayedCoincidence-and-Veto-Accidental-all-Rall2.ps";
  char *ps_file = "./picture/DelayedCoincidence-and-Veto-Accidental-v1-10000-5-30.ps";





  //  string ofs_file_name = "./table/list-for-Oki-shimizu-2011Paper.dat"; 
#endif

  //  string ofs_file_name = "./table/list-for-Oki-vertex-2013-9626.dat"; 
  //  string ofs_file_name = "./table/list-for-Oki-shimizu-2013-9626.dat"; 


  // -------------------------------------------------------------------------------------------------------- //



  //----------Output-----------//
  //  ofstream ofs(ofs_file_name.c_str());

  

  
  // set canvas style
  //
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


  const float XMIN = 0.0;
  const float XMAX = 30.0;
  const float BINWIDTH = 0.2;
  int bin = (int)((XMAX - XMIN) / BINWIDTH);
  TH1F* h_ln = new TH1F("h_ln", "", bin, XMIN, XMAX);//200keV bin
  TH1F* h_all = new TH1F("h_all", "", bin, XMIN, XMAX);//200keV bin




  // read Tree
  //
  // delayed coincidence
  int run, EventNumber, veto, prompt, NhitOD, N200OD, multiplicity;
  float Ed, dR, dT, Ep, xp, yp, zp, xd, yd, zd, rp, rd, Evis, vsTp, chiQp, chiTp, vsTd, chiQd, chiTd, totalQ17, totalQOD, VertexBadness;


  for(int i=1; i <10000; i = i+500)
  //    for(int i=11001; i <11500; i = i+500)
    {
      //    if (i == 8501)continue;


      // -- set root file name
      char RootFile[256];

      //      sprintf(RootFile, "./result/DelayedCoincidence-and-Veto-Accidental_%06d-%06d.root", i, i+499);
      //      sprintf(RootFile, "./result/DelayedCoincidence-and-Veto-Accidental-Rall2_%06d-%06d.root", i, i+499);
      sprintf(RootFile, "./result/DelayedCoincidence-and-Veto-Accidental-v1-%06d-%06d.root", i, i+499);
      //      sprintf(RootFile, "./result/DelayedCoincidence-and-Veto-Accidental_%06d-%06d-V2.root", i, i+499);
      //sprintf(RootFile, "./result/DelayedCoincidence-and-Veto-Accidental_%06d-%06d-V2.root", i, i+499);
      cerr << RootFile << endl;
      
      
      int deadtime_veto, full_volume_veto, track_veto, badness_veto, run_grade, RunNumber, ln;
      int run, pevent, devent, unixtime, livetime, ispps, NsumMax, N200OD;
      float Ep, Ed, Rp, Rd, Xp,Yp,Zp, Xd,Yd,Zd, dR, Rxyd, energy, chiQ, chiT, VertexBadness;
      double dT, dTmuon;
      
      
      
      TFile *tfile = new TFile(RootFile);
      TTree *nt_dc = (TTree*)tfile -> Get("nt");
      TBranch *b_run           = nt_dc -> GetBranch("run");
      TBranch *b_run_grade     = nt_dc -> GetBranch("run_grade");
      TBranch *b_unixtime      = nt_dc -> GetBranch("unixtime");
      TBranch *b_pevent        = nt_dc -> GetBranch("pevent");
      TBranch *b_devent        = nt_dc -> GetBranch("devent");
      TBranch *b_Ep            = nt_dc -> GetBranch("Ep");
      TBranch *b_Ed            = nt_dc -> GetBranch("Ed");
      TBranch *b_Xp            = nt_dc -> GetBranch("Xp");
      TBranch *b_Yp            = nt_dc -> GetBranch("Yp");
      TBranch *b_Zp            = nt_dc -> GetBranch("Zp");
      TBranch *b_Rp            = nt_dc -> GetBranch("Rp");
      TBranch *b_Xd            = nt_dc -> GetBranch("Xd");
      TBranch *b_Yd            = nt_dc -> GetBranch("Yd");
      TBranch *b_Zd            = nt_dc -> GetBranch("Zd");
      TBranch *b_Rd            = nt_dc -> GetBranch("Rd");
      TBranch *b_dR            = nt_dc -> GetBranch("dR");
      TBranch *b_dT            = nt_dc -> GetBranch("dT");
      TBranch *b_Rxyd          = nt_dc -> GetBranch("Rxyd");
      TBranch *b_NsumMax       = nt_dc -> GetBranch("NsumMax");
      TBranch *b_N200OD        = nt_dc -> GetBranch("N200OD");
      TBranch *b_deadtime_veto = nt_dc -> GetBranch("deadtime_veto");
      TBranch *b_chiQ          = nt_dc -> GetBranch("chiQ");
      TBranch *b_chiT          = nt_dc -> GetBranch("chiT");
      TBranch *b_VertexBadness = nt_dc -> GetBranch("VertexBadness");
      TBranch *b_dTmuon        = nt_dc -> GetBranch("dTmuon");
      TBranch *b_ln            = nt_dc -> GetBranch("ln");
      
      
      
      
      
      
      // draw histogram
      //
      // veto = 0 -> with spallation cut
      // set address
      b_run              -> SetAddress(&run);
      b_ln               -> SetAddress(&ln);
      b_Ep               -> SetAddress(&Ep);
      b_dT               -> SetAddress(&dT);
      b_dR               -> SetAddress(&dR);
      b_Ed               -> SetAddress(&Ed);
      b_deadtime_veto    -> SetAddress(&deadtime_veto);
      b_NsumMax          -> SetAddress(&NsumMax);      
      b_N200OD           -> SetAddress(&N200OD);
      b_Rp               -> SetAddress(&Rp);
      b_Rd               -> SetAddress(&Rd);
      b_Xd               -> SetAddress(&Xd);
      b_Yd               -> SetAddress(&Yd);
      b_Zd               -> SetAddress(&Zd);



      int N_dc = nt_dc -> GetEntries();
      for (int i = 0 ; i < N_dc ; i++)
	{
	  
	  b_run              -> GetEntry(i);
	  b_ln               -> GetEntry(i);
	  b_Ep               -> GetEntry(i);
	  b_dT               -> GetEntry(i);
	  b_dR               -> GetEntry(i);
	  b_Ed               -> GetEntry(i);
	  b_deadtime_veto    -> GetEntry(i);
	  b_NsumMax          -> GetEntry(i);
	  b_N200OD           -> GetEntry(i);
	  b_Rp               -> GetEntry(i);
	  b_Rd               -> GetEntry(i);
	  b_Xd               -> GetEntry(i);
	  b_Yd               -> GetEntry(i);
	  b_Zd               -> GetEntry(i);


	  if (! (Ep >= 5.0 && Ep <= 30))continue;
	  //	  if (! (Ep <= 30))continue;
	
	  
	if( run <= 10675){

       	if (Rp >= 600.0 || Rd >= 600.0) continue;
	//       	if (Rp < 600.0 && Rd < 600.0) {
	}


	//	if (Rp < 600.0 && Rd < 600.0 && Rd > 250) {
	if(run >= 11000){

	if (Rp >= 600.0 || Rd >= 600.0 || Rd <= 250) continue;

	  // delayed; cylinder cut
	  if (Zd > 0 && sqrt(Xd*Xd+Yd*Yd) < 250) continue;
	  
	}
	  
	  
	  	  
	//same cut with anti-neutrino except time window cut(0.2sec~1.2sec)
	if(dT<200000 || dR>200 || dT>1200000) continue;
	//	if(dT<200000 || dR>160 || dT>1200000) continue;
	
	//delayed energy cut 
	if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.4 && Ed < 5.6))) continue;
	//	if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.0 && Ed < 5.8))) continue;
	//	if (!(Ed >= 1.8 && Ed < 2.6)) continue;
	

	//	cout << "Ed   " << Ed << endl;


	//dead time cut
	if(deadtime_veto>0) continue;

	//NsumMax cut
	if(NsumMax < 200) continue;

      // spallation cut
	//	if (isVeto >= 1) continue;

	
	//N200OD cut
	if(N200OD > 5) continue;


	//fiducial volume cut XeLS cut


	  


	  h_all -> Fill(Ep);
	  


	  if (ln == 1)
	{
	  h_ln -> Fill(Ep);
	}
	  
	  
	}

      cout << "only 1 cycle : " << h_all -> GetEntries() << endl;

    }
      
  cout << "all events :  " << h_all -> GetEntries() << endl; 
  cout << "ln events :  " << h_ln -> GetEntries() << endl; 

  //  double y_min1 = h_all -> GetMinimum();
  //  double y_max1 = h_all -> GetMaximum();
  
  //  h1 -> SetMinimum(1.0);
  //  h1 -> SetMaximum(y_max1*1.7);
  h_all -> SetXTitle("Prompt Energy [MeV]");
  h_all -> SetYTitle("Events/0.2MeV");
  h_all -> SetLineWidth(3);
  h_all -> SetLineColor(4);
  h_all -> Draw();
  //  h_ln -> SetFillColor(TColor::GetColor("#33ccff")); 
  h_ln -> SetLineWidth(3);
  h_ln -> SetLineColor(5);

  h_ln -> Draw("same");
  c1 -> Update();
  
  
  //    TGaxis* gaxis_1[2];
  //    gaxis_1[0] = new TGaxis(XMIN1, y_min1, XMAX1, y_min1, XMIN1, XMAX1, 510,"");
  //    gaxis_1[1] = new TGaxis(XMIN1, y_min1, XMIN1, y_max1, y_min1 , y_max1*1.7, 510,"G");
  
  


  //    for (int i = 0 ; i < 2 ; i++) {
  //      gaxis_1[i] -> SetLabelSize(0);
  //      gaxis_1[i] -> Draw();
  //    }
  
  //  c1 -> Update();


      

  
  c1 -> Print(ps_file);
      
  return 0;
    }



#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_DelayedCoincidence_and_Veto_Accidental_Fast();
}
#endif
