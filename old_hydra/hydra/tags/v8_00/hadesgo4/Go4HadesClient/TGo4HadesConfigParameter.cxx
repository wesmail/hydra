#include "TGo4HadesConfigParameter.h"

ClassImp(TGo4HadesConfigParameter)

TGo4HadesConfigParameter::TGo4HadesConfigParameter()
{
  hasChanged=kFALSE;
}

TGo4HadesConfigParameter::TGo4HadesConfigParameter(Text_t* name, Text_t* title)
    :TGo4Parameter(name,title)
{
    hasChanged=kFALSE;
}
TGo4HadesConfigParameter::TGo4HadesConfigParameter(Text_t * name)
    :TGo4Parameter(name)
{
    hasChanged=kFALSE;
}
TGo4HadesConfigParameter::~TGo4HadesConfigParameter(){;}
Bool_t TGo4HadesConfigParameter::UpdateFrom(TGo4Parameter* rhs)
{
    TGo4HadesConfigParameter* param=(TGo4HadesConfigParameter*)rhs;
    par.copySet(param->GetPar());
    mdcset.copySet(param->GetMdcSet());
    hasChanged=param->HasChanged();
    return kTRUE;
}
void TGo4HadesConfigParameter::SetPar(HGo4OnlineParam* p)
{
    par.copySet(p);
}
void TGo4HadesConfigParameter::SetMdcSet(HMdcSetup* p)
{
    mdcset.copySet(p);
}
void TGo4HadesConfigParameter::Print()
{
    cout<<"######################TGo4HadesConfigParameter######################"<<endl;
    par.printParam();
    cout<<"####################################################################"<<endl;
}
