#ifndef HSHOWERDIGI_PAD
#define HSHOWERDIGI_PAD

#include <TObject.h>
#include <TObjArray.h>
#include <TClonesArray.h>
#include "hparset.h"
#include "hshowerframe.h"
#include "hshowergeantwire.h"

class HShowerPad  : public TObject{
public:
   HShowerPad();
   virtual ~HShowerPad();
   HShowerPad(const HShowerPad& srcPad);

   void reset();

   Int_t isOut(HShowerGeantWire *qhit);
   Int_t isOut(Float_t x, Float_t y);
   Int_t isOutX(Float_t x, Float_t y);
   Int_t isOutY(Float_t x, Float_t y);

   void getPadCenter(Float_t *pfX, Float_t *pfY);

   Int_t getPadNr(){return nPadNr;}
   void getPadPos(Int_t *pRow, Int_t *pCol);
   Int_t getPadFlag(){return nPadFlag;}
   Int_t getLinkedNr(){return nLinkedNr;}
   void setPadFlag(Int_t nFlag){nPadFlag = nFlag;}
   void setPadNr(Int_t nPad){nPadNr = nPad;}
   void setPadsX(Int_t nPads){nPadsX = nPads;}
   void setLinkedNr(Int_t nNr){nLinkedNr = nNr;}

   Float_t getXld_mm(void) const { return 10.0f * fXld; }
   Float_t getYld_mm(void) const { return 10.0f * fYld; }
   Float_t getXlu_mm(void) const { return 10.0f * fXlu; }
   Float_t getYlu_mm(void) const { return 10.0f * fYlu; }
   Float_t getXrd_mm(void) const { return 10.0f * fXrd; }
   Float_t getYrd_mm(void) const { return 10.0f * fYrd; }
   Float_t getXru_mm(void) const { return 10.0f * fXru; }
   Float_t getYru_mm(void) const { return 10.0f * fYru; }

   HShowerPad& operator=(const HShowerPad& source); 

   //coordinates of corners of pad
   Float_t fXld,fYld,fXlu,fYlu; // d - down, u - up
   Float_t fXrd,fYrd,fXru,fYru; // l - left, r - right
   HShowerPad **linkPad; //!  excluded by root3 ????

private:
   Int_t nPadNr;   //pad identification
   Int_t nPadsX;   //number of pads in one raw
   Int_t nPadFlag; // 0-total outside, 1-inside, 2-partial inside
   Float_t qpad;
   Int_t nLinkedNr;  //????


   ClassDef(HShowerPad, 2)
};

class HShowerPadTab  : public HShowerParSet{
public:
   HShowerPadTab();
   ~HShowerPadTab();

   Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
   Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
   void reset();

   Int_t createPads(Int_t nX, Int_t nY);
   void deletePads();

   void setPad(HShowerPad* pPad, Int_t nRow, Int_t nCol);
   void setPad(HShowerPad* pPad, Int_t nPadNr);

   HShowerPad* getPad(Int_t nPadNr);
   HShowerPad* getPad(Int_t nRow, Int_t nCol);
   HShowerPad* getPad(Float_t  fXpos, Float_t fYpos);

   Int_t getNPad(){return nPads;}
   Int_t getNPadX(){return nPadsX;}
   Int_t getNPadY(){return nPadsY;}

   Bool_t isOut(Int_t nX, Int_t nY);

   void setFrame(HShowerFrame *pFrame){m_pFrame = pFrame;}
   HShowerFrame* getFrame(){return m_pFrame;}
   void setModuleID(Int_t id){m_nModuleID = id;}
   Int_t getModuleID(){return m_nModuleID;}

  
private:
//   Int_t calcAddr(Int_t nX, Int_t nY){return nX + nPadsX * nY;}
   Int_t calcAddr(Int_t nRow, Int_t nCol){return nCol + nPadsX * nRow;}

   TObject*& getSlot(Int_t nIndx);
   TObject* getObject(Int_t nIndx);
   TClonesArray* m_pPadArr;  //table of pads

   Int_t nPads;  //total number of pads
   Int_t nPadsX; //number of rows
   Int_t nPadsY; //number of columns

   Int_t m_nModuleID; //module identification

   HShowerFrame *m_pFrame;  //reference to frame for this module

   ClassDef(HShowerPadTab, 2)
};
#endif
