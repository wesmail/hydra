/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/
#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "Go4LockGuard/TGo4LockGuard.h"
#include "Go4HadesOnline/TGo4HadesOnlinePanelSlots.h"
#include "Go4HadesOnline/TGo4HadesOnlinePanel.h"
#include <iostream>
using namespace std;

extern "C" void* StartUserGui(const char* path, void* parent)
{

   cout << "Before panel is created" << endl;
   
   // return pointer on created user top widget
   return new TGo4MainUserPanel((QWidget*) parent);
   
   cout << "Here is done " << endl;
}



void TGo4MainUserPanel::destroy()
{
	TGo4LockGuard glob;
}

void TGo4MainUserPanel::init()
{
    TGo4LockGuard glob;
    setCaption( trUtf8( "Go4Hades Monitor" ) ); 
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
}


void TGo4MainUserPanel::openHadesMon()
{
	TGo4LockGuard glob;
	TGo4HadesOnlinePanelSlots * fxTGo4HadesOnlinePanelSlots =(TGo4HadesOnlinePanelSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4HadesOnlinePanelSlots");
	if (!fxTGo4HadesOnlinePanelSlots){
	    TGo4HadesOnlinePanel* fxTGo4HadesOnlinePanel =new  TGo4HadesOnlinePanel(); 
	  fxTGo4HadesOnlinePanel->show();
	}else{
		TGo4HadesOnlinePanel *fxTGo4HadesOnlinePanel= fxTGo4HadesOnlinePanelSlots->GetGUI();
		if (fxTGo4HadesOnlinePanel!=0){
		    fxTGo4HadesOnlinePanel->show();
		}else{
		    fxTGo4HadesOnlinePanel =new  TGo4HadesOnlinePanel(); 
		    fxTGo4HadesOnlinePanel->show();
		}
	}
}
