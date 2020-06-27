#include "KVFParticleEnergyKat.hh"
#include "KVFConstHandler.hh"
#include <string.h>
#include <iostream>



KVFSplineKat::KVFSplineKat(int na,const double* v,const double* w)
{
    n=na;
    x=new double[n];
    y=new double[n];
    y2=new double[n];
    double *u=new double[n];
    memcpy(x,v,sizeof(double)*n);
    memcpy(y,w,sizeof(double)*n);
    y2[0]=u[0]=0.;
    for(int i=1;i<n-1;i++){
       double sig=(x[i]-x[i-1])/(x[i+1]-x[i-1]);
       double p=sig*y2[i-1]+2.0;
       y2[i]=(sig-1.0)/p;
       u[i]=(y[i+1]-y[i])/(x[i+1]-x[i])-(y[i]-y[i-1])/(x[i]-x[i-1]);
       u[i]=(6.0*u[i]/(x[i+1]-x[i-1])-sig*u[i-1])/p;
    }
    y2[n-1]=0.0;
    for(int k=n-2;k>=0;k--)y2[k]=y2[k]*y2[k+1]+u[k];
    delete [] u;
    klo=0;khi=1;
}

///////////////////////////////////////////////////////////////////
KVFSplineKat* KVFParticleEnergyKat :: _gamma = 0;
KVFSplineKat* KVFParticleEnergyKat :: _electron = 0;
KVFSplineKat* KVFParticleEnergyKat :: _positron = 0;

KVFSplineKat* KVFParticleEnergyKat :: _from_gamma = 0;
KVFSplineKat* KVFParticleEnergyKat :: _from_electron = 0;
KVFSplineKat* KVFParticleEnergyKat :: _from_positron = 0;

bool KVFParticleEnergyKat :: _isSetTable = false;

ifstream*  KVFParticleEnergyKat :: _file_Gamma = 0;
ifstream*  KVFParticleEnergyKat :: _file_Electron = 0;
ifstream*  KVFParticleEnergyKat :: _file_Positron = 0;

string     KVFParticleEnergyKat :: _filename_Gamma1;
string     KVFParticleEnergyKat :: _filename_Electron1;
string     KVFParticleEnergyKat :: _filename_Positron1;

string     KVFParticleEnergyKat :: _filename_Gamma2;
string     KVFParticleEnergyKat :: _filename_Electron2;
string     KVFParticleEnergyKat :: _filename_Positron2;

string     KVFParticleEnergyKat :: _filename_Gamma3;
string     KVFParticleEnergyKat :: _filename_Electron3;
string     KVFParticleEnergyKat :: _filename_Positron3;

// ///Version1
// const char*  KVFParticleEnergyKat :: _filename_Gamma1 = "/kamland/const/vf-new/ParticleEnergy-Version1/Gamma.table";
// const char*  KVFParticleEnergyKat :: _filename_Electron1 = "/kamland/const/vf-new/ParticleEnergy-Version1/Electron.table";
// const char*  KVFParticleEnergyKat :: _filename_Positron1 = "/kamland/const/vf-new/ParticleEnergy-Version1/Positron.table";

// ///Version2
// const char*  KVFParticleEnergyKat :: _filename_Gamma2 = "/kamland/const/vf-new/ParticleEnergy-Version2/Gamma.table";
// const char*  KVFParticleEnergyKat :: _filename_Electron2 = "/kamland/const/vf-new/ParticleEnergy-Version2/Electron.table";
// const char*  KVFParticleEnergyKat :: _filename_Positron2 = "/kamland/const/vf-new/ParticleEnergy-Version2/Positron.table";

// ///Version3 or 4
// const char*  KVFParticleEnergyKat :: _filename_Gamma3 = "/kamland/const/vf-new/ParticleEnergy/Gamma.table";
// const char*  KVFParticleEnergyKat :: _filename_Electron3 = "/kamland/const/vf-new/ParticleEnergy/Electron.table";
// const char*  KVFParticleEnergyKat :: _filename_Positron3 = "/kamland/const/vf-new/ParticleEnergy/Positron.table";

int      KVFParticleEnergyKat :: _N_Gamma = 0;
double*  KVFParticleEnergyKat :: _Ereal_Gamma = 0;
double*  KVFParticleEnergyKat :: _InverseRatio_Gamma = 0;
double*  KVFParticleEnergyKat :: _Ratio_Gamma = 0;
double*  KVFParticleEnergyKat ::  _Evis_Gamma = 0;

int      KVFParticleEnergyKat :: _N_Electron = 0;
double*  KVFParticleEnergyKat :: _Ereal_Electron = 0;
double*  KVFParticleEnergyKat :: _InverseRatio_Electron = 0;
double*  KVFParticleEnergyKat :: _Ratio_Electron = 0;
double*  KVFParticleEnergyKat :: _Evis_Electron = 0;

