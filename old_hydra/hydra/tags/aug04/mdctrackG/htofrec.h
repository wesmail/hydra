#ifndef  HTOFREC_H
#define  HTOFREC_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hevent.h"
#include "hrecevent.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TNtuple.h>

class HIterator;
class HCategory;

class HTofRec : public HReconstructor {

protected:
    Bool_t fRichMode;
    Bool_t fPlusMode;
    Bool_t fMinusMode;
    Float_t fFieldCorrection;
    Bool_t fDebugMode;
    Bool_t fKickHires;
    Bool_t fKickLowres;
    Bool_t fKickLowresB;
    Bool_t fSpline;

    HCategory* fSplineCat;
    HCategory* fKickLowCat;
    HCategory* fKickLowBCat;
    HCategory* fKickHiCat;
    HCategory* fRichCat;
    HCategory* fTofCat;
    HCategory* fTofinoCat;
 

    HIterator* iterSpline; 
    HIterator* iterKickLowres;
    HIterator* iterKickLowresB;
    HIterator* iterKickHires;
    HIterator* iterRich;
    
    Double_t C;
    Float_t correct[2];
    Float_t rich_params[5];
    TNtuple* before;
    TNtuple* after;
    TNtuple* richcorr;
    Bool_t fillNTuple;
    TFile* r;
    
    void track_recalc(Int_t tracking,Float_t start = 0, Int_t flag = 0); 
 
 public:    
    HTofRec(void);
    HTofRec(Text_t name[],Text_t title[],const char *select="");
    ~HTofRec(void);
    Bool_t init(void);
    Bool_t finalize(void);
    Int_t execute(void);

    void setRichMode(Bool_t r)         {fRichMode=r;}
    void setMinusMode(Bool_t m)        {fMinusMode=m;}
    void setPlusMode(Bool_t p)         {fMinusMode=p;}
    void setFieldCorrection(Float_t f) {fFieldCorrection=f;}
    void setDebug(Bool_t d)            {fDebugMode=d;}
    void setFillNTuple(Bool_t fill)    {fillNTuple=fill;}
    void setRichParams(Float_t theta0, Float_t dtheta, Float_t phi0, Float_t dphi, Float_t lambda2) {
      rich_params[0] = theta0;
      rich_params[1] = dtheta;
      rich_params[2] = phi0;
      rich_params[3] = dphi;
      rich_params[4] = lambda2;
    }
    
    ClassDef(HTofRec,0)
      };
#endif /* !HTOFREC_H */







