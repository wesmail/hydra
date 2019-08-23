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
  
    TList* ActivePads = new TList();
    ActivePads->SetOwner(kTRUE);
    //########################### REGISTER HISTS ###################################
    fxTGo4GUIRegistry->ReDirectObject("hRichPulseheight",fxTGo4RichOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hRichMultiplicity",fxTGo4RichOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("hRichPulseDist",fxTGo4RichOnlineSlots, true);
    fxTGo4GUIRegistry->ReDirectObject("RichOnlineDisplay",fxTGo4RichOnlineSlots, true);	    
    fxTGo4GUIRegistry->ReDirectObject("RichPattern",fxTGo4RichOnlineSlots, true);	    
}

void TGo4RichOnline::destroy()
{
   //########################### UNREGISTER HISTS ###################################
    fxTGo4GUIRegistry->RemoveRedirectEntry("hRichpulsheight",fxTGo4RichOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hRichPulseheight",fxTGo4RichOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hRichMultiplicity",fxTGo4RichOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hRichPulseDist",fxTGo4RichOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("RichOnlineDisplay",fxTGo4RichOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("RichPattern",fxTGo4RichOnlineSlots);
}

void TGo4RichOnline::closeEvent( QCloseEvent * ce )
{
   TGo4LockGuard glob;
    ce->accept();
    fxTGo4RichOnlineSlots->SetGUI(0);
}

void TGo4RichOnline::RichDisplayLCD1( Float_t t )
{
    RichLCDNumber1->display(t);
}

void TGo4RichOnline::RichDisplayLCD2( Float_t t )
{
 RichLCDNumber2->display(t);
}

void TGo4RichOnline::RichDisplayLCD3( Float_t t )
{
    RichLCDNumber3->display(t);
}

void TGo4RichOnline::DrawHis( HMonHistAddon* His )
{
    if (strcmp(His->GetName(),"hRichPulseheight")==0)
    {
	RichfxQtRootCanvas1_1->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas1_1->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,0)))->Draw();
	RichfxQtRootCanvas1_1->getCanvas()->Update();
	
	RichfxQtRootCanvas1_2->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas1_2->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,1)))->Draw();
	RichfxQtRootCanvas1_2->getCanvas()->Update();
	
	RichfxQtRootCanvas1_3->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas1_3->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,2)))->Draw();
	RichfxQtRootCanvas1_3->getCanvas()->Update();
	
	RichfxQtRootCanvas1_4->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas1_4->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,3)))->Draw();
	RichfxQtRootCanvas1_4->getCanvas()->Update();
	
	RichfxQtRootCanvas1_5->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas1_5->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,4)))->Draw();
	RichfxQtRootCanvas1_5->getCanvas()->Update();
	
	RichfxQtRootCanvas1_6->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas1_6->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,5)))->Draw();
	RichfxQtRootCanvas1_6->getCanvas()->Update();
    }
    
    if (strcmp(His->GetName(),"hRichPulseDist")==0)
    {
	RichfxQtRootCanvas3_1->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas3_1->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,0)))->Draw();
	RichfxQtRootCanvas3_1->getCanvas()->Update();
	
	RichfxQtRootCanvas3_2->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas3_2->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,1)))->Draw();
	RichfxQtRootCanvas3_2->getCanvas()->Update();
	
	RichfxQtRootCanvas3_3->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas3_3->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,2)))->Draw();
	RichfxQtRootCanvas3_3->getCanvas()->Update();
	
	RichfxQtRootCanvas3_4->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas3_4->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,3)))->Draw();
	RichfxQtRootCanvas3_4->getCanvas()->Update();
	
	RichfxQtRootCanvas3_5->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas3_5->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,4)))->Draw();
	RichfxQtRootCanvas3_5->getCanvas()->Update();
	
	RichfxQtRootCanvas3_6->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas3_6->getCanvas());
	((TH2F*)(((HHistArray2*)His)->getP(0,5)))->Draw();
	RichfxQtRootCanvas3_6->getCanvas()->Update();
    }
    
     if (strcmp(His->GetName(),"hRichMultiplicity")==0)
    {
	RichfxQtRootCanvas2_1->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas2_1->getCanvas());
	((TH1F*)(((HTrendArray*)His)->getP(0,0)))->Draw();
	RichfxQtRootCanvas2_1->getCanvas()->Update();
	
	RichfxQtRootCanvas2_2->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas2_2->getCanvas());
	((TH1F*)(((HTrendArray*)His)->getP(0,1)))->Draw();
	RichfxQtRootCanvas2_2->getCanvas()->Update();
	
	RichfxQtRootCanvas2_3->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas2_3->getCanvas());
	((TH1F*)(((HTrendArray*)His)->getP(0,2)))->Draw();
	RichfxQtRootCanvas2_3->getCanvas()->Update();
	
	RichfxQtRootCanvas2_4->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas2_4->getCanvas());
	((TH1F*)(((HTrendArray*)His)->getP(0,3)))->Draw();
	RichfxQtRootCanvas2_4->getCanvas()->Update();
	
	RichfxQtRootCanvas2_5->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas2_5->getCanvas());
	((TH1F*)(((HTrendArray*)His)->getP(0,4)))->Draw();
	RichfxQtRootCanvas2_5->getCanvas()->Update();
	
	RichfxQtRootCanvas2_6->getCanvas()->cd();
             His->setCanvas(RichfxQtRootCanvas2_6->getCanvas());
	((TH1F*)(((HTrendArray*)His)->getP(0,5)))->Draw();
	RichfxQtRootCanvas2_6->getCanvas()->Update();
    }
    
}

