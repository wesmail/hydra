/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void TGo4HadesOnline::init() 
{

    setCaption(" Hades Go4 Online " );
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/
    
    fxTGo4HadesOnlineSlots = dynamic_cast <TGo4HadesOnlineSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4HadesOnlineSlots"));   
    
    /* test if  a Slot Object already exist for this GUI*/
    
   if(fxTGo4HadesOnlineSlots  !=0) {
   /* if it is already there, set this GUI  as window of the slot Object */
		fxTGo4HadesOnlineSlots->SetGUI(this);
   }else{
   /* if no Slot Object where found creat a new one*/
		fxTGo4HadesOnlineSlots = new TGo4HadesOnlineSlots ("TGo4HadesOnlineSlots" , "Slot Class for Qt GUI", this);
   }
   /* Get the pointer for the Status Object */
    fxTGo4HadesOnlineStatus= dynamic_cast <TGo4HadesOnlineStatus *> (fxTGo4HadesOnlineSlots->GetStatus());
    
    //########################### REGISTER HISTS ###################################
    fxTGo4GUIRegistry->ReDirectObject("hStartstrips_start_veto_raw",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas1);
    fxTGo4GUIRegistry->ReDirectObject("hRichmult_raw",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas2);
    fxTGo4GUIRegistry->ReDirectObject("hMdccal1hits",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas3);
    fxTGo4GUIRegistry->ReDirectObject("hTofmult_raw",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas4);
    fxTGo4GUIRegistry->ReDirectObject("hTofinomult_raw",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas5);
    fxTGo4GUIRegistry->ReDirectObject("hShowermult_raw",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas6);


    fxTGo4GUIRegistry->ReDirectObject("hStartmeanstrip_trend_raw",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas_2_1);
    //fxTGo4GUIRegistry->ReDirectObject("hRichChargedParticles",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas_2_2);
    fxTGo4GUIRegistry->ReDirectObject("hRichmult_trend_raw"  ,fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas_2_2);
    fxTGo4GUIRegistry->ReDirectObject("hMdccal1hitstrend"    ,fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas_2_3);
    fxTGo4GUIRegistry->ReDirectObject("hTofmult_trend_raw"   ,fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas_2_4);
    fxTGo4GUIRegistry->ReDirectObject("hTofinomult_trend_raw",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas_2_5);
    fxTGo4GUIRegistry->ReDirectObject("hShowermult_trend_raw",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas_2_6);

}

void TGo4HadesOnline::destroy()
{
    //########################### UNREGISTER HISTS ###################################
    fxTGo4GUIRegistry->RemoveRedirectEntry("hStartstrips_start_veto_raw",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hRichmult_raw",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdccal1hits",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofmult_raw",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinomult_raw",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hShowermult_raw",fxTGo4HadesOnlineSlots);

    fxTGo4GUIRegistry->RemoveRedirectEntry("hStartmeanstrip_trend_raw",fxTGo4HadesOnlineSlots);
    //fxTGo4GUIRegistry->RemoveRedirectEntry("hRichChargedParticles",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hRichmult_trend_raw",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdccal1hitstrend",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofmulttrend_raw",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinomulttrend_raw",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hShowermulttrend_raw",fxTGo4HadesOnlineSlots);
}


void TGo4HadesOnline::closeEvent( QCloseEvent *ce )
{
    TGo4LockGuard glob;
    ce->accept();
    fxTGo4HadesOnlineSlots->SetGUI(0);
}


void TGo4HadesOnline::MainDisplayLCD1( Float_t t )
{
      MainLCDNumber1->display(t);
}
void TGo4HadesOnline::MainDisplayLCD2( Float_t t )
{
    MainLCDNumber2->display(t);
}
void TGo4HadesOnline::MainDisplayLCD3( Float_t t )
{
 MainLCDNumber3->display(t);
}

void TGo4HadesOnline::DrawHis(HMonHistAddon* His)
{
}
 
 
 


void TGo4HadesOnline::Reset()
{
 	TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
	reset->Set("All",0);     
	TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
	com->SetParameter(reset);
	fxTGo4GUIRegistry->SubmitCommand(com);
}

