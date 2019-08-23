// @(#)$Id: hpiddilepton.h,v 1.1 2002-12-03 14:50:09 jaskula Exp $
//*-- Author : Marcin Jaskula 01/12/2002

#ifndef HPIDDILEPTON_H
#define HPIDDILEPTON_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidDileption                                                              //
//                                                                            //
// Dilepton parameters                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <TLorentzVector.h>
#include <TMath.h>

// -----------------------------------------------------------------------------

class HPidParticle;
class HCategory;

// -----------------------------------------------------------------------------

class HPidDilepton: public TLorentzVector
{
public:

                    HPidDilepton(Short_t nId1 = -1, Short_t nId2 = -1);
                    HPidDilepton(const HPidParticle *pPart1,
                                 const HPidParticle *pPart2);

    // -------------------------------------------------------------------------

    Bool_t          setParticles(Short_t nId1, Short_t nId2);
    Bool_t          setParticles(const HPidParticle *pPart1,
                                 const HPidParticle *pPart2);

    // -------------------------------------------------------------------------

    Short_t             getParticleId1(void) const  { return nParticleId1;     }
    const HPidParticle* getParticle1(void) const;

    Short_t             getParticleId2(void) const  { return nParticleId2;     }
    const HPidParticle* getParticle2(void) const;

    // -------------------------------------------------------------------------

    Char_t          getCharge(void) const           { return cCharge;          }

    // -------------------------------------------------------------------------

    Float_t         getOpeningAngleDeg(void) const  { return fAngle;           }
    Float_t         getOpeningAngleRad(void) const
                                        { return TMath::DegToRad() * fAngle;   }

    Double_t        thetaDeg(void) const;
    Double_t        phiDeg(void) const;

    // -------------------------------------------------------------------------

    virtual void    Clear(Option_t *opt = "")       { setDefault();            }
    virtual void    Print(Option_t *opt = "") const { print();                 }
    virtual void    print(void) const;

    // -------------------------------------------------------------------------

    Bool_t          isOk(void) const
                        { return ((nParticleId1 >= 0) && (nParticleId2 >= 0)); }

    // -------------------------------------------------------------------------

    static HCategory*    buildPidDileptonCategory(void);
    static HPidDilepton* getPidDilepton(Int_t iIdx, HCategory *pCat = NULL);

protected:

    void            setDefault(void);
    void            calculate(const HPidParticle *pPart1,
                              const HPidParticle *pPart2);

    // -------------------------------------------------------------------------

    Short_t         nParticleId1;
    Short_t         nParticleId2;

    Char_t          cCharge;
    Float_t         fAngle;

    // -------------------------------------------------------------------------

    ClassDef(HPidDilepton, 1)  // Parameters of a dilepton
};

// -----------------------------------------------------------------------------

inline Double_t HPidDilepton::thetaDeg(void) const
{
    return TMath::RadToDeg() * Theta();
}

// -----------------------------------------------------------------------------

inline Double_t HPidDilepton::phiDeg(void) const
{
Double_t dPhi;

    if((dPhi = TMath::RadToDeg() * Phi()) < 0.0)
        dPhi += 360.0;

    return dPhi;
}

// -----------------------------------------------------------------------------

#endif /* HPIDDILEPTON_H */
