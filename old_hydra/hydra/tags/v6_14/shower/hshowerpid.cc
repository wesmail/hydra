//###################################################
//#
//#       
//#                      FRAMEWORK
//#
//#       Authors:                           W.Koenig
//#       adoption to framework & ROOT       W.Schoen
//#    
//#       last mod. W.Schoen 19.1.98
// ###################################################

#pragma implementation
#include "hshowerpid.h"
ClassImp(HShowerPID)

////////// **********ShowerPID defintion ****************/////////////77

Int_t HShowerPID::clear()
{
   m_fCharge=0.0F;
   m_fX = 0.0;
   m_fY = 0.0;

   m_fShower = 0.0; //shower - diff m_fSum2 - m_fSum1 if > 0

   m_nSector = -1;
   m_nModule = -1;
   m_nRow = -1;
   m_nCol = -1;
   m_nAddress = -1;
 
   return 1;
}

Int_t HShowerPID::calcAddress() {
  Char_t s = (m_nSector) ? m_nSector : 6;
  m_nAddress = 100000 * s;
  m_nAddress += 10000 * m_nModule;
  m_nAddress += 100 * m_nRow;
  m_nAddress += m_nCol;

  return m_nAddress;
}

//______________________________________________________________________________
void HShowerPID::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerPID.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); 
      if (R__v == 1) 
      { 
      Version_t R__v;
         R__v = R__b.ReadVersion(); // simulate HDataObject v 1 readout
      }
      TObject::Streamer(R__b);
      R__b >> m_fX;
      R__b >> m_fY;
      R__b >> m_fZ;
      R__b >> m_fRad;
      R__b >> m_fPhi;
      R__b >> m_fTheta;
      R__b >> m_fShower;
      R__b >> m_nSector;
      R__b >> m_nModule;
      R__b >> m_nRow;
      R__b >> m_nCol;
      R__b >> m_fCharge;
      R__b >> m_nAddress;
	  if(R__v <= 2)
	  {
	  Int_t iEventNr;
		  R__b >> iEventNr;
	  }
   } else {
      R__b.WriteVersion(HShowerPID::IsA());
      TObject::Streamer(R__b);
      R__b << m_fX;
      R__b << m_fY;
      R__b << m_fZ;
      R__b << m_fRad;
      R__b << m_fPhi;
      R__b << m_fTheta;
      R__b << m_fShower;
      R__b << m_nSector;
      R__b << m_nModule;
      R__b << m_nRow;
      R__b << m_nCol;
      R__b << m_fCharge;
      R__b << m_nAddress;
   }
}
