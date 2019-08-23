/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4PhysicsOnlineSlots.h"
#include "TGo4PhysicsOnlineStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4PhysicsOnline.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
//#include "hphysicsconfig.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"
#include "TPad.h"
#include "tqrootcanvas.h"


TGo4PhysicsOnlineSlots::TGo4PhysicsOnlineSlots(const char * name, const char * title, TGo4PhysicsOnline * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4PhysicsOnline(p)
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4PhysicsOnlineStatus= new TGo4PhysicsOnlineStatus("TGo4PhysicsOnlineStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4PhysicsOnlineStatus, p);
}
void TGo4PhysicsOnlineSlots::StartMonitor()
{
  // do now init+start analysis here
}
void TGo4PhysicsOnlineSlots::StopMonitor()
{
}
void TGo4PhysicsOnlineSlots::Reset()
{
}
TGo4PhysicsOnlineSlots::~TGo4PhysicsOnlineSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4PhysicsOnlineStatus);
	delete fxTGo4PhysicsOnlineStatus;
}
void TGo4PhysicsOnlineSlots::SetGUI(TGo4PhysicsOnline *GUI)
{
	TGo4LockGuard Global;
	fxTGo4PhysicsOnline=GUI;
}
TGo4Status * TGo4PhysicsOnlineSlots::GetStatus()
{
	return fxTGo4PhysicsOnlineStatus;
}
void TGo4PhysicsOnlineSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)

	TObject* obj=fxTGo4PhysicsOnlineStatus->OverWriteLocalObject(fxdata);

	/* As an example the incomming object is saved to the status object */
	//cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/

	if(fxTGo4PhysicsOnline!=0)  UpdateGUI(obj);
}
void TGo4PhysicsOnlineSlots::AddObject(TNamed *fxdata, TQRootCanvas* c,TPad* p)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4PhysicsOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/

	if(obj && fxTGo4PhysicsOnline)
	{
	    c->getCanvas()->cd();
            ((HMonHistAddon*)obj)->setCanvas(c->getCanvas());
            ((HMonHistAddon*)obj)->draw();
	    c->getCanvas()->Update();
	}

	//if(fxTGo4PhysicsOnline!=0)  UpdateGUI(obj);
}	
void TGo4PhysicsOnlineSlots::UpdateGUI(TObject* Obj)
{
  TGo4LockGuard Global;

  if(Obj!=0){
    if (Obj->InheritsFrom("HMonHistAddon")) {
      HMonHistAddon* hist = (HMonHistAddon*) Obj;
      fxTGo4PhysicsOnline->DrawHis(hist);
    }
  }
}

ClassImp(TGo4PhysicsOnlineSlots)
