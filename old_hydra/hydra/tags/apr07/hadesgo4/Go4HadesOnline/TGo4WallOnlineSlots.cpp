/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4WallOnlineSlots.h"
#include "TGo4WallOnlineStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4WallOnline.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"
#include "TPad.h"
#include "tqrootcanvas.h"


TGo4WallOnlineSlots::TGo4WallOnlineSlots(const char * name, const char * title, TGo4WallOnline * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4WallOnline(p)
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4WallOnlineStatus= new TGo4WallOnlineStatus("TGo4WallOnlineStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4WallOnlineStatus, p);
}
void TGo4WallOnlineSlots::StartMonitor()
{
  // do now init+start analysis here
}
void TGo4WallOnlineSlots::StopMonitor()
{
}
void TGo4WallOnlineSlots::Reset()
{
}
TGo4WallOnlineSlots::~TGo4WallOnlineSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4WallOnlineStatus);
	delete fxTGo4WallOnlineStatus;
}
void TGo4WallOnlineSlots::SetGUI(TGo4WallOnline *GUI)
{
	TGo4LockGuard Global;
	fxTGo4WallOnline=GUI;

}
TGo4Status * TGo4WallOnlineSlots::GetStatus()
{
	return fxTGo4WallOnlineStatus;
}
void TGo4WallOnlineSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4WallOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	
	if(fxTGo4WallOnline!=0)  UpdateGUI(obj);
}	
void TGo4WallOnlineSlots::AddObject(TNamed *fxdata, TQRootCanvas* c,TPad* p)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4WallOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	if(obj && fxTGo4WallOnline)
	{
	    c->getCanvas()->cd();
            ((HMonHistAddon*)obj)->setCanvas(c->getCanvas());
	    ((HMonHistAddon*)obj)->draw();
	    c->getCanvas()->Update();
	}

	//if(fxTGo4WallOnline!=0)  UpdateGUI(obj);
}	
void TGo4WallOnlineSlots::UpdateGUI(TObject *Obj)
{
  TGo4LockGuard Global;

  if(Obj!=0){
      if (Obj->InheritsFrom("HMonHistAddon")) {
	  HMonHistAddon* hist = (HMonHistAddon*) Obj;
	  fxTGo4WallOnline->DrawHis(hist);
      }
  }
}

ClassImp(TGo4WallOnlineSlots)
