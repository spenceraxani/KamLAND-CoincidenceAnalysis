#include "KVFEnergyCorrection.hh"
#include "KVFConstHandler.hh"
#include <fstream>
#include <iostream>
#include <cstdlib>

int KVFEnergyCorrection :: _version = 2;
bool KVFEnergyCorrection :: _IsSetTable = false;

vector<int>    KVFEnergyCorrection :: _run_number_k40_time0;
vector<int>    KVFEnergyCorrection :: _run_number_k40_time0_A2;
vector<int>    KVFEnergyCorrection :: _run_number_k40_time;
vector<int>    KVFEnergyCorrection :: _run_number_k40_z;
vector<int>    KVFEnergyCorrection :: _run_number_k40_z2;
vector<int>    KVFEnergyCorrection :: _run_number_k40_r;
vector<int>    KVFEnergyCorrection :: _run_number_k40_r2;
vector<int>    KVFEnergyCorrection :: _run_number_co_z;
vector<int>    KVFEnergyCorrection :: _run_number_np_z2;
vector<int>    KVFEnergyCorrection :: _run_number_np_rho;
vector<int>    KVFEnergyCorrection :: _run_number_np_rho2;
vector<int>    KVFEnergyCorrection :: _run_number_np_time;
vector<int>    KVFEnergyCorrection :: _run_number_np_time2;

vector<double>    KVFEnergyCorrection :: _bin_k40_z;
vector<double>    KVFEnergyCorrection :: _bin_k40_z2;
vector<double>    KVFEnergyCorrection :: _bin_k40_r;
vector<double>    KVFEnergyCorrection :: _bin_k40_r2;
vector< vector<double> >    KVFEnergyCorrection :: _bin_co_z;
vector<double>    KVFEnergyCorrection :: _bin_np_z2;
vector<double>    KVFEnergyCorrection :: _bin_np_rho;
vector<double>    KVFEnergyCorrection :: _bin_np_rho2;

vector<double>    KVFEnergyCorrection :: _energy_correction_factor_k40_time0;
vector<double>    KVFEnergyCorrection :: _energy_correction_factor_k40_time0_A2;
vector<double>    KVFEnergyCorrection :: _energy_correction_factor_k40_time;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_k40_z;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_k40_z2;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_k40_r;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_k40_r2;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_co_z;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_co_z_A2;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_np_z2;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_np_z2_A2;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_np_rho;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_np_rho_A2;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_np_rho2;
vector< vector<double> >    KVFEnergyCorrection :: _energy_correction_factor_np_rho2_A2;
vector<double>    KVFEnergyCorrection :: _energy_correction_factor_np_time_in;
vector<double>    KVFEnergyCorrection :: _energy_correction_factor_np_time_in_A2;
vector<double>    KVFEnergyCorrection :: _energy_correction_factor_np_time_out;
vector<double>    KVFEnergyCorrection :: _energy_correction_factor_np_time_in2;
vector<double>    KVFEnergyCorrection :: _energy_correction_factor_np_time_out2;

double*        KVFEnergyCorrection :: _factor = new double[2];
double*        KVFEnergyCorrection :: _factorA2 = new double[2];
kuFUNC*        KVFEnergyCorrection :: _20inchCorrection;
kuFUNC*        KVFEnergyCorrection :: _20inchCorrectionA2;


