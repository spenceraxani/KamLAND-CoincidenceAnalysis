#ifndef __KVFSingleEvent_HH
#define __KVFSingleEvent_HH

#include "KVFGeneralEvent.hh"
#include <iostream>
using namespace std;

class KVFSingleEvent : public KVFVirtualEvent {
public:
  KVFSingleEvent(void){
    Prompt.disableRunDependentEnergyCorrection();
    Prompt.disableEnergyVertexCorrection();
  }
  virtual ~KVFSingleEvent(void){}
  
  bool restoreFrom(const char* buf);
  bool storeTo(char* buf);
  int  getBufSize(void){return Prompt.getBufSize()+40;}
  
public:
  short             RunNumber;

  // 80byte
  KVFGeneralEvent Prompt;
  
  // 8byte
  int               FirstEventNumberOfNearMuon;
  unsigned int      NofNearMuon;

  // 4 x 8byte
  unsigned int      Veto[8];
};

#endif
