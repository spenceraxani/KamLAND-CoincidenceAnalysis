#include "KVFDelayedCoincidenceEventMuon.hh"
#include <string.h>
#include <iostream>

bool KVFDelayedCoincidenceEventMuon :: restoreFrom(const char* buf)
{
  int offset=0;
  unsigned long long int L = 0;
  if(Prompt.restoreFrom(buf)==false) return false;
  offset += Prompt.getBufSize();
  if(Delayed.restoreFrom(buf, offset)==false) return false;
  offset += Delayed.getBufSize();
  if(Muon.restoreFrom(buf, offset)==false) return false;
  offset += Muon.getBufSize();
  if(Prompt.RunNumber!=Delayed.RunNumber) return false;
  if(Prompt.RunNumber!=Muon.RunNumber) return false;
  RunNumber=Prompt.RunNumber;

  return true;
}

bool KVFDelayedCoincidenceEventMuon :: storeTo(char* buf)
{
  if(Prompt.RunNumber!=Delayed.RunNumber) return false;
  if(Prompt.RunNumber!=Muon.RunNumber) return false;

  int offset=0;
  unsigned long long int L = 0;
  if(Prompt.storeTo(buf)==false) return false;
  offset += Prompt.getBufSize();
  if(Delayed.storeTo(buf, offset)==false) return false;
  offset += Delayed.getBufSize();
  if(Muon.storeTo(buf, offset)==false) return false;
  offset += Muon.getBufSize();

  return true;
}
