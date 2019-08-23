#ifndef __HSHOWERDIGI_FRAME
#define __HSHOWERDIGI_FRAME
#pragma interface

#include <TObject.h>
#include <TObjArray.h>
#include "hshowerparset.h"

class HShowerFrameCorner: public TObject
{
public:
  HShowerFrameCorner();

  Float_t getX(){return fXcor;}
  Float_t getY(){return fYcor;}
  Int_t getFlagArea(){return nFlagArea;}

  void setX(Float_t fX){fXcor = fX;}
  void setY(Float_t fY){fYcor = fY;}
  void setFlagArea(Int_t nFlag){nFlagArea = nFlag;}
private:
  Float_t fXcor;  //X coordinate of frame 
  Float_t fYcor;  //Y coordinate of frame
  Int_t nFlagArea;  //sensitive area flag
 
  ClassDef(HShowerFrameCorner, 1)
};

class HShowerFrame: public HShowerParSet
{
  public:
    HShowerFrame();
    virtual ~HShowerFrame();

    void reset();
    virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
    virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);

    Int_t isOut(Float_t x, Float_t y);

    HShowerFrameCorner* getCorner(Int_t n);
    Int_t addCorner(HShowerFrameCorner*);
    Int_t setCorner(HShowerFrameCorner* pCorner, Int_t n); 

    void setCornerNr(Int_t n){nCornerNr = n;}
    Int_t getCornerNr(){return nCornerNr;}

    void setModuleID(Int_t id){m_nModuleID = id;}
    Int_t getModuleID(){return m_nModuleID;}


    ClassDef(HShowerFrame, 1)
private:
  Int_t nCornerNr;  //number of corners
  Int_t m_nModuleID;  //module identification

  TObjArray m_FrameArr;  //table of corners coordinates
};



class HShowerFrameTab : public HShowerParSet
{
  public:
    HShowerFrameTab();
    ~HShowerFrameTab();

    virtual Bool_t init();

    void createFrames(Int_t n);
    Int_t isOut(Int_t nFrame, Float_t x, Float_t y);
    void deleteFrames();
    Int_t getNFrames();
    HShowerFrame* getFrame(Int_t n);    
   
  private:
    Int_t nrFrames;
    HShowerFrame *phFrame;


  ClassDef(HShowerFrameTab, 0)
};

#endif
