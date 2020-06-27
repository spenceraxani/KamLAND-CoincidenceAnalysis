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

Int_t root_DelayedCoincidence_spallation_scale()
{
  // -------------------------------------------------------------------------------------------------------- //



#if 1
  //  char *ps_file = "./picture/DelayedCoincidence-and-Veto-Accidental-v1-10000.ps";
  char *ps_file = "./picture/DelayedCoincidence-all-check-11947-7.5-30.ps";

#endif

  string ofs_file_name1 = "./table/spallation-all.dat"; 
  string ofs_file_name2 = "./table/spallation-9626.dat"; 
  string ofs_file_name3 = "./table/spallation-DS3.dat"; 


  // -------------------------------------------------------------------------------------------------------- //





  //-------- Live Time ----------//

  double LT_DS1  = 129269000;
  double LT_DS2  = 98873400;
  double LT_DS3  = 37844600;
  double LT_2011 = 202454000;
  double LT_all  = 265987000;

  
  //---------------------------//



  //--------- Intergral -----------//

#if 0
  double spallation_integral_Paper = 29.4914; //hiroko  93.58%
  double spallation_integral_DS12 = 31.7453;
  double spallation_integral_DS3 = 4.64918;
  double spallation_integral_all = spallation_integral_DS12 + spallation_integral_DS3;
#endif


#if 1
  double spallation_integral_Paper = 19.2187;     //hisashi 95.8%
  double spallation_integral_DS12 = 20.7095;
  double spallation_integral_DS3 =   3.02928;
  double spallation_integral_all = spallation_integral_DS12 + spallation_integral_DS3;


#endif


#if 0
  double spallation_integral_Paper = 28.4517; //shimizu 93.8%
  double spallation_integral_DS12 = 30.6283;
  double spallation_integral_DS3 = 4.48523;
  double spallation_integral_all = spallation_integral_DS12 + spallation_integral_DS3;


#endif


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
  float Ep_DS1[ARRAY];
  float efficiency_DS1 [ARRAY];

  int DS1=0;
  while(getline(flist_efficiency_DS1, line_efficiency_DS1)) 
    {
     istringstream iss_DS1(line_efficiency_DS1); 
     iss_DS1  >> Ep_DS1[DS1] >> efficiency_DS1[DS1]; 
    
     DS1++;}

  char list_efficiency_DS2[256] = "./table/SelectionEfficiency-WeightedAverage.dat-DS2";

  string line_efficiency_DS2;
  ifstream flist_efficiency_DS2(list_efficiency_DS2);
  if (!flist_efficiency_DS2)
    {
      cerr << "ERROR : don't exite such a data DS2 file! " << endl;
      return 0;
    }


  float Ep_DS2[ARRAY];
  float efficiency_DS2 [ARRAY];

  int DS2=0;
  while(getline(flist_efficiency_DS2, line_efficiency_DS2)) 
    {
     istringstream iss_DS2(line_efficiency_DS2); 
     iss_DS2  >> Ep_DS2[DS2] >> efficiency_DS2[DS2]; 
    
     DS2++;}


  char list_efficiency_DS3[256] = "./table/SelectionEfficiency-WeightedAverage.dat-DS3";

  string line_efficiency_DS3;
  ifstream flist_efficiency_DS3(list_efficiency_DS3);
  if (!flist_efficiency_DS3)
    {
      cerr << "ERROR : don't exite such a data DS3 file! " << endl;
      return 0;
    }


  float Ep_DS3[ARRAY];
  float efficiency_DS3 [ARRAY];

  int DS3=0;
  while(getline(flist_efficiency_DS3, line_efficiency_DS3)) 
    {
     istringstream iss_DS3(line_efficiency_DS3); 
     iss_DS3  >> Ep_DS3[DS3] >> efficiency_DS3[DS3]; 
    
     DS3++;}





  float efficiency_weighted[ARRAY]; 
  float efficiency_weighted_Paper[ARRAY]; 

  for (int k = 0; k < DS1; k++)
    {
      for (int l = 0; l < DS2; l++)
	{
	  for (int m = 0; m < DS3; m++)
	    {
	      if (!(Ep_DS1[k] == Ep_DS2[l] && Ep_DS1[k] == Ep_DS3[m]))continue;

	      efficiency_weighted[k] = (efficiency_DS1[k]*LT_DS1/LT_all) + (efficiency_DS2[l]*LT_DS2/LT_all) + (efficiency_DS3[m]*LT_DS3/LT_all);
	      efficiency_weighted_Paper[k] = (efficiency_DS1[k]*LT_DS1/LT_2011) + (efficiency_DS2[l]*(LT_2011-LT_DS1)/LT_2011);

	      //     if(Ep_DS1[k] == 2.0)
	      	      //		{
	      //     cerr << Ep_DS1[k] << " " << Ep_DS2[l] << " " << Ep_DS3[m] << " " <<efficiency_DS1[k] << " " << efficiency_DS2[l] << " " << efficiency_DS3[m] << " " << efficiency_weighted[k] << endl; 
		    
		  //	}
	    }
	}
    }




  
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
  TH1D* h_spa = new TH1D("h_spa", "", binX, XMIN, XMAX);
  TH1D* h_spa_weighted = new TH1D("h_spa_weighted", "", binX, XMIN, XMAX);
  TH1D* h_spa_weighted_DS3 = new TH1D("h_spa_weighted_DS3", "", binX, XMIN, XMAX);
  TH1D* h_spa_weighted_Paper = new TH1D("h_spa_weighted_Paper", "", binX, XMIN, XMAX);


  //  TH1D* h_spa_scaled = new TH1D("h_spa_scaled", "", binX, XMIN, XMAX);
  // TH1D* h_spa_scaled_DS3 = new TH1D("h_spa_scaled_DS3", "", binX, XMIN, XMAX);
  //  TH1D* h_spa_scaled_Paper = new TH1D("h_spa_scaled_Paper", "", binX, XMIN, XMAX);
  

  //spallation


  char list_spallation[256] = "./table/Li9.dat";

  string line;
  ifstream flist_spallation(list_spallation);
  if (!flist_spallation)
    {
      cerr << "ERROR : don't exite such a data spallation file! " << endl;
      return 0;
    }

  /*
  float Ep_spa;
  float N_spa = 0;

  while(flist_spallation >> Ep_spa >> N_spa) 
    {
      h_spa -> Fill(Ep_spa, N_spa);
  

    }
  */

  //  int ARRAY = 20000;
  float Ep[ARRAY];
  float N [ARRAY];
  float N_weighted [ARRAY];
  float N_weighted_DS3 [ARRAY];
  float N_weighted_Paper [ARRAY];
  float Ep_weighted [ARRAY];


  int index=0;
  while(getline(flist_spallation, line)) 
    {
     istringstream iss(line); 
     iss  >> Ep[index] >> N[index]; 
    
     index++;}

  for (int i = 0; i <index; i++)
    {
      h_spa -> Fill(Ep[i],N[i]);

      for (int n = 0; n < DS1; n++)
	{
	  //	  if(!(Ep_DS1[n] - 0.05 <= Ep[i]  && Ep[i] <  Ep_DS1[n] + 0.05))continue;
	  
	  if(!(Ep[i] == Ep_DS1[n]))continue;

	  N_weighted[i] = N[i] * efficiency_weighted[n]; 
	  N_weighted_DS3[i] = N[i] * efficiency_DS3[n]; 
	  N_weighted_Paper[i] = N[i] * efficiency_weighted_Paper[n]; 

	  Ep_weighted[i] = Ep[i];

	  h_spa_weighted -> Fill(Ep[i],N_weighted[i]);  
	  h_spa_weighted_DS3 -> Fill(Ep[i],N_weighted_DS3[i]);  
	  h_spa_weighted_Paper -> Fill(Ep[i],N_weighted_Paper[i]);  

	  ofs_all << Ep[i] << " " << N_weighted[i] << endl;
	  ofs_DS3 << Ep[i] << " " << N_weighted_DS3[i] << endl;
	  ofs_Paper << Ep[i] << " " << N_weighted_Paper[i] << endl;

	  cerr << Ep[i] << " " << N_weighted[i] << endl;


	}
    }


  h_spa -> SetMarkerColor(4);//ao
  h_spa -> SetMarkerSize(0.5);
  h_spa -> SetMarkerStyle(20);

  //  h_spa -> Draw("C");

  h_spa_weighted -> SetMarkerColor(4);//ao
  h_spa_weighted -> SetMarkerSize(0.5);
  h_spa_weighted -> SetMarkerStyle(20);

  h_spa_weighted -> Draw("P");



  int X_min = h_spa_weighted -> FindBin(0);
  int X_max = h_spa_weighted -> FindBin(15);

  //  cerr << X_min << X_max << endl;

  float sum = h_spa_weighted -> Integral(X_min,X_max);
  float sum_DS3 = h_spa_weighted_DS3 -> Integral(X_min,X_max);
  float sum_Paper = h_spa_weighted_Paper -> Integral(X_min,X_max);





  cerr << "sum " << sum << endl;


  //spallation scale

  float scale = spallation_integral_all/sum;
  //  float scale = 1/sum;
  float scale_DS3 = spallation_integral_DS3/sum_DS3;
  float scale_Paper = spallation_integral_Paper/sum_Paper;

  cerr << "scale " << scale << endl;


  //  cerr << scale << endl;


#if 0

  float N_scaled[ARRAY];
  float N_scaled_DS3[ARRAY];
  float N_scaled_Paper[ARRAY];

  for (int o = 0; o <index; o++)
    {

      for (int p = 0; p < index; p++)
	{
	  if(!(Ep[o] == Ep_weighted[p]))continue;

	  if (!(Ep[o] >= 0.9))continue;
	  
	  N_scaled[o] = N_weighted[p] * scale; 
	  N_scaled_DS3[o] = N_weighted_DS3[p] * scale_DS3; 
	  N_scaled_Paper[o] = N_weighted_Paper[p] * scale_Paper; 
	  
	  ofs_all << Ep[o] << " " << N_scaled[o] << endl;
	  ofs_DS3 << Ep[o] << " " << N_scaled_DS3[o] << endl;
	  ofs_Paper << Ep[o] << " " << N_scaled_Paper[o] << endl;


	  //	  h_spa_scaled -> Fill(Ep[o],N_scaled[o]);  
	}
    }
#endif

#if 0
  TFile f_all ();
  h_spa_weighted -> Write();
  //  f_all.Close;

  TFile f_DS3 ("h_spa_weighted_DS3.root"," ");
  h_spa_weighted_DS3 -> Write();
  //  f_DS3.Close;

  TFile f_Paper ("h_spa_weighted_Paper.root"," "); 
  h_spa_weighted_Paper -> Write();
  //  f_Paper.Close;


#endif

  TH1D *h_spa_scaled = new TH1D(scale*(*h_spa_weighted));
  TH1D *h_spa_scaled_DS3 = new TH1D(scale_DS3*(*h_spa_weighted));
  TH1D *h_spa_scaled_Paper = new TH1D(scale_Paper*(*h_spa_weighted));

  //  h_spa_scaled -> Scale(spallation_integral_all);

  int X_min_scaled = h_spa_scaled_DS3 -> FindBin(0.9);
  int X_max_scaled = h_spa_scaled_DS3 -> FindBin(15);

  //  cerr << X_min << X_max << endl;

  //  h_spa_scaled -> Scale(spallation_integral_all);




  float sum_scaled = h_spa_scaled_DS3 -> Integral(X_min_scaled,X_max_scaled);

  cerr << "sum_scaled_all " << sum_scaled << endl; 


  ;
  //  TH1D *h_spa_scaled = new TH1D(scale*h_spa_weighted);
  h_spa_scaled -> SetMarkerColor(3);
  h_spa_scaled -> SetMarkerSize(0.5);
  h_spa_scaled -> SetMarkerStyle(20);
  h_spa_scaled -> SetLineColor(3);
  h_spa_scaled -> Draw("PC");

  c1 -> Update();





 












  //




  c1 -> Update();



      

 
  c1 -> Print(ps_file);
  gSystem -> Exec(Form("gv  %s &", ps_file));

  
  return 0;
}

     


#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_DelayedCoincidence_spallation_scale();
}
#endif
