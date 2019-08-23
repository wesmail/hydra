// @(#)$Id: hpidparticlesim.h,v 1.1 2002-11-20 17:22:43 jaskula Exp $
//*-- Author : Marcin Jaskula 11/11/2002

#ifndef HPIDPARTICLESIM_H
#define HPIDPARTICLESIM_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleSim                                                            //
//                                                                            //
// Identified particle for SIM data                                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticle.h"

// -----------------------------------------------------------------------------

class HGeantKine;

// -----------------------------------------------------------------------------

class HPidParticleSim: public HPidParticle
{
public:
                    HPidParticleSim(Double_t dM0 = 0.0, Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);
                    HPidParticleSim(Short_t nPid, Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);
                    HPidParticleSim(const Char_t *sPid, Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);

                   ~HPidParticleSim(void) {}

    // -------------------------------------------------------------------------

    virtual void    Clear(Option_t *opt = "");
    virtual void    print(void) const;

    // -------------------------------------------------------------------------

    void            setGeantId(Short_t n)       { nGeantId = n;              }
    Short_t         getGeantId(void) const      { return nGeantId;           }

    void            setGeantTrack(Short_t n)    { nGeantTrack = n;           }
    Short_t         getGeantTrack(void) const   { return nGeantTrack;        }

    void            setGeantParams(Short_t nTrack, Short_t nId)
                                     { nGeantId = nId; nGeantTrack = nTrack; }

    // -------------------------------------------------------------------------

    HGeantKine*         getGeantKine(HCategory *pCat = NULL) const;
    static HGeantKine*  getGeantKine(Short_t nTrack, HCategory *pCat = NULL);

    // -------------------------------------------------------------------------

    static HCategory*   buildPidParticleSimCategory(void);

protected:

    void            setDefault(void);

    // -------------------------------------------------------------------------

    Short_t         nGeantId;    // Geant pid
    Short_t         nGeantTrack; // Geant track number

    // -------------------------------------------------------------------------

    ClassDef(HPidParticleSim, 1)   // Identified particle for sim data
};

#endif /* HPIDPARTICLESIM_H */
