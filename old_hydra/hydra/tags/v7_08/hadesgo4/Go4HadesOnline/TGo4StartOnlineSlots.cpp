/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4StartOnlineSlots.h"
#include "TGo4StartOnlineStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4StartOnline.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"
#include "TPad.h"
#include "tqrootcanvas.h"


TGo4StartOnlineSlots::TGo4StartOnlineSlots(const char * name, const char * title, TGo4StartOnline * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4StartOnline(p)
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4StartOnlineStatus= new TGo4StartOnlineStatus("TGo4StartOnlineStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4StartOnlineStatus, p);
        legenddrawer=new HLegendDrawer("mdcdraw","mdcdraw");
}
void TGo4StartOnlineSlots::StartMonitor()
{
  // do now init+start analysis here
}
void TGo4StartOnlineSlots::StopMonitor()
{
}
void TGo4StartOnlineSlots::Reset()
{
}
TGo4StartOnlineSlots::~TGo4StartOnlineSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4StartOnlineStatus);
	delete fxTGo4StartOnlineStatus;
}
void TGo4StartOnlineSlots::SetGUI(TGo4StartOnline *GUI)
{
	TGo4LockGuard Global;
	fxTGo4StartOnline=GUI;

}
TGo4Status * TGo4StartOnlineSlots::GetStatus()
{
	return fxTGo4StartOnlineStatus;
}
void TGo4StartOnlineSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4StartOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	
	if(fxTGo4StartOnline!=0)  UpdateGUI(obj);
}	
void TGo4StartOnlineSlots::AddObject(TNamed *fxdata, TQRootCanvas* c,TPad* p)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4StartOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	if(obj && fxTGo4StartOnline)
	{
	    c->getCanvas()->cd();
            ((HMonHistAddon*)obj)->setCanvas(c->getCanvas());
	    ((HMonHistAddon*)obj)->draw();
            //c->getCanvas()->Update();
	    //legenddrawer->draw((HMonHistAddon*)obj);
	    c->getCanvas()->Update();
	}

	//if(fxTGo4StartOnline!=0)  UpdateGUI(obj);
}	
void TGo4StartOnlineSlots::UpdateGUI(TObject* Obj)
{
  TGo4LockGuard Global;

  if(Obj!=0){
      if (Obj->InheritsFrom("HMonHistAddon")) {
	  HMonHistAddon* hist = (HMonHistAddon*) Obj;
	  fxTGo4StartOnline->DrawHis(hist);
      }
  }
}

ClassImp(TGo4StartOnlineSlots)
