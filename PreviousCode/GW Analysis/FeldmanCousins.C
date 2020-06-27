
void FeldmanCousins()
{
 // Example macro of using the TFeldmanCousins class in root.
 //
 // get a FeldmanCousins calculation object with the default limits
 // of calculating a 90% CL with the minimum signal value scanned
 // = 0.0 and the maximum signal value scanned of 50.0
 //Author : Adrian John Bevan <bevan@SLAC.Stanford.EDU>
	
 // Modified by Emmett Krupczak - Mar 9 2016. 

 if (!gROOT->GetClass("TFeldmanCousins")) gSystem->Load("libPhysics");

 TFeldmanCousins f;

  // calculate either the upper or lower limit for 10 observerd
  // events with an estimated background of 3.  The calculation of
  // either upper or lower limit will return that limit and fill
  // data members with both the upper and lower limit for you.
  Double_t Nobserved   = 0; 
//Double_t Nbackground = 0.00151208171832; //1.51208171832e-06 events/sec * 1000 sec window
//Double_t Nbackground2 = 0.201369839271; //without likelihood and miniballon cuts
  Double_t Nbackground2 = 0.180756665177;
//Double_t Nbackground2 = 0.000201579865258*1000*0.8967*0.931; //with recalculated post xenon runs and corrections for efficiency and livetime/runtime
//Double_t Nbackground2 = 0.241*0.89*0.931; //without likelihood and miniballon cuts
//  Double_t Nbackground2 =0.240717008688; //without likelihood and miniballon cuts and excluding runs after Nov 4 (Xe extraction)
//Double_t ul = f.CalculateUpperLimit(Nobserved, Nbackground);
  Double_t ul2 = f.CalculateUpperLimit(Nobserved, Nbackground2);
  Double_t ll = f.GetLowerLimit();

  cout << "For " <<  Nobserved << " data observed with an estimated background of "<<Nbackground2 <<endl;
//cout << "of " << Nbackground << " (with cuts) and " << Nbackground2 << " (without cuts), the Feldman-Cousins method gives" <<endl;
//cout << "\tUpper Limit with cuts = " <<  ul << endl;
  cout << "\tUpper Limit without cuts = " <<  ul2 << endl;
  cout << "\tLower Limit = " <<  ll << endl;
  cout << "at the 90% CL"<< endl;
}

