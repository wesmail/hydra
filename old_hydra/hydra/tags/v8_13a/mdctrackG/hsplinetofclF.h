#ifndef HSPLINETOFCLF_H
#define HSPLINETOFCLF_H
#include <iostream>
#include "hreconstructor.h"
class HCategory;
class HIterator;
class HRuntimeDb;
class HShowerGeometry;
class HEvent;
class HMetaMatch;
class HSplineTofClF : public HReconstructor {
private:
   Int_t ind1,ind2,ind3,ind4;
   Int_t mode;
   Int_t step,step1; 
   HMetaMatch *pMetaMatch1, *pMetaMatch2;
   HIterator *iterMetaMatch1, *iterMetaMatch2;
   HCategory *fCatMetaMatch;
   HCategory *fCatTrkCand;
   Bool_t cond(Int_t , Int_t , Int_t , Int_t );
   Bool_t checkForSeg(HMetaMatch *, HMetaMatch*);
   Bool_t condMeta(HMetaMatch*, HMetaMatch*);
   void checkCluster(HMetaMatch *pMetaMatch);
   Bool_t checkNextTofHits(HMetaMatch *);
   Bool_t checkCandForSameMeta(HMetaMatch *);
   void checkShowerOverlap();
public:
   HSplineTofClF(void);
   HSplineTofClF(Text_t name[], Text_t title[],Int_t mode=0);
   ~HSplineTofClF();
   Int_t execute();
   Bool_t init();
   Bool_t reinit();
   Bool_t finalize();
   Int_t unsetTofHitsFromCluster();
   
   
   ClassDef(HSplineTofClF,0)
      };
#endif

