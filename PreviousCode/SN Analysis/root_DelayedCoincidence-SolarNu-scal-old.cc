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
#include "kuFUNC.hh"
using namespace std;
#endif

Int_t root_DelayedCoincidence_SolarNu_scale()
{
  // -------------------------------------------------------------------------------------------------------- //


  string ofs_file_name1 = "./table/SolarNeutrino-all.dat"; 
  string ofs_file_name2 = "./table/SolarNeutrino-9626.dat"; 
  string ofs_file_name3 = "./table/SolarNeutrino-DS3.dat"; 

  char *ps_file = "./picture/DelayedCoincidence-SolarNeutrino.ps";


  // -------------------------------------------------------------------------------------------------------- //


  //  double efficiency_2011Paper = 0.92;



  //-------- Live Time ----------//

  double LT_DS1  = 129269000;
  double LT_DS2  = 98873400;
  double LT_DS3  = 37844600;
  double LT_2011 = 202454000;
  double LT_all  = 265987000;

  
  //---------------------------//






  //----------Output-----------//
  ofstream ofs_all(ofs_file_name1.c_str());
  ofstream ofs_Paper(ofs_file_name2.c_str());
  ofstream ofs_DS3(ofs_file_name3.c_str());




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
     //     cerr  << Ep_DS1[DS1] <<" "<< efficiency_DS1[DS1]<< endl;; 
    
     DS1++;}

  kuFUNC func_DS1(DS1, Ep_DS1, efficiency_DS1);

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
    
     DS2++;}
  kuFUNC func_DS2(DS2, Ep_DS2, efficiency_DS2);


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
    
     DS3++;}


  kuFUNC func_DS3(DS3, Ep_DS3, efficiency_DS3);


  double efficiency_weighted[ARRAY]; 
  double efficiency_weighted_Paper[ARRAY]; 

  int num =0;

  for (int k = 0; k < DS1; k++)
    {
      for (int l = 0; l < DS2; l++)
	{
	  for (int m = 0; m < DS3; m++)
	    {
	      if (!(Ep_DS1[k] == Ep_DS2[l] && Ep_DS1[k] == Ep_DS3[m]))continue;

	      efficiency_weighted[k] = (efficiency_DS1[k]*LT_DS1/LT_all) + (efficiency_DS2[l]*LT_DS2/LT_all) + (efficiency_DS3[m]*LT_DS3/LT_all);
	      efficiency_weighted_Paper[k] = (efficiency_DS1[k]*LT_DS1/LT_2011) + (efficiency_DS2[l]*(LT_2011-LT_DS1)/LT_2011);

	      //	      cerr << Ep_DS1[k] << " " << efficiency_weighted[k] << endl; 

	    }
	}
      num++; }

  kuFUNC func_e_wei(num, Ep_DS1, efficiency_weighted);
  kuFUNC func_e_wei_Paper(num, Ep_DS1, efficiency_weighted_Paper);


  //----------- Solar Neutrino --------------//


  char list_SN[256] = "./SolarNu/SolarNu.dat";

  string line_SN;
  ifstream flist_SN(list_SN);
  if (!flist_SN)
    {
      cerr << "ERROR : don't exite such a data SN file! " << endl;
      return 0;
    }


  double Ep_SN[ARRAY];
  double N_SN [ARRAY];

  int SN=0;
  while(getline(flist_SN, line_SN)) 
    {
     istringstream iss_SN(line_SN); 
     iss_SN  >> Ep_SN[SN] >> N_SN[SN]; 
    
     SN++;}
  kuFUNC func_SN(SN, Ep_SN, N_SN);



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
  const double XMAX = 30.0;
  const double YMIN = 0;
  const double YMAX = 0.1;
  const double BIN_WIDTHX = 0.1;
  const double BIN_WIDTHY = 0.01;
  int binX = (int)((XMAX - XMIN) / BIN_WIDTHX);//100keV
  int binY = (int)((YMAX - YMIN) / BIN_WIDTHY);

  TH1D* h_SN_wei = new TH1D("h_SN_wei", "", binX, XMIN, XMAX);
  TH1D* h_SN_wei_DS3 = new TH1D("h_SN_wei_DS3", "", binX, XMIN, XMAX);
  TH1D* h_SN_wei_Paper = new TH1D("h_SN_wei_Paper", "", binX, XMIN, XMAX);


  double SN_wei_all[ARRAY];
  double SN_wei_DS3[ARRAY];
  double SN_wei_Paper[ARRAY];




  for (int o = 0; o < DS1; o++)
    {


      double E_wei = Ep_DS1[o];




      SN_wei_all[o] = func_SN.linint(E_wei) * func_e_wei.linint(E_wei);

      SN_wei_DS3[o] = func_SN.linint(E_wei) * func_DS3.linint(E_wei);
      
      SN_wei_Paper[o] = func_SN.linint(E_wei) * func_e_wei_Paper.linint(E_wei);

      h_SN_wei -> SetBinContent(o+1, SN_wei_all[o]);     
      h_SN_wei_DS3 -> SetBinContent(o+1, SN_wei_DS3[o]);     
      h_SN_wei_Paper -> SetBinContent(o+1, SN_wei_Paper[o]); 
    
      //      cerr << E_wei << " "  << SN_wei_all[o] <<" "  <<func_e_wei_Paper.linint(E_wei) << endl;

    }


  h_SN_wei_Paper -> SetMarkerColor(5);
  h_SN_wei_Paper -> SetMarkerSize(0.5);
  h_SN_wei_Paper -> SetMarkerStyle(20);
  h_SN_wei_Paper -> SetLineColor(4);
  h_SN_wei_Paper -> SetLineWidth(2);
  //  h_SN_wei_Paper -> Draw("P");





	  
  double set_integral_SN = 2.2 * LT_all/LT_2011; 
  double set_integral_SN_DS3 = 2.2 * LT_DS3/LT_2011; 
  double set_integral_SN_Paper = 2.2; 
	
  int X_min_SN_wei = h_SN_wei -> FindBin(7.5);
  int X_max_SN_wei = h_SN_wei -> FindBin(30);
  
  float in_wei_SN = h_SN_wei -> Integral(X_min_SN_wei,X_max_SN_wei);
  float in_wei_SN_DS3 = h_SN_wei_DS3 -> Integral(X_min_SN_wei,X_max_SN_wei);
  float in_wei_SN_Paper = h_SN_wei_Paper -> Integral(X_min_SN_wei,X_max_SN_wei);

  float scale_wei_SN = set_integral_SN/in_wei_SN;
  float scale_wei_SN_DS3 = set_integral_SN_DS3/in_wei_SN_DS3;
  float scale_wei_SN_Paper = set_integral_SN_Paper/in_wei_SN_Paper;

  TH1D *h_SN_scaled = new TH1D(scale_wei_SN*(*h_SN_wei));
  TH1D *h_SN_scaled_DS3 = new TH1D(scale_wei_SN_DS3*(*h_SN_wei_DS3));
  TH1D *h_SN_scaled_Paper = new TH1D(scale_wei_SN_Paper*(*h_SN_wei_Paper));



  h_SN_scaled_Paper -> SetMarkerColor(5);
  h_SN_scaled_Paper -> SetMarkerSize(0.5);
  h_SN_scaled_Paper -> SetMarkerStyle(20);
  h_SN_scaled_Paper -> SetLineColor(4);
  h_SN_scaled_Paper -> SetLineWidth(2);
  h_SN_scaled_Paper -> Draw("P");






  double Ep[ARRAY];

  int Ep_SN_bin[ARRAY];
  int Ep_SN_bin_DS3[ARRAY];
  int Ep_SN_bin_Paper[ARRAY];

  double N_SN_scale[ARRAY];
  double N_SN_scale_DS3[ARRAY];
  double N_SN_scale_Paper[ARRAY];



  int m = 0;

  for (int q = 0; q < 291; q++)
    {
      Ep[q] = 0.95 + (0.1*q);

      Ep_SN_bin[q] = h_SN_scaled -> FindBin(Ep[q]);
      Ep_SN_bin_DS3[q] = h_SN_scaled_DS3 -> FindBin(Ep[q]);
      Ep_SN_bin_Paper[q] = h_SN_scaled_Paper -> FindBin(Ep[q]);

      N_SN_scale[q] = h_SN_scaled -> GetBinContent(Ep_SN_bin[q]);
      N_SN_scale_DS3[q] = h_SN_scaled_DS3 -> GetBinContent(Ep_SN_bin_DS3[q]);
      N_SN_scale_Paper[q] = h_SN_scaled_Paper -> GetBinContent(Ep_SN_bin_Paper[q]);


      ofs_all << Ep[q] << " " << N_SN_scale[q] << endl;
      ofs_DS3 << Ep[q] << " " << N_SN_scale_DS3[q] << endl;
      ofs_Paper << Ep[q] << " " << N_SN_scale_Paper[q] << endl;

      m++;}


  c1 -> Update();



      

 
  c1 -> Print(ps_file);
  gSystem -> Exec(Form("gv  %s &", ps_file));



  
  return 0;
}

     


#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_DelayedCoincidence_SolarNu_scale();
}
#endif
