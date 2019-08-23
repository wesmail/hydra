/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

void TGo4TriggerOnline::init()
{
  fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
  fxTGo4TriggerOnlineSlots = (TGo4TriggerOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4TriggerOnlineSlots");

  if(fxTGo4TriggerOnlineSlots  !=0) { fxTGo4TriggerOnlineSlots->SetGUI(this); }
  else                              { fxTGo4TriggerOnlineSlots = new TGo4TriggerOnlineSlots ("TGo4TriggerOnlineSlots" , "Slot Class for Qt GUI", this); }

  fxTGo4TriggerOnlineStatus= dynamic_cast <TGo4TriggerOnlineStatus *> (fxTGo4TriggerOnlineSlots->GetStatus());

  // --- one time array setting flags -----------------------------------
  tofArraySetting=kFALSE;
  richArraySetting=kFALSE;
  showerArraySetting=kFALSE;
  matchuArraySetting=kFALSE;
  tofTimeArraySetting=kFALSE;
  richTimeArraySetting=kFALSE;
  showerTimeArraySetting=kFALSE;
  matchuTimeArraySetting=kFALSE;
  multArraySetting=kFALSE;
  richRingArraySetting=kFALSE;
  tofHitArraySetting=kFALSE;
  showerHitArraySetting=kFALSE;
  matchuHitArraySetting=kFALSE;
  timeTrendSetting=kFALSE;
  tofCorrArraySettingsPhi = kFALSE;
  tofCorrArraySettingsTheta = kFALSE;
  showerCorrArraySettingsPhi=kFALSE;
  showerCorrArraySettingsTheta=kFALSE;
  richCorrArraySettingsPhi = kFALSE;
  richCorrArraySettingsTheta = kFALSE;
  muCorrArraySettingsPhi = kFALSE;
  muCorrArraySettingsTheta = kFALSE;
  butterfly=kFALSE;
  mulepmom=kFALSE;
  // --------------------------------------------------------------------
  timeAxis=kFALSE;
  noMoreNaming=kFALSE;
  newAxisColor=0;

  initCanvasColor( 1 );
  initLCDNumbers();

  //##########################REGISTER HISTS#########################################
  fxTGo4GUIRegistry->ReDirectObject("hTriggerIPUtrendRich"       ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerIPUtrendTof"        ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerIPUtrendShower"     ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerIPUtrendMatchU"     ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerTofNumbers"         ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerRichNumbers"        ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerMultDist"           ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerRichRingMultDist"   ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerTofHitMultDist"     ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerShowerHitMultDist"  ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerMatchUHitMultDist"  ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerTrendTime"          ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerIPUtrendRichTime"   ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerIPUtrendTofTime"    ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerIPUtrendShowerTime" ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerIPUtrendMatchUTime" ,fxTGo4TriggerOnlineSlots, true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerTofCorrPhi"         ,fxTGo4TriggerOnlineSlots ,true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerTofCorrTheta"       ,fxTGo4TriggerOnlineSlots ,true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerRichCorrPhi"        ,fxTGo4TriggerOnlineSlots ,true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerRichCorrTheta"      ,fxTGo4TriggerOnlineSlots ,true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerMUCorrPhi"          ,fxTGo4TriggerOnlineSlots ,true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerMUCorrTheta"        ,fxTGo4TriggerOnlineSlots ,true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerShowerCorrPhi"      ,fxTGo4TriggerOnlineSlots ,true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerShowerCorrTheta"    ,fxTGo4TriggerOnlineSlots ,true);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerIPUButterfly"       ,fxTGo4TriggerOnlineSlots ,true,fxQtCanvasButterfly);
  fxTGo4GUIRegistry->ReDirectObject("hTriggerMULepMom"           ,fxTGo4TriggerOnlineSlots ,true,fxQtCanvasLepMom);
  fxTGo4GUIRegistry->ReDirectObject("hTriggertof"                ,fxTGo4TriggerOnlineSlots ,true,fxQtCanvasTofTime);
}

void TGo4TriggerOnline::destroy()
{
  //########################### UNREGISTER HISTS ###################################
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerIPUtrendRich"      ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerIPUtrendTof"       ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerIPUtrendShower"    ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerIPUtrendMatchU"    ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerTofNumbers"        ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerRichNumbers"       ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerMultDist"          ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerRichRingMultDist"  ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerTofHitMultDist"    ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerShowerHitMultDist" ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerMatchUHitMultDist" ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerTrendTime"         ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerIPUtrendRichTime"  ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerIPUtrendTofTime"   ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerIPUtrendShowerTime",fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerIPUtrendMatchUTime",fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerTofCorrPhi"        ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerTofCorrTheta"      ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerRichCorrPhi"       ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerRichCorrTheta"     ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerMUCorrPhi"         ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerMUCorrTheta"       ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerShowerCorrPhi"     ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerShowerCorrTheta"   ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerIPUButterfly"      ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggerMULepMom"          ,fxTGo4TriggerOnlineSlots);
  fxTGo4GUIRegistry->RemoveRedirectEntry("hTriggertof"               ,fxTGo4TriggerOnlineSlots);
}


void TGo4TriggerOnline::closeEvent( QCloseEvent *ce )
{
  TGo4LockGuard glob;
  ce->accept();
  fxTGo4TriggerOnlineSlots->SetGUI(0);
  //delete this;
}

// -------------------------- Historgrams -----------------------------------------------------------
void TGo4TriggerOnline::DrawHis(TH1  *His )
{

}

