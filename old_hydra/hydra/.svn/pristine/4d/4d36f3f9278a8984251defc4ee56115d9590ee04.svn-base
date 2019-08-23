// @(#)$Id: hpidparticlesim.h,v 1.3 2003-03-04 17:52:33 jaskula Exp $
//*-- Author : Marcin Jaskula 11/11/2002
//  Modified : Marcin Jaskula 01/12/2002
//             new method getGeantTrackSet()
//  Modified : Marcin Jaskula 26/02/2003
//             nGeantId & nGeantTrack -> cCommonTrack structure

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
#include "hpidcommontrack.h"

// -----------------------------------------------------------------------------

class HGeantKine;
class HPidGeantTrackSet;

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

    virtual const HPidGeantTrackSet* getGeantTrackSet(void) const;
    virtual void  setGeantTrackSet(const HPidGeantTrackSet *) { }

    // -------------------------------------------------------------------------

    HGeantKine*         getGeantKine(HCategory *pCat = NULL) const;
    static HGeantKine*  getGeantKine(Short_t nTrack, HCategory *pCat = NULL);

    // -------------------------------------------------------------------------

    static HCategory*   buildPidParticleSimCategory(void);

    // -------------------------------------------------------------------------

    virtual const HPidCommonTrack*  getTrack(UInt_t iPos = 0) const;

    // -------------------------------------------------------------------------

    Bool_t              setTrack(const HPidCommonTrack *pSet, UInt_t iPos = 0);
    void                clearTracks(void);
    void                printTracks(Bool_t bAll = kFALSE) const;

    // -------------------------------------------------------------------------

    virtual Int_t       getMaxCommonTracks(void) const          { return 1; }

    Int_t               getValidCommonTracks(void) const;

    Char_t              getTrackDetectors(UInt_t iPos = 0) const;
    UInt_t              getTrackProcess(UInt_t iPos = 0) const;
    UInt_t              getTrackMask(UInt_t iPos = 0) const;

    Short_t             getGeantTrack(UInt_t iPos = 0) const;
    Short_t             getTrackGeant(UInt_t iPos = 0) const;

    Short_t             getGeantId(UInt_t iPos = 0) const;
    Short_t             getTrackPid(UInt_t iPos = 0) const;

    Double_t            getTrackMomX(UInt_t iPos = 0) const;
    Double_t            getTrackMomY(UInt_t iPos = 0) const;
    Double_t            getTrackMomZ(UInt_t iPos = 0) const;

    const TVector3*     getTrackMom(UInt_t iPos = 0) const;

protected:

    void                setDefault(void);
    HPidCommonTrack*    getTrackFullAccess(UInt_t iPos = 0);

    // -------------------------------------------------------------------------

    HPidCommonTrack cTrack0;    // the first "common track"

    // -------------------------------------------------------------------------

    ClassDef(HPidParticleSim, 2)   // Identified particle for sim data
};

// -----------------------------------------------------------------------------

inline const HPidCommonTrack* HPidParticleSim::getTrack(UInt_t iPos) const
{
    if(iPos != 0)
    {
        Error("getTrack", "iPos out of bounds");
        return NULL;
    }

    return &cTrack0;
}

// -----------------------------------------------------------------------------

inline HPidCommonTrack* HPidParticleSim::getTrackFullAccess(UInt_t iPos)
{
// Private function

    return (HPidCommonTrack *) getTrack(iPos);
}

// -----------------------------------------------------------------------------

inline Char_t HPidParticleSim::getTrackDetectors(UInt_t iPos) const
{
const HPidCommonTrack *p;

    return ((p = getTrack(iPos)) != NULL)
                ? p->getDetectors() : HPidCommonTrack::kTrackNotSet;
}

// -----------------------------------------------------------------------------

inline UInt_t HPidParticleSim::getTrackProcess(UInt_t iPos) const
{
const HPidCommonTrack *p;

    return ((p = getTrack(iPos)) != NULL) ? p->getProcess() : 0;
}

// -----------------------------------------------------------------------------

inline UInt_t HPidParticleSim::getTrackMask(UInt_t iPos) const
{
const HPidCommonTrack *p;

    return ((p = getTrack(iPos)) != NULL) ? p->getMask() : 0;
}

// -----------------------------------------------------------------------------

inline Short_t HPidParticleSim::getGeantTrack(UInt_t iPos) const
{
    return getTrackGeant(iPos);
}

// -----------------------------------------------------------------------------

inline Short_t HPidParticleSim::getTrackGeant(UInt_t iPos) const
{
const HPidCommonTrack *p;

    return ((p = getTrack(iPos)) != NULL) ? p->getTrackId() : 0;
}

// -----------------------------------------------------------------------------

inline Short_t HPidParticleSim::getGeantId(UInt_t iPos) const
{
    return getTrackPid(iPos);
}

// -----------------------------------------------------------------------------

inline Short_t HPidParticleSim::getTrackPid(UInt_t iPos) const
{
const HPidCommonTrack *p;

    return ((p = getTrack(iPos)) != NULL) ? p->getTrackId() : 0;
}

// -----------------------------------------------------------------------------

inline Double_t HPidParticleSim::getTrackMomX(UInt_t iPos) const
{
const TVector3 *v;

    return ((v = getTrackMom(iPos)) != NULL) ? v->X() : 0.0f;
}

// -----------------------------------------------------------------------------

inline Double_t HPidParticleSim::getTrackMomY(UInt_t iPos) const
{
const TVector3 *v;

    return ((v = getTrackMom(iPos)) != NULL) ? v->Y() : 0.0f;
}

// -----------------------------------------------------------------------------

inline Double_t HPidParticleSim::getTrackMomZ(UInt_t iPos) const
{
const TVector3 *v;

    return ((v = getTrackMom(iPos)) != NULL) ? v->Z() : 0.0f;
}

// -----------------------------------------------------------------------------

inline const TVector3* HPidParticleSim::getTrackMom(UInt_t iPos) const
{
const HPidCommonTrack *p;

    return ((p = getTrack(iPos)) != NULL) ? p->getMomentum() : NULL;
}

#endif /* HPIDPARTICLESIM_H */
