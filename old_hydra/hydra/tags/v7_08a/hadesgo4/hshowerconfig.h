#ifndef HSHOWERCONFIG_H
#define HSHOWERCONFIG_H

#include "TNamed.h"
#include "TArrayI.h"
/** Class to hold some numbers for the Shower Online Display.
*/
class HShowerConfig : public TNamed {
private:
    Int_t fEventNb;      ///< event number
    Int_t fEventSeqNb;   ///< sequence number
    Int_t calMult;       ///< size of the raw category
    Int_t rawMult;       ///< size of the cal category
    Int_t scale;         ///< scale factor (->kEvents)
    Int_t nHits[3][8];
    TArrayI colorsSectors;

public:
    HShowerConfig(const char*,const char*);
    HShowerConfig();
    ~HShowerConfig();
    Float_t getEvtNb()    {return fEventNb/(Float_t)scale;}
    Int_t   getEvtSeqNb() {return fEventSeqNb;}
    Int_t   getRawMult()  {return rawMult;}
    Int_t   getCalMult()  {return calMult;}
    Int_t   getScale()    {return scale;}
    Int_t*  getHits(Int_t);
    const TArrayI getColorsSector(){return colorsSectors;}

    void    setEvtSeqNb(Int_t i){fEventSeqNb=i;}
    void    setEvtNb   (Int_t j){fEventNb=j;}
    void    setRawMult (Int_t i){rawMult=i;}
    void    setCalMult (Int_t i){calMult=i;}
    void    setScale   (Int_t i){scale=i;}
    void    setColorsSector(TArrayI colors){colorsSectors = colors;}
    void    setHits(Int_t,Int_t);
    void    resetHits(Int_t);

    ClassDef(HShowerConfig,1)
};
#endif  /*!HSHOWERCONFIG_H*/

