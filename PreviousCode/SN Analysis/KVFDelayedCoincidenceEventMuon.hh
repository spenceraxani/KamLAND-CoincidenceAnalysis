#ifndef __KVFDelayedCoincidenceEventMuon_HH
#define __KVFDelayedCoincidenceEventMuon_HH

#include "KVFGeneralEvent.hh"
using namespace std;

class KVFDelayedCoincidenceEventMuon : public KVFVirtualEvent {
public:
  KVFDelayedCoincidenceEventMuon(void){
    Prompt.disableDefaultEnergyCorrection();
    Prompt.disablePositionDependentEnergyCorrection();
    Delayed.disableDefaultEnergyCorrection();
    Delayed.disablePositionDependentEnergyCorrection();

    Prompt.header_off();    
    Delayed.header_off();    
    Muon.header_off();    
 
    _enable_header = false;
  }
  virtual ~KVFDelayedCoincidenceEventMuon(void){}
  
  bool restoreFrom(const char* buf);
  bool storeTo(char* buf);
  int  getBufSize(void){return Prompt.getBufSize() + Delayed.getBufSize()
			  + Muon.getBufSize();}

  bool  isHeader(void){return _enable_header;}

public:
  short           RunNumber;
  KVFGeneralEvent Prompt;
  KVFGeneralEvent Delayed;
  KVFGeneralEvent Muon;

  bool _enable_header;
};

#endif
