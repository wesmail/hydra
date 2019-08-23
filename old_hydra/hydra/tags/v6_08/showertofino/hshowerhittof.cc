///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HShowerHitTof
// class joins data in hit level with Shower data
//                              
///////////////////////////////////////////////////////////////////////////////
#pragma implementation

#include "hshowerhittof.h"

ClassImp(HShowerHitTof)

HShowerHitTof::HShowerHitTof() {
  m_fTof = 0.0;
  m_fDriftTime = 0.0;
  m_fADC = 0;
  m_nTofinoCell = -1;
  m_nTofinoMult = 0; 
}

HShowerHitTof& HShowerHitTof::operator=(HShowerHitTof& ht) {
  HShowerHit::operator=(ht);
  m_fTof = ht.m_fTof;
  m_fDriftTime = ht.m_fDriftTime;
  m_fADC = ht.m_fADC;
  m_nTofinoCell = ht.m_nTofinoCell;
  m_nTofinoMult = ht.m_nTofinoMult;

  return *this;
}

HShowerHitTof& HShowerHitTof::operator=(HShowerHit& ht) {
  HShowerHit::operator=(ht);

  m_fTof = 0.0;
  m_fDriftTime = 0.0;
  m_fADC = 0.0;
  m_nTofinoCell = 9;
  m_nTofinoMult = 0;  
  
  return *this;
}

