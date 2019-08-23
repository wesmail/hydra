/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4TofOnlineSlots.h"
#include "TGo4TofOnlineStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4TofOnline.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"
#include "TPad.h"
#include "tqrootcanvas.h"


TGo4TofOnlineSlots::TGo4TofOnlineSlots(const char * name, const char * title, TGo4TofOnline * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4TofOnline(p)
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4TofOnlineStatus= new TGo4TofOnlineStatus("TGo4TofOnlineStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4TofOnlineStatus, p);
}
void TGo4TofOnlineSlots::StartMonitor()
{
  // do now init+start analysis here
}
void TGo4TofOnlineSlots::StopMonitor()
{
}
void TGo4TofOnlineSlots::Reset()
{
}
TGo4TofOnlineSlots::~TGo4TofOnlineSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4TofOnlineStatus);
	delete fxTGo4TofOnlineStatus;
}
void TGo4TofOnlineSlots::SetGUI(TGo4TofOnline *GUI)
{
	TGo4LockGuard Global;
	fxTGo4TofOnline=GUI;

}
TGo4Status * TGo4TofOnlineSlots::GetStatus()
{
	return fxTGo4TofOnlineStatus;
}
void TGo4TofOnlineSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4TofOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	
	if(fxTGo4TofOnline!=0)  UpdateGUI(obj);
}	
void TGo4TofOnlineSlots::AddObject(TNamed *fxdata, TQRootCanvas* c,TPad* p)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4TofOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	if(obj && fxTGo4TofOnline)
	{
	    c->getCanvas()->cd();
            ((HMonHistAddon*)obj)->setCanvas(c->getCanvas());
	    ((HMonHistAddon*)obj)->draw();
	    c->getCanvas()->Update();
	}

	//if(fxTGo4TofOnline!=0)  UpdateGUI(obj);
}	
void TGo4TofOnlineSlots::UpdateGUI(TObject* Obj)
{
  TGo4LockGuard Global;

  if(Obj!=0){
      if (Obj->InheritsFrom("HMonHistAddon")) {
	  HMonHistAddon* hist = (HMonHistAddon*) Obj;
	  fxTGo4TofOnline->DrawHis(hist);
      }
  }
}

ClassImp(TGo4TofOnlineSlots)