void TGo4TriggerOnline::DrawHis( HMonHistAddon * His )
{
  if( His == 0 ) { return; }
  //cout << "draw his name: " << His->GetName() << endl;

  //--------- Take care of additional color settings
  if( (His ->getP()->GetXaxis()->GetAxisColor()) != (Color_t)newAxisColor ) { histColor( His ); }
  if(noMoreNaming==kFALSE) { checkNaming( His ); }
  if( His->InheritsFrom("HTrendArray") ) { arrayScaling( His ); }

  //---------- Draw histograms and lcds
  if(strcmp(TabWidget->currentPage()->name(),"Overview")==0)
    {
      drawOverviewTabNumbers( His );
      if(timeAxis == kFALSE) { drawOverviewTab( His );     }
      else                   { drawOverviewTabTime( His ); }
    }
  else if(strcmp(TabWidget->currentPage()->name(),"multdistr")==0)
    {
      drawMultDistTab( His );
    }
  else if( strcmp(TabWidget->currentPage()->name(),"Rich_sub")==0)
  {
    if( strcmp(TabWidgetRich->currentPage()->name(),"Rich_sect")==0 )
    {
      if(timeAxis == kFALSE) { drawSubTab( 0, His );     }
      else                   { drawSubTabTime( 0, His ); }
    }
    else if(strcmp(TabWidgetRich->currentPage()->name(),"Rich_corr_phi")==0 )
    {
      drawSubTabCorr(0,His);
    }
    else if(strcmp(TabWidgetRich->currentPage()->name(),"Rich_corr_theta")==0 )
    {
      drawSubTabCorr(0,His);
    }
  }
  else if( strcmp(TabWidget->currentPage()->name(),"Tof_sub")==0)
  {
      if(strcmp(TabWidgetTof->currentPage()->name(),"Tof_sect")==0 )
      {
       if(timeAxis == kFALSE) { drawSubTab( 1, His );     }
        else                   { drawSubTabTime( 1, His ); }
     }
    else if(strcmp(TabWidgetTof->currentPage()->name(),"Tof_corr_phi")==0 )
    {
      drawSubTabCorr(0,His);
    }
    else if(strcmp(TabWidgetTof->currentPage()->name(),"Tof_corr_theta")==0 )
    {
      drawSubTabCorr(0,His);
    }
  }
  else if( strcmp(TabWidget->currentPage()->name(),"Shower_sub")==0)
      if(strcmp(TabWidgetShower->currentPage()->name(),"Shower_sect")==0 )
    {
      if(timeAxis == kFALSE) { drawSubTab( 2, His );     }
      else                   { drawSubTabTime( 2, His ); }
    }
  else if(strcmp(TabWidgetShower->currentPage()->name(),"Shower_corr_phi")==0)
  {
      drawSubTabCorr(0,His);
  }
  else if(strcmp(TabWidgetShower->currentPage()->name(),"Shower_corr_theta")==0)
  {
      drawSubTabCorr(0,His);
  }
  else if( strcmp(TabWidget->currentPage()->name(),"MatchU_sub")==0)
  {
    if( strcmp(TabWidgetMatchU->currentPage()->name(),"MatchU_sect")==0 )
    {
      if(timeAxis == kFALSE) { drawSubTab( 3, His );     }
      else                   { drawSubTabTime( 3, His ); }
    }
    else if(strcmp(TabWidgetMatchU->currentPage()->name(),"MU_corr_phi")==0 )
    {
      drawSubTabCorr(0,His);
    }
    else if(strcmp(TabWidgetMatchU->currentPage()->name(),"MU_corr_theta")==0 )
    {
      drawSubTabCorr(0,His);
    }
  }
  else if(strcmp(TabWidget->currentPage()->name(),"time")==0)
    {
      drawTimeTab( His );
    }
}

