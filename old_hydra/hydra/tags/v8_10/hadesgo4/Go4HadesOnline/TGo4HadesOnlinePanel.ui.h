/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

#include "Go4GUIRegistry/TGo4GUIRegistry.h"
#include "Go4LockGuard/TGo4LockGuard.h"

void TGo4HadesOnlinePanel::destroy()
{
 TGo4LockGuard glob;
}

void TGo4HadesOnlinePanel::init()
{
 TGo4LockGuard glob;
    /*
      *  Set  the Name (Caption) of the UserPanel 
      */
      Char_t *libFiles[4] = {
      "$HADDIR_BASE_LIB/libHydra.so",
      "$HADDIR_RICH_LIB/libRich.so",
      "$HADDIR_HADESGO4_LIB/libHadesGo4.so",
       "$HADDIR_HADESGO4/Go4Library/libGo4HadesClient.so",
 };

  printf("\nrootlogon\nloading shared libraries...\n\n");
  for(Int_t i=0;i<4;i++) 
  {
      char *path = gSystem->ExpandPathName(libFiles[i]);
      if (gSystem->Load(path)==0) printf("Library %s loaded\n",path);
      else printf("Unable to load %s\n",path);
      path =NULL;
  }
 
 fxTGo4HadesOnline=0;
 fxTGo4HadesOnlineSlots=0;
 fxTGo4TriggerOnline=0;
 fxTGo4TriggerOnlineSlots=0;
 fxTGo4StartOnline=0;
 fxTGo4StartOnlineSlots=0;
 fxTGo4MdcOnline=0;
 fxTGo4MdcOnlineSlots=0;
 fxTGo4TofOnline=0;
 fxTGo4TofOnlineSlots=0;
 fxTGo4TofinoOnline=0;
 fxTGo4TofinoOnlineSlots=0;
 fxTGo4HodoOnline=0;
 fxTGo4HodoOnlineSlots=0;
 fxTGo4ShowerOnline=0;
 fxTGo4ShowerOnlineSlots=0;
 fxTGo4PhysicsOnline=0;
 fxTGo4PhysicsOnlineSlots=0;
 fxTGo4HadesConfig=0;
 fxTGo4HadesConfigSlots=0;
  
  setCaption( trUtf8( "HadesMain" ) ); 
  fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
  fxTGo4HadesOnlinePanelSlots = dynamic_cast <TGo4HadesOnlinePanelSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4HadesOnlinePanelSlots"));   
    
    /* test if  a Slot Object already exist for this GUI*/
    
   if(fxTGo4HadesOnlinePanelSlots  !=0) {
   /* if it is already there, set this GUI  as window of the slot Object */
    fxTGo4HadesOnlinePanelSlots->SetGUI(this);
   }else{
   /* if no Slot Object where found creat a new one*/
   fxTGo4HadesOnlinePanelSlots = new TGo4HadesOnlinePanelSlots ("TGo4HadesOnlinePanelSlots" , "Slot Class for Qt GUI", this);
   }
   /* Get the pointer for the Status Object */
    fxTGo4HadesOnlinePanelStatus= dynamic_cast <TGo4HadesOnlinePanelStatus *> (fxTGo4HadesOnlinePanelSlots->GetStatus());
 
    //########################### REGISTER HISTS ###################################
    fxTGo4GUIRegistry->ReDirectObject("HadesParameter",fxTGo4HadesOnlinePanelSlots, true);
 
    
    
}

