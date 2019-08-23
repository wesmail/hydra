/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void TGo4MdcOnline::init()
{


    setCaption(" Mdc Go4 Online " );
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/
    
    fxTGo4MdcOnlineSlots = dynamic_cast <TGo4MdcOnlineSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4MdcOnlineSlots"));   
    
    /* test if  a Slot Object already exist for this GUI*/
    
   if(fxTGo4MdcOnlineSlots  !=0) {
   /* if it is already there, set this GUI  as window of the slot Object */
		fxTGo4MdcOnlineSlots->SetGUI(this);
   }else{
   /* if no Slot Object where found creat a new one*/
		fxTGo4MdcOnlineSlots = new TGo4MdcOnlineSlots ("TGo4MdcOnlineSlots" , "Slot Class for Qt GUI", this);
   }
   /* Get the pointer for the Status Object */
    fxTGo4MdcOnlineStatus= dynamic_cast <TGo4MdcOnlineStatus *> (fxTGo4MdcOnlineSlots->GetStatus());
    
}

void TGo4MdcOnline::destroy()
{

}

void TGo4MdcOnline::closeEvent( QCloseEvent * ce )
{
   TGo4LockGuard glob;
    ce->accept();
    fxTGo4MdcOnlineSlots->SetGUI(0);
    delete this;
}

void TGo4MdcOnline::MdcDisplayLCD1( float t )
{
    MdcLCDNumber1->display(t);
}

void TGo4MdcOnline::MdcDisplayLCD2( float t )
{
 MdcLCDNumber2->display(t);
}

void TGo4MdcOnline::MdcDisplayLCD3( float t )
{
    MdcLCDNumber3->display(t);
}

void TGo4MdcOnline::DrawHis( HMonHistAddon* His )
{
    /*
    if ( strcmp(His->GetName(),"hMdctime1Raw") == 0 )
    {
               MdcfxQtRootCanvas1->getCanvas()->cd();
               His->setCanvas(MdcfxQtRootCanvas1->getCanvas());
	((TH1F*)(((HMdcMonHist*)His)->getP()))->Draw();
	MdcfxQtRootCanvas1->getCanvas()->Update();
    }*/
    
    /*
    if ( strcmp(His->GetName(),"hMdctime1Cal1") ==  0 )
    {
	MdcfxQtRootCanvas2->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas2->getCanvas());
	((TH1F*)(((HMonHist*)His)->getP()))->Draw();
	MdcfxQtRootCanvas2->getCanvas()->Update();
    }*/
    if ( strcmp(His->GetName(),"hMdctime1Cal1MeanTrend") ==  0 )
    {
	MdcfxQtRootCanvas3->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas3->getCanvas());
	((TH1F*)(((HTrendHist*)His)->getP()))->Draw();
	MdcfxQtRootCanvas3->getCanvas()->Update();
    }
    if ( strcmp(His->GetName(),"hMdccutstat") ==  0 )
    {
	MdcfxQtRootCanvas4->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas4->getCanvas());
	((HHistArray*)His)->drawAll();
	MdcfxQtRootCanvas4->getCanvas()->Update();
    }

    if ( strcmp(His->GetName(),"hMdccal1hits") ==  0 )
    {
	MdcfxQtRootCanvas5->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas5->getCanvas());
	((TH2F*)(((HMonHist2*)His)->getP()))->Draw();
	MdcfxQtRootCanvas5->getCanvas()->Update();
    }
    
    if ( strcmp(His->GetName(),"hMdcrawRoc_Subev") ==  0 )
    {
	MdcfxQtRootCanvas6->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas6->getCanvas());
	((TH2F*)(((HMonHist2*)His)->getP()))->Draw();
	MdcfxQtRootCanvas6->getCanvas()->Update();
    }
    if ( strcmp(His->GetName(),"hMdcrawError_Stat") ==  0 )
    {
	MdcfxQtRootCanvas1->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas1->getCanvas());
	((TH2F*)(((HMonHist2*)His)->getP()))->Draw();
	MdcfxQtRootCanvas1->getCanvas()->Update();
    }

}

void TGo4MdcOnline::Start()
{
   fxTGo4MdcOnlineSlots->StartMonitor();
}

void TGo4MdcOnline::Stop()
{
fxTGo4MdcOnlineSlots->StopMonitor();
}

void TGo4MdcOnline::Reset()
{
}



