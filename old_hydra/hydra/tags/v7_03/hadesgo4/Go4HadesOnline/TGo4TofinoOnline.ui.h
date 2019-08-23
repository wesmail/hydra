/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void TGo4TofinoOnline::init()
{


    setCaption(" Tofino Go4 Online " );
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/
    
    fxTGo4TofinoOnlineSlots = dynamic_cast <TGo4TofinoOnlineSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4TofinoOnlineSlots"));   
    
    /* test if  a Slot Object already exist for this GUI*/
    
   if(fxTGo4TofinoOnlineSlots  !=0) {
   /* if it is already there, set this GUI  as window of the slot Object */
		fxTGo4TofinoOnlineSlots->SetGUI(this);
   }else{
   /* if no Slot Object where found creat a new one*/
		fxTGo4TofinoOnlineSlots = new TGo4TofinoOnlineSlots ("TGo4TofinoOnlineSlots" , "Slot Class for Qt GUI", this);
   }
   /* Get the pointer for the Status Object */
    fxTGo4TofinoOnlineStatus= dynamic_cast <TGo4TofinoOnlineStatus *> (fxTGo4TofinoOnlineSlots->GetStatus());
 
    
    //########################### REGISTER HISTS ###################################
    fxTGo4GUIRegistry->ReDirectObject("hTofinohits_raw",fxTGo4TofinoOnlineSlots, true,TofinofxQtRootCanvas1);
    fxTGo4GUIRegistry->ReDirectObject("hTofinoprof",fxTGo4TofinoOnlineSlots, true,TofinofxQtRootCanvas2);
    fxTGo4GUIRegistry->ReDirectObject("hTofinoMultTdcAll",fxTGo4TofinoOnlineSlots, true,TofinofxQtRootCanvas3);
    fxTGo4GUIRegistry->ReDirectObject("hTofinoTDC",fxTGo4TofinoOnlineSlots, true,TofinofxQtRootCanvas4);
    fxTGo4GUIRegistry->ReDirectObject("hTofinoADC",fxTGo4TofinoOnlineSlots, true,TofinofxQtRootCanvas5);
    fxTGo4GUIRegistry->ReDirectObject("hTofinoShower",fxTGo4TofinoOnlineSlots, true,TofinofxQtRootCanvas6);
 }

void TGo4TofinoOnline::destroy()
{
    //########################### UNREGISTER HISTS ###################################
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinohits_raw",fxTGo4TofinoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinoprof",fxTGo4TofinoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinoMultTdcAll",fxTGo4TofinoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinoTDC",fxTGo4TofinoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinoADC",fxTGo4TofinoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinoShower",fxTGo4TofinoOnlineSlots);
}

void TGo4TofinoOnline::closeEvent( QCloseEvent * ce )
{
   TGo4LockGuard glob;
    ce->accept();
    fxTGo4TofinoOnlineSlots->SetGUI(0);
}

void TGo4TofinoOnline::TofinoDisplayLCD1( float t )
{
    TofinoLCDNumber1->display(t);
}

void TGo4TofinoOnline::TofinoDisplayLCD2( float t )
{
 TofinoLCDNumber2->display(t);
}

void TGo4TofinoOnline::TofinoDisplayLCD3( float t )
{
    TofinoLCDNumber3->display(t);
}

void TGo4TofinoOnline::DrawHis( HMonHistAddon* His )
{
 
}

void TGo4TofinoOnline::Reset()
{
 	TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
	reset->SetTofinoChanged(kTRUE);
	reset->Set("Tofino",0);     
	TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
	com->SetParameter(reset);
	fxTGo4GUIRegistry->SubmitCommand(com);
}



