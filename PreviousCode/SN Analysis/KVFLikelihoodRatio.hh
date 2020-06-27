#ifndef __KVFLikelihoodRatio_HH
#define __KVFLikelihoodRatio_HH

#include <string>
using namespace std;

class KVFLikelihoodRatio {
public:
  KVFLikelihoodRatio(void){}
  ~KVFLikelihoodRatio(void){}
  
  static bool setVersion(int version){_LRVersion = version;}
  static int  getVersion(void){return _LRVersion;}
  
  static bool changeRun(int RunNumber);
  
  static int PeriodID(int RunNumber);

  static double Likelihood_Ratio_Cut(void);
  
  static bool set_Ep(double Ep);
  
  static double Likelihood_Ratio(double dR, double Rp, double Rd, double Ed, double dT);
  static double PDF_SG(double dR, double Rp, double Rd, double Ed, double dT);
  static double PDF_BG(double dR, double Rp, double Rd, double Ed, double dT);
  
  static double PDF_dR_SG(double dR);
  static double PDF_Rp_SG(double Rp);
  static double PDF_Rd_SG(double Rd);
  static double PDF_Ed_SG(double Ed);
  static double PDF_dT_SG(double dT);
  
  static double PDF_dT_BG(double dT);
  
  static bool disable20inch(){Resolution_enable20inch = false;}
  static bool enable20inch(){Resolution_enable20inch = true;}

private:
  static bool setConstant();
  static bool setPDF();
  
  static bool _isSetConstant;
  static bool _isSetPDF;
  
  static int _RunNumber;
  
  static int _LastPeriodID;

  static int _LRVersion;
  
  static double* Likelihood_Ratio_Constant;

  static int index_Ep;

  static const double R_Max;

  static bool Resolution_enable20inch;

  static const double Energy_np;
  static const double Energy_nC;
  static const double Energy_np_Resolution_disable20inch;
  static const double Energy_nC_Resolution_disable20inch;
  static const double Energy_np_Resolution_enable20inch;
  static const double Energy_nC_Resolution_enable20inch;
  static const double Energy_np_Resolution_disable20inch_after_1st_distillation;
  static const double Energy_nC_Resolution_disable20inch_after_1st_distillation;
  static const double Energy_np_Resolution_enable20inch_after_1st_distillation;
  static const double Energy_nC_Resolution_enable20inch_after_1st_distillation;
  static const double Energy_np_Resolution_disable20inch_after_2nd_distillation;
  static const double Energy_nC_Resolution_disable20inch_after_2nd_distillation;
  static const double Energy_np_Resolution_enable20inch_after_2nd_distillation;
  static const double Energy_nC_Resolution_enable20inch_after_2nd_distillation;
  static const double CaptureRatio;

  static const double CaptureTime;


  static double* PDF_dR_SG_Hist;
  static double** PDF_BG_Hist;

  static int NofPDF_dR_SG;
  static int NofPDF_BG;

  static double Normalize_dR_SG;
  static double Normalize_dT_SG;
  static double* Normalize_BG;

  static double dummy;
};
#endif
