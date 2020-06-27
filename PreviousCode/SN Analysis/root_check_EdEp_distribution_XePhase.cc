#ifndef __CINT__
#include "TROOT.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
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

Int_t root_check_EdEp_distribution_XePhase()
{
  // -------------------------------------------------------------------------------------------------------- //
  //  char delayed_coincidence_filename[256] = "./result/DelayedCoincidence_wo_Veto_Dthesis.root";
  //  char delayed_coincidence_filename[256] = "./result/DelayedCoincidence_Dthesis.root";
  //  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_2011Paper.root";
  //  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_2013-9626.root";
  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_XePhase_2013_11648.root";

  //  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_Dthesis.root";
  //  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_2011Paper.root";
  //  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_2011Paper.root";
  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_XePhase_2013_11648.root";

  //  char *ps_file = "./picture/picture_delayed_vertex.ps";
  //  char *ps_file = "./picture/picture_prompt_vertex.ps";
  char *ps_file = "./picture/picture_EdEp_2013_XePhase_narrow2.ps";
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


  // read Tree
  //
  // delayed coincidence
  int run, EventNumber, veto, prompt, NhitOD, N200OD, multiplicity;
  float Ed, dR, dT, Ep, xp, yp, zp, xd, yd, zd, rp, rd, Evis;
  float vsTp, chiQp, chiTp, vsTd, chiQd, chiTd, VertexBadness, totalQ17, totalQOD; 



  TFile *tfile = new TFile(delayed_coincidence_filename);
  TTree *nt_dc = (TTree*)tfile -> Get("nt");
  TBranch *b_run           = nt_dc -> GetBranch("run");
  TBranch *b_EventNumber   = nt_dc -> GetBranch("EventNumber");
  TBranch *b_veto          = nt_dc -> GetBranch("veto");
  TBranch *b_prompt        = nt_dc -> GetBranch("prompt");
  TBranch *b_Ed            = nt_dc -> GetBranch("Ed");
  TBranch *b_dR            = nt_dc -> GetBranch("dR");
  TBranch *b_dT            = nt_dc -> GetBranch("dT");
  TBranch *b_Ep            = nt_dc -> GetBranch("Ep");
  TBranch *b_xp            = nt_dc -> GetBranch("xp");
  TBranch *b_yp            = nt_dc -> GetBranch("yp");
  TBranch *b_zp            = nt_dc -> GetBranch("zp");
  TBranch *b_xd            = nt_dc -> GetBranch("xd");
  TBranch *b_yd            = nt_dc -> GetBranch("yd");
  TBranch *b_zd            = nt_dc -> GetBranch("zd");
  TBranch *b_rp            = nt_dc -> GetBranch("rp");
  TBranch *b_rd            = nt_dc -> GetBranch("rd");
  TBranch *b_Evis          = nt_dc -> GetBranch("Evis");
  TBranch *b_vsTp          = nt_dc -> GetBranch("vsTp");
  TBranch *b_chiQp         = nt_dc -> GetBranch("chiQp");
  TBranch *b_chiTp         = nt_dc -> GetBranch("chiTp");
  TBranch *b_vsTd          = nt_dc -> GetBranch("vsTd");
  TBranch *b_chiQd         = nt_dc -> GetBranch("chiQd");
  TBranch *b_chiTd         = nt_dc -> GetBranch("chiTd");
  TBranch *b_NhitOD        = nt_dc -> GetBranch("NhitOD");
  TBranch *b_N200OD        = nt_dc -> GetBranch("N200OD");
  TBranch *b_totalQ17      = nt_dc -> GetBranch("totalQ17");
  TBranch *b_totalQOD      = nt_dc -> GetBranch("totalQOD");
  TBranch *b_VertexBadness = nt_dc -> GetBranch("VertexBadness");
  TBranch *b_multiplicity  = nt_dc -> GetBranch("multiplicity");





  // draw histogram
  //
  const double XMIN = 0.9;
  //  const double XMAX = 10;
  const double XMAX = 7.5;
  const double YMIN = 1.8;
  const double YMAX = 3.0;
  const double BIN_WIDTHX = 0.05;
  const double BIN_WIDTHY = 0.05;
  int binX = (int)((XMAX - XMIN) / BIN_WIDTHX);
  int binY = (int)((YMAX - YMIN) / BIN_WIDTHY);

  TH2D* h = new TH2D("h", "", binX, XMIN, XMAX, binY, YMIN, YMAX);
 
  //  TGraph *h = new TGraph; 

  TGraph *g = new TGraph; //serection




  // events rejented by likelihood selection
  //
  b_Ep              -> SetAddress(&Ep);
  b_Ed              -> SetAddress(&Ed);
  b_dR              -> SetAddress(&dR);
  b_dT              -> SetAddress(&dT);
  b_veto            -> SetAddress(&veto);
  b_N200OD          -> SetAddress(&N200OD);
  b_multiplicity    -> SetAddress(&multiplicity);
  b_rp              -> SetAddress(&rp);
  b_rd              -> SetAddress(&rd);

  b_xp              -> SetAddress(&xp);
  b_yp              -> SetAddress(&yp);
  b_zp              -> SetAddress(&zp);
  b_xd              -> SetAddress(&xd);
  b_yd              -> SetAddress(&yd);
  b_zd              -> SetAddress(&zd);
  b_EventNumber     -> SetAddress(&EventNumber);
  b_run             -> SetAddress(&run);



  int index = 0;
  int N_dc = nt_dc -> GetEntries();
  for (int i = 0 ; i < N_dc ; i++) {

    b_Ep           -> GetEntry(i);
    b_Ed           -> GetEntry(i);
    b_dR           -> GetEntry(i);
    b_dT           -> GetEntry(i);
    b_veto         -> GetEntry(i);
    b_N200OD       -> GetEntry(i);
    b_multiplicity -> GetEntry(i);
    b_rp           -> GetEntry(i);
    b_rd           -> GetEntry(i);
    
    b_xp           -> GetEntry(i);
    b_yp           -> GetEntry(i);
    b_zp           -> GetEntry(i);
    b_xd           -> GetEntry(i);
    b_yd           -> GetEntry(i);
    b_zd           -> GetEntry(i);
    b_EventNumber  -> GetEntry(i);
    b_run          -> GetEntry(i);

    

    // space correlation cut
    if (dR >= 200) continue;

    // delayed energy cut
    //    if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.4 && Ed < 5.6))) continue;
    if ( Ed > 6.0)continue;

    // spallation cut
    if (veto >= 1) continue;

    /// ---- reactor neutrino selection ---- ///
    //    if (Ep >= 0.9 && Ep < 8.5 && N200OD < 5 && multiplicity <= 1) {
    //    if (Ep >= 7.5 && Ep < 30 && N200OD < 5 && multiplicity <= 1) { //130618
    if (Ep < 35 && N200OD < 5 && multiplicity <= 1) { //130618
 
      //fiducial volume cut & XeLS cut
      if (rp < 600.0 && rd < 600.0 && rd > 250) {


	// delayed; cylinder cut
	if ((zd > 0) && (sqrt(xd*xd+yd*yd) < 250)) continue;



		h -> Fill( Ep, Ed);

	//	h -> SetPoint(index, Ep, Ed);

	if (7.5 <= Ep && Ep <= 30)
	  {
	    cerr << " 1 " << endl;
	  }
      }
    }

    if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.4 && Ed < 5.6))) continue;

    /// ---- reactor neutrino selection ---- ///
    //    if (Ep >= 0.9 && Ep < 8.5 && N200OD < 5 && multiplicity <= 1) {
    if (Ep >= 7.5 && Ep < 30 && N200OD < 5 && multiplicity <= 1) { //130618
 
      //fiducial volume cut & XeLS cut
      if (rp < 600.0 && rd < 600.0 && rd > 250) {


	// delayed; cylinder cut
	if ((zd > 0) && (sqrt(xd*xd+yd*yd) < 250)) continue;

	g -> SetPoint(index, Ep, Ed);
	cerr << "Ed= " << Ed << " Ep= " << Ep << endl;       

      }
    }
    

 
    index++;}
