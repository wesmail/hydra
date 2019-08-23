#ifndef HSHOWERDIGI_PAD
#define HSHOWERDIGI_PAD

#include <TObject.h>
#include <TObjArray.h>
#include "hparset.h"
#include "hshowerframe.h"
#include "hshowergeantwire.h"
#include "hadioascii.h"

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

   Int_t getPadNr(){return nPadNr;}
   void getPadPos(Int_t *pnX, Int_t *pnY);
   Int_t getPadFlag(){return nPadFlag;}
   Int_t getLinkedNr(){return nLinkedNr;}
   void setPadFlag(Int_t nFlag){nPadFlag = nFlag;}
   void setPadNr(Int_t nPad){nPadNr = nPad;}
   void setPadsX(Int_t nPads){nPadsX = nPads;}
   void setLinkedNr(Int_t nNr){nLinkedNr = nNr;}

   HShowerPad& operator=(const HShowerPad& source); 

   Float_t fXld,fYld,fXlu,fYlu; // d - down, u - up
   Float_t fXrd,fYrd,fXru,fYru; // l - left, r - right
   HShowerPad **linkPad;

private:
   int nPadNr;
   int nPadsX;
   int nPadFlag; // 0-total outside, 1-inside, 2-partial inside
   float qpad;
   int nLinkedNr;


   ClassDef(HShowerPad, 0)
};

class HShowerPadTab  : public HParSet{
public:
   HShowerPadTab();
   ~HShowerPadTab();

   Bool_t init();
   void reset();

   Int_t createPads(Int_t nX, Int_t nY);
   void deletePads();

   void setPad(HShowerPad* pPad, Int_t nRow, Int_t nCol);
   void setPad(HShowerPad* pPad, Int_t nPadNr);

   HShowerPad* getPad(Int_t nPadNr);
   HShowerPad* getPad(Int_t nRaw, Int_t nCol);
   HShowerPad* getPad(Float_t  fXpos, Float_t fYpos);

   Int_t getNPad(){return nPads;}
   Int_t getNPadX(){return nPadsX;}
   Int_t getNPadY(){return nPadsY;}

   Bool_t isOut(Int_t nX, Int_t nY);

   void setIO(HadIoAscii *pReadParam){m_pReadParam = pReadParam;}
   HadIoAscii *getIO(){return m_pReadParam;}
   void setFrame(HShowerFrame *pFrame){m_pFrame = pFrame;}
   HShowerFrame* getFrame(){return m_pFrame;}
  
private:
   Int_t calcAddr(Int_t nX, Int_t nY){return nX + nPadsX * nY;}

   TObject*& getSlot(int nIndx);
   TObject* getObject(int nIndx);
   TClonesArray* m_pPadArr;

   Int_t nPads;
   Int_t nPadsX;
   Int_t nPadsY;

   HShowerFrame *m_pFrame;

   HadIoAscii *m_pReadParam;
   ClassDef(HShowerPadTab, 0)
};
#endif