int      KVFParticleEnergyKat :: _N_Positron = 0;
double*  KVFParticleEnergyKat :: _Ereal_Positron = 0;
double*  KVFParticleEnergyKat :: _InverseRatio_Positron = 0;
double*  KVFParticleEnergyKat :: _Ratio_Positron = 0;
double*  KVFParticleEnergyKat :: _Evis_Positron = 0;


bool KVFParticleEnergyKat :: SetTable(){

  if(_isSetTable==true) return true;

  ///Version 1
  _filename_Gamma1 = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/ParticleEnergy-Version1/Gamma.table");
  _filename_Electron1 = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/ParticleEnergy-Version1/Electron.table");
  _filename_Positron1 = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/ParticleEnergy-Version1/Positron.table");

  ///Version 2
  _filename_Gamma2 = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/ParticleEnergy-Version2/Gamma.table");
  _filename_Electron2 = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/ParticleEnergy-Version2/Electron.table");
  _filename_Positron2 = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/ParticleEnergy-Version2/Positron.table");

  ///Version 3 or 4
  _filename_Gamma3 = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/ParticleEnergy/Gamma.table");
  _filename_Electron3 = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/ParticleEnergy/Electron.table");
  _filename_Positron3 = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/ParticleEnergy/Positron.table");
  
  if(KVFConstHandler::getVersion() == 1){
    _file_Gamma = new ifstream(_filename_Gamma1.c_str());
    _file_Electron = new ifstream(_filename_Electron1.c_str());
    _file_Positron = new ifstream(_filename_Positron1.c_str());
  }
  else if(KVFConstHandler::getVersion() == 2){
    _file_Gamma = new ifstream(_filename_Gamma2.c_str());
    _file_Electron = new ifstream(_filename_Electron2.c_str());
    _file_Positron = new ifstream(_filename_Positron2.c_str());
  }
  else if(KVFConstHandler::getVersion() == 3 || KVFConstHandler::getVersion() == 4 || KVFConstHandler::getVersion() == 5 || KVFConstHandler::getVersion() == 6){
    //else if(KVFConstHandler::getVersion() == 3 || KVFConstHandler::getVersion() == 4 || KVFConstHandler::getVersion() == 5){
    _file_Gamma = new ifstream(_filename_Gamma3.c_str());
    _file_Electron = new ifstream(_filename_Electron3.c_str());
    _file_Positron = new ifstream(_filename_Positron3.c_str());
  }
  else {
    cerr << "Error: no version set" << endl;
    abort();
  }
    
  if((!*_file_Gamma) || (!*_file_Electron) || (!*_file_Positron)){
    cerr << "ERROR: Cannot open particle energy file: ";
    cerr << _file_Gamma << endl;
    cerr << _file_Electron << endl;
    cerr << _file_Positron << endl;
    cerr << "Please check environmental variable: KAMLAND_CONST_DIR = " << getenv("KAMLAND_CONST_DIR") << endl;
    abort();
  }

  double dummy;

  ///Gamma
  if(_Ereal_Gamma == 0) _Ereal_Gamma = new double [MaxData];
  if(_InverseRatio_Gamma == 0) _InverseRatio_Gamma = new double [MaxData]; 
  if(_Ratio_Gamma == 0) _Ratio_Gamma = new double [MaxData];
  if(_Evis_Gamma == 0) _Evis_Gamma = new double [MaxData];

  _N_Gamma=0;
  while(*_file_Gamma >> _Ereal_Gamma[_N_Gamma] >> _InverseRatio_Gamma[_N_Gamma] >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy){
    _N_Gamma++;
  }
  
  for(int i=0; i<_N_Gamma; i++){
    _Evis_Gamma[i] = _InverseRatio_Gamma[i]*_Ereal_Gamma[i];
    _Ratio_Gamma[i] = _Ereal_Gamma[i]/_Evis_Gamma[i];
  }

  _gamma = new KVFSplineKat(_N_Gamma, _Evis_Gamma, _Ratio_Gamma);
  _from_gamma = new KVFSplineKat(_N_Gamma, _Ereal_Gamma, _InverseRatio_Gamma);

  ///Electron
  if(_Ereal_Electron == 0) _Ereal_Electron = new double [MaxData];
  if(_InverseRatio_Electron == 0) _InverseRatio_Electron = new double [MaxData]; 
  if(_Ratio_Electron == 0) _Ratio_Electron = new double [MaxData];
  if(_Evis_Electron == 0) _Evis_Electron = new double [MaxData];

  _N_Electron=0;
  while(*_file_Electron >> _Ereal_Electron[_N_Electron] >> _InverseRatio_Electron[_N_Electron] >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy){
    _N_Electron++;
  }
  
  for(int i=0; i<_N_Electron; i++){
    _Evis_Electron[i] = _InverseRatio_Electron[i]*_Ereal_Electron[i];
    _Ratio_Electron[i] = _Ereal_Electron[i]/_Evis_Electron[i];
  }

  _electron = new KVFSplineKat(_N_Electron, _Evis_Electron, _Ratio_Electron);
  _from_electron = new KVFSplineKat(_N_Electron, _Ereal_Electron, _InverseRatio_Electron);

  ///Positron
  if(_Ereal_Positron == 0) _Ereal_Positron = new double [MaxData];
  if(_InverseRatio_Positron == 0) _InverseRatio_Positron = new double [MaxData]; 
  if(_Ratio_Positron == 0) _Ratio_Positron = new double [MaxData];
  if(_Evis_Positron == 0) _Evis_Positron = new double [MaxData];

  _N_Positron=0;
  if(*_file_Positron >> _Ereal_Positron[_N_Positron] >> _InverseRatio_Positron[_N_Positron] >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy){}
  while(*_file_Positron >> _Ereal_Positron[_N_Positron] >> _InverseRatio_Positron[_N_Positron] >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy){
    _N_Positron++;
  }
  
  for(int i=0; i<_N_Positron; i++){
    _Evis_Positron[i] = _InverseRatio_Positron[i]*_Ereal_Positron[i];
    _Ratio_Positron[i] = _Ereal_Positron[i]/_Evis_Positron[i];
  }

  _positron = new KVFSplineKat(_N_Positron, _Evis_Positron, _Ratio_Positron);
  _from_positron = new KVFSplineKat(_N_Positron, _Ereal_Positron, _InverseRatio_Positron);

  _isSetTable = true;

  return true;
}

