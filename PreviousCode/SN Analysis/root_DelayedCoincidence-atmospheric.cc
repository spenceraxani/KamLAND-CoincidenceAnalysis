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

Int_t root_DelayedCoincidence_atmospheric_scale()
{
  // -------------------------------------------------------------------------------------------------------- //


  string ofs_file_name1_CC = "./table/atmospheric-CC-all.dat"; 
  string ofs_file_name2_CC = "./table/atmospheric-CC-9626.dat"; 
  string ofs_file_name3_CC = "./table/atmospheric-CC-DS3.dat"; 

  string ofs_file_name1_NC = "./table/atmospheric-NC-all.dat"; 
  string ofs_file_name2_NC = "./table/atmospheric-NC-9626.dat"; 
  string ofs_file_name3_NC = "./table/atmospheric-NC-DS3.dat"; 

  char *ps_file = "./picture/DelayedCoincidence-Atmospheric.ps";


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
  ofstream ofs_all_CC(ofs_file_name1_CC.c_str());
  ofstream ofs_Paper_CC(ofs_file_name2_CC.c_str());
  ofstream ofs_DS3_CC(ofs_file_name3_CC.c_str());

  ofstream ofs_all_NC(ofs_file_name1_NC.c_str());
  ofstream ofs_Paper_NC(ofs_file_name2_NC.c_str());
  ofstream ofs_DS3_NC(ofs_file_name3_NC.c_str());



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





  char list_CC[256] = "./data-BackgroundSpectrum/CC-Polynomial.dat";

  string line_CC;
  ifstream flist_CC(list_CC);
  if (!flist_CC)
    {
      cerr << "ERROR : don't exite such a data CC file! " << endl;
      return 0;
    }


  double Ep_CC[ARRAY];
  double N_CC [ARRAY];

  int CC=0;
  while(getline(flist_CC, line_CC)) 
    {
     istringstream iss_CC(line_CC); 
     iss_CC  >> Ep_CC[CC] >> N_CC[CC]; 
    
     CC++;}
  kuFUNC func_CC(CC, Ep_CC, N_CC);



  char list_NC[256] = "./data-BackgroundSpectrum/picture-neutral-current-energy-spactrum-100MeV.dat";

  string line_NC;
  ifstream flist_NC(list_NC);
  if (!flist_NC)
    {
      cerr << "ERROR : don't exite such a data NC file! " << endl;
      return 0;
    }


  double Ep_NC[ARRAY];
  //  double Ep_NC_low[ARRAY];
  //  double Ep_NC_high[ARRAY];
  double N_NC [ARRAY];

  int NC=0;
  while(getline(flist_NC, line_NC)) 
    {
     istringstream iss_NC(line_NC); 
     //     iss_NC  >> Ep_NC[NC] >> Ep_NC_low[NC] >> Ep_NC_high[NC] >> N_NC[NC]; 
     iss_NC  >> Ep_NC[NC] >> N_NC[NC]; 
    
     NC++;}

  kuFUNC func_NC(NC, Ep_NC, N_NC);



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

  TH1D* h_CC_wei = new TH1D("h_CC_wei", "", binX, XMIN, XMAX);
  TH1D* h_CC_wei_DS3 = new TH1D("h_CC_wei_DS3", "", binX, XMIN, XMAX);
  TH1D* h_CC_wei_Paper = new TH1D("h_CC_wei_Paper", "", binX, XMIN, XMAX);

  TH1D* h_NC_wei = new TH1D("h_NC_wei", "", binX, XMIN, XMAX);
  TH1D* h_NC_wei_DS3 = new TH1D("h_NC_wei_DS3", "", binX, XMIN, XMAX);
  TH1D* h_NC_wei_Paper = new TH1D("h_NC_wei_Paper", "", binX, XMIN, XMAX);





  double NC_wei_all[ARRAY];
  double NC_wei_DS3[ARRAY];
  double NC_wei_Paper[ARRAY];

  double CC_wei_all[ARRAY];
  double CC_wei_DS3[ARRAY];
  double CC_wei_Paper[ARRAY];







  for (int o = 0; o < DS1; o++)
    {


      double E_wei = Ep_DS1[o];




      CC_wei_all[o] = func_CC.linint(E_wei) * func_e_wei.linint(E_wei);

      CC_wei_DS3[o] = func_CC.linint(E_wei) * func_DS3.linint(E_wei);
      
      CC_wei_Paper[o] = func_CC.linint(E_wei) * func_e_wei_Paper.linint(E_wei);

      h_CC_wei -> SetBinContent(o+1, CC_wei_all[o]);     
      h_CC_wei_DS3 -> SetBinContent(o+1, CC_wei_DS3[o]);     
      h_CC_wei_Paper -> SetBinContent(o+1, CC_wei_Paper[o]); 
    
      cerr << E_wei << " "  << CC_wei_all[o]<< endl;
		
      NC_wei_all[o] = func_NC.linint(E_wei) * func_e_wei.linint(E_wei);

      NC_wei_DS3[o] = func_NC.linint(E_wei) * func_DS3.linint(E_wei);
      
      NC_wei_Paper[o] = func_NC.linint(E_wei) * func_e_wei_Paper.linint(E_wei);

      h_NC_wei -> SetBinContent(o+1, NC_wei_all[o]);     
      h_NC_wei_DS3 -> SetBinContent(o+1, NC_wei_DS3[o]);     
      //      h_NC_wei_Paper -> Fill(Ep_DS1[o], NC_wei_Paper[o]);     
      h_NC_wei_Paper -> SetBinContent(o+1, NC_wei_Paper[o]);     

      //      cerr << E_wei << " " << func_CC.linint(E_wei) << " " << CC_wei_all[o] << " " << func_e_wei.linint(E_wei) << endl;


    }


  h_NC_wei_Paper -> SetMarkerColor(5);
  h_NC_wei_Paper -> SetMarkerSize(0.5);
  h_NC_wei_Paper -> SetMarkerStyle(20);
  h_NC_wei_Paper -> SetLineColor(4);
  h_NC_wei_Paper -> SetLineWidth(2);
  //  h_NC_wei_Paper -> Draw("P");




	  
  double set_integral_CC = 0.9 * LT_all/LT_2011; 
  double set_integral_CC_DS3 = 0.9 * LT_DS3/LT_2011; 
  double set_integral_CC_Paper = 0.9; 
	
  int X_min_CC_wei = h_CC_wei -> FindBin(7.5);
  int X_max_CC_wei = h_CC_wei -> FindBin(30);
  
  float in_wei_CC = h_CC_wei -> Integral(X_min_CC_wei,X_max_CC_wei);
  float in_wei_CC_DS3 = h_CC_wei_DS3 -> Integral(X_min_CC_wei,X_max_CC_wei);
  float in_wei_CC_Paper = h_CC_wei_Paper -> Integral(X_min_CC_wei,X_max_CC_wei);

  float scale_wei_CC = set_integral_CC/in_wei_CC;
  float scale_wei_CC_DS3 = set_integral_CC_DS3/in_wei_CC_DS3;
  float scale_wei_CC_Paper = set_integral_CC_Paper/in_wei_CC_Paper;

  TH1D *h_CC_scaled = new TH1D(scale_wei_CC*(*h_CC_wei));
  TH1D *h_CC_scaled_DS3 = new TH1D(scale_wei_CC_DS3*(*h_CC_wei_DS3));
  TH1D *h_CC_scaled_Paper = new TH1D(scale_wei_CC_Paper*(*h_CC_wei_Paper));




  double set_integral_NC = 16.4 * LT_all/LT_2011; 
  double set_integral_NC_DS3 = 16.4 * LT_DS3/LT_2011; 
  double set_integral_NC_Paper = 16.4; 
	
  int X_min_NC_wei = h_NC_wei -> FindBin(7.5);
  int X_max_NC_wei = h_NC_wei -> FindBin(30);
  
  float in_wei_NC = h_NC_wei -> Integral(X_min_NC_wei,X_max_NC_wei);
  float in_wei_NC_DS3 = h_NC_wei_DS3 -> Integral(X_min_NC_wei,X_max_NC_wei);
  float in_wei_NC_Paper = h_NC_wei_Paper -> Integral(X_min_NC_wei,X_max_NC_wei);

  float scale_wei_NC = set_integral_NC/in_wei_NC;
  float scale_wei_NC_DS3 = set_integral_NC_DS3/in_wei_NC_DS3;
  float scale_wei_NC_Paper = set_integral_NC_Paper/in_wei_NC_Paper;

  TH1D *h_NC_scaled = new TH1D(scale_wei_NC*(*h_NC_wei));
  TH1D *h_NC_scaled_DS3 = new TH1D(scale_wei_NC_DS3*(*h_NC_wei_DS3));
  TH1D *h_NC_scaled_Paper = new TH1D(scale_wei_NC_Paper*(*h_NC_wei_Paper));



  h_NC_scaled_Paper -> SetMarkerColor(5);
  h_NC_scaled_Paper -> SetMarkerSize(0.5);
  h_NC_scaled_Paper -> SetMarkerStyle(20);
  h_NC_scaled_Paper -> SetLineColor(4);
  h_NC_scaled_Paper -> SetLineWidth(2);
  h_NC_scaled_Paper -> Draw("P");






  double Ep[ARRAY];

  int Ep_CC_bin[ARRAY];
  int Ep_CC_bin_DS3[ARRAY];
  int Ep_CC_bin_Paper[ARRAY];

  double N_CC_scale[ARRAY];
  double N_CC_scale_DS3[ARRAY];
  double N_CC_scale_Paper[ARRAY];


  int Ep_NC_bin[ARRAY];
  int Ep_NC_bin_DS3[ARRAY];
  int Ep_NC_bin_Paper[ARRAY];

  double N_NC_scale[ARRAY];
  double N_NC_scale_DS3[ARRAY];
  double N_NC_scale_Paper[ARRAY];





  int m = 0;

  for (int q = 0; q < 291; q++)
    {
      Ep[q] = 0.95 + (0.1*q);

      Ep_CC_bin[q] = h_CC_scaled -> FindBin(Ep[q]);
      Ep_CC_bin_DS3[q] = h_CC_scaled_DS3 -> FindBin(Ep[q]);
      Ep_CC_bin_Paper[q] = h_CC_scaled_Paper -> FindBin(Ep[q]);

      N_CC_scale[q] = h_CC_scaled -> GetBinContent(Ep_CC_bin[q]);
      N_CC_scale_DS3[q] = h_CC_scaled_DS3 -> GetBinContent(Ep_CC_bin_DS3[q]);
      N_CC_scale_Paper[q] = h_CC_scaled_Paper -> GetBinContent(Ep_CC_bin_Paper[q]);


      Ep_NC_bin[q] = h_NC_scaled -> FindBin(Ep[q]);
      Ep_NC_bin_DS3[q] = h_NC_scaled_DS3 -> FindBin(Ep[q]);
      Ep_NC_bin_Paper[q] = h_NC_scaled_Paper -> FindBin(Ep[q]);

      N_NC_scale[q] = h_NC_scaled -> GetBinContent(Ep_NC_bin[q]);
      N_NC_scale_DS3[q] = h_NC_scaled_DS3 -> GetBinContent(Ep_NC_bin_DS3[q]);
      N_NC_scale_Paper[q] = h_NC_scaled_Paper -> GetBinContent(Ep_NC_bin_Paper[q]);


      ofs_all_CC << Ep[q] << " " << N_CC_scale[q] << endl;
      ofs_DS3_CC << Ep[q] << " " << N_CC_scale_DS3[q] << endl;
      ofs_Paper_CC << Ep[q] << " " << N_CC_scale_Paper[q] << endl;

      ofs_all_NC << Ep[q] << " " << N_NC_scale[q] << endl;
      ofs_DS3_NC << Ep[q] << " " << N_NC_scale_DS3[q] << endl;
      ofs_Paper_NC << Ep[q] << " " << N_NC_scale_Paper[q] << endl;




      m++;}


  c1 -> Update();



      

 
  c1 -> Print(ps_file);
  gSystem -> Exec(Form("gv  %s &", ps_file));



  
  return 0;
}

     


#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_DelayedCoincidence_atmospheric_scale();
}
#endif
