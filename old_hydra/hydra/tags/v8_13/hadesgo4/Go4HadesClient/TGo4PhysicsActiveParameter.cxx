#include "TGo4PhysicsActiveParameter.h"
#include "TString.h"
#include "TObjString.h"
#include "TIterator.h"
#include "hphysicshistbooker.h"
#include "hmonhist.h"

ClassImp(TGo4PhysicsActiveParameter)

TGo4PhysicsActiveParameter::TGo4PhysicsActiveParameter()
{
    hasChanged=kFALSE;
    isInit=kFALSE;
    array=0;
}

TGo4PhysicsActiveParameter::TGo4PhysicsActiveParameter(Text_t* name, Text_t* title)
    :TGo4Parameter(name,title)
{
    hasChanged=kFALSE;
    isInit=kFALSE;
    array=0;
}
TGo4PhysicsActiveParameter::TGo4PhysicsActiveParameter(Text_t * name)
    :TGo4Parameter(name)
{
    hasChanged=kFALSE;
    isInit=kFALSE;
    array=0;
}
TGo4PhysicsActiveParameter::~TGo4PhysicsActiveParameter(){;}
Bool_t TGo4PhysicsActiveParameter::UpdateFrom(TGo4Parameter* rhs)
{
    TGo4PhysicsActiveParameter* p=(TGo4PhysicsActiveParameter*)rhs;
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
void TGo4PhysicsActiveParameter::Execute()
{
    HPhysicsHistBooker* booker=HPhysicsHistBooker::getExObject();

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
void TGo4PhysicsActiveParameter::Print()
{
    cout<<"######################TGo4PhysicsActiveParameter##################"<<endl;
    if(array)
    {

	for(Int_t i=0;i<array->GetLast()+1;i++)
	{
           cout<<(TString)(((TObjString*)((*array)[i]))->GetString()).Data()<<endl;
	}
    }
    cout<<"##################################################################"<<endl;
}
