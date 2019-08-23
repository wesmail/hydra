// $Id: hpairsimext.h,v 1.1 2006-08-16 15:39:39 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-06-30 16:45:08
//
//
// ---------------------------------------------------------------------------
#ifndef HPAIRSIMEXT_H
#define HPAIRSIMEXT_H
// ---------------------------------------------------------------------------
#include "hpairsim.h"
#include <hpidparticlesim.h>
// ---------------------------------------------------------------------------
//class HPidParticleSim;
// ---------------------------------------------------------------------------

class HPairSimExt: public HPairSim {


public:

    HPairSimExt();
    HPairSimExt(HPidParticleSim*, HPidParticleSim*);
    virtual ~HPairSimExt();

    HPairSimExt(const HPairSimExt& source);
    virtual void print(void);

     HPidParticleSim*    getPidParticle1(void)  { return &itsParticle1;}
     HPidParticleSim*    getPidParticle2(void)  { return &itsParticle2;}

     virtual Char_t getCharge(void) const;
// ---------------------------------------------------------------------------
    virtual void    Clear(Option_t *opt = "");

protected:

    virtual void reset(void);
    //
    HPidParticleSim itsParticle1;
    HPidParticleSim itsParticle2;

// ---------------------------------------------------------------------------
    ClassDef(HPairSimExt,0)

};
#endif // HPAIRSIMEXT_H
