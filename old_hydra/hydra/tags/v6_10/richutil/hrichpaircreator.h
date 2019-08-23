// File: hrichpaircreator.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/05/10 13:59:39
//
#ifndef HRICHPAIRCREATOR_H
#define HRICHPAIRCREATOR_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;
class HDiHitMatch;
class HHitMatch;
class TObjArray;
class TFile;

class HRichPairCreator : public HReconstructor {
public:

	HRichPairCreator();
        HRichPairCreator(Text_t *name,Text_t *title,Char_t*);
	~HRichPairCreator();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        HDiHitMatch* createHitDiMatch(HHitMatch*,HHitMatch*);
        TObjArray* selectTracklets(void);
        Int_t makePairs(TObjArray*);
        Int_t fillHistosFromPairs();
        Int_t fillHistosRingProperties(TObjArray*,Char_t*);
        Int_t fillRingAmplitude(HDiHitMatch*,Int_t*,Int_t,Char_t*);
        Int_t fillRingAmplitudeTheta(HDiHitMatch*,Int_t*,Int_t,Char_t*);
        Int_t fillRingAmplitudeThetaSec(HDiHitMatch*,Int_t*,Int_t,Char_t*,
				    Char_t*,Int_t,Int_t);
        Int_t fillRingPadNr(HDiHitMatch*,Int_t*,Int_t,Char_t*);
        Int_t fillRingPadNrTheta(HDiHitMatch*,Int_t*,Int_t,Char_t*);
        Int_t fillRingPadNrThetaSec(HDiHitMatch*,Int_t*,Int_t,Char_t*,
				    Char_t*,Int_t,Int_t);
        Int_t fillRingLocMax4(HDiHitMatch*,Int_t*,Int_t,Char_t*);
        Int_t fillRingLocMax4Theta(HDiHitMatch*,Int_t*,Int_t,Char_t*);
        Int_t fillRingLocMax4ThetaSec(HDiHitMatch*,Int_t*,Int_t,Char_t*,
				    Char_t*,Int_t,Int_t);
        Int_t fillRingHT(HDiHitMatch*,Int_t*,Int_t,Char_t*);
        Int_t fillRingHTTheta(HDiHitMatch*,Int_t*,Int_t,Char_t*);
        Int_t fillRingHTThetaSec(HDiHitMatch*,Int_t*,Int_t,Char_t*,
				    Char_t*,Int_t,Int_t);
        Int_t fillRingPM(HDiHitMatch*,Int_t*,Int_t,Char_t*);
        Int_t fillRingPMTheta(HDiHitMatch*,Int_t*,Int_t,Char_t*);
        Int_t fillRingPMThetaSec(HDiHitMatch*,Int_t*,Int_t,Char_t*,
				    Char_t*,Int_t,Int_t);
        Int_t fillOpangMDC(HDiHitMatch*,Int_t*,Int_t,const char*);
        Int_t fillOpangKICK(HDiHitMatch*,Int_t*,Int_t,const char*);
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
	HCategory* pHitMatchCat; //!
        HIterator* pIterMatchHit; //!
	HCategory* pHitDiMatchCat; //!
        HIterator* pIterDiMatchHit; //!
        TObjArray* pHistArray;//!
        //FILENAME
        char *pFileName;//!
        //FILE
        TFile *pFileOut;//!
};

#endif // HRICHPAIRCREATOR_H

