#ifndef TGO4MDCACTIVEPARAMETER_H
#define TGO4MDCACTIVEPARAMETER_H

#include "Go4StatusBase/TGo4Parameter.h"
#include "TObjArray.h"

/** Class to the config of the mdc active hists.
 */
class TGo4MdcActiveParameter : public TGo4Parameter {
private:
    TObjArray* array;
    Bool_t hasChanged;
    Bool_t isInit;
public:
    TGo4MdcActiveParameter();
    TGo4MdcActiveParameter(Text_t * name, Text_t * title);
    TGo4MdcActiveParameter(Text_t * name);
    ~TGo4MdcActiveParameter();
    void Print();
    Bool_t UpdateFrom(TGo4Parameter*);
    Bool_t HasChanged()             {return hasChanged;}
    void   SetChanged()             {hasChanged=kTRUE;}
    Bool_t IsInit()                 {return isInit;}
    void   SetInit()                {isInit=kTRUE;}
    TObjArray* GetPar()             {return array;}
    void       SetPar(TObjArray* a) {array=(TObjArray*)a->Clone();}
    void       Execute();
    ClassDef(TGo4MdcActiveParameter,1)
};
#endif  /*!TGO4MDCACTIVEPARAMETER_H*/
