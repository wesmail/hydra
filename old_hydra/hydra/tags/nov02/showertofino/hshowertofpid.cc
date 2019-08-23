//##################################################################################
//#  This class keeps the hits which fullfill shower condition and they are in 
//#  tofino time of flight window. This container is filled by HShowerTofPIDFinder     
//##################################################################################
#pragma implementation
#include "hshowertofpid.h"
ClassImp(HShowerTofPID)

////////// **********ShowerTofPID defintion ****************/////////////77

HShowerTofPID::HShowerTofPID() {
clear();
}

HShowerTofPID::~HShowerTofPID() {
}

Int_t HShowerTofPID::clear(void)
{
   m_fRad=0.0;
   m_fPhi = 0.0;
   m_fTheta = 0.0;

   m_fFact10 = 0.0; 
   m_fFact20 = 0.0; 
   
   m_fTof = 0.0; 
   m_nTofinoCell = -1; 
   m_nTofinoMult = -1; 

   m_nSector = -1;
   m_nModule = -1;
   m_nRow = -1;
   m_nCol = -1;
   m_nAddress = -1;
   m_nOuterHit = -1;
   m_fTrackLength = 0;
 
   return 1;
}
//______________________________________________________________________________
void HShowerTofPID::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerTofPID.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); 
      TObject::Streamer(R__b);
      R__b >> m_fRad;
      R__b >> m_fPhi;
      R__b >> m_fTheta;
      R__b >> m_fFact10;
      R__b >> m_fFact20;
      R__b >> m_fTof;
      R__b >> m_nTofinoMult;
      R__b >> m_nTofinoCell;
      R__b >> m_nOuterHit;
      R__b >> m_nSector;
      R__b >> m_nModule;
      R__b >> m_nRow;
      R__b >> m_nCol;
      R__b >> m_nAddress;

      if (R__v == 2) { 
      R__b >> m_fTrackLength;
      }

   } else {
      R__b.WriteVersion(HShowerTofPID::IsA());
      TObject::Streamer(R__b);
      R__b << m_fRad;
      R__b << m_fPhi;
      R__b << m_fTheta;
      R__b << m_fFact10;
      R__b << m_fFact20;
      R__b << m_fTof;
      R__b << m_nTofinoMult;
      R__b << m_nTofinoCell;
      R__b << m_nOuterHit;
      R__b << m_nSector;
      R__b << m_nModule;
      R__b << m_nRow;
      R__b << m_nCol;
      R__b << m_nAddress;
      R__b << m_fTrackLength;
   }
}
