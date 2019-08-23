#ifndef TGO4RESETPARAMETER_H
#define TGO4RESETPARAMETER_H

#include "Go4StatusBase/TGo4Parameter.h"
#include "hgo4onlineconfig.h"
#include "TString.h"

/** Class to the config of the hades client.
 */
class TGo4ResetParameter : public TGo4Parameter {
private:
    TString message;
    Int_t level;

    Bool_t triggerChanged;
    Bool_t startChanged;
    Bool_t richChanged;
    Bool_t mdcChanged;
    Bool_t tofChanged;
    Bool_t tofinoChanged;
    Bool_t showerChanged;

    Bool_t isInit;
public:
    TGo4ResetParameter();
    TGo4ResetParameter(Text_t * name, Text_t * title);
    TGo4ResetParameter(Text_t * name);
    ~TGo4ResetParameter();
    void Print();
    Bool_t  UpdateFrom(TGo4Parameter*);
    void    Set(TString mes,Int_t lev){message=mes;level=lev;}
    void    SetLevel(Int_t lev)        {level=lev;}
    Int_t   GetLevel()                 {return level;}
    Bool_t  GetTriggerChanged()        {return triggerChanged;}
    void    SetTriggerChanged(Bool_t b){triggerChanged=b;}
    Bool_t  GetStartChanged()          {return startChanged;}
    void    SetStartChanged(Bool_t b)  {startChanged=b;}
    Bool_t  GetRichChanged()           {return richChanged;}
    void    SetRichChanged(Bool_t b)   {richChanged=b;}
    Bool_t  GetMdcChanged()            {return mdcChanged;}
    void    SetMdcChanged(Bool_t b)    {mdcChanged=b;}
    Bool_t  GetTofChanged()            {return tofChanged;}
    void    SetTofChanged(Bool_t b)    {tofChanged=b;}
    Bool_t  GetTofinoChanged()         {return tofinoChanged;}
    void    SetTofinoChanged(Bool_t b) {tofinoChanged=b;}
    Bool_t  GetShowerChanged()         {return showerChanged;}
    void    SetShowerChanged(Bool_t b) {showerChanged=b;}
    TString GetMessage()               {return message;}
    void    SetMessage(TString mes)    {message=mes;}
    Bool_t  IsInit()                   {return isInit;}
    void    SetInit()                  {isInit=kTRUE;}
    void    Execute();
    void    Copy(TGo4Parameter*);
    ClassDef(TGo4ResetParameter,1)
};
#endif  /*!TGO4RESETPARAMETER_H*/