KVFEnergyCorrection :: KVFEnergyCorrection()
{
  if(_IsSetTable == false){
    
    // K40 time correction (traditional)
    string energy_correction_filename1 = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/old/vf/run-K40peak.dat");
    string energy_correction_filename2 = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/run-K40peak.dat");
    string energy_correction_filename;
    
    int run;
    double K, K_base;
    static const double K_base1 = 1.385;
    static const double K_base2 = 1.387;    
    ifstream f;
    
    if(KVFConstHandler::getVersion()==1){
      energy_correction_filename = energy_correction_filename1;
      K_base = K_base1;
    }
    else{
      energy_correction_filename = energy_correction_filename2;
      K_base = K_base2;
    }      
    
    f.open(energy_correction_filename.c_str());
    if(!f){
      cerr << "ERROR: Cannot open K40 time correction file (traditional): ";
      cerr << energy_correction_filename << endl;
      abort();
    }
    
    string line;
    int NofLine=0;
    while(getline(f,line)){ NofLine++; }
    _run_number_k40_time0.clear();
    _energy_correction_factor_k40_time0.clear();
    
    f.clear();
    f.seekg(0);
    while(f>>run>>K){
      _run_number_k40_time0.push_back(run);
      _energy_correction_factor_k40_time0.push_back(K_base/K);
    }
    f.close();
    
    energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/run-K40peak-A2.dat");
    
    f.clear();
    f.open(energy_correction_filename.c_str());
    if(!f){
      cerr << "ERROR: Cannot open K40 time correction file (A2): ";
      cerr << energy_correction_filename << endl;
      abort();
    }
    
    NofLine=0;
    while(getline(f,line)){ NofLine++; }
    _run_number_k40_time0_A2.clear();
    _energy_correction_factor_k40_time0_A2.clear();
    
    f.clear();
    f.seekg(0);
    while(f>>run>>K){
      _run_number_k40_time0_A2.push_back(run);
      _energy_correction_factor_k40_time0_A2.push_back(K_base/K);
    }
    f.close();

    
    if(_version==1){
      // k40 time correction after purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/after-distillation/run-K40peak.dat");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      
      if(!f){
	cerr << "ERROR: Cannot open K40 time correction file (purification): ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      K_base = 1.387*2.211/2.203;

      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      _run_number_k40_time.clear();
      _energy_correction_factor_k40_time.clear();
      
      f.clear();
      f.seekg(0);
      while(f>>run>>K){
	_run_number_k40_time.push_back(run);
	_energy_correction_factor_k40_time.push_back(K_base/K);
      }
      f.close();
      
      
      // K40 Z correction after purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/after-distillation/z_depend_k40.table");
      string bin_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/after-distillation/z_bin.table");
      
      ifstream f2;
      f.clear();
      f.open(energy_correction_filename.c_str());
      f2.open(bin_filename.c_str());
      if(!f || !f2){
	cerr << "ERROR: Cannot open K40 Z correction or bin file: ";
	if(!f){
	  cerr << energy_correction_filename << endl;
	}
	if(!f2){
	  cerr << bin_filename << endl;
	}
      abort();
      }
      
      int dummy; 
      double bin_tmp;
      int i=0;
      _bin_k40_z.clear();

      while(f2>>dummy>>bin_tmp){
	_bin_k40_z.push_back(bin_tmp);
	i++;
      }
      f2.close();
      
      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      
      int nbin=i;
      i=0;
      int j=0;
      double element;
      _run_number_k40_z.clear();
      _energy_correction_factor_k40_z.clear();
      _energy_correction_factor_k40_z.resize(NofLine);
      
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i%(nbin+1)==0){
	  _run_number_k40_z.push_back((int)element);
	  j=0;
	}
	else if(i%nbin==0){
	  ;
	}
	else{
	  _energy_correction_factor_k40_z[i/(nbin+1)].push_back(element);
	  j++;
	}
	i++;
      }
      f.close();
      
      
      // K40 Z correction during purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/during-distillation/z_depend_k40.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      if(!f){
	cerr << "ERROR: Cannot open K40 Z correction file: ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      
      i=0,j=0;
      _run_number_k40_z2.clear();
      _energy_correction_factor_k40_z2.clear();
      _energy_correction_factor_k40_z2.resize(NofLine);
      
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i%(nbin+1)==0){
	  _run_number_k40_z2.push_back((int)element);
	  j=0;
	}
	else if(i%nbin==0){
	  ;
	}
	else{
	  _energy_correction_factor_k40_z2[i/(nbin+1)].push_back(element);
	  j++;
	}
	i++;
      }
      f.close();
      
      _bin_k40_z2.assign(_bin_k40_z.begin(),_bin_k40_z.end());
      
      
      // K40 R correction after purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/after-distillation/r_depend_k40.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      
      if(!f){
	cerr << "ERROR: Cannot open K40 R correction or bin file: ";
	if(!f){
	  cerr << energy_correction_filename << endl;
	}
	abort();
      }
      
      nbin=10; 
      _bin_k40_r.clear();

      for(int k=0;k<nbin+1;k++){
	_bin_k40_r.push_back((double)(55+k)*0.1);
      }
      
      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      
      i=0,j=0;
      K_base=1.387;
      _run_number_k40_r.clear();
      _energy_correction_factor_k40_r.clear();
      _energy_correction_factor_k40_r.resize(NofLine);
      
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i%(nbin+1)==0){
	  _run_number_k40_r.push_back((int)element);
	  j=0;
	}
	else{
	  _energy_correction_factor_k40_r[i/(nbin+1)].push_back(K_base/element);
	  j++;
	}
	i++;
      }
      f.close();
      
      
      // K40 R correction during purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/during-distillation/r_depend_k40.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      if(!f){
	cerr << "ERROR: Cannot open K40 R correction file: ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      _bin_k40_r2.clear();
      _bin_k40_r2.assign(_bin_k40_r.begin(),_bin_k40_r.end());
      
      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      
      i=0,j=0;
      _run_number_k40_r2.clear();
      _energy_correction_factor_k40_r2.clear();
      _energy_correction_factor_k40_r2.resize(NofLine);
      
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i%(nbin+1)==0){
	  _run_number_k40_r2.push_back((int)element);
	  j=0;
	  i++;
	}
	else{
	  _energy_correction_factor_k40_r2[i/(nbin+1)].push_back(element);
	  j++;
	  i++;
	}
      }
      f.close();
      
      
      // Co60 Z correction after purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/after-distillation/z_depend_co.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      
      if(!f){
	cerr << "ERROR: Cannot open Co60 Z correction file: ";
	if(!f){
	  cerr << energy_correction_filename << endl;
	}
	abort();
      }
      
      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      
      _run_number_co_z.clear();
      _bin_co_z.clear();
      _energy_correction_factor_co_z.clear(); 

      _bin_co_z.resize(NofLine);
      _energy_correction_factor_co_z.resize(NofLine); 
      
      i=0;
      int NofRun=-1;
      vector<double> v;
      v.clear();

      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i==0){
	  _run_number_co_z.push_back((int)element);
	  NofRun++;
	}
	if(element==-10000){    
	  for(j=1;j<v.size();j++){
	    if(j%2!=0){
	      _bin_co_z[NofRun].push_back(v[j]);
	    }
	    else{
	      _energy_correction_factor_co_z[NofRun].push_back(v[j]);
	    }
	  }
	  i=0;
	  v.clear();
	  continue;
	}
	v.push_back(element);
	i++;
      }
      f.close();
      
      
      // Np Z correction during purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/during-distillation/z_depend_np.table");
      bin_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/during-distillation/zbin_np.table");
      
      f.clear();
      f2.clear();
      f.open(energy_correction_filename.c_str());
      f2.open(bin_filename.c_str());
      if(!f || !f2){
	cerr << "ERROR: Cannot open Np Z correction or bin file: ";
	if(!f){
	  cerr << energy_correction_filename << endl;
	}
	if(!f2){
	  cerr << bin_filename << endl;
	}
	abort();
      }
      
      i=0;
      _bin_np_z2.clear();

      while(f2>>dummy>>bin_tmp){
	_bin_np_z2.push_back(bin_tmp);
	i++;
      }
      f2.close();
      
      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      
      nbin=i-1;    
      i=0,j=0;
      _run_number_np_z2.clear();
      _energy_correction_factor_np_z2.clear();
      _energy_correction_factor_np_z2.resize(NofLine);
      
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i%(nbin+1)==0){
	  _run_number_np_z2.push_back((int)element);
	  j=0;
	}
	else{
	  _energy_correction_factor_np_z2[i/(nbin+1)].push_back(element);
	  j++;
	}
	i++;
      }
      f.close();
      
      
      // Np Rho correction after purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/after-distillation/rho_depend_np.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
    
      if(!f){
	cerr << "ERROR: Cannot open Np Rho correction file: ";
	if(!f){
	  cerr << energy_correction_filename << endl;
	}
	abort();
      }
      
      nbin=55;
      _bin_np_rho.clear();

      for(i=0;i<nbin+1;i++){
	_bin_np_rho.push_back(0.1*(double)i);
      }
      
      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      
      _run_number_np_rho.clear();
      _energy_correction_factor_np_rho.clear(); 
      _energy_correction_factor_np_rho.resize(NofLine);
      
      i=0,j=0;
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i%(nbin+1)==0){
	  _run_number_np_rho.push_back((int)element);
	  j=0;
	}
	else{
	  _energy_correction_factor_np_rho[i/(nbin+1)].push_back(element);
	  j++;
	}
	i++;
      }
      f.close();
      
      
      // Np Rho correction during purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/during-distillation/rho_depend_np.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      if(!f){
	cerr << "ERROR: Cannot open Np Rho correction file: ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      nbin=10;
      _bin_np_rho2.clear();
      _bin_np_rho2.push_back(0.0);
      for(int k=0;k<nbin+1;k++){
	_bin_np_rho2.push_back((double)(10+5*k)*0.1);
      }
      
      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      
      i=0,j=0;
      _run_number_np_rho2.clear();
      _energy_correction_factor_np_rho2.clear();
      _energy_correction_factor_np_rho2.resize(NofLine);
    
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i%(nbin+1)==0){
	  _run_number_np_rho2.push_back((int)element);
	  j=0;
	  i++;
	}
	else{
	  _energy_correction_factor_np_rho2[i/(nbin+1)].push_back(element);
	  j++;
	  i++;
	}
      }
      f.close();
      
      
      // Np time correction after purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/after-distillation/run_depend_np.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      if(!f){
	cerr << "ERROR: Cannot open Np time correction file: ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      double np,np2;
      double np_base=2.211;
      int k=1;
      _run_number_np_time.clear();
      _energy_correction_factor_np_time_out.clear();
      _energy_correction_factor_np_time_in.clear();
      
      while(f>>run>>np>>np2){
	_run_number_np_time.push_back(run);
	_energy_correction_factor_np_time_out.push_back(np_base/np);
	_energy_correction_factor_np_time_in.push_back(np_base/np2);
      }
      f.close();
      
      
      // Np time correction during purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/during-distillation/run_depend_np.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      if(!f){
	cerr << "ERROR: Cannot open energy correction file: ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      np_base=2.211;
      k=1;
      _run_number_np_time2.clear();
      _energy_correction_factor_np_time_out2.clear();
      _energy_correction_factor_np_time_in2.clear();
      
      while(f>>run>>np>>np2){
	_run_number_np_time2.push_back(run);
	_energy_correction_factor_np_time_out2.push_back(np_base/np);
	_energy_correction_factor_np_time_in2.push_back(np_base/np2);
      }
      f.close();
      
      
      // 20inch correction after purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/after-distillation/20inch_correction.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());    
      if(!f){
	cerr << "ERROR: Cannot open 20inch correction file: " << energy_correction_filename << endl;
	abort();
      }
      
      double Energy[2000], Correction[2000];
      float energy, correction;
      int N=0;
      
      while(f >> energy >> correction){
	if(N==0) _factor[0]=correction;
	Energy[N] = energy;    
	Correction[N] = correction;
	N++;
      } 
      f.close();
      
      _factor[1]=Correction[N-1];  
      _20inchCorrection = new kuFUNC(N,Energy,Correction);
      
    }
    
    else if(_version==2){
      // Co60 Z correction after purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/new-energy-correction/z_depend_co.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      
      if(!f){
	cerr << "ERROR: Cannot open Co60 Z correction file: ";
	if(!f){
	  cerr << energy_correction_filename << endl;
	}
	abort();
      }
      
      int NofLine=0;
      string line;
      while(getline(f,line)){ NofLine++; }
      
      _run_number_co_z.clear();
      _bin_co_z.clear();
      _energy_correction_factor_co_z.clear(); 
      
      _bin_co_z.resize(NofLine);
      _energy_correction_factor_co_z.resize(NofLine); 
      
      int i=0;
      int j=0;
      int NofRun=-1;
      vector<double> v;
      v.clear();
      
      double element;
      
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i==0){
	  NofRun++;
	  _run_number_co_z.push_back((int)element);	
	}
	if(element==-10000){    
	  for(j=1;j<v.size();j++){
	    if(j%2!=0){
	      _bin_co_z[NofRun].push_back(v[j]);
	    }
	    else{
	      _energy_correction_factor_co_z[NofRun].push_back(v[j]);
	    }
	  }
	  i=0;
	  v.clear();
	  continue;
	}
	v.push_back(element);
	i++;
      }
      f.close();
      
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/NewEnergyCorrection/z_depend_co_A2.table");
      //energy_correction_filename = "/data/sp04/tmpdata/konno/NewEnergyCorrection/z_depend_co_A2.table";
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      
      if(!f){
	cerr << "ERROR: Cannot open Co60 Z correction file (A2): ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      
      _energy_correction_factor_co_z_A2.clear(); 
      _energy_correction_factor_co_z_A2.resize(NofLine); 
      
      i=0;
      NofRun=-1;
      v.clear();
      
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i==0){
	  NofRun++;
	}
	if(element==-10000){    
	  for(j=1;j<v.size();j++){
	    if(j%2!=0){
	      ;
	    }
	    else{
	      _energy_correction_factor_co_z_A2[NofRun].push_back(v[j]);
	    }
	  }
	  i=0;
	  v.clear();
	  continue;
	}
	v.push_back(element);
	i++;
      }
      f.close();
      
      
      // Np Z correction during purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/new-energy-correction/z_depend_np.table");
      string bin_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/new-energy-correction/zbin_np.table");
      
      ifstream f2; 
      f.clear();
      f.open(energy_correction_filename.c_str());
      f2.open(bin_filename.c_str());
      if(!f || !f2){
	cerr << "ERROR: Cannot open Np Z correction or bin file: ";
	if(!f){
	  cerr << energy_correction_filename << endl;
	}
	if(!f2){
	  cerr << bin_filename << endl;
	}
	abort();
      }
      
      _bin_np_z2.clear();

      double dummy, bin_tmp;
      while(f2>>dummy>>bin_tmp){
	_bin_np_z2.push_back(bin_tmp);
      }
      f2.close();
      
      i=0,j=0;
      int nbin=_bin_np_z2.size()-1;
      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      _run_number_np_z2.clear();
      _energy_correction_factor_np_z2.clear();
      _energy_correction_factor_np_z2.resize(NofLine);
      
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i%(nbin+1)==0){
	  _run_number_np_z2.push_back((int)element);
	  j=0;
	}
	else{
	  _energy_correction_factor_np_z2[i/(nbin+1)].push_back(element);
	  j++;
	}
	i++;
      }
      f.close();
      
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/new-energy-correction/z_depend_np_A2.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      
      if(!f){
	cerr << "ERROR: Cannot open Np Z correction (A2): ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      i=0,j=0;
      _energy_correction_factor_np_z2_A2.clear();
      _energy_correction_factor_np_z2_A2.resize(NofLine);
      
      while(f>>element){
	if(i%(nbin+1)==0){
	  j=0;
	}
	else{
	  _energy_correction_factor_np_z2_A2[i/(nbin+1)].push_back(element);
	  j++;
	}
	i++;
      }
      f.close();
      

      // Np Rho correction after purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/new-energy-correction/rho_depend_np.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      
      if(!f){
	cerr << "ERROR: Cannot open Np Rho correction file: ";
	if(!f){
	  cerr << energy_correction_filename << endl;
	}
	abort();
      }
    
      nbin=28;
      _bin_np_rho.clear();
      _bin_np_rho.push_back(0.0);
      for(int k=0;k<nbin-1;k++){
	_bin_np_rho.push_back((double)(4+2*k)*0.1);
      }
      _bin_np_rho.push_back(6.0);
      
      NofLine=0;
      while(getline(f,line)){ NofLine++; }
      
      _run_number_np_rho.clear();
      _energy_correction_factor_np_rho.clear(); 
      _energy_correction_factor_np_rho.resize(NofLine);
      
      i=0,j=0;
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i%(nbin+1)==0){
	  _run_number_np_rho.push_back((int)element);
	  j=0;
	}
	else{
	  _energy_correction_factor_np_rho[i/(nbin+1)].push_back(element);
	  j++;
	}
	i++;
      }
      f.close();
      
      
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/new-energy-correction/rho_depend_np_A2.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      
      if(!f){
	cerr << "ERROR: Cannot open Np Rho correction file (A2): ";
	cerr << energy_correction_filename << endl;
	abort();
      }
    
      _energy_correction_factor_np_rho_A2.clear(); 
      _energy_correction_factor_np_rho_A2.resize(NofLine);
      
      i=0,j=0;
      
      while(f>>element){
	if(i%(nbin+1)==0){
	  j=0;
	}
	else{
	  _energy_correction_factor_np_rho_A2[i/(nbin+1)].push_back(element);
	  j++;
	}
	i++;
      }
      f.close();
      

      // Np Rho correction during purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/new-energy-correction/rho_depend_np2.table");
      
      f.clear(); 
      f.open(energy_correction_filename.c_str());
      if(!f){
	cerr << "ERROR: Cannot open Np Rho correction file: ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      i=0,j=0;
      nbin=11;
      NofLine=0;
      while(getline(f,line)){ NofLine++; }  
      _run_number_np_rho2.clear();
      _energy_correction_factor_np_rho2.clear();
      _energy_correction_factor_np_rho2.resize(NofLine);
      
      f.clear();
      f.seekg(0);
      while(f>>element){
	if(i%(nbin+1)==0){
	  _run_number_np_rho2.push_back((int)element);
	  j=0;
	  i++;
	}
	else{
	  _energy_correction_factor_np_rho2[i/(nbin+1)].push_back(element);
	  j++;
	  i++;
	}
      }
      f.close();
      
      _bin_np_rho2.clear();
      _bin_np_rho2.push_back(0.0);
      _bin_np_rho2.push_back(1.2);
      _bin_np_rho2.push_back(2.0);
      for(int k=0;k<nbin-3;k++){
	_bin_np_rho2.push_back((double)(24+4*k)*0.1);
      }
      _bin_np_rho2.push_back(6.0);
      
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/new-energy-correction/rho_depend_np2_A2.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      if(!f){
	cerr << "ERROR: Cannot open Np Rho correction file (A2): ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      i=0,j=0;
      _energy_correction_factor_np_rho2_A2.clear();
      _energy_correction_factor_np_rho2_A2.resize(NofLine);
      
      while(f>>element){
	if(i%(nbin+1)==0){
	  j=0;
	  i++;
	}
	else{
	  _energy_correction_factor_np_rho2_A2[i/(nbin+1)].push_back(element);
	  j++;
	  i++;
	}
      }
      f.close();
      
      
      // Np time correction
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/new-energy-correction/run_depend_np.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      if(!f){
	cerr << "ERROR: Cannot open Np time correction file: ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      NofLine=0;
      while(getline(f,line)){ NofLine++; }  
      
      double np;
      double np_base=2.214;
      _run_number_np_time.clear();
      _energy_correction_factor_np_time_in.clear();
      
      f.clear();
      f.seekg(0);
      while(f>>run>>np){
	_run_number_np_time.push_back(run);
	_energy_correction_factor_np_time_in.push_back(np_base/np);
      }
      f.close();
      
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/new-energy-correction/run_depend_np_A2.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());
      if(!f){
	cerr << "ERROR: Cannot open Np time correction file (A2): ";
	cerr << energy_correction_filename << endl;
	abort();
      }
      
      np_base=2.211;
      _energy_correction_factor_np_time_in_A2.clear();
      
      while(f>>run>>np){
	_energy_correction_factor_np_time_in_A2.push_back(np_base/np);
      }
      f.close();
      
      
      // 20inch correction after purification
      energy_correction_filename = ((string) getenv("KAMLAND_CONST_DIR")) + ((string) "/vf/new-energy-correction/20inch_correction.table");
      
      f.clear();
      f.open(energy_correction_filename.c_str());    
      if(!f){
	cerr << "ERROR: Cannot open 20inch correction file: " << energy_correction_filename << endl;
	abort();
      }
      
      double Energy[1000], Correction[1000];
      double CorrectionA2[1000];
      float energy, correction, correctionA2;
      int N=0;
      
      while(f >> energy >> correction >> correctionA2){
	if(N==0){
	  _factor[0]=correction;
	  _factorA2[0]=correctionA2;
	}
	Energy[N] = energy;    
	Correction[N] = correction;
	CorrectionA2[N] = correctionA2;
	N++;
      } 
      f.close();
      
      _factor[1]=Correction[N-1];  
      _factorA2[1]=CorrectionA2[N-1];  
      
      _20inchCorrection = new kuFUNC(N,Energy,Correction);
      _20inchCorrectionA2 = new kuFUNC(N,Energy,CorrectionA2);
      
    }
    
    
    else if(_version>2){
      cerr << "Error: illegal version number" << endl;
      abort();
    }
    
    _IsSetTable = true;
  }
}


