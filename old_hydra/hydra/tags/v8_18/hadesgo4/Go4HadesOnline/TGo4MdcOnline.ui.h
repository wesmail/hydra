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
    
   for(Int_t i=0;i<6;i++){colorsSec[i]=0;}
   for(Int_t i=0;i<6;i++){colorsMod[i]=0;}
   for(Int_t i=0;i<6;i++){colorsSector[i]=new QColor();}
   for(Int_t i=0;i<6;i++){colorsModule[i]=new QColor();}
   //########################### REGISTER HISTS ###################################
    fxTGo4GUIRegistry->ReDirectObject("MdcConfig",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("MdcLookup",fxTGo4MdcOnlineSlots, true);
  
    fxTGo4GUIRegistry->ReDirectObject("hMdcrawError_Stat",fxTGo4MdcOnlineSlots, true,MdcfxQtRootCanvas1);
    fxTGo4GUIRegistry->ReDirectObject("hMdcrawRoc_Subev",fxTGo4MdcOnlineSlots, true,MdcfxQtRootCanvas2);
    fxTGo4GUIRegistry->ReDirectObject("hMdccutstat",fxTGo4MdcOnlineSlots, true,MdcfxQtRootCanvas3);
    fxTGo4GUIRegistry->ReDirectObject("hMdccal1hits",fxTGo4MdcOnlineSlots, true,MdcfxQtRootCanvas4);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1Cal1",fxTGo4MdcOnlineSlots, true,MdcfxQtRootCanvas5);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1Cal1MeanTrend",fxTGo4MdcOnlineSlots, true,MdcfxQtRootCanvas6);
    fxTGo4GUIRegistry->ReDirectObject("hMdclayer",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdcmbotdc",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdcmbotdcCalib",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdcmodtimestatCalib",fxTGo4MdcOnlineSlots, true);

    fxTGo4GUIRegistry->ReDirectObject("hMdctime1Mod0",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1Mod1",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1Mod2",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1Mod3",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime2Mod0",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime2Mod1",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime2Mod2",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime2Mod3",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime12Mod0",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime12Mod1",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime12Mod2",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime12Mod3",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1_12",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1rawMod0",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1rawMod1",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1rawMod2",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1rawMod3",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime2rawMod0",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime2rawMod1",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime2rawMod2",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime2rawMod3",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime12rawMod0",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime12rawMod1",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime12rawMod2",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime12rawMod3",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1_12raw",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdcMbo",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdcMboTrendMod0",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdcMboTrendMod1",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdcMboTrendMod2",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdcMboTrendMod3",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1Cal1MeanTrendMod0",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1Cal1MeanTrendMod1",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1Cal1MeanTrendMod2",fxTGo4MdcOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hMdctime1Cal1MeanTrendMod3",fxTGo4MdcOnlineSlots, true);

}

