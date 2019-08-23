///*-- AUTHOR : Ilse Koenig
//*-- Created : 13/08/2004 by Ilse Koenig

//////////////////////////////////////////////////////////////////////////////
//
// HOraSlowChanRunSum
//
// Class for run based summary data for a slowcontrol channel
//
//////////////////////////////////////////////////////////////////////////////

#include "horaslowchanrunsum.h"
#include "horaslowperiod.h"

ClassImp(HOraSlowChanRunSum)

HOraSlowChanRunSum::HOraSlowChanRunSum() {
  // Default constructor
  periodIndex=-1;
  nData=0;
  status=0;
  mean=sigma=minVal=maxVal=0.;
}

void HOraSlowChanRunSum::fill(Int_t pInd,Double_t mea,Double_t sig,
                 Double_t min,Double_t max,Int_t nD,Int_t st) {
  // Fills the data
  periodIndex=pInd;
  mean=mea;
  sigma=sig;
  minVal=min;
  maxVal=max;
  nData=nD;
  status=st;
}

void HOraSlowChanRunSum::print(Int_t valPrec,HOraSlowPeriod* period) {
  // Prints the data to stdout
  // Takes as arguments the precision of the data and the pointer to the period
  Int_t n=valPrec+10;
  cout.setf(ios::fixed,ios::floatfield);
  cout.setf(ios::showpoint);
  cout<<setprecision(valPrec)<<setw(n)<<mean
      <<setw(n)<<sigma<<setw(n)<<minVal<<setw(n)<<maxVal<<setw(8)<<nData
      <<setprecision(6);
  if (period!=0&&period->getRunId()>0) {
    cout<<setw(12)<<period->getRunId()<<" "<<period->getFilename();
  }
  cout<<'\n';
}

void HOraSlowChanRunSum::write(fstream& fout,Int_t valPrec,HOraSlowPeriod* period) {
  // Streams the data
  // Takes as arguments the precision of the data and the pointer to the period
  Int_t n=valPrec+10;
  fout.setf(ios::fixed,ios::floatfield);
  fout.setf(ios::showpoint);
  fout<<setprecision(valPrec)<<setw(n)<<mean
      <<setw(n)<<sigma<<setw(n)<<minVal<<setw(n)<<maxVal<<setw(8)<<nData
      <<setprecision(6);
  if (period!=0&&period->getRunId()>0) {
    fout<<setw(12)<<period->getRunId()<<"  "<<period->getFilename();
  }
  fout<<'\n';
}



