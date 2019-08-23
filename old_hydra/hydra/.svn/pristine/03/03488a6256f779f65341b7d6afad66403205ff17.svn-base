///*-- AUTHOR : Ilse Koenig
//*-- Created : 09/09/2004 by Ilse Koenig

//////////////////////////////////////////////////////////////////////////////
//
// HOraSlowChanRaw
//
// Class for raw data for a slowcontrol channel
//
//////////////////////////////////////////////////////////////////////////////

#include "horaslowchanraw.h"
#include "TDatime.h"

ClassImp(HOraSlowChanRaw)

HOraSlowChanRaw::HOraSlowChanRaw() {
  // Default constructor
  nanoSeconds=0;
  value=0.;
}

void HOraSlowChanRaw::fill(const char* t,Int_t nano,Double_t val) {
  // Fills the timestamp, nanoseconds and value
  timestamp=t;
  nanoSeconds=nano;
  value=val;
}

void HOraSlowChanRaw::fill(const char* t,Int_t nano,Int_t val) {
  // Fills the timestamp, nanoseconds and value
  timestamp=t;
  nanoSeconds=nano;
  value=(Double_t)val;
}

Int_t HOraSlowChanRaw::getTimeDiff(const char* t) {
  // Returns the time difference (in seconds) relative to time t
  TDatime t1(t);
  TDatime t2(timestamp);
  return (t2.Get()-t1.Get());  
}

void HOraSlowChanRaw::print(Int_t valPrec) {
  // Prints the data to stdout
  cout.setf(ios::fixed,ios::floatfield);
  cout<<timestamp<<setw(11)<<nanoSeconds
      <<cout.precision(valPrec)<<setw(valPrec+10)<<value<<'\n';
}

void HOraSlowChanRaw::write(fstream& fout,Int_t valPrec) {
  // Streams the data
  fout.setf(ios::fixed,ios::floatfield);
  fout<<timestamp<<setw(11)<<nanoSeconds
      <<fout.precision(valPrec)<<setw(valPrec+10)<<value<<'\n';
}

