//###################################################

#pragma implementation
#include "TString.h"
#include "fstream.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hrichgeometrypar.h"
#include "hrichdetector.h"
#include "hparhadasciifileio.h"

ClassImp(HRichGeometryPar)


//----------------------------------------------------------------------------
HRichGeometryPar::HRichGeometryPar(const char* name,const char* title,
                                   const char* context)
                : HRichParSet(name,title,context) {
  fSectorsNr = 0;
  for (Int_t i = 0; i < 6; i++) fSectorActive[i] = 0;
  fColumns = 0;
  fRows = 0;
  fLab = 0;
  fDistanceWiresPads = 0.;
  fSectorShift = 0.;
}
//============================================================================

//----------------------------------------------------------------------------
HRichGeometryPar::~HRichGeometryPar()
{
  clear(); 
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichGeometryPar::getSectorsNr() {
 return fSectorsNr;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichGeometryPar::getSector(Int_t s) {
 return fSectorActive[s];
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichGeometryPar::getColumns() {
 return fColumns;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichGeometryPar::getRows() {
 return fRows;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichGeometryPar::getPadsNr() {
 return (fColumns*fRows);
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichGeometryPar::getPadStatus(Int_t padnr) {
 return fPads.getPad(padnr)->getPadActive();
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichGeometryPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 using namespace HadAsciiAuxiliaryFunctions;
 if (!pHadAsciiFile) return kFALSE;

 HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
 
 Bool_t status = kTRUE;
 Int_t i;

 try {

  HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

  mydata.SetActiveSection("RICH detector layout");

  for (i = 0; i < 6; i++) {
   fSectorActive[i] = pRichDet->getModule(i,0);
   if (fSectorActive[i] > 0) fSectorsNr++;
  }
  if (fSectorsNr > 0) {
   fColumns = pRichDet->getColumns();
   fRows = pRichDet->getRows();
  }
  fDistanceWiresPads = mydata.ReadFloat("Distance between wire and pad planes");
  fSectorShift = mydata.ReadFloat("Sector shift");


  // sector parameters
  for (i = 0; i < 6; i++)
   if (fSectorActive[i] > 0) {
    fSectors[i].setSectorId(i);
    switch (i) {
     case 0: fSectors[i].setPhi(90.);
             break;
     case 1: fSectors[i].setPhi(150.);
             break;
     case 2: fSectors[i].setPhi(210.);
             break;
     case 3: fSectors[i].setPhi(270.);
             break;
     case 4: fSectors[i].setPhi(330.);
             break;
     case 5: fSectors[i].setPhi(30.);
             break;
    }
   }

  if (fSectorsNr > 0) {
   // pads parameters
   fPads.initAscii(pHadAsciiFile);
   // frame parameters
   fFrame.initAscii(pHadAsciiFile);
   // wires parameters
   fWires.initAscii(pHadAsciiFile);
  }

  initParameters();

 } // eof try block

 catch (Bool_t ret) {
  status = ret;
 }
 catch (...) {
  status = kFALSE;
  ErrorMsg(2,"HRichGeometryPar::initAscii",1,"Unidentified exception catched.");
 }

 if (status)
  ErrorMsg(0,"HRichGeometryPar::initAscii",
   1,"Container \'RichGeometryParameters\' has been read from ASCII file.");

 return status;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichGeometryPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 using namespace HadAsciiAuxiliaryFunctions;
 if (!pHadAsciiFile) return kFALSE;

 Bool_t status = kTRUE;

 try {

  HAsciiKey &mydata = *pHadAsciiFile->GetKeyAscii();

  mydata.WriteSection("RICH detector layout");

  mydata.WriteFloat("Distance between wire and pad planes", fDistanceWiresPads);
  mydata.WriteFloat("Sector shift", fSectorShift);

  for (Int_t i = 0; i < 1; i++)
   //if (fSectorActive[i] > 0) {
   // fSectors[i].writeAscii(pHadAsciiFile);
   //}         // this is obsolete

  if (fSectorsNr > 0) {

   // write frame parameters
   fFrame.writeAscii(pHadAsciiFile);
   // write wires parameters
   fWires.writeAscii(pHadAsciiFile);
   // writing pads parameters
   fPads.writeAscii(pHadAsciiFile);
  }

 } // eof try block

 catch (Bool_t ret) {
  status = ret;
 }
 catch (...) {
  status = kFALSE;
  ErrorMsg(2,"HRichGeometryPar::writeAscii",1,"Unidentified exception catched.");
 }

 if (status)
  ErrorMsg(0,"HRichGeometryPar::writeAscii",
   1,"Container \'RichGeometryParameters\' has been written to ASCII file.");

 return status;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichGeometryPar::initParameters() {

  // here we calculate x, y, z in LAB and phi and store it for each pad

  Int_t i, j;
  Float_t x, y, z, xl, yl, zl;
  HLocation loc;
  Float_t len, phi;

  for(i = 0; i < 6; i++)
   if (getSector(i) > 0) {

    for (j = 0; j < getPadsNr(); j++) {

     x = fPads.getPad(j)->getY(); // local y is X in lab before rotation and shift
     y = fPads.getPad(j)->getX(); // local x is Y in lab before rotation and shift
     z = 0.;



     xl = cos(3.141592654*20./180.)*x;
     yl = y;
     zl = sin(3.141592654*20./180.)*x; 

     //   xl += fSectorShift;

     x = cos(3.141592654*fSectors[i].getPhi()/180.)*xl -
         sin(3.141592654*fSectors[i].getPhi()/180.)*yl;
     y = sin(3.141592654*fSectors[i].getPhi()/180.)*xl +
         cos(3.141592654*fSectors[i].getPhi()/180.)*yl; 
     z = zl;

     fPads.getPad(j)->setXYZlab(i, x, y, z);

     len = sqrt(xl*xl + yl*yl);
     phi = 0.;
     if (len > 0) phi = 57.2957795 * acos(xl / len);
     if (yl < 0) phi = -1.*phi;

     phi += fSectors[i].getPhi();

     Float_t sphi = fPads.getPad(j)->getPhi(i);
     if (sphi>0. && sphi-phi>1.e-2)
     {
	 Error("HRichGeometryPar::initParameters","inconsistency between phi from param source and calculated phi (from pad plane coord in mm)");
	 cout<<" ora phi: "<<sphi<<"  calc phi: "<<phi<<endl;
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
void HRichGeometryPar::Streamer(TBuffer &R__b)
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
      int R__i;
      for (R__i = 0; R__i < 6; R__i++)
         fSectors[R__i].Streamer(R__b);
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
      int R__i;
      for (R__i = 0; R__i < 6; R__i++)
         fSectors[R__i].Streamer(R__b);
      fFrame.Streamer(R__b);
      fWires.Streamer(R__b);                             
      fPads.Streamer(R__b);
   }
}      
//============================================================================

//----------------------------------------------------------------------------
void HRichGeometryPar::clear() { }
//============================================================================