void TGo4MdcOnline::destroy()
{
    //########################### UNREGISTER HISTS ###################################
    fxTGo4GUIRegistry->RemoveRedirectEntry("MdcConfig",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("MdcLookup",fxTGo4MdcOnlineSlots);
   
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdcrawError_Stat",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdcrawRoc_Subev",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdccutstat",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdccal1hits",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1Cal1",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1Cal1MeanTrend",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdclayer",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdcmbotdc",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdcmbotdcCalib",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdcmodtimestatCalib",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1Mod0",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1Mod1",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1Mod2",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1Mod3",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime2Mod0",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime2Mod1",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime2Mod2",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime2Mod3",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime12Mod0",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime12Mod1",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime12Mod2",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime12Mod3",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1_12",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1rawMod0",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1rawMod1",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1rawMod2",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1rawMod3",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime2rawMod0",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime2rawMod1",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime2rawMod2",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime2rawMod3",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime12rawMod0",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime12rawMod1",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime12rawMod2",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime12rawMod3",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1_12raw",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdcMbo",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdcMboTrendMod0",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdcMboTrendMod1",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdcMboTrendMod2",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdcMboTrendMod3",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1Cal1MeanTrendMod0",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1Cal1MeanTrendMod1",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1Cal1MeanTrendMod2",fxTGo4MdcOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdctime1Cal1MeanTrendMod3",fxTGo4MdcOnlineSlots);
}

void TGo4MdcOnline::closeEvent( QCloseEvent * ce )
{
   TGo4LockGuard glob;
    ce->accept();
    fxTGo4MdcOnlineSlots->SetGUI(0);
}

void TGo4MdcOnline::MdcDisplayLCD1( Float_t t )
{
    MdcLCDNumber1->display(t);
}

void TGo4MdcOnline::MdcDisplayLCD2( Float_t t )
{
 MdcLCDNumber2->display(t);
}

void TGo4MdcOnline::MdcDisplayLCD3( Float_t t )
{
    MdcLCDNumber3->display(t);
}

void TGo4MdcOnline::DrawHis( HMonHistAddon* His )
{
    if(strcmp(TabWidget2->currentPage()->name(),"layer")==0)
    {    
	if(strcmp(His->GetName(),"hMdclayer")==0)
	{
          DrawLayer(His);
	}
    }
    else if(strcmp(TabWidget2->currentPage()->name(),"tdc")==0)
    {    
	DrawTdc(His);
    }
    else if(strcmp(TabWidget2->currentPage()->name(),"mbo")==0)
    {    
	DrawMbo(His);
    }

    else if(strcmp(TabWidget2->currentPage()->name(),"times")==0)
    {    
       if(levelselect->currentItem ()==0)DrawTimesRaw(His);
       if(levelselect->currentItem ()==1)DrawTimesCal(His);
    }
    else if(strcmp(TabWidget2->currentPage()->name(),"calibration")==0)
    {    
	DrawCalibration(His);
    }

}

void TGo4MdcOnline::Reset()
{
 	TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
	reset->SetMdcChanged(kTRUE);
	reset->Set("Mdc",0);     
	TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
	com->SetParameter(reset);
	fxTGo4GUIRegistry->SubmitCommand(com);
}

HMdcConnectedTdc* TGo4MdcOnline::getLookup()
{
    HMdcConnectedTdc* lookup=0;
    if(fxTGo4MdcOnlineSlots)lookup=fxTGo4MdcOnlineSlots->getMdcLookup();
    return lookup;
}

void TGo4MdcOnline::calcRawToCal()
{
    Int_t sec=rawsec->value();
    Int_t mod=rawmod->value();
    Int_t mb =rawmb ->value();
    Int_t t  =rawt  ->value();

    HMdcConnectedTdc* lookup=getLookup();
    if(lookup)
    {
	Int_t l,c;
	lookup->getCalAddress(sec,mod,mb,t,&l,&c);
	QString tmp;
        Char_t  tmp1[4];

	sprintf(tmp1,"%i",lookup->getMboName(sec,mod,mb));
        tmp=tmp1;
	rawmbname->setText(tmp);
	sprintf(tmp1,"%i",l);
        tmp=tmp1;
	rawlay->setText(tmp);
        sprintf(tmp1,"%i",c);
	tmp=tmp1;
	rawwire->setText(tmp);
	sprintf(tmp1,"%i",lookup->getNTdcs(sec,mod,mb));
	tmp=tmp1;
	rawmaxt->setText(tmp);

    }else cout<<"TGo4MdcOnline:calcRawToCal() : RECEIVED ZERO POINTER FOR LOOKUP!"<<endl;
}

void TGo4MdcOnline::calcCalToRaw()
{
    Int_t sec=calsec ->value();
    Int_t mod=calmod ->value();
    Int_t l  =callay ->value();
    Int_t c  =calcell->value();

    HMdcConnectedTdc* lookup=getLookup();
    if(lookup)
    {
	Int_t mb,t;
	lookup->getRawAddress(sec,mod,l,c,&mb,&t);
	QString tmp;
        Char_t  tmp1[4];

	sprintf(tmp1,"%i",lookup->getMboName(sec,mod,mb));
        tmp=tmp1;
	calmbname->setText(tmp);
	sprintf(tmp1,"%i",mb);
        tmp=tmp1;
	calmb->setText(tmp);
        sprintf(tmp1,"%i",t);
	tmp=tmp1;
	calt->setText(tmp);
        sprintf(tmp1,"%i",lookup->getNCells(sec,mod,l));
	tmp=tmp1;
	calmaxwire->setText(tmp);

    }else cout<<"TGo4MdcOnline:calcCalToRaw() : RECEIVED ZERO POINTER FOR LOOKUP!"<<endl;
}


void TGo4MdcOnline::DrawLayer(HMonHistAddon* His)
{
    Int_t sec=sectorselect->value();
    Int_t mod=moduleselect->value();

    if(strcmp(His->GetName(),"hMdclayer")==0)
    {
	HHistArray* h=(HHistArray*)His;

	MdcfxQtRootCanvas1_2->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas1_2->getCanvas());
	((TH1*)(h->getP(0,sec*6*4+mod*6+0)))->Draw();
	MdcfxQtRootCanvas1_2->getCanvas()->Update();

	MdcfxQtRootCanvas2_2->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas2_2->getCanvas());
	((TH1*)(h->getP(0,sec*6*4+mod*6+1)))->Draw();
	MdcfxQtRootCanvas2_2->getCanvas()->Update();

	MdcfxQtRootCanvas3_2->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas3_2->getCanvas());
	((TH1*)(h->getP(0,sec*6*4+mod*6+2)))->Draw();
	MdcfxQtRootCanvas3_2->getCanvas()->Update();

	MdcfxQtRootCanvas4_2->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas4_2->getCanvas());
	((TH1*)(h->getP(0,sec*6*4+mod*6+3)))->Draw();
	MdcfxQtRootCanvas4_2->getCanvas()->Update();

	MdcfxQtRootCanvas5_2->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas5_2->getCanvas());
	((TH1*)(h->getP(0,sec*6*4+mod*6+4)))->Draw();
	MdcfxQtRootCanvas5_2->getCanvas()->Update();

	MdcfxQtRootCanvas6_2->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas6_2->getCanvas());
	((TH1*)(h->getP(0,sec*6*4+mod*6+5)))->Draw();
	MdcfxQtRootCanvas6_2->getCanvas()->Update();
    }
}