bool KVFEnergyCorrection :: setVersion(int v)
{
  if(v>2){
    cerr << "Error: illegal version number" << endl;
    return false;
  }    
  _version = v;
  return true;
}


int KVFEnergyCorrection :: getVersion(){
  return _version;
}


float KVFEnergyCorrection :: RunDependenceOfVisibleEnergy(int run_number, bool IsK40Corr, bool IsA2, float R)
{
  float correction_factor;

  if(run_number <= 6801 || IsK40Corr==true){
    if(IsA2==true)
      correction_factor = KVFEnergyCorrection::_K40RunDependentEnergyCorrection(run_number, &_run_number_k40_time0_A2, &_energy_correction_factor_k40_time0_A2);      
    else{
      correction_factor = KVFEnergyCorrection::_K40RunDependentEnergyCorrection(run_number, &_run_number_k40_time0, &_energy_correction_factor_k40_time0);      
    }
  }

  else if(_version==1){
    if(6801<run_number&&run_number<6940 || 7832<run_number&&run_number<8293)
      correction_factor = KVFEnergyCorrection::_NpRunDependentEnergyCorrection(run_number, R, &_run_number_np_time, &_energy_correction_factor_np_time_in2, &_energy_correction_factor_np_time_out2);
    else if(6940<=run_number && run_number<=7832)
      correction_factor = KVFEnergyCorrection::_K40RunDependentEnergyCorrection(run_number, &_run_number_k40_time, &_energy_correction_factor_k40_time);
    else
      correction_factor = KVFEnergyCorrection::_NpRunDependentEnergyCorrection(run_number, R, &_run_number_np_time, &_energy_correction_factor_np_time_in, &_energy_correction_factor_np_time_out);      
  } 
  
  else{
    if(IsA2==true){
      correction_factor = KVFEnergyCorrection::_NpRunDependentEnergyCorrection(run_number, &_run_number_np_time, &_energy_correction_factor_np_time_in_A2);
    }
    else{
      correction_factor = KVFEnergyCorrection::_NpRunDependentEnergyCorrection(run_number, &_run_number_np_time, &_energy_correction_factor_np_time_in);      
    }
  }

  return correction_factor;
}

 
float KVFEnergyCorrection :: ZPositionDependenceOfVisibleEnergy(int run_number, bool IsA2, float R, float z)
{
  float correction_factor;
  
  if(run_number <= 6801 || _version==0) return 1;

  else if(_version==1){
    if(6801<run_number&&run_number<6940 || 7832<run_number&&run_number<8293){
      if(R/100<5.5)
	correction_factor = KVFEnergyCorrection::_ZPositionDependentEnergyCorrection(run_number, z, &_run_number_np_z2, &_bin_np_z2, &_energy_correction_factor_np_z2);
      else
	correction_factor = KVFEnergyCorrection::_ZPositionDependentEnergyCorrection(run_number, z, &_run_number_k40_z2, &_bin_k40_z2, &_energy_correction_factor_k40_z2);      
    }
    else{
      if(R/100<5.5)
	correction_factor = KVFEnergyCorrection::_CoZPositionDependentEnergyCorrection(run_number, z, &_energy_correction_factor_co_z);
      else
	correction_factor = KVFEnergyCorrection::_ZPositionDependentEnergyCorrection(run_number, z, &_run_number_k40_z, &_bin_k40_z, &_energy_correction_factor_k40_z);
    }
  }

  else{
    if(IsA2==true){
      if(6801<run_number&&run_number<6940 || 7832<run_number&&run_number<8293){
	correction_factor = KVFEnergyCorrection::_ZPositionDependentEnergyCorrection(run_number, z, &_run_number_np_z2, &_bin_np_z2, &_energy_correction_factor_np_z2_A2);      
      }
      else{
	correction_factor = KVFEnergyCorrection::_CoZPositionDependentEnergyCorrection(run_number, z, &_energy_correction_factor_co_z_A2);
      }
    }
    else{
      if(6801<run_number&&run_number<6940 || 7832<run_number&&run_number<8293){
	correction_factor = KVFEnergyCorrection::_ZPositionDependentEnergyCorrection(run_number, z, &_run_number_np_z2, &_bin_np_z2, &_energy_correction_factor_np_z2);      
      }
      else{
	correction_factor = KVFEnergyCorrection::_CoZPositionDependentEnergyCorrection(run_number, z, &_energy_correction_factor_co_z);
      }      
    }    
  }

  return correction_factor;
}


