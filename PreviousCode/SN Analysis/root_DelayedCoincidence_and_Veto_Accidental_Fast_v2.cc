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

Int_t root_DelayedCoincidence_and_Veto_Accidental_Fast_v2()
{
  // -------------------------------------------------------------------------------------------------------- //

#if 1
  char *ps_file = "./picture/DelayedCoincidence-and-Veto-Accidental-all-v2.ps";
#endif
  
  // -------------------------------------------------------------------------------------------------------- //



  //----------Output-----------//
  
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
  gStyle -> SetTitleOffset(1.1, "Y");

  TCanvas *c1 = new TCanvas("c1", "Title", 0, 0, 1000, 700);
  c1 -> SetFillStyle(4001);
  c1 -> SetBottomMargin(0.12);
  c1 -> SetLeftMargin(0.18);

  // make picture
  int type = 112;
  TPostScript *ps = new TPostScript(ps_file, type);
  ps -> Range(25, 12);


  //  const float XMIN = 4.0;
  const float XMIN = 0.0;
  const float XMAX = 30.0;
  const float BINWIDTH = 0.02;
  int bin = (int)((XMAX - XMIN) / BINWIDTH);
  TH1F* h_ln = new TH1F("h_ln", "", bin, XMIN, XMAX);//200keV bin
  TH1F* h_all = new TH1F("h_all", "", bin, XMIN, XMAX);//200keV bin

  double y_all[100][bin], y_ln[100][bin];

  for (int i = 0 ; i < 100 ; i++) {  
    for (int j = 0 ; j < bin ; j++) {  
      y_all[i][j] = 0;
      y_ln[i][j]  = 0;
    }
  }
  
  // read Tree
  //
  // delayed coincidence
  int run, EventNumber, veto, prompt, NhitOD, N200OD, multiplicity;
  float Ed, dR, dT, Ep, xp, yp, zp, xd, yd, zd, rp, rd, Evis, vsTp, chiQp, chiTp, vsTd, chiQd, chiTd, totalQ17, totalQOD, VertexBadness;

  int Noffile = 0;
  for(int i = 1 ; i < 11500 ; i = i+500) {
    //    if (i == 8501)continue;


    // -- set root file name
    char RootFile[256];
    //      sprintf(RootFile, "./result/DelayedCoincidence-and-Veto-Accidental_%06d-%06d.root", i, i+499);
    sprintf(RootFile, "/data/work/yukiko/Solar-Anti-Neutrino/vector-file-Kat/result/DelayedCoincidence-and-Veto-Accidental_%06d-%06d-V2.root", i, i+499);
    cerr << RootFile << endl;

    TFile *tfile = new TFile(RootFile, "open");
    TH1F *h_all_root = (TH1F *)tfile -> Get("h_all_root");
    TH1F *h_ln_root  = (TH1F *)tfile -> Get("h_ln_root");

    cout << "only 1 cycle : " << h_all_root -> GetEntries() << endl;
    cout << "only 1 cycle : " << h_ln_root -> GetEntries() << endl;

    for (int j = 0 ; j < bin ; j++) {
      y_all[Noffile][j] = h_all_root -> GetBinContent(j+1);
      y_ln[Noffile][j]  = h_ln_root -> GetBinContent(j+1);
    }    

    Noffile++;
  }

  double y_all_total[bin], y_ln_total[bin];

  for (int j = 0 ; j < bin ; j++) {  
    y_all_total[j] = 0;
    y_ln_total[j]  = 0;
  }
  
  for (int j = 0 ; j < bin ; j++) {  
    for (int i = 0 ; i < Noffile ; i++) {
      y_all_total[j] += y_all[i][j];
      y_ln_total[j] += y_ln[i][j];
    }

    h_all -> SetBinContent(j+1, y_all_total[j]);
    h_ln  -> SetBinContent(j+1, y_ln_total[j]);

  }

  gPad -> SetLeftMargin(0.19);
  //  c1 -> SetLogy();
  //  double y_min1 = h_all -> GetMinimum();
  //  double y_max1 = h_all -> GetMaximum();
  
  //  h1 -> SetMinimum(1.0);
  //  h1 -> SetMaximum(y_max1*1.7);
  h_all -> SetXTitle("Prompt Energy [MeV]");
  //  h_all -> SetYTitle("Events/0.2MeV");
  h_all -> SetYTitle("Events/0.02MeV");
  h_all -> SetLineWidth(3);
  h_all -> SetLineColor(4);
  //  h_ln -> SetFillColor(TColor::GetColor("#33ccff")); 
  h_ln -> SetLineWidth(3);
  h_ln -> SetLineColor(5);
  //  h_all -> Draw();
  //  h_ln -> Draw("same");
    h_ln -> Draw();
  c1 -> Update();
    
  c1 -> Print(ps_file);
  
  gSystem -> Exec(Form("gv -scale=0.8 %s &", ps_file));
  
  return 0;
}



#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_DelayedCoincidence_and_Veto_Accidental_Fast_v2();
}
#endif