//--------------------------------------------------------------------------------------------------------
void TGo4TriggerOnline::checkNaming( HMonHistAddon *His )
{
  if     ( (strcmp(His->GetName(),"hTriggerIPUtrendTof"       )==0) && (tofArraySetting       ==kFALSE) ) { histNaming( His ); tofArraySetting       =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerIPUtrendRich"      )==0) && (richArraySetting      ==kFALSE) ) { histNaming( His ); richArraySetting      =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerIPUtrendShower"    )==0) && (showerArraySetting    ==kFALSE) ) { histNaming( His ); showerArraySetting    =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerIPUtrendMatchU"    )==0) && (matchuArraySetting    ==kFALSE) ) { histNaming( His ); matchuArraySetting    =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerIPUtrendTofTime"   )==0) && (tofTimeArraySetting   ==kFALSE) ) { histNaming( His ); tofTimeArraySetting   =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerIPUtrendRichTime"  )==0) && (richTimeArraySetting  ==kFALSE) ) { histNaming( His ); richTimeArraySetting  =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerIPUtrendShowerTime")==0) && (showerTimeArraySetting==kFALSE) ) { histNaming( His ); showerTimeArraySetting=kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerIPUtrendMatchUTime")==0) && (matchuTimeArraySetting==kFALSE) ) { histNaming( His ); matchuTimeArraySetting=kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerMultDist"          )==0) && (multArraySetting      ==kFALSE) ) { histNaming( His ); multArraySetting      =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerRichRingMultDist"  )==0) && (richRingArraySetting  ==kFALSE) ) { histNaming( His ); richRingArraySetting  =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerTofCorrPhi")==0) && (tofCorrArraySettingsPhi == kFALSE)){histNaming(His); tofCorrArraySettingsPhi = kTRUE;}
 else if( (strcmp(His->GetName(),"hTriggerTofCorrTheta")==0) && (tofCorrArraySettingsTheta == kFALSE)){histNaming(His); tofCorrArraySettingsTheta = kTRUE;}
  else if( (strcmp(His->GetName(),"hTriggerTofHitMultDist"    )==0) && (tofHitArraySetting    ==kFALSE) ) { histNaming( His ); tofHitArraySetting    =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerShowerHitMultDist" )==0) && (showerHitArraySetting ==kFALSE) ) { histNaming( His ); showerHitArraySetting =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerShowerCorrPhi")==0) && (showerCorrArraySettingsPhi == kFALSE)) {histNaming(His); showerCorrArraySettingsPhi = kTRUE;}
  else if( (strcmp(His->GetName(),"hTriggerShowerCorrTheta")==0) && (showerCorrArraySettingsTheta == kFALSE) ) {histNaming(His); showerCorrArraySettingsTheta = kTRUE;} 
  else if( (strcmp(His->GetName(),"hTriggerRichCorrPhi")==0) && (richCorrArraySettingsPhi == kFALSE)) {histNaming(His); richCorrArraySettingsPhi = kTRUE;}
  else if( (strcmp(His->GetName(),"hTriggerRichCorrTheta")==0) && (richCorrArraySettingsTheta == kFALSE) ) {histNaming(His); richCorrArraySettingsTheta = kTRUE;}
  else if( (strcmp(His->GetName(),"hTriggerMUCorrPhi")==0) && (muCorrArraySettingsPhi == kFALSE)) {histNaming(His); muCorrArraySettingsPhi = kTRUE;}
  else if( (strcmp(His->GetName(),"hTriggerMUCorrTheta")==0) && (muCorrArraySettingsTheta == kFALSE) ) {histNaming(His); muCorrArraySettingsTheta = kTRUE;}
  else if( (strcmp(His->GetName(),"hTriggerMatchUHitMultDist" )==0) && (matchuHitArraySetting ==kFALSE) ) { histNaming( His ); matchuHitArraySetting =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerTrendTime"         )==0) && (timeTrendSetting      ==kFALSE) ) { histNaming( His ); timeTrendSetting      =kTRUE; }
  else if( (strcmp(His->GetName(),"hTriggerTofNumbers"        )==0) || (strcmp(His->GetName(),"hTriggerRichNumbers" )==0) ) { return; }
  else if( (strcmp(His->GetName(),"hTriggerIPUButterfly"        )==0)&& (butterfly== kFALSE)) {histNaming(His); butterfly = kTRUE;}
  else if( (strcmp(His->GetName(),"hTriggerMULepMom"        )==0)&& (mulepmom== kFALSE)) {histNaming(His); mulepmom = kTRUE;}
  else { noMoreNaming=kTRUE; }
}
//--------------------------------------------------------------------------------------------------------
// used to modify settings which have to be modified only once (color settings, title modifications, ...)
void TGo4TriggerOnline::histColor( HMonHistAddon *his )
{
  if(his == 0) { return; }
  if(strcmp(his->GetName(),"hTriggerIPUtrendTof"       )==0 || strcmp(his->GetName(),"hTriggerIPUtrendRich"      )==0 ||
     strcmp(his->GetName(),"hTriggerIPUtrendShower"    )==0 || strcmp(his->GetName(),"hTriggerIPUtrendMatchU"    )==0 ||
     strcmp(his->GetName(),"hTriggerIPUtrendTofTime"   )==0 || strcmp(his->GetName(),"hTriggerIPUtrendRichTime"  )==0 ||
     strcmp(his->GetName(),"hTriggerIPUtrendShowerTime")==0 || strcmp(his->GetName(),"hTriggerIPUtrendMatchUTime")==0   )
    {
      for(Int_t i=0;i<8;i++) // "8" to include also the dummie histogram which contains the global format settings.
 {
   his ->getP(0,i)->GetXaxis()->SetAxisColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetYaxis()->SetAxisColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetXaxis()->SetLabelColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetYaxis()->SetLabelColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetXaxis()->SetTitleColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetYaxis()->SetTitleColor((Color_t)newAxisColor);
 }
    }
  else if(strcmp(his->GetName(),"hTriggerMultDist")==0 )
    {
      for(Int_t i=0;i<4;i++)
 {
   his ->getP(0,i)->GetXaxis()->SetAxisColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetYaxis()->SetAxisColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetXaxis()->SetLabelColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetYaxis()->SetLabelColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetXaxis()->SetTitleColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetYaxis()->SetTitleColor((Color_t)newAxisColor);
 }
    }
  else if( (strcmp(his->GetName(),"hTriggerRichRingMultDist" )==0)  || (strcmp(his->GetName(),"hTriggerTofHitMultDist"   )==0 ) ||
           (strcmp(his->GetName(),"hTriggerShowerHitMultDist")==0 ) || (strcmp(his->GetName(),"hTriggerMatchUHitMultDist")==0 ) ||
    (strcmp(his->GetName(),"hTriggerTofCorrPhi")==0)  || (strcmp(his->GetName(),"hTriggerShowerCorrPhi") == 0) ||
     (strcmp(his->GetName(),"hTriggerTofCorrTheta")==0)  ||  (strcmp(his->GetName(),"hTriggerShowerCorrTheta")==0) ||
    (strcmp(his->GetName(),"hTriggerRichCorrPhi")==0)  || (strcmp(his->GetName(),"hTriggerMUCorrPhi") == 0) ||
     (strcmp(his->GetName(),"hTriggerRichCorrTheta")==0)  ||  (strcmp(his->GetName(),"hTriggerMUCorrTheta")==0) )
    {
      for(Int_t i=0;i<6;i++)
 {
   his ->getP(0,i)->GetXaxis()->SetAxisColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetYaxis()->SetAxisColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetXaxis()->SetLabelColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetYaxis()->SetLabelColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetXaxis()->SetTitleColor((Color_t)newAxisColor);
   his ->getP(0,i)->GetYaxis()->SetTitleColor((Color_t)newAxisColor);
 }
    }

  else if( strcmp(his->GetName(),"hTriggerTrendTime")==0 )
    {
      his ->getP()->GetXaxis()->SetAxisColor((Color_t)newAxisColor);
      his ->getP()->GetYaxis()->SetAxisColor((Color_t)newAxisColor);
      his ->getP()->GetXaxis()->SetLabelColor((Color_t)newAxisColor);
      his ->getP()->GetYaxis()->SetLabelColor((Color_t)newAxisColor);
      his ->getP()->GetXaxis()->SetTitleColor((Color_t)newAxisColor);
      his ->getP()->GetYaxis()->SetTitleColor((Color_t)newAxisColor);      
    }
}
//--------------------------------------------------------------------------------------------------------
// used to modify settings which have to be modified only once (color settings, title modifications, ...)
void TGo4TriggerOnline::histNaming( HMonHistAddon *his )
{
  if(his == 0) { return; }
  //cout << "his nameing: " << his->GetName() << endl;
  if(strcmp(his->GetName(),"hTriggerIPUtrendTof"       )==0 || strcmp(his->GetName(),"hTriggerIPUtrendRich"      )==0 ||
     strcmp(his->GetName(),"hTriggerIPUtrendShower"    )==0 || strcmp(his->GetName(),"hTriggerIPUtrendMatchU"    )==0 ||

     strcmp(his->GetName(),"hTriggerIPUtrendTofTime"   )==0 || strcmp(his->GetName(),"hTriggerIPUtrendRichTime"  )==0 ||
     strcmp(his->GetName(),"hTriggerIPUtrendShowerTime")==0 || strcmp(his->GetName(),"hTriggerIPUtrendMatchUTime")==0   )
    {
      TString secnum[6] = { "0","1","2","3","4","5" };
      his->getP(0,6)->SetTitle( (TString)his->getP(0,6)->GetTitle()+" global" );                //--- append " global" to the title of the global trendhist of each ipu
      for(Int_t i=0;i<6;i++) // "6" to exclude the global histogram.
 {
   his->getP(0,i)->SetTitle( (TString)his->getP(0,i)->GetTitle()+" sector_"+secnum[i] ); //--- append sector number to non global trend hists
 }
    }
  else if(strcmp(his->GetName(),"hTriggerMultDist")==0 )
    {
      TString names[4] = { "_Rich","_Tof","_Shower","_Matching Unit" };
      for(Int_t i=0;i<4;i++)
 {
   his->getP(0,i)->SetTitle( (TString)his->getP(0,i)->GetTitle()+names[i] );   
 }
    }
  else if( (strcmp(his->GetName(),"hTriggerRichRingMultDist" )==0)  ||  (strcmp(his->GetName(),"hTriggerTofHitMultDist"   )==0 ) ||
           (strcmp(his->GetName(),"hTriggerShowerHitMultDist")==0 ) || (strcmp(his->GetName(),"hTriggerMatchUHitMultDist")==0 ) ||
           (strcmp(his->GetName(),"hTriggerTofCorrPhi")==0)  || (strcmp(his->GetName(),"hTriggerShowerCorrPhi")==0) ||
           (strcmp(his->GetName(),"hTriggerMUCorrPhi")==0)  || (strcmp(his->GetName(),"hTriggerRichCorrPhi")==0) ||
           (strcmp(his->GetName(),"hTriggerRichCorrTheta")==0)  || (strcmp(his->GetName(),"hTriggerShowerCorrTheta")==0) ||
            (strcmp(his->GetName(),"hTriggerTofCorrTheta")==0) || (strcmp(his->GetName(),"hTriggerMUCorrTheta")==0))
    {
      TString secnum[6] = { "0","1","2","3","4","5" };
      for(Int_t i=0;i<6;i++)
      {
	  his->getP(0,i)->SetTitle( (TString)his->getP(0,i)->GetTitle()+"_sector_"+secnum[i] );   
      }
  }    
}

