/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4ShowerOnlineSlots.h"
#include "TGo4ShowerOnlineStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4ShowerOnline.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hshowerconfig.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"


TGo4ShowerOnlineSlots::TGo4ShowerOnlineSlots(const char * name, const char * title, TGo4ShowerOnline * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4ShowerOnline(p)
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4ShowerOnlineStatus= new TGo4ShowerOnlineStatus("TGo4ShowerOnlineStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4ShowerOnlineStatus, p);
}
void TGo4ShowerOnlineSlots::StartMonitor()
{
  // do now init+start analysis here
  //-----------------------Shower----------------------------------
  fxTGo4GUIRegistry->ReDirectObject("hShowerhits_raw",this,true);
}
void TGo4ShowerOnlineSlots::StopMonitor()
{
}
void TGo4ShowerOnlineSlots::Reset()
{
}
TGo4ShowerOnlineSlots::~TGo4ShowerOnlineSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4ShowerOnlineStatus);
	delete fxTGo4ShowerOnlineStatus;
}
void TGo4ShowerOnlineSlots::SetGUI(TGo4ShowerOnline *GUI)
{
	TGo4LockGuard Global;
	fxTGo4ShowerOnline=GUI;

}
TGo4Status * TGo4ShowerOnlineSlots::GetStatus()
{
	return fxTGo4ShowerOnlineStatus;
}
void TGo4ShowerOnlineSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	fxTGo4ShowerOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	
	if(fxTGo4ShowerOnline!=0)  UpdateGUI(fxdata->GetName());
}	
void TGo4ShowerOnlineSlots::UpdateGUI(const char *ObjName)
{
  TGo4LockGuard Global;

  TObject *Obj = fxTGo4ShowerOnlineStatus->GetObjectfromLocalList(ObjName);

  if(Obj!=0){
    if (Obj->InheritsFrom("HMonHistAddon")) {
      HMonHistAddon* hist = (HMonHistAddon*) Obj;
      fxTGo4ShowerOnline->DrawHis(hist);
    }else if( strcmp (ObjName,"ShowerConfig") == 0 ) {
      HShowerConfig *fxHShower= (HShowerConfig *) Obj;
      fxTGo4ShowerOnline->ShowerDisplayLCD1( fxHShower->getEvtNb());
      fxTGo4ShowerOnline->ShowerDisplayLCD2( fxHShower->getRawMult());
      fxTGo4ShowerOnline->ShowerDisplayLCD3( fxHShower->getCalMult());
    }
  }
}

ClassImp(TGo4ShowerOnlineSlots)
