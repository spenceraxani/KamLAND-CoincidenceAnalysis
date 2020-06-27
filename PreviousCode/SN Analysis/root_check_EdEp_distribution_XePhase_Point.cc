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

Int_t root_check_vertex_distribution_XePhase()
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
  char *ps_file = "./picture/picture_EdEp_XePhase_2013_11648.ps";
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


  // delayed coincidence & likelihood selection
  TFile *tfile_lh = new TFile(delayed_coincidence_and_likelihood_filename);
  //TTree *nt_lh = (TTree*)tfile_lh -> Get("nt");
  TTree *nt_lh = (TTree*)tfile_lh -> Get("nt_ls");
  TBranch *b_run_lh           = nt_lh -> GetBranch("run");
  TBranch *b_EventNumber_lh   = nt_lh -> GetBranch("EventNumber");
  TBranch *b_veto_lh          = nt_lh -> GetBranch("veto");
  TBranch *b_prompt_lh        = nt_lh -> GetBranch("prompt");
  TBranch *b_Ed_lh            = nt_lh -> GetBranch("Ed");
  TBranch *b_dR_lh            = nt_lh -> GetBranch("dR");
  TBranch *b_dT_lh            = nt_lh -> GetBranch("dT");
  TBranch *b_Ep_lh            = nt_lh -> GetBranch("Ep");
  TBranch *b_xp_lh            = nt_lh -> GetBranch("xp");
  TBranch *b_yp_lh            = nt_lh -> GetBranch("yp");
  TBranch *b_zp_lh            = nt_lh -> GetBranch("zp");
  TBranch *b_xd_lh            = nt_lh -> GetBranch("xd");
  TBranch *b_yd_lh            = nt_lh -> GetBranch("yd");
  TBranch *b_zd_lh            = nt_lh -> GetBranch("zd");
  TBranch *b_rp_lh            = nt_lh -> GetBranch("rp");
  TBranch *b_rd_lh            = nt_lh -> GetBranch("rd");
  TBranch *b_Evis_lh          = nt_lh -> GetBranch("Evis");
  TBranch *b_vsTp_lh          = nt_lh -> GetBranch("vsTp");
  TBranch *b_chiQp_lh         = nt_lh -> GetBranch("chiQp");
  TBranch *b_chiTp_lh         = nt_lh -> GetBranch("chiTp");
  TBranch *b_vsTd_lh          = nt_lh -> GetBranch("vsTd");
  TBranch *b_chiQd_lh         = nt_lh -> GetBranch("chiQd");
  TBranch *b_chiTd_lh         = nt_lh -> GetBranch("chiTd");
  TBranch *b_NhitOD_lh        = nt_lh -> GetBranch("NhitOD");
  TBranch *b_N200OD_lh        = nt_lh -> GetBranch("N200OD");
  TBranch *b_totalQ17_lh      = nt_lh -> GetBranch("totalQ17");
  TBranch *b_totalQOD_lh      = nt_lh -> GetBranch("totalQOD");
  TBranch *b_VertexBadness_lh = nt_lh -> GetBranch("VertexBadness");
  TBranch *b_multiplicity_lh  = nt_lh -> GetBranch("multiplicity");






  // draw histogram
  //
  const double XMIN = 0.1;
  const double XMAX = 35.0;
  const double YMIN = 0.1;
  const double YMAX = 6.0;
  const double BIN_WIDTHX = 0.3;
  const double BIN_WIDTHY = 0.15;
  int binX = (int)((XMAX - XMIN) / BIN_WIDTHX);
  int binY = (int)((YMAX - YMIN) / BIN_WIDTHY);

  TH2D* h = new TH2D("h", "", binX, XMIN, XMAX, binY, YMIN, YMAX);
  TH2D* h_lh = new TH2D("h_lh", "", binX, XMIN, XMAX, binY, YMIN, YMAX);
  TGraph *g_lh = new TGraph;

  // likelihood selection
  //
  b_xp_lh          -> SetAddress(&xp);
  b_yp_lh          -> SetAddress(&yp);
  b_zp_lh          -> SetAddress(&zp);
  b_xd_lh          -> SetAddress(&xd);
  b_yd_lh          -> SetAddress(&yd);
  b_zd_lh          -> SetAddress(&zd);
  b_EventNumber_lh -> SetAddress(&EventNumber);
  b_run_lh         -> SetAddress(&run);




  int N_lh = nt_lh -> GetEntries();
  
  double R_lh_p, R_lh_d;
  int index = 0;
  const int ARRAY = 3000;
  int EventNumber_lh[ARRAY];
  int RunNumber_lh[ARRAY];
  for (int i = 0 ; i < ARRAY ; i++) {
    EventNumber_lh[i] = 0;
    RunNumber_lh[i] = 0;
  }

  for (int i = 0 ; i < N_lh ; i++) {
    b_xp_lh          -> GetEntry(i);
    b_yp_lh          -> GetEntry(i);
    b_zp_lh          -> GetEntry(i);
    b_xd_lh          -> GetEntry(i);
    b_yd_lh          -> GetEntry(i);
    b_zd_lh          -> GetEntry(i);
    b_EventNumber_lh -> GetEntry(i);
    b_run_lh         -> GetEntry(i);

    R_lh_p = (xp*xp + yp*yp) * 1.e-4;
    R_lh_d = (xd*xd + yd*yd) * 1.e-4;
    zp   *= 1.e-2; // cm -> m
    zd   *= 1.e-2; // cm -> m
    
    EventNumber_lh[index] = EventNumber;
    RunNumber_lh[index] = int(run);
    
#if 1 // prompt
    g_lh -> SetPoint(index, R_lh_p, zp);
    h_lh -> Fill(R_lh_p, zp);
#endif
    
#if 0 // delayed
    g_lh -> SetPoint(index, R_lh_d, zd);
    h_lh -> Fill(R_lh_d, zd);
#endif
    
    
    index++;
  }

  cerr << index << endl;

  for (int j = 0 ; j < index ; j++) {
    cerr << EventNumber_lh[j] << " " << RunNumber_lh[j] << endl;
  }


  //  return 0;
  
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


  double R_p, R_d;
  int lh_f = 0;
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

    
    R_p = double((xp*xp + yp*yp) * 1.e-4);
    R_d = double((xd*xd + yd*yd) * 1.e-4);
    zp  = double(zp * 1.e-2); // cm -> m
    zd  = double(zd * 1.e-2); // cm -> m
  

    // space correlation cut
    if (dR >= 200) continue;

    // delayed energy cut
    if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.4 && Ed < 5.6))) continue;

    // spallation cut
    if (veto >= 1) continue;


    
    //    if (lh_f == 1) cerr << EventNumber << " " << run << " " << lh_f << endl;


    /// ---- reactor neutrino selection ---- ///
    //    if (Ep >= 0.9 && Ep < 8.5 && N200OD < 5 && multiplicity <= 1) {
    if (Ep >= 7.5 && Ep < 30 && N200OD < 5 && multiplicity <= 1) { //130618
 
      //fiducial volume cut & XeLS cut
      if (rp < 600.0 && rd < 600.0 && rd > 250) {


	// delayed; cylinder cut
	if ((zd > 0) && (sqrt(xd*xd+yd*yd) < 250)) continue;




   
       // wo likelihood selection events
       for (int j = 0 ; j < index ; j++) {
 	if(EventNumber == EventNumber_lh[j] && run == RunNumber_lh[j]) {
 	  lh_f = 1;
 	  break;
 	}
       }
      
       if (lh_f != 1) {
	 
#if 1 // prompt
	 h -> Fill(R_p, zp);
#endif

#if 0 // delayed
	 h -> Fill(R_d, zd);
#endif

       }
      }

	
    }

      //fiducial volume cut & cylinder cut
      //      if (rp < 600.0 && rd < 600.0) {
      //      h -> Fill(R, zp);
      //      }

    lh_f = 0;
    
  }

#if 1 // prompt
  h -> SetXTitle("x_{prompt}^{2}+y_{prompt}^{2} [m^{2}]");
  h -> SetYTitle("z_{prompt} [m]");
#endif

#if 0 // delayed
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



  // likelihood selection
  //
  g_lh -> SetMarkerSize(0.5);
  g_lh -> SetMarkerStyle(20);
  g_lh -> SetMarkerColor(2);
  g_lh -> Draw("P,same");
    




  cerr << N_dc << " " << N_lh << endl;


  c1 -> Print(ps_file);
  gSystem -> Exec(Form("gv  %s &", ps_file));

  
  return 0;
}



#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_check_vertex_distribution_XePhase();
}
#endif
