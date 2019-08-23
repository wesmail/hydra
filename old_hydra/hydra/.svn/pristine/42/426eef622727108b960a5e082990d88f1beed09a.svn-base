// $Id: hrichpaircreator.h,v 1.6 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:12:48
//
#ifndef HRICHPAIRCREATOR_H
#define HRICHPAIRCREATOR_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "TString.h"

class HCategory;
class HIterator;
class HDiHitMatch;
class HHitMatch;
class TObjArray;
class TFile;

class HRichPairCreator : public HReconstructor {
public:

	HRichPairCreator();
        HRichPairCreator(const Text_t *name,const Text_t *title,const Char_t*);
	~HRichPairCreator();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        HDiHitMatch* createHitDiMatch(HHitMatch*,HHitMatch*);
        TObjArray* selectTracklets(void);
        Int_t makePairs(TObjArray*);
        Int_t fillHistosFromTracklets(TObjArray*);
        Int_t fillHistosFromPairs();
        Int_t fillHistosRingProperties(TObjArray*,const Char_t*);
        Int_t fillRingAmplitude(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingAmplitudeTheta(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingAmplitudeThetaSec(HDiHitMatch*,Int_t*,Int_t,const Char_t*,
				    const Char_t*,Int_t,Int_t);
        Int_t fillRingRadius(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingRadiusTheta(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingRadiusThetaSec(HDiHitMatch*,Int_t*,Int_t,const Char_t*,
				    const Char_t*,Int_t,Int_t);
        Int_t fillRingPadNr(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingPadNrTheta(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingPadNrThetaSec(HDiHitMatch*,Int_t*,Int_t,const Char_t*,
				    const Char_t*,Int_t,Int_t);
        Int_t fillRingLocMax4(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingLocMax4Theta(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingLocMax4ThetaSec(HDiHitMatch*,Int_t*,Int_t,const Char_t*,
				    const Char_t*,Int_t,Int_t);
        Int_t fillRingHT(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingHTTheta(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingHTThetaSec(HDiHitMatch*,Int_t*,Int_t,const Char_t*,
				    const Char_t*,Int_t,Int_t);
        Int_t fillRingPM(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingPMTheta(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillRingPMThetaSec(HDiHitMatch*,Int_t*,Int_t,const Char_t*,
				    const Char_t*,Int_t,Int_t);
        Int_t fillOpangMDC(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillOpangKICK(HDiHitMatch*,Int_t*,Int_t,const Char_t*);
        Int_t fillOpangMETA(HDiHitMatch*,Int_t*,Int_t);
        Int_t fillInvMass(HDiHitMatch*,Int_t*,Int_t);
        Int_t fillTof(HDiHitMatch*,Int_t*,Int_t*,Int_t);
        Int_t fillPt_vs_Pt(HDiHitMatch*,Int_t*,Int_t);
        Int_t fillPt_vs_Ptot(HDiHitMatch*,Int_t*,Int_t);
        Int_t fillPtot_vs_Ptot(HDiHitMatch*,Int_t*,Int_t);
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getHitDiMatchCat(){return pHitDiMatchCat;};
        ClassDef(HRichPairCreator,1) //ROOT extension

protected:
	Int_t nCounterProcessedNbEvents; 
	void initHistos();
        void initCuts();
	HCategory* pHitMatchCat; //!
        HIterator* pIterMatchHit; //!
	HCategory* pHitDiMatchCat; //!
        HIterator* pIterDiMatchHit; //!
        TObjArray* pHistArray;//!
        //FILENAME
        TString pFileName;//!
        //FILE
        TFile *pFileOut;//!
        // cut params
        Float_t nRichPolarLow;//!
        Float_t nRichPolarHigh;//!
};

#endif // HRICHPAIRCREATOR_H

