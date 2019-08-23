// @(#)$Id: hpidparticle.h,v 1.10 2003-11-21 14:43:51 halo Exp $
//*-- Author : Marcin Jaskula 30/11/2002
//  Modified : Marcin Jaskula 11/11/2002
//             fExpMass added
//  Modified : Marcin Jaskula 01/12/2002
//             new methods getIndex(), getPidParticle()
//  Modified : Marcin Jaskula 25/02/2003
//             new methods getTrackCand() and getKickTrack()
//  Modified : Marcin Jaskula ???
//             fPull, fTestVal, fMomRescal variables added
//  Modified : Marcin Jaskula 24/06/2003
//             *MultiUsed flags and interface added

#ifndef HPIDPARTICLE_H
#define HPIDPARTICLE_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticle                                                               //
//                                                                            //
// Identified particle                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <TLorentzVector.h>
#include <TMath.h>
#include "hpidphysicsconstants.h"

// -----------------------------------------------------------------------------

class HPidCandidate;
class HCategory;
class HPidTrackCand;
class HKickTrack;
class HPidParticleSim;
class HPidHitData;

// -----------------------------------------------------------------------------

class HPidParticle: public TLorentzVector
{
public:
                    HPidParticle(Double_t dM0 = 0.0, Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);
                    HPidParticle(Short_t nPid, Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);
                    HPidParticle(const Char_t *sPid, Double_t dPx = 0.0,
                                    Double_t dPy = 0.0, Double_t dPz = 0.0);

                   ~HPidParticle(void) {}

    // -------------------------------------------------------------------------

    enum {
        kMassExpInTLV = -1,
        kMassExpNoPhy = -2
    };

    // -------------------------------------------------------------------------

    void            setPidCandidateId(Short_t n)    { nPidCandidate = n;     }
    Short_t         getPidCandidateId(void) const   { return nPidCandidate ; }
    HPidCandidate*  getPidCandidate(HCategory *pCat = NULL) const;

    void            setBetaExp(Float_t f)           { fBetaExp = f;          }
    Float_t         getBetaExp(void) const          { return fBetaExp;       }

    void            setMassExp(Float_t f)           { fMassExp = f;          }
    Float_t         getMassExp(void) const;
    Float_t         getMassIdeal(void) const;
    Bool_t          getTLVUseExpMass(void) const
                              { return (fMassExp == Float_t(kMassExpInTLV)); }

    void            setR(Float_t f)                 { fR = f;                }
    Float_t         getR(void) const                { return fR;             }
    void            setZ(Float_t f)                 { fZ = f;                }
    Float_t         getZ(void) const                { return fZ;             }
    void            setPull(Float_t f)              { fPull = f;             }
    Float_t         getPull(void) const             { return fPull;          }
    void            setSystem(Short_t n)            { nSystem = n;           }
    Short_t         getSystem(void) const           { return nSystem;        }
    void            setExpParams(Short_t nSystem, Float_t fBeta,
                                    Float_t fR, Float_t fZ, Float_t fPull);

    Int_t           charge(void) const
                                { return HPidPhysicsConstants::charge(nPid); }
    Int_t           getCharge(void) const
                                { return HPidPhysicsConstants::charge(nPid); }

    void            setPid(Short_t n)               { nPid = n;              }
    Short_t         getPid(void) const              { return nPid;           }
    void            setTestVal(Float_t f)           { fTestVal = f;          }
    Float_t         getTestVal(void) const          { return fTestVal;       }

    void            setWeight(Float_t f)            { fWeight = f;           }
    Float_t         getWeight(void) const           { return fWeight;        }

    inline void     setPidParams(Short_t nPid, Float_t fTestVal);

    // -------------------------------------------------------------------------

    void            setMomRescal(Float_t f)         { fMomRescal = f;        }
    Float_t         getMomRescal(void) const        { return fMomRescal;     }

    TLorentzVector  getRescaledVector(void) const;

    // -------------------------------------------------------------------------

    virtual void    Clear(Option_t *opt = "");
    virtual void    Print(Option_t* option = "") const     { print();      }
    virtual void    print(void) const;

    // -------------------------------------------------------------------------

    inline Double_t thetaDeg(void) const;
    inline Double_t phiDeg(void) const;
    inline Short_t  sector(void) const;

