/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void TGo4PhysicsOnline::init()
{


    setCaption(" Physics Go4 Online " );
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/
    
    fxTGo4PhysicsOnlineSlots = dynamic_cast <TGo4PhysicsOnlineSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4PhysicsOnlineSlots"));   
    
    /* test if  a Slot Object already exist for this GUI*/
    
   if(fxTGo4PhysicsOnlineSlots  !=0) {
   /* if it is already there, set this GUI  as window of the slot Object */
		fxTGo4PhysicsOnlineSlots->SetGUI(this);
   }else{
   /* if no Slot Object where found creat a new one*/
		fxTGo4PhysicsOnlineSlots = new TGo4PhysicsOnlineSlots ("TGo4PhysicsOnlineSlots" , "Slot Class for Qt GUI", this);
   }
   /* Get the pointer for the Status Object */
    fxTGo4PhysicsOnlineStatus= dynamic_cast <TGo4PhysicsOnlineStatus *> (fxTGo4PhysicsOnlineSlots->GetStatus());
    
   //########################### REGISTER HISTS ###################################
    fxTGo4GUIRegistry->ReDirectObject("hMdcdedx"     ,fxTGo4PhysicsOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofdedx"     ,fxTGo4PhysicsOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hTofinodedx"  ,fxTGo4PhysicsOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hphimdctof"   ,fxTGo4PhysicsOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hphimdctofino",fxTGo4PhysicsOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hphimdcrich"  ,fxTGo4PhysicsOnlineSlots, true);


   //fxTGo4GUIRegistry->ReDirectObject("hPhysicsrawError_Stat",fxTGo4PhysicsOnlineSlots, true,PhysicsfxQtRootCanvas1);
}

void TGo4PhysicsOnline::destroy()
{
    //########################### UNREGISTER HISTS ###################################
    fxTGo4GUIRegistry->RemoveRedirectEntry("hMdcdedx"     ,fxTGo4PhysicsOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofdedx"     ,fxTGo4PhysicsOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hTofinodedx"  ,fxTGo4PhysicsOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hphimdctof"   ,fxTGo4PhysicsOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hphimdctofino",fxTGo4PhysicsOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hphimdcrich"  ,fxTGo4PhysicsOnlineSlots);
}

void TGo4PhysicsOnline::closeEvent( QCloseEvent * ce )
{
    TGo4LockGuard glob;
    ce->accept();
    fxTGo4PhysicsOnlineSlots->SetGUI(0);
}
void TGo4PhysicsOnline::DrawHis( HMonHistAddon* His )
{
    if(strncmp(MainTab->currentPage()->name(),"Correlations",strlen("Correlations"))==0)
    {    
	drawCorrelations(His);
    }
    if(strncmp(MainTab->currentPage()->name(),"dEdx",strlen("dEdx"))==0)
    {
	drawDeDx(His);
    }
}

void TGo4PhysicsOnline::Reset()
{
 	TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
	reset->SetPhysicsChanged(kTRUE);
	reset->Set("Physics",0);     
	TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
	com->SetParameter(reset);
	fxTGo4GUIRegistry->SubmitCommand(com);
}
void TGo4PhysicsOnline::drawDeDx( HMonHistAddon* His )
{
    if(strncmp(dEdxTab->currentPage()->name(),"Mdc_dEdx",strlen("Mdc_dEdx"))==0)
    {    
	if(strncmp(His->GetName(),"hMdcdedx",strlen(His->GetName()))==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    PhysicsfxQtRootCanvas1_mdcdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas1_mdcdedx->getCanvas());
	    ((TH1*)(h->getP(0,0)))->Draw();
	    PhysicsfxQtRootCanvas1_mdcdedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas2_mdcdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas2_mdcdedx->getCanvas());
	    ((TH1*)(h->getP(1,0)))->Draw();
	    PhysicsfxQtRootCanvas2_mdcdedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas3_mdcdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas3_mdcdedx->getCanvas());
	    ((TH1*)(h->getP(2,0)))->Draw();
	    PhysicsfxQtRootCanvas3_mdcdedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas4_mdcdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas4_mdcdedx->getCanvas());
	    ((TH1*)(h->getP(3,0)))->Draw();
	    PhysicsfxQtRootCanvas4_mdcdedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas5_mdcdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas5_mdcdedx->getCanvas());
	    ((TH1*)(h->getP(4,0)))->Draw();
	    PhysicsfxQtRootCanvas5_mdcdedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas6_mdcdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas6_mdcdedx->getCanvas());
	    ((TH1*)(h->getP(5,0)))->Draw();
	    PhysicsfxQtRootCanvas6_mdcdedx->getCanvas()->Update();

	}
    }
    if(strncmp(dEdxTab->currentPage()->name(),"Mdc_dEdx2",strlen("Mdc_dEdx2"))==0)
    {    
	if(strncmp(His->GetName(),"hMdcdedx",strlen(His->GetName()))==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    PhysicsfxQtRootCanvas1_mdcdedx2->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas1_mdcdedx2->getCanvas());
	    ((TH1*)(h->getP(0,1)))->Draw();
	    PhysicsfxQtRootCanvas1_mdcdedx2->getCanvas()->Update();

	    PhysicsfxQtRootCanvas2_mdcdedx2->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas2_mdcdedx2->getCanvas());
	    ((TH1*)(h->getP(1,1)))->Draw();
	    PhysicsfxQtRootCanvas2_mdcdedx2->getCanvas()->Update();

	    PhysicsfxQtRootCanvas3_mdcdedx2->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas3_mdcdedx2->getCanvas());
	    ((TH1*)(h->getP(2,1)))->Draw();
	    PhysicsfxQtRootCanvas3_mdcdedx2->getCanvas()->Update();

	    PhysicsfxQtRootCanvas4_mdcdedx2->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas4_mdcdedx2->getCanvas());
	    ((TH1*)(h->getP(3,1)))->Draw();
	    PhysicsfxQtRootCanvas4_mdcdedx2->getCanvas()->Update();

	    PhysicsfxQtRootCanvas5_mdcdedx2->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas5_mdcdedx2->getCanvas());
	    ((TH1*)(h->getP(4,1)))->Draw();
	    PhysicsfxQtRootCanvas5_mdcdedx2->getCanvas()->Update();

	    PhysicsfxQtRootCanvas6_mdcdedx2->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas6_mdcdedx2->getCanvas());
	    ((TH1*)(h->getP(5,1)))->Draw();
	    PhysicsfxQtRootCanvas6_mdcdedx2->getCanvas()->Update();

	}
    }
    if(strncmp(dEdxTab->currentPage()->name(),"Tof_dEdx",strlen("Tof_dEdx"))==0)
    {    
	if(strncmp(His->GetName(),"hTofdedx",strlen(His->GetName()))==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    PhysicsfxQtRootCanvas1_tofdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas1_tofdedx->getCanvas());
	    ((TH1*)(h->getP(0,0)))->Draw();
	    PhysicsfxQtRootCanvas1_tofdedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas2_tofdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas2_tofdedx->getCanvas());
	    ((TH1*)(h->getP(0,1)))->Draw();
	    PhysicsfxQtRootCanvas2_tofdedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas3_tofdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas3_tofdedx->getCanvas());
	    ((TH1*)(h->getP(0,2)))->Draw();
	    PhysicsfxQtRootCanvas3_tofdedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas4_tofdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas4_tofdedx->getCanvas());
	    ((TH1*)(h->getP(0,3)))->Draw();
	    PhysicsfxQtRootCanvas4_tofdedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas5_tofdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas5_tofdedx->getCanvas());
	    ((TH1*)(h->getP(0,4)))->Draw();
	    PhysicsfxQtRootCanvas5_tofdedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas6_tofdedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas6_tofdedx->getCanvas());
	    ((TH1*)(h->getP(0,5)))->Draw();
	    PhysicsfxQtRootCanvas6_tofdedx->getCanvas()->Update();

	}
    }
    if(strncmp(dEdxTab->currentPage()->name(),"Tofino_dEdx",strlen("Tofino_dEdx"))==0)
    {
	if(strncmp(His->GetName(),"hTofinodedx",strlen(His->GetName()))==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    PhysicsfxQtRootCanvas1_tofinodedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas1_tofinodedx->getCanvas());
	    ((TH1*)(h->getP(0,0)))->Draw();
	    PhysicsfxQtRootCanvas1_tofinodedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas2_tofinodedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas2_tofinodedx->getCanvas());
	    ((TH1*)(h->getP(0,1)))->Draw();
	    PhysicsfxQtRootCanvas2_tofinodedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas3_tofinodedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas3_tofinodedx->getCanvas());
	    ((TH1*)(h->getP(0,2)))->Draw();
	    PhysicsfxQtRootCanvas3_tofinodedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas4_tofinodedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas4_tofinodedx->getCanvas());
	    ((TH1*)(h->getP(0,3)))->Draw();
	    PhysicsfxQtRootCanvas4_tofinodedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas5_tofinodedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas5_tofinodedx->getCanvas());
	    ((TH1*)(h->getP(0,4)))->Draw();
	    PhysicsfxQtRootCanvas5_tofinodedx->getCanvas()->Update();

	    PhysicsfxQtRootCanvas6_tofinodedx->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas6_tofinodedx->getCanvas());
	    ((TH1*)(h->getP(0,5)))->Draw();
	    PhysicsfxQtRootCanvas6_tofinodedx->getCanvas()->Update();

	}
    }
}
void TGo4PhysicsOnline::drawCorrelations( HMonHistAddon* His )
{
    if(strncmp(CorrelationsTab->currentPage()->name(),"Deltaphi_Mdc_Tof",strlen("Deltaphi_Mdc_Tof"))==0)
    {    
	if(strncmp(His->GetName(),"hphimdctof",strlen(His->GetName()))==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    PhysicsfxQtRootCanvas1_tofphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas1_tofphi->getCanvas());
	    ((TH1*)(h->getP(0,0)))->Draw();
	    PhysicsfxQtRootCanvas1_tofphi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas2_tofphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas2_tofphi->getCanvas());
	    ((TH1*)(h->getP(0,1)))->Draw();
	    PhysicsfxQtRootCanvas2_tofphi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas3_tofphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas3_tofphi->getCanvas());
	    ((TH1*)(h->getP(0,2)))->Draw();
	    PhysicsfxQtRootCanvas3_tofphi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas4_tofphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas4_tofphi->getCanvas());
	    ((TH1*)(h->getP(0,3)))->Draw();
	    PhysicsfxQtRootCanvas4_tofphi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas5_tofphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas5_tofphi->getCanvas());
	    ((TH1*)(h->getP(0,4)))->Draw();
	    PhysicsfxQtRootCanvas5_tofphi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas6_tofphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas6_tofphi->getCanvas());
	    ((TH1*)(h->getP(0,5)))->Draw();
	    PhysicsfxQtRootCanvas6_tofphi->getCanvas()->Update();

	}
    }
    if(strncmp(CorrelationsTab->currentPage()->name(),"Deltaphi_Mdc_Tofino",strlen("Deltaphi_Mdc_Tofino"))==0)
    {
	if(strncmp(His->GetName(),"hphimdctofino",strlen(His->GetName()))==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    PhysicsfxQtRootCanvas1_tofinophi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas1_tofinophi->getCanvas());
	    ((TH1*)(h->getP(0,0)))->Draw();
	    PhysicsfxQtRootCanvas1_tofinophi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas2_tofinophi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas2_tofinophi->getCanvas());
	    ((TH1*)(h->getP(0,1)))->Draw();
	    PhysicsfxQtRootCanvas2_tofinophi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas3_tofinophi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas3_tofinophi->getCanvas());
	    ((TH1*)(h->getP(0,2)))->Draw();
	    PhysicsfxQtRootCanvas3_tofinophi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas4_tofinophi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas4_tofinophi->getCanvas());
	    ((TH1*)(h->getP(0,3)))->Draw();
	    PhysicsfxQtRootCanvas4_tofinophi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas5_tofinophi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas5_tofinophi->getCanvas());
	    ((TH1*)(h->getP(0,4)))->Draw();
	    PhysicsfxQtRootCanvas5_tofinophi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas6_tofinophi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas6_tofinophi->getCanvas());
	    ((TH1*)(h->getP(0,5)))->Draw();
	    PhysicsfxQtRootCanvas6_tofinophi->getCanvas()->Update();

	}
    }
    if(strncmp(CorrelationsTab->currentPage()->name(),"Deltaphi_Mdc_Rich",strlen("Deltaphi_Mdc_Rich"))==0)
    {
	if(strncmp(His->GetName(),"hphimdcrich",strlen(His->GetName()))==0)
	{
	    HHistArray* h=(HHistArray*)His;

	    PhysicsfxQtRootCanvas1_richphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas1_richphi->getCanvas());
	    ((TH1*)(h->getP(0,0)))->Draw();
	    PhysicsfxQtRootCanvas1_richphi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas2_richphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas2_richphi->getCanvas());
	    ((TH1*)(h->getP(0,1)))->Draw();
	    PhysicsfxQtRootCanvas2_richphi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas3_richphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas3_richphi->getCanvas());
	    ((TH1*)(h->getP(0,2)))->Draw();
	    PhysicsfxQtRootCanvas3_richphi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas4_richphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas4_richphi->getCanvas());
	    ((TH1*)(h->getP(0,3)))->Draw();
	    PhysicsfxQtRootCanvas4_richphi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas5_richphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas5_richphi->getCanvas());
	    ((TH1*)(h->getP(0,4)))->Draw();
	    PhysicsfxQtRootCanvas5_richphi->getCanvas()->Update();

	    PhysicsfxQtRootCanvas6_richphi->getCanvas()->cd();
	    His->setCanvas(PhysicsfxQtRootCanvas6_richphi->getCanvas());
	    ((TH1*)(h->getP(0,5)))->Draw();
	    PhysicsfxQtRootCanvas6_richphi->getCanvas()->Update();

	}
    }
}

/*
void DrawMain(HMonHistAddon* His)
{
    // example
    if(strcmp(His->GetName(),"hPhysicsBla")==0)
    {
	//DrawColors(6,0,His);
	HHistArray* h=(HHistArray*)His;
	PhysicsfxQtRootCanvas1_5->getCanvas()->cd();
	His->setCanvas(PhysicsfxQtRootCanvas1_5->getCanvas());
	h->drawAll();
	PhysicsfxQtRootCanvas1_5->getCanvas()->Update();
    }
}
*/

void TGo4PhysicsOnline::SetActiveHists()
{
    TObjArray* array=new TObjArray();

    if(strcmp(MainTab->currentPage()->name(),"main")==0)
    {
	array->AddLast(new TObjString("hPhysicsmain"));
    }

    array->Expand(array->GetLast()+1);
    TGo4PhysicsActiveParameter* active = new TGo4PhysicsActiveParameter("PhysicsActiveHists","PhysicsActiveHists");
    active->SetPar(array);
    TGo4ComSetParameter* com=new TGo4ComSetParameter("PhysicsActiveHists");
    com->SetParameter(active);

    fxTGo4GUIRegistry->SubmitCommand(com);

    array->Delete();
}

