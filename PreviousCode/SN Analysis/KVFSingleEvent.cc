#include "KVFSingleEvent.hh"
#include <string.h>
#include <iostream>

bool KVFSingleEvent :: restoreFrom(const char* buf)
{
  int offset=0;
  unsigned long long int L = 0;
  if(Prompt.restoreFrom(buf)==false) return false;
  offset += Prompt.getBufSize();
  RunNumber=Prompt.RunNumber;

  for(int i=0; i<8; i+=2){
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

bool KVFSingleEvent :: storeTo(char* buf)
{
  int offset=0;
  unsigned long long int L = 0;
  if(Prompt.storeTo(buf)==false) return false;
  offset += Prompt.getBufSize();

  unsigned long long int tmpUL;
  for(int i=0; i<8; i+=2){
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