void TGo4MdcOnline::DrawTdc(HMonHistAddon* His)
{
    Int_t mod=moduleselect->value();

    if(strcmp(His->GetName(),"hMdcmbotdc")==0)
    {
	HHistArray2* h=(HHistArray2*)His;

	MdcfxQtRootCanvas1_3->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas1_3->getCanvas());
	((TH2*)(h->getP(0,mod)))->Draw();
	MdcfxQtRootCanvas1_3->getCanvas()->Update();

	MdcfxQtRootCanvas2_3->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas2_3->getCanvas());
	((TH2*)(h->getP(1,mod)))->Draw();
	MdcfxQtRootCanvas2_3->getCanvas()->Update();

	MdcfxQtRootCanvas3_3->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas3_3->getCanvas());
	((TH2*)(h->getP(2,mod)))->Draw();
	MdcfxQtRootCanvas3_3->getCanvas()->Update();

	MdcfxQtRootCanvas4_3->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas4_3->getCanvas());
	((TH2*)(h->getP(3,mod)))->Draw();
	MdcfxQtRootCanvas4_3->getCanvas()->Update();

	MdcfxQtRootCanvas5_3->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas5_3->getCanvas());
	((TH2*)(h->getP(4,mod)))->Draw();
	MdcfxQtRootCanvas5_3->getCanvas()->Update();

	MdcfxQtRootCanvas6_3->getCanvas()->cd();
	His->setCanvas(MdcfxQtRootCanvas6_3->getCanvas());
	((TH2*)(h->getP(5,mod)))->Draw();
	MdcfxQtRootCanvas6_3->getCanvas()->Update();
    }
}

