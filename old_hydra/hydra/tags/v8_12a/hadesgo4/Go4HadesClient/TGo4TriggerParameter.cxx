#include "TGo4TriggerParameter.h"
#include "htriggergo4mon.h"

ClassImp(TGo4TriggerParameter)

TGo4TriggerParameter::TGo4TriggerParameter()
{
    useDownscaled     = 1;
    downscaledChanged = kFALSE;
    isInit            = kFALSE;
}

TGo4TriggerParameter::TGo4TriggerParameter(Text_t* name, Text_t* title)
    :TGo4Parameter(name,title)
{
  cout << "TriggerParameter constructed..." << endl;
    useDownscaled     = 1;
    downscaledChanged = kFALSE;
    isInit            = kFALSE;
}

TGo4TriggerParameter::TGo4TriggerParameter(Text_t * name)
    :TGo4Parameter(name)
{
    useDownscaled     = 1;
    downscaledChanged = kFALSE;
    isInit            = kFALSE;
}

TGo4TriggerParameter::~TGo4TriggerParameter()
{
  ;
}

void TGo4TriggerParameter::Copy(TGo4Parameter* rhs)
{
    TGo4TriggerParameter* p = (TGo4TriggerParameter*)rhs;

    this->SetUseDownscaled    ( p->GetUseDownscaled()     );
    this->SetDownscaledChanged( p->GetDownscaledChanged() );
}

void TGo4TriggerParameter::Execute()
{
  if( downscaledChanged==kTRUE )
    {
      HTriggerGo4Mon::getExObject()->setDownscaled( this->GetUseDownscaled() );
      this->SetDownscaledChanged(kFALSE);
    }

}

Bool_t TGo4TriggerParameter::UpdateFrom(TGo4Parameter* rhs)
{
    Copy(rhs);
    if(isInit) Execute();

    return kTRUE;
}
