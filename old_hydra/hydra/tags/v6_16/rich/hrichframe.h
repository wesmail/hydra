#ifndef HRICHFRAME_H
#define HRICHFRAME_H
#pragma interface

#include <TObject.h>
#include <TObjArray.h>
#include "hrichparset.h"


class HRichFrameCorner: public TObject
{
public:

  HRichFrameCorner();
  ~HRichFrameCorner() {}

  Float_t getX() { return fXcor; }
  Float_t getY() { return fYcor; }
  void getXY(Float_t *fX, Float_t *fY) { *fX = fXcor; *fY = fYcor; }
  Int_t getCornerNr() { return fCornerNr; }
  Int_t getFlagArea() { return nFlagArea; }

  void setX(Float_t fX) { fXcor = fX; }
  void setY(Float_t fY) { fYcor = fY; }
  void setXY(Float_t fX, Float_t fY) { fXcor = fX; fYcor = fY; }
  void setCornerNr(Int_t corner) { fCornerNr = corner; }
  void setFlagArea(Int_t nFlag) { nFlagArea = nFlag; }

private:

  Float_t fXcor;
  Float_t fYcor;
  Int_t fCornerNr;
  Int_t nFlagArea;
 
  ClassDef(HRichFrameCorner, 1)
};


class HRichFrame: public HRichParSet
{
  public:

    HRichFrame();
    virtual ~HRichFrame();

    void reset();
    virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
    virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);

    Int_t isOut(Float_t x, Float_t y);

    HRichFrameCorner* getCorner(Int_t n);
    Int_t addCorner(HRichFrameCorner*);
    Int_t setCorner(HRichFrameCorner* pCorner, Int_t n); 

    void setCornerNr(Int_t n) { nCornerNr = n; }
    Int_t getCornerNr() { return nCornerNr; }

    Float_t getPadsWiresDist() { return fPadsWiresDistance; }
    void setPadsWiresDist(Float_t dist) { fPadsWiresDistance = dist; }

    ClassDef(HRichFrame, 1)

private:

  Int_t nCornerNr;
  TObjArray m_FrameArr;
  Float_t fPadsWiresDistance;
};




class HRichFrameTab : public HRichParSet
{
  public:

    HRichFrameTab();
    ~HRichFrameTab();

    virtual Bool_t init();

    void createFrames(Int_t n);
    Int_t isOut(Int_t nFrame, Float_t x, Float_t y);
    void deleteFrames();
    Int_t getNFrames();
    HRichFrame* getFrame(Int_t n);    

   
  private:

    Int_t nrFrames;
    HRichFrame *phFrame;


  ClassDef(HRichFrameTab, 1)
};

#endif // HRICHFRAME_H
