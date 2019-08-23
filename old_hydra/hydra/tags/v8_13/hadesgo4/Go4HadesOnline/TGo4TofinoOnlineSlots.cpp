/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4TofinoOnlineSlots.h"
#include "TGo4TofinoOnlineStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4TofinoOnline.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"
#include "TPad.h"
#include "tqrootcanvas.h"


TGo4TofinoOnlineSlots::TGo4TofinoOnlineSlots(const char * name, const char * title, TGo4TofinoOnline * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4TofinoOnline(p)
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4TofinoOnlineStatus= new TGo4TofinoOnlineStatus("TGo4TofinoOnlineStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4TofinoOnlineStatus, p);
}
void TGo4TofinoOnlineSlots::StartMonitor()
{
  // do now init+start analysis here
}
void TGo4TofinoOnlineSlots::StopMonitor()
{
}
void TGo4TofinoOnlineSlots::Reset()
{
}
TGo4TofinoOnlineSlots::~TGo4TofinoOnlineSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4TofinoOnlineStatus);
	delete fxTGo4TofinoOnlineStatus;
}
void TGo4TofinoOnlineSlots::SetGUI(TGo4TofinoOnline *GUI)
{
	TGo4LockGuard Global;
	fxTGo4TofinoOnline=GUI;

}
TGo4Status * TGo4TofinoOnlineSlots::GetStatus()
{
	return fxTGo4TofinoOnlineStatus;
}
void TGo4TofinoOnlineSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4TofinoOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	
	if(fxTGo4TofinoOnline!=0)  UpdateGUI(obj);
}	
void TGo4TofinoOnlineSlots::AddObject(TNamed *fxdata, TQRootCanvas* c,TPad* p)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4TofinoOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	if(obj && fxTGo4TofinoOnline)
	{
	    c->getCanvas()->cd();
            ((HMonHistAddon*)obj)->setCanvas(c->getCanvas());
	    ((HMonHistAddon*)obj)->draw();
	    c->getCanvas()->Update();
	}

	//if(fxTGo4TofinoOnline!=0)  UpdateGUI(obj);
}	
void TGo4TofinoOnlineSlots::UpdateGUI(TObject *Obj)
{
  TGo4LockGuard Global;

  if(Obj!=0){
      if (Obj->InheritsFrom("HMonHistAddon")) {
	  HMonHistAddon* hist = (HMonHistAddon*) Obj;
	  fxTGo4TofinoOnline->DrawHis(hist);
      }
  }
}

ClassImp(TGo4TofinoOnlineSlots)
