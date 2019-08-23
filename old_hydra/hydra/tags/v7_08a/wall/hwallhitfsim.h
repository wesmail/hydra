#ifndef HWALLHITFSIM_H
#define HWALLHITFSIM_H

#include "hwallhitf.h"
#include "walldef.h"

class HWallHit;
class HWallRaw;
class HCategory;
class HIterator;

class HWallHitFSim : public HWallHitF {
  protected:
    HWallHit *makeHit(TObject *address);
    void fillHit(HWallHit *, HWallRaw *);
  public:
    HWallHitFSim(void);
    HWallHitFSim(Text_t* name,Text_t* title);
    ~HWallHitFSim(void);
    Bool_t init();
    ClassDef(HWallHitFSim,0) // Wall hit finder for simulated data.
};

#endif /* !HWALLHITFSIM_H */

