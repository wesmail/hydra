#include "TGo4MdcActiveParameter.h"
#include "TString.h"
#include "TObjString.h"
#include "TIterator.h"
#include "hmdchistbooker.h"
#include "hmonhist.h"

ClassImp(TGo4MdcActiveParameter)

TGo4MdcActiveParameter::TGo4MdcActiveParameter()
{
    hasChanged=kFALSE;
    isInit=kFALSE;
    array=0;
}

TGo4MdcActiveParameter::TGo4MdcActiveParameter(Text_t* name, Text_t* title)
    :TGo4Parameter(name,title)
{
    hasChanged=kFALSE;
    isInit=kFALSE;
    array=0;
}
TGo4MdcActiveParameter::TGo4MdcActiveParameter(Text_t * name)
    :TGo4Parameter(name)
{
    hasChanged=kFALSE;
    isInit=kFALSE;
    array=0;
}
TGo4MdcActiveParameter::~TGo4MdcActiveParameter(){;}
Bool_t TGo4MdcActiveParameter::UpdateFrom(TGo4Parameter* rhs)
{
    TGo4MdcActiveParameter* p=(TGo4MdcActiveParameter*)rhs;
    if(p->GetPar())
    {
        SetPar(p->GetPar());
	if(array)
	{
	   if(isInit)
	   {
	       Execute();
	   }
	   array->Delete();
           array=0;
	}
    }
    return kTRUE;
}
void TGo4MdcActiveParameter::Execute()
{
    HMdcHistBooker* booker=HMdcHistBooker::getExObject();

    if(booker)
    {
	TList* list=booker->getListOfActiveHists();
        TIterator* iter=list->MakeIterator();
        TString tmp;
        HMonHistAddon* h;
	while ((h=(HMonHistAddon*)iter->Next())!= 0)
	{
	    h->setSend(0);
	}
	delete iter;
	for(Int_t i=0;i<array->GetLast()+1;i++)
	{
            tmp=(TString)(((TObjString*)((*array)[i]))->GetString());
            ((HMonHistAddon*)(list->FindObject(tmp.Data())))->setSend(1);
	}
    }
}
void TGo4MdcActiveParameter::Print()
{
    cout<<"######################TGo4MdcActiveParameter######################"<<endl;
    if(array)
    {

	for(Int_t i=0;i<array->GetLast()+1;i++)
	{
           cout<<(TString)(((TObjString*)((*array)[i]))->GetString()).Data()<<endl;
	}
    }
    cout<<"##################################################################"<<endl;
}
