/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4MdcOnlineSlots.h"
#include "TGo4MdcOnlineStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "TGo4MdcOnline.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hmdcconfig.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "TH2.h"
#include "TPad.h"
#include "tqrootcanvas.h"


TGo4MdcOnlineSlots::TGo4MdcOnlineSlots(const char * name, const char * title, TGo4MdcOnline * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4MdcOnline(p)
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4MdcOnlineStatus= new TGo4MdcOnlineStatus("TGo4MdcOnlineStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4MdcOnlineStatus, p);
	mdclookup =0;
}
void TGo4MdcOnlineSlots::StartMonitor()
{
  // do now init+start analysis here
}
void TGo4MdcOnlineSlots::StopMonitor()
{
}
void TGo4MdcOnlineSlots::Reset()
{
}
TGo4MdcOnlineSlots::~TGo4MdcOnlineSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4MdcOnlineStatus);
	delete fxTGo4MdcOnlineStatus;
        delete mdclookup;
}
void TGo4MdcOnlineSlots::SetGUI(TGo4MdcOnline *GUI)
{
	TGo4LockGuard Global;
	fxTGo4MdcOnline=GUI;

}
TGo4Status * TGo4MdcOnlineSlots::GetStatus()
{
	return fxTGo4MdcOnlineStatus;
}
void TGo4MdcOnlineSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)

	TObject* obj=fxTGo4MdcOnlineStatus->OverWriteLocalObject(fxdata);

	if(strcmp(((TNamed*)obj)->GetName(),"MdcLookup")==0)
	{
	    mdclookup=(HMdcConnectedTdc*)obj;
	}
	else
	{
	    /* As an example the incomming object is saved to the status object */
	    //cout << " Name AddObj : " << fxdata->GetName() << endl;
	    /* and action take place in the UpdateGUI*/

	    if(fxTGo4MdcOnline!=0)  UpdateGUI(obj);
	}
}	
void TGo4MdcOnlineSlots::AddObject(TNamed *fxdata, TQRootCanvas* c,TPad* p)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	TObject* obj=fxTGo4MdcOnlineStatus->OverWriteLocalObject(fxdata);
	/* As an example the incomming object is saved to the status object */
        //cout << " Name AddObj : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	if(obj && fxTGo4MdcOnline)
	{
	    c->getCanvas()->cd();
            ((HMonHistAddon*)obj)->setCanvas(c->getCanvas());
            ((HMonHistAddon*)obj)->draw();
	    c->getCanvas()->Update();
	}

	//if(fxTGo4MdcOnline!=0)  UpdateGUI(obj);
}	
void TGo4MdcOnlineSlots::UpdateGUI(TObject* Obj)
{
  TGo4LockGuard Global;

  if(Obj!=0){
    if (Obj->InheritsFrom("HMonHistAddon")) {
      HMonHistAddon* hist = (HMonHistAddon*) Obj;
      fxTGo4MdcOnline->DrawHis(hist);
    }else if( strcmp (Obj->GetName(),"MdcConfig") == 0 ) {
      HMdcConfig *fxHMdc= (HMdcConfig *) Obj;
      fxTGo4MdcOnline->MdcDisplayLCD1( fxHMdc->getEvtNb());
      fxTGo4MdcOnline->MdcDisplayLCD2( fxHMdc->getRawMult());
      fxTGo4MdcOnline->MdcDisplayLCD3( fxHMdc->getCalMult());
    }
  }
}

ClassImp(TGo4MdcOnlineSlots)
