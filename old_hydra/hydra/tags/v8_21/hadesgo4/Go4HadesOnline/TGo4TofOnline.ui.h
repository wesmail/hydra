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
  
    
    //########################### REGISTER HISTS ###################################
   // fxTGo4GUIRegistry->ReDirectObject("hTofhits_raw",fxTGo4TofOnlineSlots, true,ToffxQtRootCanvas1);
    fxTGo4GUIRegistry->ReDirectObject("hTofmulttof_raw",fxTGo4TofOnlineSlots, true,ToffxQtRootCanvas1);
    fxTGo4GUIRegistry->ReDirectObject("hTofpatternL_raw",fxTGo4TofOnlineSlots, true,ToffxQtRootCanvas2);
    fxTGo4GUIRegistry->ReDirectObject("hTofpatternR_raw",fxTGo4TofOnlineSlots, true,ToffxQtRootCanvas3);
    fxTGo4GUIRegistry->ReDirectObject("hTofmulttoftofino_raw",fxTGo4TofOnlineSlots, true,ToffxQtRootCanvas4);
    fxTGo4GUIRegistry->ReDirectObject("hToftdcL_raw",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hToftdcR_raw",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofadcL_raw",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofadcR_raw",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hToftdcSum_hit",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hToftdcDiff_hit",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofadcSum_hit",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofadcPos_hit",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofsum_hit",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hToftheta_hit",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofphi_hit",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofyvsx_hit",fxTGo4TofOnlineSlots, true);

    //################## START ######################################################
    fxTGo4GUIRegistry->ReDirectObject("hStartstrips_start_veto_raw",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hStartmult_start_veto_raw",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hStarttime_start_raw",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hStarttime_veto_raw",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hStartstart_veto_corr_raw",fxTGo4TofOnlineSlots, true);

    //################## TOFINO #####################################################
    fxTGo4GUIRegistry->ReDirectObject("hTofinoprof",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofinoMultTdcAll",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofinoTDC",fxTGo4TofOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofinoADC",fxTGo4TofOnlineSlots, true);
}

void TGo4TofOnline::destroy()
{
  //########################### UNREGISTER HISTS ###################################
    //fxTGo4GUIRegistry->RemoveRedirectEntry("hTofhits_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofmulttof_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofmulttoftofino_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofpatternL_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofpatternR_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hToftdcL_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hToftdcR_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofadcL_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofadcR_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hToftdcSum_hit",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hToftdcDiff_hit",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofadcSum_hit",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofadcPos_hit",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofsum_hit",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hToftheta_hit",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofphi_hit",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofyvsx_hit",fxTGo4TofOnlineSlots);

    //################## START ######################################################
    fxTGo4GUIRegistry->RemoveRedirectEntry("hStartstrips_start_veto_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hStartmult_start_veto_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hStarttime_start_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hStarttime_veto_raw",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hStartstart_veto_corr_raw",fxTGo4TofOnlineSlots);
    //################## TOFINO #####################################################
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinoprof",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinoMultTdcAll",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinoTDC",fxTGo4TofOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinoADC",fxTGo4TofOnlineSlots);
}

void TGo4TofOnline::closeEvent( QCloseEvent * ce )
{
   TGo4LockGuard glob;
    ce->accept();
    fxTGo4TofOnlineSlots->SetGUI(0);
}

void TGo4TofOnline::TofDisplayLCD1( Float_t t )
{
    TofLCDNumber1->display(t);
}

void TGo4TofOnline::TofDisplayLCD2( Float_t t )
{
 TofLCDNumber2->display(t);
}

void TGo4TofOnline::TofDisplayLCD3( Float_t t )
{
    TofLCDNumber3->display(t);
}

void TGo4TofOnline::DrawHis( HMonHistAddon* His )
{
    //###################### TDC ####################################
    if(strcmp(TabWidget3->currentPage()->name(),"tdc")==0)
    {    
	if(strcmp(His->GetName(),"hToftdcL_raw")==0)
	{
           ToffxQtRootCanvas1_2->getCanvas()->cd();
	   His->setCanvas(ToffxQtRootCanvas1_2->getCanvas());
           His->draw();
	   ToffxQtRootCanvas1_2->getCanvas()->Update();
	}
 	if(strcmp(His->GetName(),"hToftdcR_raw")==0)
	{
           ToffxQtRootCanvas2_2->getCanvas()->cd();
	   His->setCanvas(ToffxQtRootCanvas2_2->getCanvas());
           His->draw();
	   ToffxQtRootCanvas2_2->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hToftdcSum_hit")==0)
	{
           ToffxQtRootCanvas3_2->getCanvas()->cd();
	   His->setCanvas(ToffxQtRootCanvas3_2->getCanvas());
           His->draw();
	   ToffxQtRootCanvas3_2->getCanvas()->Update();
	}
 	if(strcmp(His->GetName(),"hToftdcDiff_hit")==0)
	{
           ToffxQtRootCanvas4_2->getCanvas()->cd();
	   His->setCanvas(ToffxQtRootCanvas4_2->getCanvas());
           His->draw();
	   ToffxQtRootCanvas4_2->getCanvas()->Update();
	}

    }
    //###################### ADC ####################################
    if(strcmp(TabWidget3->currentPage()->name(),"adc")==0)
    {    
	if(strcmp(His->GetName(),"hTofadcL_raw")==0)
	{
	    ToffxQtRootCanvas1_3->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas1_3->getCanvas());
            His->draw();
	    ToffxQtRootCanvas1_3->getCanvas()->Update();
	}
 	if(strcmp(His->GetName(),"hTofadcR_raw")==0)
	{
	    ToffxQtRootCanvas2_3->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas2_3->getCanvas());
            His->draw();
	    ToffxQtRootCanvas2_3->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hTofadcSum_hit")==0)
	{
	    ToffxQtRootCanvas3_3->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas3_3->getCanvas());
            His->draw();
	    ToffxQtRootCanvas3_3->getCanvas()->Update();
	}
 	if(strcmp(His->GetName(),"hTofadcPos_hit")==0)
	{
	    ToffxQtRootCanvas4_3->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas4_3->getCanvas());
            His->draw();
	    ToffxQtRootCanvas4_3->getCanvas()->Update();
	}
    }
    //###################### HIT ####################################
    if(strcmp(TabWidget3->currentPage()->name(),"hit")==0)
    {    
	if(strcmp(His->GetName(),"hTofsum_hit")==0)
	{
	    ToffxQtRootCanvas1_4->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas1_4->getCanvas());
            His->draw();
	    ToffxQtRootCanvas1_4->getCanvas()->Update();
	}
 	if(strcmp(His->GetName(),"hToftheta_hit")==0)
	{
	    ToffxQtRootCanvas2_4->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas2_4->getCanvas());
            His->draw();
	    ToffxQtRootCanvas2_4->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hTofphi_hit")==0)
	{
	    ToffxQtRootCanvas3_4->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas3_4->getCanvas());
            His->draw();
	    ToffxQtRootCanvas3_4->getCanvas()->Update();
	}
 	if(strcmp(His->GetName(),"hTofyvsx_hit")==0)
	{
	    ToffxQtRootCanvas4_4->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas4_4->getCanvas());
            His->draw();
	    ToffxQtRootCanvas4_4->getCanvas()->Update();
	}
    }
    //###################### START ##################################
    if(strcmp(TabWidget3->currentPage()->name(),"start")==0)
    {    
	if(strcmp(His->GetName(),"hStartstrips_start_veto_raw")==0)
	{
	    ToffxQtRootCanvas1_5->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas1_5->getCanvas());
            His->draw();
	    ToffxQtRootCanvas1_5->getCanvas()->Update();
	}
 	if(strcmp(His->GetName(),"hStartmult_start_veto_raw")==0)
	{
	    ToffxQtRootCanvas2_5->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas2_5->getCanvas());
            His->draw();
	    ToffxQtRootCanvas2_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hStarttime_start_raw")==0)
	{
	    ToffxQtRootCanvas3_5->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas3_5->getCanvas());
            His->draw();
	    ToffxQtRootCanvas3_5->getCanvas()->Update();
	}
 	if(strcmp(His->GetName(),"hStarttime_veto_raw")==0)
	{
	    ToffxQtRootCanvas4_5->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas4_5->getCanvas());
            His->draw();
	    ToffxQtRootCanvas4_5->getCanvas()->Update();
	}
 	if(strcmp(His->GetName(),"hStartstart_veto_corr_raw")==0)
	{
	    ToffxQtRootCanvas5_5->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas5_5->getCanvas());
            His->draw();
	    ToffxQtRootCanvas5_5->getCanvas()->Update();
	}
    }

    //###################### TOFINO ##################################
    if(strcmp(TabWidget3->currentPage()->name(),"tofino")==0)
    {    
	if(strcmp(His->GetName(),"hTofinoprof")==0)
	{
	    ToffxQtRootCanvas1_6->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas1_6->getCanvas());
            His->draw();
	    ToffxQtRootCanvas1_6->getCanvas()->Update();
	}
 	if(strcmp(His->GetName(),"hTofinoMultTdcAll")==0)
	{
	    ToffxQtRootCanvas2_6->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas2_6->getCanvas());
            His->draw();
	    ToffxQtRootCanvas2_6->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hTofinoTDC")==0)
	{
	    ToffxQtRootCanvas3_6->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas3_6->getCanvas());
            His->draw();
	    ToffxQtRootCanvas3_6->getCanvas()->Update();
	}
 	if(strcmp(His->GetName(),"hTofinoADC")==0)
	{
	    ToffxQtRootCanvas4_6->getCanvas()->cd();
	    His->setCanvas(ToffxQtRootCanvas4_6->getCanvas());
            His->draw();
	    ToffxQtRootCanvas4_6->getCanvas()->Update();
	}
    }
}



void TGo4TofOnline::Reset()
{
 	TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
	reset->SetTofChanged(kTRUE);
	reset->Set("Tof",0);     
	TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
	com->SetParameter(reset);
	fxTGo4GUIRegistry->SubmitCommand(com);
}



