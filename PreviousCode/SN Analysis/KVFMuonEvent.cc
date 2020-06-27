#include "KVFMuonEvent.hh"
#include "KVFConstHandler.hh"
#include <iostream>
#include <string.h>
#include <math.h>

int KVFMuonEvent :: getBufSize(void){
  if(KVFConstHandler::getVersion()==1) return 48;
  else if(KVFConstHandler::getVersion()==2) return 56;
  else if(KVFConstHandler::getVersion()==3 || KVFConstHandler::getVersion()==4) return 80;
  else {
    cerr << "Error: no version set" << endl;
    abort();
  }
  
  return 80;
}

bool KVFMuonEvent :: restoreFrom(const char* buf, int offset)
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
  Nhit5inch = (unsigned int)( (L>>59) & 0x07 );
  NhitOD = (unsigned int)( (L>>51) & 0x0FF );
  NhitID = (unsigned int)( (L>>40) & 0x07FF );  // added 2002/06/25
  EventStatus = (int)( (L>>36) & 0x0F );
  //  EventStatus = (int)( (L >> 40) & 0x0F );
  unsigned long long int HigherTriggerType = (unsigned int)( (L >> 32) & 0x0F );
  TriggerType = ( (HigherTriggerType << 32) | (L & 0x0FFFFFFFF) );

  restore(&L, buf, offset);
  offset += sizeof(L);
  EntranceX = (float)( (int)((L >> 53) & 0x07FF) - 1024);
  EntranceY = (float)( (int)((L >> 42) & 0x07FF) - 1024);
  EntranceZ = (float)( (int)((L >> 31) & 0x07FF) - 1024);
  TotalChargeID = (float)( L & 0x07FFFFFFF )/10.0;

  restore(&L, buf, offset);
  offset += sizeof(L);
  Badness   = (unsigned int)( (L >> 36) & 0x0FFFFFFF );
  DirectionX = (float)((int)((L >> 24) & 0x0FFF) - 2048)/2048.0;
  DirectionY = (float)((int)((L >> 12) & 0x0FFF) - 2048)/2048.0;
  DirectionZ = (float)((int)(   L      & 0x0FFF) - 2048)/2048.0;

  restore(&L, buf, offset);
  offset += sizeof(L);
  TotalCharge5inch = (float)( (L >> 32) & 0x0FFFFFFFF )/10.0;
  TotalChargeOD = (float)( L & 0x0FFFFFFFF )/10.0;

  if(KVFConstHandler::getVersion()==2){
    restore(&L, buf, offset);
    offset += sizeof(L);
    N100 = (unsigned int)( (L >> 53) & 0x07FF );
  }

  if(KVFConstHandler::getVersion()==3 || KVFConstHandler::getVersion()==4){
    restore(&L, buf, offset);
    offset += sizeof(L);
    N100 = (unsigned int)( (L >> 53) & 0x07FF );
    TotalCharge17 = (float)( (L >> 22) & 0x07FFFFFFF )/10.0;
    TrackSigmaT = (float)( (L >> 10) & 0x0FFF )/100.0;    
    //cout<<TotalChargeID<<" "<<TotalCharge17<<" "<<TrackSigmaT<<endl;
  }

  if(KVFConstHandler::getVersion()==3 || KVFConstHandler::getVersion()==4){
    restore(&L, buf, offset);
    offset += sizeof(L);
    VertexSigmaT = (float)( (L >> 52) & 0x0FFF )/100.0;    
    VertexChi2Q = (float)( (L >> 40) & 0x0FFF )/100.0;    
    VertexChi2T = (float)( (L >> 28) & 0x0FFF )/100.0;    
    VertexNdf = (int)( (L >> 22) & 0x3F );    
    Energy17 = (float)( ( (unsigned int)( (L >> 2) & 0x0FFFFF ) )/5000.0 );
  }

  if(KVFConstHandler::getVersion()==3 || KVFConstHandler::getVersion()==4){
    restore(&L, buf, offset);
    offset += sizeof(L);
    X = (float)( (int)((L >> 53) & 0x07FF) - 1024);
    Y = (float)( (int)((L >> 42) & 0x07FF) - 1024);
    Z = (float)( (int)((L >> 31) & 0x07FF) - 1024);
    Energy = (float)( ( (unsigned int)( (L >> 11) & 0x0FFFFF ) )/5000.0 );
    NsumMax = (int)( L & 0x007FF );    
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

bool KVFMuonEvent :: storeTo(char* buf, int offset)
{
  //int offset=0;
  unsigned long long int L = 0;
  
  if(KVFConstHandler::getVersion()==3){  
    if(Energy<0) Energy = 0;
    if(Energy17<0) Energy17 = 0;
    if(Energy>200) Energy = 209.7151;
    if(Energy17>200) Energy17 = 209.7151;
  }
  
  if(KVFConstHandler::getVersion()==4){  
    if(Energy<0) Energy = 0;
    if(Energy17<0) Energy17 = 0;
    if(EnergyWithNsumMax<0) EnergyWithNsumMax = 0;
    if(Energy>200) Energy = 209.7151;
    if(Energy17>200) Energy17 = 209.7151;
    if(EnergyWithNsumMax>200) EnergyWithNsumMax = 209.7151;
  }

  unsigned long long int run = ( (unsigned int)RunNumber & 0x0FFFF);
  L = ( (run << 48) | (TimeStamp) );
  store(L, buf, offset);
  offset += sizeof(L);

  unsigned long long int unix_time;
  unix_time = (unsigned long long int)( UnixTime & 0x0FFFFFFFF);
  L = ( (unix_time << 32) | EventNumber );
  store(L, buf, offset);
  offset += sizeof(L);

  unsigned long long int n5inch = (unsigned long long int)(Nhit5inch & 0x07);
  unsigned long long int nOD = (unsigned long long int)(NhitOD & 0x0FF);
  unsigned long long int nID = (unsigned long long int)(NhitID & 0x07FF);
  unsigned long long int event_status = (EventStatus & 0x0F);
  L = ( (n5inch << 59) | (nOD<<51) | (nID<<40) | (event_status<<36) | TriggerType );
  store(L, buf, offset);
  offset += sizeof(L);

  unsigned long long int x = ( (unsigned long long int)(EntranceX + 1024) & 0x07FF);
  unsigned long long int y = ( (unsigned long long int)(EntranceY + 1024) & 0x07FF);
  unsigned long long int z = ( (unsigned long long int)(EntranceZ + 1024) & 0x07FF);
  unsigned long long int qID;
  qID = (unsigned long long int)(TotalChargeID*10.0);
  L = ( (x << 53) | (y << 42) | (z << 31) | (qID & 0x7FFFFFFF) );
  store(L, buf, offset);
  offset += sizeof(L);

  unsigned long long int goodness = (unsigned long long int)( Badness & 0x0FFFFFFF);
  unsigned long long int dx = ( (unsigned long long int)( 2048*(DirectionX + 1) ) & 0x0FFF);
  unsigned long long int dy = ( (unsigned long long int)( 2048*(DirectionY + 1) ) & 0x0FFF);
  unsigned long long int dz = ( (unsigned long long int)( 2048*(DirectionZ + 1) ) & 0x0FFF);
  L = ( (goodness << 36) | (dx << 24) | (dy << 12) | (dz) );
  store(L, buf, offset);
  offset += sizeof(L);

  unsigned long long int qOD, q5inch;
  qOD = (unsigned long long int)(TotalChargeOD*10.0);
  q5inch = ( (unsigned long long int)(TotalCharge5inch*10.0) & 0x0FFFFFFFF);
  //  L = ( (q5inch << 32) | qID );
  L = ( (q5inch << 32) | qOD );
  store(L, buf, offset);
  offset += sizeof(L);
  
  if(KVFConstHandler::getVersion()==2){
    unsigned long long int n100;
    n100 = (unsigned long long int)(N100);
    L = ( (n100 << 53) );
    store(L, buf, offset);
    offset += sizeof(L);
  }

  if(KVFConstHandler::getVersion()==3 || KVFConstHandler::getVersion()==4){
    unsigned long long int n100,q17,TrSigmat;
    n100 = (unsigned long long int)(N100);
    q17 = ((unsigned long long int)(TotalCharge17*10.0) & 0x7FFFFFFF);
    if(TrackSigmaT>40.95) TrackSigmaT=40.95;
    TrSigmat = ((unsigned long long int)(TrackSigmaT*100.0) & 0xFFF);
    L = ( (n100 << 53) |  (q17 << 22) |  (TrSigmat << 10));
    store(L, buf, offset);
    offset += sizeof(L);
  }

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
 
  if(KVFConstHandler::getVersion()==3 || KVFConstHandler::getVersion()==4){  
    unsigned long long int x = ( (unsigned long long int)(X + 1024) & 0x07FF);
    unsigned long long int y = ( (unsigned long long int)(Y + 1024) & 0x07FF);
    unsigned long long int z = ( (unsigned long long int)(Z + 1024) & 0x07FF);
    unsigned long long int energy = (unsigned long long int)( (int)(Energy*5000) & 0x0FFFFF) ;
    unsigned long long int NsumMaxBuf = (unsigned long long int)(NsumMax & 0x07FF);

    L = ( (x << 53) | (y << 42) | (z << 31) | (energy << 11) | NsumMaxBuf );
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

float KVFMuonEvent :: getDistanceFrom(float x, float y, float z)
{
  float dX = x - EntranceX;
  float dY = y - EntranceY;
  float dZ = z - EntranceZ;
  float l = ( DirectionX*dX + DirectionY*dY + DirectionZ*dZ ) / ( DirectionX*DirectionX + DirectionY*DirectionY + DirectionZ*DirectionZ );
  l = -1*l;
  return sqrt(
	      (dX + DirectionX*l)*(dX + DirectionX*l)
	      + (dY + DirectionY*l)*(dY + DirectionY*l)
	      + (dZ + DirectionZ*l)*(dZ + DirectionZ*l)
	      );
}

void KVFMuonEvent :: getTrackLength(float *sciL, float *cheL)
{
  float sciR = 650;
  float cheR = 830;
  float l = getDistanceFrom(0,0,0);
  if(l<=sciR){
    *sciL=2*sqrt(sciR*sciR-l*l);
    *cheL=2*sqrt(cheR*cheR-l*l)-*sciL;
  } else if(l<=cheR){
    *sciL=0;
    *cheL=2*sqrt(cheR*cheR-l*l);
  } else{
    *sciL=0;
    *cheL=0;
  }
}

bool KVFMuonEvent :: isNoise(void)
{
  if((int)NhitID-(int)N100>=(int)N100-50) return true;
  else return false;
}
