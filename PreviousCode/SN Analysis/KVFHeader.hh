#ifndef __KVFHeader_HH
#define __KVFHeader_HH

#include "KVFVirtualHeader.hh"
using namespace std;

class KVFHeader : public KVFVirtualHeader {
public:
  KVFHeader(void){
    DataType = 0;
    TimeStampNanoSec = 0;
    MajorVersion = 0;
    MinorVersion = 0;
    Size = 0;

    MicroSec = 0;
    MilliSec = 0;
    Sec = 0;
    Min = 0;
    Hour = 0;
    Day = 0;
  }
  ~KVFHeader(void){}

  bool  restoreFrom(const char* buf);
  bool  storeTo(char* buf);
  int   getSize(void){return (int)Size;}

public:
  unsigned int DataType; // 64bit
  unsigned int TimeStampNanoSec; // 64bit
  unsigned int MajorVersion; // 64bit
  unsigned int MinorVersion; // 64bit
  unsigned int Size; // 64bit

public:
  unsigned long long int MicroSec;
  unsigned long long int MilliSec;
  unsigned long long int Sec;
  unsigned long long int Min;
  unsigned long long int Hour;
  unsigned long long int Day;

  static const unsigned long long int MicroSec_FBE;
  static const unsigned long long int MilliSec_FBE;
  static const unsigned long long int Sec_FBE;
  static const unsigned long long int Min_FBE;
  static const unsigned long long int Hour_FBE;
  static const unsigned long long int Day_FBE;

  static const unsigned long long int MicroSec_MoGURA;
  static const unsigned long long int MilliSec_MoGURA;
  static const unsigned long long int Sec_MoGURA;
  static const unsigned long long int Min_MoGURA;
  static const unsigned long long int Hour_MoGURA;
  static const unsigned long long int Day_MoGURA;
};

#endif