float KVFEnergyCorrection :: OffAxisPositionDependenceOfVisibleEnergy(int run_number, bool IsA2, float R, float Rho)
{
  float correction_factor;

  if(run_number <= 6801 || _version==0) return 1;
 
  else if(_version==1){
    if(6801<run_number&&run_number<6940 || 7832<run_number&&run_number<8293){
      if(R/100<5.5)
	correction_factor = KVFEnergyCorrection::_OffAxisPositionDependentEnergyCorrection(run_number, Rho, &_run_number_np_rho2, &_bin_np_rho2, &_energy_correction_factor_np_rho2);
      else
	correction_factor = KVFEnergyCorrection::_OffAxisPositionDependentEnergyCorrection(run_number, R, &_run_number_k40_r2, &_bin_k40_r2, &_energy_correction_factor_k40_r2);
    }
    else{
      if(R/100<5.5)
	correction_factor = KVFEnergyCorrection::_OffAxisPositionDependentEnergyCorrection(run_number, Rho, &_run_number_np_rho, &_bin_np_rho, &_energy_correction_factor_np_rho);        
      else
	correction_factor = KVFEnergyCorrection::_OffAxisPositionDependentEnergyCorrection(run_number, R, &_run_number_k40_r, &_bin_k40_r, &_energy_correction_factor_k40_r);        
    } 
  }

  else{
    if(IsA2==true){
      if(6801<run_number&&run_number<6940 || 7832<run_number&&run_number<8293)
	correction_factor = KVFEnergyCorrection::_OffAxisPositionDependentEnergyCorrection(run_number, Rho, &_run_number_np_rho2, &_bin_np_rho2, &_energy_correction_factor_np_rho2_A2);
      else
	correction_factor = KVFEnergyCorrection::_OffAxisPositionDependentEnergyCorrection(run_number, Rho, &_run_number_np_rho, &_bin_np_rho, &_energy_correction_factor_np_rho_A2);  
    }
    else{
      if(6801<run_number&&run_number<6940 || 7832<run_number&&run_number<8293)
	correction_factor = KVFEnergyCorrection::_OffAxisPositionDependentEnergyCorrection(run_number, Rho, &_run_number_np_rho2, &_bin_np_rho2, &_energy_correction_factor_np_rho2);
      else
	correction_factor = KVFEnergyCorrection::_OffAxisPositionDependentEnergyCorrection(run_number, Rho, &_run_number_np_rho, &_bin_np_rho, &_energy_correction_factor_np_rho);  
    }
  }

  return correction_factor;
}


