// @(#)$Id: hpidparticlefillersim.h,v 1.4 2002-12-03 14:49:19 jaskula Exp $
//*-- Author : Marcin Jaskula 13/11/2002
//  Modified : Marcin Jaskula 01/12/2002
//             support for HPidParticleSimExt class

#ifndef HPIDPARTICLEFILLERSIM_H
#define HPIDPARTICLEFILLERSIM_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleFillerSim                                                      //
//                                                                            //
// HPidParticleFiller for simulation data (output: HPidParticleSim)           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidparticlefiller.h"

// -----------------------------------------------------------------------------

class HPidParticleFillerSim : public HPidParticleFiller
{
public:
                        HPidParticleFillerSim( Text_t name[], Text_t title[],
                                            Option_t par[] = "");

                        HPidParticleFillerSim(Option_t par[] = "");

                       ~HPidParticleFillerSim(void);

    // -------------------------------------------------------------------------

    virtual void        setParameters(Option_t par[]);

    // -------------------------------------------------------------------------

    virtual void        print(void) const;

    // -------------------------------------------------------------------------

    virtual Bool_t          buildOutputCategory(void);

    virtual HPidParticle*   getNextSlot(void);


    virtual HPidParticle*   makeParticle(const HPidCandidate *pCand,
                                    HKickTrack *pKickTrack, Int_t iIdx);

    // -------------------------------------------------------------------------

    void                setNoRich(Bool_t b)     { bNoRich = b;          }
    Bool_t              getNoRich(void) const   { return bNoRich;       }

protected:

    Bool_t              bNoRich;    // should Geant Tracks from RICH be used
    Bool_t              bExt;       // output category: HPidParticleSimExt

    // -------------------------------------------------------------------------

    void                setDefault(void);

    // -------------------------------------------------------------------------

    ClassDef(HPidParticleFillerSim, 0)     // Filler of HPidParticleSim
};

// -----------------------------------------------------------------------------

#endif //HPIDPARTICLEFILLERSIM_H
