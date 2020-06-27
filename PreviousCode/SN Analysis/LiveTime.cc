#include "KVF.hh"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "TROOT.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"

using namespace std;

int main(int argc, char* argv[])
{

  //--- File Name --------------------------------------//

  // dcvf list
  char dcvf_list_input_filename[256]  = "./analysis_list/dcvf-Kat.list-2013-179-11947";

  // constant table <FIX>
  char trigger_filename[256]          = "./table/trigger_conditions_11947.dat";
  char run_info_filename[256]         = "./table/run-info-11947.table";
  
  char list_file_name[256] = "./livetime/livetime-6.0m.dat";

  // output file
  string ofs_file_name = "/home/yukiko/work/Solar-Anti-Neutrino/vector-file-Kat/table/LiveTime.dat"; 


  //---------------------------------------------------//


  ofstream ofs_lt(ofs_file_name.c_str());



  // read constant table & set filename
  //
  // ### trigger conditiosn
  ifstream trigger(trigger_filename);
  if(!trigger){
    cerr<<"ERROR: daq-log.dat is not exist."<<endl;
    abort();
  }
  
  // ### run info table
  string RunInfoFile  = run_info_filename;
  if (KVFRunInfo::ReadTable(RunInfoFile.c_str()) == false) {
    cerr << "Cannot set run info table" << endl;
    cerr << "Please check environmental variable: KAMLAND_CONST_DIR = " << getenv("KAMLAND_CONST_DIR") << endl;
    abort();
  }
  
  // ### read input list
  string ListFile  = dcvf_list_input_filename;
  


  //## read LiveTime table

  const int ARRAY_lt = 20000;
  int index_lt = 0;
  int Run_lt[ARRAY_lt];
    double RunTime[ARRAY_lt], VetoTime[ARRAY_lt], LiveTime[ARRAY_lt], Ratio[ARRAY_lt], DeadTime[ARRAY_lt];


  string line;
  ifstream data(list_file_name);
  if(!data)
    {
      cerr << "ERROR : don't exite such a data list file! " << endl;
    }

  while(getline(data, line))
  {
    istringstream iss_lt(line);
    iss_lt >> Run_lt[index_lt] >> RunTime[index_lt]>> VetoTime[index_lt] >> LiveTime[index_lt] >> Ratio[index_lt] >> DeadTime[index_lt] ;
    //    cerr << Run_lt[index_lt] << endl;

    index_lt++;

    

  }

  int NumberOfRunsInFile = index_lt;

      double LT_2011 = 0;
      double LT_11947 = 0;
      double LT_P1 = 0;
      double LT_P2 = 0;
      double LT_P3 = 0;




  //----------------//

  char file[256];
  int runnum;
  ifstream flist(ListFile.c_str());
  if (!flist) {
    cerr << "ERROR : don't exite such a list! -> " << dcvf_list_input_filename << endl;
    exit(8);
  }
  while (flist >> runnum >> file) {

    //   cerr << file << endl;
    int RunNumber = runnum;
    
    if ( KVFRunInfo::HasReactorInfo(RunNumber) == false ) continue;    
    int run_grade = KVFRunInfo::Grade(RunNumber);
    if (run_grade < 0 || run_grade >= 10) continue;

   
    KVFReader dcf(file);



    if (dcf.bad()) {
      cerr << "ERROR: Cannot open file run" << RunNumber << endl;
      dcf.close();
      continue;
    }
    


     //----- Live Time -----------
      bool IsTargetFound = false;
      int runindex_lt;

      for (runindex_lt = 0; runindex_lt < NumberOfRunsInFile; runindex_lt++)
	{
	if (RunNumber == Run_lt[runindex_lt]) 
	  {
	  IsTargetFound = true;
	  break;
	  }
	}
      if (!IsTargetFound) {
	cerr << "#ERROR: Target run was not found: " << Run_lt << " : " << RunNumber<< endl;

      }

      

      for (int i = 0; i < NumberOfRunsInFile ; i++)
	{
	  if (!(runnum == Run_lt[i]))continue;

	  if (runnum < 179 )continue;
	  if (6820 < runnum && runnum < 6890)continue;
	  if (7930 < runnum && runnum < 8070)continue;
	  if (10675 < runnum && runnum < 11000 )continue;


	  if (runnum <= 9626)
	    {
	      LT_2011 = LT_2011 + LiveTime[i];
	      
	    }

	  LT_11947 = LT_11947 + LiveTime[i];

	  if (179 <= runnum && runnum <= 6820 )
	    {
	      LT_P1 = LT_P1 + LiveTime[i];
	    }

	  if (6890 <= runnum && runnum <= 10675 )
	    {
	      LT_P2 = LT_P2 + LiveTime[i];
	    }

	  
	  if (runnum >= 11000 )
	    {
	      LT_P3 = LT_P3 + LiveTime[i];
	    }

	  //     cerr << "2011 : " << LT_2011 << " -11947 : " << LT_11947 << " Period3 : " << LT_P3 << endl;

	}
      

  }

  cerr << "2011 : " << LT_2011 << " -11947 : " << LT_11947 << " Period1 : " << LT_P1 << " Period2 : " << LT_P2 << " Period3 : " << LT_P3 << endl;

      ofs_lt << LT_2011 << LT_11947 << LT_P3 << endl;




  return 0;
}