void TGo4MdcOnline::DrawTimesCal(HMonHistAddon* His)
{
    Int_t sec=sectorselect->value();
    Int_t mod=moduleselect->value();

    if(timesselect->currentItem ()==0) // time1
    {
	if(strcmp(His->GetName(),"hMdctime1Mod0")==0)
	{
	    DrawColors(6,0,His);

	    HHistArray* h=(HHistArray*)His;
	    MdcfxQtRootCanvas1_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas1_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime1Mod1")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas2_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas2_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime1Mod2")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas3_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas3_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime1Mod3")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas4_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas4_5->getCanvas()->Update();
	}
    }
    if(timesselect->currentItem ()==1) // time2
    {
	if(strcmp(His->GetName(),"hMdctime2Mod0")==0)
	{
	    DrawColors(6,0,His);
	    HHistArray* h=(HHistArray*)His;
	    MdcfxQtRootCanvas1_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas1_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime2Mod1")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas2_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas2_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime2Mod2")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas3_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas3_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime2Mod3")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas4_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas4_5->getCanvas()->Update();
	}
    }
    if(timesselect->currentItem ()==2)  // time2-time1
    {
	if(strcmp(His->GetName(),"hMdctime12Mod0")==0)
	{
	    DrawColors(6,0,His);
	    HHistArray* h=(HHistArray*)His;
	    MdcfxQtRootCanvas1_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas1_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime12Mod1")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas2_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas2_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime12Mod2")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas3_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas3_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime12Mod3")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas4_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas4_5->getCanvas()->Update();
	}
    }
    if(timesselect->currentItem ()==3)  // individual selected module
    {
	Char_t myname[50];
	sprintf(myname,"%s%i","hMdctime1Mod",mod);
	if(strcmp(His->GetName(),myname)==0)
	{
	    DrawColors(6,0,His);
	    HHistArray* h=(HHistArray*)His;
	    MdcfxQtRootCanvas1_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_5->getCanvas());
	    h->getP(0,sec)->Draw();
	    MdcfxQtRootCanvas1_5->getCanvas()->Update();
	}
	sprintf(myname,"%s%i","hMdctime2Mod",mod);
	if(strcmp(His->GetName(),myname)==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas2_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_5->getCanvas());
	    h->getP(0,sec)->Draw();
	    MdcfxQtRootCanvas2_5->getCanvas()->Update();
	}
	sprintf(myname,"%s%i","hMdctime12Mod",mod);
	if(strcmp(His->GetName(),myname)==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas3_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_5->getCanvas());
	    h->getP(0,sec)->Draw();
	    MdcfxQtRootCanvas3_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime1_12")==0)
	{
	    HHistArray2* h=(HHistArray2*)His;

	    MdcfxQtRootCanvas4_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_5->getCanvas());
	    h->getP(sec,mod)->Draw();
	    MdcfxQtRootCanvas4_5->getCanvas()->Update();
	}
    }

    if(timesselect->currentItem ()==4)  // mean trend of time1
    {
	if(strcmp(His->GetName(),"hMdctime1Cal1MeanTrendMod0")==0)
	{
	    DrawColors(6,0,His);
	    HTrendArray* h=(HTrendArray*)His;
	    MdcfxQtRootCanvas1_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas1_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime1Cal1MeanTrendMod1")==0)
	{
	    HTrendArray* h=(HTrendArray*)His;

	    MdcfxQtRootCanvas2_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas2_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime1Cal1MeanTrendMod2")==0)
	{
	    HTrendArray* h=(HTrendArray*)His;

	    MdcfxQtRootCanvas3_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas3_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime1Cal1MeanTrendMod3")==0)
	{
	    HTrendArray* h=(HTrendArray*)His;

	    MdcfxQtRootCanvas4_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas4_5->getCanvas()->Update();
	}
    }





}
void TGo4MdcOnline::DrawTimesRaw(HMonHistAddon* His)
{
    Int_t sec=sectorselect->value();
    Int_t mod=moduleselect->value();

    if(timesselect->currentItem ()==0) // time1
    {
	if(strcmp(His->GetName(),"hMdctime1rawMod0")==0)
	{
	    DrawColors(6,0,His);
	    HHistArray* h=(HHistArray*)His;
	    MdcfxQtRootCanvas1_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas1_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime1rawMod1")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas2_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas2_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime1rawMod2")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas3_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas3_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime1rawMod3")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas4_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas4_5->getCanvas()->Update();
	}
    }
    if(timesselect->currentItem ()==1) // time2
    {
	if(strcmp(His->GetName(),"hMdctime2rawMod0")==0)
	{
	    DrawColors(6,0,His);
	    HHistArray* h=(HHistArray*)His;
	    MdcfxQtRootCanvas1_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas1_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime2rawMod1")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas2_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas2_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime2rawMod2")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas3_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas3_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime2rawMod3")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas4_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas4_5->getCanvas()->Update();
	}
    }
    if(timesselect->currentItem ()==2)  // time2-time1
    {
	if(strcmp(His->GetName(),"hMdctime12rawMod0")==0)
	{
	    DrawColors(6,0,His);
	    HHistArray* h=(HHistArray*)His;
	    MdcfxQtRootCanvas1_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas1_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime12rawMod1")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas2_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas2_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime12rawMod2")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas3_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas3_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime12rawMod3")==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas4_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_5->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas4_5->getCanvas()->Update();
	}
    }
    if(timesselect->currentItem ()==3)  // individual selected module
    {
	Char_t myname[50];
	sprintf(myname,"%s%i","hMdctime1rawMod",mod);
	if(strcmp(His->GetName(),myname)==0)
	{
	    DrawColors(6,0,His);
	    HHistArray* h=(HHistArray*)His;
	    MdcfxQtRootCanvas1_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_5->getCanvas());
	    h->getP(0,sec)->Draw();
	    MdcfxQtRootCanvas1_5->getCanvas()->Update();
	}
	sprintf(myname,"%s%i","hMdctime2rawMod",mod);
	if(strcmp(His->GetName(),myname)==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas2_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_5->getCanvas());
	    h->getP(0,sec)->Draw();
	    MdcfxQtRootCanvas2_5->getCanvas()->Update();
	}
	sprintf(myname,"%s%i","hMdctime12rawMod",mod);
	if(strcmp(His->GetName(),myname)==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    MdcfxQtRootCanvas3_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_5->getCanvas());
	    h->getP(0,sec)->Draw();
	    MdcfxQtRootCanvas3_5->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdctime1_12raw")==0)
	{
	    HHistArray2* h=(HHistArray2*)His;

	    MdcfxQtRootCanvas4_5->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_5->getCanvas());
	    h->getP(sec,mod)->Draw();
	    MdcfxQtRootCanvas4_5->getCanvas()->Update();
	}
    }

}


