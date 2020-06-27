#ifndef __KVFVirtualEvent_HH
#define __KVFVirtualEvent_HH

#include <unistd.h>
#include <stdlib.h>
#include "KVFHeader.hh"
using namespace std;

class KVFVirtualEvent {
public:
  KVFVirtualEvent(void){Header = 0;}
  virtual ~KVFVirtualEvent(void){}
  
  virtual bool restoreFrom(const char* buf) = 0;
  virtual bool storeTo(char* buf) = 0;
  virtual int  getBufSize(void) = 0;

  virtual bool isHeader(void) = 0;

  static bool isBigEndian(void);
  enum {DefaultBufSize = 64};

public:
  KVFHeader*  Header;

protected:
  static bool restore(unsigned long long int* L, const char* buf, int whence);
  static bool store(unsigned long long int L, char* buf, int whence);
  static unsigned long long int swap(unsigned long long int value);

};

#endif
