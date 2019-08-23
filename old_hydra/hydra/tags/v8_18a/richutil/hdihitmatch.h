// $Id: hdihitmatch.h,v 1.4 2008-09-18 13:05:35 halo Exp $
// Last update by Thomas Eberl: 04/08/03 15:15:34
//
#ifndef HDIHITMATCH_H
#define HDIHITMATCH_H
#define MAXIND 5
#include "TObject.h"
#include "hcategory.h"
class HHitMatch;
class HDiHitMatch: public TObject {

protected:
    void reset();
    Int_t nbDRichHit;//nb of different RICH hits used in 2Hit
    Int_t nbDMdcHit;
    Int_t nbDMetaHit;
    Int_t nCorrCode1;//correlation code trk1, [1;7]
    //Rich-Mdc Rich-Meta Mdc-Meta  CorrCode
    //    0        0        1     :   1 
    //    0        1        0     :   2 
    //    0        1        1     :   3 
    //    1        0        0     :   4 
    //    1        0        1     :   5 
    //    1        1        0     :   6 
    //    1        1        1     :   7 
    Int_t nCorrCode2;
    Float_t opangMDC;//opening angle calculated w/ MDC
    Float_t opangKICK;//
    Float_t opangMETA;//opening angle calc w/ META
    Int_t charge;// +2:e+e+ , -2:e-e- , 0:e+e-
    Int_t KICKcharge;//same info from KickTrack cntr
    Int_t indTrk1;//index of hhitmatch
    Int_t indTrk2;
    Float_t invMass;
    Float_t mom1;//total momentum of 1st hhitmatch
    Float_t mom2;
    Float_t momt1;//transverse momentum of 1st hhitmatch
    Float_t momt2;
    Float_t pt;//pair transverse momentum
    Float_t rap;//pair rapidity
    Int_t indHitsTrk1[MAXIND];//hit indexes in hhitmatch
    Int_t indHitsTrk2[MAXIND];
public:

    HDiHitMatch();
    HDiHitMatch(HHitMatch*, HHitMatch*);
    virtual ~HDiHitMatch();

    HDiHitMatch(const HDiHitMatch& source);
    virtual HDiHitMatch& operator=(const HDiHitMatch& source);

 
    virtual void dumpToStdout();

    //------------------------------------------------------------------------
    void setNbDRichHit(Int_t n){nbDRichHit=n;};
    void setNbDMdcHit(Int_t n){nbDMdcHit=n;};
    void setNbDMetaHit(Int_t n){nbDMetaHit=n;};
    void setCorrCode1(Int_t n){nCorrCode1=n;};
    void setCorrCode2(Int_t n){nCorrCode2=n;};
    void setOpangMDC(Float_t o){opangMDC=o;};
    void setOpangKICK(Float_t o){opangKICK=o;};
    void setOpangMETA(Float_t o){opangMETA=o;};
    void setCharge(Int_t n){charge=n;};
    void setKICKCharge(Int_t n){KICKcharge=n;};
    void setIndTrk1(Int_t n){indTrk1=n;};
    void setIndTrk2(Int_t n){indTrk2=n;};
    void setPt(HHitMatch*, HHitMatch*);
    void setPairPt(HHitMatch*, HHitMatch*);
    void setPairRap(HHitMatch*, HHitMatch*);
    void setMomTrk1(Float_t mom){mom1=mom;};
    void setMomTrk2(Float_t mom){mom2=mom;};
    void setInvMass(Float_t o){invMass=o;};
    void setIndHitsTrk1(Int_t r,Int_t m,Int_t t,Int_t s,Int_t k){indHitsTrk1[0]=r;
                                                          indHitsTrk1[1]=m;
                                                          indHitsTrk1[2]=t;
							  indHitsTrk1[3]=s;
                                                          indHitsTrk1[4]=k; };
    void setIndHitsTrk2(Int_t r,Int_t m,Int_t t,Int_t s,Int_t k){indHitsTrk2[0]=r;
                                                          indHitsTrk2[1]=m;
                                                          indHitsTrk2[2]=t;
							  indHitsTrk2[3]=s;
                                                          indHitsTrk2[4]=k; };
    //------------------------------------------------------------------------
    Int_t getNbDRichHit(void){return nbDRichHit;};
    Int_t getNbDMdcHit(void){return nbDMdcHit;};
    Int_t getNbDMetaHit(void){return nbDMetaHit;};
    Int_t getCorrCode1(void){return nCorrCode1;};
    Int_t getCorrCode2(void){return nCorrCode2;};
    Float_t getOpangMDC(void){return opangMDC;};
    Float_t getOpangKICK(void){return opangKICK;};
    Float_t getOpangMETA(void){return opangMETA;};
    Int_t getCharge(void){return charge;};
    Int_t getKICKCharge(void){return KICKcharge;};
    Int_t getIndTrk1(void){return indTrk1;};
    Int_t getIndTrk2(void){return indTrk2;};
    Float_t getInvMass(void){return invMass;};
    Float_t getPairPt(void){return pt;};
    Float_t getPairRap(void){return rap;};
    Int_t* getIndHitsTrk1(void){return indHitsTrk1;};
    Int_t* getIndHitsTrk2(void){return indHitsTrk2;};
    Float_t getMomTrk1(void){return mom1;};
    Float_t getMomTrk2(void){return mom2;};
    Float_t getMomtTrk1(void){return momt1;};
    Float_t getMomtTrk2(void){return momt2;};
    Int_t getIndHitsTrk1(Int_t n){return indHitsTrk1[n];};
    Int_t getIndHitsTrk2(Int_t n){return indHitsTrk2[n];};
    //------------------------------------------------------------------------

    Float_t calcInvMass(HHitMatch*,HHitMatch*);
    Float_t calcOpangMDC(HHitMatch*,HHitMatch*);
    Float_t calcOpangKICK(HHitMatch*,HHitMatch*);
    Float_t calcOpangMETA(HHitMatch*,HHitMatch*);
    Int_t calcCorrCode(HHitMatch*);
    Int_t calcCharge(HHitMatch*,HHitMatch*);

    
 ClassDef(HDiHitMatch,1)
};


#endif // HDIHITMATCH_H