void TGo4MdcOnline::DrawMbo( HMonHistAddon * His )
{
     Int_t select=mbselect->currentItem();

    if(select==0) // stat
    {
	if(strcmp(His->GetName(),"hMdcMbo")==0)
	{
	    HHistArray2* h=(HHistArray2*)His;

	    MdcfxQtRootCanvas1_4->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_4->getCanvas());
	    h->getP(0,0)->Draw();
	    MdcfxQtRootCanvas1_4->getCanvas()->Update();

	    MdcfxQtRootCanvas2_4->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_4->getCanvas());
	    h->getP(0,1)->Draw();
	    MdcfxQtRootCanvas2_4->getCanvas()->Update();

	    MdcfxQtRootCanvas3_4->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_4->getCanvas());
	    h->getP(0,2)->Draw();
	    MdcfxQtRootCanvas3_4->getCanvas()->Update();

	    MdcfxQtRootCanvas4_4->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_4->getCanvas());
	    h->getP(0,3)->Draw();
	    MdcfxQtRootCanvas4_4->getCanvas()->Update();

	}
    }
    else if(select==1) // trend
    {
	if(strcmp(His->GetName(),"hMdcMboTrendMod0")==0)
	{
	    DrawColors(6,0,His);
	    HTrendArray* h=(HTrendArray*)His;
	    MdcfxQtRootCanvas1_4->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_4->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas1_4->getCanvas()->Update();
	}

	if(strcmp(His->GetName(),"hMdcMboTrendMod1")==0)
	{
	    HTrendArray* h=(HTrendArray*)His;

	    MdcfxQtRootCanvas2_4->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_4->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas2_4->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdcMboTrendMod2")==0)
	{
	    HTrendArray* h=(HTrendArray*)His;

	    MdcfxQtRootCanvas3_4->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_4->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas3_4->getCanvas()->Update();
	}
	if(strcmp(His->GetName(),"hMdcMboTrendMod3")==0)
	{
	    HTrendArray* h=(HTrendArray*)His;

	    MdcfxQtRootCanvas4_4->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_4->getCanvas());
	    h->drawAll();
	    MdcfxQtRootCanvas4_4->getCanvas()->Update();
	}
    }
}

