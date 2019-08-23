// $Id: hhitmatchheadersim.h,v 1.4 2008-09-18 13:05:35 halo Exp $
// Last update by Thomas Eberl: 04/08/02 14:30:38
//
#ifndef HHITMATCHHEADERSIM_H
#define HHITMATCHHEADERSIM_H
#define MAXPARIDS 100
#include "TObject.h"
#include "hcategory.h"
#include "hhitmatchheader.h"

class HHitMatchHeaderSim: public HHitMatchHeader {

protected:

    
    Int_t nNb_Part;
    Float_t fAvrgRichWeight;
    Float_t fAvrgMdcWeight;
    Float_t imppar;
    Int_t   nParIdArr[MAXPARIDS];
    Int_t nNbFakes;
    Int_t nNbConfTracklet;
    Int_t nGNb_RM;
    Int_t nGNb_RT_S;
    Int_t nGNb_MT_S;
    Int_t nGNb_RMT_S; 
    
public:

    HHitMatchHeaderSim();
    virtual ~HHitMatchHeaderSim();
    
    HHitMatchHeaderSim(const HHitMatchHeaderSim& source);
    virtual HHitMatchHeaderSim& operator=(const HHitMatchHeaderSim& source);
    Int_t getNbPart(){return nNb_Part;};
    Int_t getNbConfTracklets(){return nNbConfTracklet;};
    Int_t getNbFakes(){return nNbFakes;};
    Float_t getAvrgRichWeight(){return fAvrgRichWeight;};
    Float_t getAvrgMdcWeight(){return fAvrgMdcWeight;};
    Int_t getParId(Int_t index){return nParIdArr[index];};
    Int_t getGeantNb_RM(){return nGNb_RM;};
    Int_t getGeantNb_RT_S(){return nGNb_RT_S;};
    Int_t getGeantNb_MT_S(){return nGNb_MT_S;};
    Int_t getGeantNb_RMT_S(){return nGNb_RMT_S;};
    Float_t getImpactParam(){return imppar;};
    void setNbPart(Int_t i){nNb_Part=i;};
    void setNbConfTracklets(Int_t i){nNbConfTracklet=i;};
    void setNbFakes(Int_t i){nNbFakes=i;};
    void setAvrgRichWeight(Float_t f){fAvrgRichWeight=f;};
    void setAvrgMdcWeight(Float_t f){fAvrgMdcWeight=f;};
    void setParId(Int_t index,Int_t val){nParIdArr[index]+=val;};
    void setGeantNb_RM(Int_t i){nGNb_RM=i;};
    void setGeantNb_RT_S(Int_t i){nGNb_RT_S=i;};
    void setGeantNb_MT_S(Int_t i){nGNb_MT_S=i;};
    void setGeantNb_RMT_S(Int_t i){nGNb_RMT_S=i;};
    void setImpactParam(Float_t imp){imppar=imp;};

    void dumpToStdoutSim(); 
    void resetSim();
 ClassDef(HHitMatchHeaderSim,1)
};


#endif // HHITMATCHHEADERSIM_H
