#ifndef __CINT__
#include "TROOT.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TF1.h"
#include "TCut.h"
#include "TLine.h"
#include "TLatex.h"
#include "TGraphErrors.h"
#include "TPostScript.h"
#include "TMath.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;
#endif

Int_t root_draw_8He9Li_time_correlation()
{
  
  // ----------------------------------------------------------------------------------- //

#if 0
  char input_filename[256]  = "./result/DelayedCoincidence-Muon_KamLAND_DS2.root";
  char output_filename[256] = "./result/8He9Li_spectrum_KamLAND_DS2.dat";
  char *ps_file = "./picture/picture_8He9Li_time_correlation_KamLAND_DS2.ps";
#endif


#if 1
  char input_filename[256]  = "./result/DelayedCoincidence-Muon.root";
  char output_filename[256] = "./result/8He9Li_spectrum.dat";
  char *ps_file = "./picture/picture_8He9Li_time_correlation.ps";
#endif


  // ----------------------------------------------------------------------------------- //
  
  
  // set canvas style
  //
  gROOT  -> SetStyle("Plain");
  gStyle -> SetStatFont(42);
  gStyle -> SetOptStat(0);
  gStyle -> SetOptFit(1);
  gStyle -> SetLabelFont(42,"XY");
  gStyle -> SetLabelSize(0.054, "XY");
  gStyle -> SetLabelOffset(0.007, "X");
  gStyle -> SetLabelOffset(0.008, "Y");
  gStyle -> SetTitleFont(42, "XY");
  gStyle -> SetTitleSize(0.055, "XY");
  gStyle -> SetTitleOffset(0.86, "X");
  gStyle -> SetTitleOffset(1.02, "Y");

  TCanvas *c1 = new TCanvas("c1", "Title", 0, 0, 1100, 750);
  c1 -> SetFillStyle(4001);
  c1 -> SetTopMargin(0.15);
  c1 -> SetBottomMargin(0.15);
  c1 -> SetLeftMargin(0.12);
  c1 -> Divide(2,2);

  // make picture
  int type = 112;
  TPostScript *ps = new TPostScript(ps_file, type);
  ps -> Range(25, 12);
  
  // read Tree
  //
  //  int run;
  float run;
  float Ed, dR, dT, Ep, Q, dQ, rp, rd;
  float dL, dTM, badness;
  float xd, yd, zd, ln;
  
  TFile *tfile = new TFile(input_filename);
  TTree *nt_heli = (TTree*)tfile -> Get("nt");
  TBranch *b_run           = nt_heli -> GetBranch("run");
  TBranch *b_Ed            = nt_heli -> GetBranch("Ed");
  TBranch *b_dR            = nt_heli -> GetBranch("dR");
  TBranch *b_dT            = nt_heli -> GetBranch("dT");
  TBranch *b_Ep            = nt_heli -> GetBranch("Ep");
  TBranch *b_Q             = nt_heli -> GetBranch("Q");
  TBranch *b_dQ            = nt_heli -> GetBranch("dQ");
  TBranch *b_dL            = nt_heli -> GetBranch("dL");
  TBranch *b_dTM           = nt_heli -> GetBranch("dTM");
  TBranch *b_badness       = nt_heli -> GetBranch("badness");
  TBranch *b_rp            = nt_heli -> GetBranch("rp");
  TBranch *b_rd            = nt_heli -> GetBranch("rd");
  TBranch *b_xd            = nt_heli -> GetBranch("xd");
  TBranch *b_yd            = nt_heli -> GetBranch("yd");
  TBranch *b_zd            = nt_heli -> GetBranch("zd");
  TBranch *b_ln            = nt_heli -> GetBranch("ln");
 

  // draw histogram (time difference from muon)
  //
  // for cut conditions
  //
  float TotalCharge_cut      = 4.e+4; // [p.e.]
  float ChargeDifference_cut = 1.e+6; // [p.e.]
  float Distance_cut         = 300;   // [cm]
  float Badness_cut          = 100;  

  // set address
  //
  b_run     -> SetAddress(&run);
  b_dTM     -> SetAddress(&dTM);
  b_Q       -> SetAddress(&Q);
  b_dQ      -> SetAddress(&dQ);
  b_dL      -> SetAddress(&dL);
  b_badness -> SetAddress(&badness);
  b_ln      -> SetAddress(&ln);
  b_Ep      -> SetAddress(&Ep);

  
  // 1. time correlation for showering muon
  //
  c1 -> cd(1);
  const double XMIN1 = 0.0;
  const double XMAX1 = 2.0;
  const double BIN_WIDTH1 = 0.1;
  int bin1 = (int)((XMAX1 - XMIN1) / BIN_WIDTH1);
  TH1F* h1 = new TH1F("dTM_showering_muon", "",  bin1, XMIN1, XMAX1);
  
  int N_heli = nt_heli -> GetEntries();
  
  for (int i = 0 ; i < N_heli ; i++) {
    b_run     -> GetEntry(i);
    b_dTM     -> GetEntry(i);
    b_Q       -> GetEntry(i);
    b_dQ      -> GetEntry(i);
    b_badness -> GetEntry(i);
    b_ln      -> GetEntry(i);
    b_Ep      -> GetEntry(i);

    if (ln==0)continue;
    if (run > 9626)continue;
    //    if (Ep > 8.5)continue;
    if (Ep < 0.9)continue;    


    
    if (Q > TotalCharge_cut && (dQ > ChargeDifference_cut || badness >= Badness_cut)) {
      h1 -> Fill(dTM);
    }
  }
  

  // fitting
  TF1 *f1 = new TF1("f1", "[0]/[1] * 0.1 * exp(-x/[1]) + [2] * 0.1", XMIN1, XMAX1); // original
  //TF1 *f1 = new TF1("f1", "[0]/[1] * 0.1 * exp(-x/[1]) + [2]", XMIN1, XMAX1);
  f1 -> FixParameter(1, 0.2572); // 9Li lifetime = 257.2 [msec] <FIX>
  f1 -> SetParameter(0, 1365);
  f1 -> SetParameter(2, 23);
  f1 -> SetParName(0, "^{8}He/^{9}Li");
  f1 -> SetParName(2, "Offset");
  f1 -> SetLineWidth(2);
  f1 -> SetLineColor(2);
  h1 -> Fit("f1");
  
  // draw histogram  
  h1 -> SetMaximum(800);
  h1 -> SetXTitle("time difference from muon [sec]");
  h1 -> SetYTitle("Events/0.1sec");
  h1 -> SetLineWidth(2);
  h1 -> SetLineColor(4);
  h1 -> SetMarkerStyle(20);
  h1 -> SetMarkerColor(4);
  h1 -> SetMarkerSize(0.8);
  h1 -> Draw("e");

  // conditions
  TLatex* tex1 = new TLatex(-0.2, 825, "dQ > 10^{6} p.e.");
  tex1 -> SetTextFont(42);
  tex1 -> SetTextSize(0.06);
  tex1 -> Draw();

  c1 -> Update();
  
  
  // 2. time correlation for non-showering muon
  //
  c1 -> cd(2);
  const double XMIN2 = 0.0;
  const double XMAX2 = 2.0;
  const double BIN_WIDTH2 = 0.1;
  int bin2 = (int)((XMAX2 - XMIN2) / BIN_WIDTH2);
  TH1F* h2 = new TH1F("dTM_non-showering_muon", "",  bin2, XMIN2, XMAX2);
  
  for (int i = 0 ; i < N_heli ; i++) {
    b_run     -> GetEntry(i);
    b_dTM     -> GetEntry(i);
    b_Q       -> GetEntry(i);
    b_dQ      -> GetEntry(i);
    b_dL      -> GetEntry(i);
    b_badness -> GetEntry(i);
    b_ln      -> GetEntry(i);
    b_Ep      -> GetEntry(i);

    if (ln==0)continue;
    if (run > 9626)continue;    
    //    if (Ep > 8.5)continue;
    if (Ep < 0.9)continue;    

    if (Q > TotalCharge_cut && dQ <= ChargeDifference_cut && badness < Badness_cut && dL < Distance_cut) {
      h2 -> Fill(dTM);
    }
  }
  

  // fitting
  TF1 *f2 = new TF1("f2", "[0]/[1] * 0.1 * exp(-x/[1]) + [2] * 0.1", XMIN1, XMAX1); // original
  //TF1 *f2 = new TF1("f2", "[0]/[1] * 0.1 * exp(-x/[1]) + [2]", XMIN1, XMAX1);
  f2 -> FixParameter(1, 0.2572); // 9Li lifetime = 257.2 [msec] <FIX>
  f2 -> SetParameter(0, 360);
  f2 -> SetParameter(2, 153);
  f2 -> SetParName(0, "^{8}He/^{9}Li");
  f2 -> SetParName(2, "Offset");
  f2 -> SetLineWidth(2);
  f2 -> SetLineColor(2);
  h2 -> Fit("f2");

  // draw histogram  
  h2 -> SetMaximum(600);  
  h2 -> SetXTitle("time difference from muon [sec]");
  h2 -> SetYTitle("Events/0.1sec");
  h2 -> SetLineWidth(2);
  h2 -> SetLineColor(4);
  h2 -> SetMarkerStyle(20);
  h2 -> SetMarkerColor(4);
  h2 -> SetMarkerSize(0.8);
  h2 -> Draw("e");

  // conditions
  TLatex* tex2 = new TLatex(-0.2, 625, "dQ < 10^{6} p.e., dL < 3m");
  tex2 -> SetTextFont(42);
  tex2 -> SetTextSize(0.06);
  tex2 -> Draw();

  c1 -> Update();


  // 3. time corrilation (other events)
  //
  c1 -> cd(4);
  const double XMIN3 = 0.00;
  const double XMAX3 = 2.00;
  const double BIN_WIDTH3 = 0.1;
  int bin3 = (int)((XMAX3 - XMIN3) / BIN_WIDTH3);
  TH1F* h3 = new TH1F("dTM_other", "",  bin3, XMIN3, XMAX3);
  
  for (int i = 0 ; i < N_heli ; i++) {
    b_run     -> GetEntry(i);
    b_dTM     -> GetEntry(i);
    b_Q       -> GetEntry(i);
    b_dQ      -> GetEntry(i);
    b_dL      -> GetEntry(i);
    b_badness -> GetEntry(i);
    b_ln      -> GetEntry(i);
    b_Ep      -> GetEntry(i);

    if (ln==0)continue;
    if (run > 9626)continue;
    //    if (Ep > 8.5)continue;    
    if (Ep < 0.9)continue;    

    if (Q > TotalCharge_cut && dQ <= ChargeDifference_cut && badness < Badness_cut && dL >= Distance_cut) {
      h3 -> Fill(dTM);
    }
  }
  
  h3 -> SetMinimum(0);  
  h3 -> SetMaximum(600);  
  h3 -> SetXTitle("time difference from muon [sec]");
  h3 -> SetYTitle("Events/0.1sec");
  h3 -> SetLineWidth(2);
  h3 -> SetLineColor(4);
  h3 -> SetMarkerStyle(20);
  h3 -> SetMarkerColor(4);
  h3 -> SetMarkerSize(0.8);
  h3 -> Draw("e");

  // conditions
  TLatex* tex3 = new TLatex(-0.2, 625, "dQ < 10^{6} p.e., dL > 3m");
  tex3 -> SetTextFont(42);
  tex3 -> SetTextSize(0.06);
  tex3 -> Draw();

  c1 -> Update();
  
  cerr << "showering muon     : " << h1 -> GetEntries() << " events" << endl
       << "non-showering muon : " << h2 -> GetEntries() << " events" << endl;


  // get spectrum shape
  //
  ofstream ofs(output_filename);
  const int Nofbin = bin1;
  double x, y_shower, y_non_shower;

  for (int i = 0 ; i < Nofbin ; i++) {
    x            = XMIN1 + (i + 0.5) * BIN_WIDTH1;
    y_shower     = h1 -> GetBinContent(i+1);
    y_non_shower = h2 -> GetBinContent(i+1);

    ofs << i << " " << x << " " << y_shower << " " << y_non_shower << endl;
  }


  if (ofs!=0) {
    cerr << "finish making output file. : " << output_filename << endl;
  }


  // calcrate background
  //
  double showering       = f1 -> GetParameter(0);
  double showering_error = f1 -> GetParError(0);

  double non_showering       = f2 -> GetParameter(0);
  double non_showering_error = f2 -> GetParError(0);

  const double eff = 0.9358; // sylinder cut efficiency

  double Nof_showering, Nof_showering_error;
  double Nof_non_showering, Nof_non_showering_error;
  double Nof_non_showering1, Nof_non_showering_error1;
  double Nof_non_showering2, Nof_non_showering_error2;
  double Nof_bg, Nof_bg_error;

  // showring mion (2sec after muon)
  Nof_showering       = showering * exp(-2.0 / 0.2572);
  Nof_showering_error = showering_error * exp(-2.0 / 0.2572);
  
  // non-showring mion
  // -- 2sec after muon
  Nof_non_showering1       = non_showering * exp(-2.0 / 0.2572);
  Nof_non_showering_error1 = non_showering_error * exp(-2.0 / 0.2572);
  // -- 3msec sylinder sut
  Nof_non_showering2       = non_showering * (1-eff) / eff;
  Nof_non_showering_error2 = non_showering_error * (1-eff) / eff;

  Nof_non_showering       = Nof_non_showering1 + Nof_non_showering2;
  Nof_non_showering_error = sqrt(Nof_non_showering_error1*Nof_non_showering_error1 + Nof_non_showering_error2*Nof_non_showering_error2);

  // total
  Nof_bg = Nof_showering + Nof_non_showering;
  Nof_bg_error = sqrt(Nof_showering_error * Nof_showering_error + Nof_non_showering_error * Nof_non_showering_error);

  cerr << endl
       << "Number of backgrounds" << endl
       << "fit -> Ns = " << showering << " +- " << showering_error << ", N_non = " << non_showering << " +- " << non_showering_error << endl
       << "[showering muon]    " << Nof_showering << " +- " << Nof_showering_error << endl
       << "[non-showering muon]" << Nof_non_showering << " +- " << Nof_non_showering_error << endl
       << "(" << Nof_non_showering1 << " +- " << Nof_non_showering_error1 << ") + (" << Nof_non_showering2 << " +- " << Nof_non_showering_error2 << ")" << endl
       << "[total]             " <<  Nof_bg << " +- " << Nof_bg_error << endl;


#if 1
  c1 -> Print(ps_file);
  gSystem -> Exec(Form("gv %s &", ps_file));
#endif
  
  return 0;
}



#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_draw_8He9Li_time_correlation();
}
#endif