//--------------------------------------------------------------------------------------------------------  
// used to take care of settings which have to be redone every time the array drops in. (y-axis scaling, ...)
void TGo4TriggerOnline::arrayScaling( HMonHistAddon *his )
{
  if(his ==0 ) { return; }
  if(strcmp(his->GetName(),"hTriggerIPUtrendTof"       )==0 || strcmp(his->GetName(),"hTriggerIPUtrendRich"      )==0 ||
     strcmp(his->GetName(),"hTriggerIPUtrendShower"    )==0 || strcmp(his->GetName(),"hTriggerIPUtrendMatchU"    )==0 ||
     strcmp(his->GetName(),"hTriggerIPUtrendTofTime"   )==0 || strcmp(his->GetName(),"hTriggerIPUtrendRichTime"  )==0 ||
     strcmp(his->GetName(),"hTriggerIPUtrendShowerTime")==0 || strcmp(his->GetName(),"hTriggerIPUtrendMatchUTime")==0   )
    {
      Double_t maxtemp=0, MAX=0;
      for(Int_t i=0;i<6;i++)// "6" to exclude the global histogram.
 {
   maxtemp = his->getP(0,i)->GetBinContent( his->getP(0,i)->GetMaximumBin() );
   if(MAX < maxtemp) { MAX = maxtemp; }     //--- determine y maximum of sectorwise trendhists.
 }
      for(Int_t i=0;i<6;i++)// "6" to exclude the global histogram.
 {
   his->getP(0,i)->SetMaximum(MAX*1.1);     //--- scale all y axis to y maximum+10%.
 }
    }
}

//--------------------------------------------------------------------------------------------------------
void TGo4TriggerOnline::downscaled( int index )
{
  TGo4TriggerParameter* triggerParameter = new TGo4TriggerParameter("TriggerParam","TriggerParam");
  triggerParameter->SetUseDownscaled( index );     
  
  TGo4ComSetParameter* com = new TGo4ComSetParameter("TriggerParam");
  com->SetParameter(triggerParameter);
  fxTGo4GUIRegistry->SubmitCommand(com);
}
//--------------------------------------------------------------------------------------------------------
void TGo4TriggerOnline::setTimeAxis( )
{
    timeAxis  =  checkboxTimeAxis->isChecked();
}

//--------------------------------------------------------------------------------------------------------
void TGo4TriggerOnline::Reset()
{
  TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
  reset->SetTriggerChanged(kTRUE);
  reset->Set("Trigger",0);     
  TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
  com->SetParameter(reset);
  fxTGo4GUIRegistry->SubmitCommand(com);
}

//--------------------------------------------------------------------------------------------------------
void TGo4TriggerOnline::drawOverviewTab( HMonHistAddon *His )
{
  Int_t ipu = -1;
  if     ( strcmp(His->GetName(),"hTriggerIPUtrendRich"  )==0 && (freezeRich  ->isChecked()==0) ) { ipu=0;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendTof"   )==0 && (freezeTof   ->isChecked()==0) ) { ipu=1;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendShower")==0 && (freezeShower->isChecked()==0) ) { ipu=2;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendMatchU")==0 && (freezeMatchU->isChecked()==0) ) { ipu=3;  }
  else                                                                                            { return; }

  if( (ipu==0) || (ipu==1) || (ipu==2) || (ipu==3) )
    {
      CanvasArrayMainIpu[ipu]->getCanvas()->cd();
      His->setCanvas(CanvasArrayMainIpu[ipu]->getCanvas());
      His->draw(0);
      CanvasArrayMainIpu[ipu]->getCanvas()->Update();
      for(Int_t sec=0;sec<7;sec++)
 {
   TH1 *p = His->getP(0,sec);
   LCDArrayMainIpu[ipu][sec]->display( p->GetBinContent( (p->GetNbinsX()) ) );
 }
    }
}

//--------------------------------------------------------------------------------------------------------
void TGo4TriggerOnline::drawOverviewTabTime( HMonHistAddon *His )
{
  Int_t ipu = -1;
  if     ( strcmp(His->GetName(),"hTriggerIPUtrendRichTime"  )==0 && (freezeRich  ->isChecked()==0) ) { ipu=0;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendTofTime"   )==0 && (freezeTof   ->isChecked()==0) ) { ipu=1;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendShowerTime")==0 && (freezeShower->isChecked()==0) ) { ipu=2;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendMatchUTime")==0 && (freezeMatchU->isChecked()==0) ) { ipu=3;  }
  else                                                                                                { return; }

  if( (ipu==0) || (ipu==1) || (ipu==2) || (ipu==3) )
    {
      CanvasArrayMainIpu[ipu]->getCanvas()->cd();
      His->setCanvas(CanvasArrayMainIpu[ipu]->getCanvas());
      His->draw(0);
      CanvasArrayMainIpu[ipu]->getCanvas()->Update();
      for(Int_t sec=0;sec<7;sec++)
 {
   TH1 *p = His->getP(0,sec);
   LCDArrayMainIpu[ipu][sec]->display( p->GetBinContent( (p->GetNbinsX()) ) );
 }
    }
}

//--------------------------------------------------------------------------------------------------------
void TGo4TriggerOnline::drawOverviewTabNumbers( HMonHistAddon *His )
{
  if(strcmp(His->GetName(),"hTriggerRichNumbers")==0)
    {
      TH1 *p = His ->getP();
      for(Int_t i=0;i<6;i++)
 {
   LCDArrayRichNumbers[i]->display( p->GetBinContent(i+1) );
 }
    }
  else if(strcmp(His->GetName(),"hTriggerTofNumbers")==0)
    {
      TH1 *p = His ->getP();
      for(Int_t i=0;i<7;i++)
 {
   LCDArrayTofNumbers[i]->display( p->GetBinContent(i+1) );
 }
    }
}

