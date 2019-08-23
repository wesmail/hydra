/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

#define SHW_TAB_STATUS 3
#define SHW_TAB_CAL 2
#define SHW_TAB_HIT 1

void TGo4ShowerOnline::init()
{
	char buf[80];
	setCaption(" Shower Go4 Online " );
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/
    fxTGo4ShowerOnlineSlots = 
	dynamic_cast <TGo4ShowerOnlineSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4ShowerOnlineSlots"));   
    
    /* test if  a Slot Object already exist for this GUI*/
	if(fxTGo4ShowerOnlineSlots  !=0) {
   		/* if it is already there, set this GUI  as window of the slot Object */
		fxTGo4ShowerOnlineSlots->SetGUI(this);
	}
	else{
   		/* if no Slot Object where found creat a new one*/
		fxTGo4ShowerOnlineSlots = 
			new TGo4ShowerOnlineSlots ("TGo4ShowerOnlineSlots" , "Slot Class for Qt GUI", this);
 	}
  	/* Get the pointer for the Status Object */
	fxTGo4ShowerOnlineStatus= dynamic_cast <TGo4ShowerOnlineStatus *> (fxTGo4ShowerOnlineSlots->GetStatus());

    //########################### REGISTER HISTS AND OBJECTS ###################################
	fxTGo4GUIRegistry->ReDirectObject("ShowerConfig",fxTGo4ShowerOnlineSlots, true);   
	for(Int_t m=0;m<3;m++){
        sprintf(buf,"hCalCols_%d",m);
		fxTGo4GUIRegistry->ReDirectObject(buf,fxTGo4ShowerOnlineSlots, true);		
		sprintf(buf,"hCalRows_%d",m);
		fxTGo4GUIRegistry->ReDirectObject(buf,fxTGo4ShowerOnlineSlots, true);		
	}
    fxTGo4GUIRegistry->
		ReDirectObject("hShowerhits_raw",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas1_1);
	fxTGo4GUIRegistry->
		ReDirectObject("hShowerhits_cal",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas2_1);
	fxTGo4GUIRegistry->
		ReDirectObject("hShowerhits_hit",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas3_1);
	fxTGo4GUIRegistry->
		ReDirectObject("hSums_0",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas1_3);
	fxTGo4GUIRegistry->
		ReDirectObject("hSums_1",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas2_3);
	fxTGo4GUIRegistry->
		ReDirectObject("hSums_2",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas3_3);
	fxTGo4GUIRegistry->
		ReDirectObject("hSums_3",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas4_3);
	fxTGo4GUIRegistry->
		ReDirectObject("hSums_4",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas5_3);
	fxTGo4GUIRegistry->
		ReDirectObject("hSums_5",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas6_3);
	for(Int_t s=0;s<6;s++){
		sprintf(buf,"hSumTrend_%d",s);
		fxTGo4GUIRegistry->ReDirectObject(buf,fxTGo4ShowerOnlineSlots, true);
	}
    fxTGo4GUIRegistry->
		ReDirectObject("hRawMultTrend",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas4_1);
    fxTGo4GUIRegistry->
		ReDirectObject("hCalMultTrend",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas5_1);
    fxTGo4GUIRegistry->
		ReDirectObject("hHitMultTrend",fxTGo4ShowerOnlineSlots, true,ShowerfxQtRootCanvas6_1);
}

