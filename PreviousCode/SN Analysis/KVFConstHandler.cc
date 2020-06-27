#include "KVFConstHandler.hh"
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include "kuFUNC.hh"

string          KVFConstHandler :: _delta_correction_filename;
bool           KVFConstHandler :: _isSetDeltaCorrectionTable = false;
kuFUNC*        KVFConstHandler :: _DeltaCorrection;

int KVFConstHandler :: _version = 6;//default

bool KVFConstHandler :: setVersion(int v)
{
  if(v!=1 && v!=2 && v!=3 && v!=4 && v!=5 && v!=6){
    cerr << "Error: version is 1 or 2 or 3 or 4 or 5 or 6" << endl;
    return false;
  }    
  _version = v;
  return true;
}

int KVFConstHandler :: getVersion(){
  return _version;
}

float KVFConstHandler :: CorrectionOfVisibleEnergy(float visible_energy)
{
  if(_isSetDeltaCorrectionTable==false){
    _delta_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/delta_correction.dat");
    
    ifstream fdelta(_delta_correction_filename.c_str());

    if(!fdelta){
      cerr << "ERROR: Cannot open delta correction file: " << _delta_correction_filename << endl;
      cerr << "Please check environmental variable: KAMLAND_CONST_DIR = " << getenv("KAMLAND_CONST_DIR") << endl;
      abort();
    }

    double Energy[200], Correction[200];
    float energy, correction;
    int N=0;
    while(fdelta >> energy >> correction){
      Energy[N] = energy;    
      Correction[N] = correction;
      N++;
    }  
    _DeltaCorrection = new kuFUNC(N,Energy,Correction);

    _isSetDeltaCorrectionTable = true;
  }
  
  if(visible_energy<0) visible_energy = 0;
  if(visible_energy>15) visible_energy = 15;
  
  return _DeltaCorrection->splint(visible_energy);
}

