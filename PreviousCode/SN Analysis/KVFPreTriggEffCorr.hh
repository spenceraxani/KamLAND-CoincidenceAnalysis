#ifndef KVFPRETRIGGEFFCORR_HH
#define KVFPRETRIGGEFFCORR_HH
/*
Correction Class to apply prescale efficiency correction that is Z and R dependent.
Values are obtrained from runs 6972-7100 by using low threshold runs 6972 and 6973.
For each Radial shell with Z >= 3.0 m and Z < 3.0 m there is an average mean and 
sigma value along with the associated statistical and systematic error.
-- 02/05/08
-- 02/09/08 added the correlation coefficient to error propogation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <math.h>
#include <TMath.h>

using namespace std;

class KVFPreTriggEffCorr{

public:
  KVFPreTriggEffCorr();
  ~KVFPreTriggEffCorr();

  double EffFunc(double E, double mean, double sigma);
  double EffFuncErr(double E, double mean, double sigma, double meanErr, double sigmaErr, double corr);

  double GetEff(double Z, double R, double E);
  double GetEffErr(double Z, double R, double E);

  void SetEnergy17Corr();
  void SetEnergy17();

private:
  string fEffVals;           // for Energy17 values
  string fEffValsCorr;       // for Ecorr 17 values
  static const int Rpos = 7; // low -> high Radius
  static const int Zpos = 2; // 0 -- high, 1 -- low
  static const int Epos = 2; // 0 -- Energy17, 1 -- Ecorr17
  int Ecorr;             // Array Designator for Epos

  double Mean[Zpos][Rpos][Epos];
  double Sigma[Zpos][Rpos][Epos];
  double MeanErr[Zpos][Rpos][Epos];
  double SigmaErr[Zpos][Rpos][Epos];
  double CorrCoef[Zpos][Rpos][Epos];

  double Rmin[Rpos];
  double Rmax[Rpos];
};


#endif