void TGo4ShowerOnline::destroy()
{
	char buf[80];
   //########################### UNREGISTER HISTS ###################################
    fxTGo4GUIRegistry->RemoveRedirectEntry("ShowerConfig",fxTGo4ShowerOnlineSlots); 
    fxTGo4GUIRegistry->RemoveRedirectEntry("hShowerhits_raw",fxTGo4ShowerOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hShowerhits_cal",fxTGo4ShowerOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hShowerhits_hit",fxTGo4ShowerOnlineSlots);
	for(Int_t s=0;s<6;s++){
    	sprintf(buf,"hSums_%d",s);
		fxTGo4GUIRegistry->RemoveRedirectEntry(buf,fxTGo4ShowerOnlineSlots);
		sprintf(buf,"hSumTrend_%d",s);
   		fxTGo4GUIRegistry->RemoveRedirectEntry(buf,fxTGo4ShowerOnlineSlots);
    }
	for(Int_t m=0;m<3;m++){
		sprintf(buf,"hCalCols_%d",m);
		fxTGo4GUIRegistry->RemoveRedirectEntry(buf,fxTGo4ShowerOnlineSlots);
		sprintf(buf,"hCalRows_%d",m);
    	fxTGo4GUIRegistry->RemoveRedirectEntry(buf,fxTGo4ShowerOnlineSlots);
  	} 
    fxTGo4GUIRegistry->RemoveRedirectEntry("hCalCol",fxTGo4ShowerOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hCalRow",fxTGo4ShowerOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hRawMultTrend",fxTGo4ShowerOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hCalMultTrend",fxTGo4ShowerOnlineSlots);
    fxTGo4GUIRegistry->RemoveRedirectEntry("hHitMultTrend",fxTGo4ShowerOnlineSlots);
  
}

void TGo4ShowerOnline::closeEvent( QCloseEvent * ce ) {
	TGo4LockGuard glob;
	ce->accept();
	fxTGo4ShowerOnlineSlots->SetGUI(0);
}

void TGo4ShowerOnline::ShowerDisplayLCD1( float t ){
	ShowerLCDNumber1->display(t);
}

void TGo4ShowerOnline::ShowerDisplayLCD2( float t ) {
	ShowerLCDNumber2->display(t);
}

void TGo4ShowerOnline::ShowerDisplayLCD3( float t ) {
	ShowerLCDNumber3->display(t);
}

void TGo4ShowerOnline::DrawHis( HMonHistAddon* His ) {
	//Draw all in SHOWER_CAL_TAB
	if(ShowerTab->currentPageIndex()==SHW_TAB_CAL){
		int currentItem = ShowerSectorsCombo->currentItem();
		if (strcmp(His->GetName(),"hCalRows_0") ==  0){
			ShowerfxQtRootCanvas1_2->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas1_2->getCanvas());
			if( currentItem==0 ){
	 			((HHistArray*)His)->drawAll();
			}
			else{
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->SetMaximum();
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->Draw(); 
			}
			ShowerfxQtRootCanvas1_2->getCanvas()->Update();
		}
		if (strcmp(His->GetName(),"hCalRows_1") ==  0){
			ShowerfxQtRootCanvas2_2->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas2_2->getCanvas());
			if( currentItem==0 ){
	 			((HHistArray*)His)->drawAll();
			}
			else{
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->SetMaximum();
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->Draw(); 
			}
			ShowerfxQtRootCanvas2_2->getCanvas()->Update();
		}
		if (strcmp(His->GetName(),"hCalRows_2") ==  0){
			ShowerfxQtRootCanvas3_2->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas3_2->getCanvas());
			if( currentItem==0 ){
	 			((HHistArray*)His)->drawAll();
			}
			else{
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->SetMaximum();
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->Draw(); 
			}
			ShowerfxQtRootCanvas3_2->getCanvas()->Update();
		}	
		if (strcmp(His->GetName(),"hCalCols_0") ==  0){
			ShowerfxQtRootCanvas4_2->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas4_2->getCanvas());
			if( currentItem==0 ){
	 			((HHistArray*)His)->drawAll();
			}
			else{
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->SetMaximum();
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->Draw(); 
			}
			ShowerfxQtRootCanvas4_2->getCanvas()->Update();
		}
		if (strcmp(His->GetName(),"hCalCols_1") ==  0){
			ShowerfxQtRootCanvas5_2->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas5_2->getCanvas());
			if( currentItem==0 ){
	 			((HHistArray*)His)->drawAll();
			}
			else{
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->SetMaximum();
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->Draw(); 
			}
			ShowerfxQtRootCanvas5_2->getCanvas()->Update();
		}
		if (strcmp(His->GetName(),"hCalCols_2") ==  0){
			ShowerfxQtRootCanvas6_2->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas6_2->getCanvas());
			if( currentItem==0 ){
	 			((HHistArray*)His)->drawAll();
			}
			else{
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->SetMaximum();
				((TH1F*)(((HHistArray*)His)->getP(0,currentItem-1)))->Draw(); 
			}
			ShowerfxQtRootCanvas6_2->getCanvas()->Update();
		}	
	
	}	
	if(ShowerTab->currentPageIndex()==SHW_TAB_STATUS){
		if (strcmp(His->GetName(),"hSumTrend_0")==0){
			ShowerfxQtRootCanvas1_4->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas1_4->getCanvas());
			((HTrendArray*)His)->drawAll();
			ShowerfxQtRootCanvas1_4->getCanvas()->Update();
		}
		if (strcmp(His->GetName(),"hSumTrend_1")==0){
			ShowerfxQtRootCanvas2_4->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas2_4->getCanvas());
			((HTrendArray*)His)->drawAll();
			ShowerfxQtRootCanvas2_4->getCanvas()->Update();
		}
		if (strcmp(His->GetName(),"hSumTrend_2")==0){
			ShowerfxQtRootCanvas3_4->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas3_4->getCanvas());
			((HTrendArray*)His)->drawAll();
			ShowerfxQtRootCanvas3_4->getCanvas()->Update();
		}
		if (strcmp(His->GetName(),"hSumTrend_3")==0){
			ShowerfxQtRootCanvas4_4->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas4_4->getCanvas());
			((HTrendArray*)His)->drawAll();
			ShowerfxQtRootCanvas4_4->getCanvas()->Update();
		}
		if (strcmp(His->GetName(),"hSumTrend_4")==0){
			ShowerfxQtRootCanvas5_4->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas5_4->getCanvas());
			((HTrendArray*)His)->drawAll();
			ShowerfxQtRootCanvas5_4->getCanvas()->Update();
		}
		if (strcmp(His->GetName(),"hSumTrend_5")==0){
			ShowerfxQtRootCanvas6_4->getCanvas()->cd();
			His->setCanvas(ShowerfxQtRootCanvas6_4->getCanvas());
			((HTrendArray*)His)->drawAll();
			ShowerfxQtRootCanvas6_4->getCanvas()->Update();
		}
	}
}

