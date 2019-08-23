// File: hshowerhitfindersim.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 00/02/17 18:49:01
//
#ifndef HSHOWERHITFINDERSIM_H
#define HSHOWERHITFINDERSIM_H

#include "hshowerhitfinder.h"
#include "hshowerhitsim.h"
#include "hshowercalsim.h"

class HShowerHitFinderSim : public HShowerHitFinder{
public:
       HShowerHitFinderSim(void) : HShowerHitFinder() {};
       HShowerHitFinderSim(Text_t *name,Text_t *title) :
                                      HShowerHitFinder(name,title) {};
       ~HShowerHitFinderSim(){};
       Int_t execute(void);
       Bool_t init(void);
       Bool_t lookForHit(HShowerCalSim* cal, HLocation &fLoc);
       void fillSums(HShowerHitSim* hit, HLocation &fLoc);
       virtual void calcCoord(HShowerHitSim* hit, HLocation &fLoc);
       void fillPID(HShowerHitSim* hit, HShowerPID* pid); 

       ClassDef(HShowerHitFinderSim,1) // SHOWER hit finder for simulated data
};
#endif /* !HSHOWERHITFINDERSIM_H */
