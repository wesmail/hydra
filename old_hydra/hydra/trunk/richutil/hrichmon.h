// $Id: hrichmon.h,v 1.9 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/11/22 00:13:47
//
#ifndef HRICHMON_H
#define HRICHMON_H

#include "hreconstructor.h"
#include "hparset.h"
#include "TFile.h"
#include "TEventList.h"
#include "TText.h"
#include "TNode.h"
#include "TMatrix.h"
#include "TString.h"
#include "TCanvas.h"
#include "TVector.h"
#include "TView.h"
#include "TMath.h"
class HRichPadTab;
class HCategory;
class HIterator;
class HRichHit;
class HRichMon : public HReconstructor {
public:

    HRichMon();
    HRichMon(const Text_t *name,const Text_t *title);
    HRichMon(const Text_t *name,const Text_t *title,const Char_t *swt,const Char_t* geofilename);
    ~HRichMon();

    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    ClassDef(HRichMon,1) //ROOT extension

	protected:
    Int_t nEvtsProcessed;//!
    TString *pCatSwitch; //!
    TFile *pFile; //!
    TEventList *el; //!
    TText *evtnumbertext; //! 
    TText *datetext; //!
    Int_t nPads; //!
    Int_t nSec; //!
    Float_t *memCharges;//!
    HParSet* pGeomPar;//!
    HCategory *pCat;//!
    HIterator *pIter; //!
    HCategory *pHitCat;//!
    HIterator *pHitIter; //!
    HCategory *pKickCat;//!
    HIterator *pKickIter; //!
    HCategory *pMatchCat;//!
    HIterator *pMatchIter; //!
    HCategory *pPairCat;//!
    HIterator *pPairIter; //!
    HRichPadTab *pPadsTable; //!
    void setGeometryPar(HParSet* pPar){ pGeomPar = pPar;};
    HParSet*   getGeometryPar(){return pGeomPar;};
    void setPadTable(HRichPadTab* tab){pPadsTable = tab;};
    HRichPadTab* getPadTable(void){return pPadsTable;};
    void initGeoData();
    void initWindow();
    void resetMemCharges();
    void makePadsInvisible();
    void rotateDetectorFancy();
    Bool_t testHits();
    Bool_t testPairs();
    Bool_t isGoodRing(HRichHit*);
    void makeHitAnnotation(TObjArray&,TObjArray&);
    void makeTrackAnnotation(TObjArray&,TObjArray&);
    void makeLepTrackAnnotation(TObjArray&,TObjArray&);
    void makeEvtAnnotation();
    void deleteAnnotation(TObjArray&,TObjArray&);
    void waitOnKey();
    void showFiredPads();
    void printWindow();
    void updateWindow();
    void delayUpdate(Float_t);
    Bool_t selectEvt();
    void getPadXYforThetaPhi(Float_t,Float_t,Int_t&,Int_t&,Int_t);
    Int_t getPadXforPhi(Float_t,Int_t,Int_t);
    Int_t getPadYforTheta(Float_t,Int_t);
    TNode** getNode(){return node;};
    TMatrix* getMapTable(){return mapTable;};
    TCanvas* window;//!
    TView* viewer;//!
    TVector *c1x,*c1y,*c2x,*c2y,*c3x,*c3y,*c4x,*c4y;//!
    TNode** node;//!
    TMatrix* mapTable;//!
    Float_t zoomFactor;//!
    Float_t h1(Float_t x1,Float_t x2,Float_t x3, Float_t x4){
	return (x3-x4+x2-x1)/4.0;
    }

    Float_t h2(Float_t x1,Float_t x2,Float_t x3, Float_t x4){
	return (x3-x4+x2-x1)/4.0;
    }

    Float_t tl1(Float_t y1, Float_t y4){
	return (y4-y1)/2.0;
    }

    Float_t tl2(Float_t y1, Float_t y4){
	return (y4-y1)/2.0;
    }

    Float_t bl1(Float_t y2, Float_t y3){
	return (y3-y2)/2.0;
    }
    
    Float_t bl2(Float_t y2, Float_t y3){
  return (y3-y2)/2.0;
    }
    
    Float_t alpha1(Float_t x1, Float_t x2, Float_t y1, Float_t y2, Float_t y3, Float_t y4){
	return (TMath::ATan((y3+y2-y4-y1)/(x2-x1)))*180.0/(TMath::Pi());
    }
    
    Float_t alpha2(Float_t x1, Float_t x2, Float_t y1, Float_t y2, Float_t y3, Float_t y4){
	return (TMath::ATan((y3+y2-y4-y1)/(x2-x1)))*180.0/TMath::Pi();
    }

    Float_t xd(Float_t x1, Float_t x2, Float_t x3, Float_t x4){
	return (x1+x2+x3+x4)/4.;
    }

    Float_t yd(Float_t y1, Float_t y2, Float_t y3, Float_t y4){
	return (y1+y2+y3+y4)/4;
    }

};

#endif // HRICHMON_H

