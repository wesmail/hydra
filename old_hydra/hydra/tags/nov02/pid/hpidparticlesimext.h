// @(#)$Id: hpidparticlesimext.h,v 1.1 2002-12-03 14:46:10 jaskula Exp $
//*-- Author : Marcin Jaskula 01/12/2002

#ifndef HPidParticleSimExt_H
#define HPidParticleSimExt_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleSimExt                                                         //
//                                                                            //
// Identified particle for SIM data with Geant track ids                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticlesim.h"
#include "hpidgeanttrackset.h"

// -----------------------------------------------------------------------------

class HPidParticleSimExt: public HPidParticleSim
{
public:
                    HPidParticleSimExt(Double_t dM0 = 0.0, Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);
                    HPidParticleSimExt(Short_t nPid, Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);
                    HPidParticleSimExt(const Char_t *sPid, Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);

                   ~HPidParticleSimExt(void) {}

    // -------------------------------------------------------------------------

    virtual void  Clear(Option_t *opt = "");
    virtual void  print(void) const;

    // -------------------------------------------------------------------------

    virtual const HPidGeantTrackSet* getGeantTrackSet(void) const
                                                    { return &cGeantTracks; }

    virtual void  setGeantTrackSet(const HPidGeantTrackSet *pSet);

    // -------------------------------------------------------------------------

    static HCategory* buildPidParticleSimExtCategory(void);

protected:

    // -------------------------------------------------------------------------

    HPidGeantTrackSet  cGeantTracks;

    // -------------------------------------------------------------------------

    ClassDef(HPidParticleSimExt, 1)   // Identified particle for sim data with tracks
};

#endif /* HPidParticleSimExt_H */
