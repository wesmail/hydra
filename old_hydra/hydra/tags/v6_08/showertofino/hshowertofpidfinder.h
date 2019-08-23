#ifndef HShowerTofPIDFinder_H
#define HShowerTofPIDFinder_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hratree.h"
#include "hshowerhittof.h"
#include "hkicktrack.h"
#include "hshowertofpid.h"

class HCategory;
class HIterator;
class HShowerTofPID;
class HShowerHitTof;
class HKickTrack;

class HShowerTofPIDFinder : public HReconstructor {
public:
        HShowerTofPIDFinder();
        HShowerTofPIDFinder(Text_t *name,Text_t *title);
        virtual ~HShowerTofPIDFinder();

        Int_t execute(void);
        Bool_t init(void);
        Bool_t finalize(void);
        Bool_t initParameters(void);

        HShowerTofPIDFinder &operator=(HShowerTofPIDFinder &c);

        HCategory* getKickTrackCat(){return m_pKickTrackCat;}
        HCategory* getHitTofCat(){return m_pHitTofCat;}
        HCategory* getPIDCat(){return m_pPIDCat;}
        HParSet* getHitFPar(){return m_pHitFPar;}
        HParSet* getGeometry(){return m_pGeometry;}
        HParSet* getTofinoCalPar(){return m_pTofinoCalPar;}
		
        void setKickTrackCat(HCategory* pKickTrackCat){m_pKickTrackCat = pKickTrackCat;}
        void setHitTofCat(HCategory* pHitTofCat){m_pHitTofCat = pHitTofCat;}
        void setPIDCat(HCategory* pPIDCat){m_pPIDCat = pPIDCat;}
        void setHitFPar(HParSet* pPar){m_pHitFPar = pPar;}
        void setGeometry(HParSet* pGeometry){m_pGeometry = pGeometry;}
        void setTofinoCalPar(HParSet* pTofinoCalPar){m_pTofinoCalPar = pTofinoCalPar;}

        Bool_t IsSortFlagSet(){return m_bIsSort;}
        Bool_t IsFillPID(){return m_bIsFillPID;}
	    //Bool_t IsLepInShower(HKickTrack *pKick,HShowerHitTof *pShower); 
	    Bool_t IsLepton(HKickTrack *pKick); 
        Bool_t IsShower(HKickTrack *pKick);
        Bool_t IsTofInWindow(HShowerHitTof *pShower);
        Bool_t IsTofInWindow(Float_t time);
        void setSortFlag(Bool_t bSort = kTRUE){m_bIsSort = bSort;}
        void setFillPID(Bool_t bIsFillPID = kTRUE){m_bIsFillPID = bIsFillPID;}
        void fillPID(HKickTrack *pKick , HShowerTofPID* pid);

        Float_t getShowFactor(Float_t *pPar,Float_t fMom,Float_t fParam);
        Float_t calcFromFit(Float_t *pParTab,Float_t fMom);


private:
        HLocation m_Loc;

        HCategory *m_pKickTrackCat; //!Pointer to the kicktrack data category
        HCategory *m_pHitTofCat; //!Pointer to the hittof data category
        HCategory *m_pPIDCat; //!Pointer to the tofpid data category

        HParSet *m_pHitFPar; //Pointer to the analysis parameters container
        HParSet *m_pGeometry; //!Pointer to geometry parameters container
		HParSet *m_pTofinoCalPar; //!Pointer to the tofino calibration parameters 

        HIterator *fKickTrackIter; //!Iterator for kick plane 
        Bool_t  m_bIsFillPID;//flag for filling PID level - default TRUE
        Bool_t  m_bIsSort;//flag for sorting data by m_nAddress - default FALSE

        ClassDef(HShowerTofPIDFinder,0) //ROOT extension
};

#endif