double KVFParticleEnergyKat :: Gamma(double visible_energy)
{
  if(!SetTable()){
    cerr << "KVFParticleEnergyKat :: Cannot Set Data ! " << endl;
    abort();
  }

  if(visible_energy<=_Evis_Gamma[0]) return visible_energy*_Ratio_Gamma[0];
  else if(visible_energy>=_Evis_Gamma[_N_Gamma-1]) return visible_energy*_Ratio_Gamma[_N_Gamma-1];
  
  return ( visible_energy * (_gamma->interpolation(visible_energy)) );
}

double KVFParticleEnergyKat :: Electron(double visible_energy)
{
  if(!SetTable()){
    cerr << "KVFParticleEnergyKat :: Cannot Set Data ! " << endl;
    abort();
  }

  if(visible_energy<=_Evis_Electron[0]) return visible_energy*_Ratio_Electron[0];
  else if(visible_energy>=_Evis_Electron[_N_Electron-1]) return visible_energy*_Ratio_Electron[_N_Electron-1];
  
  return ( visible_energy * (_electron->interpolation(visible_energy)) );
}

double KVFParticleEnergyKat :: Positron(double visible_energy)
{
  if(!SetTable()){
    cerr << "KVFParticleEnergyKat :: Cannot Set Data ! " << endl;
    abort();
  }

  if(visible_energy<=_Evis_Positron[0]) return visible_energy*_Ratio_Positron[0];
  else if(visible_energy>=_Evis_Positron[_N_Positron-1]) return visible_energy*_Ratio_Positron[_N_Positron-1];
  
  return ( visible_energy * (_positron->interpolation(visible_energy)) );
}


double KVFParticleEnergyKat :: GammaToVisible(double energy)
{
  if(!SetTable()){
    cerr << "KVFParticleEnergyKat :: Cannot Set Data ! " << endl;
    abort();
  }

  if(energy<=_Ereal_Gamma[0]) return energy*_InverseRatio_Gamma[0];
  else if(energy>=_Ereal_Gamma[_N_Gamma-1]) return energy*_InverseRatio_Gamma[_N_Gamma-1];

  return ( energy * (_from_gamma->interpolation(energy)) );
}

double KVFParticleEnergyKat :: ElectronToVisible(double energy)
{
  if(!SetTable()){
    cerr << "KVFParticleEnergyKat :: Cannot Set Data ! " << endl;
    abort();
  }

  if(energy<=_Ereal_Electron[0]) return energy*_InverseRatio_Electron[0];
  else if(energy>=_Ereal_Electron[_N_Electron-1]) return energy*_InverseRatio_Electron[_N_Electron-1];

  return ( energy * (_from_electron->interpolation(energy)) );
}

double KVFParticleEnergyKat :: PositronToVisible(double energy)
{
  if(!SetTable()){
    cerr << "KVFParticleEnergyKat :: Cannot Set Data ! " << endl;
    abort();
  }

  if(energy<=_Ereal_Positron[0]) return energy*_InverseRatio_Positron[0];
  else if(energy>=_Ereal_Positron[_N_Positron-1]) return energy*_InverseRatio_Positron[_N_Positron-1];

  return ( energy * (_from_positron->interpolation(energy)) );
}

