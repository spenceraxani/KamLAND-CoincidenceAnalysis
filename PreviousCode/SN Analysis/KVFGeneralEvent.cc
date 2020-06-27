#include "KVFGeneralEvent.hh"
#include "KVFConstHandler.hh"
#include "KVFEnergyCorrection.hh"
#include <iostream>
#include <string.h>
#include <math.h>

int KVFGeneralEvent :: getBufSize(void){
  if(_enable_header == true){
    return (int)Header->getSize();
  }

  if(KVFConstHandler::getVersion()==1) return 48;
  else if(KVFConstHandler::getVersion()==2) return 56;
  else if(KVFConstHandler::getVersion()==3 || KVFConstHandler::getVersion()==4 ||  KVFConstHandler::getVersion()==5 ) return 80;
  else if(KVFConstHandler::getVersion()==6) return 96;
  else {
    cerr << "Error: no version set" << endl;
    abort();
  }

  return 80;
}

bool KVFGeneralEvent :: _ECorr_mode = true;

bool KVFGeneralEvent :: restoreFrom(const char* buf, int offset)
{
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
  NhitID = (unsigned int)( (L>>40) & 0x07FF );
  EventStatus = (int)( (L>>36) & 0x0F );
  unsigned long long int HigherTriggerType = (unsigned int)( (L >> 32) & 0x0F );
  TriggerType = ( (HigherTriggerType << 32) | (L & 0x0FFFFFFFF) );
  
  if(KVFConstHandler::getVersion()==5 || KVFConstHandler::getVersion() == 6){
    restore(&L, buf, offset);
    offset += sizeof(L);
    EntranceX = (float)( (int)((L >> 53) & 0x07FF) - 1024);
    EntranceY = (float)( (int)((L >> 42) & 0x07FF) - 1024);
    EntranceZ = (float)( (int)((L >> 31) & 0x07FF) - 1024);
    unsigned long long int TotalChargeID_exponent = (unsigned long long int)( (L>>27) & 0x0F );
    unsigned long long int TotalChargeID_significand = (unsigned long long int)( (L>>10) & 0x01FFFF );
    TotalChargeID   = (float)(TotalChargeID_significand / 10000.0)  * pow(10.0 , (int)TotalChargeID_exponent);
    N200OD          = (int)((L >> 2) & 0x0FF);
    
    restore(&L, buf, offset);
    offset += sizeof(L);
    Badness   = (unsigned int)( (L >> 50) & 0x03FFF );
    DirectionX = (float)((int)((L >> 38) & 0x0FFF) - 2048)/2048.0;
    DirectionY = (float)((int)((L >> 26) & 0x0FFF) - 2048)/2048.0;
    DirectionZ = (float)((int)((L >> 14) & 0x0FFF) - 2048)/2048.0;
    VertexBadness = (float)( (L >> 2) & 0x0FFF )/100.0;
    
    restore(&L, buf, offset);
    offset += sizeof(L);
    unsigned long long int TotalCharge5inch_exponent = (unsigned long long int)( (L>>60) & 0x0F);
    unsigned long long int TotalCharge5inch_significand = (unsigned long long int)( (L>>43) & 0x01FFFF );
    TotalCharge5inch   = (float)(TotalCharge5inch_significand / 10000.0)  * pow(10.0 , (int)TotalCharge5inch_exponent);
    unsigned long long int TotalChargeOD_exponent = (unsigned long long int)( (L>>39) & 0x0F );
    unsigned long long int TotalChargeOD_significand = (unsigned long long int)( (L>>22) & 0x01FFFF );
    TotalChargeOD   = (float)(TotalChargeOD_significand / 10000.0)  * pow(10.0 , (int)TotalChargeOD_exponent);
    unsigned long long int MaxChargeID_exponent = (unsigned long long int)( (L>>18) & 0x0F );
    unsigned long long int MaxChargeID_significand = (unsigned long long int)( (L>>1) & 0x01FFFF );
    MaxChargeID   = (float)(MaxChargeID_significand / 10000.0)  * pow(10.0 , (int)MaxChargeID_exponent);
    
    restore(&L, buf, offset);
    offset += sizeof(L);
    N100 = (unsigned int)( (L >> 53) & 0x07FF );
    unsigned long long int TotalCharge17_exponent = (unsigned long long int)( (L>>49) & 0x0F );
    unsigned long long int TotalCharge17_significand = (unsigned long long int)( (L>>32) & 0x01FFFF );
    TotalCharge17   = (float)(TotalCharge17_significand / 10000.0)  * pow(10.0 , (int)TotalCharge17_exponent);
    TrackSigmaT = (float)( (L >> 20) & 0x0FFF )/100.0;
    NsumMax     =  (int)( (L >> 9) & 0x007FF );    
    NhitODFromMuonFitter   = (unsigned int)((L >> 1) & 0x0FF);
    
    restore(&L, buf, offset);
    offset += sizeof(L);
    VertexSigmaT = (float)( (L >> 52) & 0x0FFF )/100.0;    
    VertexChi2Q = (float)( (L >> 40) & 0x0FFF )/100.0;    
    VertexChi2T = (float)( (L >> 28) & 0x0FFF )/100.0;    
    VertexNdf = (int)( (L >> 22) & 0x3F );    
    unsigned long long int Energy17_exponent = (unsigned long long int)( (L>>18) & 0x0F );
    unsigned long long int Energy17_significand = (unsigned long long int)( (L>>1) & 0x01FFFF );
    Energy17   = (float)(Energy17_significand / 10000.0)  * pow(10.0 , (int)Energy17_exponent);
    
    restore(&L, buf, offset);
    offset += sizeof(L);
    X = (float)((L >> 44) & 0x0FFFFF)/100.0 - 1024;
    Y = (float)((L >> 24) & 0x0FFFFF)/100.0 - 1024;
    Z = (float)((L >>  4) & 0x0FFFFF)/100.0 - 1024;
    GivenUp =  (int)((L >> 3) & 0x01);
    
    restore(&L, buf, offset);
    offset += sizeof(L);
    unsigned long long int Energy_exponent = (unsigned long long int)( (L>>60) & 0x0F );
    unsigned long long int Energy_significand = (unsigned long long int)( (L>>43) & 0x01FFFF );
    Energy   = (float)(Energy_significand / 10000.0)  * pow(10.0 , (int)Energy_exponent);

    unsigned long long int EnergyWithNsumMax_exponent = (unsigned long long int)( (L>>39) & 0x0F );
    unsigned long long int EnergyWithNsumMax_significand = (unsigned long long int)( (L>>22) & 0x01FFFF );
    EnergyWithNsumMax   = (float)(EnergyWithNsumMax_significand / 10000.0)  * pow(10.0 , (int)EnergyWithNsumMax_exponent);
    Nhit17 = (unsigned int)((L >> 11) & 0x07FF );
    NeighborCharge     = (int)(L & 0x07FF );

    if (KVFConstHandler::getVersion() == 6) {
      restore(&L, buf, offset);
      offset += sizeof(L);
      X_Kat = (float)((L >> 44) & 0x0FFFFF)/100.0 - 1024; // 20bit
      Y_Kat = (float)((L >> 24) & 0x0FFFFF)/100.0 - 1024; // 20bit
      Z_Kat = (float)((L >>  4) & 0x0FFFFF)/100.0 - 1024; // 20bit

      restore(&L, buf, offset);
      offset += sizeof(L);
      V2VertexTimeBadness = (float)((L >> 52) & 0x0FFF)/100.0; // 12bit
      V2Niterations  = (int)((L >> 45) & 0x7F); // 7bit
      V2StoppedBecause = (int)((L >> 43) & 0x3) - 1; // 2bit
      unsigned long long int EnergyA2_exponent = (unsigned long long int)( (L>>39) & 0x0F ); // 4bit
      unsigned long long int EnergyA2_significand = (unsigned long long int)( (L>>22) & 0x01FFFF ); // 17bit
      EnergyA2 = (float)(EnergyA2_significand / 10000.0) * pow(10.0, (int)EnergyA2_exponent); // 4+17 = 21bit
      unsigned long long int Energy17A2_exponent = (unsigned long long int)( (L>>18) & 0x0F ); // 4bit
      unsigned long long int Energy17A2_significand = (unsigned long long int)( (L>>1) & 0x01FFFF ); // 17bit
      Energy17A2 = (float)(Energy17A2_significand / 10000.0) * pow(10.0, (int)Energy17A2_exponent); // 4+17 = 21bit
    }
  }
  
  
  
  else{
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
  }


  restore(&L, buf, offset);
  offset += sizeof(L);
  MicroSec = (unsigned int)( (L >> 44) & 0x0FFFFF );
  NanoSec  = (unsigned int)( (L >> 34) & 0x03FF );  
  SubRunNumber = (short)( (L >> 20) & 0x03FFF );  

  float R = sqrt(X*X + Y*Y + Z*Z);
  float Rho = sqrt(X*X + Y*Y);

  // temporary
  int RunNumber_Tmp = RunNumber;
  if(_enable_header == true && Header->DataType==2){ // mogura
    RunNumber = 10195;
  }

  if(_ECorr_mode == true){
    EnergyCorr = Energy * (KVFEnergyCorrection::ZPositionDependenceOfVisibleEnergy(RunNumber,false,R,Z)) * (KVFEnergyCorrection::OffAxisPositionDependenceOfVisibleEnergy(RunNumber,false,R,Rho)) * (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,false,false,R));

    Energy17Corr = Energy17 * (KVFEnergyCorrection::ZPositionDependenceOfVisibleEnergy(RunNumber,false,R,Z)) * (KVFEnergyCorrection::OffAxisPositionDependenceOfVisibleEnergy(RunNumber,false,R,Rho)) * (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,false,false,R));

    EnergyA2Corr = EnergyA2 * (KVFEnergyCorrection::ZPositionDependenceOfVisibleEnergy(RunNumber,true,R,Z)) * (KVFEnergyCorrection::OffAxisPositionDependenceOfVisibleEnergy(RunNumber,true,R,Rho)) * (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,false,true,R));

    Energy17A2Corr = Energy17A2 * (KVFEnergyCorrection::ZPositionDependenceOfVisibleEnergy(RunNumber,true,R,Z)) * (KVFEnergyCorrection::OffAxisPositionDependenceOfVisibleEnergy(RunNumber,true,R,Rho)) * (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,false,true,R));

    EnergyCorr *= KVFEnergyCorrection::CorrectionOf20inchEnergy(RunNumber,Energy17Corr,false);
    EnergyA2Corr *= KVFEnergyCorrection::CorrectionOf20inchEnergy(RunNumber,Energy17A2Corr,true);

    EnergyCorr /= KVFConstHandler::CorrectionOfVisibleEnergy(EnergyCorr);
    Energy17Corr /= KVFConstHandler::CorrectionOfVisibleEnergy(Energy17Corr);
  }
  

  if(_enable_z_position_dependent_energy_correction==true){
    Energy *= (KVFEnergyCorrection::ZPositionDependenceOfVisibleEnergy(RunNumber,false,R,Z));
    Energy17 *= (KVFEnergyCorrection::ZPositionDependenceOfVisibleEnergy(RunNumber,false,R,Z));
    EnergyWithNsumMax *= (KVFEnergyCorrection::ZPositionDependenceOfVisibleEnergy(RunNumber,false,R,Z));
    EnergyA2 *= (KVFEnergyCorrection::ZPositionDependenceOfVisibleEnergy(RunNumber,true,R,Z));
    Energy17A2 *= (KVFEnergyCorrection::ZPositionDependenceOfVisibleEnergy(RunNumber,true,R,Z));
  }

  if(_enable_offaxis_position_dependent_energy_correction==true){
    Energy *= (KVFEnergyCorrection::OffAxisPositionDependenceOfVisibleEnergy(RunNumber,false,R,Rho));
    Energy17 *= (KVFEnergyCorrection::OffAxisPositionDependenceOfVisibleEnergy(RunNumber,false,R,Rho));
    EnergyWithNsumMax *= (KVFEnergyCorrection::OffAxisPositionDependenceOfVisibleEnergy(RunNumber,false,R,Rho));
    EnergyA2 *= (KVFEnergyCorrection::OffAxisPositionDependenceOfVisibleEnergy(RunNumber,true,R,Rho));
    Energy17A2 *= (KVFEnergyCorrection::OffAxisPositionDependenceOfVisibleEnergy(RunNumber,true,R,Rho));
  }

  if(_enable_new_run_dependent_energy_correction==true){
    Energy *= (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,false,false,R));
    Energy17 *= (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,false,false,R));
    EnergyWithNsumMax *= (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,false,false,R));
    EnergyA2 *= (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,false,true,R));
    Energy17A2 *= (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,false,true,R));
  }
  else if(_enable_run_dependent_energy_correction==true){
    Energy *= (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,true,false,R));
    Energy17 *= (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,true,false,R));
    EnergyWithNsumMax *= (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,true,false,R));
    EnergyA2 *= (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,true,true,R));
    Energy17A2 *= (KVFEnergyCorrection::RunDependenceOfVisibleEnergy(RunNumber,true,true,R));
  }


  if(_enable_20inch_energy_correction==true){
    Energy *= KVFEnergyCorrection::CorrectionOf20inchEnergy(RunNumber,Energy17,false);
    EnergyA2 *= KVFEnergyCorrection::CorrectionOf20inchEnergy(RunNumber,Energy17A2,true);
  }  


  float CorrectionFactor = 1.0;
  float CorrectionFactor17 = 1.0;
  float CorrectionFactorWithNsumMax = 1.0;
  float CorrectionFactorA2 = 1.0;

  if(_enable_energy_vertex_correction && KVFConstHandler::getVersion()==3){
    CorrectionFactor = KVFConstHandler::CorrectionOfVisibleEnergy(Energy);
    CorrectionFactor17 = KVFConstHandler::CorrectionOfVisibleEnergy(Energy17);
  }

  if(_enable_energy_vertex_correction && KVFConstHandler::getVersion()==4){
    CorrectionFactor = KVFConstHandler::CorrectionOfVisibleEnergy(Energy);
    CorrectionFactor17 = KVFConstHandler::CorrectionOfVisibleEnergy(Energy17);
    CorrectionFactorWithNsumMax = KVFConstHandler::CorrectionOfVisibleEnergy(EnergyWithNsumMax);
  }
  
  if(_enable_energy_vertex_correction && (KVFConstHandler::getVersion()==5 || KVFConstHandler::getVersion()==6)){
    CorrectionFactor = KVFConstHandler::CorrectionOfVisibleEnergy(Energy);
    CorrectionFactor17 = KVFConstHandler::CorrectionOfVisibleEnergy(Energy17);
    CorrectionFactorWithNsumMax = KVFConstHandler::CorrectionOfVisibleEnergy(EnergyWithNsumMax);
  }

  Energy /= CorrectionFactor;
  Energy17 /= CorrectionFactor17;
  EnergyWithNsumMax /= CorrectionFactorWithNsumMax;

  if(_enable_header == true && Header->DataType==2){ // mogura
    RunNumber = RunNumber_Tmp;
  }
  return true;
}


