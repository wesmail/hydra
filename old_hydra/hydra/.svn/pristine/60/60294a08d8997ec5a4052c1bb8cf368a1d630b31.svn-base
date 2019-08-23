#pragma implementation
#include "htofinocalparcell.h"
ClassImp(HTofinoCalParCell)

///////////////////////////////////////////////////////////////////////
// HTofinoCalParCell
//
// HTofinoCalParCell contains slope and offset parameters 
// for calibration of one pad 
//
//////////////////////////////////////////////////////////////////////

HTofinoCalParCell::HTofinoCalParCell() {
   reset();
}

HTofinoCalParCell::HTofinoCalParCell(Float_t fSlope, Float_t fOffset) {
   reset();
   setParams(fSlope, fOffset);
}

int HTofinoCalParCell::reset()
{
//reset of all data
   m_fTimeChannel = 0.03;
   m_fResolParam[0] = m_fResolParam[1] = m_fResolParam[2] = 0.0;
   m_fDistParam[0] = m_fDistParam[1] = m_fDistParam[2] = 0.0;
   m_fCoordPos = 690.;
   setParams(1.0, 0.0);
  
   return 1;
}

void HTofinoCalParCell::setParams(Float_t fSlope, Float_t fOffset) {
//set slope and offset
 setSlope(fSlope);
 setOffset(fOffset);
}
 
void HTofinoCalParCell::
  setDeltaParams(Float_t fDeltaSlope, Float_t fDeltaOffset) {
//set slope and offset
 setDeltaSlope(fDeltaSlope);
 setDeltaOffset(fDeltaOffset);
}
 
void HTofinoCalParCell::getResolParams(Float_t* resol) {
  int i;
  for(i = 0; i < 3; i++) resol[i] = m_fResolParam[i];
}

void HTofinoCalParCell::setResolParams(Float_t* resol) {
  int i;
  for(i = 0; i < 3; i++) m_fResolParam[i] = resol[i];
}

void HTofinoCalParCell::setResolParams(Float_t dpar1, 
                                 Float_t dpar2, Float_t dpar3) {
  m_fResolParam[0] = dpar1;
  m_fResolParam[1] = dpar2;
  m_fResolParam[2] = dpar3;
}
void HTofinoCalParCell::getDistanceParams(Float_t* dist) {
  int i;
  for(i = 0; i < 3; i++) dist[i] = m_fDistParam[i];
}

void HTofinoCalParCell::setDistanceParams(Float_t* dist) {
  int i;
  for(i = 0; i < 3; i++) m_fDistParam[i] = dist[i];
}

void HTofinoCalParCell::setDistanceParams(Float_t dpar1, 
                                 Float_t dpar2, Float_t dpar3) {
  m_fDistParam[0] = dpar1;
  m_fDistParam[1] = dpar2;
  m_fDistParam[2] = dpar3;
}

Float_t HTofinoCalParCell::calcDistance(Float_t row) {
  return m_fDistParam[0] + row*(m_fDistParam[1]+row*m_fDistParam[2]);
}

Float_t HTofinoCalParCell::calcDistPMT(Float_t y) {
  return (m_fCoordPos - y);
}

Float_t HTofinoCalParCell::calcTimeResol(Float_t y) {
  return m_fResolParam[0] + y*(m_fResolParam[1]+y*m_fResolParam[2]);
}






