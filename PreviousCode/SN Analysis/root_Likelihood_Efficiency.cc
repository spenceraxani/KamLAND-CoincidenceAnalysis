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
#include "TGaxis.h"
#include "TGraphErrors.h"
#include "TPostScript.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <math.h>
//#include "kuFUNC.hh"
using namespace std;
#endif

Int_t root_Likelihood_Efficiency()
{

  char *ps_file = "./picture/Likelihood_efficiency.ps";



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

  const double XMIN = 0.9;
  const double XMAX = 8.5;
  const double YMIN = 0.2;
  const double YMAX = 1;
  //  const double BIN_WIDTHX = 0.1;
  //  const double BIN_WIDTHY = 0.01;
  //  int binX = (int)((XMAX - XMIN) / BIN_WIDTHX);//100keV
  //  int binY = (int)((YMAX - YMIN) / BIN_WIDTHY);


  TH1F *frame = gPad -> DrawFrame(XMIN, YMIN, XMAX, YMAX);

  c1 -> cd(1);
  frame -> SetXTitle("Ep(MeV)");
  frame -> SetYTitle("Efficiency");





  //  TH1D* h_DS1 = new TH1D("h_DS1", "", binX, XMIN, XMAX);
  //  TH1D* h_DS2 = new TH1D("h_DS2", "", binX, XMIN, XMAX);
  //  TH1D* h_DS3 = new TH1D("h_DS3", "", binX, XMIN, XMAX);






  //-------- efficiency----------//

  char list_efficiency_DS1[256] = "./table/SelectionEfficiency-WeightedAverage.dat-DS1";

  string line_efficiency_DS1;
  ifstream flist_efficiency_DS1(list_efficiency_DS1);
  if (!flist_efficiency_DS1)
    {
      cerr << "ERROR : don't exite such a data DS1 file! " << endl;
      return 0;
    }

  int ARRAY = 20000;
  double Ep_DS1[ARRAY];
  double efficiency_DS1 [ARRAY];

  int DS1=0;
  while(getline(flist_efficiency_DS1, line_efficiency_DS1)) 
    {
     istringstream iss_DS1(line_efficiency_DS1); 
     iss_DS1  >> Ep_DS1[DS1] >> efficiency_DS1[DS1]; 
     //     h_DS1 -> Fill( Ep_DS1[DS1], efficiency_DS1[DS1]);
     cerr << Ep_DS1[DS1] << " " << efficiency_DS1[DS1] << endl;
    
     DS1++;}





  char list_efficiency_DS2[256] = "./table/SelectionEfficiency-WeightedAverage.dat-DS2";

  string line_efficiency_DS2;
  ifstream flist_efficiency_DS2(list_efficiency_DS2);
  if (!flist_efficiency_DS2)
    {
      cerr << "ERROR : don't exite such a data DS2 file! " << endl;
      return 0;
    }


  double Ep_DS2[ARRAY];
  double efficiency_DS2 [ARRAY];

  int DS2=0;
  while(getline(flist_efficiency_DS2, line_efficiency_DS2)) 
    {
     istringstream iss_DS2(line_efficiency_DS2); 
     iss_DS2  >> Ep_DS2[DS2] >> efficiency_DS2[DS2]; 
     //     h_DS2 -> Fill( Ep_DS2[DS2], efficiency_DS2[DS2])
    
     DS2++;}



  char list_efficiency_DS3[256] = "./table/SelectionEfficiency-WeightedAverage.dat-DS3";

  string line_efficiency_DS3;
  ifstream flist_efficiency_DS3(list_efficiency_DS3);
  if (!flist_efficiency_DS3)
    {
      cerr << "ERROR : don't exite such a data DS3 file! " << endl;
      return 0;
    }


  double Ep_DS3[ARRAY];
  double efficiency_DS3 [ARRAY];

  int DS3=0;
  while(getline(flist_efficiency_DS3, line_efficiency_DS3)) 
    {
     istringstream iss_DS3(line_efficiency_DS3); 
     iss_DS3  >> Ep_DS3[DS3] >> efficiency_DS3[DS3]; 
     //     h_DS3 -> Fill( Ep_DS3[DS3], efficiency_DS3[DS3])
    
     DS3++;}


  TGraph *h_DS1 = new TGraph(DS1, Ep_DS1, efficiency_DS1);
  TGraph *h_DS2 = new TGraph(DS2, Ep_DS2, efficiency_DS2);
  TGraph *h_DS3 = new TGraph(DS3, Ep_DS3, efficiency_DS3);


  h_DS1 -> SetMarkerColor(TColor::GetColor("#339900"));
  h_DS1 -> SetMarkerSize(1.0);
  h_DS1 -> SetMarkerStyle(20);
  h_DS1 -> SetLineColor(TColor::GetColor("#339900"));
  h_DS1 -> SetLineWidth(2);
  h_DS1 -> Draw("LP");





  h_DS2 -> SetMarkerColor(TColor::GetColor("#FF33CC"));
  h_DS2 -> SetMarkerSize(1.0);
  h_DS2 -> SetMarkerStyle(20);
  h_DS2 -> SetLineColor(TColor::GetColor("#FF33CC"));
  h_DS2 -> SetLineWidth(2);
  h_DS2 -> Draw("LP");

  h_DS3 -> SetMarkerColor(TColor::GetColor("#3300FF"));
  h_DS3 -> SetMarkerSize(1.0);
  h_DS3 -> SetMarkerStyle(20);
  h_DS3 -> SetLineColor(TColor::GetColor("#3300FF"));
  h_DS3 -> SetLineWidth(2);
  h_DS3 -> Draw("LP");



  c1 -> Update();

 
  c1 -> Print(ps_file);
  gSystem -> Exec(Form("gv  %s &", ps_file));



  
  return 0;
}

     


#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_Likelihood_Efficiency();
}
#endif
