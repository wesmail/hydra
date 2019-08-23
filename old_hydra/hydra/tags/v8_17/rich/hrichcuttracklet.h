// $Id: hrichcuttracklet.h,v 1.2 2008-05-09 16:16:19 halo Exp $
// Last update by Thomas Eberl: 03/07/23 20:34:23
//
#ifndef HRICHCUTTRACKLET_H
#define HRICHCUTTRACKLET_H

#include "hrichcuto.h"

class HHitMatch;

class HRichCutTracklet : public HRichCutO {

public:
  HRichCutTracklet();
  HRichCutTracklet(Text_t *name, Text_t *title);
  ~HRichCutTracklet() {}

  void printCutList(Char_t *);
  void printCutList();
  void reset();
  void setStandardCuts();
    Bool_t switchTo(const Char_t*,Bool_t clear=kTRUE);
    Bool_t check(HHitMatch*);
//     void setRingAmplCut(Int_t n){nRingAmpl=n;};
//     void setRingAmplStatus(Int_t c){kRingAmpl=c;};

    void setRingPadNrCut(Float_t n){nRingPadNr=n;};
    void setRingPadNrStatus(Int_t c){kRingPadNr=c;};

    void setRingAvChrgCut(Float_t n){nRingAvChrg=n;};
    void setRingAvChrgStatus(Int_t c){kRingAvChrg=c;};

    void setRingCentroidCut(Float_t n){nRingCentroid=n;};
    void setRingCentroidStatus(Int_t c){kRingCentroid=c;};

    void setRingPatMatCut(Float_t n){nRingPatMat=n;};
    void setRingPatMatStatus(Int_t c){kRingPatMat=c;};

    void setRichMdcThetaCut(Float_t n){nRichMdcTheta=n;};
    void setRichMdcThetaStatus(Int_t c){kRichMdcTheta=c;};

    void setRichMdcPhiCut(Float_t n){nRichMdcPhi=n;};
    void setRichMdcPhiStatus(Int_t c){kRichMdcPhi=c;};
    //
    //
//     Int_t getRingAmplCut(){return nRingAmpl;};
//     Int_t getRingAmplStatus(){return kRingAmpl;};

    Float_t getRingPadNrCut(){return nRingPadNr;};
    Int_t getRingPadNrStatus(){return kRingPadNr;};

    Float_t getRingAvChrgCut(){return nRingAvChrg;};
    Int_t getRingAvChrgStatus(){return kRingAvChrg;};

    Float_t getRingCentroidCut(){return nRingCentroid;};
    Int_t getRingCentroidStatus(){return kRingCentroid;};

    Float_t getRingPatMatCut(){return nRingPatMat;};
    Int_t getRingPatMatStatus(){return kRingPatMat;};

    Float_t getRichMdcThetaCut(){return nRichMdcTheta;};
    Int_t getRichMdcThetaStatus(){return kRichMdcTheta;};

    Float_t getRichMdcPhiCut(){return nRichMdcPhi;};
    Int_t getRichMdcPhiStatus(){return kRichMdcPhi;};


    Bool_t isAngMatch(HHitMatch *h);
    Bool_t isGoodRing(HHitMatch *h);
    Bool_t isSector(HHitMatch *h);
    Bool_t isBetaLT(HHitMatch *h);
    Bool_t isBetaGT(HHitMatch *h);
    Bool_t isTofLT(HHitMatch *h);
    Bool_t isTofGT(HHitMatch *h);
    Bool_t isSys(HHitMatch *h);
    Bool_t isEle(HHitMatch *h);
    Bool_t isPos(HHitMatch *h);
    Bool_t isMdcChi2(HHitMatch *h);
    Bool_t isPull(HHitMatch *h);
    Bool_t isShowerCond(HHitMatch *h);
    Bool_t isBetaMom3s(HHitMatch *h);
    Bool_t isPullMom3s(HHitMatch *h);

protected:

    void setAngularMatch();
    void setGoodRing();
    void setMatchedGoodRing();
    void setSector(Int_t);
    void setEvtType(Int_t t){evtType = t;};
  // meta info on cut
      Bool_t isExp;//!
    Bool_t listCut;//!
  // values
    Float_t nRingPadNr;
    Float_t nRingAvChrg;
    Float_t nRingCentroid;
    Float_t nRingPatMat;
    Float_t nRichMdcTheta;
    Float_t nRichMdcPhi;
    Int_t   nSector;
    Float_t nBetaGT;
    Float_t nBetaLT;
    Float_t nTofGT;
    Float_t nTofLT;
    Int_t   nSys;
    Float_t nPull;
    // log whether active
    Int_t  kAngularMatch;
    Int_t  kGoodRing;
    Int_t  kRingPadNr;
    Int_t  kRingAvChrg;
    Int_t  kRingCentroid;
    Int_t  kRingPatMat;
    Int_t  kRichMdcTheta;
    Int_t  kRichMdcPhi;
    Int_t  kSector;
    Int_t  kBetaGT;
    Int_t  kBetaLT;
    Int_t  kTofGT;
    Int_t  kTofLT;
    Int_t  kSys;
    Int_t  kEle;
    Int_t  kPos;
    Int_t  kPull;
    Int_t  kShower;
    Int_t  kMdcChi2;
    Int_t  kBetaMom3s;
    Int_t  kPullMom3s;
    // input is sim or exp file
    Int_t evtType;
    ClassDef(HRichCutTracklet,2) // Single particle cuts

};

#endif
