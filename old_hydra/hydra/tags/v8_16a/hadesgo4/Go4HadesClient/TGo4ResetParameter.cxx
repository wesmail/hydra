#include "TGo4ResetParameter.h"
#include "htriggerhistbooker.h"
#include "hstarthistbooker.h"
#include "hrichhistbooker.h"
#include "hmdchistbooker.h"
#include "htofhistbooker.h"
#include "htofinohistbooker.h"
#include "hhodohistbooker.h"
#include "hwallhistbooker.h"
#include "hshowerhistbooker.h"
#include "hphysicshistbooker.h"

ClassImp(TGo4ResetParameter)

TGo4ResetParameter::TGo4ResetParameter()
{
    triggerChanged=kFALSE;
    startChanged  =kFALSE;
    richChanged   =kFALSE;
    mdcChanged    =kFALSE;
    tofChanged    =kFALSE;
    tofinoChanged =kFALSE;
    hodoChanged   =kFALSE;
    wallChanged   =kFALSE;
    showerChanged =kFALSE;
    physicsChanged=kFALSE;
    message="no";
    level=0;
    isInit=       kFALSE;
}

TGo4ResetParameter::TGo4ResetParameter(Text_t* name, Text_t* title)
    :TGo4Parameter(name,title)
{
    triggerChanged=kFALSE;
    startChanged  =kFALSE;
    richChanged   =kFALSE;
    mdcChanged    =kFALSE;
    tofChanged    =kFALSE;
    tofinoChanged =kFALSE;
    hodoChanged   =kFALSE;
    wallChanged   =kFALSE;
    showerChanged =kFALSE;
    physicsChanged=kFALSE;
    message="no";
    level=0;
    isInit=       kFALSE;
}
TGo4ResetParameter::TGo4ResetParameter(Text_t * name)
    :TGo4Parameter(name)
{
    triggerChanged=kFALSE;
    startChanged  =kFALSE;
    richChanged   =kFALSE;
    mdcChanged    =kFALSE;
    tofChanged    =kFALSE;
    tofinoChanged =kFALSE;
    hodoChanged   =kFALSE;
    wallChanged   =kFALSE;
    showerChanged =kFALSE;
    physicsChanged=kFALSE;
    message="no";
    level=0;
    isInit=       kFALSE;
}
TGo4ResetParameter::~TGo4ResetParameter(){;}
void TGo4ResetParameter::Copy(TGo4Parameter* rhs)
{
    TGo4ResetParameter* p=(TGo4ResetParameter*)rhs;
    SetMessage       (p->GetMessage());
    SetLevel         (p->GetLevel());
    SetTriggerChanged(p->GetTriggerChanged());
    SetStartChanged  (p->GetStartChanged());
    SetRichChanged   (p->GetRichChanged());
    SetMdcChanged    (p->GetMdcChanged());
    SetTofChanged    (p->GetTofChanged());
    SetTofinoChanged (p->GetTofinoChanged());
    SetHodoChanged   (p->GetHodoChanged());
    SetWallChanged   (p->GetWallChanged());
    SetShowerChanged (p->GetShowerChanged());
    SetPhysicsChanged(p->GetPhysicsChanged());
}
void TGo4ResetParameter::Execute()
{
    if(message.CompareTo("All")==0)
    {
	HTriggerHistBooker::getExObject()->resetActiveList(level,0);
	HStartHistBooker  ::getExObject()->resetActiveList(level,0);
	HRichHistBooker   ::getExObject()->resetActiveList(level,0);
	HMdcHistBooker    ::getExObject()->resetActiveList(level,0);
	HTofHistBooker    ::getExObject()->resetActiveList(level,0);
	HTofinoHistBooker ::getExObject()->resetActiveList(level,0);
	HHodoHistBooker   ::getExObject()->resetActiveList(level,0);
	HWallHistBooker   ::getExObject()->resetActiveList(level,0);
	HShowerHistBooker ::getExObject()->resetActiveList(level,0);
        HPhysicsHistBooker::getExObject()->resetActiveList(level,0);

	triggerChanged =kFALSE;
        startChanged   =kFALSE;
        richChanged    =kFALSE;
        mdcChanged     =kFALSE;
        tofChanged     =kFALSE;
        tofinoChanged  =kFALSE;
        hodoChanged    =kFALSE;
        wallChanged    =kFALSE;
        showerChanged  =kFALSE;
        physicsChanged =kFALSE;
        cout<<"TGo4ResetParameter: Reset All Histograms!"<<endl;
    }
    else if(message.CompareTo("Trigger")==0 && triggerChanged)
    {
	HTriggerHistBooker::getExObject()->resetActiveList(level,0);
        triggerChanged=kFALSE;
        cout<<"TGo4ResetParameter: Reset Trigger Histograms!"<<endl;
    }
    else if(message.CompareTo("Start")==0 && startChanged)
    {
	HStartHistBooker::getExObject()->resetActiveList(level,0);
        startChanged=kFALSE;
        cout<<"TGo4ResetParameter: Reset Start Histograms!"<<endl;
    }
    else if(message.CompareTo("Rich")==0 && richChanged)
    {
	HRichHistBooker::getExObject()->resetActiveList(level,0);
        richChanged=kFALSE;
        cout<<"TGo4ResetParameter: Reset Rich Histograms!"<<endl;
    }
    else if(message.CompareTo("Mdc")==0 && mdcChanged)
    {
	HMdcHistBooker::getExObject()->resetActiveList(level,0);
        mdcChanged=kFALSE;
        cout<<"TGo4ResetParameter: Reset Mdc Histograms!"<<endl;
    }
    else if(message.CompareTo("Tof")==0 && tofChanged)
    {
	HTofHistBooker::getExObject()->resetActiveList(level,0);
        tofChanged=kFALSE;
        cout<<"TGo4ResetParameter: Reset Tof Histograms!"<<endl;
    }
    else if(message.CompareTo("Tofino")==0 && tofinoChanged)
    {
	HTofinoHistBooker::getExObject()->resetActiveList(level,0);
        tofinoChanged=kFALSE;
        cout<<"TGo4ResetParameter: Reset Tofino Histograms!"<<endl;
    }
    else if(message.CompareTo("Hodo")==0 && hodoChanged)
    {
	HHodoHistBooker::getExObject()->resetActiveList(level,0);
        hodoChanged=kFALSE;
        cout<<"TGo4ResetParameter: Reset Hodo Histograms!"<<endl;
    }
    else if(message.CompareTo("Wall")==0 && wallChanged)
    {
	HWallHistBooker::getExObject()->resetActiveList(level,0);
        wallChanged=kFALSE;
        cout<<"TGo4ResetParameter: Reset Wall Histograms!"<<endl;
    }
    else if(message.CompareTo("Shower")==0 && showerChanged)
    {
	HShowerHistBooker::getExObject()->resetActiveList(level,0);
        showerChanged=kFALSE;
        cout<<"TGo4ResetParameter: Reset Shower Histograms!"<<endl;
    }
    else if(message.CompareTo("Physics")==0 && physicsChanged)
    {
	HPhysicsHistBooker::getExObject()->resetActiveList(level,0);
	physicsChanged=kFALSE;
        cout<<"TGo4ResetParameter: Reset Physics Histograms!"<<endl;
    }
    else
    {
	Error("TGo4ResetParameter:Execute()","UNKNOWN ARGUMENT %s , WILL SKIP!",message.Data());
    }
    message="no";
}


Bool_t TGo4ResetParameter::UpdateFrom(TGo4Parameter* rhs)
{
    Copy(rhs);
    if(isInit) Execute();

    return kTRUE;
}
void TGo4ResetParameter::Print()
{
    cout<<"######################TGo4ResetParameter######################"<<endl;
    printf("message          : %s \n",message.Data());
    printf("level            : %i \n",level);
    printf("triggerChanged   : %i \n",(Int_t)triggerChanged);
    printf("StartChanged     : %i \n",(Int_t)startChanged);
    printf("richChanged      : %i \n",(Int_t)richChanged);
    printf("mdcChanged       : %i \n",(Int_t)mdcChanged);
    printf("tofChanged       : %i \n",(Int_t)tofChanged);
    printf("tofinoChanged    : %i \n",(Int_t)tofinoChanged);
    printf("hodoChanged      : %i \n",(Int_t)hodoChanged);
    printf("wallChanged      : %i \n",(Int_t)wallChanged);
    printf("showerChanged    : %i \n",(Int_t)showerChanged);
    printf("physicsChanged   : %i \n",(Int_t)physicsChanged);
    printf("isInit           : %i \n",(Int_t)isInit);
    cout<<"##############################################################"<<endl;
}
