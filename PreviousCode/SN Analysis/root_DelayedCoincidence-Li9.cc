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
using namespace std;
#endif

Int_t root_DelayedCoincidence_all_check()
{
  // -------------------------------------------------------------------------------------------------------- //



#if 1
  //  char *ps_file = "./picture/DelayedCoincidence-and-Veto-Accidental-v1-10000.ps";
  char *ps_file = "./picture/DelayedCoincidence-all-check-11947-7.5-30.ps";

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

  const double XMIN = 0;
  const double XMAX = 30.0;
  const double YMIN = 0;
  const double YMAX = 0.1;
  const double BIN_WIDTHX = 0.1;
  const double BIN_WIDTHY = 0.01;
  int binX = (int)((XMAX - XMIN) / BIN_WIDTHX);//100keV
  int binY = (int)((YMAX - YMIN) / BIN_WIDTHY);

  //  TH2D* h_spa = new TH2D("h_spa", "", binX, XMIN, XMAX, binY, YMIN, YMAX);



  /*
  const float XMIN = 7.5;
  const float XMAX = 30.0;
  const float BINWIDTH = 0.1;
  int bin = (int)((XMAX - XMIN) / BINWIDTH);
  TH1F* h = new TH1F("h", "", bin, XMIN, XMAX);//100keV bin


  */

  

  //spallation


  char list_spallation[256] = "./table/Li9.dat";

  string line;
  ifstream flist_spallation(list_spallation);
  if (!flist_spallation)
    {
      cerr << "ERROR : don't exite such a data spallation file! " << endl;
      return 0;
    }


  int ARRAY = 20000;
  float Ep[ARRAY];
  float N [ARRAY];

  int index=0;
  while(getline(flist_spallation, line)) 
    {
     istringstream iss(line); 
     iss  >> Ep[ARRAY] >> N[ARRAY]; 
    index++}

  TGraph *h_spa = new TGraph(index, Ep, N)


  h_spa -> SetMarkerColor(4);//ao
  h_spa -> SetMarkerSize(0.5);
  h_spa -> SetMarkerStyle(20);

  h_spa -> Draw("PC");

  int X_min = h_spa -> FindBin(0);
  int X_max = h_spa -> FindBin(15);

  cerr << X_min << X_max << endl;

  float sum = h_spa -> Integral(X_min,X_max);

  cerr << sum << endl;
  //  TH2D *h_spa_scaleed = new TH2D()



  //reactor



  //




  c1 -> Update();



      

 
  c1 -> Print(ps_file);
  gSystem -> Exec(Form("gv  %s &", ps_file));

  
  return 0;
}

     


#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_DelayedCoincidence_all_check();
}
#endif
