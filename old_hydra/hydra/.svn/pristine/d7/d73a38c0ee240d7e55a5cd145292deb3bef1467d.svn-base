// @(#)$Id: hpidparticle.h,v 1.6 2002-12-03 14:38:37 jaskula Exp $
//*-- Author : Marcin Jaskula 30/11/2002
//  Modified : Marcin Jaskula 11/11/2002
//             fExpMass added
//  Modified : Marcin Jaskula 01/12/2002
//             new methods getIndex(), getPidParticle()

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
    void            setSystem(Short_t n)            { nSystem = n;           }
    Short_t         getSystem(void) const           { return nSystem;        }
    void            setExpParams(Short_t nSystem, Float_t fBeta,
                                    Float_t fR, Float_t fZ);

    Int_t           charge(void) const
                                { return HPidPhysicsConstants::charge(nPid); }
    Int_t           getCharge(void) const
                                { return HPidPhysicsConstants::charge(nPid); }

    void            setPid(Short_t n)               { nPid = n;              }
    Short_t         getPid(void) const              { return nPid;           }
    void            setPdf(Float_t f)               { fPdf = f;              }
    Float_t         getPdf(void) const              { return fPdf;           }
    void            setBayes(Float_t f)             { fBayes = f;            }
    Float_t         getBayes(void) const            { return fBayes;         }
    void            setCL(Float_t f)                { fCL = f;               }
    Float_t         getCL(void) const               { return fCL;            }

    void            setPidParams(Float_t fPdf, Float_t fBayes, Float_t fCL);
    void            setPidParams(Short_t nPid, Float_t fPdf,
                                    Float_t fBayes, Float_t fCL);

    // -------------------------------------------------------------------------

    virtual void    Clear(Option_t *opt = "");
    virtual void    Print(Option_t *opt = "") const { print();               }
    virtual void    print(void) const;

    // -------------------------------------------------------------------------

    Double_t        thetaDeg(void) const;
    Double_t        phiDeg(void) const;
    Short_t         sector(void) const;

    // -------------------------------------------------------------------------

    Int_t           getIndex(HCategory *pCat = NULL);

    // -------------------------------------------------------------------------

    static HCategory*    buildPidParticleCategory(void);
    static HPidParticle* getPidParticle(Int_t iIdx, HCategory *pCat = NULL);

protected:

    void            setDefault(void);

    // -------------------------------------------------------------------------

    Short_t     nPidCandidate;  // index of the HPidCandidate

    Float_t     fBetaExp;       // speed calculated from experimental data
    Float_t     fMassExp;       // experimental mass; use getMass*() methods !
    Float_t     fR;             // rho component of the 'vertex' coordinates
    Float_t     fZ;             // z component of the 'vertex' coordinates
    Short_t     nSystem;        // system (0 - TOFino, 1 - TOF)

    Short_t     nPid;           // identified PID
    Float_t     fPdf;           // merged pdf value
    Float_t     fBayes;         // pdf with resp. Bayes merging output
    Float_t     fCL;            // final confidencial level ???

    // -------------------------------------------------------------------------

    ClassDef(HPidParticle, 2)   // Identified particle
};

// -----------------------------------------------------------------------------

inline void HPidParticle::setExpParams(Short_t nSystem, Float_t fBeta,
                    Float_t fR, Float_t fZ)
{
    fBetaExp      = fBeta;
    this->fR      = fR,
    this->fZ      = fZ;
    this->nSystem = nSystem;
}

// -----------------------------------------------------------------------------

inline void HPidParticle::setPidParams(Float_t fPdf,
                    Float_t fBayes, Float_t fCL)
{
    this->fPdf   = fPdf;
    this->fBayes = fBayes;
    this->fCL    = fCL;
}

// -----------------------------------------------------------------------------

inline void HPidParticle::setPidParams(Short_t nPid, Float_t fPdf,
                    Float_t fBayes, Float_t fCL)
{
    setPid(nPid);
    setPidParams(fPdf, fBayes, fCL);
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
