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
#include "TH2.h"
#include "TF1.h"
#include "TCut.h"
#include "TLine.h"
#include "TLatex.h"
#include "TGraphErrors.h"
#include "TPostScript.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
using namespace std;
#endif

Int_t root_check_vertex_distribution_accidental()
{
  // -------------------------------------------------------------------------------------------------------- //

  //  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_shimizu_2013-11947.root";


  //  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_shimizu_2013-11947.root";


  char *ps_file = "./picture/picture_prompt_vertex_accidental.ps";
  // -------------------------------------------------------------------------------------------------------- //

  // set canvas style
  //
  gROOT  -> SetStyle("Plain");
  gStyle -> SetPalette(100);
  //  gStyle -> SetPalette(1);
  gStyle -> SetStatFont(42);
  gStyle -> SetOptStat(0);
  gStyle -> SetLabelFont(42,"XYZ");
  gStyle -> SetLabelSize(0.055, "XY");
  gStyle -> SetLabelOffset(0.008, "X");
  gStyle -> SetLabelOffset(0.008, "Y");
  gStyle -> SetTitleFont(42, "XY");
  gStyle -> SetTitleSize(0.055, "XY");
  gStyle -> SetTitleOffset(1.1, "X");
  gStyle -> SetTitleOffset(0.9, "Y");

  TCanvas *c1 = new TCanvas("c1", "Title", 0, 0, 700, 700);
  c1 -> SetFillStyle(4001);
  c1 -> SetBottomMargin(0.15);
  c1 -> SetLeftMargin(0.12);
  c1 -> SetRightMargin(0.12);
  c1 -> SetLogz();

  // make picture
  int type = 112;
  TPostScript *ps = new TPostScript(ps_file, type);
  ps -> Range(25, 12);



  // draw histogram
  //
  const double XMIN = 0.0;
  const double XMAX = 50.0;
  const double YMIN = -8.5;
  const double YMAX = 8.5;
  const double BIN_WIDTHX = 0.3;
  const double BIN_WIDTHY = 0.15;
  int binX = (int)((XMAX - XMIN) / BIN_WIDTHX);
  int binY = (int)((YMAX - YMIN) / BIN_WIDTHY);

  TH2D* h = new TH2D("h", "", binX, XMIN, XMAX, binY, YMIN, YMAX);
  TH2D* h_lh = new TH2D("h_lh", "", binX, XMIN, XMAX, binY, YMIN, YMAX);
  TGraph *g_lh = new TGraph;


  // read Tree
  //
  // delayed coincidence
  int run, EventNumber, veto, prompt, NhitOD, N200OD, multiplicity;
  float Ed, dR, dT, Ep, xp, yp, zp, xd, yd, zd, rp, rd, Evis, vsTp, chiQp, chiTp, vsTd, chiQd, chiTd, totalQ17, totalQOD, VertexBadness;


  double R_lh_p, R_lh_d;
  int index=0;

  //  for(int i=11001; i <11500; i = i+500)
  for(int i=1; i <12000; i = i+500)

    {



      // -- set root file name
      char RootFile[256];


      sprintf(RootFile, "./result/DelayedCoincidence-and-Veto-Accidental-v3-%06d-%06d.root", i, i+499);
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
      b_Xp               -> SetAddress(&Xp);
      b_Yp               -> SetAddress(&Yp);
      b_Zp               -> SetAddress(&Zp);




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
	  b_Xp               -> GetEntry(i);
	  b_Yp               -> GetEntry(i);
	  b_Zp               -> GetEntry(i);


	  //	  cerr << run << " " << Ep << " " << dT << " " <<dR << " "  <<Ed << " " << deadtime_veto  << " " << NsumMax  << " " << N200OD  << " " << Rp  << " " << Rd  << " " << Xd  << " " << Yd  << " " << Zd  << " " << Xp  << " " << Yp  << " " << Zp << endl;

	  Rp = sqrt(Xp*Xp+Yp*Yp+Zp*Zp);

	  //	  if (! (Ep >= 7.5 && Ep <= 30))continue;
	  if (! (Ep >= 0.9 && Ep <= 30))continue;

	  if (run < 11000)continue;

	
	  
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

	
	//delayed energy cut 
	if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.4 && Ed < 5.6))) continue;

	


	//dead time cut
	if(deadtime_veto>0) continue;

	//NsumMax cut
	if(NsumMax < 200) continue;

      // spallation cut
	//	if (isVeto >= 1) continue;

	
	//N200OD cut
	if(N200OD > 5) continue;


	//fiducial volume cut XeLS cut

    R_lh_p = (Xp*Xp + Yp*Yp) * 1.e-4;
    R_lh_d = (Xd*Xd + Yd*Yd) * 1.e-4;
    Zp   *= 1.e-2; // cm -> m
    Zd   *= 1.e-2; // cm -> m
    
	  



