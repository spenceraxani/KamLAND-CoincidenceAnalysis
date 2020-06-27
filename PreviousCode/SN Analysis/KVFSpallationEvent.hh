#ifndef __KVFSpallationEvent_HH
#define __KVFSpallationEvent_HH

#include "KVFLowEnergyEvent.hh"
#include "KVFMuonEvent.hh"
using namespace std;

class KVFSpallationEvent : public KVFVirtualEvent {
public:
  KVFSpallationEvent(void){
    Product.disableRunDependentEnergyCorrection();
  }
  virtual ~KVFSpallationEvent(void){}
  
  bool restoreFrom(const char* buf);
  bool storeTo(char* buf);
  int  getBufSize(void){return (Product.getBufSize() + Muon.getBufSize());}

public:
  KVFLowEnergyEvent Product;
  KVFMuonEvent      Muon;

  // follows are not recorded into a file
  short                RunNumber;
  signed long long int TimeStampDiff;
  double               Distance;
};

#endif