void TGo4ShowerOnline::Reset()
{
     TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
     reset->SetShowerChanged(kTRUE);
     reset->Set("Shower",0);     
     TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
     com->SetParameter(reset);
     fxTGo4GUIRegistry->SubmitCommand(com);
	
     //fxTGo4GUIRegistry->SubmitCommand("ANClearObject") ;
}







void TGo4ShowerOnline::DrawLegend()
{
   
}


void TGo4ShowerOnline::ShowerRawDisplay(int *n)
{
    
    if (ShowerTab->currentPageIndex()==SHW_TAB_STATUS){
	ShowerRawLCDNumber->display(n[6]/500.);
	ProgressRaw1->setProgress((float)n[0]/float(n[6])*100.0);
	ProgressRaw2->setProgress((float)n[1]/float(n[6])*100.0);
	ProgressRaw3->setProgress((float)n[2]/float(n[6])*100.0);
	ProgressRaw4->setProgress((float)n[3]/float(n[6])*100.0);
	ProgressRaw5->setProgress((float)n[4]/float(n[6])*100.0);
	ProgressRaw6->setProgress((float)n[5]/float(n[6])*100.0);
   }
}
void TGo4ShowerOnline::ShowerCalDisplay(int *n)
{
    
    if (ShowerTab->currentPageIndex()==SHW_TAB_STATUS){
	 ShowerCalLCDNumber->display(n[6]/500.);
	ProgressCal1->setProgress((float)n[0]/float(n[6])*100.0);
	ProgressCal2->setProgress((float)n[1]/float(n[6])*100.0);
	ProgressCal3->setProgress((float)n[2]/float(n[6])*100.0);
	ProgressCal4->setProgress((float)n[3]/float(n[6])*100.0);
	ProgressCal5->setProgress((float)n[4]/float(n[6])*100.0);
	ProgressCal6->setProgress((float)n[5]/float(n[6])*100.0);
   }
}
void TGo4ShowerOnline::ShowerHitDisplay(int *n)
{
    
    if (ShowerTab->currentPageIndex()==SHW_TAB_STATUS){
	ShowerHitLCDNumber->display(n[6]/500.);
	ProgressHit1->setProgress((float)n[0]/float(n[6])*100.0);
	ProgressHit2->setProgress((float)n[1]/float(n[6])*100.0);
	ProgressHit3->setProgress((float)n[2]/float(n[6])*100.0);
	ProgressHit4->setProgress((float)n[3]/float(n[6])*100.0);
	ProgressHit5->setProgress((float)n[4]/float(n[6])*100.0);
	ProgressHit6->setProgress((float)n[5]/float(n[6])*100.0);
   }
}






void TGo4ShowerOnline::DrawSectors( int n )
{
}


void TGo4ShowerOnline::printCanvas()
{
//
//    TGo4PrintCanvas ala;
//    ala.show();
//    if(ala.exec())
//    ShowerfxQtRootCanvas3_2->getCanvas()->Print(ala.LineEdit->text()); 
    
}
