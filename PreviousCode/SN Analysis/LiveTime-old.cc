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

Int_t LiveTime()
{

  //---------------------------------------------------------------------//
  // set file mame



  char list_file_name1[256] = "/home/yukiko/work/Solar-Anti-Neutrino/vector-file-Kat/table/livetime-dQ-6.0m.dat-13-03-11";

  char list_file_name2[256] = "/home/yukiko/work/Solar-Anti-Neutrino/vector-file-Kat/table/LiveTimeRun.dat";


  //  string ofs_file_name = "/home/yukiko/work/calibration/vector-file/table/4pi_Calibration_new_shita_between_pin_and_main.dat"; 
  //---------------------------------------------------------------------//


  //----------output-----------//
  //  ofstream ofs(ofs_file_name.c_str());



  //reading file

  const int ARRAY_lt = 20000;
  int index_lt = 0;
  int Run_lt[ARRAY_lt];
    double RunTime[ARRAY_lt], VetoTime[ARRAY_lt], LiveTime[ARRAY_lt], Ratio[ARRAY_lt], DeadTime[ARRAY_lt];


  string line;
  ifstream data(list_file_name1);
  if(!data)
    {
      cerr << "ERROR : don't exite such a data list file! " << endl;
    }

  while(getline(data, line))
  {
    istringstream iss(line);
    iss >> Run_lt[index_lt] >> RunTime[index_lt]>> VetoTime[index_lt] >> LiveTime[index_lt] >> Ratio[index_lt] >> DeadTime[index_lt] ;
    index_lt++;
  }

  int NumberOfRunsInFile1 = index_lt;




  

  ifstream flist(list_file_name2);
  if (!flist)
    {
      cerr << "ERROR : don't exite such a data file2! " << endl;
      return 0;
    }
  int Run2;

  double LT = 0;

  while(flist >> Run2 ) 
    {


      //-----file1-----------
      bool IsTargetFound = false;
      int runindex_lt;

      for (runindex_lt = 0; runindex_lt < NumberOfRunsInFile1; runindex_lt++)
	{
	if (Run2 == Run_lt[runindex_lt]) 
	  {
	  IsTargetFound = true;
	  break;
	  }
	}
      if (!IsTargetFound) {
	cerr << "#ERROR: Target run was not found: " << Run2 << endl;
      }


      //      if (11000 <= Run2)
      if (9626 >= Run2)
	{

      LT = LT + LiveTime[runindex_lt];
	}
    }
  cerr << LT << endl;

}

int main()
{
  TROOT root("root", "root");
  return LiveTime();
}
