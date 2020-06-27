#ifndef __KVFEnergyCorrection_HH
#define __KVFEnergyCorrection_HH

#include <vector>
#include <string>
#include "kuFUNC.hh"

using namespace std;

class KVFEnergyCorrection
{
public:
  KVFEnergyCorrection();
  ~KVFEnergyCorrection(){}
  // you can select the version of the energy correction
  // 0: Not applying the energy correction
  // 1: Old energy correction
  // 2: New energy correction (recommend)
  static bool setVersion(int v);
  static int  getVersion();

  static float RunDependenceOfVisibleEnergy(int run_number, bool IsK40Corr, bool IsA2, float R);
  static float ZPositionDependenceOfVisibleEnergy(int run_number, bool IsA2, float R, float z);
  static float OffAxisPositionDependenceOfVisibleEnergy(int run_number, bool IsA2, float R, float Rho);
  static float CorrectionOf20inchEnergy(int run_number, float visible_energy, bool IsA2);

private:
  static float _K40RunDependentEnergyCorrection(int run_number, vector<int>* _run_number, vector<double>* _energy_correction_factor);
  static float _NpRunDependentEnergyCorrection(int run_number, vector<int>* _run_number, vector<double>* _energy_correction_factor);
  static float _NpRunDependentEnergyCorrection(int run_number, float r, vector<int>* _run_number, vector<double>* _energy_correction_factor_in, vector<double>* _energy_correction_factor_out);
  static float _ZPositionDependentEnergyCorrection(int run_number, float z, vector<int>* _run_number, vector<double>* _bin, vector< vector<double> >* _energy_correction_factor);
  static float _CoZPositionDependentEnergyCorrection(int run_number, float z, vector< vector<double> >* _energy_correction_factor);
  static float _OffAxisPositionDependentEnergyCorrection(int run_number, float off_axis, vector<int>* _run_number, vector<double>* _bin, vector< vector<double> >* _energy_correction_factor);
  static float _20inchEnergyCorrection(float visible_energy, double* _factor, kuFUNC* _energy_correction_fuction);

  static double* _factor;
  static double* _factorA2;
 
  static vector<int> _run_number_k40_time0;
  static vector<int> _run_number_k40_time0_A2;
  static vector<int> _run_number_k40_time;
  static vector<int> _run_number_k40_z;
  static vector<int> _run_number_k40_z2;
  static vector<int> _run_number_k40_r;
  static vector<int> _run_number_k40_r2;
  static vector<int> _run_number_co_z;
  static vector<int> _run_number_np_z2;
  static vector<int> _run_number_np_rho;
  static vector<int> _run_number_np_rho2;
  static vector<int> _run_number_np_time;
  static vector<int> _run_number_np_time2;

  static vector<double> _bin_k40_z;
  static vector<double> _bin_k40_z2;
  static vector<double> _bin_k40_r;
  static vector<double> _bin_k40_r2;
  static vector< vector<double> >  _bin_co_z;
  static vector<double> _bin_np_z2;
  static vector<double> _bin_np_rho;
  static vector<double> _bin_np_rho2;

  static vector<double> _energy_correction_factor_k40_time0;
  static vector<double> _energy_correction_factor_k40_time0_A2;
  static vector<double> _energy_correction_factor_k40_time;
  static vector< vector<double> >  _energy_correction_factor_k40_z;
  static vector< vector<double> >  _energy_correction_factor_k40_z2;
  static vector< vector<double> >  _energy_correction_factor_k40_r;
  static vector< vector<double> >  _energy_correction_factor_k40_r2;
  static vector< vector<double> >  _energy_correction_factor_co_z;
  static vector< vector<double> >  _energy_correction_factor_co_z_A2;
  static vector< vector<double> >  _energy_correction_factor_np_z2;
  static vector< vector<double> >  _energy_correction_factor_np_z2_A2;
  static vector< vector<double> >  _energy_correction_factor_np_rho;
  static vector< vector<double> >  _energy_correction_factor_np_rho_A2;
  static vector< vector<double> >  _energy_correction_factor_np_rho2;
  static vector< vector<double> >  _energy_correction_factor_np_rho2_A2;
  static vector<double>  _energy_correction_factor_np_time_in;
  static vector<double>  _energy_correction_factor_np_time_in_A2;
  static vector<double>  _energy_correction_factor_np_time_out;
  static vector<double>  _energy_correction_factor_np_time_in2;
  static vector<double>  _energy_correction_factor_np_time_out2;

  static kuFUNC* _20inchCorrection;
  static kuFUNC* _20inchCorrectionA2;

  static int _version;  
  static bool _IsSetTable;
};

#endif
