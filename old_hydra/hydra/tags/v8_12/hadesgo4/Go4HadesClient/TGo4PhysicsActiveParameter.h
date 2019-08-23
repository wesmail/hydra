#ifndef TGO4PYSICSACTIVEPARAMETER_H
#define TGO4PYSICSACTIVEPARAMETER_H

#include "Go4StatusBase/TGo4Parameter.h"
#include "TObjArray.h"

/** Class to the config of the physics active hists.
 */
class TGo4PhysicsActiveParameter : public TGo4Parameter {
private:
    TObjArray* array;
    Bool_t hasChanged;
    Bool_t isInit;
public:
    TGo4PhysicsActiveParameter();
    TGo4PhysicsActiveParameter(Text_t * name, Text_t * title);
    TGo4PhysicsActiveParameter(Text_t * name);
    ~TGo4PhysicsActiveParameter();
    void Print();
    Bool_t UpdateFrom(TGo4Parameter*);
    Bool_t HasChanged()             {return hasChanged;}
    void   SetChanged()             {hasChanged=kTRUE;}
    Bool_t IsInit()                 {return isInit;}
    void   SetInit()                {isInit=kTRUE;}
    TObjArray* GetPar()             {return array;}
    void       SetPar(TObjArray* a) {array=(TObjArray*)a->Clone();}
    void       Execute();
    ClassDef(TGo4PhysicsActiveParameter,1)
};
#endif  /*!TGO4PHYSICSACTIVEPARAMETER_H*/