void TGo4HadesOnlinePanel::openConfig()
{
    TGo4LockGuard glob;
    
    fxTGo4HadesConfigSlots =(TGo4HadesConfigSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4HadesConfigSlots");
	if (!fxTGo4HadesConfigSlots){
	    fxTGo4HadesConfig= new TGo4HadesConfig(); 
	    fxTGo4HadesConfig->show();
	}else{
	    fxTGo4HadesConfig= fxTGo4HadesConfigSlots->GetGUI();
		if (fxTGo4HadesConfig!=0){
		    fxTGo4HadesConfig->show();
		}else{
		    fxTGo4HadesConfig =new  TGo4HadesConfig(); 
		    fxTGo4HadesConfig->show();
		}
	    
	}
	
    //this will creat the new widget as a top level widget
}


void TGo4HadesOnlinePanel::openMain()
{
	TGo4LockGuard glob;
	fxTGo4HadesOnlineSlots =(TGo4HadesOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4HadesOnlineSlots");
	if (!fxTGo4HadesOnlineSlots){
	  //	TGo4HadesOnline *fxTGo4HadesOnline =new  TGo4HadesOnline(fxTGo4GUIRegistry->GetCentralWindow()); 
	  fxTGo4HadesOnline =new  TGo4HadesOnline(); 
	  fxTGo4HadesOnline->show();
	}else{
		fxTGo4HadesOnline= fxTGo4HadesOnlineSlots->GetGUI();
		if (fxTGo4HadesOnline!=0){
		    fxTGo4HadesOnline->show();
		}else{
		    fxTGo4HadesOnline =new  TGo4HadesOnline(); 
		    fxTGo4HadesOnline->show();
		}
	}
}

void TGo4HadesOnlinePanel::openMdc()
{
	TGo4LockGuard glob;
	fxTGo4MdcOnlineSlots =(TGo4MdcOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4MdcOnlineSlots");
	if (!fxTGo4MdcOnlineSlots){
	fxTGo4MdcOnline =new  TGo4MdcOnline(); 
	  fxTGo4MdcOnline->show();
	}else{
	    fxTGo4MdcOnline= fxTGo4MdcOnlineSlots->GetGUI();
		if (fxTGo4MdcOnline!=0){
		    fxTGo4MdcOnline->show();
		}else{
		    fxTGo4MdcOnline =new  TGo4MdcOnline(); 
		    fxTGo4MdcOnline->show();
		}
	}
}

void TGo4HadesOnlinePanel::StartMonitor()
{
    fxTGo4HadesOnlinePanelSlots->StartMonitor();
}

void TGo4HadesOnlinePanel::StopMonitor()
{
  fxTGo4HadesOnlinePanelSlots->StopMonitor();
}

void TGo4HadesOnlinePanel::ResetMonitor()
{
 	TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
	reset->Set("All",0);     
	TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
	com->SetParameter(reset);
	fxTGo4GUIRegistry->SubmitCommand(com);
}


void TGo4HadesOnlinePanel::openStart()
{
	TGo4LockGuard glob;
	fxTGo4StartOnlineSlots =(TGo4StartOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4StartOnlineSlots");
	if (!fxTGo4StartOnlineSlots){
	fxTGo4StartOnline =new  TGo4StartOnline(); 
	  fxTGo4StartOnline->show();
	}else{
	    fxTGo4StartOnline= fxTGo4StartOnlineSlots->GetGUI();
		if (fxTGo4StartOnline!=0){
		    fxTGo4StartOnline->show();
		}else{
		    fxTGo4StartOnline =new  TGo4StartOnline(); 
		    fxTGo4StartOnline->show();
		}
	}
}

void TGo4HadesOnlinePanel::openRich()
{
    TGo4LockGuard glob;
    fxTGo4RichOnlineSlots =(TGo4RichOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4RichOnlineSlots");
    if (!fxTGo4RichOnlineSlots){
	fxTGo4RichOnline =new  TGo4RichOnline();
	fxTGo4RichOnline->show();
    }else{
	fxTGo4RichOnline= fxTGo4RichOnlineSlots->GetGUI();
	if (fxTGo4RichOnline!=0){
	    fxTGo4RichOnline->show();
	}else{
	    fxTGo4RichOnline =new  TGo4RichOnline();
	    fxTGo4RichOnline->show();
	}
    }

}

void TGo4HadesOnlinePanel::openTof()
{
    TGo4LockGuard glob;
    fxTGo4TofOnlineSlots =(TGo4TofOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4TofOnlineSlots");
    if (!fxTGo4TofOnlineSlots){
	fxTGo4TofOnline =new  TGo4TofOnline();
	fxTGo4TofOnline->show();
    }else{
	fxTGo4TofOnline= fxTGo4TofOnlineSlots->GetGUI();
	if (fxTGo4TofOnline!=0){
	    fxTGo4TofOnline->show();
	}else{
	    fxTGo4TofOnline =new  TGo4TofOnline();
	    fxTGo4TofOnline->show();
	}
    }

}

void TGo4HadesOnlinePanel::openTofino()
{
    TGo4LockGuard glob;
    fxTGo4TofinoOnlineSlots =(TGo4TofinoOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4TofinoOnlineSlots");
    if (!fxTGo4TofinoOnlineSlots){
	fxTGo4TofinoOnline =new  TGo4TofinoOnline();
	fxTGo4TofinoOnline->show();
    }else{
	fxTGo4TofinoOnline= fxTGo4TofinoOnlineSlots->GetGUI();
	if (fxTGo4TofinoOnline!=0){
	    fxTGo4TofinoOnline->show();
	}else{
	    fxTGo4TofinoOnline =new  TGo4TofinoOnline();
	    fxTGo4TofinoOnline->show();
	}
    }

}

void TGo4HadesOnlinePanel::openShower()
{
    TGo4LockGuard glob;
    fxTGo4ShowerOnlineSlots =(TGo4ShowerOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4ShowerOnlineSlots");
    if (!fxTGo4ShowerOnlineSlots){
	fxTGo4ShowerOnline =new  TGo4ShowerOnline();
	fxTGo4ShowerOnline->show();
    }else{
	fxTGo4ShowerOnline= fxTGo4ShowerOnlineSlots->GetGUI();
	if (fxTGo4ShowerOnline!=0){
	    fxTGo4ShowerOnline->show();
	}else{
	    fxTGo4ShowerOnline =new  TGo4ShowerOnline();
	    fxTGo4ShowerOnline->show();
	}
    }
}

void TGo4HadesOnlinePanel::openPhysics()
{
	TGo4LockGuard glob;
	fxTGo4PhysicsOnlineSlots =(TGo4PhysicsOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4PhysicsOnlineSlots");
	if (!fxTGo4PhysicsOnlineSlots){
	fxTGo4PhysicsOnline =new  TGo4PhysicsOnline(); 
	  fxTGo4PhysicsOnline->show();
	}else{
	    fxTGo4PhysicsOnline= fxTGo4PhysicsOnlineSlots->GetGUI();
		if (fxTGo4PhysicsOnline!=0){
		    fxTGo4PhysicsOnline->show();
		}else{
		    fxTGo4PhysicsOnline =new  TGo4PhysicsOnline(); 
		    fxTGo4PhysicsOnline->show();
		}
	}
}

void TGo4HadesOnlinePanel::openTrigger()
{
        TGo4LockGuard glob;
	fxTGo4TriggerOnlineSlots =(TGo4TriggerOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4TriggerOnlineSlots");
	if (!fxTGo4TriggerOnlineSlots){
	  fxTGo4TriggerOnline =new  TGo4TriggerOnline(); 
	  fxTGo4TriggerOnline->show();
	}else{
		fxTGo4TriggerOnline= fxTGo4TriggerOnlineSlots->GetGUI();
		if (fxTGo4TriggerOnline!=0){
		    fxTGo4TriggerOnline->show();
		}else{
		    fxTGo4TriggerOnline =new  TGo4TriggerOnline(); 
		    fxTGo4TriggerOnline->show();
		}
	}

}


void TGo4HadesOnlinePanel::DrawLCDEventRate( double rate )
{
    LCDEventRate->display (rate );
}

void TGo4HadesOnlinePanel::DrawHis( HMonHistAddon * His )
{
  
}

void TGo4HadesOnlinePanel::openHodo()
{
    TGo4LockGuard glob;
    fxTGo4HodoOnlineSlots =(TGo4HodoOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4HodoOnlineSlots");
    if (!fxTGo4HodoOnlineSlots){
        fxTGo4HodoOnline =new  TGo4HodoOnline();
        fxTGo4HodoOnline->show();
    }else{
        fxTGo4HodoOnline= fxTGo4HodoOnlineSlots->GetGUI();
        if (fxTGo4HodoOnline!=0){
            fxTGo4HodoOnline->show();
        }else{
            fxTGo4HodoOnline =new  TGo4HodoOnline();
            fxTGo4HodoOnline->show();
        }
    }

}