float KVFEnergyCorrection :: CorrectionOf20inchEnergy(int run_number, float visible_energy, bool IsA2)
{
  float correction_factor;
  
  if(run_number <= 6801 || _version==0) return 1;

  else if(_version==1){
    correction_factor = KVFEnergyCorrection::_20inchEnergyCorrection(visible_energy, _factor, _20inchCorrection);        
  }

  else{
    if(IsA2==true){
      correction_factor = KVFEnergyCorrection::_20inchEnergyCorrection(visible_energy, _factorA2, _20inchCorrectionA2);
    }
    else{
      correction_factor = KVFEnergyCorrection::_20inchEnergyCorrection(visible_energy, _factor, _20inchCorrection);
    }
  }

  return correction_factor;
}


float KVFEnergyCorrection :: _K40RunDependentEnergyCorrection(int run_number, vector<int>* _run_number, vector<double>* _energy_correction_factor)
{
  if(run_number < (*_run_number)[0])
    return (*_energy_correction_factor)[0];
  if(run_number > (*_run_number)[(*_run_number).size()-1])
    return (*_energy_correction_factor)[(*_run_number).size()-1];
  
  int last_index = 0;
  if(last_index >(int)(*_run_number).size()) last_index = (*_run_number).size()-1;
  else if(last_index <0) last_index = 0;
  
  if(run_number == (*_run_number)[last_index]){}
  else if(run_number < (*_run_number)[last_index]){
    while(true){
      if(last_index==0) break;
      last_index--;
      if(run_number>=(*_run_number)[last_index]) break;
    }
  }
  else{
    while(true){
      if(last_index==((int)(*_run_number).size()-1)) break;
      last_index++;
      if(run_number<(*_run_number)[last_index]){
	last_index--;
	break;
      }
    }
  }
  
  return (*_energy_correction_factor)[last_index];
}


