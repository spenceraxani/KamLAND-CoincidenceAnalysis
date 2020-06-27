#ifndef __KVFLowEnergyEvent_HH
#define __KVFLowEnergyEvent_HH

#include "KVFVirtualEvent.hh"
using namespace std;

class KVFLowEnergyEvent : public KVFVirtualEvent {
public:
  KVFLowEnergyEvent(void)
  {
    _enable_run_dependent_energy_correction = true;
    _enable_energy_vertex_correction = true;
  }
  virtual ~KVFLowEnergyEvent(void){}
  
  bool restoreFrom(const char* buf){
    return restoreFrom(buf, 0);
  }
  bool storeTo(char* buf){
    return storeTo(buf, 0);
  }

  bool restoreFrom(const char* buf, int offset);
  bool storeTo(char* buf, int offset);
  int  getBufSize(void);
  bool isNoise(void);

  void enableEnergyVertexCorrection(){
    _enable_energy_vertex_correction = true;
  }
  void disableEnergyVertexCorrection(){
    _enable_energy_vertex_correction = false;
  }

  void enableRunDependentEnergyCorrection(){
    _enable_run_dependent_energy_correction = true;
  }
  void disableRunDependentEnergyCorrection(){
    _enable_run_dependent_energy_correction = false;
  }

public:
  // 8byte
  short                  RunNumber; // 16bit
  unsigned long long int TimeStamp; // 48bit

  // 8byte
  unsigned int           UnixTime; // 32bit
  int                    EventNumber; // 32bit

  // 8byte
  int                    PID; // 4bit
  float                  Energy; // 20bit
  int                    EventStatus; // 4bit
  unsigned long long int TriggerType; // 36bit

  // 8byte
  float  TotalChargeID; // 32bit
  float  TotalChargeOD; // 21bit
  int    NsumMax; // 11bit

  // 8byte
  float  X; // 11bit
  float  Y; // 11bit
  float  Z; // 11bit
  unsigned int    N100; // 11bit
  unsigned int    NhitID; // 11bit
  unsigned int    NhitOD; // 8bit

  // 8byte
  float  VertexSigmaT; //12bit
  float  VertexChi2Q; //12bit
  float  VertexChi2T; //12bit
  int    VertexNdf; //6bit
  float  Energy17; // 20bit

  // 8byte
  float  Xx; // 7bit
  float  Yy; // 7bit
  float  Zz; // 7bit
  float  EnergyWithNsumMax; // 20bit
  float  VertexBadness; //12bit
  unsigned int    Nhit17; // 11bit

private:
  bool _enable_run_dependent_energy_correction;
  bool _enable_energy_vertex_correction;
};

#endif
