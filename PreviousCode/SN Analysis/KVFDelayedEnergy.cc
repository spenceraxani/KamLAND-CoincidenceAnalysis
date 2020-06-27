#include "KVFDelayedEnergy.hh"
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

bool KVFDelayedEnergy :: _isSetTable = false;

string* KVFDelayedEnergy :: filename = new string[7];
double** KVFDelayedEnergy :: Offset = new double*[7];
double** KVFDelayedEnergy :: Slope = new double*[7];
int KVFDelayedEnergy :: index_Rd;
int KVFDelayedEnergy :: index_dT;

bool KVFDelayedEnergy :: SetTable()
{
  if(_isSetTable==true) return true;
  
  // set file name
  filename[0] = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/DelayedEnergy/data-AmBe/run003587.dat"); // center
  filename[1] = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/DelayedEnergy/data-AmBe/run003601.dat"); // -1.0 m
  filename[2] = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/DelayedEnergy/data-AmBe/run003588.dat"); // -3.0 m
  filename[3] = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/DelayedEnergy/data-AmBe/run003600.dat"); // -4.0 m
  filename[4] = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/DelayedEnergy/data-AmBe/run003599.dat"); // -4.5 m
  filename[5] = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/DelayedEnergy/data-AmBe/run003598.dat"); // -5.0 m
  filename[6] = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/DelayedEnergy/data-AmBe/run003597.dat"); // -5.5 m
  
  for(int i=0;i<7;i++){
    //cerr << "read file : " << filename[i] << endl;

    ifstream fin(filename[i].c_str());

    if(!fin){
      cerr << "KVFDelayedEnergy :: Cannot open input file" << endl;
      return false;
    }

    index_Rd = i;
    Offset[index_Rd] = new double[5];
    Slope[index_Rd] = new double[5];

    int index;
    double offset;
    double slope;
    
    while(fin >> index >> offset >> slope){
      
      index_dT = index;
      Offset[index_Rd][index_dT] = offset;
      Slope[index_Rd][index_dT] = slope;

      //cerr << index_dT << " " << Offset[index_Rd][index_dT] << " " << Slope[index_Rd][index_dT] << endl;
    }

  }

  _isSetTable = true;
 
  return true;
}


double KVFDelayedEnergy :: CorrectionFactor(double Ep, double Rd, double dT)
{
  if(Ep>20.0){
    return 1.0; // no Ed correction
  }
  
  if(!SetTable()){
    cerr << "KVFDelayedEnergy :: Cannot set correction table ! " << endl;
    abort();
  }
  
  // set index_dT
  static const double dT_lower[] = {  0.5,  15.0,  30.0,  50.0, 100.0};
  static const double dT_upper[] = { 15.0,  30.0,  50.0, 100.0, 1000.0};

  index_dT = -1;

  for(int i=0;i<5;i++){
    if(dT>=dT_lower[i] && dT<dT_upper[i]){ 
      index_dT = i;
    }
  }
  if(index_dT<0) return 1.0;

  if(index_dT==4) return 1.0;

  
  // set index_Rd
  static const double Rd_lower[] = {  0.0, 100.0, 300.0, 400.0, 450.0, 500.0, 550.0};
  static const double Rd_upper[] = {100.0, 300.0, 400.0, 450.0, 500.0, 550.0, 860.0};
  
  int index_Rd_lower = -1;
  int index_Rd_upper = -1;
  double r;
  
  for(int i=0;i<7;i++){
    if(Rd>=Rd_lower[i] && Rd<Rd_upper[i]){ 
      if(i<6){
	index_Rd_lower = i;
	r = (Rd - Rd_lower[i]) / (Rd_upper[i] - Rd_lower[i]); 
      }
      else{ // extraporate
	index_Rd_lower = i-1;
	r = (Rd - Rd_lower[i-1]) / (Rd_upper[i-1] - Rd_lower[i-1]); 
      }
    }
  }
  if(index_Rd_lower<0) return 1.0;

  index_Rd_upper = index_Rd_lower + 1;

  
  // set correction for index_Rd_upper
  double Ed_upper_4        = Offset[index_Rd_upper][4]        + Slope[index_Rd_upper][4]        * Ep;
  double Ed_upper_index_dT = Offset[index_Rd_upper][index_dT] + Slope[index_Rd_upper][index_dT] * Ep;
  
  double CorrectionFactor_upper = Ed_upper_index_dT / Ed_upper_4;
  
  // set correction for index_Rd_lower
  double Ed_lower_4        = Offset[index_Rd_lower][4]        + Slope[index_Rd_lower][4]        * Ep;
  double Ed_lower_index_dT = Offset[index_Rd_lower][index_dT] + Slope[index_Rd_lower][index_dT] * Ep;
  
  double CorrectionFactor_lower = Ed_lower_index_dT / Ed_lower_4;
  
  // linear interporation or extraporation
  double CorrectionFactor = (1.0 - r) * CorrectionFactor_lower + r * CorrectionFactor_upper;
  
  return CorrectionFactor;
}
