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

   int isOut(HShowerGeantWire *qhit);
   int isOut(float x, float y);
   int isOutX(float x, float y);
   int isOutY(float x, float y);

   void getPadCenter(Float_t *pfX, Float_t *pfY);

   Int_t getPadNr(){return nPadNr;}
   void getPadPos(Int_t *pRow, Int_t *pCol);
   Int_t getPadFlag(){return nPadFlag;}
   Int_t getLinkedNr(){return nLinkedNr;}
   void setPadFlag(Int_t nFlag){nPadFlag = nFlag;}
   void setPadNr(Int_t nPad){nPadNr = nPad;}
   void setPadsX(Int_t nPads){nPadsX = nPads;}
   void setLinkedNr(Int_t nNr){nLinkedNr = nNr;}

   HShowerPad& operator=(const HShowerPad& source); 

   //coordinates of corners of pad
   Float_t fXld,fYld,fXlu,fYlu; // d - down, u - up
   Float_t fXrd,fYrd,fXru,fYru; // l - left, r - right
   HShowerPad **linkPad; //????

private:
   int nPadNr;   //pad identification
   int nPadsX;   //number of pads in one raw
   int nPadFlag; // 0-total outside, 1-inside, 2-partial inside
   float qpad;
   int nLinkedNr;  //????


   ClassDef(HShowerPad, 1)
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
   void setModuleID(int id){m_nModuleID = id;}
   int getModuleID(){return m_nModuleID;}

  
private:
//   Int_t calcAddr(Int_t nX, Int_t nY){return nX + nPadsX * nY;}
   Int_t calcAddr(Int_t nRow, Int_t nCol){return nCol + nPadsX * nRow;}

   TObject*& getSlot(int nIndx);
   TObject* getObject(int nIndx);
   TClonesArray* m_pPadArr;  //table of pads

   Int_t nPads;  //total number of pads
   Int_t nPadsX; //number of rows
   Int_t nPadsY; //number of columns

   Int_t m_nModuleID; //module identification

   HShowerFrame *m_pFrame;  //reference to frame for this module

   ClassDef(HShowerPadTab, 1)
};
#endif
