#ifndef __KVFParticleEnergyKat_HH
#define __KVFParticleEnergyKat_HH

#include <math.h>
#include <fstream>
using namespace std;


class KVFSplineKat{
public:
    KVFSplineKat(int na, const double* v,const double* w);
    ~KVFSplineKat(){delete [] x;delete [] y;delete [] y2;};
    double interpolation(double t);
private:
    double *x,*y,*y2;
    int n,klo,khi;
};



class KVFParticleEnergyKat {
public:
  KVFParticleEnergyKat(void){}
  ~KVFParticleEnergyKat(void){}

  static double Gamma(double visible_energy);
  static double Electron(double visible_energy);
  static double Positron(double visible_energy);

  static double GammaToVisible(double energy);
  static double ElectronToVisible(double energy);
  static double PositronToVisible(double energy);
private:
  static bool SetTable();

  static KVFSplineKat* _gamma;
  static KVFSplineKat* _electron;
  static KVFSplineKat* _positron;
  static KVFSplineKat* _my;

  static KVFSplineKat* _from_gamma;
  static KVFSplineKat* _from_electron;
  static KVFSplineKat* _from_positron;

  static bool _isSetTable;

  static ifstream*  _file_Gamma;
  static ifstream*  _file_Electron;
  static ifstream*  _file_Positron;

  static string     _filename_Gamma1;
  static string     _filename_Electron1;
  static string     _filename_Positron1;

  static string     _filename_Gamma2;
  static string     _filename_Electron2;
  static string     _filename_Positron2;

  static string     _filename_Gamma3;
  static string     _filename_Electron3;
  static string     _filename_Positron3;

  static int _N_Gamma;
  static double* _Ereal_Gamma;
  static double* _InverseRatio_Gamma;
  static double* _Ratio_Gamma;
  static double* _Evis_Gamma;

  static int _N_Electron;
  static double* _Ereal_Electron;
  static double* _InverseRatio_Electron;
  static double* _Ratio_Electron;
  static double* _Evis_Electron;

  static int _N_Positron;
  static double* _Ereal_Positron;
  static double* _InverseRatio_Positron;
  static double* _Ratio_Positron;
  static double* _Evis_Positron;

  enum {MaxData = 20};
};

////////////////////////////////////////////////////////////////
inline double KVFSplineKat::interpolation(double t)
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

#endif
