#ifndef HMDCEXTRACTORS_H
#define HMDCEXTRACTORS_H

#include "TObject.h"
#include "TString.h"
#include "hlocation.h"

class HMdcHit;
class HMdcSeg;
class HMdcTrkCand;
class HMdcClusInf;
class HMdcClusFit;
class HKickTrack;
class HRecEvent;

//---------------------------CAL1---------------------------------------------------

class HMdcCal1Extractor : public TObject {
protected:
    HLocation loccal;              //! location for new cal object.
    HRecEvent* cpevent;            //! pointer to copy event
public:
    HMdcCal1Extractor(HRecEvent* cpev=0);
    ~HMdcCal1Extractor();
    void extract(HMdcSeg*,Int_t);
    void clear();
    ClassDef(HMdcCal1Extractor,1) // extracts cal hit on a MDC
};

//---------------------------WIREFIT------------------------------------------------

class HMdcWireFitExtractor : public TObject {
protected:
    HLocation locwirefit;          //! location for new wirefit object.
    HRecEvent* cpevent;            //! pointer to copy event
public:
    HMdcWireFitExtractor(HRecEvent* cpev=0);
    ~HMdcWireFitExtractor();
    void extract(HMdcClusFit*,HMdcClusFit*,Int_t);
    void clear();
    ClassDef(HMdcWireFitExtractor,1) // extracts WireFit on a MDC
};

//---------------------------CLUSFIT------------------------------------------------

class HMdcClusFitExtractor : public TObject {
protected:
    HRecEvent* cpevent;                //! pointer to copy event
    HLocation locclusfit;              //! location for new clusfit object
    HMdcWireFitExtractor* wirefitext;  //! pointer to wirefit extractor
    Bool_t wirefitlevel;               //!
    Int_t hitlookup[500];             //! book keeping of hit index
    Int_t hitcplookup[500];           //! book keeping of copied hit index
    Int_t hitctr;                     //! counter of hits in lookup
public:
    HMdcClusFitExtractor(HRecEvent* cpev=0);
    ~HMdcClusFitExtractor();
    void extract(HMdcClusInf*,HMdcClusInf*,Int_t);
    void setDataLevels(TString);
    Int_t  checkHitIndex(Int_t index){for(Int_t i=0;i<hitctr;i++){if(hitlookup[i]==index) return hitcplookup[i] ;}return -1;}
    void   resetLookups() {for(Int_t i=0;i<hitctr;i++){hitlookup[i]=-1;hitcplookup[i]=-1;}hitctr=0;}
    void   initLookups()  {for(Int_t i=0;i<500;i++){hitlookup[i]=-1;hitcplookup[i]=-1;}hitctr=0;}
    void   clear();
    ClassDef(HMdcClusFitExtractor,1) // extracts ClusFit on a MDC
};

//---------------------------CLUSINF------------------------------------------------

class HMdcClusInfExtractor : public TObject {
protected:
    HRecEvent* cpevent;               //! pointer to copy event
    HMdcClusFitExtractor* clusfitext; //! pointer to clusfit extractor
    HLocation locclusinf;             //! location for new clusinf object.
    Bool_t clusfitlevel;              //!
    Bool_t clusterlevel;              //!
public:
    HMdcClusInfExtractor(HRecEvent* cpev=0);
    ~HMdcClusInfExtractor();
    void   extract(HMdcHit*,HMdcHit*,Int_t);
    void   setDataLevels(TString);
    void   clear();
    ClassDef(HMdcClusInfExtractor,1) // extracts ClusInf on a MDC
};

//---------------------------HIT----------------------------------------------------

class HMdcHitExtractor : public TObject {
protected:
    HRecEvent* cpevent;               //! pointer to copy event
    HMdcClusInfExtractor* clusinfext; //! pointer to clusinf extractor
    Int_t hitct[6][4];                //! counter of hit index
    Int_t hitctr;                     //! counter of hits in lookup
    Int_t hitlookup[500];             //! book keeping of hit index
    HLocation lochit;                 //! location for new hit object.
    Bool_t clusinflevel;              //!
public:
    HMdcHitExtractor(HRecEvent* cpev=0);
    ~HMdcHitExtractor();
    void   extract(HMdcSeg*,HMdcSeg*,Int_t);
    void   setDataLevels(TString);
    Bool_t checkHitIndex(Int_t index){for(Int_t i=0;i<hitctr;i++){if(hitlookup[i]==index) return kTRUE ;}return kFALSE;}
    void   resetLookup()   {for(Int_t i=0;i<hitctr;i++){hitlookup[i]=-1;}hitctr=0;}
    void   initLookup()    {for(Int_t i=0;i<500;i++){hitlookup[i]=-1;}hitctr=0;}
    void   resetCounters() {for(Int_t s=0;s<6;s++){for(Int_t m=0;m<4;m++){hitct[s][m]=0;}}}
    void   clear();
    ClassDef(HMdcHitExtractor,1) // extracts hit on a MDC
};

