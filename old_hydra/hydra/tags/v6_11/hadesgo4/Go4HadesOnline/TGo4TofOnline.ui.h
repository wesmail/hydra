/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void TGo4TofOnline::init()
{


    setCaption(" Tof Go4 Online " );
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/
    
    fxTGo4TofOnlineSlots = dynamic_cast <TGo4TofOnlineSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4TofOnlineSlots"));   
    
    /* test if  a Slot Object already exist for this GUI*/
    
   if(fxTGo4TofOnlineSlots  !=0) {
   /* if it is already there, set this GUI  as window of the slot Object */
		fxTGo4TofOnlineSlots->SetGUI(this);
   }else{
   /* if no Slot Object where found creat a new one*/
		fxTGo4TofOnlineSlots = new TGo4TofOnlineSlots ("TGo4TofOnlineSlots" , "Slot Class for Qt GUI", this);
   }
   /* Get the pointer for the Status Object */
    fxTGo4TofOnlineStatus= dynamic_cast <TGo4TofOnlineStatus *> (fxTGo4TofOnlineSlots->GetStatus());
    
}

void TGo4TofOnline::destroy()
{

}

void TGo4TofOnline::closeEvent( QCloseEvent * ce )
{
   TGo4LockGuard glob;
    ce->accept();
    fxTGo4TofOnlineSlots->SetGUI(0);
    delete this;
}

void TGo4TofOnline::TofDisplayLCD1( float t )
{
    TofLCDNumber1->display(t);
}

void TGo4TofOnline::TofDisplayLCD2( float t )
{
 TofLCDNumber2->display(t);
}

void TGo4TofOnline::TofDisplayLCD3( float t )
{
    TofLCDNumber3->display(t);
}

void TGo4TofOnline::DrawHis( HMonHistAddon* His )
{
    //************************TOF**************************

    if ( strcmp(His->GetName(),"hTofhits_raw") ==  0 )
    {
	ToffxQtRootCanvas6->getCanvas()->cd();
	His->setCanvas(ToffxQtRootCanvas6->getCanvas());
	((TH1F*)(((HHistArray*)His)->getP(0,0)))->Draw("");
	((TH1F*)(((HHistArray*)His)->getP(0,1)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,2)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,3)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,4)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,5)))->Draw("same");
	ToffxQtRootCanvas6->getCanvas()->Update();
    }
}

void TGo4TofOnline::Start()
{
   fxTGo4TofOnlineSlots->StartMonitor();
}

void TGo4TofOnline::Stop()
{
fxTGo4TofOnlineSlots->StopMonitor();
}

void TGo4TofOnline::Reset()
{
}



