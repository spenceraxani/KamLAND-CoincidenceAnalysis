#include "KVFSpallationEvent.hh"
#include <string.h>
#include <iostream>

bool KVFSpallationEvent :: restoreFrom(const char* buf)
{
  int offset=0;
  //unsigned long long int L = 0;
  if(Muon.restoreFrom(buf)==false) return false;
  offset += Muon.getBufSize();
  if(Product.restoreFrom(buf, offset)==false) return false;
  //offset += Delayed.getBufSize();
  if(Muon.RunNumber!=Product.RunNumber) return false;

  RunNumber=Muon.RunNumber;
  TimeStampDiff = Product.TimeStamp - Muon.TimeStamp;
  Distance = Muon.getDistanceFrom( Product.X, Product.Y, Product.Z );

  return true;
}

bool KVFSpallationEvent :: storeTo(char* buf)
{
  if(Muon.RunNumber!=Product.RunNumber) return false;

  int offset=0;
  //unsigned long long int L = 0;
  if(Muon.storeTo(buf)==false) return false;
  offset += Muon.getBufSize();
  if(Product.storeTo(buf, offset)==false) return false;
  //offset += Product.getBufSize();

  return true;
}