//---------------------------SEG----------------------------------------------------

class HMdcSegExtractor : public TObject {
protected:
    HRecEvent* cpevent;        //! pointer to copy event
    Int_t segct[6][2];         //! counter of seg index
    Int_t seglookup[500];      //! book keeping of seg index
    Int_t segcplookup[500];    //! book keeping of copied seg index
    Int_t segctr;              //! counter of segments in lookup
    HLocation locseg;          //! location for new seg object.
    HLocation lockick;         //! location for new kick object.
    Int_t kickct[6];           //! counter of kick track index
  
    HMdcSegExtractor* segext;  //! pointer to the seg extractor
    HMdcHitExtractor* hitext;  //! pointer to the hit extractor
    HMdcCal1Extractor* cal1ext;//! pointer to the cal1 extractor

    Bool_t kicklevel;  //!
    Bool_t hitlevel;   //!
    Bool_t callevel;   //!

    Int_t counter;
public:
    HMdcSegExtractor(HRecEvent* cpev=0);
    ~HMdcSegExtractor();
    void   extract(HKickTrack*,HMdcSeg*,Int_t,Int_t&);
    void   extract(HMdcSeg*,Int_t);
    void   setDataLevels(TString);
    Int_t  checkSegIndex(Int_t index){for(Int_t i=0;i<segctr;i++){if(seglookup[i]==index) return segcplookup[i] ;}return -1;}
    void   resetLookup()   {for(Int_t i=0;i<segctr;i++){seglookup[i]=-1;segcplookup[i]=-1;}segctr=0;}
    void   initLookup()    {for(Int_t i=0;i<500;i++){seglookup[i]=-1;segcplookup[i]=-1;}segctr=0;}
    void   resetCounters() {for(Int_t s=0;s<6;s++){ kickct[s]=0; for(Int_t m=0;m<2;m++){segct[s][m]=0;}}}
    void   clear();
    ClassDef(HMdcSegExtractor,1) // extracts seg on a MDC
};

//---------------------------TrackCand----------------------------------------------------

class HMdcTrkCandExtractor : public TObject {
protected:
    HRecEvent* cpevent;          //! pointer to copy event
    Int_t trkcandct[6];          //! counter of trkcand index
    Int_t trkcandlookup[500];    //! book keeping of trkcand index
    Int_t trkcandcplookup[500];  //! book keeping of copied trkcand index
    Int_t trkcandctr;            //! counter of trkcand in lookup
    HLocation loctrkcand;        //! location for new trkcand object.

    HMdcSegExtractor* segext;    //! pointer to the seg extractor

    Int_t counter;
public:
    HMdcTrkCandExtractor(HRecEvent* cpev=0);
    ~HMdcTrkCandExtractor();
    void   extract(HKickTrack*,HMdcTrkCand*,Int_t);
    void   setDataLevels(TString);
    Int_t  checkTrkCandIndex(Int_t index){for(Int_t i=0;i<trkcandctr;i++){if(trkcandlookup[i]==index) return trkcandcplookup[i] ;}return -1;}
    void   resetLookup()   {for(Int_t i=0;i<trkcandctr;i++){trkcandlookup[i]=-1;trkcandcplookup[i]=-1;}trkcandctr=0;}
    void   initLookup()    {for(Int_t i=0;i<500;i++){trkcandlookup[i]=-1;trkcandcplookup[i]=-1;}trkcandctr=0;}
    void   resetCounters() {for(Int_t s=0;s<6;s++){ trkcandct[s]=0;}}
    void   clear();
    ClassDef(HMdcTrkCandExtractor,1) // extracts HMdcTrkCand on a MDC
};

//---------------------------WIREINFO-------------------------------------------------

class HMdcWireInfoExtractor : public TObject {
protected:
    HRecEvent* cpevent;//! pointer to copy event
    HLocation loccal;  //!
    Bool_t kicklevel;  //!
    Bool_t seglevel;   //!
    Bool_t hitlevel;   //!
    Bool_t clusinflevel;   //!
    Bool_t clusfitlevel;   //!
    Bool_t wirefitlevel;   //!
    Bool_t clusterlevel;   //!

    Bool_t callevel;   //!

public:
    HMdcWireInfoExtractor(HRecEvent* cpev=0);
    ~HMdcWireInfoExtractor();
    void   extract(HKickTrack*,Int_t);
    void   setDataLevels(TString);
    void   clear(){;}
    ClassDef(HMdcWireInfoExtractor,1) // extracts wireinfo on a MDC
};

#endif  /* HMDCEXTRACTORS_H */

