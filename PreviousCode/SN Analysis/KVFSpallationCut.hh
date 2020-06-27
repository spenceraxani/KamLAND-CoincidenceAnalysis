#ifndef __KVFSpallationCut_HH
#define __KVFSpallationCut_HH

#include <cmath>

using namespace std;

class KVFSpallationCut{
private:
  static double EPS;
  static int    JMAX;
  static int    JMAXP;
  static int    K;
  static double pi;
  static double R_b;
  static double R_mu;
  static double L;
  static double x_max;
  static double x_min;
public:
  KVFSpallationCut(){}
  ~KVFSpallationCut(){}
  static double getCutVolume(double r_b, double r_mu, double l){
    setCondition(r_b, r_mu, l);
    return getCutVolume();
  }
  static double getCutWeight(double r_b, double r_mu, double l){
    double fiducial_volume = 4.0/3.0*pi * r_b*r_b*r_b;
    return (getCutVolume(r_b, r_mu, l)/fiducial_volume);
  }

  static void setCondition(double r_b, double r_mu, double l);
  static double B(double x){
    if(L==0)return 0;
    return (L*L + R_mu*R_mu - (R_b*R_b - x*x))/ (2*L);
  }
  static double A(double x){  
    if((R_mu*R_mu - B(x) * B(x))<0)return 0;
    return sqrt( R_mu*R_mu - B(x) * B(x) );
  }
  static double theta(double x){
    if(((R_mu*R_mu - B(x) * B(x))<0) || ((R_b*R_b-x*x)==0))return 0;
    return acos((L-B(x))/sqrt(R_b*R_b-x*x));
  }
  static double theta_dash(double x){
    if((R_mu*R_mu - B(x) * B(x))<0 && (L<=(sqrt(R_b*R_b-x*x)-R_mu)))return pi;
    if(L>=(sqrt(R_b*R_b-x*x)+R_mu))return 0;
    return acos(B(x)/R_mu);
  }
  static double S(double x){
    if((L==0&&(x*x>=(R_b*R_b-R_mu*R_mu)))||
       (x*x>(R_b*R_b-(R_mu-L)*(R_mu-L)))) return pi * (R_b*R_b-x*x);
    if (L==0&&(x*x<(R_b*R_b-R_mu*R_mu))) return pi * R_mu * R_mu;
    return theta(x) * ( R_b*R_b -x*x ) + theta_dash(x) * R_mu * R_mu - L * A(x);
  }
  static double getCutVolume();
  static double qromb(double (*func)(double),double a, double b);
  static double qromb(double a, double b);
  static void polint(double xa[], double ya[], int n, double x, double *y, double *dy);
  static double trapzd(double (*func)(double), double a, double b, int n);
  static double trapzd(double a, double b, int n);
};

#endif
