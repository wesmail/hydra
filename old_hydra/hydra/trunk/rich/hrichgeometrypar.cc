//*-- Author : RICH team member
// ***************************************************************************
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
//  HRichGeometryPar
//  
//  
//  
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

#pragma implementation

#include "TMath.h"
#include "TString.h"

#include "hades.h"
#include "hparhadasciifileio.h"
#include "hrichdetector.h"
#include "hrichgeometrypar.h"
#include "hspectrometer.h"

#include <cmath>
#include <fstream>

using namespace std;

ClassImp(HRichGeometryPar)


//----------------------------------------------------------------------------
HRichGeometryPar::HRichGeometryPar(const Char_t* name,
                                   const Char_t* title,
                                   const Char_t* context)
   : HRichParSet(name, title, context)
{
   fSectorsNr         = 0;
   fColumns           = 0;
   fRows              = 0;
   fLab               = 0;
   fDistanceWiresPads = 0.;
   fSectorShift       = 0.;

   for (Int_t i = 0; i < 6; i++)
   {
      fSectorPhi[i]    = 0.;
      fSectorActive[i] = 0;
   }

}
//============================================================================

//----------------------------------------------------------------------------
HRichGeometryPar::~HRichGeometryPar()
{
   clear(); 
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t 
HRichGeometryPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{

   using namespace HadAsciiAuxiliaryFunctions;

   if ( NULL == pHadAsciiFile)
   {
      ErrorMsg(2, "HRichGeometryPar::initAscii",
               1, "Pointer to HParHadAsciiFileIo is NULL.");
      return kFALSE;
   }

   HRichDetector *pRichDet = static_cast<HRichDetector*>(gHades->getSetup()->getDetector("Rich"));
   Bool_t status = kTRUE;
   Int_t i;

   try
   {
      HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();
      mydata.SetActiveSection("RICH detector layout");
      for (i = 0; i < 6; i++)
      {
         fSectorActive[i] = pRichDet->getModule(i, 0);
         if (fSectorActive[i] > 0)
            fSectorsNr++;
      }
      if (fSectorsNr > 0)
      {
         fColumns = pRichDet->getColumns();
         fRows = pRichDet->getRows();
      }
      fDistanceWiresPads = mydata.ReadFloat("Distance between wire and pad planes");
      fSectorShift = mydata.ReadFloat("Sector shift");


      // sector parameters
      for (i = 0; i < 6; i++)
         if (fSectorActive[i] > 0) {
            switch (i) {
               case 0: fSectorPhi[i] = 90.;
                  break;
               case 1: fSectorPhi[i] = 150.;
                  break;
               case 2: fSectorPhi[i] = 210.;
                  break;
               case 3: fSectorPhi[i] = 270.;
                  break;
               case 4: fSectorPhi[i] = 330.;
                  break;
               case 5: fSectorPhi[i] = 30.;
                  break;
            }
         }

      if (fSectorsNr > 0)
      {
         // pads parameters
         fPads.initAscii(pHadAsciiFile);
         // frame parameters
         fFrame.initAscii(pHadAsciiFile);
         // wires parameters
         fWires.initAscii(pHadAsciiFile);
      }

      initParameters();

   } // eof try block

   catch (Bool_t ret)
   {
      status = ret;
   }

   catch (...)
   {
      status = kFALSE;
      ErrorMsg(2, "HRichGeometryPar::initAscii",
               1, "Unidentified exception catched.");
   }

   if (status)
      ErrorMsg(0, "HRichGeometryPar::initAscii",
               1, "Container \'RichGeometryParameters\' has been read from ASCII file.");

   return status;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichGeometryPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{

   using namespace HadAsciiAuxiliaryFunctions;

   if ( NULL == pHadAsciiFile)
   {
      ErrorMsg(2, "HRichGeometryPar::writeAscii",
               1, "Pointer to HParHadAsciiFileIo is NULL.");
      return kFALSE;
   }

   Bool_t status = kTRUE;

   try
   {
      HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();
      mydata.WriteSection("RICH detector layout");
      mydata.WriteFloat("Distance between wire and pad planes", fDistanceWiresPads);
      mydata.WriteFloat("Sector shift", fSectorShift);

      for (Int_t i = 0; i < 1; i++)
         if (fSectorsNr > 0)
         {
            // write frame parameters
            fFrame.writeAscii(pHadAsciiFile);
            // write wires parameters
            fWires.writeAscii(pHadAsciiFile);
            // writing pads parameters
            fPads.writeAscii(pHadAsciiFile);
         }
   } // eof try block

   catch (Bool_t ret)
   {
      status = ret;
   }
   catch (...)
   {
      status = kFALSE;
      ErrorMsg(2, "HRichGeometryPar::writeAscii",
               1, "Unidentified exception catched.");
   }

   if (status)
      ErrorMsg(0, "HRichGeometryPar::writeAscii",
               1, "Container \'RichGeometryParameters\' has been written to ASCII file.");

   return status;
}
//============================================================================

//----------------------------------------------------------------------------
void
HRichGeometryPar::initParameters()
{
// here we calculate x, y, z in LAB and phi and store it for each pad

   Int_t i;
   Int_t j;
   Float_t x;
   Float_t y;
   Float_t z;
   Float_t xl;
   Float_t yl;
   Float_t zl;
   Float_t len;
   Float_t phi;
   Float_t sphi;
   HLocation loc;

   for(i = 0; i < 6; i++)
      if (getSectorActive(i) > 0)
      {
         for (j = 0; j < getPadsNr(); j++)
         {
            x = fPads.getPad(j)->getY(); // local y is X in lab before rotation and shift
            y = fPads.getPad(j)->getX(); // local x is Y in lab before rotation and shift
            z = 0.;

            xl = cos(20. * TMath::DegToRad()) * x;
            yl = y;
            zl = sin(20. * TMath::DegToRad()) * x; 

            //   xl += fSectorShift;

            x = cos(fSectorPhi[i] * TMath::DegToRad()) * xl -
            sin(fSectorPhi[i] * TMath::DegToRad()) * yl;
            y = sin(fSectorPhi[i] * TMath::DegToRad()) * xl +
            cos(fSectorPhi[i] * TMath::DegToRad()) * yl;
            z = zl;

            fPads.getPad(j)->setXYZlab(i, x, y, z);

            len = sqrt(xl * xl + yl * yl);
            phi = 0.;
            if (len > 0)
               phi = TMath::RadToDeg() * acos(xl / len);
            if (yl < 0)
               phi = -1.*phi;

            phi += fSectorPhi[i];

            sphi = fPads.getPad(j)->getPhi(i);
            if (sphi > 0. && sphi - phi > 1.e-2)
            {
               Error("initParameters", "Inconsistency between phi from param source and calculated phi (from pad plane coord in mm)");
               cerr <<" ora phi: "  << sphi 
                    <<" calc phi: " << phi <<endl;
            }

            fPads.getPad(j)->setPhi(i, phi);

            //if (i==5) cout << fPads.getPad(j)->getPadX() << " " << fPads.getPad(j)->getPadY() 
            //              << "    " << phi << "   ...   " << fPads.getPad(j)->getX() 
            //             << " " << fPads.getPad(j)->getY() << endl;

         }
      }                         
}
//============================================================================

//----------------------------------------------------------------------------
void
HRichGeometryPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class HRichGeometryPar.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HRichParSet::Streamer(R__b);
      R__b >> fSectorsNr;
      R__b.ReadStaticArray(fSectorActive);
      R__b >> fColumns;
      R__b >> fRows;
      R__b >> fLab;
      R__b >> fDistanceWiresPads;
      R__b >> fSectorShift;
      Int_t R__i;
      for (R__i = 0; R__i < 6; R__i++)
         R__b >> fSectorPhi[R__i];
      fFrame.Streamer(R__b);
      fWires.Streamer(R__b);
      fPads.Streamer(R__b);

      initParameters();

   } else {
      R__b.WriteVersion(HRichGeometryPar::IsA());
      HRichParSet::Streamer(R__b);
      R__b << fSectorsNr;
      R__b.WriteArray(fSectorActive, 6);
      R__b << fColumns;
      R__b << fRows;
      R__b << fLab;
      R__b << fDistanceWiresPads;
      R__b << fSectorShift;
      Int_t R__i;
      for (R__i = 0; R__i < 6; R__i++)
         R__b << fSectorPhi[R__i];
      fFrame.Streamer(R__b);
      fWires.Streamer(R__b);                             
      fPads.Streamer(R__b);
   }
}      
//============================================================================
