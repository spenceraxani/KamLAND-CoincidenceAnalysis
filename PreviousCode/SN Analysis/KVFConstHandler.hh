#ifndef __KVFConstHandler_HH
#define __KVFConstHandler_HH

#include <vector>
#include <string>
#include "kuFUNC.hh"
using namespace std;

class KVFConstHandler {
public:
  KVFConstHandler(){}
  ~KVFConstHandler(){}
  // Before you read event, you have to set version
  // 1: first result
  // 2: update 03-07-28
  // 3: update 03-10-09
  // 4: second result
  // 5: update ( default )
  // 6: update 09-03-05 (V2 version 1.04)
  static bool setVersion(int v);
  static int  getVersion();
  static float CorrectionOfVisibleEnergy(float visible_energy);
  static bool resetTable();
private:
  static string         _delta_correction_filename;
  static bool           _isSetDeltaCorrectionTable;
  static kuFUNC*        _DeltaCorrection;
  static int _version;
};

#endif
