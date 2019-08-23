// File: hrichpad.h
// ****************************************************************************
//
// HRichPad
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//
// ****************************************************************************

#ifndef RICHPAD_H
#define RICHPAD_H

using namespace std;
#include <TObject.h>
#include <TObjArray.h>
#include "hrichparset.h"
#include <iostream> 
#include <iomanip>

// ****************************************************************************

class HRichPadCorner : public TObject {

// ****************************************************************************

public:

  HRichPadCorner();
  HRichPadCorner(Int_t nrcor, Float_t xcor, Float_t ycor, Int_t flag);
  virtual ~HRichPadCorner();
  HRichPadCorner(const HRichPadCorner& source);
  HRichPadCorner& operator=(const HRichPadCorner& source);

  Float_t getX() { return fXcor; }
  Float_t getY() { return fYcor; }
  void getXY(Float_t *fX, Float_t *fY) { *fX = fXcor; *fY = fYcor; }
  Int_t getCornerNr() { return fCornerNr; }
  Int_t getAreaFlag() { return fAreaFlag; }

  void setX(Float_t fX) { fXcor = fX; }
  void setY(Float_t fY) { fYcor = fY; }
  void setXY(Float_t fX, Float_t fY) { fXcor = fX; fYcor = fY; }
  void setCornerNr(Int_t corner) { fCornerNr = corner; }
  void setAreaFlag(Int_t flag) { fAreaFlag = flag; }

private:

  Float_t fXcor;
  Float_t fYcor;
  Int_t fCornerNr;
  Int_t fAreaFlag;

  ClassDef(HRichPadCorner, 1)
};
//============================================================================

// ****************************************************************************

class HRichPad : public TObject {

// ****************************************************************************

public:

   HRichPad();
   virtual ~HRichPad();
   HRichPad(const HRichPad& source);
   HRichPad& operator=(const HRichPad& source); 

   void reset();
   // void clear();

   Bool_t isOut(Float_t x, Float_t y);
   Bool_t isOutX(Float_t x);

   HRichPadCorner *getCorner(Int_t n);
   Int_t addCorner(HRichPadCorner* corner);
   Int_t setCorner(HRichPadCorner* corner, Int_t n);
   Int_t getCornersNr() { return fCornersNr; }


   Float_t getXmin() { return fXmin; }
   Float_t getYmin() { return fYmin; }
   Float_t getXmax() { return fXmax; }
   Float_t getYmax() { return fYmax; }
   void setXmin(Float_t xmin) { fXmin = xmin; }
   void setYmin(Float_t ymin) { fYmin = ymin; }
   void setXmax(Float_t xmax) { fXmax = xmax; }
   void setYmax(Float_t ymax) { fYmax = ymax; }

   void calcPadCenter();

   Float_t getX() { return fX; }
   Float_t getY() { return fY; }
   Int_t  getPadX() { return fPadX; }
   Int_t  getPadY() { return fPadY; }
   Int_t  getPadNr() { return fPadNr; }
   void   getXY(Float_t *pX, Float_t *pY);
   void   getPadXY(Int_t *pX, Int_t *pY);
   Int_t  getPadFlag() { return fPadFlag; }
   Bool_t getPadActive() { return fPadActive; }

   Float_t getXlab(const Int_t sec) { return fXlab[sec]; }
   Float_t getYlab(const Int_t sec) { return fYlab[sec]; }
   Float_t getZlab(const Int_t sec) { return fZlab[sec]; }
   Float_t getTheta() { return fTheta; }
   Float_t getPhi(const Int_t sec) { return fPhi[sec]; }
   void getXYZlab(const Int_t sec, Float_t *pX, Float_t *pY, Float_t *pZ);
   void getAngles(const Int_t sec, Float_t *pTheta, Float_t *pPhi);
   Float_t getAmplitFraction() { return fAmplitFraction; }

