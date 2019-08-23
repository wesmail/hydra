// @(#)$Id: hpidparticlesimext.h,v 1.3 2003-11-21 14:54:16 halo Exp $
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

    virtual void        Clear(Option_t *opt = "");
    virtual void        print(void) const;

    // -------------------------------------------------------------------------

    virtual const HPidGeantTrackSet* getGeantTrackSet(void) const;
    virtual void        setGeantTrackSet(const HPidGeantTrackSet *pSet);

    // -------------------------------------------------------------------------

    static HCategory*   buildPidParticleSimExtCategory(void);

    // -------------------------------------------------------------------------

    virtual Int_t       getMaxCommonTracks(void) const { return 5; }
    inline virtual const HPidCommonTrack*  getTrack(UInt_t iPos = 0) const;

protected:

    // -------------------------------------------------------------------------

    HPidGeantTrackSet   cTrackSet;

    // -------------------------------------------------------------------------

    // Data for the next "common tracks"
    HPidCommonTrack     cTrack1;
    HPidCommonTrack     cTrack2;
    HPidCommonTrack     cTrack3;
    HPidCommonTrack     cTrack4;

    // -------------------------------------------------------------------------

    void                setDefault(void);

    // -------------------------------------------------------------------------

    ClassDef(HPidParticleSimExt, 2)  // Identified particle for sim data with tracks
};

// -----------------------------------------------------------------------------

inline const HPidCommonTrack* HPidParticleSimExt::getTrack(UInt_t iPos) const
{
    switch(iPos)
    {
        case 0: return &cTrack0;
        case 1: return &cTrack1;
        case 2: return &cTrack2;
        case 3: return &cTrack3;
        case 4: return &cTrack4;
    }

    Error("getTrack", "iPos=%d out of bounds", iPos);

    return NULL;
}

#endif /* HPidParticleSimExt_H */