void TGo4RichOnline::Reset()
{
  	TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
	reset->SetRichChanged(kTRUE);
	reset->Set("Rich",0);     
	TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
	com->SetParameter(reset);
	fxTGo4GUIRegistry->SubmitCommand(com);
}







void TGo4RichOnline::DrawFancy(HRichFancy* fancy)
{
/*    RichfxQtRootCanvas1_1->getCanvas()->cd();	
    fancy->draw();
    TheMesh=fancy;
    RichfxQtRootCanvas1_1->getCanvas()->Update();*/
}


void TGo4RichOnline::DrawPattern(HRichPattern * pat )
{
    Int_t sec, row, col, index, key, color;
    Float_t charge;
    HRichCal* currentCal;
    HRichCal* oldCal; 
    TListIter* currentIterator = new TListIter(pat->getNewList());
    TListIter* oldIterator = new TListIter(ActivePads);
    oldIterator->Reset();
    currentIterator->Reset();
    while((oldCal = (HRichCal*)oldIterator->Next())){
	sec=oldCal->getSector();
	row=oldCal->getRow();	
	col=oldCal->getCol();	
	key = 10000*sec+100*col+row;
	index = TheMesh->getMapping(key);
	TheMesh->getGeoNode(index) -> SetVisibility(0);
    }	
    ActivePads->Clear();
    while((currentCal = (HRichCal*)currentIterator->Next())){
	ActivePads->Add(new HRichCal(*currentCal));
	sec=currentCal->getSector();
	row=currentCal->getRow();		
	col=currentCal->getCol();		
	charge=currentCal->getCharge();	
	key = 10000*sec+100*col+row;
	index = TheMesh->getMapping(key);
	color = TMath::Nint(charge*0.5 + 50);
	if (color>100) color=99;
	TheMesh->getGeoNode(index) -> SetVisibility(1);	
	TheMesh->getGeoNode(index) -> SetLineColor(color);
	TheMesh->getGeoNode(index) -> SetLineWidth(1);
    }  
    delete currentIterator;
    delete oldIterator;
    RichfxQtRootCanvas_OD->getCanvas()->Modified();
    RichfxQtRootCanvas_OD->getCanvas()->Update();
 /*   DrawFancy(TheMesh);*/
}
