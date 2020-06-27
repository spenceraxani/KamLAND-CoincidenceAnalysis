#ifndef __KVFVirtualHeader_HH
#define __KVFVirtualHeader_HH

#include <unistd.h>
#include <stdlib.h>
using namespace std;

class KVFVirtualHeader {
public:
  KVFVirtualHeader(void){
    FormatVersion = 0;
    FormatSize = 0;
  }
  virtual ~KVFVirtualHeader(void){}

  virtual bool restoreFrom(const char* buf) = 0;
  virtual bool storeTo(char* buf) = 0;
  virtual int  getSize(void) = 0;
  
  static bool isBigEndian(void);
  enum {DefaultBufSize = 64};

public:
  unsigned int FormatVersion; // 64bit
  unsigned int FormatSize; // 64bit

  int          Offset;

protected:
  static bool restore(unsigned long long int* L, const char* buf, int whence);
  static bool store(unsigned long long int L, char* buf, int whence);
  static unsigned long long int swap(unsigned long long int value);

};

#endif
