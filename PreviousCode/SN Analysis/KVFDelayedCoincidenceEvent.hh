#ifndef __KVFDelayedCoincidenceEvent_HH
#define __KVFDelayedCoincidenceEvent_HH

#include "KVFGeneralEvent.hh"
using namespace std;

class KVFDelayedCoincidenceEvent : public KVFVirtualEvent {
public:
  KVFDelayedCoincidenceEvent(void){
    Prompt.disableDefaultEnergyCorrection();
    Prompt.disablePositionDependentEnergyCorrection();
    Delayed.disableDefaultEnergyCorrection();
    Delayed.disablePositionDependentEnergyCorrection();

    Prompt.header_off();    
    Delayed.header_off();    

    _enable_header = false;
  }
  virtual ~KVFDelayedCoincidenceEvent(void){}
  
  bool restoreFrom(const char* buf);
  bool storeTo(char* buf);
  int  getBufSize(void);

  bool  isHeader(void){return _enable_header;}

public:
  short             RunNumber;
  KVFGeneralEvent Prompt;
  KVFGeneralEvent Delayed;

  int               FirstEventNumberOfNearMuon;
  unsigned int      NofNearMuon;
  unsigned int      Veto[6];

  bool _enable_header;
};

#endif
