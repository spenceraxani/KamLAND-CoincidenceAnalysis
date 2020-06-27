#include "KVFLowEnergyEvent.hh"
#include "KVFConstHandler.hh"
#include <string.h>
#include <iostream>

int KVFLowEnergyEvent :: getBufSize(void){
  if(KVFConstHandler::getVersion()==1 || KVFConstHandler::getVersion()==2) return 40;
  else if(KVFConstHandler::getVersion()==3 || KVFConstHandler::getVersion()==4) return 56;
  else {
    cerr << "Error: no version set" << endl;
    abort();
  }
  
  return 56;
}

bool KVFLowEnergyEvent :: restoreFrom(const char* buf, int offset)
{
  //int offset=0;
  unsigned long long int L = 0;

  restore(&L, buf, offset);
  offset += sizeof(L);
  RunNumber = (short)( (L >> 48) & 0x0FFFF );
  unsigned long long int HigherTimeStamp 
    = (unsigned long long int)( (L >> 32) & 0x0FFFF );
  unsigned long long int LowerTimeStamp 
    = (unsigned long long int)( L & 0x0FFFFFFFF );
  TimeStamp = (HigherTimeStamp << 32) + LowerTimeStamp;

  restore(&L, buf, offset);
  offset += sizeof(L);
  EventNumber = (int)( L & 0x0FFFFFFFF );
  UnixTime = (unsigned int)( (L >> 32) & 0x0FFFFFFFF );
  
  restore(&L, buf, offset);
  offset += sizeof(L);
  PID = (int)( (L>>60) & 0x0F );
  Energy = (float)( ( (unsigned int)( (L >> 40) & 0x0FFFFF ) )/5000.0 );
  //  EventStatus = (int)( (L >> 40) & 0x0F );
  EventStatus = (int)( (L >> 36) & 0x0F );
  unsigned long long int HigherTriggerType = (unsigned int)( (L >> 32) & 0x0F );
  TriggerType = ( (HigherTriggerType << 32) | ( L & 0x0FFFFFFFF ) );
  
  restore(&L, buf, offset);
  offset += sizeof(L);
  //TotalChargeOD = (float)( (L >> 32) & 0x0FFFFFFFF )/100.0;
  TotalChargeOD = (float)( (L >> 43) & 0x001FFFFF );
  NsumMax = (int)( (L >> 32) & 0x007FF );
  TotalChargeID = (float)( L & 0x0FFFFFFFF )/100.0;

  restore(&L, buf, offset);
  offset += sizeof(L);
  X = (float)( (int)((L >> 52) & 0x07FF) - 1024);
  Y = (float)( (int)((L >> 41) & 0x07FF) - 1024);
  Z = (float)( (int)((L >> 30) & 0x07FF) - 1024);
  N100 = (unsigned int)( (L >> 19) & 0x07FF );
  NhitID = (unsigned int)( (L >> 8) & 0x07FF );
  NhitOD = (unsigned int)( L  & 0x0FF );

  if(KVFConstHandler::getVersion()==3 || KVFConstHandler::getVersion()==4){
    restore(&L, buf, offset);
    offset += sizeof(L);
    VertexSigmaT = (float)( (L >> 52) & 0x0FFF )/100.0;    
    VertexChi2Q = (float)( (L >> 40) & 0x0FFF )/100.0;    
    VertexChi2T = (float)( (L >> 28) & 0x0FFF )/100.0;    
    VertexNdf = (int)( (L >> 22) & 0x3F );    
    Energy17 = (float)( ( (unsigned int)( (L >> 2) & 0x0FFFFF ) )/5000.0 );
  }

  if(KVFConstHandler::getVersion()==3){
    restore(&L, buf, offset);
    offset += sizeof(L);
    Xx = (float)( (L >> 57) & 0x7F )/100.0;
    Yy = (float)( (L >> 50) & 0x7F )/100.0;
    Zz = (float)( (L >> 43) & 0x7F )/100.0;
    X = X + Xx;
    Y = Y + Yy;
    Z = Z + Zz;
  }

  if(KVFConstHandler::getVersion()==4){
    restore(&L, buf, offset);
    offset += sizeof(L);
    Xx = (float)( (L >> 57) & 0x7F )/100.0;
    Yy = (float)( (L >> 50) & 0x7F )/100.0;
    Zz = (float)( (L >> 43) & 0x7F )/100.0;
    X = X + Xx;
    Y = Y + Yy;
    Z = Z + Zz;
    EnergyWithNsumMax = (float)( ( (unsigned int)( (L >> 23) & 0x0FFFFF ) )/5000.0 );
    VertexBadness = (float)( (L >> 11) & 0x0FFF )/100.0;
    Nhit17 = (unsigned int)( L & 0x07FF );
  }

  if(_enable_run_dependent_energy_correction){
    Energy /= (KVFConstHandler::RunDependenceOfVisibleEnergy(RunNumber));
    Energy17 /= (KVFConstHandler::RunDependenceOfVisibleEnergy(RunNumber));
    EnergyWithNsumMax /= (KVFConstHandler::RunDependenceOfVisibleEnergy(RunNumber));
  }
    
  float CorrectionFactor = 1.0;
  float CorrectionFactor17 = 1.0;
  float CorrectionFactorWithNsumMax = 1.0;

  if(_enable_energy_vertex_correction && KVFConstHandler::getVersion()==2){
    CorrectionFactor = KVFConstHandler::CorrectionOfVisibleEnergy(Energy);
    X = X * 1.008841;
    Y = Y * 1.008841;
  }
  
  if(_enable_energy_vertex_correction && KVFConstHandler::getVersion()==3){
    CorrectionFactor = KVFConstHandler::CorrectionOfVisibleEnergy(Energy);
    CorrectionFactor17 = KVFConstHandler::CorrectionOfVisibleEnergy(Energy17);
  }

  if(_enable_energy_vertex_correction && KVFConstHandler::getVersion()==4){
    CorrectionFactor = KVFConstHandler::CorrectionOfVisibleEnergy(Energy);
    CorrectionFactor17 = KVFConstHandler::CorrectionOfVisibleEnergy(Energy17);
    CorrectionFactorWithNsumMax = KVFConstHandler::CorrectionOfVisibleEnergy(EnergyWithNsumMax);
  }

  Energy /= CorrectionFactor;
  Energy17 /= CorrectionFactor17;
  EnergyWithNsumMax /= CorrectionFactorWithNsumMax;
  
  return true;
}

