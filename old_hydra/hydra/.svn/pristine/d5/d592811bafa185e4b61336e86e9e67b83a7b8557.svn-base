// $Id: hpair.h,v 1.7 2008-09-18 13:06:48 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-04-20 15:44:23
//
// ---------------------------------------------------------------------------
#ifndef HPAIR_H
#define HPAIR_H
// ---------------------------------------------------------------------------
#include "TLorentzVector.h"
// ---------------------------------------------------------------------------
class HPidParticle;
// ---------------------------------------------------------------------------

class HPair: public TLorentzVector {


public:

    HPair();
    HPair(HPidParticle*, HPidParticle*);
    virtual ~HPair();
    HPair(const HPair& source);
    virtual void print(void);
    virtual void reset(void);

    
    Int_t   getIndexParticle1(void)  const {return sIdxP1;       };
    Int_t   getIndexParticle2(void)  const {return sIdxP2;       };
    Int_t   getPid1(void)            const {return nPid1;        }; 
    Int_t   getPid2(void)            const {return nPid2;        }; 
    Float_t getPidProbability1(void) const {return fProbability1;};
    Float_t getPidProbability2(void) const {return fProbability2;};
    Float_t getOpeningAngleDeg(void) const {return fOpang;       };
    Float_t getInvariantMass(void)   const {return fInvMass;     };
    virtual Char_t  getCharge(void)          const;
    Char_t  getIsCutFlag(void)       const {return cIsCut;       };
    void    setIsCutFlag(Char_t c)         {cIsCut=c;            };
    void    setBitFieldPassedCuts(Int_t i)     {nBitFieldPassedCuts=i;};
    Int_t   getBitFieldPassedCuts(void) const  {return nBitFieldPassedCuts;};
// ---------------------------------------------------------------------------
	
protected:

    void setIndexParticle1(Short_t sIndex)  {sIdxP1=sIndex;};
    void setIndexParticle2(Short_t sIndex)  {sIdxP2=sIndex;};
    void setPid1(Int_t nPid)                {nPid1=nPid;};
    void setPid2(Int_t nPid)                {nPid2=nPid;};
    void setPidProbability1(Float_t fProb)  {fProbability1=fProb;};
    void setPidProbability2(Float_t fProb)  {fProbability2=fProb;};
    void setOpeningAngleDeg(Float_t fOp)    {fOpang=fOp;};
    void setInvariantMass(Float_t fM)       {fInvMass=fM;};
    //
    Bool_t calcPairProperties(HPidParticle*, HPidParticle*);
    //
    Short_t sIdxP1; 
    Short_t sIdxP2;
    Float_t fProbability1;
    Float_t fProbability2;
    Int_t   nPid1;
    Int_t   nPid2;
    //
    Float_t fOpang;
    Float_t fInvMass;
    Char_t cIsCut; // 0 - not cut, 1 - direct cut, 2 - recursive cut, 3 - both
    //
    Int_t nBitFieldPassedCuts; // individual for object
    // should cut flags be changed dynamically according to event topology ???
// ---------------------------------------------------------------------------
    ClassDef(HPair,1)
};
#endif // HPAIR_H
