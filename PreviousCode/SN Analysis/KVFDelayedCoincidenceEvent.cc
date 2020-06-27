#include "KVFDelayedCoincidenceEvent.hh"
#include "KVFConstHandler.hh"
#include <string.h>
#include <iostream>

int KVFDelayedCoincidenceEvent :: getBufSize(void){
  if(KVFConstHandler::getVersion()==1 || KVFConstHandler::getVersion()==2) return 40*2+32;
  else if(KVFConstHandler::getVersion()==3 || KVFConstHandler::getVersion()==4) return 56*2+32;
  else if(KVFConstHandler::getVersion()==5) return 80*2+32;
  else if(KVFConstHandler::getVersion()==6) return 96*2+32;
  else {
    cerr << "Error: no version set" << endl;
    abort();
  }
  return 96*2+32;
}

bool KVFDelayedCoincidenceEvent :: restoreFrom(const char* buf)
{
  int offset=0;
  unsigned long long int L = 0;
  if(Prompt.restoreFrom(buf)==false) return false;
  offset += Prompt.getBufSize();
  if(Delayed.restoreFrom(buf, offset)==false) return false;
  offset += Delayed.getBufSize();
  if(Prompt.RunNumber!=Delayed.RunNumber) return false;
  RunNumber=Prompt.RunNumber;

  for(int i=0; i<6; i+=2){
    restore(&L, buf, offset);
    offset += sizeof(L);
    Veto[i] = (unsigned int)( (L >> 32) & 0x0FFFFFFFF );
    Veto[i+1] = (unsigned int)( L & 0x0FFFFFFFF );
  }
  restore(&L, buf, offset);
  offset += sizeof(L);
  FirstEventNumberOfNearMuon = (int)( L & 0x0FFFFFFFF );
  NofNearMuon = (unsigned int)( (L >> 32) & 0x0FFFFFFFF );

  return true;
}

bool KVFDelayedCoincidenceEvent :: storeTo(char* buf)
{
  if(Prompt.RunNumber!=Delayed.RunNumber) return false;
  //if(RunNumber!=Prompt.RunNumber) return false;

  int offset=0;
  unsigned long long int L = 0;
  if(Prompt.storeTo(buf)==false) return false;
  offset += Prompt.getBufSize();
  if(Delayed.storeTo(buf, offset)==false) return false;
  offset += Delayed.getBufSize();

  unsigned long long int tmpUL;
  for(int i=0; i<6; i+=2){
    tmpUL = Veto[i];
    L = ( (tmpUL << 32) | Veto[i+1] );
    store(L, buf, offset);
    offset += sizeof(L);
  }
  tmpUL = NofNearMuon;
  L = ( (tmpUL << 32) | FirstEventNumberOfNearMuon );
  store(L, buf, offset);
  offset += sizeof(L);

  return true;
}