bool KVFGeneralEvent :: storeTo(char* buf, int offset)
{
  //int offset=0;
  unsigned long long int L = 0;
  
  //round off to two decimal places
  X = X + 0.005;
  Y = Y + 0.005;
  Z = Z + 0.005;

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
  
  if(KVFConstHandler::getVersion()==5 || KVFConstHandler::getVersion()==6){  
    if(Badness  > 16383)Badness = 16383;
    if(NeighborCharge > 2000)NeighborCharge = 2000;
    if(EnergyA2<0) EnergyA2 = 0;
    if(EnergyA2>200) EnergyA2 = 209.7151;
    if(Energy17A2<0) Energy17A2 = 0;
    if(Energy17A2>200) Energy17A2 = 209.7151;
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
  
  if(KVFConstHandler::getVersion()==5 || KVFConstHandler::getVersion()==6){ 
    unsigned long long int ex = ( (unsigned long long int)(EntranceX + 1024) & 0x07FF);
    unsigned long long int ey = ( (unsigned long long int)(EntranceY + 1024) & 0x07FF);
    unsigned long long int ez = ( (unsigned long long int)(EntranceZ + 1024) & 0x07FF);
    unsigned long long qID_exponent = ((unsigned long long int)log10(TotalChargeID) & 0x0F);
    if(TotalChargeID < 1.0)qID_exponent = 0;
    unsigned long long qID_significand =  ( (unsigned long long int)(TotalChargeID / (pow(10.0,(int)qID_exponent) ) *  pow(10.0 , 4))  & 0x01FFFF); 
    unsigned long long int n200OD  = (unsigned long long int)(N200OD & 0x0FF);
    L  = ( (ex << 53) | (ey << 42) | (ez << 31) | (qID_exponent << 27) | (qID_significand << 10) |  (n200OD << 2) );
    store(L, buf, offset);
    offset += sizeof(L);
    
    unsigned long long int goodness = (unsigned long long int)( Badness & 0x03FFF);
    unsigned long long int dx = ( (unsigned long long int)( 2048*(DirectionX + 1) ) & 0x0FFF);
    unsigned long long int dy = ( (unsigned long long int)( 2048*(DirectionY + 1) ) & 0x0FFF);
    unsigned long long int dz = ( (unsigned long long int)( 2048*(DirectionZ + 1) ) & 0x0FFF);
    unsigned long long int VBadness = ((unsigned long long int)(VertexBadness*100.0) & 0xFFF);   
    L = ( (goodness << 50) | (dx << 38) | (dy << 26) | (dz << 14) | (VBadness << 2));
    store(L, buf, offset);
    offset += sizeof(L);
    
    unsigned long long int qOD, q5inch;
    unsigned long long qOD_exponent = ((unsigned long long int)log10(TotalChargeOD)  & 0x0F);
    if(TotalChargeOD < 1.0)qOD_exponent = 0;
    unsigned long long qOD_significand =  ( (unsigned long long int)(TotalChargeOD / (pow(10.0,(int)qOD_exponent) ) *  pow(10.0 , 4))  & 0x01FFFF);
    unsigned long long q5inch_exponent = ((unsigned long long int)log10(TotalCharge5inch)  & 0x0F);
    if(TotalCharge5inch < 1.0)q5inch_exponent = 0;
    unsigned long long q5inch_significand =  ( (unsigned long long int)(TotalCharge5inch / (pow(10.0,(int)q5inch_exponent) ) *  pow(10.0 , 4))  & 0x01FFFF);
    unsigned long long maxq_exponent = ((unsigned long long int)log10(MaxChargeID)  & 0x0F);
    if(MaxChargeID < 1.0)maxq_exponent = 0;
    unsigned long long maxq_significand =  ( (unsigned long long int)(MaxChargeID / (pow(10.0,(int)maxq_exponent) ) *  pow(10.0 , 4))  & 0x01FFFF);
    L = ((q5inch_exponent << 60) |  (q5inch_significand << 43) | (qOD_exponent << 39) | (qOD_significand << 22) | (maxq_exponent << 18) | (maxq_significand << 1));
    store(L, buf, offset);
    offset += sizeof(L);
    
    
    unsigned long long int n100,q17,TrSigmat;
    n100 = (unsigned long long int)(N100);
    unsigned long long q17_exponent = ((unsigned long long int)log10(TotalCharge17)  & 0x0F);
    if(TotalCharge17 < 1.0)q17_exponent = 0;
    unsigned long long q17_significand =  ( (unsigned long long int)(TotalCharge17 / (pow(10.0,(int)q17_exponent) ) *  pow(10.0 , 4))  & 0x01FFFF);
    if(TrackSigmaT>40.95) TrackSigmaT=40.95;
    TrSigmat = ((unsigned long long int)(TrackSigmaT*100.0) & 0xFFF);
    unsigned long long int NsumMaxBuf = (unsigned long long int)(NsumMax & 0x07FF);
    unsigned long long int nODfrommufitter  = (unsigned long long int)(NhitODFromMuonFitter & 0x0FF);
    L = ( (n100 << 53) |  (q17_exponent << 49) |  (q17_significand << 32) | (TrSigmat << 20) | (NsumMaxBuf << 9) | (nODfrommufitter << 1));
    store(L, buf, offset);
    offset += sizeof(L);
    
    unsigned long long int VSigmat, VChi2q, VChi2t, VNdf, energy17;
    if(VertexSigmaT>40.95) VertexSigmaT=40.95;
    if(VertexChi2Q>40.95) VertexChi2Q=40.95;
    if(VertexChi2T>40.95) VertexChi2T=40.95;
    VSigmat = ((unsigned long long int)(VertexSigmaT*100.0) & 0xFFF);
    VChi2q = ((unsigned long long int)(VertexChi2Q*100.0) & 0xFFF);
    VChi2t = ((unsigned long long int)(VertexChi2T*100.0) & 0xFFF);
    VNdf = ((unsigned long long int)(VertexNdf) & 0x3F);
    unsigned long long energy17_exponent = ((unsigned long long int)(log10(Energy17))  & 0x0F);
    if(Energy17 < 1.0)energy17_exponent = 0;
    unsigned long long energy17_significand =  ( (unsigned long long int)(Energy17 / (pow(10.0,(int)energy17_exponent) ) *  pow(10.0 , 4))  & 0x01FFFF);

    L = ( (VSigmat << 52) |  (VChi2q << 40) |  (VChi2t << 28) |  (VNdf << 22) |  (energy17_exponent << 18) | (energy17_significand << 1) );
    store(L, buf, offset);
    offset += sizeof(L);
    
    unsigned long long int x = ( (unsigned long long int)(((X + 1024) * 100.0)) & 0x0FFFFF);
    unsigned long long int y = ( (unsigned long long int)(((Y + 1024) * 100.0)) & 0x0FFFFF);
    unsigned long long int z = ( (unsigned long long int)(((Z + 1024) * 100.0)) & 0x0FFFFF); 
    unsigned long long int given_up = ( (unsigned long long int)(GivenUp) & 0x01); 
    L = ( (x << 44) | (y << 24) | (z << 4) | (given_up << 3) );
    store(L, buf, offset);
    offset += sizeof(L);
    
    unsigned long long energy_exponent = ((unsigned long long int)log10(Energy) & 0x0F);
    if(Energy < 1.0)energy_exponent = 0;
    unsigned long long energy_significand =  ( (unsigned long long int)(Energy / (pow(10.0,(int)energy_exponent) ) *  pow(10.0 , 4))  & 0x01FFFF);

    unsigned long long energy_with_nsummax_exponent = ((unsigned long long int)log10(EnergyWithNsumMax) & 0x0F);
    if(EnergyWithNsumMax < 1.0)energy_with_nsummax_exponent = 0;
    unsigned long long energy_with_nsummax_significand =  ( (unsigned long long int)(EnergyWithNsumMax / (pow(10.0,(int)energy_with_nsummax_exponent) ) *  pow(10.0 , 4))  & 0x01FFFF);
    unsigned long long int nhit17   = ((unsigned long long int)Nhit17 & 0x07FF);
    unsigned long long int ncharge   = ((unsigned long long int)NeighborCharge & 0x07FF);
    L = ( (energy_exponent << 60) | (energy_significand << 43) | (energy_with_nsummax_exponent << 39) | (energy_with_nsummax_significand << 22) | (nhit17 << 11) | (ncharge));
    store(L, buf, offset);
    offset += sizeof(L);

    if (KVFConstHandler::getVersion()==6) { 
      unsigned long long int x_Kat = ( (unsigned long long int)(((X_Kat + 1024) * 100.0)) & 0x0FFFFF);
      unsigned long long int y_Kat = ( (unsigned long long int)(((Y_Kat + 1024) * 100.0)) & 0x0FFFFF);
      unsigned long long int z_Kat = ( (unsigned long long int)(((Z_Kat + 1024) * 100.0)) & 0x0FFFFF); 
      L = ( (x_Kat << 44) | (y_Kat << 24) | (z_Kat << 4) );
      store(L, buf, offset);
      offset += sizeof(L);

      unsigned long long int v2_vertex_time_badness = ((unsigned long long int)(V2VertexTimeBadness*100.0) & 0xFFF);   
      unsigned long long int v2_n_iterations        = ((unsigned long long int)(V2Niterations) & 0x7F);
      unsigned long long int v2_stopped_because     = ((unsigned long long int)(V2StoppedBecause + 1) & 0x3);
      unsigned long long energy_A2_exponent         = ((unsigned long long int)log10(EnergyA2) & 0x0F);
      if (EnergyA2 < 1.0) energy_A2_exponent = 0;
      unsigned long long energy_A2_significand      = ((unsigned long long int)(EnergyA2 / (pow(10.0, (int)energy_A2_exponent) ) *  pow(10.0, 4))  & 0x01FFFF);
      unsigned long long energy17_A2_exponent         = ((unsigned long long int)log10(Energy17A2) & 0x0F);
      if (Energy17A2 < 1.0) energy17_A2_exponent = 0;
      unsigned long long energy17_A2_significand      = ((unsigned long long int)(Energy17A2 / (pow(10.0, (int)energy17_A2_exponent) ) *  pow(10.0, 4))  & 0x01FFFF);
      L = ( (v2_vertex_time_badness << 52) | (v2_n_iterations << 45) | (v2_stopped_because << 43) | (energy_A2_exponent << 39) | (energy_A2_significand << 22) | (energy17_A2_exponent << 18) | (energy17_A2_significand << 1));
      store(L, buf, offset);
      offset += sizeof(L);
    }
  }

  else{
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
  }

  unsigned long long int micro_sec, nano_sec, subrun;
  micro_sec = (unsigned long long int)( MicroSec & 0x0FFFFF);
  nano_sec = (unsigned long long int)( NanoSec & 0x03FF);
  subrun = ( (unsigned long long int)SubRunNumber & 0x03FFF);
  L = ( (micro_sec << 44) | (nano_sec << 34) | (subrun << 20) );
  store(L, buf, offset);
  offset += sizeof(L);
  
  return true;
}

float KVFGeneralEvent :: getDistanceFrom(float x, float y, float z)
{
  float dX = x - EntranceX;
  float dY = y - EntranceY;
  float dZ = z - EntranceZ;

  if((DirectionX*DirectionX + DirectionY*DirectionY + DirectionZ*DirectionZ) == 0){
    return -1.0;
  }

  float l = ( DirectionX*dX + DirectionY*dY + DirectionZ*dZ ) / ( DirectionX*DirectionX + DirectionY*DirectionY + DirectionZ*DirectionZ );
  l = -1*l;
  return sqrt(
	      (dX + DirectionX*l)*(dX + DirectionX*l)
	      + (dY + DirectionY*l)*(dY + DirectionY*l)
	      + (dZ + DirectionZ*l)*(dZ + DirectionZ*l)
	      );
}

void KVFGeneralEvent :: getTrackLength(float *sciL, float *cheL)
{
  float sciR = 650;
  float cheR = 830;
  float l = getDistanceFrom(0,0,0);

  if(l<0){
    *sciL=-1.0;
    *cheL=-1.0;
  }

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

bool KVFGeneralEvent :: isNoise(void)
{
  if((int)NhitID-(int)N100>=(int)N100-50) return true;
  else return false;
}

bool KVFGeneralEvent :: isThisMuon_NoTimeWindowForODhit(int NhitOD_Cut, float TotalCharge17_Cut1, float TotalCharge17_Cut2)
{
  if(NhitID==0) return false;
  if((NhitODFromMuonFitter >= NhitOD_Cut && TotalCharge17 >= TotalCharge17_Cut1)
     || (TotalCharge17  >= TotalCharge17_Cut2)
     ) return true;
  else return false;
}

bool KVFGeneralEvent :: isThisMuon(int NhitOD_Cut, float TotalCharge17_Cut1, float TotalCharge17_Cut2)
{
  if(NhitID==0) return false;
  if((N200OD >= NhitOD_Cut && TotalCharge17 >= TotalCharge17_Cut1)
     || (TotalCharge17  >= TotalCharge17_Cut2)
     ) return true;
  else return false;
}

bool KVFGeneralEvent :: isThisFlasher(void)
{
  if(KVFGeneralEvent::isThisMuon() == true)return false;
  if(TotalChargeID > 2500.0 && MaxChargeID/TotalChargeID >= 0.6 && NeighborCharge >= 20){
    if((int)NhitID - (int)N100 < (int)N100 - 50)return true;
    else return false;
  }
  else return false;

}
