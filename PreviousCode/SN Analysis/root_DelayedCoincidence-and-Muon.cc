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

Int_t root_DelayedCoincidence_and_Muon()
{
  // -------------------------------------------------------------------------------------------------------- //



#if 1
  //  char *ps_file = "./picture/DelayedCoincidence-and-Veto-Accidental-v1-10000.ps";
  char *ps_file = "./picture/DelayedCoincidence-and-Muon.ps";

#endif

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


  const float XMIN = 0;
  const float XMAX = 30.0;
  const float BINWIDTH = 0.2;
  int bin = (int)((XMAX - XMIN) / BINWIDTH);
  TH1F* h_ln = new TH1F("h_ln", "", bin, XMIN, XMAX);//200keV bin
  TH1F* h_all = new TH1F("h_all", "", bin, XMIN, XMAX);//200keV bin




  // read Tree
  //
  // delayed coincidence
  int run, ln;
  float Ed, dR, dT, Ep, dQ, dL, badness, rp, rd, xd, yd, zd;


  //  for(int i=1; i <10000; i = i+500)

  //    {
      //    if (i == 8501)continue;


      // -- set root file name
      char RootFile[256];

      sprintf(RootFile, "./result/DelayedCoincidence-Muon.root");
      cerr << RootFile << endl;
      
      
      
      
      TFile *tfile = new TFile(RootFile);
      TTree *nt_dc = (TTree*)tfile -> Get("nt");
      TBranch *b_run           = nt_dc -> GetBranch("run");
      TBranch *b_Ed            = nt_dc -> GetBranch("Ed");
      TBranch *b_dR            = nt_dc -> GetBranch("dR");
      TBranch *b_dT            = nt_dc -> GetBranch("dT");
      TBranch *b_Ep            = nt_dc -> GetBranch("Ep");
      TBranch *b_Q             = nt_dc -> GetBranch("Q");
      TBranch *b_dQ            = nt_dc -> GetBranch("dQ");
      TBranch *b_dL            = nt_dc -> GetBranch("dL");
      TBranch *b_dTM           = nt_dc -> GetBranch("dTM");
      TBranch *b_badness       = nt_dc -> GetBranch("badness");
      TBranch *b_rp            = nt_dc -> GetBranch("rp");
      TBranch *b_rd            = nt_dc -> GetBranch("rd");
      TBranch *b_xd            = nt_dc -> GetBranch("xd");
      TBranch *b_yd            = nt_dc -> GetBranch("yd");
      TBranch *b_zd            = nt_dc -> GetBranch("zd");
      TBranch *b_ln            = nt_dc -> GetBranch("ln");

      
      
      
      
      
      
      // draw histogram
      //
      // veto = 0 -> with spallation cut
      // set address
      b_run              -> SetAddress(&run);
      b_Ep               -> SetAddress(&Ep);
      b_dT               -> SetAddress(&dT);
      b_dR               -> SetAddress(&dR);
      b_Ed               -> SetAddress(&Ed);
      //      b_NsumMax          -> SetAddress(&NsumMax);      
      //      b_N200OD           -> SetAddress(&N200OD);
      b_rp               -> SetAddress(&rp);
      b_rd               -> SetAddress(&rd);
      b_xd               -> SetAddress(&xd);
      b_yd               -> SetAddress(&yd);
      b_zd               -> SetAddress(&zd);
      b_ln               -> SetAddress(&ln);
      //      cerr << "ok" <<

      int N_dc = nt_dc -> GetEntries();

      //      cerr << N_dc << endl;

      for (int i = 0 ; i < N_dc ; i++)
	{
	  
	  b_run              -> GetEntry(i);
	  b_Ep               -> GetEntry(i);
	  b_dT               -> GetEntry(i);
	  b_dR               -> GetEntry(i);
	  b_Ed               -> GetEntry(i);
	  //	  b_deadtime_veto    -> GetEntry(i);
	  //	  b_NsumMax          -> GetEntry(i);
	  //	  b_N200OD           -> GetEntry(i);
	  b_rp               -> GetEntry(i);
	  b_rd               -> GetEntry(i);
	  b_xd               -> GetEntry(i);
	  b_yd               -> GetEntry(i);
	  b_zd               -> GetEntry(i);
	  b_ln               -> GetEntry(i);

	  //	  cerr << ln << endl;

	  
	  if (! (Ep >= 0 && Ep <= 30))continue;
	
	  
	if( run <= 10675){

       	if (rp >= 600.0 || rd >= 600.0) continue;
	//       	if (Rp < 600.0 && Rd < 600.0) {
	}


	//	if (Rp < 600.0 && Rd < 600.0 && Rd > 250) {
	if(run >= 11000){

	if (rp >= 600.0 || rd >= 600.0 || rd <= 250) continue;

	  // delayed; cylinder cut
	  if (zd > 0 && sqrt(xd*xd+yd*yd) < 250) continue;
	  
	}
	  
    // space and time correlation cut
    if (dR >= 200 || dT < 0.5 || dT >= 1000) continue;	  
	  	  
	//delayed energy cut 
	if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.4 && Ed < 5.6))) continue;
	//	if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.0 && Ed < 5.8))) continue;
	//	if (!(Ed >= 1.8 && Ed < 2.6)) continue;
	

	//	cout << "Ed   " << Ed << endl;


	//NsumMax cut
	//	if(NsumMax < 200) continue;

      // spallation cut
	//	if (isVeto >= 1) continue;

	  


	  h_all -> Fill(Ep);
	  


	  if (ln == 1)
	{
	  h_ln -> Fill(Ep);
	}
	  
	  
	}


      //    }
      
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
  return root_DelayedCoincidence_and_Muon();
}
#endif