bool KVFLowEnergyEvent :: storeTo(char* buf, int offset)
{
  //int offset=0;
  unsigned long long int L = 0;

  if(KVFConstHandler::getVersion()==3){  
    if(Energy<0) Energy = 0;
    if(Energy17<0) Energy17 = 0;
  }

  if(KVFConstHandler::getVersion()==4){  
    if(Energy<0) Energy = 0;
    if(Energy17<0) Energy17 = 0;
    if(EnergyWithNsumMax<0) EnergyWithNsumMax = 0;
  }

  //round off to two decimal places
  X = X + 0.005;
  Y = Y + 0.005;
  Z = Z + 0.005;

  unsigned long long int run = ( (unsigned int)RunNumber & 0x0FFFF);
  L = ( (run << 48) | (TimeStamp) );
  store(L, buf, offset);
  offset += sizeof(L);

  unsigned long long int unix_time;
  unix_time = (unsigned long long int)( UnixTime & 0x0FFFFFFFF);
  L = ( (unix_time << 32) | EventNumber );
  store(L, buf, offset);
  offset += sizeof(L);

  unsigned long long int event_status = (EventStatus & 0x0F);
  unsigned long long int energy = (unsigned long long int)( (int)(Energy*5000) & 0x0FFFFF) ;
  unsigned long long int pid = (unsigned long long int)( PID & 0x0F);
  L = ( (pid<< 60) | (energy<<40) | (event_status<<36) | TriggerType );
  store(L, buf, offset);
  offset += sizeof(L);

  unsigned long long int qID, qOD, NsumMaxBuf;
  qID = (unsigned long long int)(TotalChargeID*100.0);
  //qOD = (unsigned long long int)(TotalChargeOD*100.0);
  qOD = (unsigned long long int)(TotalChargeOD);
  if(qOD>=2097152) qOD = 2097151; // Max 21bit
  NsumMaxBuf = (unsigned long long int)(NsumMax);
  //L = ( (qOD << 32) | qID );
  L = ( (qOD << 43) | (NsumMaxBuf << 32 )| qID );
  store(L, buf, offset);
  offset += sizeof(L);

  unsigned long long int x = ( (unsigned long long int)(X + 1024) & 0x07FF);
  unsigned long long int y = ( (unsigned long long int)(Y + 1024) & 0x07FF);
  unsigned long long int z = ( (unsigned long long int)(Z + 1024) & 0x07FF);
  L = ( (x << 52) | (y << 41) | (z << 30) 
	| ((N100 & 0x07FF) << 19) | ((NhitID & 0x07FF) << 8) | (NhitOD & 0x0FF) );
  store(L, buf, offset);
  offset += sizeof(L);
  
  if(KVFConstHandler::getVersion()==3 || KVFConstHandler::getVersion()==4){  
    unsigned long long int VSigmat, VChi2q, VChi2t, VNdf, energy17;
    if(VertexSigmaT>40.95) VertexSigmaT=40.95;
    if(VertexChi2Q>40.95) VertexChi2Q=40.95;
    if(VertexChi2T>40.95) VertexChi2T=40.95;
    VSigmat = ((unsigned long long int)(VertexSigmaT*100.0) & 0xFFF);
    VChi2q = ((unsigned long long int)(VertexChi2Q*100.0) & 0xFFF);
    VChi2t = ((unsigned long long int)(VertexChi2T*100.0) & 0xFFF);
    VNdf = ((unsigned long long int)(VertexNdf) & 0x3F);
    energy17 = (unsigned long long int)( (int)(Energy17*5000) & 0x0FFFFF) ;
    L = ( (VSigmat << 52) |  (VChi2q << 40) |  (VChi2t << 28) |  (VNdf << 22) |  (energy17 << 2) );
    store(L, buf, offset);
    offset += sizeof(L);
  }
  

  if(KVFConstHandler::getVersion()==3){  
    unsigned long long int xx, yy, zz;
    xx = (unsigned long long int)(((X+1024) - int(X+1024))*100.0) & 0x7F;
    yy = (unsigned long long int)(((Y+1024) - int(Y+1024))*100.0) & 0x7F;
    zz = (unsigned long long int)(((Z+1024) - int(Z+1024))*100.0) & 0x7F;
    L = ( (xx << 57) | (yy << 50) | (zz << 43) );
    store(L, buf, offset);
    offset += sizeof(L);
  }

  if(KVFConstHandler::getVersion()==4){  
    unsigned long long int xx, yy, zz;
    xx = (unsigned long long int)(((X+1024) - int(X+1024))*100.0) & 0x7F;
    yy = (unsigned long long int)(((Y+1024) - int(Y+1024))*100.0) & 0x7F;
    zz = (unsigned long long int)(((Z+1024) - int(Z+1024))*100.0) & 0x7F;
    unsigned long long int energy_with_nsummax = (unsigned long long int)( (int)(EnergyWithNsumMax*5000) & 0x0FFFFF) ;
    unsigned long long int VBadness = ((unsigned long long int)(VertexBadness*100.0) & 0xFFF);    
    L = ( (xx << 57) | (yy << 50) | (zz << 43) | (energy_with_nsummax << 23) | (VBadness << 11) | (Nhit17 & 0x07FF) );
    store(L, buf, offset);
    offset += sizeof(L);
  }

  return true;
}

bool KVFLowEnergyEvent :: isNoise(void)
{
  if((int)NhitID-(int)N100>=(int)N100-50) return true;
  else return false;
}