//--------------------------------------------------------------------------------------------------------
void TGo4TriggerOnline::drawMultDistTab( HMonHistAddon *His )
{
  if(strcmp(TabWidgetMultDistr->currentPage()->name(),"multall")==0)
    {
      if(strcmp(His->GetName(),"hTriggerMultDist")==0)
 {
   for(Int_t ipu=0;ipu<4;ipu++)
     {
       CanvasArrayMultDist[ipu]->getCanvas()->cd();
       His->setCanvas(CanvasArrayMultDist[ipu]->getCanvas());
       His->getP(0,ipu)->Draw();
       CanvasArrayMultDist[ipu]->getCanvas()->Update();
     }
 }
    }
  else 
    {
      Int_t ipu = -1;
      if     ( (strcmp(TabWidgetMultDistr->currentPage()->name(),"multrich"  )==0) && (strcmp(His->GetName(),"hTriggerRichRingMultDist" )==0) ) { ipu=0;  }
      else if( (strcmp(TabWidgetMultDistr->currentPage()->name(),"multtof"   )==0) && (strcmp(His->GetName(),"hTriggerTofHitMultDist"   )==0) ) { ipu=1;  }
      else if( (strcmp(TabWidgetMultDistr->currentPage()->name(),"multshower")==0) && (strcmp(His->GetName(),"hTriggerShowerHitMultDist")==0) ) { ipu=2;  }
      else if( (strcmp(TabWidgetMultDistr->currentPage()->name(),"multmatchu")==0) && (strcmp(His->GetName(),"hTriggerMatchUHitMultDist")==0) ) { ipu=3;  }
      else                                                                                                                                      { return; }

      if( (ipu==0) || (ipu==1) || (ipu==2) || (ipu==3) )
 {
   for(Int_t sec=0;sec<6;sec++)
     {
       CanvasArrayMultDistIpu[ipu][sec]->getCanvas()->cd();
       His->setCanvas(CanvasArrayMultDistIpu[ipu][sec]->getCanvas());
       His ->getP(0,sec)->Draw();
       CanvasArrayMultDistIpu[ipu][sec]->getCanvas()->Update();
     }
 }
    }
}

//--------------------------------------------------------------------------------------------------------
void TGo4TriggerOnline::drawTimeTab( HMonHistAddon *His )
{
  if(strcmp(His->GetName(),"hTriggerIPUButterfly")==0)
    {
      fxQtCanvasButterfly->getCanvas()->cd();
      His ->getP()->Draw() ;
      fxQtCanvasButterfly->getCanvas()->Update();
    }
	else if(strcmp(His->GetName(),"hTriggertof")==0)
    {
      fxQtCanvasTofTime->getCanvas()->cd();
      His ->getP()->Draw() ;
      fxQtCanvasTofTime->getCanvas()->Update();
    }
	else if(strcmp(His->GetName(),"hTriggerMULepMom")==0)
    {
      fxQtCanvasLepMom->getCanvas()->cd();
      His ->getP()->Draw() ;
      fxQtCanvasLepMom->getCanvas()->Update();
    }
}

//--------------------------------------------------------------------------------------------------------
void TGo4TriggerOnline::drawSubTab( int forceIpu, HMonHistAddon *His )
{
  Int_t ipu = -1;
  if     ( strcmp(His->GetName(),"hTriggerIPUtrendRich"   )==0 ) { ipu=0;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendTof"    )==0 ) { ipu=1;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendShower" )==0 ) { ipu=2;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendMatchU" )==0 ) { ipu=3;  }
  else                                                           { return; }

  if( ((ipu==1) || (ipu==0) || (ipu==2) || (ipu==3)) && (ipu == forceIpu) )
    {
      for(Int_t sec=0;sec<7;sec++)
 {
   CanvasArrayIpuSect[ipu][sec]->getCanvas()->cd();
   His ->getP(0,sec)->Draw();
   CanvasArrayIpuSect[ipu][sec]->getCanvas()->Update();
 }
    }
}

//--------------------------------------------------------------------------------------------------------
void TGo4TriggerOnline::drawSubTabTime(  int forceIpu, HMonHistAddon *His )
{
  Int_t ipu = -1;
  if     ( strcmp(His->GetName(),"hTriggerIPUtrendRichTime"   )==0 ) { ipu=0;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendTofTime"    )==0 ) { ipu=1;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendShowerTime" )==0 ) { ipu=2;  }
  else if( strcmp(His->GetName(),"hTriggerIPUtrendMatchUTime" )==0 ) { ipu=3;  }
  else                                                               { return; }

  if( ((ipu==1) || (ipu==0) || (ipu==2) || (ipu==3)) && (ipu == forceIpu) )
    {
      for(Int_t sec=0;sec<7;sec++)
 {
   CanvasArrayIpuSect[ipu][sec]->getCanvas()->cd();
   His ->getP(0,sec)->Draw();
   CanvasArrayIpuSect[ipu][sec]->getCanvas()->Update();
 }
    }
}