void TGo4MdcOnline::SetActiveHists()
{
    TObjArray* array=new TObjArray();

    if(strcmp(TabWidget2->currentPage()->name(),"layer")==0)
    {
	array->AddLast(new TObjString("hMdclayer"));
    }
    else if(strcmp(TabWidget2->currentPage()->name(),"tdc")==0)
    {    
	array->AddLast(new TObjString("hMdcmbotdc"));
    }
    else if(strcmp(TabWidget2->currentPage()->name(),"calibration")==0)
    {    
  	if(calibrationselect->currentItem()==0) // tdc
	{
	    array->AddLast(new TObjString("hMdcmbotdcCalib"));
	}
  	else if(calibrationselect->currentItem()==1) // time stat mod
	{
	    array->AddLast(new TObjString("hMdcmodtimestatCalib"));
	}
    }
    else if(strcmp(TabWidget2->currentPage()->name(),"mbo")==0)
    {
	if(mbselect->currentItem()==0) // stat
	{
	    array->AddLast(new TObjString("hMdcMbo"));
	}
	else if(mbselect->currentItem()==1) // trend for stat
	{
	    array->AddLast(new TObjString("hMdcMboTrendMod0"));
	    array->AddLast(new TObjString("hMdcMboTrendMod1"));
	    array->AddLast(new TObjString("hMdcMboTrendMod2"));
	    array->AddLast(new TObjString("hMdcMboTrendMod3"));
	}
    }
    else if(strcmp(TabWidget2->currentPage()->name(),"times")==0)
    {    
	if(levelselect->currentItem ()==0)     // raw time
	{
	    if(timesselect->currentItem()==0)  // time1
	    {
		array->AddLast(new TObjString("hMdctime1rawMod0"));
		array->AddLast(new TObjString("hMdctime1rawMod1"));
		array->AddLast(new TObjString("hMdctime1rawMod2"));
		array->AddLast(new TObjString("hMdctime1rawMod3"));
	    }
            else if(timesselect->currentItem()==1) // time2
	    {
		array->AddLast(new TObjString("hMdctime2rawMod0"));
		array->AddLast(new TObjString("hMdctime2rawMod1"));
		array->AddLast(new TObjString("hMdctime2rawMod2"));
		array->AddLast(new TObjString("hMdctime2rawMod3"));
	    }
            else if(timesselect->currentItem()==2) // time2-time1
	    {
		array->AddLast(new TObjString("hMdctime12rawMod0"));
		array->AddLast(new TObjString("hMdctime12rawMod1"));
		array->AddLast(new TObjString("hMdctime12rawMod2"));
		array->AddLast(new TObjString("hMdctime12rawMod3"));
	    }
            else if(timesselect->currentItem()==3) // select specific module
	    {
		if(moduleselect->value()==0)
		{
                  array->AddLast(new TObjString("hMdctime1rawMod0"));
		  array->AddLast(new TObjString("hMdctime2rawMod0"));
		  array->AddLast(new TObjString("hMdctime12rawMod0"));
	          array->AddLast(new TObjString("hMdctime1_12raw"));
		}
                else if(moduleselect->value()==1)
		{
                  array->AddLast(new TObjString("hMdctime1rawMod1"));
		  array->AddLast(new TObjString("hMdctime2rawMod1"));
		  array->AddLast(new TObjString("hMdctime12rawMod1"));
	          array->AddLast(new TObjString("hMdctime1_12raw"));
		}
                else if(moduleselect->value()==2)
		{
                  array->AddLast(new TObjString("hMdctime1rawMod2"));
		  array->AddLast(new TObjString("hMdctime2rawMod2"));
		  array->AddLast(new TObjString("hMdctime12rawMod2"));
	          array->AddLast(new TObjString("hMdctime1_12raw"));
		}
                else if(moduleselect->value()==3)
		{
                  array->AddLast(new TObjString("hMdctime1rawMod3"));
		  array->AddLast(new TObjString("hMdctime2rawMod3"));
		  array->AddLast(new TObjString("hMdctime12rawMod3"));
	          array->AddLast(new TObjString("hMdctime1_12raw"));
		}
	    }
	}
	if(levelselect->currentItem ()==1)  // cal times
	{
	    if(timesselect->currentItem()==0)  // time1
	    {
		array->AddLast(new TObjString("hMdctime1Mod0"));
		array->AddLast(new TObjString("hMdctime1Mod1"));
		array->AddLast(new TObjString("hMdctime1Mod2"));
		array->AddLast(new TObjString("hMdctime1Mod3"));
	    }
            else if(timesselect->currentItem()==1) // time2
	    {
		array->AddLast(new TObjString("hMdctime2Mod0"));
		array->AddLast(new TObjString("hMdctime2Mod1"));
		array->AddLast(new TObjString("hMdctime2Mod2"));
		array->AddLast(new TObjString("hMdctime2Mod3"));
	    }
            else if(timesselect->currentItem()==2) // time2-time1
	    {
		array->AddLast(new TObjString("hMdctime12Mod0"));
		array->AddLast(new TObjString("hMdctime12Mod1"));
		array->AddLast(new TObjString("hMdctime12Mod2"));
		array->AddLast(new TObjString("hMdctime12Mod3"));
	    }
            else if(timesselect->currentItem()==3) // select specific module
	    {
		if(moduleselect->value()==0)
		{
                  array->AddLast(new TObjString("hMdctime1Mod0"));
		  array->AddLast(new TObjString("hMdctime2Mod0"));
		  array->AddLast(new TObjString("hMdctime12Mod0"));
	          array->AddLast(new TObjString("hMdctime1_12"));
		}
                else if(moduleselect->value()==1)
		{
                  array->AddLast(new TObjString("hMdctime1Mod1"));
		  array->AddLast(new TObjString("hMdctime2Mod1"));
		  array->AddLast(new TObjString("hMdctime12Mod1"));
	          array->AddLast(new TObjString("hMdctime1_12"));
		}
                else if(moduleselect->value()==2)
		{
                  array->AddLast(new TObjString("hMdctime1Mod2"));
		  array->AddLast(new TObjString("hMdctime2Mod2"));
		  array->AddLast(new TObjString("hMdctime12Mod2"));
	          array->AddLast(new TObjString("hMdctime1_12"));
		}
                else if(moduleselect->value()==3)
		{
                  array->AddLast(new TObjString("hMdctime1Mod3"));
		  array->AddLast(new TObjString("hMdctime2Mod3"));
		  array->AddLast(new TObjString("hMdctime12Mod3"));
	          array->AddLast(new TObjString("hMdctime1_12"));
		}
	    }
	    else if(timesselect->currentItem()==4)  // <t1> trend
	    {
		array->AddLast(new TObjString("hMdctime1Cal1MeanTrendMod0"));
		array->AddLast(new TObjString("hMdctime1Cal1MeanTrendMod1"));
		array->AddLast(new TObjString("hMdctime1Cal1MeanTrendMod2"));
		array->AddLast(new TObjString("hMdctime1Cal1MeanTrendMod3"));
	    }
	}

    }
    array->AddLast(new TObjString("hMdcrawError_Stat"));
    array->AddLast(new TObjString("hMdcrawRoc_Subev"));
    array->AddLast(new TObjString("hMdccutstat"));
    array->AddLast(new TObjString("hMdccal1hits"));
    array->AddLast(new TObjString("hMdctime1Cal1"));
    array->AddLast(new TObjString("hMdctime1Cal1MeanTrend"));

    array->Expand(array->GetLast()+1);
    TGo4MdcActiveParameter* active = new TGo4MdcActiveParameter("MdcActiveHists","MdcActiveHists");
    active->SetPar(array);
    TGo4ComSetParameter* com=new TGo4ComSetParameter("MdcActiveHists");
    com->SetParameter(active);

    fxTGo4GUIRegistry->SubmitCommand(com);

    array->Delete();
}

