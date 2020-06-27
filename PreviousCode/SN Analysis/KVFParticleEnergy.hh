#ifndef __KVFParticleEnergy_HH
#define __KVFParticleEnergy_HH

#include <math.h>
#include <fstream>
using namespace std;


class KVFSpline{
public:
    KVFSpline(int na, const double* v,const double* w);
    ~KVFSpline(){delete [] x;delete [] y;delete [] y2;};
    double interpolation(double t);
private:
    double *x,*y,*y2;
    int n,klo,khi;
};



class KVFVParticleEnergy {
public:
  KVFVParticleEnergy(void);
  ~KVFVParticleEnergy(void){}

  double Gamma(double visible_energy);
  double Electron(double visible_energy);
  double Positron(double visible_energy);

  double GammaToVisible(double energy);
  double ElectronToVisible(double energy);
  double PositronToVisible(double energy);

  void setVersionTagName(string name){_VersionTagName = name;}
  string getVersionTagName(){return _VersionTagName;}

  void enablePositronExtrapolate(){_PositronExtrapolate = true;}
  void disablePositronExtrapolate(){_PositronExtrapolate = false;}
private:
  bool SetTable();

  KVFSpline* _gamma;
  KVFSpline* _electron;
  KVFSpline* _positron;
  KVFSpline* _my;

  KVFSpline* _from_gamma;
  KVFSpline* _from_electron;
  KVFSpline* _from_positron;

  bool _isSetTable;

  ifstream*  _file_Gamma;
  ifstream*  _file_Electron;
  ifstream*  _file_Positron;

  string     _filename_Gamma1;
  string     _filename_Electron1;
  string     _filename_Positron1;

  string     _filename_Gamma2;
  string     _filename_Electron2;
  string     _filename_Positron2;

  string     _filename_Gamma3;
  string     _filename_Electron3;
  string     _filename_Positron3;
  string     _filename_Positron_Extrapolate3;

  string     _filename_Gamma_tag;
  string     _filename_Electron_tag;
  string     _filename_Positron_tag;
  string     _filename_Positron_Extrapolate_tag;

  int _N_Gamma;
  double* _Ereal_Gamma;
  double* _InverseRatio_Gamma;
  double* _Ratio_Gamma;
  double* _Evis_Gamma;

  int _N_Electron;
  double* _Ereal_Electron;
  double* _InverseRatio_Electron;
  double* _Ratio_Electron;
  double* _Evis_Electron;

  int _N_Positron;
  double* _Ereal_Positron;
  double* _InverseRatio_Positron;
  double* _Ratio_Positron;
  double* _Evis_Positron;

  string  _VersionTagName;
  bool    _PositronExtrapolate;

  enum {MaxData = 20};
};

////////////////////////////////////////////////////////////////
inline double KVFSpline::interpolation(double t)
{
    if(t<x[klo])klo=0;
    if(t>x[khi])khi=n-1;
    if(t<x[klo] || t>x[khi])return 0.0;
    while(khi-klo>1){
       int k=(khi+klo)>>1;
       if(x[k]>t)khi=k;else klo=k;
    }
    double h=x[khi]-x[klo];
    double a=(x[khi]-t)/h;
    double b=(t-x[klo])/h;
    return (a*y[klo]+b*y[khi]+((a*a*a-a)*y2[klo]+(b*b*b-b)*y2[khi])*h*h/6.0);
}


class KVFParticleEnergy {
public:
  KVFParticleEnergy(void);
  ~KVFParticleEnergy(void){}
  
  static void SetVParticleEnergy(void);

  static void Kat(void){energy_estimator = 0;}
  static void A2(void){energy_estimator = 1;}

  static void PositronExtrapolate(void){positron_extrapolate = 1;}

  static double Gamma(double visible_energy, int RunNumber = 0);
  static double Electron(double visible_energy, int RunNumber = 0);
  static double Positron(double visible_energy, int RunNumber = 0);

  static double GammaToVisible(double energy, int RunNumber = 0);
  static double ElectronToVisible(double energy, int RunNumber = 0);
  static double PositronToVisible(double energy, int RunNumber = 0);

  static bool _IsSetVParticleEnergy;
  
  static int energy_estimator;
  static int positron_extrapolate;
  
  static KVFVParticleEnergy* particle_energy_0;
  static KVFVParticleEnergy* particle_energy_1; // before purification
  static KVFVParticleEnergy* particle_energy_2; // after purification
};

#endif
