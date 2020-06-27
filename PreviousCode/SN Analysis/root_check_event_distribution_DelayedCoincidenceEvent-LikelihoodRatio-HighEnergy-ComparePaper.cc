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
using namespace std;
#endif

Int_t root_check_event_distribution_DelayedCoincidenceEvent_LikelihoodRatio_HighEnergy()
{
  // -------------------------------------------------------------------------------------------------------- //



#if 0
  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_2011Paper.root";
  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_2011Paper.root";
  char *ps_file = "./picture/picture_sample_of_anti_neutrino_Compare_2011Paper.ps";
#endif

#if 0
  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_2013-9626.root";
  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_2013-9626.root";
  char *ps_file = "./picture/picture_sample_of_anti_neutrino_2013_9626_Compare_2011Paper.ps";
#endif


#if 0
  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_vertex_2013_9626.root";
  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_vertex_2013_9626.root";
  char *ps_file = "./picture/picture_sample_of_anti_neutrino_vertex_2013_9626_Compare_2011Paper.ps";


#endif



#if 0
  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_shimizu_2011Paper.root";
  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_shimizu_2011Paper.root";
  char *ps_file = "./picture/picture_sample_of_anti_neutrino_shimizu_2011Paper_Compare_2011Paper.ps";
#endif

#if 1
  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_shimizu_2013_9626.root";
  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_shimizu_2013_9626.root";
  char *ps_file = "./picture/picture_sample_of_anti_neutrino_shimizu_2011Paper_Compare_2013_9626.ps";

  string ofs_file = "./table/candidate-9626.dat"; 
  const double XMIN1 = 7.5; 
  const double energyKat = 7.5; 

#endif




#if 0
  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_shimizu_2013-11947.root";
  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_shimizu_2013-11947.root";
  char *ps_file = "./picture/picture_sample_of_anti_neutrino_shimizu_2011Paper_Compare_2013-11947.ps";

  string ofs_file = "./table/candidate-all.dat"; 
  const double XMIN1 = 7.5; 
  const double energyKat = 7.5; 


#endif


#if 0
  char delayed_coincidence_filename[256]                = "./result/DelayedCoincidence_shimizu_2013-11000-11947.root";
  char delayed_coincidence_and_likelihood_filename[256] = "./result/DelayedCoincidence_and_LikelihoodSelection_shimizu_2013-11000-11947.root";
  char *ps_file = "./picture/picture_sample_of_anti_neutrino_shimizu_2011Paper_Compare_2013-11000-11947.ps";

  string ofs_file = "./table/candidate-DS3.dat"; 
  const double XMIN1 = 0.5; 
  const double energyKat = 0.9; 
#endif


  // -------------------------------------------------------------------------------------------------------- //




  //----------Output-----------//
  ofstream ofs(ofs_file.c_str());




  

  
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
  c1 -> Divide(2,2);

  // make picture
  int type = 112;
  TPostScript *ps = new TPostScript(ps_file, type);
  ps -> Range(25, 12);



  
  // read Tree
  //
  // delayed coincidence
  int run, EventNumber, veto, prompt, NhitOD, N200OD, multiplicity;
  float Ed, dR, dT, Ep, xp, yp, zp, xd, yd, zd, rp, rd, Evis, vsTp, chiQp, chiTp, vsTd, chiQd, chiTd, totalQ17, totalQOD, VertexBadness;


  
  //  float xp, yp, zp, xd, yd, zd, rp, rd;
  //  float dR, Ed, dT, Ep, Evis;

   //  float vsTp, chiQp, chiTp, vsTd, chiQd, chiTd, VertexBadness, totalQ17, totalQOD; 

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
  // veto = 0 -> with spallation cut
  // set address
  b_Ep              -> SetAddress(&Ep);
  b_Ep_lh           -> SetAddress(&Ep);
  b_Ed              -> SetAddress(&Ed);
  b_Ed_lh           -> SetAddress(&Ed);
  b_dR              -> SetAddress(&dR);
  b_dR_lh           -> SetAddress(&dR);
  b_dT              -> SetAddress(&dT);
  b_dT_lh           -> SetAddress(&dT);
  b_veto            -> SetAddress(&veto);
  b_N200OD          -> SetAddress(&N200OD);
  b_multiplicity    -> SetAddress(&multiplicity);
  b_rp              -> SetAddress(&rp);
  b_rd              -> SetAddress(&rd);
  b_run             -> SetAddress(&run);
  b_run_lh             -> SetAddress(&run);


  // 1. prompt energy
  //
  c1 -> cd(1);
  //  gPad -> SetLogy();
  gPad -> SetBottomMargin(0.12);

  //  const double XMIN1 = 0.5; 
  const double XMAX1 = 30;
  //  const double BIN_WIDTH1 = 0.5;
  const double BIN_WIDTH1 = 0.5;
  int bin1 = (int)((XMAX1 - XMIN1) / BIN_WIDTH1);
  TH1F* h1 = new TH1F("Ep", "",  bin1, XMIN1, XMAX1);
  TH1F* h2 = new TH1F("Ep_lh", "",  bin1, XMIN1, XMAX1);


  const double XMIN1_ln2 = 0;
  const double XMAX1_ln2 = 30;
  const double BIN_WIDTH1_ln2 = 1.0;//1Mev bin
  int bin1_ln2 = (int)((XMAX1_ln2 - XMIN1_ln2) / BIN_WIDTH1_ln2);


  TH1F* h2_ln2 = new TH1F("h2_ln2", "",  bin1_ln2, XMIN1_ln2, XMAX1_ln2);

 
#if 0
 
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
    b_run          -> GetEntry(i);


    if (run == 5380)continue;
    //    if (run == 1125 ||  run == 1287 || run == 3079 || run == 6723 || run == 8253 || run == 9236 || run == 5380)continue;

    // space and time correlation cut
    if (dR >= 200 || dT < 0.5 || dT >= 1000) continue;

    // delayed energy cut
    if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.4 && Ed < 5.6))) continue;
    //    if (!(Ed >= 1.8 && Ed < 2.6)) continue;

    // spallation cut
    if (veto >= 1) continue;
    
    /// ---- reactor neutrino selection ---- ///
    //    if (Ep >= 0.9 && Ep < 8.5 && N200OD < 5 && multiplicity <= 1) {
    //    if (N200OD < 5 && multiplicity <= 1) {
    if (N200OD < 5 ) {
      
      //fiducial volume cut & cylinder cut
      if (rp < 600.0 && rd < 600.0) {
       h1 -> Fill(Ep);
      }
    }
  }