void TGo4MdcOnline::DrawColors(Int_t size,Int_t symbol,HMonHistAddon* His)
{
    if(strcmp(His->ClassName(),"HHistArray" )==0)
    {
	HHistArray*  h=(HHistArray*)His;
	if(colorsSec[0]==0&&size==6&&symbol==0)for(Int_t i=0;i<h->getSize2();i++){colorsSec[i]=h->getP(0,i)->GetLineColor();}
	if(colorsMod[0]==0&&size==4&&symbol==1)for(Int_t i=0;i<h->getSize2();i++){colorsMod[i]=h->getP(0,i)->GetFillColor();}
	if(colorsSec[0]==0&&size==6&&symbol==0)for(Int_t i=0;i<h->getSize2();i++){colorsSec[i]=h->getP(0,i)->GetLineColor();}
	if(colorsMod[0]==0&&size==4&&symbol==1)for(Int_t i=0;i<h->getSize2();i++){colorsMod[i]=h->getP(0,i)->GetFillColor();}
    }
    else if(strcmp(His->ClassName(),"HTrendArray")==0)
    {
	HTrendArray* h=(HTrendArray*)His;
	if(colorsSec[0]==0&&size==6&&symbol==0)for(Int_t i=0;i<h->getSize2();i++){colorsSec[i]=h->getP(0,i)->GetLineColor();}
	if(colorsMod[0]==0&&size==4&&symbol==1)for(Int_t i=0;i<h->getSize2();i++){colorsMod[i]=h->getP(0,i)->GetFillColor();}
	if(colorsSec[0]==0&&size==6&&symbol==0)for(Int_t i=0;i<h->getSize2();i++){colorsSec[i]=h->getP(0,i)->GetLineColor();}
	if(colorsMod[0]==0&&size==4&&symbol==1)for(Int_t i=0;i<h->getSize2();i++){colorsMod[i]=h->getP(0,i)->GetFillColor();}
    }
    if(size==6&&colorsSec[0]!=0)
    {
	TSeqCollection * ColorTable= gROOT->GetListOfColors();
	TColor* col;

	for(Int_t i=0;i<6;i++)
	{
	    col=(TColor*)ColorTable->At(colorsSec[i]);
	    colorsSector[i]->setRgb((Int_t)(col->GetRed()*255),(Int_t)(col->GetGreen()*255),(Int_t)(col->GetBlue()*255));
	}
    }
    if(size==4&&colorsMod[0]!=0)
    {
	TSeqCollection * ColorTable= gROOT->GetListOfColors();
	TColor* col;

	for(Int_t i=0;i<4;i++)
	{
	    col=(TColor*)ColorTable->At(colorsMod[i]);
	    colorsModule[i]->setRgb((Int_t)(col->GetRed()*255),(Int_t)(col->GetGreen()*255),(Int_t)(col->GetBlue()*255));
	}
	colorsModule[4]->setRgb(223,223,223);
	colorsModule[5]->setRgb(223,223,223);
    }
    if(size==6)
    {
	ColorLabel0->setText("S0");
        ColorLabel1->setText("S1");
        ColorLabel2->setText("S2");
        ColorLabel3->setText("S3");
        ColorLabel4->setText("S4");
        ColorLabel5->setText("S5");
	ColorLabel0->setPaletteBackgroundColor(*colorsSector[0]);
	ColorLabel1->setPaletteBackgroundColor(*colorsSector[1]);
	ColorLabel2->setPaletteBackgroundColor(*colorsSector[2]);
	ColorLabel3->setPaletteBackgroundColor(*colorsSector[3]);
	ColorLabel4->setPaletteBackgroundColor(*colorsSector[4]);
	ColorLabel5->setPaletteBackgroundColor(*colorsSector[5]);
    }
    else
    {
       	ColorLabel0->setText("M0");
        ColorLabel1->setText("M1");
        ColorLabel2->setText("M2");
        ColorLabel3->setText("M3");
        ColorLabel4->setText("--");
        ColorLabel5->setText("--");
	ColorLabel0->setPaletteBackgroundColor(*colorsModule[0]);
	ColorLabel1->setPaletteBackgroundColor(*colorsModule[1]);
	ColorLabel2->setPaletteBackgroundColor(*colorsModule[2]);
	ColorLabel3->setPaletteBackgroundColor(*colorsModule[3]);
	ColorLabel4->setPaletteBackgroundColor(*colorsModule[4]);
	ColorLabel5->setPaletteBackgroundColor(*colorsModule[5]);
    }
}
void TGo4MdcOnline::DrawCalibration(HMonHistAddon* His)
{
    Int_t mod=moduleselect->value();

    if(calibrationselect->currentItem ()==0) // tdc
    {
	if(strcmp(His->GetName(),"hMdcmbotdcCalib")==0)
	{
	    HHistArray2* h=(HHistArray2*)His;

	    MdcfxQtRootCanvas1_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_6->getCanvas());
	    ((TH2*)(h->getP(0,mod)))->Draw();
	    MdcfxQtRootCanvas1_6->getCanvas()->Update();

	    MdcfxQtRootCanvas2_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_6->getCanvas());
	    ((TH2*)(h->getP(1,mod)))->Draw();
	    MdcfxQtRootCanvas2_6->getCanvas()->Update();

	    MdcfxQtRootCanvas3_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_6->getCanvas());
	    ((TH2*)(h->getP(2,mod)))->Draw();
	    MdcfxQtRootCanvas3_6->getCanvas()->Update();

	    MdcfxQtRootCanvas4_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_6->getCanvas());
	    ((TH2*)(h->getP(3,mod)))->Draw();
	    MdcfxQtRootCanvas4_6->getCanvas()->Update();

	    MdcfxQtRootCanvas5_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas5_6->getCanvas());
	    ((TH2*)(h->getP(4,mod)))->Draw();
	    MdcfxQtRootCanvas5_6->getCanvas()->Update();

	    MdcfxQtRootCanvas6_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas6_6->getCanvas());
	    ((TH2*)(h->getP(5,mod)))->Draw();
	    MdcfxQtRootCanvas6_6->getCanvas()->Update();
	}
    }
    else if(calibrationselect->currentItem ()==1) // time stat mod
    {
	if(strcmp(His->GetName(),"hMdcmodtimestatCalib")==0)
	{
	    DrawColors(4,1,His);
	    HHistArray* h=(HHistArray*)His;
	    MdcfxQtRootCanvas1_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas1_6->getCanvas());
	    ((TH1*)(h->getP(0,mod)))->Draw();
	    MdcfxQtRootCanvas1_6->getCanvas()->Update();

	    MdcfxQtRootCanvas2_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas2_6->getCanvas());
	    ((TH1*)(h->getP(1,mod)))->Draw();
	    MdcfxQtRootCanvas2_6->getCanvas()->Update();

	    MdcfxQtRootCanvas3_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas3_6->getCanvas());
	    ((TH1*)(h->getP(2,mod)))->Draw();
	    MdcfxQtRootCanvas3_6->getCanvas()->Update();

	    MdcfxQtRootCanvas4_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas4_6->getCanvas());
	    ((TH1*)(h->getP(3,mod)))->Draw();
	    MdcfxQtRootCanvas4_6->getCanvas()->Update();

	    MdcfxQtRootCanvas5_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas5_6->getCanvas());
	    ((TH1*)(h->getP(4,mod)))->Draw();
	    MdcfxQtRootCanvas5_6->getCanvas()->Update();

	    MdcfxQtRootCanvas6_6->getCanvas()->cd();
	    His->setCanvas(MdcfxQtRootCanvas6_6->getCanvas());
	    ((TH1*)(h->getP(5,mod)))->Draw();
	    MdcfxQtRootCanvas6_6->getCanvas()->Update();
	}
    }
}
