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
    
}

void TGo4TofinoOnline::destroy()
{

}

void TGo4TofinoOnline::closeEvent( QCloseEvent * ce )
{
   TGo4LockGuard glob;
    ce->accept();
    fxTGo4TofinoOnlineSlots->SetGUI(0);
    delete this;
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
    if ( strcmp(His->GetName(),"hTofinohits_raw") ==  0 )
    {
	TofinofxQtRootCanvas6->getCanvas()->cd();
	His->setCanvas(TofinofxQtRootCanvas6->getCanvas());
	((TH1F*)(((HHistArray*)His)->getP(0,0)))->Draw("");
	((TH1F*)(((HHistArray*)His)->getP(0,1)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,2)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,3)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,4)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,5)))->Draw("same");
	TofinofxQtRootCanvas6->getCanvas()->Update();
    }
}

void TGo4TofinoOnline::Start()
{
   fxTGo4TofinoOnlineSlots->StartMonitor();
}

void TGo4TofinoOnline::Stop()
{
fxTGo4TofinoOnlineSlots->StopMonitor();
}

void TGo4TofinoOnline::Reset()
{
}



