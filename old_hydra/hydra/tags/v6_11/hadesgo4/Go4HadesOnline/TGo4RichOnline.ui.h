/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void TGo4RichOnline::init()
{


    setCaption(" Rich Go4 Online " );
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/
    
    fxTGo4RichOnlineSlots = dynamic_cast <TGo4RichOnlineSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4RichOnlineSlots"));   
    
    /* test if  a Slot Object already exist for this GUI*/
    
   if(fxTGo4RichOnlineSlots  !=0) {
   /* if it is already there, set this GUI  as window of the slot Object */
		fxTGo4RichOnlineSlots->SetGUI(this);
   }else{
   /* if no Slot Object where found creat a new one*/
		fxTGo4RichOnlineSlots = new TGo4RichOnlineSlots ("TGo4RichOnlineSlots" , "Slot Class for Qt GUI", this);
   }
   /* Get the pointer for the Status Object */
    fxTGo4RichOnlineStatus= dynamic_cast <TGo4RichOnlineStatus *> (fxTGo4RichOnlineSlots->GetStatus());
    
}

void TGo4RichOnline::destroy()
{

}

void TGo4RichOnline::closeEvent( QCloseEvent * ce )
{
   TGo4LockGuard glob;
    ce->accept();
    fxTGo4RichOnlineSlots->SetGUI(0);
    delete this;
}

void TGo4RichOnline::RichDisplayLCD1( float t )
{
    RichLCDNumber1->display(t);
}

void TGo4RichOnline::RichDisplayLCD2( float t )
{
 RichLCDNumber2->display(t);
}

void TGo4RichOnline::RichDisplayLCD3( float t )
{
    RichLCDNumber3->display(t);
}

void TGo4RichOnline::DrawHis( HMonHistAddon* His )
{
    Int_t gridX,gridY;

    //************************RICH**************************
    
    if ( strcmp(His->GetName(),"hRichpulsheight") ==  0 )
    {
	
	RichfxQtRootCanvas2->getCanvas()->cd();
        His->setCanvas(RichfxQtRootCanvas2->getCanvas());
	((TH1F*)(((HHistArray*)His)->getP(0,0)))->Draw("");
	((TH1F*)(((HHistArray*)His)->getP(0,1)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,2)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,3)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,4)))->Draw("same");
	((TH1F*)(((HHistArray*)His)->getP(0,5)))->Draw("same");
	RichfxQtRootCanvas2->getCanvas()->Update();
    }

}

void TGo4RichOnline::Start()
{
   fxTGo4RichOnlineSlots->StartMonitor();
}

void TGo4RichOnline::Stop()
{
fxTGo4RichOnlineSlots->StopMonitor();
}

void TGo4RichOnline::Reset()
{
}



