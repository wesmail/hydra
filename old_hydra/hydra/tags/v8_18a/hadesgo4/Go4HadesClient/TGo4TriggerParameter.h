#ifndef TGO4TRIGGERPARAMETER_H
#define TGO4TRIGGERPARAMETER_H

#include "Go4StatusBase/TGo4Parameter.h"
#include "hgo4onlineconfig.h"

/** Class to the config of the hades client.
 */
class TGo4TriggerParameter : public TGo4Parameter {
private:
    Int_t  useDownscaled;
    Bool_t downscaledChanged;

    Bool_t isInit;
public:
    TGo4TriggerParameter();
    TGo4TriggerParameter(Text_t * name, Text_t * title);
    TGo4TriggerParameter(Text_t * name);
    ~TGo4TriggerParameter();

    void   Copy(TGo4Parameter*);
    void   Execute();
    Bool_t UpdateFrom(TGo4Parameter*);

    Int_t  GetUseDownscaled()         { return useDownscaled; }
    void   SetUseDownscaled(Int_t b)  { useDownscaled=b; this->SetDownscaledChanged(kTRUE); }
    Bool_t GetDownscaledChanged()         { return downscaledChanged; }
    void   SetDownscaledChanged(Bool_t b) { downscaledChanged=b;      }

    Bool_t GetInit() { return isInit; }
    void   SetInit() { isInit=kTRUE;  }

    ClassDef(TGo4TriggerParameter,1)
};
#endif  /*!TGO4TRIGGERPARAMETER_H*/
