#include "KVFSpallationCut.hh"
#include <iostream>

double KVFSpallationCut :: EPS = 1.0e-6;
int    KVFSpallationCut :: JMAX = 20;
int    KVFSpallationCut :: JMAXP = 21;
int    KVFSpallationCut :: K = 5;
double    KVFSpallationCut :: pi = 3.14159265358979323846;
double    KVFSpallationCut :: R_b = 0;
double    KVFSpallationCut :: R_mu = 0;
double    KVFSpallationCut :: L = 0;
double    KVFSpallationCut :: x_max = 0;
double    KVFSpallationCut :: x_min = 0;

void KVFSpallationCut :: setCondition(double r_b, double r_mu, double l)
{
  R_b=r_b; R_mu=r_mu; L=l;
  if(L>=R_mu){
    x_max = sqrt(R_b*R_b-(L-R_mu)*(L-R_mu))-0.001;
    x_min = -sqrt(R_b*R_b-(L-R_mu)*(L-R_mu))+0.001;
  }
  if(L<R_mu){
    x_max = R_b;
    x_min = -R_b;
  }
}

double KVFSpallationCut::getCutVolume(){
  if(L>=(R_b+R_mu)) return 0;
  return qromb(x_min,x_max);
}

double KVFSpallationCut::qromb(double a, double b)
{
  double ss, dss;
  //double s[(const int)JMAXP+1], h[(const int)JMAXP+1];
  double* s = new double[JMAXP+1];
  double* h = new double[JMAXP+1];
  int j;
  
  h[1] = 1.0;
  for(j=1; j<=JMAX; j++){
    s[j] = trapzd(a, b, j);
    if(j >= K){
      polint(&h[j-K], &s[j-K], K, 0.0, &ss, &dss);
      if(fabs(dss) < EPS*fabs(ss) ){
	delete [] s;
	delete [] h;
	return ss;
      }
    }
    s[j+1]=s[j];
    h[j+1]=0.25*h[j];
  }
  delete [] s;
  delete [] h;
  cerr << "Too many steps in routine qromb" << endl;
  return -1;
}
void 
KVFSpallationCut::polint(double xa[], double ya[], int n, double x, double *y, double *dy)
{
  int i, m, ns = 1;
  double den,dif,dift,ho,hp,w;
  double *c = new double[n+1];
  double *d = new double[n+1];

  dif = fabs( x - xa[1] );

  for( i = 1; i <= n; i++){
    if( ( dift = fabs( x - xa[i] ) ) < dif ){
      ns = i;
      dif = dift;
    }
    c[i] = ya[i];
    d[i] = ya[i];
  }
  *y = ya[ns--];
  for( m = 1; m < n; m++){
    for( i = 1; i <= n - m; i++){
      ho = xa[i] - x;
      hp = xa[i+m] - x;
      w = c[i+1] - d[i];
      if( (den = ho - hp) == 0.0 ) cerr << "Error in routine polint" << endl;
      den = w / den;
      d[i] = hp * den;
      c[i] = ho * den;
    }
    *y += ( *dy = (2*ns < ( n - m ) 
                                   ? c[ns+1] : d[ns--]));
  }
  delete [] d;
  delete [] c;
}

double KVFSpallationCut::trapzd(double a, double b, int n)
{
  double x, tnm, sum, del;
  static double s;
  int it, j;
  if ( n == 1 ){
    return (s=0.5*(b-a)*(S(a)+S(b)));
  } else {
    for (it=1, j=1; j<n-1; j++) it <<=1;
    tnm=it;
    del=(b-a)/tnm;
    x=a+0.5*del;
    for(sum=0.0, j=1; j<=it; j++,x+=del) sum += S(x);
    s=0.5*(s+(b-a)*sum/tnm);
    return s;
  }
}

  

 



    
