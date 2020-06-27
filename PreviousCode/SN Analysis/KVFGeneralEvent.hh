#ifndef __KVFGeneralEvent_HH
#define __KVFGeneralEvent_HH

#include "KVFVirtualEvent.hh"
#include "KVFHeader.hh"
#include "KVFConstHandler.hh"
#include "KVFEnergyCorrection.hh"
using namespace std;

class KVFGeneralEvent : public KVFVirtualEvent {
public:
//   KVFGeneralEvent(int header_on = 1){
  KVFGeneralEvent(void){
    _enable_run_dependent_energy_correction = true;
    _enable_energy_vertex_correction = true;
    _enable_z_position_dependent_energy_correction = false;
    _enable_offaxis_position_dependent_energy_correction = false;
    _enable_new_run_dependent_energy_correction = false;
    _enable_20inch_energy_correction = false;
    KVFEnergyCorrection energy_correction;

    _enable_header = true;

//     _enable_header = false;

//     if(header_on == 1){
//       _enable_header = true;
//     }
  }
  virtual ~KVFGeneralEvent(void){}

  void header_off(){_enable_header = false;}
  
  bool  restoreFrom(const char* buf){return restoreFrom(buf, 0);}
  bool  storeTo(char* buf){return storeTo(buf, 0);}
  int   getBufSize(void);
  //int   getBufSize(void){return (int)Header->getSize();}

  bool  isHeader(void){return _enable_header;}

  bool  restoreFrom(const char* buf, int offset);
  bool  storeTo(char* buf, int offset);
  float getDistanceFrom(float x, float y, float z);
  void  getTrackLength(float *sciL, float *cheL);
  bool isNoise(void);
  bool isThisMuon_NoTimeWindowForODhit(int NhitOD_Cut = 5, float TotalCharge17_Cut1 = 500.0, float TotalCharge17_Cut2 = 10000.0);
  bool isThisMuon(int NhitOD_Cut = 5, float TotalCharge17_Cut1 = 500.0, float TotalCharge17_Cut2 = 10000.0);
  bool isThisFlasher(void);

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
  
  void disableDefaultEnergyCorrection(){
    _enable_energy_vertex_correction = false;    
    _enable_run_dependent_energy_correction = false;
  }

  void enablePositionDependentEnergyCorrection(){
    _enable_z_position_dependent_energy_correction = true;
    _enable_offaxis_position_dependent_energy_correction = true;
    _enable_new_run_dependent_energy_correction = true;    
    _enable_20inch_energy_correction = true;  
  }

  void disablePositionDependentEnergyCorrection(){
    _enable_z_position_dependent_energy_correction = false;
    _enable_offaxis_position_dependent_energy_correction = false;
    _enable_new_run_dependent_energy_correction = false;    
    _enable_20inch_energy_correction = false;  
  }

  void enableZPositionDependentEnergyCorrection(){
    _enable_z_position_dependent_energy_correction = true;
  }

  void enableOffAxisPositionDependentEnergyCorrection(){
    _enable_offaxis_position_dependent_energy_correction = true;
  }

  void enable20inchEnergyCorrection(){
    _enable_20inch_energy_correction = true;
  }

  void disable20inchEnergyCorrection(){
    _enable_20inch_energy_correction = false;
  }

  static bool setECorrMode(bool mode){
    _ECorr_mode = mode;
    return true;
  }
  static int  getECorrMode(){
    return _ECorr_mode;
  }
  
public:
  // 8byte 64bit
  short                  RunNumber; // 16bit
  unsigned long long int TimeStamp; // 48bit
  
  // 8byte 64bit
  unsigned int           UnixTime; // 32bit
  int                    EventNumber; // 32bit
  
  // 8byte 62bit
  unsigned int           Nhit5inch; // 3bit
  unsigned int           NhitOD; // 8bit
  unsigned int           NhitID; // 11bit
  int                    EventStatus; // 4bit
  unsigned long long int TriggerType; // 36bit
  
  // 8byte 62bit
  float                  EntranceX; // 11bit
  float                  EntranceY; // 11bit
  float                  EntranceZ; // 11bit
  float                  TotalChargeID; // 21bit
  unsigned int           N200OD;    // 8bit
  
  // 8byte 62bit
  unsigned int           Badness; // 14bit
  float                  DirectionX; // 12bit
  float                  DirectionY; // 12bit
  float                  DirectionZ; // 12bit
  float                  VertexBadness; //12bit
  
  // 8byte  63bit
  float  TotalCharge5inch; // 21bit
  float  TotalChargeOD; // 21bit
  float  MaxChargeID;     // 21bit
  
  // 8byte  63bit
  unsigned int    N100; // 11bit
  float   TotalCharge17; // 21bit
  float   TrackSigmaT; // 12bit
  int     NsumMax;     // 11bit
  unsigned int           NhitODFromMuonFitter; // 8bit
  // 8byte  63bit
  float  VertexSigmaT; //12bit
  float  VertexChi2Q; //12bit
  float  VertexChi2T; //12bit
  int    VertexNdf; //6bit
  float  Energy17; // 21bit

  // 8byte  61bit
  float  X; // 20bit
  float  Y; // 20bit
  float  Z; // 20bit
  int    GivenUp; // 1bit
 
  // 8byte  64bit
  float  Energy; // 21bit
  float  EnergyWithNsumMax; // 21bit
  unsigned int    Nhit17; // 11bit
  int             NeighborCharge; //11bit
  float Xx , Yy , Zz;

  // 8byte  60bit
  float X_Kat; // 20bit
  float Y_Kat; // 20bit
  float Z_Kat; // 20bit

  // 8byte  63bit
  float V2VertexTimeBadness; // 12bit
  int   V2Niterations; // 7bit
  int   V2StoppedBecause; // 2bit
  float EnergyA2; // 21bit
  float Energy17A2; // 21bit

  float  Energy17Corr; // 21bit
  float  EnergyCorr; // 21bit
  float  EnergyA2Corr; // 21bit
  float  Energy17A2Corr; // 21bit

  // 8byte 44bit
  unsigned int           MicroSec; // 20bit
  unsigned int           NanoSec; // 10bit
  short                  SubRunNumber; // 14bit

private:
  bool _enable_run_dependent_energy_correction;
  bool _enable_energy_vertex_correction;
  bool _enable_z_position_dependent_energy_correction;
  bool _enable_offaxis_position_dependent_energy_correction;
  bool _enable_new_run_dependent_energy_correction;
  bool _enable_20inch_energy_correction;
  static bool _ECorr_mode;

  bool _enable_header;
};

#endif