#if 0
  TH1F *frame2 = gPad -> DrawFrame(0, 0, 35, 6);
    //    char title[10];
    //   sprintf(title, "source ID = %02d", i);
    //   frame -> SetTitle(title);
    
    frame2 -> SetXTitle("E_{prompt} [MeV]");
    frame2 -> SetYTitle("E_{delayed} [m]");
#endif

#if 1 
  h -> SetXTitle("E_{prompt} [MeV]");
  h -> SetYTitle("E_{delayed} [m]");
#endif


  
  //  cerr << h -> GetMaximum() << endl;
  //  cerr << h -> GetEntries() << endl;

  for (int o=0; o < binX; o++)
    {
      for (int j=0; j < binY; j++)
	{

	double A =  h -> GetBinContent(o, j);
	h ->SetBinContent(o, j , A/1.2);

	}
    }


      h -> SetMinimum(1.e-2);
      h -> SetMaximum(1.e+2);
  //      h -> SetMarkerSize(1.0);
  //      h -> SetMarkerStyle(20);
  //      h -> SetMarkerColor(4);
    h -> Draw("colz");
    //  h -> Draw("P");
  //    h -> Draw("scat");
  //  h2 -> Draw("P,same");


  c1 -> Update();


  /*
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

  */

  // likelihood selection
  //
  g -> SetMarkerSize(1.0);
  g -> SetMarkerStyle(20);
  g -> SetMarkerColor(2);
  //  g -> Draw("P,same");
    




  cerr << N_dc  << endl;


  c1 -> Print(ps_file);
  gSystem -> Exec(Form("gv  %s &", ps_file));

  
  return 0;
}



#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_check_EdEp_distribution_XePhase();
}
#endif
