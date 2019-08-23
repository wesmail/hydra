/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4HadesOnlineSlots.h"
#include "TGo4HadesOnlineStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4HadesOnline.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hmdcconfig.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"


TGo4HadesOnlineSlots::TGo4HadesOnlineSlots(const char * name, const char * title, TGo4HadesOnline * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4HadesOnline(p) 
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4HadesOnlineStatus= new TGo4HadesOnlineStatus("TGo4HadesOnlineStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4HadesOnlineStatus, p);	
}
void TGo4HadesOnlineSlots::StartMonitor()
{

    // do now init+start analysis here
  //--------------------------MDC-------------------------------
  fxTGo4GUIRegistry->ReDirectObject("hMdccutstatM",this,true);
  //-------------------------RICH------------------------------------
  fxTGo4GUIRegistry->ReDirectObject("hRichpulsheightM",this,true);
  //-------------------------Start------------------------------------
  fxTGo4GUIRegistry->ReDirectObject("hStartstrips_start_veto_rawM",this,true);
  //-------------------------Tof------------------------------------
  fxTGo4GUIRegistry->ReDirectObject("hTofhits_rawM",this,true);
  //-------------------------Tofino------------------------------------
  fxTGo4GUIRegistry->ReDirectObject("hTofinohits_rawM",this,true);
  //-------------------------Shower------------------------------------
  fxTGo4GUIRegistry->ReDirectObject("hShowerhits_rawM",this,true);

}
void TGo4HadesOnlineSlots::StopMonitor()
{
}
void TGo4HadesOnlineSlots::Reset()
{
}
TGo4HadesOnlineSlots::~TGo4HadesOnlineSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4HadesOnlineStatus);
	delete fxTGo4HadesOnlineStatus;
}
void TGo4HadesOnlineSlots::SetGUI(TGo4HadesOnline *GUI)
{
	TGo4LockGuard Global;
	fxTGo4HadesOnline=GUI;

}
TGo4Status * TGo4HadesOnlineSlots::GetStatus()
{
	return fxTGo4HadesOnlineStatus;
}
void TGo4HadesOnlineSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	fxTGo4HadesOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	
	if(fxTGo4HadesOnline!=0)  UpdateGUI(fxdata->GetName());
}	
void TGo4HadesOnlineSlots::UpdateGUI(const char *ObjName)
{
  TGo4LockGuard Global;

  TObject *Obj = fxTGo4HadesOnlineStatus->GetObjectfromLocalList(ObjName);

  if(Obj!=0){
    if(Obj->InheritsFrom("TH2")) {
      TH2* hist = (TH2*) Obj;
      fxTGo4HadesOnline->DrawHis(hist);
    }else if (Obj->InheritsFrom("TH1")) { 
      TH1* hist = (TH1*) Obj;
      fxTGo4HadesOnline->DrawHis(hist);
    }else if (Obj->InheritsFrom("HMonHistAddon")) {
      HMonHistAddon* hist = (HMonHistAddon*) Obj;
      fxTGo4HadesOnline->DrawHis(hist);
    }
  }
}

ClassImp(TGo4HadesOnlineSlots)
