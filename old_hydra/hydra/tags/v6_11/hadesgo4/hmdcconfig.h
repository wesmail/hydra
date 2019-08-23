#ifndef HMDCCONFIG_H
#define HMDCCONFIG_H

#include "TNamed.h"

class HMdcConfig : public TNamed {
private:
    Int_t fEventNb;
    Int_t fEventSeqNb;
    Int_t calMult;
    Int_t rawMult;
    Int_t scale;
public:
    HMdcConfig(const char*,const char*);
    HMdcConfig();
    ~HMdcConfig();
    Float_t getEvtNb()    {return  fEventNb/(Float_t)scale;}
    Int_t   getEvtSeqNb() {return fEventSeqNb;}
    Int_t   getRawMult()  {return rawMult;}
    Int_t   getCalMult()  {return calMult;}
    Int_t   getScale()    {return scale;}

    void    setEvtSeqNb(Int_t i){fEventSeqNb=i;}
    void    setEvtNb   (Int_t j){fEventNb=j;}
    void    setRawMult (Int_t i){rawMult=i;}
    void    setCalMult (Int_t i){calMult=i;}
    void    setScale   (Int_t i){scale=i;}


    ClassDef(HMdcConfig,1)
};
#endif  /*!HMDCCONFIG_H*/
