#ifndef HRTTYPES_H
#define HRTTYPES_H

const Int_t kRtDetectionPlanes = 4;
const Int_t kRtParameterDim = 5;

typedef Float_t HRtMeasurement[2*kRtDetectionPlanes];
typedef Float_t HRtTrackParam[kRtParameterDim];

#endif
