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


  //  double Ep[ARRAY];

  double N_SN_scale[ARRAY];
  double N_SN_scale_DS3[ARRAY];
  double N_SN_scale_Paper[ARRAY];

  int m = 0;

  double target_prompt =5.9780e+31 ;
  double conversion = 5.3e-5;

  for (int q = 0; q < SN ; q++)
    {

      double  Ep = Ep_SN[q];
      N_SN_scale[q]       = func_SN.linint(Ep) * LT_all * target_prompt * conversion * func_e_wei.linint(Ep);
      N_SN_scale_DS3[q]   = func_SN.linint(Ep) * LT_DS3 * target_prompt * conversion * func_DS3.linint(Ep);
      N_SN_scale_Paper[q] = func_SN.linint(Ep) * LT_2011 * target_prompt * conversion* func_e_wei_Paper.linint(Ep);


      ofs_all << Ep << " " << N_SN_scale[q] << endl;
      ofs_DS3 << Ep << " " << N_SN_scale_DS3[q] << endl;
      ofs_Paper << Ep << " " << N_SN_scale_Paper[q] << endl;

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
