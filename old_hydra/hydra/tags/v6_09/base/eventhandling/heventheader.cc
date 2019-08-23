#include "heventheader.h"
//*-- Author : Manuel Sanchez 
//*-- Modified : 30/03/2000 by R. Holzmann
//*-- Modified : 24/03/98 by Manuel Sanchez
//*-- Copyright : GENP (Univ. Santiago de Compostela)

///////////////////////////////////////////////////////////////////////////
//HEventHeader
// 
// This class contains the header information of an event (run number, etc.)
// See Data Members for more detailed information.
///////////////////////////////////////////////////////////////////////////

void HEventHeader::Streamer(TBuffer &R__b) {
   // Stream an object of class HEventHeader.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fEventSize;
      R__b >> fEventDecoding;
      R__b >> fId;
      R__b >> fEventSeqNumber;
      R__b >> fDate;
      R__b >> fTime;
      R__b >> fEventRunNumber;
      R__b >> fEventPad;

      if (R__v > 1) {
	R__b >> fTBit;
      } else {
	fTBit = 0;
      }

      if (R__v > 2) {
	fVertex.Streamer(R__b);
      } else {
	// default vertex value
      }

   } else {
      R__b.WriteVersion(HEventHeader::IsA());
      TObject::Streamer(R__b);
      R__b << fEventSize;
      R__b << fEventDecoding;
      R__b << fId;
      R__b << fEventSeqNumber;
      R__b << fDate;
      R__b << fTime;
      R__b << fEventRunNumber;
      R__b << fEventPad;
      R__b << fTBit;
      fVertex.Streamer(R__b);
   }
}

ClassImp(HEventHeader)
ClassImp(HVertex)
