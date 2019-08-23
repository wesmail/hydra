#include "hrtmeasurement.h"
#include <iostream.h>

void HRtMeasurement::print(void) {
  for (int i=0;i<kRtMeasurementDim;i++) cout << fData[i] << ":";
  cout << endl;
}

ClassImp(HRtMeasurement)
