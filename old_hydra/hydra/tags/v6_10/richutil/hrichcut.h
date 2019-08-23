// File: hrichcut.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/05/10 14:30:23
//
#ifndef HRICHCUT_H
#define HRICHCUT_H
#include <TObject.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TCanvas.h>
class HHitMatch;
class HDiHitMatch;
class HDiHitMatchSim;
class HLinearCategory;
class HRichCut : public TObject {
public:

	HRichCut();
	~HRichCut();
    static Bool_t isFullRichMdcMetaTracklet(HHitMatch*);
    static Bool_t isTOFBetween(HHitMatch*,Float_t,Float_t);
    static Bool_t isInKickTrack(HDiHitMatch*);
    static Bool_t isRMThetaDiff(HHitMatch*,Float_t);
    static Bool_t isOpangGreaterThan(HDiHitMatch*, Float_t);
    static Bool_t isGEANTPair(HDiHitMatchSim*, HLinearCategory*,Char_t*);
    static Bool_t isNew2Tuple(Int_t,Int_t,Int_t*,Int_t);
    static Bool_t isNewIndex(Int_t,Int_t*,Int_t);
    static Bool_t isUnlikeSignPair(HDiHitMatch*);
    static Int_t  NbRingsPerMDCUNLIKEPair(HDiHitMatch*);
private:

    ClassDef(HRichCut,1)
};

#endif //HRICHCUT_H



