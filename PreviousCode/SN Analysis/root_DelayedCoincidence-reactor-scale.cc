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

Int_t root_DelayedCoincidence_reactor_scale()
{
  // -------------------------------------------------------------------------------------------------------- //



#if 1
  char *ps_file = "./picture/DelayedCoincidence-reactor-scale.ps";

#endif

  string ofs_file_name1 = "./table/reactor-all.dat"; 
  string ofs_file_name2 = "./table/reactor-9626.dat"; 
  string ofs_file_name3 = "./table/reactor-DS3.dat"; 


  // -------------------------------------------------------------------------------------------------------- //





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



  //-------- Reactor Data ----------//

  char list_DS1[256] = "./table/Expected-ReactorNeutrino-DS1.dat-130829-1485.68";

  string line_DS1;
  ifstream flist_DS1(list_DS1);
  if (!flist_DS1)
    {
      cerr << "ERROR : don't exite such a data DS1 file! " << endl;
      return 0;
    }

  int ARRAY = 20000;
  float Ep_DS1[ARRAY];
  float N_DS1 [ARRAY];

  int DS1=0;
  while(getline(flist_DS1, line_DS1)) 
    {
     istringstream iss_DS1(line_DS1); 
     iss_DS1  >> Ep_DS1[DS1] >> N_DS1[DS1]; 
    
     DS1++;}

  char list_DS2[256] = "./table/Expected-ReactorNeutrino-DS2.dat-130829-1153.95";

  string line_DS2;
  ifstream flist_DS2(list_DS2);
  if (!flist_DS2)
    {
      cerr << "ERROR : don't exite such a data DS2 file! " << endl;
      return 0;
    }

  float Ep_DS2[ARRAY];
  float N_DS2 [ARRAY];

  int DS2=0;
  while(getline(flist_DS2, line_DS2)) 
    {
     istringstream iss_DS2(line_DS2); 
     iss_DS2  >> Ep_DS2[DS2] >> N_DS2[DS2]; 
    
     DS2++;}




  char list_DS3[256] = "./table/Expected-ReactorNeutrino-DS3.dat-130829-351.63";

  string line_DS3;
  ifstream flist_DS3(list_DS3);
  if (!flist_DS3)
    {
      cerr << "ERROR : don't exite such a data DS3 file! " << endl;
      return 0;
    }

  float Ep_DS3[ARRAY];
  float N_DS3 [ARRAY];

  int DS3=0;
  while(getline(flist_DS3, line_DS3)) 
    {
     istringstream iss_DS3(line_DS3); 
     iss_DS3  >> Ep_DS3[DS3] >> N_DS3[DS3]; 
    
     DS3++;}




  
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


  TH1D* h_reactor = new TH1D("h_reactor", "", binX, XMIN, XMAX);



  float N_all[ARRAY];
  float N_Paper[ARRAY];
  float N_Period3[ARRAY];
 
  double Days_DS1 = 1485.68;
  double Days_DS2 = 1153.95;
  double Days_DS3 = 351.63;


  double scale_DS3 = LT_DS3/60/60/24/Days_DS3;
  double Paper_Days_DS2 = (LT_2011-LT_DS1)/60/60/24; 

  for (int i = 0; i <DS1; i++)
    {

      for (int k = 0; k <DS1; k++)
	{
	  for (int l = 0; l <DS1; l++)
	    {

	      if (!(Ep_DS1[i] == Ep_DS2[k] && Ep_DS1[i] == Ep_DS3[l]))continue;

	      N_all[i] = N_DS1[i]*Days_DS1 + N_DS2[k]*Days_DS2 + N_DS3[l]*scale_DS3*LT_DS3/60/60/24;
	      N_Paper[i] = N_DS1[i]*Days_DS1 + N_DS2[k]*Paper_Days_DS2;
	      N_Period3[i]=N_DS3[i]*scale_DS3*LT_DS3/60/60/24;

	      ofs_all << Ep_DS1[i] << " " << N_all[i] << endl;
	      ofs_DS3 << Ep_DS1[i] << " " << N_Period3[i] << endl;
	      ofs_Paper << Ep_DS1[i] << " " << N_Paper[i] << endl;




	      }
	}
    }

  /*
  h_spa -> SetMarkerColor(4);//ao
  h_spa -> SetMarkerSize(0.5);
  h_spa -> SetMarkerStyle(20);

  //  h_spa -> Draw("C");

  h_spa_weighted -> SetMarkerColor(4);//ao
  h_spa_weighted -> SetMarkerSize(0.5);
  h_spa_weighted -> SetMarkerStyle(20);

  h_spa_weighted -> Draw("P");

 
  c1 -> Print(ps_file);
  gSystem -> Exec(Form("gv  %s &", ps_file));

  */

  return 0;
}

     


#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_DelayedCoincidence_reactor_scale();
}
#endif
