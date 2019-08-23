// @(#)$Id: hpidparticlehit.h,v 1.1 2003-03-04 17:53:42 jaskula Exp $
//*-- Author : Marcin Jaskula 03/03/2003

#ifndef HPidParticleHit_H
#define HPidParticleHit_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleHit                                                            //
//                                                                            //
// Identified particle with data from hit level.                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticlesimext.h"
#include "hpidhitdata.h"

// -----------------------------------------------------------------------------

class HPidParticleHit: public HPidParticle
{
public:

                    HPidParticleHit(Double_t dM0 = 0.0,
                                    Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);
                    HPidParticleHit(Short_t nPid, Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);
                    HPidParticleHit(const Char_t *sPid,
                                    Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);

                   ~HPidParticleHit(void) {}

    // -------------------------------------------------------------------------

    virtual void    Clear(Option_t *opt = "");
    virtual void    print(void) const;

    // -------------------------------------------------------------------------

    virtual HPidHitData*    getHPidHitData(void)    { return &cHitData; }

    // -------------------------------------------------------------------------

protected:

    HPidHitData     cHitData;

    // -------------------------------------------------------------------------

    ClassDef(HPidParticleHit, 1)  // Identified particle with data from hit level
};

#endif /* HPidParticleHit_H */