#endif
  
  int N_lh = nt_lh -> GetEntries();
  for (int i = 0 ; i < N_lh ; i++)
 {
   b_Ep_lh  -> GetEntry(i);
   b_run_lh -> GetEntry(i);


   if (!(energyKat <= Ep && Ep <= 30))continue;   
   
    if (run == 5380)continue;

   //   if (run == 1125 ||  run == 1287 || run == 3079 || run == 6723 || run == 8253 || run == 9236 || run == 5380)continue;


   h2 -> Fill(Ep);
   
   //  if (!(0.9 <= Ep && Ep <= 30))continue;

   h2_ln2 -> Fill(Ep);
   
  }


  double y_min1 = h1 -> GetMinimum();
  double y_max1 = h1 -> GetMaximum();
  
  //  h1 -> SetMinimum(1.0);
  //  h1 -> SetMaximum(y_max1*1.7);

  h2 -> SetXTitle("Prompt Energy [MeV]");
  h2 -> SetYTitle("Events/0.5MeV");
  h2 -> Sumw2();
  h2 -> SetLineWidth(3);
  h2 -> SetLineColor(4);
  //  h2 -> SetFillColor(TColor::GetColor("#33ccff")); 
  //  h1 -> Draw();
  h2 -> Draw();
  
  c1 -> Update();


#if 0
    TGaxis* gaxis_1[2];
    gaxis_1[0] = new TGaxis(XMIN1, y_min1, XMAX1, y_min1, XMIN1, XMAX1, 510,"");
    gaxis_1[1] = new TGaxis(XMIN1, y_min1, XMIN1, y_max1, y_min1 , y_max1*1.7, 510,"G");




    for (int i = 0 ; i < 2 ; i++) {
      gaxis_1[i] -> SetLabelSize(0);
      //      gaxis_1[i] -> Draw();
    }