#if 0 // prompt
      h -> Fill(R_lh_p, Zp);
#endif

#if 1 // delayed
      h -> Fill(R_lh_d, Zd);
#endif
 
      
      
      if (ln == 1)
	{
	  
#if 0 // prompt
    g_lh -> SetPoint(index, R_lh_p, Zp);
    h_lh -> Fill(R_lh_p, zp);
#endif
    
#if 1 // delayed
    g_lh -> SetPoint(index, R_lh_d, Zd);
    h_lh -> Fill(R_lh_d, zd);
#endif
    
    

    index++;	}	  
	}



    }








#if 0 // prompt
  h -> SetXTitle("x_{prompt}^{2}+y_{prompt}^{2} [m^{2}]");
  h -> SetYTitle("z_{prompt} [m]");
#endif

#if 1 // delayed
  h -> SetXTitle("x_{delayed}^{2}+y_{delayed}^{2} [m^{2}]");
  h -> SetYTitle("z_{delayed} [m]");
#endif
  
  cerr << h -> GetMaximum() << endl;
  cerr << h -> GetEntries() << endl;

  //  h -> SetMinimum(1.e+0);
  h -> SetMaximum(1.e+1);
  h -> Draw("colz");
  //  h -> Draw("scat");
  //  h2 -> Draw("P,same");


  c1 -> Update();

  // kamland balloon  
  //
  const Int_t n = 131;
  int index0 = 0;
  Double_t x1[n], z1[n],y1[n];
  for (Int_t i= 0; i<= 130; i++) {
    Int_t j = i - 65;
    z1[i] = j*0.1;
    if (z1[i] <= 5.4){
      z1[index0] = z1[i];
      x1[index0] = -z1[i]*z1[i] + 6.5*6.5;

      index0++;
    }
  }


  TGraph* graph = new TGraph(index0, x1, z1);
  graph -> SetLineWidth(2);
  graph -> SetLineColor(3);
  graph -> Draw("C");


  ifstream data2("./table/KamLANDBalloon.dat");
  const int ARRAY2 = 100;
  double xd2, yd2;
  double x0[ARRAY2], y0[ARRAY2];
  int index2 = 0;
  string line;
  while (!data2.eof()) {
    data2 >> xd2 >> yd2;
    x0[index2] = xd2;
    y0[index2] = yd2;
    index2++;
  }
  
  TGraph* graph2 = new TGraph(index2, x0, y0);
  graph2 -> SetLineWidth(2);
  graph2 -> SetLineColor(3);
  graph2 -> Draw("C, same");


  // 6.0m fiducial
  //
  const Int_t n2 = 121;
  int index1 = 0;
  Double_t x2[n2], z2[n2],y2[n2];
  for (Int_t i= 0; i<= 120; i++) {
    Int_t j = i - 60;
    z2[i] = j*0.1;
    z2[index1] = z2[i];
    x2[index1] = -z2[i]*z2[i] + 6.0*6.0;
    index1++;
  }

  TGraph* graph3 = new TGraph(index1, x2, z2);
  graph3 -> SetLineWidth(2);
  graph3 -> SetLineColor(3);
  graph3 -> Draw("C, same");


  // 2.5m fiducial
  //
  const Int_t n3 = 51;
  int index3 = 0;
  Double_t x3[n3], z3[n3],y3[n3];
  for (Int_t k= 0; k<= 50; k++) {
    Int_t p = k - 25;
    z3[k] = p*0.1;
    z3[index3] = z3[k];
    x3[index3] = -z3[k]*z3[k] + 2.5*2.5;
    index3++;
  }

  TGraph* graph4 = new TGraph(index3, x3, z3);
  graph4 -> SetLineWidth(2);
  graph4 -> SetLineColor(TColor::GetColor("#9900CC"));
  graph4 -> Draw("C, same");




  // likelihood selection
  //
  g_lh -> SetMarkerSize(0.5);
  g_lh -> SetMarkerStyle(20);
  g_lh -> SetMarkerColor(2);
  g_lh -> Draw("P,same");
    




  // cerr << N_dc << " " << N_lh << endl;


  c1 -> Print(ps_file);
  gSystem -> Exec(Form("gv  %s &", ps_file));

  
  return 0;
}



#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_check_vertex_distribution_accidental();
}
#endif
