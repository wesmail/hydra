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
    fxTGo4GUIRegistry->ReDirectObject("hRichChargedParticles",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas2);
    fxTGo4GUIRegistry->ReDirectObject("hMdccutstat",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas3);
    fxTGo4GUIRegistry->ReDirectObject("hTofhits_raw",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas4);
    fxTGo4GUIRegistry->ReDirectObject("hTofinohits_raw",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas5);
    fxTGo4GUIRegistry->ReDirectObject("hShowerhits_raw",fxTGo4HadesOnlineSlots, true,MainfxQtRootCanvas6);
  
}

void TGo4HadesOnline::destroy()
{
    //########################### UNREGISTER HISTS ###################################
    fxTGo4GUIRegistry->RemoveRedirectEntry("hStartstrips_start_veto_raw",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hRichChargedParticles",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdccutstat",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofhits_raw",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinohits_raw",fxTGo4HadesOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hShowerhits_raw",fxTGo4HadesOnlineSlots);
}


void TGo4HadesOnline::closeEvent( QCloseEvent *ce )
{
    TGo4LockGuard glob;
    ce->accept();
    fxTGo4HadesOnlineSlots->SetGUI(0);
}


void TGo4HadesOnline::MainDisplayLCD1( float t )
{
      MainLCDNumber1->display(t);
}
void TGo4HadesOnline::MainDisplayLCD2( float t )
{
    MainLCDNumber2->display(t);
}
void TGo4HadesOnline::MainDisplayLCD3( float t )
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

