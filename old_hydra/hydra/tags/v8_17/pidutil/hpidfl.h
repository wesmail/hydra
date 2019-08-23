//Pid utilities collection - by Thomas Eberl, Marcin Jaskula, et. al.
//Redesign to conform with new tracking scheme by Tassilo Christ, Nov 2003

#ifndef HPIDFL_H
#define HPIDFL_H
#include <TObject.h>
#include <TFile.h>
#include <TObjArray.h>
#include "haddef.h"
#include "hpidgeanttrackset.h"
class HPidParticle;
class HGeantKine;
class HKickTrack;
class HKickTrackB;
class HSplineTrack;
class HKickTrackSim;
class HLinearCategory;
class HMatrixCategory;
class HIterator;
class HGeomVector;
class HHitMatchSim;
class HHitMatch;
class HRichHitSim;
class HRichHitIPUSim;
class HRichHitIPU;
class HRichHit;
class HTofHit;
class HTofHitSim;
class HTofCluster;
class HTofClusterSim;
class HMdcSeg;
class HMdcSegSim;
class HMdcTrkCand;
class HParSet;
class HCategory;
class HShowerHitTof;
class HShowerHitTofTrack;
class HPidTrackCand;
class HPidTrackCandSim;
class HPidCandidate;
class HMetaMatch;
class HRKTrackB;
class TString;

class HPidFL : public TObject {
public:
  
  static HPidFL* Instance();
  ~HPidFL();
  
  //Getter functions for different hit/track objects
  static	TObject*      getHitObject(Int_t Index, Cat_t category, Bool_t bWarning = kFALSE);
  static	HCategory*    getCategory(Cat_t, Bool_t bWarning=kFALSE);
  
  static  Float_t       getMdcPhi(Int_t iSector, Float_t fPhiMdc);
  static  Float_t       getMdcTheta(Float_t fThetaMdc);
  
  static HMdcSeg*         getMdcSeg(Int_t iId, HCategory *pCat = NULL);
  static HMdcSegSim*      getMdcSegSim(Int_t iId, HCategory *pCat = NULL);

  static HRichHitIPU*     getRichHitIPU(Int_t iId, HCategory *pCat = NULL);
  static HRichHitIPUSim*  getRichHitIPUSim(Int_t iId, HCategory *pCat = NULL);
  
  //static HMdcSeg*         getMdcSeg(Int_t iSector, Int_t iId, HCategory *pCat = NULL); 
  //static HMdcSegSim*      getMdcSegSim(Int_t iSector, Int_t iId, HCategory *pCat = NULL);
  
  static HKickTrack*      getKickTrack(Int_t iId, HCategory *pCat = NULL);
  static HKickTrackB*      getKickTrackB(Int_t iId, HCategory *pCat = NULL);
  //static HKickTrackSim*   getKickTrackSim(Int_t iId, HCategory *pCat = NULL);
  
  static HSplineTrack*    getSplineTrack(Int_t iId, HCategory *pCat = NULL);
  static HRKTrackB*     getRKTrack(Int_t iId, HCategory *pCat = NULL);
  static HMdcTrkCand*     getMdcTrkCand(Int_t iId, HCategory *pCat = NULL);
  
  static HRichHit*        getRichHit(Int_t iId, HCategory *pCat = NULL);
  static HRichHitSim*     getRichHitSim(Int_t iId, HCategory *pCat = NULL);
  
  static HTofHit*         getTofHit(Int_t iId, HCategory *pCat = NULL);
  static HTofCluster*     getTofCluster(Int_t iId, HCategory *pCat = NULL);
  static HTofClusterSim*  getTofClusterSim(Int_t iId, HCategory *pCat = NULL);
  static HTofHitSim*      getTofHitSim(Int_t iId, HCategory *pCat = NULL);

  static HShowerHitTof*   getShowerHitTof(Int_t iId, HCategory *pCat = NULL);
  static HShowerHitTofTrack*  getShowerHitTofTrack(Int_t iId, HCategory *pCat = NULL);
  /*
  static Int_t appendIndex(Int_t index, Int_t* listOfIndices, Int_t  lengthOfList);
  static Int_t enlargeArray(Int_t* array, Int_t Length);
  static Int_t enlargeArray(UInt_t* Uarray, Int_t Length);
  */
  static HPidTrackCand*   getTrackCand(Int_t trackCandIndex, HCategory *pCat=NULL);
  static HPidCandidate*   getPidCandidate(Int_t candIndex, HCategory *pCat=NULL);
  static HPidParticle*    getPidParticle(Int_t iIdx, HCategory *pCat);
  static UInt_t           countLeptonsInInnerMdc(HPidGeantTrackSet* pTrackSet,Bool_t verbose=kFALSE);
  static Bool_t           getGeantMatches(HPidTrackCandSim* giveCandidate,Int_t& nMatchesMdcMeta, Int_t& nMatchesFull,
					  Int_t& nLeptonicMatchesMdcMeta, Int_t& GeantPid);
  //static Float_t          getKineMomentum(HPidTrackCandSim* pTrackCandSim, UInt_t detectors=kTrackKick);
  static Bool_t           isDoubleLepOnMirror(HRichHitSim* r,Int_t &nTrkNbOnMirror1, Int_t &nTrkNbOnMirror2);
  static Bool_t           isLepOnMirror(HRichHitSim* r,Int_t &nTrkNbOnMirror) ;
  //static Bool_t           hasSuccessfulTracking(HMetaMatch* pMetaMatch);
  static Int_t            matchRingMirrTracks(HRichHitSim *r,Int_t trackMirr,Int_t idMirr);
  static HParSet*         getParamContainer(Char_t * contname);
  static HGeantKine*      getGeantKineObjFromTrkNbFast(Int_t nTrkNb);
  static Int_t            getGeantParID(Int_t nTrkNb);
  
  static Int_t pickBestRing(HMetaMatch* pMatch, Float_t fMdcPhi, Float_t fMdcTheta);

  // -------------------------------------------------------------------------

private:
  HPidFL();
  ClassDef(HPidFL,2)
};

#endif //HPIDFL_H




