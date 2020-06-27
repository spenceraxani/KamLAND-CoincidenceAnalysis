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

Int_t efficiency()
{
  // -------------------------------------------------------------------------------------------------------- //

  string ofs_file_name1 = "./table/SelectionEfficiency-WeightedAverage.dat-DS1-bin";
  string ofs_file_name2 =  "./table/SelectionEfficiency-WeightedAverage.dat-DS2-bin";
  string ofs_file_name3 = "./table/SelectionEfficiency-WeightedAverage.dat-DS3-bin";


  // -------------------------------------------------------------------------------------------------------- //




  //----------Output-----------//
  ofstream ofs_DS1(ofs_file_name1.c_str());
  ofstream ofs_DS2(ofs_file_name2.c_str());
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


  double Ep_DS2[ARRAY];
  double efficiency_DS2 [ARRAY];

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


  double Ep_DS3[ARRAY];
  double efficiency_DS3 [ARRAY];

  int DS3=0;
  while(getline(flist_efficiency_DS3, line_efficiency_DS3)) 
    {
     istringstream iss_DS3(line_efficiency_DS3); 
     iss_DS3  >> Ep_DS3[DS3] >> efficiency_DS3[DS3]; 
    
     DS3++;}


  double Ep_l[ARRAY];
  double bin_DS1[ARRAY];
  double bin_DS2[ARRAY];
  double bin_DS3[ARRAY];

  for (int q = 0; q < DS1*10; q++)
    {    
      Ep_l[q] = 0;
      bin_DS1[q] = 0;
      bin_DS2[q] = 0;
      bin_DS3[q] = 0;
    }






  for (int r = 0; r < DS1; r++)
    {

      cerr << Ep_DS1[r] << " " << efficiency_DS1[r]<< endl;

      for (int p = 1; p < DS1*10; p++)
	{     
	  Ep_l[p] = Ep_l[p-1] + 0.05;
	  
	  if (Ep_l[p] > 15)break;
	  
	  if (Ep_DS1[r] <= Ep_l[p] && Ep_DS1[r+1])
	    {
	      bin_DS1[p] == efficiency_DS1[r];
	      ofs_DS1 << Ep_l[p] << " " << bin_DS1[p]<< endl; 
	    }
	  
	  
	  if (Ep_DS2[r] <= Ep_l[p] && Ep_DS2[r+1])
	    {
	      bin_DS2[p] == efficiency_DS2[r];
	      ofs_DS2 << Ep_l[p] << " " << bin_DS2[p]<< endl; 
	    }
	  
	  
	  if (Ep_DS3[r] <= Ep_l[p] && Ep_DS3[r+1])
	    {
	      bin_DS3[p] == efficiency_DS3[r];
	      ofs_DS3 << Ep_l[p] << " " << bin_DS3[p]<< endl; 
	    }
	}
      

	  
    }











  
  return 0;
}

     


#ifndef __CINT__
int main(int argc, char** argv) {
  TROOT root("root", "root");
  return efficiency();
}
#endif
