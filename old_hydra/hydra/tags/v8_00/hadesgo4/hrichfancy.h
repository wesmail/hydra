#ifndef HRICHFANCY_H
#define HRICHFANCY_H
using namespace std;
#include "TNamed.h"
#include "TString.h"
#include "TArrayI.h"
#include "TObjArray.h"
#include "TBRIK.h"
#include "TNode.h"
#include "TTRAP.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream> 
#include <iomanip>

class HRichFancy: public TNamed{
protected:
    Int_t fIsActive;
    Int_t fIsResetable;
    Int_t refreshrate;
    TString Option;
    TString directory;
    TString format;
    Int_t stat;
    Int_t max;
    Int_t min;
    Bool_t isKnownToGUI;
    TBRIK* MainBrik;
    TNode* MainNode;
    TTRAP** trapezoids; 
    TObjArray* geoNodeArray;
    Int_t padnr2logcoord[60000];

public:
    HRichFancy(const char* name   ="",
	       const char* title  ="",
	       Int_t active=0,Int_t resetable=0);
    ~HRichFancy();
    Int_t    getMapping(Int_t j)         {return padnr2logcoord[j];}
    TBRIK*   getMainBrik()               {return MainBrik;}
    TNode*   getMainNode()               {return MainNode;} 
    TObjArray* getNodeArray()            {return geoNodeArray;}
    TNode*   getGeoNode(Int_t i)         {return (TNode*)((*geoNodeArray)[i]);}
    Bool_t   getKnown()                  {return isKnownToGUI;}
    void     setKnown(Bool_t k)          {isKnownToGUI=k;}
    void     setActive   (Int_t active)  {fIsActive=active;}
    Int_t    getActive   ()              {return fIsActive;}
    void     setResetable(Int_t reset)   {fIsResetable=reset;}
    Int_t    getResetable()              {return fIsResetable;}
    void     setRefreshRate(Int_t fresh) {refreshrate=fresh;}
    Int_t    getRefreshRate()            {return refreshrate;}
    void     reset(Int_t level,Int_t count=0);
    void     create();
    void     draw();
    void     init();

    //auxiliary functions to compute pad trapezoids:

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
    ClassDef(HRichFancy,1) // fancy rich display
};

#endif  /*!HRICHFANCY_H*/
