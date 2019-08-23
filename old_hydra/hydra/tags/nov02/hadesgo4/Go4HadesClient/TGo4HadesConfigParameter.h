#ifndef TGO4HADESCONFIGPARAMETER_H
#define TGO4HADESCONFIGPARAMETER_H

#include "Go4StatusBase/TGo4Parameter.h"
#include "hgo4onlineconfig.h"

/** Class to the config of the hades client.
 */
class TGo4HadesConfigParameter : public TGo4Parameter {
private:
    HGo4OnlineParam par;
    Bool_t hasChanged;
    Bool_t isInit;
public:
    TGo4HadesConfigParameter();
    TGo4HadesConfigParameter(Text_t * name, Text_t * title);
    TGo4HadesConfigParameter(Text_t * name);
    ~TGo4HadesConfigParameter();
    void Print();
    HGo4OnlineParam* GetPar(){return &par;}
    void   SetPar(HGo4OnlineParam*);
    Bool_t UpdateFrom(TGo4Parameter*);
    Bool_t HasChanged(){return hasChanged;}
    void   SetChanged(){hasChanged=kTRUE;}
    Bool_t IsInit() {return isInit;}
    void   SetInit(){isInit=kTRUE;}
    ClassDef(TGo4HadesConfigParameter,1)
};
#endif  /*!TGO4HADESCONFIGPARAMETER_H*/
