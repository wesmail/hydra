#ifndef HShowerTofPIDTrackMatcher_H
#define HShowerTofPIDTrackMatcher_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hshowerraw.h"

class HCategory;
class HIterator;
class HShowerTofPID;
class HShowerTrack;

class HShowerTofPIDTrackMatcher : public HReconstructor {
public:
	HShowerTofPIDTrackMatcher();
    HShowerTofPIDTrackMatcher(Text_t *name,Text_t *title);
	~HShowerTofPIDTrackMatcher();

        Bool_t init(void);
        Bool_t finalize(void);
        Int_t execute(void);

        void addHitTrack(HShowerTofPID* pHit, HShowerTrack* pTrack);


        HCategory* getTrackCat() {return m_pTrackCat;}
        HCategory* getHitCat() {return m_pHitCat;}
        HCategory* getHitTrack() {return m_pHitTrackCat;}

        void setTrackCat(HCategory* pTrackCat) {m_pTrackCat = pTrackCat;}
        void setHitCat(HCategory* pHitCat) {m_pHitCat = pHitCat;}
        void setHitTrack(HCategory* pHitTrackCat) {m_pHitTrackCat = pHitTrackCat;}


        ClassDef(HShowerTofPIDTrackMatcher,1) // Correlates Tofino/Shower HITs with Geant tracks

private:
        HLocation m_zeroLoc;   //! location in the category

        HCategory *m_pTrackCat; //!Pointer to the track data category
        HCategory *m_pHitCat; //!Pointer to the hit data category
        HCategory *m_pHitTrackCat; //!Pointer to the hittrack data category

        HIterator *fHitIter;    //!Iterator for local maxima
        HIterator *fTrackIter;  //!Iterator for tracks
};

#endif
