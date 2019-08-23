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
    
}

void TGo4HadesOnline::destroy()
{
	/* Delete the Slot Object by closing this window, this will also delete the status object
	if you want to keep these two objects to restore settings for example, you should commet this delete*/
	
  //	delete fxTGo4HadesOnlineSlots;
}


void TGo4HadesOnline::closeEvent( QCloseEvent *ce )
{
    TGo4LockGuard glob;
    ce->accept();
    fxTGo4HadesOnlineSlots->SetGUI(0);
    delete this;
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
void TGo4HadesOnline::DrawHis(TH1* His)
{}
 void TGo4HadesOnline::DrawHis(TH2* His)
{}
 
void TGo4HadesOnline::DrawHis(HMonHistAddon* His)
{
    //************************START**************************
    
    if ( strcmp(His->GetName(),"hStartstrips_start_veto_rawM") ==  0 )
    {
	MainfxQtRootCanvas1->getCanvas()->cd();
	His->setCanvas(MainfxQtRootCanvas1->getCanvas());
	((HHistArray*)His)->drawAll();
	MainfxQtRootCanvas1->getCanvas()->Update();
    }
    //************************RICH**************************
    
    if ( strcmp(His->GetName(),"hRichpulsheightM") ==  0 )
    {
	MainfxQtRootCanvas2->getCanvas()->cd();
	His->setCanvas(MainfxQtRootCanvas2->getCanvas());
	((HHistArray*)His)->drawAll();
	MainfxQtRootCanvas2->getCanvas()->Update();
    }
    
    //************************MDC**************************
    if ( strcmp(His->GetName(),"hMdccutstatM") ==  0 )
    {
	MainfxQtRootCanvas3->getCanvas()->cd();
	His->setCanvas(MainfxQtRootCanvas3->getCanvas());
	((HHistArray*)His)->drawAll();
	MainfxQtRootCanvas3->getCanvas()->Update();
    }
     //************************TOF**************************
    
    if ( strcmp(His->GetName(),"hTofhits_rawM") ==  0 )
    {
	MainfxQtRootCanvas4->getCanvas()->cd();
	His->setCanvas(MainfxQtRootCanvas4->getCanvas());
	((HHistArray*)His)->drawAll();
	MainfxQtRootCanvas4->getCanvas()->Update();
    }
    //************************TOFINO**************************
 
    if ( strcmp(His->GetName(),"hTofinohits_rawM") ==  0 )
    {
	MainfxQtRootCanvas5->getCanvas()->cd();
	His->setCanvas(MainfxQtRootCanvas5->getCanvas());
	((HHistArray*)His)->drawAll();
	MainfxQtRootCanvas5->getCanvas()->Update();
    }  
     //************************SHOWER**************************
     if ( strcmp(His->GetName(),"hShowerhits_rawM") ==  0 )
    {
	MainfxQtRootCanvas6->getCanvas()->cd();
	His->setCanvas(MainfxQtRootCanvas6->getCanvas());
	((TH2F*)(((HMonHist2*)His)->getP()))->Draw("");
	MainfxQtRootCanvas6->getCanvas()->Update();
    }  
        
}
 
 
 
    void TGo4HadesOnline::Start()
{
    fxTGo4HadesOnlineSlots->StartMonitor();
}


void TGo4HadesOnline::Stop()
{
 fxTGo4HadesOnlineSlots->StopMonitor();
}


void TGo4HadesOnline::Reset()
{
fxTGo4HadesOnlineSlots->Reset();
}