void TGo4TriggerOnline::initCanvasColor( short color )
{
  //------------------ Canvas color
  Color_t canvasColor = (Color_t)color;
  //-----------------

  // --- Mult Dist Tab --------------------------------------------------
  // --- Mult Dist Canvases
  CanvasArrayMultDist[0]=fxQtCanvasRichMultiplicityDistrib;
  CanvasArrayMultDist[1]=fxQtCanvasTOFMultiplicityDistrib;
  CanvasArrayMultDist[2]=fxQtCanvasShowerMultiplicityDistrib;
  CanvasArrayMultDist[3]=fxQtCanvasMatchUMultiplicityDistrib;
  for(Int_t i=0;i<4;i++)
    {
      CanvasArrayMultDist[i]->getCanvas()->SetFillColor(canvasColor); 
      CanvasArrayMultDist[i]->getCanvas()->SetTickx();
      CanvasArrayMultDist[i]->getCanvas()->SetTicky();  
    }
  // ---- Mult Dist -- Mult Rich Canvases
  CanvasArrayMultDistIpu[0][0]=TQRootCanvasMultRich0;
  CanvasArrayMultDistIpu[0][1]=TQRootCanvasMultRich1;     
  CanvasArrayMultDistIpu[0][2]=TQRootCanvasMultRich2;     
  CanvasArrayMultDistIpu[0][3]=TQRootCanvasMultRich3;
  CanvasArrayMultDistIpu[0][4]=TQRootCanvasMultRich4;   
  CanvasArrayMultDistIpu[0][5]=TQRootCanvasMultRich5;   
  // ---- Mult Dist -- Mult Tof Canvases
  CanvasArrayMultDistIpu[1][0]=TQRootCanvasMultTof0;
  CanvasArrayMultDistIpu[1][1]=TQRootCanvasMultTof1;
  CanvasArrayMultDistIpu[1][2]=TQRootCanvasMultTof2;
  CanvasArrayMultDistIpu[1][3]=TQRootCanvasMultTof3;
  CanvasArrayMultDistIpu[1][4]=TQRootCanvasMultTof4;
  CanvasArrayMultDistIpu[1][5]=TQRootCanvasMultTof5;
  // ---- Mult Dist -- Mult Shower Canvases
  CanvasArrayMultDistIpu[2][0]=TQRootCanvasMultShower0;
  CanvasArrayMultDistIpu[2][1]=TQRootCanvasMultShower1;     
  CanvasArrayMultDistIpu[2][2]=TQRootCanvasMultShower2;     
  CanvasArrayMultDistIpu[2][3]=TQRootCanvasMultShower3;
  CanvasArrayMultDistIpu[2][4]=TQRootCanvasMultShower4;   
  CanvasArrayMultDistIpu[2][5]=TQRootCanvasMultShower5;   
  // ---- Mult Dist -- Mult MatchU Canvases
  CanvasArrayMultDistIpu[3][0]=TQRootCanvasMultMatchU0;
  CanvasArrayMultDistIpu[3][1]=TQRootCanvasMultMatchU1;     
  CanvasArrayMultDistIpu[3][2]=TQRootCanvasMultMatchU2;     
  CanvasArrayMultDistIpu[3][3]=TQRootCanvasMultMatchU3;
  CanvasArrayMultDistIpu[3][4]=TQRootCanvasMultMatchU4;   
  CanvasArrayMultDistIpu[3][5]=TQRootCanvasMultMatchU5;   
  for(Int_t ipu=0;ipu<4;ipu++)
    {
      for(Int_t sec=0;sec<6;sec++)
 {
   CanvasArrayMultDistIpu[ipu][sec]->getCanvas()->SetFillColor(canvasColor); 
   CanvasArrayMultDistIpu[ipu][sec]->getCanvas()->SetTickx();
   CanvasArrayMultDistIpu[ipu][sec]->getCanvas()->SetTicky();  
 }
    }
  // --- "Sub" Tabs -----------------------------------------------------
  // ---- Sub Tab Rich Canvases  
  CanvasArrayIpuSect[0][0] = TQRootCanvasRichSec0;
  CanvasArrayIpuSect[0][1] = TQRootCanvasRichSec1;
  CanvasArrayIpuSect[0][2] = TQRootCanvasRichSec2;
  CanvasArrayIpuSect[0][3] = TQRootCanvasRichSec3;
  CanvasArrayIpuSect[0][4] = TQRootCanvasRichSec4;
  CanvasArrayIpuSect[0][5] = TQRootCanvasRichSec5;
  CanvasArrayIpuSect[0][6] = TQRootCanvasRichSec6;
  // ---- Sub Tab Tof Canvases
  CanvasArrayIpuSect[1][0] = TQRootCanvasTOFSec0;
  CanvasArrayIpuSect[1][1] = TQRootCanvasTOFSec1;
  CanvasArrayIpuSect[1][2] = TQRootCanvasTOFSec2;
  CanvasArrayIpuSect[1][3] = TQRootCanvasTOFSec3;
  CanvasArrayIpuSect[1][4] = TQRootCanvasTOFSec4;
  CanvasArrayIpuSect[1][5] = TQRootCanvasTOFSec5;
  CanvasArrayIpuSect[1][6] = TQRootCanvasTOFSec6;
  // ---- Sub Tab Shower Canvases
  CanvasArrayIpuSect[2][0] = TQRootCanvasShowerSec0;
  CanvasArrayIpuSect[2][1] = TQRootCanvasShowerSec1;
  CanvasArrayIpuSect[2][2] = TQRootCanvasShowerSec2;
  CanvasArrayIpuSect[2][3] = TQRootCanvasShowerSec3;
  CanvasArrayIpuSect[2][4] = TQRootCanvasShowerSec4;
  CanvasArrayIpuSect[2][5] = TQRootCanvasShowerSec5;
  CanvasArrayIpuSect[2][6] = TQRootCanvasShowerSec6;
  // ---- Sub Tab MatchU Canvases 
  CanvasArrayIpuSect[3][0] = TQRootCanvasMatchUSec0;
  CanvasArrayIpuSect[3][1] = TQRootCanvasMatchUSec1;
  CanvasArrayIpuSect[3][2] = TQRootCanvasMatchUSec2;
  CanvasArrayIpuSect[3][3] = TQRootCanvasMatchUSec3;
  CanvasArrayIpuSect[3][4] = TQRootCanvasMatchUSec4;
  CanvasArrayIpuSect[3][5] = TQRootCanvasMatchUSec5;
  CanvasArrayIpuSect[3][6] = TQRootCanvasMatchUSec6;
  for(Int_t ipu=0;ipu<4;ipu++)
    {
      for(Int_t sec=0;sec<7;sec++)
 {
   CanvasArrayIpuSect[ipu][sec]->getCanvas()->SetFillColor(canvasColor); 
   CanvasArrayIpuSect[ipu][sec]->getCanvas()->SetTickx();
   CanvasArrayIpuSect[ipu][sec]->getCanvas()->SetTicky();  
 }
    } 
  // --- Sub tab correlation ----------------------------------------------
  
  // Rich
  CanvasArrayCorrPhi[0][0] = TQRootCanvasRichCorrSec0phi;
  CanvasArrayCorrPhi[0][1] = TQRootCanvasRichCorrSec1phi;
  CanvasArrayCorrPhi[0][2] = TQRootCanvasRichCorrSec2phi;
  CanvasArrayCorrPhi[0][3] = TQRootCanvasRichCorrSec3phi;
  CanvasArrayCorrPhi[0][4] = TQRootCanvasRichCorrSec4phi;
  CanvasArrayCorrPhi[0][5] = TQRootCanvasRichCorrSec5phi;
  
  // -- TOF
  CanvasArrayCorrPhi[1][0] = TQRootCanvasTOFCorrSec0phi;
  CanvasArrayCorrPhi[1][1] = TQRootCanvasTOFCorrSec1phi;
  CanvasArrayCorrPhi[1][2] = TQRootCanvasTOFCorrSec2phi;
  CanvasArrayCorrPhi[1][3] = TQRootCanvasTOFCorrSec3phi;
  CanvasArrayCorrPhi[1][4] = TQRootCanvasTOFCorrSec4phi;
  CanvasArrayCorrPhi[1][5] = TQRootCanvasTOFCorrSec5phi;
 
   // --- SHOWER
  CanvasArrayCorrPhi[2][0] = TQRootCanvasSchowerCorrSec0phi;
  CanvasArrayCorrPhi[2][1] = TQRootCanvasSchowerCorrSec1phi;
  CanvasArrayCorrPhi[2][2] = TQRootCanvasSchowerCorrSec2phi;
  CanvasArrayCorrPhi[2][3] = TQRootCanvasSchowerCorrSec3phi;
  CanvasArrayCorrPhi[2][4] = TQRootCanvasSchowerCorrSec4phi;
  CanvasArrayCorrPhi[2][5] = TQRootCanvasSchowerCorrSec5phi;
 
  // MU
  CanvasArrayCorrPhi[3][0] = TQRootCanvasMUCorrSec0phi;
  CanvasArrayCorrPhi[3][1] = TQRootCanvasMUCorrSec1phi;
  CanvasArrayCorrPhi[3][2] = TQRootCanvasMUCorrSec2phi;
  CanvasArrayCorrPhi[3][3] = TQRootCanvasMUCorrSec3phi;
  CanvasArrayCorrPhi[3][4] = TQRootCanvasMUCorrSec4phi;
  CanvasArrayCorrPhi[3][5] = TQRootCanvasMUCorrSec5phi;
 
  // Theta 
  CanvasArrayCorrTheta[0][0] = TQRootCanvasRichCorrSec0theta;
  CanvasArrayCorrTheta[0][1] = TQRootCanvasRichCorrSec1theta;
  CanvasArrayCorrTheta[0][2] = TQRootCanvasRichCorrSec2theta;
  CanvasArrayCorrTheta[0][3] = TQRootCanvasRichCorrSec3theta;
  CanvasArrayCorrTheta[0][4] = TQRootCanvasRichCorrSec4theta;
  CanvasArrayCorrTheta[0][5] = TQRootCanvasRichCorrSec5theta;
  
  CanvasArrayCorrTheta[1][0] = TQRootCanvasTOFCorrSec0theta;
  CanvasArrayCorrTheta[1][1] = TQRootCanvasTOFCorrSec1theta;
  CanvasArrayCorrTheta[1][2] = TQRootCanvasTOFCorrSec2theta;
  CanvasArrayCorrTheta[1][3] = TQRootCanvasTOFCorrSec3theta;
  CanvasArrayCorrTheta[1][4] = TQRootCanvasTOFCorrSec4theta;
  CanvasArrayCorrTheta[1][5] = TQRootCanvasTOFCorrSec5theta;
  
  CanvasArrayCorrTheta[2][0] = TQRootCanvasSchowerCorrSec0theta;
  CanvasArrayCorrTheta[2][1] = TQRootCanvasSchowerCorrSec1theta;
  CanvasArrayCorrTheta[2][2] = TQRootCanvasSchowerCorrSec2theta;
  CanvasArrayCorrTheta[2][3] = TQRootCanvasSchowerCorrSec3theta;
  CanvasArrayCorrTheta[2][4] = TQRootCanvasSchowerCorrSec4theta;
  CanvasArrayCorrTheta[2][5] = TQRootCanvasSchowerCorrSec5theta;
    
  CanvasArrayCorrTheta[3][0] = TQRootCanvasMUCorrSec0theta;
  CanvasArrayCorrTheta[3][1] = TQRootCanvasMUCorrSec1theta;
  CanvasArrayCorrTheta[3][2] = TQRootCanvasMUCorrSec2theta;
  CanvasArrayCorrTheta[3][3] = TQRootCanvasMUCorrSec3theta;
  CanvasArrayCorrTheta[3][4] = TQRootCanvasMUCorrSec4theta;
  CanvasArrayCorrTheta[3][5] = TQRootCanvasMUCorrSec5theta;
	 // --- global settings
   for(Int_t ipu=0;ipu<4;ipu++)
    {
      for(Int_t sec=0;sec<6;sec++)
 {
   CanvasArrayCorrPhi[ipu][sec]->getCanvas()->SetFillColor(canvasColor); 
   CanvasArrayCorrPhi[ipu][sec]->getCanvas()->SetTickx();
   CanvasArrayCorrPhi[ipu][sec]->getCanvas()->SetTicky(); 
   CanvasArrayCorrTheta[ipu][sec]->getCanvas()->SetFillColor(canvasColor); 
   CanvasArrayCorrTheta[ipu][sec]->getCanvas()->SetTickx();
   CanvasArrayCorrTheta[ipu][sec]->getCanvas()->SetTicky(); 
           }
   }
  // --- Overview (main) Tab --------------------------------------------
  // --- Main Canvases
  CanvasArrayMainIpu[0]=fxQtCanvasRichmain;
  CanvasArrayMainIpu[1]=fxQtCanvasTOFmain;
  CanvasArrayMainIpu[2]=fxQtCanvasShowermain;
  CanvasArrayMainIpu[3]=fxQtCanvasMatchUmain;
  for(Int_t ipu=0;ipu<4;ipu++)
    {
      CanvasArrayMainIpu[ipu]->getCanvas()->SetFillColor(canvasColor);
      CanvasArrayMainIpu[ipu]->getCanvas()->SetTickx();
      CanvasArrayMainIpu[ipu]->getCanvas()->SetTicky();
    }   
  // --- Time Tab -------------------------------------------------------
//  fxQtCanvasButterfly->getCanvas()->SetFillColor(canvasColor);
  fxQtCanvasButterfly->getCanvas()->SetTickx();
  fxQtCanvasButterfly->getCanvas()->SetTicky();
}

