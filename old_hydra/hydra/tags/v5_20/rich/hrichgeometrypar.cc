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
HRichGeometryPar::HRichGeometryPar() {
  SetName("RichGeometryParameters");

  fSectorsNr = 0;
  for (Int_t i = 0; i < 6; i++) fSectorActive[i] = 0;
  fColumns = 0;
  fRows = 0;
  fLab = 0;
  fDistanceWiresPads = 0.;
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

  // sector parameters
  for (i = 0; i < 6; i++)
   if (fSectorActive[i] > 0) {
    fSectors[i].setSectorId(i);
    fSectors[i].initAscii(pHadAsciiFile);
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

  for (Int_t i = 0; i < 6; i++)
   if (fSectorActive[i] > 0) {
    fSectors[i].writeAscii(pHadAsciiFile);
   }

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
  HLocation loc;
  HGeomVector vLocalCoord;
  HGeomVector vLabCoord;
  Float_t len, phi;

  for(i = 0; i < 6; i++)
   if (getSector(i) > 0) {

    for (j = 0; j < getPadsNr(); j++) {
     loc.set(3, i, fPads.getPad(j)->getPadY(), fPads.getPad(j)->getPadX());

     // getLocalCoord(loc, vLocalCoord);
     getLabCoord(loc, vLabCoord);

     fPads.getPad(j)->setXYZlab(i, vLabCoord.getX(), vLabCoord.getY(), vLabCoord.getZ());

     len = sqrt(vLabCoord.getX()*vLabCoord.getX() + vLabCoord.getY()*vLabCoord.getY());
     phi = 0.;
     if (len > 0) phi = 57.2958 * acos(vLabCoord.getX() / len);
     if (vLabCoord.getY() < 0) phi = 360. - phi;

     fPads.getPad(j)->setPhi(i, phi);

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

//----------------------------------------------------------------------------
const HGeomTransform&  HRichGeometryPar::getTransform(Int_t sec) {
  return fSectors[sec].getTransform();
}
//============================================================================

//----------------------------------------------------------------------------
void HRichGeometryPar::getLocalCoord(HLocation& loc, HGeomVector& v) {
  float fX, fY;
  float scale = 10.0; //transform cm to mm

  HRichPadTab* pPads = getPadsPar();
  pPads->getPad(loc[2], loc[1])->getXY(&fX, &fY); // 2-col 1-row 

  v.setX(scale*fX);
  v.setY(scale*fY);
  v.setZ(0.0);
}
//============================================================================

//----------------------------------------------------------------------------
void HRichGeometryPar::getLabCoord(HLocation& loc, HGeomVector& v) {
  HGeomVector vLoc;
  HGeomTransform tt;

  getLocalCoord(loc, vLoc);
  tt = getTransform(loc[0]);

  v = tt.transFrom(vLoc);
}
//============================================================================

//----------------------------------------------------------------------------
//void HRichGeometryPar::translModules() {
//  fSectors[0].translateModules();  //Only in sector 0
//                                  //Sector 0 is reference to others
//}
//============================================================================

//----------------------------------------------------------------------------
void HRichGeometryPar::rotateSectors() {
  int i;

  HGeomTransform modt[3];
  HGeomTransform st;

  st = fSectors[0].getTransform();
  // for (j = 0; j < fSectors[0].getModules(); j++)
  //  modt[j] = fSectors[0].getModule(j)->getTransform();

  // only sectors 1, 2, 3, 4, 5 are rotated
  for(i = 1; i < fSectorsNr; i++) {
    fSectors[i].setTransform(st);
    // for(j = 0; j < fSectors[i].getModules(); j++)
    //    fSectors[i].getModule(j)->setTransform(modt[j]);

    fSectors[i].rotateSector();
  }
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichGeometryPar::transToLab() {
  if (IsLab()) {
     printf("Geometry has been already transformed to labolatory coordinates.\n");
     return kFALSE;
  }

  printf("Geometry is being transformed to labolatory coordinates now.\n");

  // translModules();
  rotateSectors();
  setLab();
  setChanged();

  return kTRUE;
}
//============================================================================