float KVFEnergyCorrection :: _NpRunDependentEnergyCorrection(int run_number, float r, vector<int>* _run_number, vector<double>* _energy_correction_factor_in, vector<double>* _energy_correction_factor_out)
{
  if(run_number > (*_run_number)[(*_run_number).size()-1]){
    if(r/100>=5.5) 
      return (*_energy_correction_factor_out)[(*_run_number).size()-1];
    else
      return (*_energy_correction_factor_in)[(*_run_number).size()-1];
  }
  
  static int last_index = 0;
  if(last_index >(int)(*_run_number).size()) 
    last_index = (*_run_number).size()-1;
  else if(last_index <0) last_index = 0;

  if(run_number == (*_run_number)[last_index])
    {}
  else if(run_number < (*_run_number)[last_index]){
    while(true){
      if(last_index==0) break;
      last_index--;
      if(run_number>=(*_run_number)[last_index]) break;
    }
  }
  else{
    while(true){
      if(last_index==((int)(*_run_number).size()-1)) break;
      last_index++;
      if(run_number<(*_run_number)[last_index]){
	last_index--;
	break;
      }
    }
  }

  if(r/100>=5.5){
    return (*_energy_correction_factor_out)[last_index];
  }
  else
    return (*_energy_correction_factor_in)[last_index];  
}