void TGo4TriggerOnline::reinitCanvasColor( const QString &color )
{
    Color_t canvasColor=1;
    if(color == (QString)"black") { canvasColor=1; newAxisColor=0; }
    if(color == (QString)"white") { canvasColor=0; newAxisColor=1; }

  // --- Mult Dist Canvases
  for(Int_t i=0;i<4;i++)
    {
      CanvasArrayMultDist[i]->getCanvas()->SetFillColor(canvasColor); 
    }
  // --- Mult Dist -- Mult "ipu" Canvases
  for(Int_t ipu=0;ipu<4;ipu++)
    {
      for(Int_t sec=0;sec<6;sec++)
 {
   CanvasArrayMultDistIpu[ipu][sec]->getCanvas()->SetFillColor(canvasColor); 
 }
    }
  // --- "Sub" Tabs Canvases
  for(Int_t ipu=0;ipu<4;ipu++)
    {
      for(Int_t sec=0;sec<7;sec++)
 {
   CanvasArrayIpuSect[ipu][sec]->getCanvas()->SetFillColor(canvasColor); 
 }
    }
  // -- Cor tabs
    for(Int_t ipu=0;ipu<4;ipu++)
    {
      for(Int_t sec=0;sec<6;sec++)
      {
	  CanvasArrayCorrPhi[ipu][sec]->getCanvas()->SetFillColor(canvasColor);
	  CanvasArrayCorrTheta[ipu][sec]->getCanvas()->SetFillColor(canvasColor);
      }
    }
  // --- Main Canvases
  for(Int_t ipu=0;ipu<4;ipu++)
    {
      CanvasArrayMainIpu[ipu]->getCanvas()->SetFillColor(canvasColor);
  }
  // --- Time Tab Canvases
//  fxQtCanvasButterfly->getCanvas()->SetFillColor(canvasColor);
}

