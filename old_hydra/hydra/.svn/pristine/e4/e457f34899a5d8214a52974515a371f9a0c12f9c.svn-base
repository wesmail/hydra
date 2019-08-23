/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void TGo4HadesConfig::init()
{
 fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/
    
    fxTGo4HadesConfigSlots = dynamic_cast <TGo4HadesConfigSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4HadesConfigSlots"));   
    
      if(fxTGo4HadesConfigSlots  !=0) {
   /* if it is already there, set this GUI  as window of the slot Object */
		fxTGo4HadesConfigSlots->SetGUI(this);
   }else{
   /* if no Slot Object where found creat a new one*/
		fxTGo4HadesConfigSlots = new TGo4HadesConfigSlots ("TGo4HadesConfigSlots" , "Slot Class for Qt GUI", this);
   }
   /* Get the pointer for the Status Object */
    fxTGo4HadesConfigStatus= dynamic_cast <TGo4HadesConfigStatus *> (fxTGo4HadesConfigSlots->GetStatus());
    
    
    
}

void TGo4HadesConfig::destroy()
{

}


void TGo4HadesConfig::closeEvent( QCloseEvent *ce )
{
    TGo4LockGuard glob;
    ce->accept();
    fxTGo4HadesConfigSlots->SetGUI(0);
    delete this;
}


void TGo4HadesConfig::newTreeSize()
{

}
