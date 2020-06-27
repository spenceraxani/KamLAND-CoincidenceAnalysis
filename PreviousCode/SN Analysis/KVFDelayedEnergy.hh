#ifndef __KVFDelayedEnergy_HH
#define __KVFDelayedEnergy_HH

#include <string>
using namespace std;

class KVFDelayedEnergy {
public:
  KVFDelayedEnergy(void){}
  ~KVFDelayedEnergy(void){}

  /////////////////////////////////////////////////////////////////////////
  //
  // -----  Correction for delayed energy  ------
  //
  // Previous event (within ~100 usec) makes energy bias for delayed event 
  // due to the missing waveform effect or electrical noise.
  // Delayed energies should be corrected in order to avoid the 
  // inefficiency in the anti-neutrino delayed coincidence selection.
  // The Correction table is based on the study of AmBe calibration.
  //
  // prompt energy : 
  //  Ep < 20.0 MeV (for geo + reactor anti-neutrino) ---> Ed correction 
  //  Ep > 20.0 MeV ---> no Ed correction
  //
  /////////////////////////////////////////////////////////////////////////
  static double DelayedEnergy(double RawDelayedEnergy, double Ep, double Rd, double dT)
  {return RawDelayedEnergy / CorrectionFactor(Ep, Rd, dT);}
  
  static double CorrectionFactor(double Ep, double Rd, double dT);

private:
  static bool SetTable();

  static bool _isSetTable;
  static string* filename;
  static double** Offset;
  static double** Slope;
  static int index_Rd;
  static int index_dT;

  static double* dT_lower;
  static double* dT_upper;

  static double* Rd_lower;
  static double* Rd_upper;
};
#endif
