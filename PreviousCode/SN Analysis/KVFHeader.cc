#include "KVFHeader.hh"
#include <string.h>
#include <math.h>

const unsigned long long int KVFHeader :: MicroSec_FBE = atoll("40");
const unsigned long long int KVFHeader :: MilliSec_FBE = atoll("40000");
const unsigned long long int KVFHeader :: Sec_FBE      = atoll("40000000");
const unsigned long long int KVFHeader :: Min_FBE      = atoll("2400000000");
const unsigned long long int KVFHeader :: Hour_FBE     = atoll("144000000000");
const unsigned long long int KVFHeader :: Day_FBE      = atoll("3456000000000");

const unsigned long long int KVFHeader :: MicroSec_MoGURA = atoll("50");
const unsigned long long int KVFHeader :: MilliSec_MoGURA = atoll("50000");
const unsigned long long int KVFHeader :: Sec_MoGURA      = atoll("50000000");
const unsigned long long int KVFHeader :: Min_MoGURA      = atoll("3000000000");
const unsigned long long int KVFHeader :: Hour_MoGURA     = atoll("180000000000");
const unsigned long long int KVFHeader :: Day_MoGURA      = atoll("4320000000000");

bool KVFHeader :: restoreFrom(const char* buf)
{
  unsigned long long int L = 0;
  Offset = 0;

  restore(&L, buf, Offset);
  Offset += sizeof(L);
  FormatVersion = (unsigned int)( L & 0x0FFFF );

  restore(&L, buf, Offset);
  Offset += sizeof(L);
  FormatSize = (unsigned int)( L & 0x0FFFF );

  if(FormatVersion == 1){
    restore(&L, buf, Offset);
    Offset += sizeof(L);
    DataType = (unsigned int)( L & 0x0FFFF );

    if(DataType==1){ // fbe
      MicroSec = MicroSec_FBE;
      MilliSec = MilliSec_FBE;
      Sec      = Sec_FBE;
      Min      = Min_FBE;
      Hour     = Hour_FBE;
      Day      = Day_FBE;
    }
    else if(DataType==2){ // mogura
      MicroSec = MicroSec_MoGURA;
      MilliSec = MilliSec_MoGURA;
      Sec      = Sec_MoGURA;
      Min      = Min_MoGURA;
      Hour     = Hour_MoGURA;
      Day      = Day_MoGURA;
    }

    restore(&L, buf, Offset);
    Offset += sizeof(L);
    TimeStampNanoSec = (unsigned int)( L & 0x0FFFF );
    
    restore(&L, buf, Offset);
    Offset += sizeof(L);
    MajorVersion = (unsigned int)( L & 0x0FFFF );

    restore(&L, buf, Offset);
    Offset += sizeof(L);
    MinorVersion = (unsigned int)( L & 0x0FFFF );

    restore(&L, buf, Offset);
    Offset += sizeof(L);
    Size = (unsigned int)( L & 0x0FFFF );
  }

  return true;
}


bool KVFHeader :: storeTo(char* buf)
{
  unsigned long long int L = 0;  
  Offset = 0;

  L = FormatVersion;
  store(L, buf, Offset);
  Offset += sizeof(L);

  L = FormatSize;
  store(L, buf, Offset);
  Offset += sizeof(L);

  if(FormatVersion == 1){
    L = DataType;
    store(L, buf, Offset);
    Offset += sizeof(L);

    L = TimeStampNanoSec;
    store(L, buf, Offset);
    Offset += sizeof(L);

    L = MajorVersion;
    store(L, buf, Offset);
    Offset += sizeof(L);

    L = MinorVersion;
    store(L, buf, Offset);
    Offset += sizeof(L);

    L = Size;
    store(L, buf, Offset);
    Offset += sizeof(L);
  }

  return true;
}
