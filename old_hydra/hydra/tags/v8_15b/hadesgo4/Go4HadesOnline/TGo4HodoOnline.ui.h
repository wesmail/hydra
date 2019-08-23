/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void TGo4HodoOnline::init()
{


    setCaption(" Hodo Go4 Online " );
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/

    fxTGo4HodoOnlineSlots = dynamic_cast <TGo4HodoOnlineSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4HodoOnlineSlots"));

    /* test if  a Slot Object already exist for this GUI*/

   if(fxTGo4HodoOnlineSlots  !=0) {
   /* if it is already there, set this GUI  as window of the slot Object */
		fxTGo4HodoOnlineSlots->SetGUI(this);
   }else{
   /* if no Slot Object where found creat a new one*/
		fxTGo4HodoOnlineSlots = new TGo4HodoOnlineSlots ("TGo4HodoOnlineSlots" , "Slot Class for Qt GUI", this);
   }
   /* Get the pointer for the Status Object */
    fxTGo4HodoOnlineStatus= dynamic_cast <TGo4HodoOnlineStatus *> (fxTGo4HodoOnlineSlots->GetStatus());


    //########################### REGISTER HISTS ###################################
    fxTGo4GUIRegistry->ReDirectObject("hHodoProf1",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasPF1);
    fxTGo4GUIRegistry->ReDirectObject("hHodoProf2",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasPF2);
    fxTGo4GUIRegistry->ReDirectObject("hHodoProf3",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasPF3);
    fxTGo4GUIRegistry->ReDirectObject("hHodoProf4",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasPF4);
    fxTGo4GUIRegistry->ReDirectObject("hHodoRawTime1",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasRT1);
    fxTGo4GUIRegistry->ReDirectObject("hHodoRawTime2",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasRT2);
    fxTGo4GUIRegistry->ReDirectObject("hHodoRawTime3",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasRT3);
    fxTGo4GUIRegistry->ReDirectObject("hHodoRawTime4",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasRT4);
    fxTGo4GUIRegistry->ReDirectObject("hHodoRawWidth1",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasRW1);
    fxTGo4GUIRegistry->ReDirectObject("hHodoRawWidth2",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasRW2);
    fxTGo4GUIRegistry->ReDirectObject("hHodoRawWidth3",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasRW3);
    fxTGo4GUIRegistry->ReDirectObject("hHodoRawWidth4",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasRW4);
    fxTGo4GUIRegistry->ReDirectObject("hHodoCrT1",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasCR1);
    fxTGo4GUIRegistry->ReDirectObject("hHodoCrT2",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasCR2);
    fxTGo4GUIRegistry->ReDirectObject("hHodoCrT3",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasCR3);
    fxTGo4GUIRegistry->ReDirectObject("hHodoCrT4",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasCR4);
    fxTGo4GUIRegistry->ReDirectObject("hHodoFib1Mult",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasFM1);
    fxTGo4GUIRegistry->ReDirectObject("hHodoFib2Mult",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasFM2);
    fxTGo4GUIRegistry->ReDirectObject("hHodoFib3Mult",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasFM3);
    fxTGo4GUIRegistry->ReDirectObject("hHodoFib4Mult",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvasFM4);

    fxTGo4GUIRegistry->ReDirectObject("hHodoSpot",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvas3);
//    fxTGo4GUIRegistry->ReDirectObject("hHodoTDCMult",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvas8);
    fxTGo4GUIRegistry->ReDirectObject("hEfficiency",fxTGo4HodoOnlineSlots, true,HodofxQtRootCanvas8);
}

void TGo4HodoOnline::destroy()
{
    //########################### UNREGISTER HISTS ###################################
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoProf1",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoProf2",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoProf3",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoProf4",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoRawTime1",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoRawTime2",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoRawTime3",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoRawTime4",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoRawWidth1",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoRawWidth2",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoRawWidth3",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoRawWidth4",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoCrT1",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoCrT2",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoCrT3",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoCrT4",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoFib1Mult",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoFib2Mult",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoFib3Mult",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoFib4Mult",fxTGo4HodoOnlineSlots);
    
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoSpot",fxTGo4HodoOnlineSlots);
//	 fxTGo4GUIRegistry->RemoveRedirectEntry("hHodoTDCMult",fxTGo4HodoOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hEfficiency",fxTGo4HodoOnlineSlots);
}

void TGo4HodoOnline::closeEvent( QCloseEvent * ce )
{
   TGo4LockGuard glob;
    ce->accept();
    fxTGo4HodoOnlineSlots->SetGUI(0);
}

void TGo4HodoOnline::HodoDisplayLCD1( float t )
{
    HodoLCDNumber1->display(t);
}

void TGo4HodoOnline::HodoDisplayLCD2( float t )
{
 HodoLCDNumber2->display(t);
}

void TGo4HodoOnline::HodoDisplayLCD3( float t )
{
    HodoLCDNumber3->display(t);
}

void TGo4HodoOnline::DrawHis( HMonHistAddon* His )
{

}

void TGo4HodoOnline::Start()
{
   fxTGo4HodoOnlineSlots->StartMonitor();
}

void TGo4HodoOnline::Stop()
{
  fxTGo4HodoOnlineSlots->StopMonitor();

}


void TGo4HodoOnline::Reset()
{
 	TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
	reset->SetHodoChanged(kTRUE);
	reset->Set("Hodo",0);
	TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
	com->SetParameter(reset);
	fxTGo4GUIRegistry->SubmitCommand(com);
}

