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

Int_t root_DelayedCoincidence_all_check()
{
  // -------------------------------------------------------------------------------------------------------- //



#if 1
  // all
  char list_file_name_candidate[256] = "./table/candidate-all.dat"; 
  char list_file_name_accidental[256] = "./table/accidental-all.dat"; 
  char list_file_name_FastNeutron[256] = "./table/FastNeutron-all.dat"; 
  char list_file_name_CC[256] = "./table/atmospheric-CC-all.dat"; 
  char list_file_name_NC[256] = "./table/atmospheric-NC-all.dat"; 
  char list_file_name_spallation[256] = "./table/spallation-all.dat"; 
  double spa_integral = 23.73878;  //hisashi  
  char list_file_name_reactor[256] = "./table/reactor-all.dat"; 
  char list_file_name_SN[256] = "./table/SolarNeutrino-all.dat"; 


  char *ps_file = "./picture/DelayedCoincidence-all-check-all.ps";
  string ofs_file_name = "./table/UpperLimit-all.dat"; 



    // frame set
  //  double F_xmin = 7.0;
  //  double F_xmin = 7.5;
  double F_xmin = 0.9;
  double F_xmax = 30;
  double F_ymin = 0;
  //  double F_ymax = 15;
  double F_ymax = 650;


  const double XMIN2 = 7.0;
  const double XMAX2 = 30;

  int min_min =8;//BG 8~
  int  w_max = 22;


  double spallation_DS12 = 20.7095;
  double spallation_DS3 = 3.02928;
  double spallation_all = spallation_DS12 + spallation_DS3;
  double spallation_error = (1.18868 * spallation_DS12 / spallation_all) + (0.425776 * spallation_DS3 / spallation_all);
  double spallation_e_per = spallation_error/spallation_all;


#endif



#if 0
  // (2011Paper)9626
  char list_file_name_candidate[256] = "./table/candidate-9626.dat"; 
  char list_file_name_accidental[256] = "./table/accidental-9626.dat"; 
  char list_file_name_FastNeutron[256] = "./table/FastNeutron-9626.dat";
  char list_file_name_CC[256] = "./table/atmospheric-CC-9626.dat"; 
  char list_file_name_NC[256] = "./table/atmospheric-NC-9626.dat";
  char list_file_name_spallation[256] = "./table/spallation-9626.dat"; 
  double spa_integral = 19.2187;  //hisashi  
  char list_file_name_reactor[256] = "./table/reactor-9626.dat";  
  char list_file_name_SN[256] = "./table/SolarNeutrino-9626.dat"; 

  char *ps_file = "./picture/DelayedCoincidence-all-check-9626.ps"; 


    // frame set
  double F_xmin = 7.0;
  double F_xmax = 30;
  double F_ymin = 0;
  double F_ymax = 12;

  const double XMIN2 = 7.0;
  const double XMAX2 = 30;

#endif 


#if 0
  // DS3
  char list_file_name_candidate[256] = "./table/candidate-DS3.dat"; 
  char list_file_name_accidental[256] = "./table/accidental-DS3.dat"; 
  char list_file_name_FastNeutron[256] = "./table/FastNeutron-DS3.dat";
  char list_file_name_CC[256] = "./table/atmospheric-CC-DS3.dat"; 
  char list_file_name_NC[256] = "./table/atmospheric-NC-DS3.dat";
  char list_file_name_spallation[256] = "./table/spallation-DS3.dat";
  double spa_integral = 3.02928;  //hisashi  
  char list_file_name_reactor[256]= "./table/reactor-DS3.dat";  
  char list_file_name_SN[256] = "./table/SolarNeutrino-DS3.dat"; 

  char *ps_file = "./picture/DelayedCoincidence-all-check-DS3.ps";  
  string ofs_file_name = "./table/UpperLimit-DS3.dat"; 


    // frame set
  double F_xmin = 0.9;
  //  double F_xmin = 7.5;
  double F_xmax = 30;
  double F_ymin = 0;
  double F_ymax = 30;
  //  double F_ymax = 1.5;


  const double XMIN2 = 0.9;
  //const double XMIN2 = 7.5;
  const double XMAX2 = 30;


  int min_min =2;//BG 8~
  int w_max = 28;
 
  double spallation_e_per = 0.425776/3.02928;

#endif


  // -------------------------------------------------------------------------------------------------------- //




  //----------Output-----------//
  ofstream ofs(ofs_file_name.c_str());

  //-------- Live Time ----------//

  double LT_DS1  = 129269000;
  double LT_DS2  = 98873400;
  double LT_DS3  = 37844600;
  double LT_2011 = 202454000;
  double LT_all  = 265987000;

  
  //---------------------------//







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



  TH1D* h_in_accidental = new TH1D("h_in_accidental", "", binX, XMIN, XMAX);
  TH1D* h_in_FastNeutron = new TH1D("h_in_fastneutron", "", binX, XMIN, XMAX);
  TH1D* h_in_CC = new TH1D("h_in_CC", "", binX, XMIN, XMAX);
  TH1D* h_in_NC = new TH1D("h_in_NC", "", binX, XMIN, XMAX);
  TH1D* h_in_reactor = new TH1D("h_in_reactor", "", binX, XMIN, XMAX);




  //-------- frame ---------//

  TH1F *frame = gPad -> DrawFrame(F_xmin, F_ymin, F_xmax, F_ymax);

  c1 -> cd(1);
  frame -> SetXTitle("Ep(MeV)");
  frame -> SetYTitle("events/MeV");





  //    frame -> Draw();




  //-------- candidate ----------//

  string line_candidate;
  ifstream flist_candidate(list_file_name_candidate);
  if (!flist_candidate)
    {
      cerr << "ERROR : don't exite such a data candidate file! " << endl;
      return 0;
    }

  //  const double XMIN2 = 7.0;

  const double BIN_WIDTHX2 = 1.0;
  int binX2 = (int)((XMAX2 - XMIN2) / BIN_WIDTHX2);//1000keV

  TH1D* h_candidate = new TH1D("h_candidate", "", binX2, XMIN2, XMAX2);


  double Ep_candidate[ARRAY];
  double N_candidate[ARRAY];


  int candidate=0;
  while(getline(flist_candidate, line_candidate)) 
    {
     istringstream iss_candidate(line_candidate); 
     iss_candidate  >> Ep_candidate[candidate] >> N_candidate[candidate]; 
     //     h_candidate -> Fill( Ep_candidate[candidate], N_candidate[candidate]);
     h_candidate ->  SetBinContent(candidate+1, N_candidate[candidate]);     
    
     candidate++;}


  kuFUNC func_candidate(candidate,Ep_candidate,N_candidate);



  //  TGraph *h_candidate = new TGraph(candidate, Ep_candidate, N_candidate);

  h_candidate -> SetMarkerColor(1);
  h_candidate -> SetMarkerSize(1.0);
  h_candidate -> SetMarkerStyle(20);
  h_candidate -> SetLineColor(1);
  h_candidate -> SetLineWidth(2);
  h_candidate -> Sumw2();
  h_candidate -> SetMaximum(F_ymax);
  //  h_candidate -> Draw("P");





  // -------- accidental -----------//


  string line_accidental;
  ifstream flist_accidental(list_file_name_accidental);
  if (!flist_accidental)
    {
      cerr << "ERROR : don't exite such a data accidental file! " << endl;
      return 0;
    }

  double Ep_accidental[ARRAY];
  double N_accidental[ARRAY];
  double N_accidental_scale[ARRAY];

  int accidental=0;
  while(getline(flist_accidental, line_accidental)) 
    {
     istringstream iss_accidental(line_accidental); 
     iss_accidental  >> Ep_accidental[accidental] >> N_accidental[accidental]; 

     h_in_accidental -> Fill(Ep_accidental[accidental], N_accidental[accidental]/1000); 
    
     N_accidental_scale [accidental] = N_accidental[accidental]/1000*10; // second scale &  event/0.1Mev -> event/MeV

     accidental++;}




  //  kuFUNC func_accidental(accidental, Ep_accidental,  N_accidental);

  kuFUNC func_accidental(accidental,Ep_accidental,N_accidental_scale);

#if 1
  int X_in_accidental_min = h_in_accidental -> FindBin(0.9);
  int X_in_accidental_max = h_in_accidental -> FindBin(30);

  float in_accidental = h_in_accidental -> Integral(X_in_accidental_min,X_in_accidental_max);

  cerr << "in_accidental " << in_accidental << endl;
#endif







  // ---------- FastNeutron ------------//

  string line_FastNeutron;
  ifstream flist_FastNeutron(list_file_name_FastNeutron);
  if (!flist_FastNeutron)
    {
      cerr << "ERROR : don't exite such a data FastNeutron file! " << endl;
      return 0;
    }


  double Ep_FastNeutron[ARRAY];
  double N_Fast[ARRAY];
  double N_FastNeutron_10[ARRAY];

  int FastNeutron=0;
  while(getline(flist_FastNeutron, line_FastNeutron)) 
    {
     istringstream iss_FastNeutron(line_FastNeutron); 
     iss_FastNeutron  >> Ep_FastNeutron[FastNeutron] >> N_Fast[FastNeutron]; 

     h_in_FastNeutron -> Fill(Ep_FastNeutron[FastNeutron], N_Fast[FastNeutron]); 
    
     N_FastNeutron_10 [FastNeutron] = N_Fast[FastNeutron]*10; // event/0.1Mev -> event/MeV

     //     cerr << Ep_FastNeutron[FastNeutron] << " " << N_Fast[FastNeutron] << endl;     

     FastNeutron++;}


  //  kuFUNC func_FastNeutron(FastNeutron, Ep_FastNeutron,  N_FastNeutron);

  kuFUNC func_FastNeutron(FastNeutron,Ep_FastNeutron,N_FastNeutron_10);

#if 1
  int X_in_FastNeutron_min = h_in_FastNeutron -> FindBin(0.9);
  int X_in_FastNeutron_max = h_in_FastNeutron -> FindBin(30);

  float in_FastNeutron = h_in_FastNeutron -> Integral(X_in_FastNeutron_min,X_in_FastNeutron_max);

  cerr << "in_FastNeutron " << in_FastNeutron << endl;

#endif

  double N_Fast_sum[ARRAY];

  for (int p = 0; p < FastNeutron; p++)

    {
      double Ep_sum_Fast = Ep_FastNeutron[p];

      N_Fast_sum[p] = func_accidental.linint(Ep_sum_Fast) + func_FastNeutron.linint(Ep_sum_Fast);



    }










  // ---------- CC -----------//

  string line_CC;
  ifstream flist_CC(list_file_name_CC);
  if (!flist_CC)
    {
      cerr << "ERROR : don't exite such a data CC file! " << endl;
      return 0;
    }


  double Ep_CC[ARRAY];
  double N_CC [ARRAY];
  double N_CC_10[ARRAY];

  int CC=0;
  while(getline(flist_CC, line_CC)) 
    {
     istringstream iss_CC(line_CC); 
     iss_CC  >> Ep_CC[CC] >> N_CC[CC]; 

     h_in_CC -> Fill(Ep_CC[CC], N_CC[CC]); 

     N_CC_10[CC] = N_CC[CC]*10; // event/0.1Mev -> event/MeV

     //     cerr << N_CC[CC] << endl;
    
     CC++;}

  kuFUNC func_CC(CC, Ep_CC, N_CC_10);

#if 1
  int X_in_CC_min = h_in_CC -> FindBin(0.9);
  int X_in_CC_max = h_in_CC -> FindBin(30);

  float in_CC = h_in_CC -> Integral(X_in_CC_min,X_in_CC_max);

  cerr << "in_CC " << in_CC << endl;
#endif

  double N_CC_sum[ARRAY];

  for (int o = 0; o < CC; o++)

    {
      double Ep_sum_CC = Ep_CC[o];

      N_CC_sum[o] = func_accidental.linint(Ep_sum_CC) + func_FastNeutron.linint(Ep_sum_CC) + func_CC.linint(Ep_sum_CC); 
      //      cerr << func_FastNeutron.linint(Ep_sum_CC) <<" " << N_CC_sum[o] << endl;


    }



  //  TGraph *h_CC = new TGraph(CC, Ep_CC, N_CC);




  //------------ NC -------------//


  string line_NC;
  ifstream flist_NC(list_file_name_NC);
  if (!flist_NC)
    {
      cerr << "ERROR : don't exite such a data NC file! " << endl;
      return 0;
    }


  double Ep_NC[ARRAY];
  double N_NC [ARRAY];
  double N_NC_scale [ARRAY];

  int NC=0;
  while(getline(flist_NC, line_NC)) 
    {
     istringstream iss_NC(line_NC); 
     iss_NC  >> Ep_NC[NC] >> N_NC[NC]; 


     h_in_NC -> Fill(Ep_NC[NC], N_NC[NC]); 

     N_NC_scale [NC] = N_NC[NC]*10; // event/0.1Mev -> event/MeV

     //     cerr << N_NC_scale[NC] << endl;
    
     NC++;}

  kuFUNC func_NC(NC, Ep_NC, N_NC_scale);
#if 1

  int X_in_NC_min = h_in_NC -> FindBin(0.9);
  int X_in_NC_max = h_in_NC -> FindBin(30);

  float in_NC = h_in_NC -> Integral(X_in_NC_min,X_in_NC_max);

  cerr << "in_NC " << in_NC << endl;
#endif

  double N_NC_sum[ARRAY];

  for (int p = 0; p < NC; p++)

    {
      double Ep_sum_NC = Ep_NC[p];

      N_NC_sum[p] = func_accidental.linint(Ep_sum_NC) + func_FastNeutron.linint(Ep_sum_NC) + func_CC.linint(Ep_sum_NC) + func_NC.linint(Ep_sum_NC);

      //      cerr << Ep_sum_NC << " " << func_NC.linint(Ep_sum_NC)  << endl;  



    }








  //--------- spallation -----------//

  string line_spallation;
  ifstream flist_spallation(list_file_name_spallation);
  if (!flist_spallation)
    {
      cerr << "ERROR : don't exite such a data spallation file! " << endl;
      return 0;
    }

  TH1D* h_spa_weighted = new TH1D("h_spa_weighted", "", binX, XMIN, XMAX);

  double Ep_spa_weighted[ARRAY];
  double N_spa_weighted [ARRAY];
  //  double N_spallation_10 [ARRAY];

  int spallation=0;
  while(getline(flist_spallation, line_spallation)) 
    {
     istringstream iss_spallation(line_spallation); 
     iss_spallation  >> Ep_spa_weighted[spallation] >> N_spa_weighted[spallation]; 

     spallation++;}


  //  kuFUNC func_spallation(spallation, Ep_spallation, N_spallation);


  for (int i = 0; i <spallation; i++)
    {
      h_spa_weighted -> Fill(Ep_spa_weighted[i], N_spa_weighted[i]);

      //      cerr << Ep_spa_weighted[i] << " " << N_spa_weighted[i] <<endl;

    }



  int X_min = h_spa_weighted -> FindBin(0);
  int X_max = h_spa_weighted -> FindBin(15);

  float sum = h_spa_weighted -> Integral(X_min,X_max);
  float scale = spa_integral/sum;

  TH1D *h_spa_scaled = new TH1D(scale*(*h_spa_weighted));



  //  TGraph *h_spallation = new TGraph(spallation, Ep_spallation, N_spallation_10);

  h_spa_scaled -> SetMarkerColor(3);
  h_spa_scaled -> SetMarkerSize(0.5);
  h_spa_scaled -> SetMarkerStyle(20);
  h_spa_scaled -> SetLineColor(3);
  h_spa_scaled -> SetLineWidth(2);
  //  h_spa_scaled -> Draw("L");




  int X_min_scaled = h_spa_scaled -> FindBin(0);
  int X_max_scaled = h_spa_scaled -> FindBin(15);

  //  cerr << X_min << X_max << endl;

  float sum_scaled = h_spa_scaled -> Integral(X_min_scaled, X_max_scaled);

  cerr << "integral " << sum_scaled << endl;


  TH1D* h_spa_copy = new TH1D(*h_spa_scaled);

  h_spa_scaled -> Scale(10);
  h_spa_scaled -> SetMaximum(F_ymax);

  //  h_spa_scaled -> Draw("L");


  double Ep_spa[ARRAY];
  int Ep_bin[ARRAY];
  double N_spa[ARRAY];
  int l = 0;

  for (int k = 0; k < 140; k++)
    {
      Ep_spa[k] = 0.95 + (0.1*k);
      Ep_bin[k] = h_spa_scaled -> FindBin(Ep_spa[k]);
      N_spa[k] = h_spa_scaled -> GetBinContent(Ep_bin[k]);

      l++;
    }

  kuFUNC func_spa(l, Ep_spa, N_spa);


  double N_spa_sum[ARRAY];

  for (int q = 0; q < l; q++)

    {
      double Ep_sum_spa = Ep_spa[q];

      N_spa_sum[q] = func_accidental.linint(Ep_sum_spa) + func_FastNeutron.linint(Ep_sum_spa) + func_CC.linint(Ep_sum_spa) + func_NC.linint(Ep_sum_spa) + func_spa.linint(Ep_sum_spa); 

    }



  //---------- reactor --------------//


  string line_reactor;
  ifstream flist_reactor(list_file_name_reactor);
  if (!flist_reactor)
    {
      cerr << "ERROR : don't exite such a data reactor file! " << endl;
      return 0;
    }


  double Ep_reactor[ARRAY];
  double N_reactor [ARRAY];
  double N_reactor_10 [ARRAY];

  int reactor=0;
  while(getline(flist_reactor, line_reactor)) 
    {
     istringstream iss_reactor(line_reactor); 
     iss_reactor  >> Ep_reactor[reactor] >> N_reactor[reactor]; 

     h_in_reactor -> Fill(Ep_reactor[reactor], N_reactor[reactor]); 

     N_reactor_10 [reactor] = N_reactor[reactor]*10; // event/0.1Mev -> event/MeV
    
     reactor++;}

  kuFUNC func_reactor(reactor, Ep_reactor, N_reactor_10);

#if 1
  int X_in_reactor_min = h_in_reactor -> FindBin(0.9);
  int X_in_reactor_max = h_in_reactor -> FindBin(30);

  float in_reactor = h_in_reactor -> Integral(X_in_reactor_min,X_in_reactor_max);

  cerr << "in_reactor " << in_reactor << endl;
#endif
  double N_reactor_sum[ARRAY];


  for (int r = 0; r < reactor; r++)

    {
      double Ep_sum_reactor = Ep_reactor[r];

      N_reactor_sum[r] = func_accidental.linint(Ep_sum_reactor) + func_FastNeutron.linint(Ep_sum_reactor) + func_CC.linint(Ep_sum_reactor) + func_NC.linint(Ep_sum_reactor) + func_spa.linint(Ep_sum_reactor) + func_reactor.linint(Ep_sum_reactor); 

    }


  int s=reactor;
  Ep_reactor[s] = F_xmax;
  N_reactor_sum[s] = 0.0;
  Ep_reactor[s+1] = F_xmin;
  N_reactor_sum[s+1] = 0;
  Ep_reactor[s+2] = F_xmin;
  N_reactor_sum[s+2] = N_reactor_sum[0];


  //  TGraph *h_reactor = new TGraph(reactor, Ep_reactor, N_reactor_10);
  TGraph *h_reactor = new TGraph(reactor+3, Ep_reactor, N_reactor_sum);

  h_reactor -> SetMarkerColor(5);
  h_reactor -> SetMarkerSize(0.5);
  h_reactor -> SetMarkerStyle(20);
  h_reactor -> SetLineColor(TColor::GetColor("#FFCC33"));
  h_reactor -> SetLineWidth(3);
  h_reactor -> SetFillColor(TColor::GetColor("#FFCC33"));
  //  h_reactor -> SetFillColor(TColor::GetColor("#33ccff")); 
  //  h_reactor -> Draw("L");
  h_reactor -> Draw("LF");


  //  int l=spa;
  Ep_spa[l] = F_xmax;
  N_spa_sum[l] = 0.0;
  Ep_spa[l+1] = F_xmin;
  N_spa_sum[l+1] = 0;
  Ep_spa[l+2] = F_xmin;
  N_spa_sum[l+2] = N_spa_sum[0];


  TGraph *h_spallation = new TGraph(l+3, Ep_spa, N_spa_sum);

  h_spallation -> SetMarkerColor(3);
  h_spallation -> SetMarkerSize(0.5);
  h_spallation -> SetMarkerStyle(20);
  h_spallation -> SetLineColor(TColor::GetColor("#66CC00"));
  h_spallation -> SetLineWidth(3);
  h_spallation -> SetFillColor(TColor::GetColor("#66CC00"));
  h_spallation -> Draw("LF");


  int u=NC;
  Ep_NC[u] = F_xmax;
  N_NC_sum[u] = 0.0;
  Ep_NC[u+1] = F_xmin;
  N_NC_sum[u+1] = 0;
  Ep_NC[u+2] = F_xmin;
  N_NC_sum[u+2] = N_NC_sum[0];


  TGraph *h_NC = new TGraph(NC+3, Ep_NC, N_NC_sum);

  h_NC -> SetMarkerColor(4);
  h_NC -> SetMarkerSize(0.5);
  h_NC -> SetMarkerStyle(20);
  h_NC -> SetLineColor(4);
  h_NC -> SetLineWidth(2);
  h_NC -> SetFillColor(4);
  h_NC -> Draw("LF");

  int v=CC;
  Ep_CC[v] = F_xmax;
  N_CC_sum[v] = 0.0;
  Ep_CC[v+1] = F_xmin;
  N_CC_sum[v+1] = 0;
  Ep_CC[v+2] = F_xmin;
  N_CC_sum[v+2] = N_CC_sum[0];


  TGraph *h_CC = new TGraph(CC+3, Ep_CC, N_CC_sum);

  h_CC -> SetMarkerColor(7);
  h_CC -> SetMarkerSize(0.5);
  h_CC -> SetMarkerStyle(20);
  h_CC -> SetLineColor(7);
  h_CC -> SetLineWidth(2);
  h_CC -> SetFillColor(7);
  h_CC -> Draw("LF");

  int w=FastNeutron;
  Ep_FastNeutron[w] = F_xmax;
  N_Fast_sum[w] = 0.0;
  Ep_FastNeutron[w+1] = F_xmin;
  N_Fast_sum[w+1] = 0;
  Ep_FastNeutron[w+2] = F_xmin;
  N_Fast_sum[w+2] = N_Fast_sum[0];

  TGraph *h_FastNeutron = new TGraph(FastNeutron+3, Ep_FastNeutron, N_Fast_sum);

  h_FastNeutron -> SetMarkerColor(6);//ao
  h_FastNeutron -> SetMarkerSize(0.5);
  h_FastNeutron -> SetMarkerStyle(20);
  h_FastNeutron -> SetLineColor(6);
  h_FastNeutron -> SetLineWidth(2);
  h_FastNeutron -> SetFillColor(6);
  h_FastNeutron -> Draw("LF");

  int a=accidental;
  Ep_accidental[a] = F_xmax;
  N_accidental_scale[a] = 0.0;
  Ep_accidental[a+1] = F_xmin;
  N_accidental_scale[a+1] = 0;
  Ep_accidental[a+2] = F_xmin;
  N_accidental_scale[a+2] = N_accidental_scale[0];


  TGraph *h_accidental = new TGraph(accidental+3, Ep_accidental, N_accidental_scale);

  h_accidental -> SetMarkerColor(13);
  h_accidental -> SetMarkerSize(0.5);
  h_accidental -> SetMarkerStyle(20);
  h_accidental -> SetLineColor(13);
  h_accidental -> SetLineWidth(2);
  h_accidental -> SetFillColor(13);
  h_accidental -> Draw("LF");


  //  frame -> Draw(same);


  //----------- Solar Neutrino --------------//

  string line_SN;
  ifstream flist_SN(list_file_name_SN);
  if (!flist_SN)
    {
      cerr << "ERROR : don't exite such a data SN file! " << endl;
      return 0;
    }

  double Ep_SN[ARRAY];
  double N_SN[ARRAY];
  double N_SN_scale[ARRAY];

  int SN=0;
  while(getline(flist_SN, line_SN)) 
    {
     istringstream iss_SN(line_SN); 
     iss_SN  >> Ep_SN[SN] >> N_SN[SN]; 
    
     //     N_SN_scale [SN] = N_SN[SN]*10; // second scale &  event/0.1Mev -> event/MeV

     SN++;}


  //  kuFUNC func_SN(SN, Ep_SN,  N_SN);

  kuFUNC func_SN(SN,Ep_SN,N_SN);


  double N_SN_sum[ARRAY];
  int t=0;

  for (int s = 0; s < SN; s++)

    {
      double Ep_sum_SN = Ep_SN[s];

      N_SN_sum[s] = func_accidental.linint(Ep_sum_SN) + func_FastNeutron.linint(Ep_sum_SN) + func_CC.linint(Ep_sum_SN) + func_NC.linint(Ep_sum_SN) + func_spa.linint(Ep_sum_SN) + func_reactor.linint(Ep_sum_SN) + func_SN.linint(Ep_sum_SN); 

//       if (Ep_SN[s] < 15)

// 	{     cerr << Ep_SN[s] << " " << N_SN_sum[s] << endl;}

      t++; }



  TGraph *h_SN = new TGraph(t, Ep_SN, N_SN_sum);

  h_SN -> SetMarkerColor(2);
  h_SN -> SetMarkerSize(0.3);
  h_SN -> SetMarkerStyle(20);
  h_SN -> SetLineColor(2);
  h_SN -> SetLineWidth(3);
  h_SN -> Draw("P");


  // out put BG & candidate



  int BGbin_accidental_min;
  int BGbin_accidental_max;
  int BGbin_FN_min;
  int BGbin_FN_max;
  int BGbin_CC_min;
  int BGbin_CC_max;
  int BGbin_NC_min;
  int BGbin_NC_max;
  int BGbin_spallation_min;
  int BGbin_spallation_max;
  int BGbin_reactor_min;
  int BGbin_reactor_max;








  for (int w=0; w < w_max ; w++) 

    {

      int      min = min_min + w ;
      int      max = min + 1;



      BGbin_accidental_min = h_in_accidental -> FindBin(min);
      BGbin_accidental_max = h_in_accidental -> FindBin(max);
      float BG_accidental = h_in_accidental -> Integral(BGbin_accidental_min,BGbin_accidental_max);
      float BG_e_accidental = sqrt(BG_accidental);


      BGbin_FN_min = h_in_FastNeutron -> FindBin(min);
      BGbin_FN_max = h_in_FastNeutron -> FindBin(max);
      float BG_FN = h_in_FastNeutron -> Integral(BGbin_FN_min,BGbin_FN_max);
      float BG_e_FN = BG_FN;


      BGbin_CC_min = h_in_CC -> FindBin(min);
      BGbin_CC_max = h_in_CC -> FindBin(max);
      float BG_CC = h_in_CC -> Integral(BGbin_CC_min,BGbin_CC_max);
      float BG_e_CC = BG_CC * 0.25;




      BGbin_NC_min = h_in_NC -> FindBin(min);
      BGbin_NC_max = h_in_NC -> FindBin(max);
      float BG_NC = h_in_NC -> Integral(BGbin_NC_min,BGbin_NC_max);
      float BG_e_NC = BG_NC * 0.287;


      BGbin_spallation_min = h_spa_copy -> FindBin(min);
      BGbin_spallation_max = h_spa_copy -> FindBin(max);
      float BG_spallation = h_spa_copy -> Integral(BGbin_spallation_min,BGbin_spallation_max);
      float BG_e_spallation =BG_spallation * spallation_e_per;


      BGbin_reactor_min = h_in_reactor -> FindBin(min);
      BGbin_reactor_max = h_in_reactor -> FindBin(max);
      float BG_reactor = h_in_reactor -> Integral(BGbin_reactor_min,BGbin_reactor_max);
      float BG_e_reactor = BG_reactor * 0.318;


      double E_mean = min+0.5;
      //      cerr << E_mean << endl;

      float BG_sum = BG_accidental + BG_FN + BG_CC + BG_NC + BG_spallation + BG_reactor;
      float BG_e_sum = BG_e_accidental + BG_e_FN + BG_e_CC + BG_e_NC + BG_e_spallation + BG_e_reactor;

      //      cerr << E_mean << " accidental " << BG_accidental << " FastNeutron " << BG_FN << " CC " << BG_CC << " NC " << BG_NC << " spallation " << BG_spallation << " reactor " << BG_reactor  << " sum " << BG_sum << endl;      

      double E_mean_Nu = E_mean + 0.84525;    
      double E_min_Nu = min + 0.84525;    
      double E_max_Nu = max + 0.84525;    

      ofs << E_mean_Nu << " " << E_min_Nu << " " << E_max_Nu << " " << func_candidate.linint(E_mean) << " " << func_e_wei.linint(E_mean) << " " << func_DS3.linint(E_mean) << " " << BG_sum << " " << BG_e_sum << endl;





    }











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

