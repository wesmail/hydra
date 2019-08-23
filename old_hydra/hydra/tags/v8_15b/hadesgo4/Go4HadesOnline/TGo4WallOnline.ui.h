/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void TGo4WallOnline::init()
{


    setCaption(" Wall Go4 Online " );
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/

    fxTGo4WallOnlineSlots = dynamic_cast <TGo4WallOnlineSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4WallOnlineSlots"));

    /* test if  a Slot Object already exist for this GUI*/

   if(fxTGo4WallOnlineSlots  !=0) {
   /* if it is already there, set this GUI  as window of the slot Object */
		fxTGo4WallOnlineSlots->SetGUI(this);
   }else{
   /* if no Slot Object where found creat a new one*/
		fxTGo4WallOnlineSlots = new TGo4WallOnlineSlots ("TGo4WallOnlineSlots" , "Slot Class for Qt GUI", this);
   }
   /* Get the pointer for the Status Object */
    fxTGo4WallOnlineStatus= dynamic_cast <TGo4WallOnlineStatus *> (fxTGo4WallOnlineSlots->GetStatus());


    //########################### REGISTER HISTS ###################################
    fxTGo4GUIRegistry->ReDirectObject("hWallProf1",fxTGo4WallOnlineSlots, true,WallfxQtRootCanvasPF1);
    fxTGo4GUIRegistry->ReDirectObject("hWallProf2",fxTGo4WallOnlineSlots, true,WallfxQtRootCanvasPF2);
    fxTGo4GUIRegistry->ReDirectObject("hWallProf3",fxTGo4WallOnlineSlots, true,WallfxQtRootCanvasPF3);
    fxTGo4GUIRegistry->ReDirectObject("hWallRawTime1",fxTGo4WallOnlineSlots, true,WallfxQtRootCanvasRT1);
    fxTGo4GUIRegistry->ReDirectObject("hWallRawWidth1",fxTGo4WallOnlineSlots, true,WallfxQtRootCanvasRW1);
    fxTGo4GUIRegistry->ReDirectObject("hWallHitTime",fxTGo4WallOnlineSlots, true,WallfxQtRootCanvasHitTime);
    fxTGo4GUIRegistry->ReDirectObject("hWallHitCharge",fxTGo4WallOnlineSlots, true,WallfxQtRootCanvasHitCharge);
    fxTGo4GUIRegistry->ReDirectObject("hWallHitXY",fxTGo4WallOnlineSlots, true,WallfxQtRootCanvasHitXY);
    fxTGo4GUIRegistry->ReDirectObject("hWallFib1Mult",fxTGo4WallOnlineSlots, true,WallfxQtRootCanvas8);
    fxTGo4GUIRegistry->ReDirectObject("hWallRawMult",fxTGo4WallOnlineSlots, true,WallfxQtRootCanvasRawMult);
}

void TGo4WallOnline::destroy()
{
    //########################### UNREGISTER HISTS ###################################
    fxTGo4GUIRegistry->RemoveRedirectEntry("hWallProf1",fxTGo4WallOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hWallRawTime1",fxTGo4WallOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hWallRawWidth1",fxTGo4WallOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hWallHitTime",fxTGo4WallOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hWallHitCharge",fxTGo4WallOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hWallHitXY",fxTGo4WallOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hWallFib1Mult",fxTGo4WallOnlineSlots);
	 fxTGo4GUIRegistry->RemoveRedirectEntry("hWallRawMult",fxTGo4WallOnlineSlots);
}

void TGo4WallOnline::closeEvent( QCloseEvent * ce )
{
   TGo4LockGuard glob;
    ce->accept();
    fxTGo4WallOnlineSlots->SetGUI(0);
}

void TGo4WallOnline::WallDisplayLCD1( float t )
{
    WallLCDNumber1->display(t);
}

void TGo4WallOnline::WallDisplayLCD2( float t )
{
 WallLCDNumber2->display(t);
}

void TGo4WallOnline::WallDisplayLCD3( float t )
{
    WallLCDNumber3->display(t);
}

void TGo4WallOnline::DrawHis( HMonHistAddon* His )
{

}

void TGo4WallOnline::Start()
{
   fxTGo4WallOnlineSlots->StartMonitor();
}

void TGo4WallOnline::Stop()
{
  fxTGo4WallOnlineSlots->StopMonitor();

}


void TGo4WallOnline::Reset()
{
 	TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
	reset->SetWallChanged(kTRUE);
	reset->Set("Wall",0);
	TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
	com->SetParameter(reset);
	fxTGo4GUIRegistry->SubmitCommand(com);
}

