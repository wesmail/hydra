// @(#)$Id: hpidparticlesimexthit.h,v 1.1 2003-03-04 17:54:12 jaskula Exp $
//*-- Author : Marcin Jaskula 03/03/2003

#ifndef HPidParticleSimExtHit_H
#define HPidParticleSimExtHit_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleSimExtHit                                                      //
//                                                                            //
// Identified particle for SIM data with Geant track ids                      //
// and data from hit level.                                                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticlesimext.h"
#include "hpidhitdata.h"

// -----------------------------------------------------------------------------

class HPidParticleSimExtHit: public HPidParticleSimExt
{
public:

                    HPidParticleSimExtHit(Double_t dM0 = 0.0,
                                    Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);
                    HPidParticleSimExtHit(Short_t nPid, Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);
                    HPidParticleSimExtHit(const Char_t *sPid,
                                    Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);

                   ~HPidParticleSimExtHit(void) {}

    // -------------------------------------------------------------------------

    virtual HPidHitData*    getHPidHitData(void)    { return &cHitData; }

    // -------------------------------------------------------------------------

    virtual void    Clear(Option_t *opt = "");
    virtual void    print(void) const;

    // -------------------------------------------------------------------------

protected:

    HPidHitData     cHitData;

    // -------------------------------------------------------------------------

    ClassDef(HPidParticleSimExtHit, 1)  // Identified particle for sim data with tracks and hit data
};

#endif /* HPidParticleSimExtHit_H */
