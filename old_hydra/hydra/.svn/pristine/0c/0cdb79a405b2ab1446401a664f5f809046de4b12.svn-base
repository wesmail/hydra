#pragma implementation
using namespace std;
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <TClass.h>
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
   m_fTimeChannel = 0.06;
   m_fResolParam[0] = m_fResolParam[1] = m_fResolParam[2] = 0.0;
   m_fDistParam[0] = m_fDistParam[1] = m_fDistParam[2] = m_fDistParam[3] = 0.0;
   m_fCoordPos = 690.; m_fCFDThreshold = 0.; m_fMinLength = 0.;
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

void HTofinoCalParCell::setElossParams(Float_t* eloss) {
  int i;
  for(i = 0; i < 4; i++) m_fElossParam[i] = eloss[i];
}


void HTofinoCalParCell::setResolParams(Float_t dpar1, 
                                 Float_t dpar2, Float_t dpar3) {
  m_fResolParam[0] = dpar1;
  m_fResolParam[1] = dpar2;
  m_fResolParam[2] = dpar3;
}
void HTofinoCalParCell::getDistanceParams(Float_t* dist) {
  int i;
  for(i = 0; i < 4; i++) dist[i] = m_fDistParam[i];
}

void HTofinoCalParCell::setDistanceParams(Float_t* dist) {
  int i;
  for(i = 0; i < 4; i++) m_fDistParam[i] = dist[i];
}

void HTofinoCalParCell::getElossParams(Float_t* eloss) {
  int i;
  for(i = 0; i < 4; i++) eloss[i] = m_fElossParam[i];
}

void HTofinoCalParCell::setDistanceParams(Float_t dpar1, 
                           Float_t dpar2, Float_t dpar3, Float_t dpar4) {
  m_fDistParam[0] = dpar1;
  m_fDistParam[1] = dpar2;
  m_fDistParam[2] = dpar3;
  m_fDistParam[2] = dpar4;
}

Float_t HTofinoCalParCell::calcDistance(Float_t row) {
  //  return m_fDistParam[0] + row*(m_fDistParam[1]+row*m_fDistParam[2]);
  //Int_t iType = getDistanceParamsType();
  //cout <<"iType = " <<iType <<endl;
 if(getDistanceParamsType() == 2) {
   if(row < 7) return (m_fCoordPos - (m_fDistParam[0] + m_fDistParam[1]*row)); 
     else return(m_fCoordPos - (m_fDistParam[2] + m_fDistParam[3]*row)); 
 }
 else {
    return m_fDistParam[0] + row*(m_fDistParam[1]+row*m_fDistParam[2]);
 }
}

Float_t HTofinoCalParCell::calcDistPMT(Float_t y) {
  return (m_fCoordPos - y);
}

Float_t HTofinoCalParCell::calcTimeResol(Float_t y) {
  return m_fResolParam[0] + y*(m_fResolParam[1]+y*m_fResolParam[2]);
}

void HTofinoCalParCell::Streamer(TBuffer &R__b)
{
   // Stream an object of class HTofinoCalParCell.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
	  if(R__v < 1 ) {
	   Error("Streamer","Wrong parameter file for this class version");
	   exit(-1);
	  }
	   
      if(R__v <= 2) {
        TObject::Streamer(R__b);
        R__b >> m_fOffset;
        R__b >> m_fSlope;
        R__b >> m_fDeltaSlope;
        R__b >> m_fDeltaOffset;
        R__b.ReadStaticArray(m_fResolParam);
        R__b.ReadStaticArray(m_fDistParam);
        R__b >> m_nDistParamType;
        R__b >> m_fTimeChannel;
        R__b >> m_fCoordPos;
      }
      if(R__v == 3 ) {

      TObject::Streamer(R__b);
      R__b >> m_fOffset;
      R__b >> m_fSlope;
      R__b >> m_fDeltaSlope;
      R__b >> m_fDeltaOffset;
      R__b.ReadStaticArray(m_fResolParam);
      R__b.ReadStaticArray(m_fDistParam);
      R__b >> m_nDistParamType;
      R__b >> m_fTimeChannel;
      R__b >> m_fCoordPos;
      R__b >> m_fCFDThreshold;
      R__b >> m_fMinLength;
     } 
	  if(R__v == 4 ) {

      TObject::Streamer(R__b);
	  
      R__b >> m_fOffset;
      R__b >> m_fSlope;
      R__b >> m_fDeltaSlope;
      R__b >> m_fDeltaOffset;
      R__b.ReadStaticArray(m_fResolParam);
      R__b.ReadStaticArray(m_fDistParam);
      R__b >> m_nDistParamType;
      R__b >> m_fTimeChannel;
      R__b >> m_fCoordPos;
      R__b >> m_fCFDThreshold;
      R__b >> m_fADCThreshold;
      R__b >> m_fADCPedestal;
      R__b.ReadStaticArray(m_fElossParam);
      R__b >> m_fMinLength;
     } 

   } else {
      R__b.WriteVersion(HTofinoCalParCell::IsA());
      TObject::Streamer(R__b);
      R__b << m_fOffset;
      R__b << m_fSlope;
      R__b << m_fDeltaSlope;
      R__b << m_fDeltaOffset;
      R__b.WriteArray(m_fResolParam, 3);
      R__b.WriteArray(m_fDistParam, 4);
      R__b << m_nDistParamType;
      R__b << m_fTimeChannel;
      R__b << m_fCoordPos;
      R__b << m_fCFDThreshold;
      R__b << m_fADCThreshold;
      R__b << m_fADCPedestal;
      R__b.WriteArray(m_fElossParam, 4);
      R__b << m_fMinLength;
   }
}