#endif

  // 2. delayed energy
  //
  c1 -> cd(2);
  //  gPad -> SetLogy();
  gPad -> SetBottomMargin(0.12);

  //  const double XMIN2 = 0;
  const double XMIN2 = 1.4;
  //  const double XMAX2 = 6;
  const double XMAX2 = 3;
  const double BIN_WIDTH2 = 0.1;
  int bin2 = (int)((XMAX2 - XMIN2) / BIN_WIDTH2);
  TH1F* h3 = new TH1F("Ed", "",  bin2, XMIN2, XMAX2);
  TH1F* h4 = new TH1F("Ed_lh", "",  bin2, XMIN2, XMAX2);
#if 0  

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
    b_run          -> GetEntry(i);

    if (run == 5380)continue;
    //    if (run == 1125 || run == 1287 || run == 3079 || run == 6723 || run == 8253 || run == 9236 || run == 5380)continue;

    // space and time correlation cut
    if (dR >= 200 || dT < 0.5 || dT >= 1000) continue;
    // spallation cut
    if (veto >= 1) continue;
    
    /// ---- reactor neutrino selection ---- ///
    //    if (Ep >= 0.9 && Ep < 8.5 && N200OD < 5 && multiplicity <= 1) {
    //    if (Ep >= 7.5 && Ep < 30 && N200OD < 5 && multiplicity <= 1) {
    //    if (Ep >= energyKat && Ep < 30 && N200OD < 5 ) {
   if (Ep >= energyKat && Ep < 30 && N200OD < 5 ) {
      
      //fiducial volume cut & cylinder cut
      if (rp < 600.0 && rd < 600.0) {
	h3 -> Fill(Ed);
      }
    }
  }
  
#endif

  for (int i = 0 ; i < N_lh ; i++) {
    b_Ed_lh   -> GetEntry(i);
    b_Ep_lh  -> GetEntry(i);
    b_run_lh  -> GetEntry(i);

   if (!(energyKat <= Ep && Ep <= 30))continue;   

    if (run == 5380)continue;
    //   if (run == 1125 ||  run == 1287 || run == 3079 || run == 6723 || run == 8253 || run == 9236 || run == 5380)continue;
    h4 -> Fill(Ed);
  }


  h4 -> SetXTitle("Delayed Energy [MeV]");
  h4 -> SetYTitle("Events/0.1MeV");
  h4 -> Sumw2();
  //  h3 -> SetMinimum(1.e-1);
  h4 -> SetLineWidth(3);
  h4 -> SetLineColor(4);
  //  h4 -> SetFillColor(TColor::GetColor("#33ccff")); 
  h4 -> Draw();
  //  h4 -> Draw("same");
  c1 -> Update();

  // select condition
  //  double ymin_E = 1.e-1;
  double ymin_E = 0;
  //  double ymax_E = (h3 -> GetMaximum()) * 1.6;
  double ymax_E = h4 -> GetMaximum();
  TLine *l_E[4];
  l_E[0] = new TLine(1.8, ymin_E, 1.8, ymax_E);
  l_E[1] = new TLine(2.6, ymin_E, 2.6, ymax_E);
  l_E[2] = new TLine(4.4, ymin_E, 4.4, ymax_E);
  l_E[3] = new TLine(5.6, ymin_E, 5.6, ymax_E);
  for (int l = 0 ; l < 4 ; l++) {
    l_E[l] -> SetLineWidth(2);
    l_E[l] -> SetLineStyle(2);
    l_E[l] -> SetLineColor(TColor::GetColor("#ff0099"));
    l_E[l] -> Draw();
  }
  c1 -> Update();


  double y_min2 = h3 -> GetMinimum();
  double y_max2 = h3 -> GetMaximum();
  
  TGaxis* gaxis_2[2];
  gaxis_2[0] = new TGaxis(XMIN2, y_min2, XMAX2, y_min2, XMIN2, XMAX2, 510,"");
  gaxis_2[1] = new TGaxis(XMIN2, y_min2, XMIN2, y_max2, y_min2, y_max2, 510,"G");

  for (int i = 0 ; i < 2 ; i++) {
    gaxis_2[i] -> SetLabelSize(0);
    gaxis_2[i] -> Draw();
  }



  // 3. space corillation
  //
  c1 -> cd(3);
  //  gPad -> SetLogy();
  gPad -> SetBottomMargin(0.12);

  const double XMIN3 = 0.000;
  //  const double XMAX3 = 300;
  const double XMAX3 = 200;
  const double BIN_WIDTH3 = 10;
  int bin3 = (int)((XMAX3 - XMIN3) / BIN_WIDTH3);
  TH1F* h5 = new TH1F("dR", "",  bin3, XMIN3, XMAX3);
  TH1F* h6 = new TH1F("dR_lh", "",  bin3, XMIN3, XMAX3);


