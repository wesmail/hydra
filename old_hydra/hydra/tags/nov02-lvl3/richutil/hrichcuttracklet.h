// $Id: hrichcuttracklet.h,v 1.1 2003-01-24 16:19:19 eberl Exp $
// Last update by Thomas Eberl: 03/01/17 15:01:13
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

  void printCutList(char *);
  void reset();
  void setStandardCuts();
    void switchTo(char*);
    Bool_t check(HHitMatch*);
    void setState(Int_t*,Float_t*);
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
protected:

  // meta info on cut
  

  // values
    Float_t nRingPadNr;
    Float_t nRingAvChrg;
    Float_t nRingCentroid;
    Float_t nRingPatMat;
    Float_t nRichMdcTheta;
    Float_t nRichMdcPhi;

    // log whether active
    Int_t  kAngularMatch;
    Int_t  kGoodRing;
    Int_t  kRingPadNr;
    Int_t  kRingAvChrg;
    Int_t  kRingCentroid;
    Int_t  kRingPatMat;
    Int_t  kRichMdcTheta;
    Int_t  kRichMdcPhi;



    ClassDef(HRichCutTracklet,1) // Single particle cuts

};

#endif