float KVFEnergyCorrection :: _NpRunDependentEnergyCorrection(int run_number, vector<int>* _run_number, vector<double>* _energy_correction_factor)
{
  if(run_number > (*_run_number)[(*_run_number).size()-1]){
    return (*_energy_correction_factor)[(*_run_number).size()-1];
  }
  
  static int last_index = 0;
  if(last_index >(int)(*_run_number).size()) 
    last_index = (*_run_number).size()-1;
  else if(last_index <0) last_index = 0;

  if(run_number == (*_run_number)[last_index])
    {}
  else if(run_number < (*_run_number)[last_index]){
    while(true){
      if(last_index==0) break;
      last_index--;
      if(run_number>=(*_run_number)[last_index]) break;
    }
  }
  else{
    while(true){
      if(last_index==((int)(*_run_number).size()-1)) break;
      last_index++;
      if(run_number<(*_run_number)[last_index]){
	last_index--;
	break;
      }
    }
  }

  return (*_energy_correction_factor)[last_index];  
}


float KVFEnergyCorrection :: _ZPositionDependentEnergyCorrection(int run_number, float z, vector<int>* _run_number, vector<double>* _bin, vector< vector<double> >* _energy_correction_factor)
{  
  int nbin=(*_bin).size()-1;

  if((*_run_number).size()==1 || run_number >= (*_run_number)[(*_run_number).size()-1]){
    if(z/100<(*_bin)[0]){
      return (*_energy_correction_factor)[(*_run_number).size()-1][0];
    }
    else if(z/100>=(*_bin)[nbin]){
      return (*_energy_correction_factor)[(*_run_number).size()-1][nbin-1];
    }
    else{
      for(int i=0;i<nbin;i++){
	if((*_bin)[i]<=z/100 && z/100<(*_bin)[i+1]){
	  return (*_energy_correction_factor)[(*_run_number).size()-1][i];
	}
      }
    }
  }
  
  int i2=0;
  int last_diff=10000;
  for(int i=0;i<(*_run_number).size();i++){
    int diff=-10000;
    diff=abs(run_number - (*_run_number)[i]);
    if(run_number < (*_run_number)[i]){   
      i2=(diff<=last_diff)?i:i-1;
      break;
    }
    else{
      last_diff=diff;
      continue;
    }
  }
  
  if(z/100<(*_bin)[0])
    return (*_energy_correction_factor)[i2][0];
  else if(z/100>=(*_bin)[nbin])
    return (*_energy_correction_factor)[i2][nbin-1];
  else{
    for(int i=0;i<nbin;i++){
      if((*_bin)[i]<=z/100 && z/100<(*_bin)[i+1]){
	return (*_energy_correction_factor)[i2][i];
      }
    }
  }
}


