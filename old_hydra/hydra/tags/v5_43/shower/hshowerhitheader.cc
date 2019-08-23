// File: HShowerHitHeader.cc
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/01/20 21:25:08
//

#include "hshowerhitheader.h" 

ClassImp(HShowerHitHeader)


HShowerHitHeader::HShowerHitHeader() {
  setFiredCells(0);
  setLocalMax(0);
  setClusters(0);

  setModule(-1);
  setSector(-1);
}

HShowerHitHeader::~HShowerHitHeader() {
}

Int_t HShowerHitHeader::incFiredCells(Int_t nStep) {
  m_nFiredCells += nStep;
  return m_nFiredCells;
}

Int_t HShowerHitHeader::incLocalMax(Int_t nStep) {
  m_nLocalMax += nStep;
  return m_nLocalMax;
}

Int_t HShowerHitHeader::incClusters(Int_t nStep) {
  m_nClusters += nStep;
  return m_nClusters;
}

// -----------------------------------------------------------------------------
void HShowerHitHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerHitHeader.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); 
	  if(R__v == 1) 
	  {
      Version_t R__v;
         R__v = R__b.ReadVersion(); // simulate HDataObject v 1 readout
	  }
      TObject::Streamer(R__b);
      R__b >> m_nFiredCells;
      R__b >> m_nLocalMax;
      R__b >> m_nClusters;
      R__b >> m_nSector;
      R__b >> m_nModule;
   } else {
      R__b.WriteVersion(HShowerHitHeader::IsA());
      TObject::Streamer(R__b);
      R__b << m_nFiredCells;
      R__b << m_nLocalMax;
      R__b << m_nClusters;
      R__b << m_nSector;
      R__b << m_nModule;
   }
}