   void setX(const Float_t x) { fX = x; }
   void setY(const Float_t y) { fY = y; }
   void setPadX(Int_t padx) { fPadX = padx; }
   void setPadY(Int_t pady) { fPadY = pady; }
   void setPadNr(Int_t padnr) { fPadNr = padnr; }
   void setXY(const Float_t x, const Float_t y) { fX = x; fY = y; }
   void setPadXY(Int_t padx, Int_t pady) { fPadX = padx; fPadY = pady; }
   void setPadFlag(Int_t flag) { fPadFlag = flag; }
   void setPadActive(Bool_t active) { fPadActive = active; }
   void setAmplitFraction(Float_t ampl) { fAmplitFraction = ampl; }

   void setXlab(const Int_t sec, const Float_t x) { fXlab[sec] = x; }
   void setYlab(const Int_t sec, const Float_t y) { fYlab[sec] = y; }
   void setZlab(const Int_t sec, const Float_t z) { fZlab[sec] = z; }
   void setXYZlab(const Int_t sec, const Float_t x, 
                  const Float_t y, const Float_t z) {
                    fXlab[sec] = x;
                    fYlab[sec] = y;
                    fZlab[sec] = z;                 
                  }
   void setTheta(const Float_t thetha) { fTheta = thetha; }
   void setPhi(const Int_t sec, const Float_t phi) { fPhi[sec] = phi; }

   void CalcNrtoXY(Int_t padsx);
   void CalcXYtoNr(Int_t padsx);


private:

   Int_t fCornersNr;
   TObjArray fPadCornersArray;

   Float_t fX; // X of a pad in mm
   Float_t fY; // Y of a pad in mm
   Int_t fPadX; // X of a pad in pads units
   Int_t fPadY; // Y of a pad in pads units
   Int_t fPadNr;
   Int_t fPadFlag; // 0-total outside, 1-inside, 2-partial inside
   Float_t fAmplitFraction; // fraction amplitude

   Float_t fXlab[6]; // X of a pad in LAB in sector 0..5
   Float_t fYlab[6]; // Y of a pad in LAB in sector 0..5
   Float_t fZlab[6]; // Z of a pad in LAB in sector 0..5
   Float_t fTheta; // Theta of a pad 
   Float_t fPhi[6]; // Phi of a pad in sector 0..5

   Float_t fXmin, fYmin, fXmax, fYmax;

   Bool_t fPadActive;

   ClassDef(HRichPad, 1)
};
//============================================================================

// ****************************************************************************

class HRichPadTab : public HParSet {

// ****************************************************************************

public:

   HRichPadTab();
   ~HRichPadTab();

   Bool_t initParameters();
   Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
   Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
   void reset();

   Int_t createPads(Int_t nX, Int_t nY);
   void deletePads();

   void setPad(HRichPad* pPad, Int_t nCol, Int_t nRow);
   void setPad(HRichPad* pPad, Int_t nPadNr);

   HRichPad* getPad(Int_t nPadNr) { return (HRichPad*)getObject(nPadNr); }
   HRichPad* getPad(Int_t nCol, Int_t nRow);
   HRichPad* getPad(Float_t  fXpos, Float_t fYpos);

   Int_t getPadsNr() { return fPadsNr; }
   Int_t getActivePadsNr() { return fActivePadsNr; }
   Int_t getPadsX() { return fPadsX; }
   Int_t getPadsY() { return fPadsY; }

   void incActivePadsNr() {fActivePadsNr++;}
   Bool_t isOut(Int_t nX, Int_t nY);


private:

   Int_t calcAddr (Int_t nX, Int_t nY) { return nX + fPadsX * nY; }

   TObject*& getSlot(int nIndx);
   TObject* getObject(int nIndx);

   TClonesArray* fPadsArray;
   Int_t fPadsNr;
   Int_t fActivePadsNr;
   Int_t fPadsX;
   Int_t fPadsY;

   Int_t fPadsLongestRow; 
   Int_t fMiddlePad; // first in the right side of the detector
   Float_t fPadsLongestRowMiddle;

   ClassDef(HRichPadTab, 1)
};
//============================================================================

#endif // RICHPAD_H
