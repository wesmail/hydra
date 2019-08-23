#pragma implementation
#include "hshowertrack.h"
ClassImp(HShowerTrack)
//*-- Author : Leszek Kidon
//*-- Modified : 22/02/2000 L.Fabbietti

///////////////////////////////////////////////////////
//______________________________________________________
//  HShowerTrack
//  This class stores the track numbers of the particle
//  that hit the PreShower. It is sortable by the
//  fired pad address.
//
//////////////////////////////////////////////////////


int HShowerTrack::clear()
{
   m_nAddress = -1;
   m_nTrack = -1;
 
   return 1;
}

//______________________________________________________________________________
void HShowerTrack::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerTrack.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); 
      if (R__v) 
      {
      Version_t R__v;
         R__v = R__b.ReadVersion(); // simulate HDataObject v 1 readout 
      }
      TObject::Streamer(R__b);
      R__b >> m_nAddress;
      R__b >> m_nTrack;
	  if(R__v <= 2)
	  {
	  Int_t m_lEventNr;
		  R__b >> m_lEventNr;
	  }
   } else {
      R__b.WriteVersion(HShowerTrack::IsA());
      TObject::Streamer(R__b);
      R__b << m_nAddress;
      R__b << m_nTrack;
   }
}

