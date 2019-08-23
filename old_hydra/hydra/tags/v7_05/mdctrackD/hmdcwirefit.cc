//*-- AUTHOR : Vladimir Pechenov

////////////////////////////////////////////////////////////////
// HMdcWireFit
//
//  Container class keep fit information for debuging.
//
////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdcwirefit.h"
#include <iostream> 
#include <iomanip>

ClassImp(HMdcWireFit)

void HMdcWireFit::print() {
  printf("S%i M%i L%i C%3i T%i:",sec+1,mod+1,layer+1,cell+1,timeNum);
  printf(" tof=%6.2f %5.2fmm ",fullTime-driftTime,minDist);
  printf("%s drTm=%6.2f fTm=%6.2f",(isInCell) ? "   ":"OUT",driftTime,fullTime);
  printf(" TDC=%6.2f dev=%7.2f WT=%g\n",tdcTime,dev,weight);
}

