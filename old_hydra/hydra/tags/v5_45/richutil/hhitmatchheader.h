// File: hhitmatchheader.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/12 17:50:04
//
#ifndef HHITMATCHHEADER_H
#define HHITMATCHHEADER_H

#include <TObject.h>
#include "hcategory.h"

class HHitMatchHeader: public TObject {

protected:
    virtual void reset();
    
    Int_t nNb_CorrObjs;
    Int_t nNb_RM;
    Int_t nNb_RT;
    Int_t nNb_RS;
    Int_t nNb_MT;
    Int_t nNb_MS;
    Int_t nNb_minRMT;
    Int_t nNb_minRMS;
    Int_t nNb_RcMcT;
    Int_t nNb_RcMcTcR;
    Int_t nNb_RMcTcR;
    Int_t nNb_RcMTcR;
    Int_t nNb_RcMcS;
    Int_t nNb_RcMcScR;
    Int_t nNb_RMcScR;
    Int_t nNb_RcMScR;
    Int_t nNb_Rich;
    Int_t nNb_MdcSeg;
    Int_t nNb_Tof;
    Int_t nNb_Shower;
    Int_t nNb_DRich;
    Int_t nNb_DMdcSeg;
    Int_t nNb_DTof;
    Int_t nNb_DShower;
public:

    HHitMatchHeader();
    virtual ~HHitMatchHeader();
    
    HHitMatchHeader(const HHitMatchHeader& source);
    virtual HHitMatchHeader& operator=(const HHitMatchHeader& source);
    virtual void dumpToStdout(); 
    void setNbCorrObjs(Int_t nb){nNb_CorrObjs=nb;};
    
    void setNbRichHits(Int_t nb){nNb_Rich=nb;};
    void setNbMdcSegHits(Int_t nb){nNb_MdcSeg=nb;};
    void setNbShowerHits(Int_t nb){nNb_Shower=nb;};
    void setNbTofHits(Int_t nb){nNb_Tof=nb;};
    void setNbDiscreteRichHits(Int_t nb){nNb_DRich=nb;};
    void setNbDiscreteMdcSegHits(Int_t nb){nNb_DMdcSeg=nb;};
    void setNbDiscreteShowerHits(Int_t nb){nNb_DShower=nb;};
    void setNbDiscreteTofHits(Int_t nb){nNb_DTof=nb;};
    void setNb_RcMcT(Int_t nb){nNb_RcMcT=nb;};
    void setNb_RcMcTcR(Int_t nb){nNb_RcMcTcR=nb;};
    void setNb_RMcTcR(Int_t nb){nNb_RMcTcR=nb;};
    void setNb_RcMTcR(Int_t nb){nNb_RcMTcR=nb;};
    void setNb_RcMcS(Int_t nb){nNb_RcMcS=nb;};
    void setNb_RcMcScR(Int_t nb){nNb_RcMcScR=nb;};
    void setNb_RMcScR(Int_t nb){nNb_RMcScR=nb;};
    void setNb_RcMScR(Int_t nb){nNb_RcMScR=nb;};
    void setNb_minRMT(Int_t nb){nNb_minRMT=nb;};
    void setNb_minRMS(Int_t nb){nNb_minRMS=nb;};

    Int_t getNbCorrObjs(){return nNb_CorrObjs;};
    Int_t getNbRichHits(void){return nNb_Rich;};
    Int_t getNbMdcSegHits(void){return nNb_MdcSeg;};
    Int_t getNbShowerHits(void){return nNb_Shower;};
    Int_t getNbTofHits(void){return nNb_Tof;};
    Int_t getNbDiscreteRichHits(void){return nNb_DRich;};
    Int_t getNbDiscreteMdcSegHits(void){return nNb_DMdcSeg;};
    Int_t getNbDiscreteShowerHits(void){return nNb_DShower;};
    Int_t getNbDiscreteTofHits(void){return nNb_DTof;};
    Int_t getNb_RcMcT(void){return nNb_RcMcT;};
    Int_t getNb_RcMcTcR(void){return nNb_RcMcTcR;};
    Int_t getNb_RMcTcR(void){return nNb_RMcTcR;};
    Int_t getNb_RcMTcR(void){return nNb_RcMTcR;};
    Int_t getNb_RcMcS(void){return nNb_RcMcS;};
    Int_t getNb_RcMcScR(void){return nNb_RcMcScR;};
    Int_t getNb_RMcScR(void){return nNb_RMcScR;};
    Int_t getNb_RcMScR(void){return nNb_RcMScR;};
    Int_t getNb_minRMT(void){return nNb_minRMT;};
    Int_t getNb_minRMS(void){return nNb_minRMS;};

    void setNb_RM(Int_t nb){nNb_RM=nb;};
    void setNb_RT(Int_t nb){nNb_RT=nb;};
    void setNb_RS(Int_t nb){nNb_RS=nb;};
    void setNb_MT(Int_t nb){nNb_MT=nb;};
    void setNb_MS(Int_t nb){nNb_MS=nb;};

    Int_t getNb_RM(){return nNb_RM;};
    Int_t getNb_RT(){return nNb_RT;};
    Int_t getNb_RS(){return nNb_RS;};
    Int_t getNb_MT(){return nNb_MT;};
    Int_t getNb_MS(){return nNb_MS;};

    
 ClassDef(HHitMatchHeader,1)
};


#endif // HHITMATCHHEADER_H
