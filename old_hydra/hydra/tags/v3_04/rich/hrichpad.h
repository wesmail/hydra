#ifndef RICHPAD_H
#define RICHPAD_H

#include <TObject.h>
#include <TObjArray.h>
#include "hrichparset.h"


//----------------------------------------------------------------------------
class HRichPadCorner : public TObject
{
public:

  HRichPadCorner();
  ~HRichPadCorner();
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

  ClassDef(HRichPadCorner, 0)
};
//============================================================================

//----------------------------------------------------------------------------
class HRichPad : public TObject {

public:

   HRichPad();
   virtual ~HRichPad();
   HRichPad(const HRichPad& source);
   HRichPad& operator=(const HRichPad& source); 

   void reset();
   void clear();

   Bool_t isOut(Float_t x, Float_t y);
   Bool_t isOutX(Float_t x);

   HRichPadCorner *getCorner(Int_t n);
   Int_t addCorner(HRichPadCorner* corner);
   Int_t setCorner(HRichPadCorner* corner, Int_t n);
   Int_t getCornersNr() { return fCornersNr; }

   HRichPadCorner *getRealCorner(Int_t n);
   Int_t addRealCorner(HRichPadCorner* corner);
   Int_t setRealCorner(HRichPadCorner* corner, Int_t n);
   Int_t getRealCornersNr() { return fRealCornersNr; }

   void getPadCenter(Float_t *pfX, Float_t *pfY);

   Int_t  getPadX() { return fPadX; }
   Int_t  getPadY() { return fPadY; }
   Int_t  getPadNr() { return fPadNr; }
   void   getPadXY(Int_t *pX, Int_t *pY);
   Int_t  getPadFlag() { return fPadFlag; }
   Bool_t getPadActive() { return fPadActive; }
   Int_t  getPadAmplitude() { return fPadAmplitude; }
   Int_t  getPadLabel() { return fPadLabel; }
   Int_t  getPadLock() { return fPadLock; }
   Int_t  getLinkedPadsNr() { return fLinkedPadsNr; }

   void setPadX(Int_t padx) { fPadX = padx; }
   void setPadY(Int_t pady) { fPadY = pady; }
   void setPadNr(Int_t padnr) { fPadNr = padnr; }
   void setPadXY(Int_t padx, Int_t pady) { fPadX = padx; fPadY = pady; }
   void setPadFlag(Int_t flag) { fPadFlag = flag; }
   void setPadActive(Bool_t active) { fPadActive = active; }
   void setPadAmplitude(Int_t amplitude) { fPadAmplitude = amplitude; }
   void setPadLabel(Int_t label) { fPadLabel = label; }
   void setPadLock(Int_t lock) { fPadLock = lock; }
   void setLinkedPadsNr(Int_t nNr) { fLinkedPadsNr = nNr; }

   void CalcNrtoXY(Int_t padsx);
   void CalcXYtoNr(Int_t padsx);

   HRichPad **fLinkedPads;

private:

   Int_t fCornersNr;
   Int_t fRealCornersNr;
   TObjArray fPadCornersArray;
   TObjArray fPadRealCornersArray;

   Int_t fPadX;
   Int_t fPadY;
   Int_t fPadNr;
   Int_t fPadFlag; // 0-total outside, 1-inside, 2-partial inside

   Bool_t fPadActive;
   Int_t fPadAmplitude;
   Int_t fPadLabel;
   Int_t fPadLock;
   
   Int_t fLinkedPadsNr;


   ClassDef(HRichPad, 0)
};
//============================================================================

//----------------------------------------------------------------------------
class HRichPadTab : public HParSet {

public:

   HRichPadTab();
   ~HRichPadTab();

   Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
   void reset();

   Int_t createPads(Int_t nX, Int_t nY);
   void deletePads();

   void setPad(HRichPad* pPad, Int_t nCol, Int_t nRow);
   void setPad(HRichPad* pPad, Int_t nPadNr);

   HRichPad* getPad(Int_t nPadNr);
   HRichPad* getPad(Int_t nCol, Int_t nRow);
   HRichPad* getPad(Float_t  fXpos, Float_t fYpos);

   Int_t getPadsNr() { return fPadsNr; }
   Int_t getPadsX() { return fPadsX; }
   Int_t getPadsY() { return fPadsY; }

   Bool_t isOut(Int_t nX, Int_t nY);


private:

   Int_t calcAddr (Int_t nX, Int_t nY) { return nX + fPadsX * nY; }

   TObject*& getSlot(int nIndx);
   TObject* getObject(int nIndx);
   TClonesArray* fPadsArray;

   Int_t fPadsNr;
   Int_t fPadsX;
   Int_t fPadsY;

   Int_t fPadsLongestRow; // remember to initialise it !!!!!!!
   Float_t fPadsLongestRowMiddle;

   ClassDef(HRichPadTab, 0)
};
//============================================================================

#endif // RICHPAD_H