float KVFEnergyCorrection :: _CoZPositionDependentEnergyCorrection(int run_number, float z, vector< vector<double> >* _energy_correction_factor)
{
  if(_run_number_co_z.size()==1 
     || run_number >= _run_number_co_z[_run_number_co_z.size()-1]){   
    if(_bin_co_z[_run_number_co_z.size()-1][_bin_co_z[_run_number_co_z.size()-1].size()-1] >= z/100)
      return (*_energy_correction_factor)[_run_number_co_z.size()-1][_bin_co_z[_run_number_co_z.size()-1].size()-1];
    for(int i=1;i<_bin_co_z[_run_number_co_z.size()-1].size();i++){
      if(_bin_co_z[_run_number_co_z.size()-1][i] < z/100){
	return (*_energy_correction_factor)[_run_number_co_z.size()-1][i-1];
      }
    }
  }
  
  int i2=0;
  int last_diff=10000;
  for(int i=0;i<_run_number_co_z.size();i++){
    int diff=-10000;
    diff = abs(run_number - _run_number_co_z[i]);
    if(run_number < _run_number_co_z[i]){   
      i2 = (diff <= last_diff) ? i:i-1;
      break;
    }
    else{
      last_diff = diff;
      continue;
    }
  }

  if(_bin_co_z[i2][_bin_co_z[i2].size()-1] >= z/100)
    return (*_energy_correction_factor)[i2][_bin_co_z[i2].size()-1];
  for(int j=1;j<_bin_co_z[i2].size();j++){
    if(_bin_co_z[i2][j] < z/100){
      return (*_energy_correction_factor)[i2][j-1];	  
    }
  }
}


float KVFEnergyCorrection :: _OffAxisPositionDependentEnergyCorrection(int run_number, float off_axis, vector<int>* _run_number, vector<double>* _bin, vector< vector<double> >* _energy_correction_factor)
{
  int nbin=(*_bin).size()-1;

  if((*_run_number).size()==1 || run_number >= (*_run_number)[(*_run_number).size()-1]){
    if(off_axis/100<0)
      return 1;
    if(off_axis/100>=(*_bin)[nbin]){
      return (*_energy_correction_factor)[(*_run_number).size()-1][nbin-1];
    }
    else{
      for(int i=0;i<nbin;i++){
	if((*_bin)[i] <= off_axis/100 && off_axis/100 < (*_bin)[i+1]){
	  return (*_energy_correction_factor)[(*_run_number).size()-1][i];
	}
      }
    }
  }
  
  int i2=0;
  int last_diff=10000;
  for(int i=0;i<(*_run_number).size();i++){
    int diff=-10000;
    diff = abs(run_number - (*_run_number)[i]);
    if(run_number < (*_run_number)[i]){   
      i2 = (diff <= last_diff) ? i:i-1;
      break;
    }
    else{
      last_diff=diff;
      continue;
    }
  }

  if(off_axis/100<0){
    return 1;
  }
  else if(off_axis/100>=(*_bin)[nbin]){
    return (*_energy_correction_factor)[i2][nbin-1];
  }
  else{
    for(int i=0;i<nbin;i++){
      if((*_bin)[i] <= off_axis/100 && off_axis/100 < (*_bin)[i+1]){
	return (*_energy_correction_factor)[i2][i];
      }
    }
  }
}

 
float KVFEnergyCorrection :: _20inchEnergyCorrection(float visible_energy, double* _factor, kuFUNC* _energy_correction_function)
{
  if(visible_energy<0.02)       return _factor[0];
  else if(6.76<visible_energy) return _factor[1];
  else                          return _energy_correction_function->splint(visible_energy);
}

