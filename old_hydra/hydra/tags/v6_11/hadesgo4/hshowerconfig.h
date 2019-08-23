#ifndef HSHOWERCONFIG_H
#define HSHOWERCONFIG_H

#include "TNamed.h"

class HShowerConfig : public TNamed {
private:
    Int_t fEventNb;
    Int_t fEventSeqNb;
    Int_t calMult;
    Int_t rawMult;
    Int_t scale;
public:
    HShowerConfig(const char*,const char*);
    HShowerConfig();
    ~HShowerConfig();
    Float_t getEvtNb()    {return fEventNb/(Float_t)scale;}
    Int_t   getEvtSeqNb() {return fEventSeqNb;}
    Int_t   getRawMult()  {return rawMult;}
    Int_t   getCalMult()  {return calMult;}
    Int_t   getScale()    {return scale;}

    void    setEvtSeqNb(Int_t i){fEventSeqNb=i;}
    void    setEvtNb   (Int_t j){fEventNb=j;}
    void    setRawMult (Int_t i){rawMult=i;}
    void    setCalMult (Int_t i){calMult=i;}
    void    setScale   (Int_t i){scale=i;}


    ClassDef(HShowerConfig,1)
};
#endif  /*!HSHOWERCONFIG_H*/
