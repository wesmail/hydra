#ifndef HShowerHitFinderOld_H
#define HShowerHitFinderOld_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hratree.h"

class HCategory;
class HShowerHitHeader;

class HShowerHitFinderOld : public HShowerHitFinder {
public:
	HShowerHitFinderOld();
        HShowerHitFinderOld(Text_t *name,Text_t *title);

        ClassDef(HShowerHitFinderOld,1) //ROOT extension

private:
        virtual void calcCoord(HShowerHit* hit, HLocation &fLoc);

};

#endif