#if 0  

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
    b_run          -> GetEntry(i);

    if (run == 5380)continue;

    //    if (run == 1125 || run == 1287 || run == 3079 || run == 6723 || run == 8253 || run == 9236 || run == 5380)continue;

    // time correlation cut
    if (dT < 0.5 || dT >= 1000) continue;
    // delayed energy cut
    //if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.0 && Ed < 5.8))) continue;
    if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.4 && Ed < 5.6))) continue;
    // spallation cut
    if (veto >= 1) continue;
    
    /// ---- reactor neutrino selection ---- ///
    //    if (Ep >= 0.9 && Ep < 8.5 && N200OD < 5 && multiplicity <= 1) {
    //          if (Ep >= 7.5 && Ep < 30 && N200OD < 5 && multiplicity <= 1) {
    //          if (Ep >= 7.5 && Ep < 30 && N200OD < 5 ) {
   if (Ep >= energyKat && Ep < 30 && N200OD < 5 ) {


      //fiducial volume cut & cylinder cut
      if (rp < 600.0 && rd < 600.0) {
	h5 -> Fill(dR);
	//	if (30 <= dR && dR <= 50)
	//	  {cerr << run << endl;}

      }
    }
  }
#endif
  

  for (int i = 0 ; i < N_lh ; i++) {
    b_Ep_lh  -> GetEntry(i);
    b_dR_lh   -> GetEntry(i);
    b_run_lh  -> GetEntry(i);

   if (!(energyKat <= Ep && Ep <= 30))continue;   

    if (run == 5380)continue;
    //   if (run == 1125 ||  run == 1287 || run == 3079 || run == 6723 || run == 8253 || run == 9236 || run == 5380)continue;
    h6 -> Fill(dR);
  }

  h6 -> SetXTitle("#DeltaR [cm]");
  h6 -> SetYTitle("Events/10cm");
  h6 -> Sumw2();
  //  h5 -> SetMinimum(1.e-1);
  h6 -> SetLineWidth(3);
  h6 -> SetLineColor(4);
  //  h6 -> SetFillColor(TColor::GetColor("#33ccff")); 
  h6 -> Draw();
  //  h6 -> Draw("same");

  // select condition
  //  double ymin_R = 1.e-1;
  double ymin_R = 0;
  //  double ymax_R = (h5 -> GetMaximum()) * 1.6;
  double ymax_R = h6 -> GetMaximum();
  TLine *l_R[1];
  l_R[0] = new TLine(200, ymin_R, 200, ymax_R);
  for (int l = 0 ; l < 1 ; l++) {
    l_R[l] -> SetLineWidth(2);
    l_R[l] -> SetLineStyle(2);
    l_R[l] -> SetLineColor(TColor::GetColor("#ff0099"));
    l_R[l] -> Draw();
  }
  c1 -> Update();


  double y_min3 = h5 -> GetMinimum();
  double y_max3 = h5 -> GetMaximum();
  
  TGaxis* gaxis_3[2];
  gaxis_3[0] = new TGaxis(XMIN3, y_min3, XMAX3, y_min3, XMIN3, XMAX3, 510,"");
  gaxis_3[1] = new TGaxis(XMIN3, y_min3, XMIN3, y_max3, y_min3, y_max3, 510,"G");

  for (int i = 0 ; i < 2 ; i++) {
    gaxis_3[i] -> SetLabelSize(0);
    gaxis_3[i] -> Draw();
  }

  

  // 4. time corillation
  c1 -> cd(4);
  //  gPad -> SetLogy();
  gPad -> SetBottomMargin(0.12);

  const double XMIN4 = 0.000;
  const double XMAX4 = 2000;
  const double BIN_WIDTH4 = 100;
  int bin4 = (int)((XMAX4 - XMIN4) / BIN_WIDTH4);
  TH1F* h7 = new TH1F("dT", "",  bin4, XMIN4, XMAX4);
  TH1F* h8 = new TH1F("dT_lh", "",  bin4, XMIN4, XMAX4);
  
  #if 0 
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
    b_run          -> GetEntry(i);

    //    if (run == 1125 || run == 1287 || run == 3079 || run == 6723 || run == 8253 || run == 9236 || run == 5380)continue;

    if (run == 5380)continue;


    // space correlation cut
    if (dR >= 200) continue;
    // delayed energy cut
    //if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.0 && Ed < 5.8))) continue;
    if (!((Ed >= 1.8 && Ed < 2.6) || (Ed >= 4.4 && Ed < 5.6))) continue;
    // spallation cut
    if (veto >= 1) continue;
    
    /// ---- reactor neutrino selection ---- ///
    //    if (Ep >= 0.9 && Ep < 8.5 && N200OD < 5 && multiplicity <= 1) {
    //          if (Ep >= 7.5 && Ep < 30 && N200OD < 5 && multiplicity <= 1) {
    //         if (Ep >= 7.5 && Ep < 30 && N200OD < 5 ) {

   if (Ep >= energyKat && Ep < 30 && N200OD < 5 ) {

      //fiducial volume cut & cylinder cut
      if (rp < 600.0 && rd < 600.0) {
	h7 -> Fill(dT);
      }
    }
  }
