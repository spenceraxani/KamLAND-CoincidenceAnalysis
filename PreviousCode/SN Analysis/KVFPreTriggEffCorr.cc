#include "KVFPreTriggEffCorr.hh"


KVFPreTriggEffCorr::KVFPreTriggEffCorr(){

  fEffVals = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/after-distillation/prescale_trigger_efficiency.table");
  ifstream fin;
  fin.open(fEffVals.c_str(),ifstream::in);
  if(fin.bad()){
    cout << "Cannot find file " << fEffVals.c_str() << endl;
  }

  char dummy[256];
  int line = 0;
  double Stat_Err;
  Ecorr = 0;

  fin.getline(dummy,256);
  while (line < Rpos){

    fin >>  Rmin[line] >> Rmax[line] >> Mean[0][line][Ecorr] >> Stat_Err >> MeanErr[0][line][Ecorr] >> Sigma[0][line][Ecorr] >> Stat_Err >> SigmaErr[0][line][Ecorr] >> Mean[1][line][Ecorr] >> Stat_Err >> MeanErr[1][line][Ecorr] >> Sigma[1][line][Ecorr] >> Stat_Err >> SigmaErr[1][line][Ecorr] >> CorrCoef[0][line][Ecorr] >> CorrCoef[1][line][Ecorr];

    line++;
    
  }

  fin.close();

  /* Now Load Energy 17 Corrected Values */


  fEffValsCorr = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/after-distillation/prescale_trigger_efficiency_Ecorr.table");
  fin.open(fEffValsCorr.c_str(),ifstream::in);
  if(fin.bad()){
    cout << "Cannot find file " << fEffValsCorr.c_str() << endl;
  }

  line = 0;
  Ecorr = 1;
  fin.getline(dummy,256);
  while (line < Rpos){

    fin >>  Rmin[line] >> Rmax[line] >> Mean[0][line][Ecorr] >> Stat_Err >> MeanErr[0][line][Ecorr] >> Sigma[0][line][Ecorr] >> Stat_Err >> SigmaErr[0][line][Ecorr] >> Mean[1][line][Ecorr] >> Stat_Err >> MeanErr[1][line][Ecorr] >> Sigma[1][line][Ecorr] >> Stat_Err >> SigmaErr[1][line][Ecorr] >> CorrCoef[0][line][Ecorr] >> CorrCoef[1][line][Ecorr];

    line++;
    
  }

  fin.close();

}



KVFPreTriggEffCorr::~KVFPreTriggEffCorr(){


}

double KVFPreTriggEffCorr::EffFunc(double E, double mean, double sigma){

  return (0.5*(TMath::Erf( (E-mean)/(sigma*TMath::Sqrt(2.0)) ) + 1.0));

}

double KVFPreTriggEffCorr::EffFuncErr(double E, double mean, double sigma, double meanErr, double sigmaErr, double corr){

  double z = (E - mean)/(TMath::Sqrt(2.0)*sigma);

  double deltaM = (mean*meanErr)/(TMath::Sqrt(2.0)*sigma);
  double deltaS = (z*sigmaErr)/sigma;
  double corrCoef = 2.0*deltaM*deltaS*corr;

  double radical = deltaM*deltaM + deltaS*deltaS + corrCoef;


  double p1 =  (2.0/TMath::Sqrt(TMath::Pi()));
  double p2 = (TMath::Exp(-z*z));

  if(radical < 0.0){
    cout <<"negative radical... return -1" << endl;
    return -1;
  }else{
    return p1*p2*TMath::Sqrt(radical);
  }
}



double KVFPreTriggEffCorr::GetEff(double Z, double R, double E){

  int i=0;

  while(R >= Rmax[i] && i < Rpos) i++;

  if(R <= Rmax[i]  && R >= Rmin[i]){
    if(Z >= 300.0){
      return KVFPreTriggEffCorr::EffFunc(E,Mean[0][i][Ecorr],Sigma[0][i][Ecorr]);
    }else{
      return KVFPreTriggEffCorr::EffFunc(E,Mean[1][i][Ecorr],Sigma[1][i][Ecorr]);
    }
  }else{
    cerr << "Didn't make loop to get efficiency, R = " << R << endl;
    return 0;
  }

}


double KVFPreTriggEffCorr::GetEffErr(double Z, double R, double E){

  int i=0;

  while(R >= Rmax[i] && i < Rpos) i++;

  if(R <= Rmax[i]  && R >= Rmin[i]){
    if(Z >= 300.0){
      return KVFPreTriggEffCorr::EffFuncErr(E,Mean[0][i][Ecorr],Sigma[0][i][Ecorr],MeanErr[0][i][Ecorr],SigmaErr[0][i][Ecorr],CorrCoef[1][i][Ecorr]);
    }else{
      return KVFPreTriggEffCorr::EffFuncErr(E,Mean[1][i][Ecorr],Sigma[1][i][Ecorr],MeanErr[1][i][Ecorr],SigmaErr[1][i][Ecorr],CorrCoef[1][i][Ecorr]);
    }
  }else{
    cerr << "Didn't make loop to get efficiency error, R = " << R << endl;
    return 0;
  }



}


void KVFPreTriggEffCorr::SetEnergy17Corr(){
  Ecorr = 1;
}


void KVFPreTriggEffCorr::SetEnergy17(){
  Ecorr = 0;
}
