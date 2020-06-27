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

Int_t root_DelayedCoincidence_FastNeutron_scale()
{
  // -------------------------------------------------------------------------------------------------------- //

  string ofs_file_name1 = "./table/FastNeutron-all.dat"; 
  string ofs_file_name2 = "./table/FastNeutron-9626.dat"; 
  string ofs_file_name3 = "./table/FastNeutron-DS3.dat"; 


  char *ps_file = "./picture/DelayedCoincidence-FastNeutron.ps";


  // -------------------------------------------------------------------------------------------------------- //

  double Neutron_2011Paper = 3.2;
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

  char list_e_DS1[256] = "./table/SelectionEfficiency-WeightedAverage.dat-DS1";

  string line_e_DS1;
  ifstream flist_e_DS1(list_e_DS1);
  if (!flist_e_DS1)
    {
      cerr << "ERROR : don't exite such a data DS1 file! " << endl;
      return 0;
    }

  int ARRAY = 1000;
  double Ep_DS1[ARRAY];
  double efficiency_DS1[ARRAY];


  int DS1=0;
  while(getline(flist_e_DS1, line_e_DS1)) 
    {
     istringstream iss_DS1(line_e_DS1); 
     iss_DS1  >> Ep_DS1[DS1] >> efficiency_DS1[DS1]; 
     //     cerr << Ep_DS1[DS1] << " " << efficiency_DS1[DS1] << endl;
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
  int l = 0;


  for (int k = 0; k < DS1; k++){
    double E = Ep_DS1[k];


    efficiency_weighted[k] = (func_DS1.linint(E)*LT_DS1/LT_all) + (func_DS2.linint(E)*LT_DS2/LT_all) + (func_DS3.linint(E)*LT_DS3/LT_all);

    efficiency_weighted_Paper[k] = (func_DS1.linint(E)*LT_DS1/LT_2011) + (func_DS2.linint(E)*(LT_2011-LT_DS1)/LT_2011);
    
    //    cout << E << " " << func_DS1.linint(E) << endl;
    l++;}

  kuFUNC func_wei(l, Ep_DS1, efficiency_weighted);
  kuFUNC func_wei_Paper(l, Ep_DS3, efficiency_weighted_Paper);




  // spallation scale


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

  TH1D* h_flat = new TH1D("h_flat", "", binX, XMIN, XMAX);
  TH1D* h_eff = new TH1D("h_eff", "", binX, XMIN, XMAX);
  TH1D* h_eff_DS3 = new TH1D("h_eff_DS3", "", binX, XMIN, XMAX);
  TH1D* h_eff_Paper = new TH1D("h_eff_Paper", "", binX, XMIN, XMAX);


  // set Paper spectrum


  for (int p = 0; p < DS1; p++)
    {

      h_flat -> SetBinContent(p+1,1);

    }




  h_flat -> SetMarkerColor(5);
  h_flat -> SetMarkerSize(0.5);
  h_flat -> SetMarkerStyle(20);
  h_flat -> SetLineColor(4);
  h_flat -> SetLineWidth(2);
  //  h_flat -> Draw("P");



  int X_min_flat = h_flat -> FindBin(7.5);
  int X_max_flat = h_flat -> FindBin(30);

  float in_flat = h_flat -> Integral(X_min_flat,X_max_flat);
  float scale_flat = Neutron_2011Paper/in_flat;

  TH1D *h_flat_scaled = new TH1D(scale_flat*(*h_flat));

  float in_flat_test = h_flat_scaled -> Integral(X_min_flat,X_max_flat);

  cerr << "3.2 " << in_flat_test << endl;; 



  double Ep_flat_scaled[ARRAY];
  int Ep_flat_bin[ARRAY];
  double N_flat[ARRAY];
  int m = 0;

  for (int q = 0; q < 291; q++)
    {
      Ep_flat_scaled[q] = 0.95 + (0.1*q);
      Ep_flat_bin[q] = h_flat_scaled -> FindBin(Ep_flat_scaled[q]);
      N_flat[q] = h_flat_scaled -> GetBinContent(Ep_flat_bin[q]);

      m++;}

  kuFUNC func_flat_scaled(m, Ep_flat_scaled, N_flat);




  double N_eff[ARRAY];
  double N_eff_DS3[ARRAY];
  double N_eff_Paper[ARRAY];
  int n=0;



   for (int r = 0; r < 291; r++)
     {

       double E2 = Ep_flat_scaled[r];
 
       N_eff[r] = func_flat_scaled.linint(E2) * func_wei.linint(E2)/0.918941;
       N_eff_DS3[r] = func_flat_scaled.linint(E2) * func_DS3.linint(E2)/0.918941;
       N_eff_Paper[r] = func_flat_scaled.linint(E2) * func_wei_Paper.linint(E2)/0.918941;

       //       cerr << E2 << " " << " " << N_eff_Paper[r] << " " << func_flat_scaled.linint(E2) << endl;

     }



  for (int s = 0; s < DS1; s++)
    {
      h_eff -> Fill(Ep_flat_scaled[s],N_eff[s]);
      h_eff_DS3 -> Fill(Ep_flat_scaled[s],N_eff_DS3[s]);
      h_eff_Paper -> Fill(Ep_flat_scaled[s],N_eff_Paper[s]);

    }


  int X_min_eff = h_eff -> FindBin(0.9);
  int X_max_eff = h_eff -> FindBin(30);

  float in_eff       = h_eff -> Integral(X_min_eff,X_max_eff);
  float in_eff_DS3   = h_eff_DS3 -> Integral(X_min_eff,X_max_eff);
  float in_eff_Paper = h_eff_Paper -> Integral(X_min_eff,X_max_eff);


  float scale_eff = in_eff * LT_all/LT_2011/in_eff;
  float scale_eff_DS3 = in_eff_DS3 * LT_DS3/LT_2011/in_eff_DS3;
  float scale_eff_Paper = in_eff_Paper/in_eff_Paper ;


     c1 -> cd(1);

  TH1D *h_FN_scaled = new TH1D(scale_eff*(*h_eff));
  TH1D *h_FN_scaled_DS3 = new TH1D(scale_eff_DS3*(*h_eff_DS3));
  TH1D *h_FN_scaled_Paper = new TH1D(scale_eff_Paper*(*h_eff_Paper));


  int X_min_eff_test = h_eff -> FindBin(7.5);
  int X_max_eff_test = h_eff -> FindBin(30);


  float in_eff_test2       = h_FN_scaled_Paper -> Integral(X_min_eff_test,X_max_eff_test);
  cerr << scale_eff_Paper << " may be 3.2 " << in_eff_test2 << endl; 




  h_eff_Paper -> SetMarkerColor(5);
  h_eff_Paper -> SetMarkerSize(0.5);
  h_eff_Paper -> SetMarkerStyle(20);
  h_eff_Paper -> SetLineColor(4);
  h_eff_Paper -> SetLineWidth(2);
  //  h_eff_Paper -> Draw("P");


  h_FN_scaled_Paper -> SetMarkerColor(5);
  h_FN_scaled_Paper -> SetMarkerSize(0.5);
  h_FN_scaled_Paper -> SetMarkerStyle(20);
  h_FN_scaled_Paper -> SetLineColor(4);
  h_FN_scaled_Paper -> SetLineWidth(2);
  h_FN_scaled_Paper -> Draw("P");



  double Ep_FN_scaled[ARRAY];
  int Ep_FN_bin[ARRAY];
  double N_FN_scaled[ARRAY];
  double N_FN_scaled_DS3[ARRAY];
  double N_FN_scaled_Paper[ARRAY];
  int v = 0;

  for (int t = 0; t < 291; t++)
    {
      Ep_FN_scaled[t] = 0.95 + (0.1*t); // 0.95~30.05 

      Ep_FN_bin[t] = h_FN_scaled -> FindBin(Ep_FN_scaled[t]);
      N_FN_scaled[t] = h_FN_scaled -> GetBinContent(Ep_FN_bin[t]);
      N_FN_scaled_DS3[t] = h_FN_scaled_DS3 -> GetBinContent(Ep_FN_bin[t]);
      N_FN_scaled_Paper[t] = h_FN_scaled_Paper -> GetBinContent(Ep_FN_bin[t]);

      ofs_all << Ep_FN_scaled[t] << " " << N_FN_scaled[t] << endl;
      ofs_DS3 << Ep_FN_scaled[t] << " " << N_FN_scaled_DS3[t] << endl;
      ofs_Paper << Ep_FN_scaled[t] << " " << N_FN_scaled_Paper[t] << endl;



      v++;}




  c1 -> Update();



      

 
  c1 -> Print(ps_file);
  gSystem -> Exec(Form("gv  %s &", ps_file));








  
  return 0;
}

     


#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_DelayedCoincidence_FastNeutron_scale();
}
#endif
