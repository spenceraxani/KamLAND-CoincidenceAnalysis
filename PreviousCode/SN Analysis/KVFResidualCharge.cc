#include "KVFResidualCharge.hh"
#include <fstream>
#include <iostream>
#include <cstdlib>


bool KVFResidualCharge :: _isSetConstant = false;

int KVFResidualCharge :: _RCVersion = 3; // default

int KVFResidualCharge :: _LastRunNumber = 0;
int KVFResidualCharge :: _TargetRunNumber = 0;

double KVFResidualCharge :: _cherenkov_peak;
double KVFResidualCharge :: _scintillation_peak;
double KVFResidualCharge :: _totalcharge17_tracklength_mean;

double KVFResidualCharge :: _cherenkov_peak_base;
double KVFResidualCharge :: _scintillation_peak_base;
double KVFResidualCharge :: _totalcharge17_tracklength_mean_base;

vector<int> KVFResidualCharge :: _Run;
map<int, double>* KVFResidualCharge :: _Cherenkov_Peak = new map<int, double>;
map<int, double>* KVFResidualCharge :: _Scintillation_Peak = new map<int, double>;
map<int, double>* KVFResidualCharge :: _TotalCharge17_TrackLength_Mean = new map<int, double>;


bool KVFResidualCharge :: changeRun(int RunNumber)
{
  if(_LastRunNumber == RunNumber) return true;

  _LastRunNumber = RunNumber;
  
  // set target run number
  _TargetRunNumber = RunNumber; 
  
  while(1){
    if(_TargetRunNumber <= _Run[0]){
      _TargetRunNumber = _Run[0];
      break;
    }
    
    if(_Scintillation_Peak->find(_TargetRunNumber) != _Scintillation_Peak->end()) break;
    
    _TargetRunNumber--;
  }

  // set parameters
  const int _run_base = 700;

  if(_Scintillation_Peak->find(_run_base) != _Scintillation_Peak->end()){   
    _cherenkov_peak_base = (*_Cherenkov_Peak)[_run_base];
    _scintillation_peak_base = (*_Scintillation_Peak)[_run_base];
    _totalcharge17_tracklength_mean_base = (*_TotalCharge17_TrackLength_Mean)[_run_base];
  }
  else{
    cerr << "ERROR : KVFResidualCharge : Cannot set _scintillation_peak_base" << endl;
    abort();
  }

  _cherenkov_peak = (*_Cherenkov_Peak)[_TargetRunNumber];
  _scintillation_peak = (*_Scintillation_Peak)[_TargetRunNumber];
  _totalcharge17_tracklength_mean = (*_TotalCharge17_TrackLength_Mean)[_TargetRunNumber];

  if(_scintillation_peak < (_scintillation_peak_base * 0.4) || _scintillation_peak >= (_scintillation_peak_base * 1.4)){
    cerr << "ERROR : ResidualCharge : _scintillation_peak is out of range -> " << _scintillation_peak << endl;
    abort();
  }
  
  if(_totalcharge17_tracklength_mean < (_totalcharge17_tracklength_mean_base * 0.4) || _totalcharge17_tracklength_mean >= (_totalcharge17_tracklength_mean_base * 1.4)){
    cerr << "ERROR : ResidualCharge : _totalcharge17_tracklength_mean is out of range -> " << _totalcharge17_tracklength_mean << endl;
    abort();
  }
  
  return true;
}


bool KVFResidualCharge :: setConstant()
{
  string muon_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/ResidualCharge/Muon-ChargePerTrackLength-LS.dat");

  ifstream fMuon(muon_filename.c_str());

  if(!fMuon){
    cerr << "ERROR : ResidualCharge : Cannot open input file -> " << muon_filename << endl;
    abort();
  }

  int run;
  double _cherenkov_peak;
  double _scintillation_peak;
  double _totalcharge17_tracklength_mean;

  while(fMuon >> run >> _cherenkov_peak >> _scintillation_peak >> _totalcharge17_tracklength_mean){
    _Run.push_back(run);
    _Cherenkov_Peak->insert( make_pair((int)run,(double)_cherenkov_peak) );
    _Scintillation_Peak->insert( make_pair((int)run,(double)_scintillation_peak) );
    _TotalCharge17_TrackLength_Mean->insert( make_pair((int)run,(double)_totalcharge17_tracklength_mean) );
  }

  return true;
}
