/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4HadesOnlinePanelSlots.h"
#include "TGo4HadesOnlinePanelStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4HadesOnlinePanel.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hmdcconfig.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"


TGo4HadesOnlinePanelSlots::TGo4HadesOnlinePanelSlots(const char * name, const char * title, TGo4HadesOnlinePanel * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4HadesOnlinePanel(p) 
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4HadesOnlinePanelStatus= new TGo4HadesOnlinePanelStatus("TGo4HadesOnlinePanelStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4HadesOnlinePanelStatus, p);	
}
void TGo4HadesOnlinePanelSlots::StartMonitor()
{
  // do now init+start analysis here
  fxTGo4GUIRegistry->SubmitCommand("ANInit");
  fxTGo4GUIRegistry->SubmitCommand("THStart");

  // fxTGo4GUIRegistry->ReDirectObject("",this,true);

}
void TGo4HadesOnlinePanelSlots::StopMonitor()
{
   fxTGo4GUIRegistry->SubmitCommand("THStop");
}
void TGo4HadesOnlinePanelSlots::Reset()
{
}
TGo4HadesOnlinePanelSlots::~TGo4HadesOnlinePanelSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4HadesOnlinePanelStatus);
	delete fxTGo4HadesOnlinePanelStatus;
}
void TGo4HadesOnlinePanelSlots::SetGUI(TGo4HadesOnlinePanel *GUI)
{
	TGo4LockGuard Global;
	fxTGo4HadesOnlinePanel=GUI;

}
TGo4Status * TGo4HadesOnlinePanelSlots::GetStatus()
{
	return fxTGo4HadesOnlinePanelStatus;
}
void TGo4HadesOnlinePanelSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	fxTGo4HadesOnlinePanelStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	
	if(fxTGo4HadesOnlinePanel!=0)  UpdateGUI(fxdata->GetName());
}	
void TGo4HadesOnlinePanelSlots::UpdateGUI(const char *ObjName)
{
  TGo4LockGuard Global;

  TObject *Obj = fxTGo4HadesOnlinePanelStatus->GetObjectfromLocalList(ObjName);

  if(Obj!=0){
      if (Obj->InheritsFrom("HMonHistAddon")) {
      HMonHistAddon* hist = (HMonHistAddon*) Obj;
      //fxTGo4HadesOnlinePanel->DrawHis(hist);
    }
  }
}

ClassImp(TGo4HadesOnlinePanelSlots)
