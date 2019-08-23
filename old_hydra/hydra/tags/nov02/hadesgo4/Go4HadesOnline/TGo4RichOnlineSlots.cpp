/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4RichOnlineSlots.h"
#include "TGo4RichOnlineStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4RichOnline.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"
#include "TPad.h"
#include "tqrootcanvas.h"


TGo4RichOnlineSlots::TGo4RichOnlineSlots(const char * name, const char * title, TGo4RichOnline * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4RichOnline(p)
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4RichOnlineStatus= new TGo4RichOnlineStatus("TGo4RichOnlineStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4RichOnlineStatus, p);
}
void TGo4RichOnlineSlots::StartMonitor()
{

  // do now init+start analysis here
}
void TGo4RichOnlineSlots::StopMonitor()
{
}
void TGo4RichOnlineSlots::Reset()
{
}
TGo4RichOnlineSlots::~TGo4RichOnlineSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4RichOnlineStatus);
	delete fxTGo4RichOnlineStatus;
}
void TGo4RichOnlineSlots::SetGUI(TGo4RichOnline *GUI)
{
	TGo4LockGuard Global;
	fxTGo4RichOnline=GUI;

}
TGo4Status * TGo4RichOnlineSlots::GetStatus()
{
	return fxTGo4RichOnlineStatus;
}
void TGo4RichOnlineSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4RichOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	
	if(fxTGo4RichOnline!=0)  UpdateGUI(obj);
}	
void TGo4RichOnlineSlots::AddObject(TNamed *fxdata, TQRootCanvas* c,TPad* p)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4RichOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	if(obj && fxTGo4RichOnline)
	{
	    c->getCanvas()->cd();
            ((HMonHistAddon*)obj)->setCanvas(c->getCanvas());
	    ((HMonHistAddon*)obj)->draw();
	    c->getCanvas()->Update();
	}

	//if(fxTGo4StartOnline!=0)  UpdateGUI(obj);
}	
void TGo4RichOnlineSlots::UpdateGUI(TObject* Obj)
{
  TGo4LockGuard Global;

  if(Obj!=0){
      if (Obj->InheritsFrom("HMonHistAddon")) {
	  HMonHistAddon* hist = (HMonHistAddon*) Obj;
	  fxTGo4RichOnline->DrawHis(hist);
      }
  // if(strcmp(Obj->GetName(),"RichOnlineDisplay")==0)
//     {
//       cout << "Drawing online display once and for all!" << endl;
//       HRichFancy* richfancy = (HRichFancy*) Obj;
//       fxTGo4RichOnline->DrawFancy(richfancy);
//     }
//   if(strcmp(Obj->GetName(),"RichPattern")==0)
//     {
//       cout << "Updating online pattern" << endl;
//       HRichPattern* richpattern = (HRichPattern*) Obj;
//       fxTGo4RichOnline->DrawPattern(richpattern);
//     }
  }
}

ClassImp(TGo4RichOnlineSlots)
