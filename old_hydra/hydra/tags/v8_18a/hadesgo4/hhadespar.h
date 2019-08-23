#ifndef HHADESPAR_H
#define HHADESPAR_H

#include "TNamed.h"
/** Class to hold numbers for Hades Online Display.
 */
class HHadesPar : public TNamed {
private:
    Double_t fEventRate;    ///< event rate
public:
    HHadesPar(const Char_t*,const Char_t*);
    HHadesPar();
    ~HHadesPar();
    Double_t getEvtRate(){return  fEventRate;}
    void     setEvtRate(Float_t i){fEventRate=i;}
    ClassDef(HHadesPar,1)
};
#endif  /*!HHADESPAR_H*/