void TGo4TriggerOnline::initLCDNumbers()
{
  // --- Overview (main) Tab --------------------------------------------
  // --- LCD Rich
  LCDArrayMainIpu[0][0]=LCDRichHistS0;
  LCDArrayMainIpu[0][1]=LCDRichHistS1; 
  LCDArrayMainIpu[0][2]=LCDRichHistS2; 
  LCDArrayMainIpu[0][3]=LCDRichHistS3; 
  LCDArrayMainIpu[0][4]=LCDRichHistS4; 
  LCDArrayMainIpu[0][5]=LCDRichHistS5; 
  LCDArrayMainIpu[0][6]=LCDRichHistS6; 
  // --- LCD Tof
  LCDArrayMainIpu[1][0]=LCDTOFHistS0; 
  LCDArrayMainIpu[1][1]=LCDTOFHistS1;
  LCDArrayMainIpu[1][2]=LCDTOFHistS2;
  LCDArrayMainIpu[1][3]=LCDTOFHistS3;
  LCDArrayMainIpu[1][4]=LCDTOFHistS4;
  LCDArrayMainIpu[1][5]=LCDTOFHistS5;
  LCDArrayMainIpu[1][6]=LCDTOFHistS6;
  // --- LCD Shower
  LCDArrayMainIpu[2][0]=LCDShowerHistS0;
  LCDArrayMainIpu[2][1]=LCDShowerHistS1;
  LCDArrayMainIpu[2][2]=LCDShowerHistS2;
  LCDArrayMainIpu[2][3]=LCDShowerHistS3;
  LCDArrayMainIpu[2][4]=LCDShowerHistS4;
  LCDArrayMainIpu[2][5]=LCDShowerHistS5;
  LCDArrayMainIpu[2][6]=LCDShowerHistS6;
  // --- LCD MatchU
  LCDArrayMainIpu[3][0]=LCDMatchUHistS0; 
  LCDArrayMainIpu[3][1]=LCDMatchUHistS1; 
  LCDArrayMainIpu[3][2]=LCDMatchUHistS2; 
  LCDArrayMainIpu[3][3]=LCDMatchUHistS3; 
  LCDArrayMainIpu[3][4]=LCDMatchUHistS4; 
  LCDArrayMainIpu[3][5]=LCDMatchUHistS5;
  LCDArrayMainIpu[3][6]=LCDMatchUHistS6;
  // --- TOF Numbers
  LCDArrayTofNumbers[0] = LCDTOFMainSec0;
  LCDArrayTofNumbers[1] = LCDTOFMainSec1;
  LCDArrayTofNumbers[2] = LCDTOFMainSec2;
  LCDArrayTofNumbers[3] = LCDTOFMainSec3;
  LCDArrayTofNumbers[4] = LCDTOFMainSec4;
  LCDArrayTofNumbers[5] = LCDTOFMainSec5;
  LCDArrayTofNumbers[6] = LCDTOFMainSec6;
  // --- Rich Numbers
  LCDArrayRichNumbers[0] = LCDRichMain_1_100;
  LCDArrayRichNumbers[1] = LCDRichMain_1_1000;
  LCDArrayRichNumbers[2] = LCDRichMain_1_10k;
  LCDArrayRichNumbers[3] = LCDRichMain_2_100;
  LCDArrayRichNumbers[4] = LCDRichMain_2_1000;
  LCDArrayRichNumbers[5] = LCDRichMain_2_10k;
}

void TGo4TriggerOnline::drawSubTabCorr( int forceIpu, HMonHistAddon *His )
{
  Int_t ipu_p = -1, ipu_t = -1;
  
  if((strcmp(TabWidgetRich->currentPage()->name(),"Rich_corr_phi")==0) && (strcmp(His->GetName(),"hTriggerRichCorrPhi")==0))
  {ipu_p=0;}  
  else if((strcmp(TabWidgetTof->currentPage()->name(),"Tof_corr_phi")==0) && (strcmp(His->GetName(),"hTriggerTofCorrPhi" )==0 ))
  {ipu_p=1;}
  else if((strcmp(TabWidgetShower->currentPage()->name(),"Shower_corr_phi")==0) && (strcmp(His->GetName(),"hTriggerShowerCorrPhi")==0))
  {ipu_p=2;}
  else if((strcmp(TabWidgetMatchU->currentPage()->name(),"MU_corr_theta")==0) && (strcmp(His->GetName(),"hTriggerMUCorrTheta")==0))
  {ipu_p=3;}  
  else if((strcmp(TabWidgetRich->currentPage()->name(),"Rich_corr_theta")==0) && (strcmp(His->GetName(),"hTriggerRichCorrTheta")==0))
  {ipu_t=0;}  
  else if((strcmp(TabWidgetTof->currentPage()->name(),"Tof_corr_theta")==0) && (strcmp(His->GetName(),"hTriggerTofCorrTheta")==0))
  {ipu_t=1;}
  else if((strcmp(TabWidgetShower->currentPage()->name(),"Shower_corr_theta")==0) && (strcmp(His->GetName(),"hTriggerShowerCorrTheta")==0))
  {ipu_t=2;}  
  else if((strcmp(TabWidgetMatchU->currentPage()->name(),"MU_corr_theta")==0) && (strcmp(His->GetName(),"hTriggerMUCorrTheta")==0))
  {ipu_t=3;}  
  else return;

  if(ipu_p>=0 && ipu_p<4 )
  {
      for(Int_t sec=0;sec<6;sec++)
      {
	  CanvasArrayCorrPhi[ipu_p][sec]->getCanvas()->cd();
	  His ->getP(0,sec)->Draw();
	  CanvasArrayCorrPhi[ipu_p][sec]->getCanvas()->Update();
      }
  }
  
  if(ipu_t>=0 && ipu_t<4 )
  {
      for(Int_t sec=0;sec<6;sec++)
      {
	  CanvasArrayCorrTheta[ipu_t][sec]->getCanvas()->cd();
	  His ->getP(0,sec)->Draw();
	  CanvasArrayCorrTheta[ipu_t][sec]->getCanvas()->Update();
      }
  }
}
