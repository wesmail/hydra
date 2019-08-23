/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void TGo4ShowerOnline::init()
{


    setCaption(" Shower Go4 Online " );
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/
    
    fxTGo4ShowerOnlineSlots = dynamic_cast <TGo4ShowerOnlineSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4ShowerOnlineSlots"));   
    
    /* test if  a Slot Object already exist for this GUI*/
    
   if(fxTGo4ShowerOnlineSlots  !=0) {
   /* if it is already there, set this GUI  as window of the slot Object */
		fxTGo4ShowerOnlineSlots->SetGUI(this);
   }else{
   /* if no Slot Object where found creat a new one*/
		fxTGo4ShowerOnlineSlots = new TGo4ShowerOnlineSlots ("TGo4ShowerOnlineSlots" , "Slot Class for Qt GUI", this);
   }
   /* Get the pointer for the Status Object */
    fxTGo4ShowerOnlineStatus= dynamic_cast <TGo4ShowerOnlineStatus *> (fxTGo4ShowerOnlineSlots->GetStatus());
    
}

void TGo4ShowerOnline::destroy()
{

}

void TGo4ShowerOnline::closeEvent( QCloseEvent * ce )
{
   TGo4LockGuard glob;
    ce->accept();
    fxTGo4ShowerOnlineSlots->SetGUI(0);
    delete this;
}

void TGo4ShowerOnline::ShowerDisplayLCD1( float t )
{
    ShowerLCDNumber1->display(t);
}

void TGo4ShowerOnline::ShowerDisplayLCD2( float t )
{
 ShowerLCDNumber2->display(t);
}

void TGo4ShowerOnline::ShowerDisplayLCD3( float t )
{
    ShowerLCDNumber3->display(t);
}

void TGo4ShowerOnline::DrawHis( HMonHistAddon* His )
{
       //************************SHOWER**************************
     if ( strcmp(His->GetName(),"hShowerhits_raw") ==  0 )
    {
	ShowerfxQtRootCanvas6->getCanvas()->cd();
	His->setCanvas(ShowerfxQtRootCanvas6->getCanvas());
	((TH2F*)(((HMonHist2*)His)->getP()))->Draw("");
	ShowerfxQtRootCanvas6->getCanvas()->Update();
    }  
}

void TGo4ShowerOnline::Start()
{
   fxTGo4ShowerOnlineSlots->StartMonitor();
}

void TGo4ShowerOnline::Stop()
{
fxTGo4ShowerOnlineSlots->StopMonitor();
}

void TGo4ShowerOnline::Reset()
{
}



