/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4HodoOnlineSlots.h"
#include "TGo4HodoOnlineStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4HodoOnline.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"
#include "TPad.h"
#include "tqrootcanvas.h"


TGo4HodoOnlineSlots::TGo4HodoOnlineSlots(const char * name, const char * title, TGo4HodoOnline * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4HodoOnline(p)
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4HodoOnlineStatus= new TGo4HodoOnlineStatus("TGo4HodoOnlineStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4HodoOnlineStatus, p);
}
void TGo4HodoOnlineSlots::StartMonitor()
{
  // do now init+start analysis here
}
void TGo4HodoOnlineSlots::StopMonitor()
{
}
void TGo4HodoOnlineSlots::Reset()
{
}
TGo4HodoOnlineSlots::~TGo4HodoOnlineSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4HodoOnlineStatus);
	delete fxTGo4HodoOnlineStatus;
}
void TGo4HodoOnlineSlots::SetGUI(TGo4HodoOnline *GUI)
{
	TGo4LockGuard Global;
	fxTGo4HodoOnline=GUI;

}
TGo4Status * TGo4HodoOnlineSlots::GetStatus()
{
	return fxTGo4HodoOnlineStatus;
}
void TGo4HodoOnlineSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4HodoOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	
	if(fxTGo4HodoOnline!=0)  UpdateGUI(obj);
}	
void TGo4HodoOnlineSlots::AddObject(TNamed *fxdata, TQRootCanvas* c,TPad* p)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4HodoOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	if(obj && fxTGo4HodoOnline)
	{
	    c->getCanvas()->cd();
            ((HMonHistAddon*)obj)->setCanvas(c->getCanvas());
	    ((HMonHistAddon*)obj)->draw();
	    c->getCanvas()->Update();
	}

	//if(fxTGo4HodoOnline!=0)  UpdateGUI(obj);
}	
void TGo4HodoOnlineSlots::UpdateGUI(TObject *Obj)
{
  TGo4LockGuard Global;

  if(Obj!=0){
      if (Obj->InheritsFrom("HMonHistAddon")) {
	  HMonHistAddon* hist = (HMonHistAddon*) Obj;
	  fxTGo4HodoOnline->DrawHis(hist);
      }
  }
}

ClassImp(TGo4HodoOnlineSlots)
