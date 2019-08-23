/*
In this example the Aanalysis is responsible for sending object to the GUI, which means the methode 
SendObjectToGui has to be used on the analysis side to send object of type TNamed to the GUI

This example will take care that these object comming from analysis will go to the write place.

Mohammad Al-Turany	

*/
#include "Go4LockGuard/TGo4LockGuard.h"
#include "TGo4HadesConfig.h"
#include "TGo4HadesConfigSlots.h"
#include "TGo4HadesConfigStatus.h"
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "Go4CommandsAnalysis/Go4CommandsAnalysis.h"
#include "hmdcconfig.h"
#include "TH2.h"


TGo4HadesConfigSlots::TGo4HadesConfigSlots(const char * name, const char * title, TGo4HadesConfig * p)
	:TGo4SlotsBaseClass(name , title), fxTGo4HadesConfig(p) 
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
	fxTGo4HadesConfigStatus= new TGo4HadesConfigStatus("TGo4HadesConfigStatus", "Status"); /*creat a status object*/
   	/*register the three component (Slot,Status,Widget) */
	fxTGo4GUIRegistry->RegisterSlotClass(this, fxTGo4HadesConfigStatus, p);	
}

void TGo4HadesConfigSlots::Start()
{
    /* Create a Commmand (see Go4 Doc) to get an object by name*/

    TGo4ComGetObject* com = new TGo4ComGetObject("HadesMdcConfig");
    /* now submit this command to the registry:

    com: 	command to be submitted

    myname:	Name of Object to be delivered from analysis

    this:	the reciever of the object, a pointer of the type TGo4SlotBaseClass,
    this reciever has to reimplement the method(s) AddObject to manage the returned objects

    true:  this flage tells the Registry that this object will be send continuosly

    */
    fxTGo4GUIRegistry->SubmitCommand(com,"HadesMdcConfig",this);
 
    /*********************************************************************************/
}
TGo4HadesConfigSlots::~TGo4HadesConfigSlots()
{
	TGo4LockGuard Global;
	fxTGo4GUIRegistry->UnRegisterClass(this, fxTGo4HadesConfigStatus);
	delete fxTGo4HadesConfigStatus;
}
void TGo4HadesConfigSlots::SetGUI(TGo4HadesConfig *GUI)
{
	TGo4LockGuard Global;
	fxTGo4HadesConfig=GUI;

}
TGo4Status * TGo4HadesConfigSlots::GetStatus()
{
	return fxTGo4HadesConfigStatus;
}
void TGo4HadesConfigSlots::AddObject(TNamed *fxdata)
{	
	TGo4LockGuard Global;
	// This method will be called from the Registry to deliver any object which were 
	// requested from the analysis via SubmitCommand(Command, Name, reciver)
	
	fxTGo4HadesConfigStatus->SetActiveConfig(fxdata);
	/* As an example the incomming object is saved to the status object */
	// cout << " Name : " << fxdata->GetName() << endl;
	/* and action take place in the UpdateGUI*/
	
	if(fxTGo4HadesConfig!=0)  UpdateGUI(fxdata->GetName());
}	
void TGo4HadesConfigSlots::UpdateGUI(const char *ObjName)
{
  TGo4LockGuard Global;
  TNamed *HadesConfig = fxTGo4HadesConfigStatus->GetConfig();
}
ClassImp(TGo4HadesConfigSlots)
