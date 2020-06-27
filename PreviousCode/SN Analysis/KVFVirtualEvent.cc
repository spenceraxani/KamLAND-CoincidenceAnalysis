#include "KVFVirtualEvent.hh"
#include <string.h>

bool KVFVirtualEvent :: isBigEndian(void)
{
  union {short i; char c[2];} endian_test;
  endian_test.i=1;
  if(endian_test.c[0])
    return false;
  else if(endian_test.c[1])
    return true;
  else
    return true;
}

unsigned long long int KVFVirtualEvent :: swap(unsigned long long int value)
{
  unsigned long long int swaped;
  unsigned char* p = (unsigned char*)(&swaped);
  unsigned char* org = (unsigned char*)(&value);

  for(int i=0; i<(int)sizeof(value); i++){
    p[i] = org[sizeof(value) - i -1];
  }
  return swaped;
}


bool KVFVirtualEvent :: restore(unsigned long long int* L, const char* buf,int whence)
{
  memcpy( (void*)L, (const void*)&(buf[whence]), sizeof((*L)) );
  if( isBigEndian() ){}
  else {
    (*L) = swap((*L));
  }
  return true;
}

bool KVFVirtualEvent :: store(unsigned long long int L, char* buf, int whence)
{
  if( isBigEndian() ){}
  else {
    L = swap(L);    
  }
  memcpy( (void*)&(buf[whence]), (const void*)(&L), sizeof((L)) );
  return true;
}