#endif

  for (int i = 0 ; i < N_lh ; i++) {
    b_Ep_lh  -> GetEntry(i);
    b_dT_lh   -> GetEntry(i);
    b_run_lh  -> GetEntry(i);

   if (!(energyKat <= Ep && Ep <= 30))continue;   

    if (run == 5380)continue;
    //   if (run == 1125 ||  run == 1287 || run == 3079 || run == 6723 || run == 8253 || run == 9236 || run == 5380)continue;
    h8 -> Fill(dT);
  }

  h8 -> SetXTitle("#DeltaT [#musec]");
  h8 -> SetYTitle("Events/100#musec");
  h8 -> Sumw2();
  //  h7 -> SetMinimum(1.e-1);
  h8 -> SetLineWidth(3);
  h8 -> SetLineColor(4);
  //  h8 -> SetFillColor(TColor::GetColor("#33ccff")); 
  h8 -> Draw();
  //  h8 -> Draw("same");

  cout << "all event : " << h7 -> GetEntries() <<endl
       << "candidate : " << h8 -> GetEntries() <<endl;

  // select condition
  //  double ymin_T = 1.e-1;
  double ymin_T = 0;
  //  double ymax_T = (h7 -> GetMaximum()) * 1.6;
  double ymax_T = h8 -> GetMaximum();
  TLine *l_T[1];
  l_T[0] = new TLine(1000, ymin_T, 1000, ymax_T);
  for (int l = 0 ; l < 1 ; l++) {
    l_T[l] -> SetLineWidth(2);
    l_T[l] -> SetLineStyle(2);
    l_T[l] -> SetLineColor(TColor::GetColor("#ff0099"));
    l_T[l] -> Draw();
  }
  c1 -> Update();

  double y_min4 = h8 -> GetMinimum();
  double y_max4 = h8 -> GetMaximum();
  
    TGaxis* gaxis_4[2];
    gaxis_4[0] = new TGaxis(XMIN4, y_min4, XMAX4, y_min4, XMIN4, XMAX4, 510,"");
    gaxis_4[1] = new TGaxis(XMIN4, y_min4, XMIN4, y_max4, y_min4, y_max4, 510,"G");

    for (int i = 0 ; i < 2 ; i++) {
      gaxis_4[i] -> SetLabelSize(0);
      gaxis_4[i] -> Draw();
    }
  

    // write down file 


    int ARRAY = 2000;
    double Ep_ln[ARRAY];
    int Ep_bin[ARRAY];
    double N[ARRAY];

    int m = 0;

  for (int q = 0; q < 30; q++)
    {

      Ep_ln[q] = 0.5 + q;

      Ep_bin[q] = h2_ln2 -> FindBin(Ep_ln[q]);

      N[q] = h2_ln2 -> GetBinContent(Ep_bin[q]);

      ofs << Ep_ln[q] << " " << N[q] << endl;

      m++;}














  c1 -> Print(ps_file);
  // gSystem -> Exec(Form("gv -scalebase 2 %s &", ps_file));


  return 0;
}



#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_check_event_distribution_DelayedCoincidenceEvent_LikelihoodRatio_HighEnergy();
}
#endif
