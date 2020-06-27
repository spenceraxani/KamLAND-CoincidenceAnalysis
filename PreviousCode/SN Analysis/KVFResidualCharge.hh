#ifndef __KVFResidualCharge_HH
#define __KVFResidualCharge_HH

#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <cstdlib>

using namespace std;

class KVFResidualCharge {
public:
  KVFResidualCharge(void){}
  ~KVFResidualCharge(void){}
  
  static bool setVersion(int version){_RCVersion = version;}
  static int  getVersion(void){return _RCVersion;}

  static bool changeRun(int RunNumber);

  static double dQ(double TotalCharge17, double cheL, double sciL, int RunNumber = 0);

private:
  static bool setConstant();
  
  static bool _isSetConstant;

  static int _RCVersion;
  
  static int _LastRunNumber;
  static int _TargetRunNumber;
  
  static double _cherenkov_peak;
  static double _scintillation_peak;
  static double _totalcharge17_tracklength_mean;

  static double _cherenkov_peak_base;
  static double _scintillation_peak_base;
  static double _totalcharge17_tracklength_mean_base;

  static vector<int> _Run;
  static map<int, double> *_Cherenkov_Peak;
  static map<int, double> *_Scintillation_Peak;
  static map<int, double> *_TotalCharge17_TrackLength_Mean;
};


inline double KVFResidualCharge :: dQ(double TotalCharge17, double cheL, double sciL, int RunNumber)
{
  double DeltaQ;
  
  if(RunNumber>0){
    if(_isSetConstant == false){
      
      setConstant();
      
      _isSetConstant = true;
    }
    
    changeRun(RunNumber);
    
    double ChargeScaleCorrection = pow(10, _scintillation_peak_base) / pow(10, _scintillation_peak);

    DeltaQ = TotalCharge17 - (31.45 * cheL) - (_totalcharge17_tracklength_mean * sciL);	
    
    DeltaQ = DeltaQ * ChargeScaleCorrection;

    return DeltaQ;
  }

  if(_RCVersion == 1){
    DeltaQ = TotalCharge17 - (32.45 * cheL) - (738.4 * sciL); // version1
  }
  else if(_RCVersion == 2){
    DeltaQ = TotalCharge17 - (26.47 * cheL) - (604.8 * sciL); // version2    
  }
  else if(_RCVersion == 3){
    DeltaQ = TotalCharge17 - (31.45 * cheL) - (629.4 * sciL); // version3
  }
  else{
    cerr << "ERROR : KVFResidualCharge : invalid version number" << endl;
    abort();
  }  
  
  return DeltaQ;
}

#endif
