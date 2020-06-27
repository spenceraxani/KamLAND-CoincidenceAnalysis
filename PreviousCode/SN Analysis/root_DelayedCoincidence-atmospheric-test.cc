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


  // -------------------------------------------------------------------------------------------------------- //


  double efficiency_2011Paper = 0.92;



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

  const int ARRAY = 20000;
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

  for (int k = 0; k < DS1; k++){
    double E = Ep_DS1[k];

    efficiency_weighted[k] = (func_DS1.linint(E)*LT_DS1/LT_all) + (func_DS2.linint(E)*LT_DS2/LT_all) + (func_DS3.linint(E)*LT_DS3/LT_all);
    efficiency_weighted_Paper[k] = (func_DS1.linint(E)*LT_DS1/LT_2011) + (func_DS2.linint(E)*LT_DS2/LT_2011) + (func_DS3.linint(E)*LT_DS3/LT_2011);

    cout << E << " " << func_DS1.linint(E) << endl;
  }

  return 0;


//   for (int k = 0; k < DS1; k++)
//     {
//       for (int l = 0; l < DS2; l++)
// 	{
// 	  for (int m = 0; m < DS3; m++)
// 	    {
// 	      if (!(Ep_DS1[k] == Ep_DS2[l] && Ep_DS1[k] == Ep_DS3[m]))continue;

// 	      efficiency_weighted[k] = (efficiency_DS1[k]*LT_DS1/LT_all) + (efficiency_DS2[l]*LT_DS2/LT_all) + (efficiency_DS3[m]*LT_DS3/LT_all);
// 	      efficiency_weighted_Paper[k] = (efficiency_DS1[k]*LT_DS1/LT_2011) + (efficiency_DS2[l]*(LT_2011-LT_DS1)/LT_2011);

// 	      //	      cerr << Ep_DS1[k] << " " << efficiency_weighted[k] << endl; 

// 	    }
// 	}
//     }






//   char list_CC[256] = "./data-BackgroundSpectrum/CC-Polynomial.dat";

//   string line_CC;
//   ifstream flist_CC(list_CC);
//   if (!flist_CC)
//     {
//       cerr << "ERROR : don't exite such a data CC file! " << endl;
//       return 0;
//     }


//   double Ep_CC[ARRAY];
//   double N_CC [ARRAY];

//   int CC=0;
//   while(getline(flist_CC, line_CC)) 
//     {
//      istringstream iss_CC(line_CC); 
//      iss_CC  >> Ep_CC[CC] >> N_CC[CC]; 
    
//      CC++;}







//   char list_NC[256] = "./data-BackgroundSpectrum/picture-neutral-current-energy-spactrum-100MeV.dat";

//   string line_NC;
//   ifstream flist_NC(list_NC);
//   if (!flist_NC)
//     {
//       cerr << "ERROR : don't exite such a data NC file! " << endl;
//       return 0;
//     }


//   double Ep_NC[ARRAY];
//   double N_NC [ARRAY];

//   int NC=0;
//   while(getline(flist_NC, line_NC)) 
//     {
//      istringstream iss_NC(line_NC); 
//      iss_NC  >> Ep_NC[NC] >> N_NC[NC]; 
    
//      NC++;}


//   double NC_all[ARRAY];
//   double NC_DS3[ARRAY];
//   double NC_Paper[ARRAY];

//   double CC_all[ARRAY];
//   double CC_DS3[ARRAY];
//   double CC_Paper[ARRAY];

//   double Ep_CC2[ARRAY];

//   double energy_scale = (30-0.9)/(30-7.5);





//   for (int o = 0; o < DS1; o++)
//     {




// 	  for (int q = 0; q < DS3; q++)
// 	    {

// 	      for (int r = 0; r < CC; r++)
// 		{

// 		  //      cerr << Ep_DS1[o] << endl;

// 		  Ep_CC2[r] = Ep_CC[r]+0.05;


// 		  if(Ep_DS1[o] > 7.5)
// 		    {
// 		      cerr << Ep_DS1[o] << " " << Ep_CC2[r] << endl;
// 		    }

// 		  if(Ep_DS1[o] == Ep_CC2[r])
// 		    {
// 		      cerr<< "ok2 "<< endl; 
// 		      //		  cerr << Ep_DS1[o] << " " << Ep_CC[r] - 0.05 << endl;
// 		    }
// 		  if (!( Ep_DS1[o] == Ep_DS3[q] && Ep_DS1[o] == Ep_CC2[r]))continue;
		  

// 		  cerr << N_CC[r] << endl;

// 		  CC_all[o] = N_CC[r] * efficiency_weighted[o] / efficiency_2011Paper * LT_all/LT_2011;

// 		  CC_DS3[o] = N_CC[r] * efficiency_DS3[q]/efficiency_2011Paper * LT_DS3/LT_2011 * energy_scale;

// 		  CC_Paper[o] = N_CC[r] * efficiency_weighted_Paper[o]/efficiency_2011Paper;


// 		  ofs_all_CC << Ep_DS1[o] << " " << CC_all[o] << endl;
// 		  ofs_DS3_CC << Ep_DS1[o] << " " << CC_DS3[o] << endl;
// 		  ofs_Paper_CC << Ep_DS1[o] << " " << CC_Paper[o] << endl;
// 		  //		  cerr << Ep_DS1[o] << " " << CC_Paper[o] << endl;


       


// 		}


// 	      //  cerr << "ok " << endl;
// 	      for (int s = 0; s < NC; s++)
// 		{
// 		  if (!( Ep_DS1[o] == Ep_DS3[q] && Ep_DS1[o] == (Ep_NC[s]+0.05)))continue;
		  
		  
// 		  NC_all[o] = N_NC[s] * efficiency_weighted[o] / efficiency_2011Paper * LT_all/LT_2011;
		  
// 		  NC_DS3[o] = N_NC[s] * efficiency_DS3[q]/efficiency_2011Paper * LT_DS3/LT_2011 * energy_scale;
		  
// 		  NC_Paper[o] = N_NC[s] * efficiency_weighted_Paper[o]/efficiency_2011Paper;


// 		  ofs_all_NC << Ep_DS1[o] << " " << NC_all[o] << endl;
// 		  ofs_DS3_NC << Ep_DS1[o] << " " << NC_DS3[o] << endl;
// 		  ofs_Paper_NC << Ep_DS1[o] << " " << NC_Paper[o] << endl;

       



// 	    }
// 	}

//     }

















  
  return 0;
}

     


#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return root_DelayedCoincidence_atmospheric_scale();
}
#endif
