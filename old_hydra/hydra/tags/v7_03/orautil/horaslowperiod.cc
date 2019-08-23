///*-- AUTHOR : Ilse Koenig
//*-- Created : 13/08/2004 by Ilse Koenig

//////////////////////////////////////////////////////////////////////////////
//
// HOraSlowPeriod
//
// Class for slowcontrol run period
//
//////////////////////////////////////////////////////////////////////////////

#include "horaslowperiod.h"

ClassImp(HOraSlowPeriod)

HOraSlowPeriod::HOraSlowPeriod() {
  // Constructor
  periodId=-1;
  runId=-1;
  filterFlag=1;
}

void HOraSlowPeriod::print() {
  // Prints the data to stdout
  cout<<startTime<<"  "<<endTime;
  if (runId>0) {
    cout<<setw(12)<<runId<<" "<<filename;
  }
  cout<<'\n';
}

void HOraSlowPeriod::write(fstream& fout) {
  // Streams the data
  fout<<startTime<<"  "<<endTime;
  if (runId>0) {
    fout<<setw(12)<<runId<<"  "<<filename;
  }
  fout<<'\n';
}
