#include <stdio.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

#define pi 3.141592
#define OMEGA_M 0.315
#define OMEGA_L 0.685
#define H_0 67.3        // <- km/s/Mpc
#define C 2.99792458e+5 // <- km/s

/// function ///
double f(double z)
{
  return C / H_0 / sqrt(OMEGA_M * pow((1+z),3) +OMEGA_L );  // <- Mpc
}

  
int main()
{
 
  ifstream l_GRB("/data/work/yuhei/analysis/list/grblist.dat");
  if(! l_GRB){
    cerr << "### ERROR : GRB data not found : " << endl;
    return -1;}


  //// input redshift z ////
  int k=0;
  double Z,T90;
  double z[500],l_d[1000];;
  string line2,name,ymd,hms;

  while(getline(l_GRB,line2)){
    if(line2.empty() || (line2[0] == '#')){
      continue;}
    istringstream stream2(line2);
    if(!(stream2 >> name >> ymd >> hms >> Z >> T90)){
      cerr << "### ERROR : Invalid data line : " << line2 << endl;
      return -1;}
    z[k]=Z;


    //// caluculate lumi_distance ////
    int n,m;
    double a=0,h,
           err=1.,
           epsilon=1.0e-7,
           trapezoid,midpoint,simpson,new_simpson,lumi_distance;

    h = Z-a;
    trapezoid = h * (f(a)+f(Z)) / 2;
    
    for(n=1 ; err>epsilon ; n*=2 ){
      midpoint = 0;
      for(m=1 ; m<=n ; m++) midpoint += f(a+h*(m-0.5));
      midpoint *= h;
      new_simpson = (trapezoid + 2*midpoint) / 3;
      err = fabs(new_simpson - simpson) / fabs(new_simpson);
      simpson = new_simpson;
      h /= 2; trapezoid = (trapezoid + midpoint) /2;
    }
    
    lumi_distance = (1 + Z) * simpson;   /// <- luminosity distance
    l_d[k]=lumi_distance * 3.08568e+18 * 1.0e+6;   /// Mpc -> cm  
    
    cerr << z[k] << " " << l_d[k] << endl;
    k++;
  }
  
}