    // -------------------------------------------------------------------------

    Int_t           getIndex(HCategory *pCat = NULL);

    // -------------------------------------------------------------------------

    static HCategory*    buildPidParticleCategory(void);
    static HPidParticle* getPidParticle(Int_t iIdx, HCategory *pCat = NULL);

    // -------------------------------------------------------------------------

    enum
    {
        kTrackCandOffset = -1000
    };

    HPidTrackCand*  getTrackCand(void) const;
    HKickTrack*     getKickTrack(void) const;

    // -------------------------------------------------------------------------

    virtual HPidHitData*    getHPidHitData(void)    { return NULL;  }

    // -------------------------------------------------------------------------

    enum
    {
        kRichMultiUsed      = (1 << 14),
        kMdcMultiUsed       = (1 << 15),
        kKickTrackMultiUsed = (1 << 16),
        kMetaMultiUsed      = (1 << 17)
    };

    Bool_t      isRichMultiUsed(void) const
                                    { return TestBit(kRichMultiUsed);       }
    void        setRichMultiUsed(Bool_t b = kTRUE)
                                    { SetBit(kRichMultiUsed, b);            }

    Bool_t      isMdcMultiUsed(void) const
                                    { return TestBit(kMdcMultiUsed);        }
    void        setMdcMultiUsed(Bool_t b = kTRUE)
                                    { SetBit(kMdcMultiUsed, b);             }

    Bool_t      isKickTrackMultiUsed(void) const
                                    { return TestBit(kKickTrackMultiUsed);  }
    void        setKickTrackMultiUsed(Bool_t b = kTRUE)
                                    { SetBit(kKickTrackMultiUsed, b);       }

    Bool_t      isMetaMultiUsed(void) const
                                    { return TestBit(kMetaMultiUsed);       }
    void        setMetaMultiUsed(Bool_t b = kTRUE)
                                    { SetBit(kMetaMultiUsed, b);            }

    void        fillMultiUsed(const HPidTrackCand *pCand);

    // -------------------------------------------------------------------------

protected:

    void            setDefault(void);

    // -------------------------------------------------------------------------

    Short_t     nPidCandidate;  // index of the HPidCandidate

    Float_t     fBetaExp;       // speed calculated from experimental data
    Float_t     fMassExp;       // experimental mass; use getMass*() methods !
    Float_t     fR;             // rho component of the 'vertex' coordinates
    Float_t     fZ;             // z component of the 'vertex' coordinates
    Float_t     fPull;          // pull component from HKickTrack
    Short_t     nSystem;        // system (0 - TOFino, 1 - TOF)

    Short_t     nPid;           // identified PID
    Float_t     fTestVal;       // test value checked during creation

    Float_t     fWeight;        // weight of the particle

    Float_t     fMomRescal;     // rescaling factor for the momentum

    // -------------------------------------------------------------------------

    ClassDef(HPidParticle, 4)   // Identified particle
};

// -----------------------------------------------------------------------------

inline void HPidParticle::setExpParams(Short_t nSystem, Float_t fBeta,
                    Float_t fR, Float_t fZ, Float_t fPull)
{
    fBetaExp      = fBeta;
    this->fR      = fR,
    this->fZ      = fZ;
    this->fPull   = fPull;
    this->nSystem = nSystem;
}

// -----------------------------------------------------------------------------

inline void HPidParticle::setPidParams(Short_t nP, Float_t fTV)
{
    setPid(nP);
    setTestVal(fTV);
}

// -----------------------------------------------------------------------------

inline Double_t HPidParticle::thetaDeg(void) const
{
    return TMath::RadToDeg() * Theta();
}

// -----------------------------------------------------------------------------

inline Double_t HPidParticle::phiDeg(void) const
{
Double_t dPhi;

    if((dPhi = TMath::RadToDeg() * Phi()) < 0.0)
        dPhi += 360.0;

    return dPhi;
}

// -----------------------------------------------------------------------------

inline Short_t HPidParticle::sector(void) const
{
Short_t n = (Short_t)(phiDeg() / 60.0);

    return (n == 0) ? 5 : (n - 1);
}

// -----------------------------------------------------------------------------

#endif /* HPIDPARTICLE_H */
