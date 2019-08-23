/* Generated by Together */

#include "TGo4TriggerOnlineStatus.h"
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TH1.h"
#include "TNamed.h"
#include "hmonhist.h"

TGo4TriggerOnlineStatus::TGo4TriggerOnlineStatus(Text_t *Name, Text_t * Title)
    :TGo4Status(Name)
{
  	TGo4LockGuard Global;
  	LocalListContent=new TObjArray();
  	LocalListIter = LocalListContent->MakeIterator();	

}
	TGo4TriggerOnlineStatus::~TGo4TriggerOnlineStatus()
{

}
TObject* TGo4TriggerOnlineStatus::OverWriteLocalObject(TNamed * fxData_Local)
{
	
 	TGo4LockGuard Global;
	TObject* OldLocalObject=0;
        TObject* NewObject=0;
	OldLocalObject=LocalListContent->FindObject(fxData_Local->GetName());

	
	if( OldLocalObject==0) {
	    NewObject=  fxData_Local->Clone();
	    LocalListContent->AddLast(NewObject);

	}else if(OldLocalObject->InheritsFrom("HMonHistAddon")){

	    HMonHistAddon* h=(HMonHistAddon*)OldLocalObject;
	    h->reset(0,0);
	    h->add((HMonHistAddon*)fxData_Local);
            NewObject=OldLocalObject;
	}else {
		LocalListContent->Remove(OldLocalObject);
		LocalListContent->Compress();
		LocalListContent->Expand(LocalListContent->GetLast()+1);
		delete OldLocalObject;
		NewObject=  fxData_Local->Clone();
		LocalListContent->AddLast( NewObject);
	}
        return NewObject;
}

TObjArray *TGo4TriggerOnlineStatus::GetLocalListStatus()
{
  	TGo4LockGuard Global;	
  	return(LocalListContent);
}

TObject* TGo4TriggerOnlineStatus::GetObjectfromLocalList(const char *Object)
{
  	TGo4LockGuard Global;
  	return(LocalListContent->FindObject(Object));
	
}
ClassImp(TGo4TriggerOnlineStatus)