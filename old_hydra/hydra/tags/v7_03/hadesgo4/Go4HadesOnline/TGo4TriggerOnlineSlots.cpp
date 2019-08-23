/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4TriggerOnlineSlots.h"
#include "TGo4TriggerOnlineStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4TriggerOnline.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"
#include "TPad.h"
#include "tqrootcanvas.h"


TGo4TriggerOnlineSlots::TGo4TriggerOnlineSlots(const char * name, const char * title, TGo4TriggerOnline * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4TriggerOnline(p)
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4TriggerOnlineStatus= new TGo4TriggerOnlineStatus("TGo4TriggerOnlineStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4TriggerOnlineStatus, p);
}
void TGo4TriggerOnlineSlots::StartMonitor()
{
  // do now init+start analysis here
}
void TGo4TriggerOnlineSlots::StopMonitor()
{
}
void TGo4TriggerOnlineSlots::Reset()
{
}
TGo4TriggerOnlineSlots::~TGo4TriggerOnlineSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4TriggerOnlineStatus);
	delete fxTGo4TriggerOnlineStatus;
}
void TGo4TriggerOnlineSlots::SetGUI(TGo4TriggerOnline *GUI)
{
	TGo4LockGuard Global;
	fxTGo4TriggerOnline=GUI;

}
TGo4Status * TGo4TriggerOnlineSlots::GetStatus()
{
	return fxTGo4TriggerOnlineStatus;
}
void TGo4TriggerOnlineSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	TObject* obj=fxTGo4TriggerOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	
	if(fxTGo4TriggerOnline!=0)  UpdateGUI(obj);
}	
void TGo4TriggerOnlineSlots::AddObject(TNamed *fxdata, TQRootCanvas* c,TPad* p)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4TriggerOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	if(obj && fxTGo4TriggerOnline)
	{
	    c->getCanvas()->cd();
            ((HMonHistAddon*)obj)->setCanvas(c->getCanvas());
	    ((HMonHistAddon*)obj)->draw();
	    c->getCanvas()->Update();
	}

	//if(fxTGo4TriggerOnline!=0)  UpdateGUI(obj);
}	
void TGo4TriggerOnlineSlots::UpdateGUI(TObject* Obj)
{
  TGo4LockGuard Global;

  if(Obj!=0){
      if (Obj->InheritsFrom("HMonHistAddon")) {
	  HMonHistAddon* hist = (HMonHistAddon*) Obj;
	  fxTGo4TriggerOnline->DrawHis(hist);
      }
  }
}

ClassImp(TGo4TriggerOnlineSlots)
