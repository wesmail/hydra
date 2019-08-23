// $Id: hpairext.h,v 1.2 2006-08-16 15:42:18 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-06-30 16:45:13
//
//
// ---------------------------------------------------------------------------
#ifndef HPAIREXT_H
#define HPAIREXT_H
// ---------------------------------------------------------------------------
#include "hpair.h"
// ---------------------------------------------------------------------------
#include <hpidparticle.h>
// ---------------------------------------------------------------------------

class HPairExt: public HPair {


public:

    HPairExt();
    HPairExt(HPidParticle*, HPidParticle*);
    virtual ~HPairExt();

    HPairExt(const HPairExt& source);
    virtual void print(void);

     HPidParticle*   getPidParticle1(void)  { return &itsParticle1;}
     HPidParticle*   getPidParticle2(void)  { return &itsParticle2;}

     virtual Char_t getCharge(void) const;
// ---------------------------------------------------------------------------
    virtual void    Clear(Option_t *opt = "");

protected:

    virtual void reset(void);
    //
    HPidParticle itsParticle1;
    HPidParticle itsParticle2;

// ---------------------------------------------------------------------------
    ClassDef(HPairExt,1)

};
#endif // HPAIREXT_H
