/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/
void arrayNamingAndColor( TNamed *his );
void arrayScaling( TNamed *his );

void TGo4TriggerOnline::init() 
{
  fxTGo4GUIRegistry = TGo4GUIRegistry::Instance();
  fxTGo4TriggerOnlineSlots = (TGo4TriggerOnlineSlots *) fxTGo4GUIRegistry->GetSlotClass("TGo4TriggerOnlineSlots");
  
  if(fxTGo4TriggerOnlineSlots  !=0) {	fxTGo4TriggerOnlineSlots->SetGUI(this); }
  else                              {	fxTGo4TriggerOnlineSlots = new TGo4TriggerOnlineSlots ("TGo4TriggerOnlineSlots" , "Slot Class for Qt GUI", this); }
  
  fxTGo4TriggerOnlineStatus= dynamic_cast <TGo4TriggerOnlineStatus *> (fxTGo4TriggerOnlineSlots->GetStatus());
  
  // --- one time array setting flags -----------------------------------
  tofArraySetting=0;
  richArraySetting=0;
  showerArraySetting=0;
  matchuArraySetting=0;
  multArraySetting=0;
  richRingArraySetting=0;
  tofHitArraySetting=0;
  showerHitArraySetting=0;
  matchuHitArraySetting=0;
  // -------------------------------------------------------------------- 
 
  // --- TOF ------------------------------------------------------------
  // ---- Main
  fxQtCanvasTOFmain->getCanvas()->SetFillColor(1);
  fxQtCanvasTOFmain->getCanvas()->SetTickx();
  fxQtCanvasTOFmain->getCanvas()->SetTicky();
  // ---- Mult Dist
  fxQtCanvasTOFMultiplicityDistrib->getCanvas()->SetFillColor(1);
  fxQtCanvasTOFMultiplicityDistrib->getCanvas()->SetTickx();
  fxQtCanvasTOFMultiplicityDistrib->getCanvas()->SetTicky();
  // ---- Mult Dist -- Mult Tof
  CanvasArrayTofHitMult[0]=TQRootCanvasMultTof0;
  CanvasArrayTofHitMult[1]=TQRootCanvasMultTof1;	    
  CanvasArrayTofHitMult[2]=TQRootCanvasMultTof2;	    
  CanvasArrayTofHitMult[3]=TQRootCanvasMultTof3;
  CanvasArrayTofHitMult[4]=TQRootCanvasMultTof4;   
  CanvasArrayTofHitMult[5]=TQRootCanvasMultTof5;   
  for(Int_t i=0;i<6;i++)
    {
      CanvasArrayTofHitMult[i]->getCanvas()->SetFillColor(1);	
      CanvasArrayTofHitMult[i]->getCanvas()->SetTickx();
      CanvasArrayTofHitMult[i]->getCanvas()->SetTicky(); 	
    }
  // ---- Sub Tab
  CanvasArrayTofSect[0] = TQRootCanvasTOFSec0;
  CanvasArrayTofSect[1] = TQRootCanvasTOFSec1;
  CanvasArrayTofSect[2] = TQRootCanvasTOFSec2;
  CanvasArrayTofSect[3] = TQRootCanvasTOFSec3;
  CanvasArrayTofSect[4] = TQRootCanvasTOFSec4;
  CanvasArrayTofSect[5] = TQRootCanvasTOFSec5;
  CanvasArrayTofSect[6] = TQRootCanvasTOFSec6;
    
  for(Int_t i=0;i<7;i++)
    {
      CanvasArrayTofSect[i]->getCanvas()->SetFillColor(1);
      CanvasArrayTofSect[i]->getCanvas()->SetTickx();
      CanvasArrayTofSect[i]->getCanvas()->SetTicky();
    }

  LCDArrayTof[0] = LCDTOFHistS0;
  LCDArrayTof[1] = LCDTOFHistS1;
  LCDArrayTof[2] = LCDTOFHistS2;
  LCDArrayTof[3] = LCDTOFHistS3;
  LCDArrayTof[4] = LCDTOFHistS4;
  LCDArrayTof[5] = LCDTOFHistS5;
  LCDArrayTof[6] = LCDTOFHistS6;

  LCDArrayTofNumbers[0] = LCDTOFMainSec0;
  LCDArrayTofNumbers[1] = LCDTOFMainSec1;
  LCDArrayTofNumbers[2] = LCDTOFMainSec2;
  LCDArrayTofNumbers[3] = LCDTOFMainSec3;
  LCDArrayTofNumbers[4] = LCDTOFMainSec4;
  LCDArrayTofNumbers[5] = LCDTOFMainSec5;
  LCDArrayTofNumbers[6] = LCDTOFMainSec6;
  // --------------------------------------------------------------------

  // --- Rich -----------------------------------------------------------
  // ---- Main
  fxQtCanvasRichmain->getCanvas()->SetFillColor(1);
  fxQtCanvasRichmain->getCanvas()->SetTickx();
  fxQtCanvasRichmain->getCanvas()->SetTicky(); 
  // ---- Mult Dist
  fxQtCanvasRichMultiplicityDistrib->getCanvas()->SetFillColor(1);
  fxQtCanvasRichMultiplicityDistrib->getCanvas()->SetTickx();
  fxQtCanvasRichMultiplicityDistrib->getCanvas()->SetTicky(); 
  // ---- Mult Dist -- Mult Rich
  CanvasArrayRichRingMult[0]=TQRootCanvasMultRich0;
  CanvasArrayRichRingMult[1]=TQRootCanvasMultRich1;	    
  CanvasArrayRichRingMult[2]=TQRootCanvasMultRich2;	    
  CanvasArrayRichRingMult[3]=TQRootCanvasMultRich3;
  CanvasArrayRichRingMult[4]=TQRootCanvasMultRich4;   
  CanvasArrayRichRingMult[5]=TQRootCanvasMultRich5;   
  for(Int_t i=0;i<6;i++)
    {
      CanvasArrayRichRingMult[i]->getCanvas()->SetFillColor(1);	
      CanvasArrayRichRingMult[i]->getCanvas()->SetTickx();
      CanvasArrayRichRingMult[i]->getCanvas()->SetTicky(); 	
    }
  // ---- Sub Tab    
  CanvasArrayRichSect[0] = TQRootCanvasRichSec0;
  CanvasArrayRichSect[1] = TQRootCanvasRichSec1;
  CanvasArrayRichSect[2] = TQRootCanvasRichSec2;
  CanvasArrayRichSect[3] = TQRootCanvasRichSec3;
  CanvasArrayRichSect[4] = TQRootCanvasRichSec4;
  CanvasArrayRichSect[5] = TQRootCanvasRichSec5;
  CanvasArrayRichSect[6] = TQRootCanvasRichSec6;

  for(Int_t i=0;i<7;i++)
    {
      CanvasArrayRichSect[i]->getCanvas()->SetFillColor(1);
      CanvasArrayRichSect[i]->getCanvas()->SetTickx();
      CanvasArrayRichSect[i]->getCanvas()->SetTicky();
    }

  LCDArrayRich[0] = LCDRichHistS0;
  LCDArrayRich[1] = LCDRichHistS1;
  LCDArrayRich[2] = LCDRichHistS2;
  LCDArrayRich[3] = LCDRichHistS3;
  LCDArrayRich[4] = LCDRichHistS4;
  LCDArrayRich[5] = LCDRichHistS5;
  LCDArrayRich[6] = LCDRichHistS6;

  LCDArrayRichNumbers[0] = LCDRichMain_1_100;
  LCDArrayRichNumbers[1] = LCDRichMain_1_1000;
  LCDArrayRichNumbers[2] = LCDRichMain_1_10k;
  LCDArrayRichNumbers[3] = LCDRichMain_2_100;
  LCDArrayRichNumbers[4] = LCDRichMain_2_1000;
  LCDArrayRichNumbers[5] = LCDRichMain_2_10k;
  // --------------------------------------------------------------------

  // --- Shower ---------------------------------------------------------
  // ---- Main
  fxQtCanvasShowermain->getCanvas()->SetFillColor(1);
  fxQtCanvasShowermain->getCanvas()->SetTickx();
  fxQtCanvasShowermain->getCanvas()->SetTicky();
  // ---- Mult Dist
  fxQtCanvasShowerMultiplicityDistrib->getCanvas()->SetFillColor(1);
  fxQtCanvasShowerMultiplicityDistrib->getCanvas()->SetTickx();
  fxQtCanvasShowerMultiplicityDistrib->getCanvas()->SetTicky(); 
  // ---- Mult Dist -- Mult Shower
  CanvasArrayShowerHitMult[0]=TQRootCanvasMultShower0;
  CanvasArrayShowerHitMult[1]=TQRootCanvasMultShower1;	    
  CanvasArrayShowerHitMult[2]=TQRootCanvasMultShower2;	    
  CanvasArrayShowerHitMult[3]=TQRootCanvasMultShower3;
  CanvasArrayShowerHitMult[4]=TQRootCanvasMultShower4;   
  CanvasArrayShowerHitMult[5]=TQRootCanvasMultShower5;   
  for(Int_t i=0;i<6;i++)
    {
      CanvasArrayShowerHitMult[i]->getCanvas()->SetFillColor(1);	
      CanvasArrayShowerHitMult[i]->getCanvas()->SetTickx();
      CanvasArrayShowerHitMult[i]->getCanvas()->SetTicky(); 	
    }
  // ---- Sub Tab   
  CanvasArrayShowerSect[0] = TQRootCanvasShowerSec0;
  CanvasArrayShowerSect[1] = TQRootCanvasShowerSec1;
  CanvasArrayShowerSect[2] = TQRootCanvasShowerSec2;
  CanvasArrayShowerSect[3] = TQRootCanvasShowerSec3;
  CanvasArrayShowerSect[4] = TQRootCanvasShowerSec4;
  CanvasArrayShowerSect[5] = TQRootCanvasShowerSec5;
  CanvasArrayShowerSect[6] = TQRootCanvasShowerSec6;

  for(Int_t i=0;i<7;i++)
    {
      CanvasArrayShowerSect[i]->getCanvas()->SetFillColor(1);
      CanvasArrayShowerSect[i]->getCanvas()->SetTickx();
      CanvasArrayShowerSect[i]->getCanvas()->SetTicky();
    }

  LCDArrayShower[0] = LCDShowerHistS0;
  LCDArrayShower[1] = LCDShowerHistS1;
  LCDArrayShower[2] = LCDShowerHistS2;
  LCDArrayShower[3] = LCDShowerHistS3;
  LCDArrayShower[4] = LCDShowerHistS4;
  LCDArrayShower[5] = LCDShowerHistS5;
  LCDArrayShower[6] = LCDShowerHistS6;
  // --------------------------------------------------------------------
    
  // --- Matching Unit --------------------------------------------------
  // ---- Main
  fxQtCanvasMatchUmain->getCanvas()->SetFillColor(1);
  fxQtCanvasMatchUmain->getCanvas()->SetTickx();
  fxQtCanvasMatchUmain->getCanvas()->SetTicky();
  // ---- Mult Dist
  fxQtCanvasMatchUMultiplicityDistrib->getCanvas()->SetFillColor(1);
  fxQtCanvasMatchUMultiplicityDistrib->getCanvas()->SetTickx();
  fxQtCanvasMatchUMultiplicityDistrib->getCanvas()->SetTicky(); 
  // ---- Mult Dist -- Mult MatchU
  CanvasArrayMatchUHitMult[0]=TQRootCanvasMultMatchU0;
  CanvasArrayMatchUHitMult[1]=TQRootCanvasMultMatchU1;	    
  CanvasArrayMatchUHitMult[2]=TQRootCanvasMultMatchU2;	    
  CanvasArrayMatchUHitMult[3]=TQRootCanvasMultMatchU3;
  CanvasArrayMatchUHitMult[4]=TQRootCanvasMultMatchU4;   
  CanvasArrayMatchUHitMult[5]=TQRootCanvasMultMatchU5;   
  for(Int_t i=0;i<6;i++)
    {
      CanvasArrayMatchUHitMult[i]->getCanvas()->SetFillColor(1);	
      CanvasArrayMatchUHitMult[i]->getCanvas()->SetTickx();
      CanvasArrayMatchUHitMult[i]->getCanvas()->SetTicky(); 	
    }
  // ---- Sub Tab   
  CanvasArrayMatchUSect[0] = TQRootCanvasMatchUSec0;
  CanvasArrayMatchUSect[1] = TQRootCanvasMatchUSec1;
  CanvasArrayMatchUSect[2] = TQRootCanvasMatchUSec2;
  CanvasArrayMatchUSect[3] = TQRootCanvasMatchUSec3;
  CanvasArrayMatchUSect[4] = TQRootCanvasMatchUSec4;
  CanvasArrayMatchUSect[5] = TQRootCanvasMatchUSec5;
  CanvasArrayMatchUSect[6] = TQRootCanvasMatchUSec6;

  for(Int_t i=0;i<7;i++)
    {
      CanvasArrayMatchUSect[i]->getCanvas()->SetFillColor(1);
      CanvasArrayMatchUSect[i]->getCanvas()->SetTickx();
      CanvasArrayMatchUSect[i]->getCanvas()->SetTicky();
    }

  LCDArrayMatchU[0] = LCDMatchUHistS0;
  LCDArrayMatchU[1] = LCDMatchUHistS1;
  LCDArrayMatchU[2] = LCDMatchUHistS2;
  LCDArrayMatchU[3] = LCDMatchUHistS3;
  LCDArrayMatchU[4] = LCDMatchUHistS4;
  LCDArrayMatchU[5] = LCDMatchUHistS5;
  LCDArrayMatchU[6] = LCDMatchUHistS6;    
  // --------------------------------------------------------------------
    
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

  //cout << "draw his name: " << His->GetName() << endl;
  //--------- Take care of additional color settings

  if( (strcmp(His->GetName(),"hTriggerIPUtrendTof")      ==0) && (tofArraySetting==0)      ) { arrayNamingAndColor( (TNamed*)His ); tofArraySetting=1;      }
  if( (strcmp(His->GetName(),"hTriggerIPUtrendRich")     ==0) && (richArraySetting==0)     ) { arrayNamingAndColor( (TNamed*)His ); richArraySetting=1;     }
  if( (strcmp(His->GetName(),"hTriggerIPUtrendShower")   ==0) && (showerArraySetting==0)   ) { arrayNamingAndColor( (TNamed*)His ); showerArraySetting=1;   }
  if( (strcmp(His->GetName(),"hTriggerIPUtrendMatchU")   ==0) && (matchuArraySetting==0)   ) { arrayNamingAndColor( (TNamed*)His ); matchuArraySetting=1;   }
  if( (strcmp(His->GetName(),"hTriggerMultDist")         ==0) && (multArraySetting==0)     ) { arrayNamingAndColor( (TNamed*)His ); multArraySetting=1;     }
  if( (strcmp(His->GetName(),"hTriggerRichRingMultDist") ==0) && (richRingArraySetting==0) ) { arrayNamingAndColor( (TNamed*)His ); richRingArraySetting=1; }
  if( (strcmp(His->GetName(),"hTriggerTofHitMultDist")   ==0) && (tofHitArraySetting==0)   ) { arrayNamingAndColor( (TNamed*)His ); tofHitArraySetting=1;   }
  if( (strcmp(His->GetName(),"hTriggerShowerHitMultDist")==0) && (showerHitArraySetting==0)) { arrayNamingAndColor( (TNamed*)His ); showerHitArraySetting=1;}
  if( (strcmp(His->GetName(),"hTriggerMatchUHitMultDist")==0) && (matchuHitArraySetting==0)) { arrayNamingAndColor( (TNamed*)His ); matchuHitArraySetting=1;}
    
  if( His->InheritsFrom("HTrendArray") ) { arrayScaling( (TNamed *)His ); }


  //---------- Draw histograms and lcds
  //------------------------------------------------------------------------------------------------------------
  if(strcmp(TabWidget->currentPage()->name(),"Overview")==0)
    {
      //---------------------------------------------------------------------------------------
      if( (strcmp (His->GetName(),"hTriggerIPUtrendTof")==0) && (freezeTof->isChecked()==0) )
	{
	  fxQtCanvasTOFmain->getCanvas()->cd();
	  His->setCanvas(fxQtCanvasTOFmain->getCanvas());
	  ((HTrendArray *) His)->draw(0);
	  fxQtCanvasTOFmain->getCanvas()->Update();
	  for(Int_t i=0;i<7;i++)
	    {
	      TH1 *p = ((HTrendArray *) His)->getP(0,i);
	      LCDArrayTof[i]->display( p->GetBinContent( (p->GetNbinsX()) ) );
	    }
	}
      //---------------------------------------------------------------------------------------
      else if(strcmp(His->GetName(),"hTriggerTofNumbers")==0)
	{
	  TH1 *p = ((HMonHist *) His)->getP();
	  for(Int_t i=0;i<7;i++)
	    {
	      LCDArrayTofNumbers[i]->display( p->GetBinContent(i+1) );
	    }
	}
      //---------------------------------------------------------------------------------------
      else if( (strcmp(His->GetName(),"hTriggerIPUtrendRich")==0) && (freezeRich->isChecked()==0) )
	{
	  fxQtCanvasRichmain->getCanvas()->cd();
	  His->setCanvas(fxQtCanvasRichmain->getCanvas());
	  ((HTrendArray *) His)->draw(0);
	  fxQtCanvasRichmain->getCanvas()->Update();
	  for(Int_t i=0;i<7;i++)
	    {
	      TH1 *p = ((HTrendArray *) His)->getP(0,i);
	      LCDArrayRich[i]->display( p->GetBinContent( (p->GetNbinsX()) ) );
	    }
	}
      //---------------------------------------------------------------------------------------
      else if(strcmp(His->GetName(),"hTriggerRichNumbers")==0)
	{
	  TH1 *p = ((HMonHist *) His)->getP();
	  for(Int_t i=0;i<6;i++)
	    {
	      LCDArrayRichNumbers[i]->display( p->GetBinContent(i+1) );
	    }
	}
      //---------------------------------------------------------------------------------------
      else if( (strcmp(His->GetName(),"hTriggerIPUtrendShower")==0) && (freezeShower->isChecked()==0) )
	{
	  fxQtCanvasShowermain->getCanvas()->cd();
	  His->setCanvas(fxQtCanvasShowermain->getCanvas());
	  ((HTrendArray *) His)->draw(0);
	  fxQtCanvasShowermain->getCanvas()->Update();
	  for(Int_t i=0;i<7;i++)
	    {
	      TH1 *p = ((HTrendArray *) His)->getP(0,i);
	      LCDArrayShower[i]->display( p->GetBinContent( (p->GetNbinsX()) ) );
	    }
	}
      //---------------------------------------------------------------------------------------
      else if( (strcmp(His->GetName(),"hTriggerIPUtrendMatchU")==0) && (freezeMatchU->isChecked()==0) )
	{
          fxQtCanvasMatchUmain->getCanvas()->cd();
	  His->setCanvas(fxQtCanvasMatchUmain->getCanvas());
	  ((HTrendArray *) His)->draw(0);
	  fxQtCanvasMatchUmain->getCanvas()->Update();
	  for(Int_t i=0;i<7;i++)
	    {
	      TH1 *p = ((HTrendArray *) His)->getP(0,i);
	      LCDArrayMatchU[i]->display( p->GetBinContent( (p->GetNbinsX()) ) );
	    }
	}
    }
  //------------------------------------------------------------------------------------------------------------
  else if( strcmp(TabWidget->currentPage()->name(),"Tof_sub")==0 && strcmp(TabWidgetTof->currentPage()->name(),"Tof_sect")==0 )
    {
      if(strcmp(His->GetName(),"hTriggerIPUtrendTof")==0)
	{
	  for(Int_t i=0;i<7;i++)
	    {
	      CanvasArrayTofSect[i]->getCanvas()->cd();
	      ((HTrendArray *) His) ->getP(0,i)->Draw() ;
	      CanvasArrayTofSect[i]->getCanvas()->Update();
	    }
	} 
    }
  //------------------------------------------------------------------------------------------------------------
  else if( strcmp(TabWidget->currentPage()->name(),"Rich_sub")==0 && strcmp(TabWidgetRich->currentPage()->name(),"Rich_sect")==0 )
    {
      if(strcmp(His->GetName(),"hTriggerIPUtrendRich")==0)
	{
	  for(Int_t i=0;i<7;i++)
	    {
	      CanvasArrayRichSect[i]->getCanvas()->cd();
	      ((HTrendArray *) His) ->getP(0,i)->Draw() ;
	      CanvasArrayRichSect[i]->getCanvas()->Update();
	    }
	} 
    }
  //------------------------------------------------------------------------------------------------------------
  else if( strcmp(TabWidget->currentPage()->name(),"Shower_sub")==0 && strcmp(TabWidgetShower->currentPage()->name(),"Shower_sect")==0 )
    {
      if(strcmp(His->GetName(),"hTriggerIPUtrendShower")==0)
	{
	  for(Int_t i=0;i<7;i++)
	    {
	      CanvasArrayShowerSect[i]->getCanvas()->cd();
	      ((HTrendArray *) His) ->getP(0,i)->Draw() ;
	      CanvasArrayShowerSect[i]->getCanvas()->Update();
	    }
	} 
    }
  //------------------------------------------------------------------------------------------------------------
  else if( strcmp(TabWidget->currentPage()->name(),"MatchU_sub")==0 && strcmp(TabWidgetMatchU->currentPage()->name(),"MatchU_sect")==0 )
    {
      if(strcmp(His->GetName(),"hTriggerIPUtrendMatchU")==0)
	{
	  for(Int_t i=0;i<7;i++)
	    {
	      CanvasArrayMatchUSect[i]->getCanvas()->cd();
	      ((HTrendArray *) His) ->getP(0,i)->Draw() ;
	      CanvasArrayMatchUSect[i]->getCanvas()->Update();
	    }
	} 
    }
  //------------------------------------------------------------------------------------------------------------
  else if(strcmp(TabWidget->currentPage()->name(),"multdistr")==0)
    {
      //---------------------------------------------------------------------------------------
      if(strcmp(TabWidgetMultDistr->currentPage()->name(),"multall")==0)
	{
	  if(strcmp(His->GetName(),"hTriggerMultDist")==0)
	    {
	      fxQtCanvasRichMultiplicityDistrib->getCanvas()->cd();
	      ((HHistArray *) His)->getP(0,0)->Draw();
	      fxQtCanvasRichMultiplicityDistrib->getCanvas()->Update();
	  
	      fxQtCanvasTOFMultiplicityDistrib->getCanvas()->cd();
	      ((HHistArray *) His)->getP(0,1)->Draw();
	      fxQtCanvasTOFMultiplicityDistrib->getCanvas()->Update();
	  
	      fxQtCanvasShowerMultiplicityDistrib->getCanvas()->cd();
	      ((HHistArray *) His)->getP(0,2)->Draw();
	      fxQtCanvasShowerMultiplicityDistrib->getCanvas()->Update();

	      fxQtCanvasMatchUMultiplicityDistrib->getCanvas()->cd();
	      ((HHistArray *) His)->getP(0,3)->Draw();
	      fxQtCanvasMatchUMultiplicityDistrib->getCanvas()->Update();	    }
	}
      //---------------------------------------------------------------------------------------
      else if(strcmp(TabWidgetMultDistr->currentPage()->name(),"multrich")==0)
	{
	  if(strcmp(His->GetName(),"hTriggerRichRingMultDist")==0)
	    {
	      for(Int_t i=0;i<6;i++)
		{
		  CanvasArrayRichRingMult[i]->getCanvas()->cd();
		  ((HHistArray *) His)->getP(0,i)->Draw();
		  CanvasArrayRichRingMult[i]->getCanvas()->Update();
		}
	    }
	}
      //---------------------------------------------------------------------------------------
      else if(strcmp(TabWidgetMultDistr->currentPage()->name(),"multtof")==0)
	{
	  if(strcmp(His->GetName(),"hTriggerTofHitMultDist")==0)
	    {
	      for(Int_t i=0;i<6;i++)
		{
		  CanvasArrayTofHitMult[i]->getCanvas()->cd();
		  ((HHistArray *) His)->getP(0,i)->Draw();
		  CanvasArrayTofHitMult[i]->getCanvas()->Update();
		}
	    }
	}
      //---------------------------------------------------------------------------------------
      else if(strcmp(TabWidgetMultDistr->currentPage()->name(),"multshower")==0)
	{
	  if(strcmp(His->GetName(),"hTriggerShowerHitMultDist")==0)
	    {
	      for(Int_t i=0;i<6;i++)
		{
		  CanvasArrayShowerHitMult[i]->getCanvas()->cd();
		  ((HHistArray *) His)->getP(0,i)->Draw();
		  CanvasArrayShowerHitMult[i]->getCanvas()->Update();
		}
	    }
	}
      //---------------------------------------------------------------------------------------
      else if(strcmp(TabWidgetMultDistr->currentPage()->name(),"multmatchu")==0)
	{
	  if(strcmp(His->GetName(),"hTriggerMatchUHitMultDist")==0)
	    {
	      for(Int_t i=0;i<6;i++)
		{
		  CanvasArrayMatchUHitMult[i]->getCanvas()->cd();
		  ((HHistArray *) His)->getP(0,i)->Draw();
		  CanvasArrayMatchUHitMult[i]->getCanvas()->Update();
		}
	    }
	}
      //---------------------------------------------------------------------------------------
      //---------------------------------------------------------------------------------------
      //---------------------------------------------------------------------------------------
    }
  //------------------------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------------------------
}

// used to modify settings which have to be modified only once (color settings, title modifications, ...)
void arrayNamingAndColor( TNamed *his )
{
  if(his)
    {
      //---------- Checking for Trendarray
      if( his->InheritsFrom("HTrendArray")  )
	{
	  HTrendArray *His = (HTrendArray*) his;
	  if(His)
	    {
	      //--------------------------
	      if(strcmp(His->GetName(),"hTriggerIPUtrendTof"   )==0 || strcmp(His->GetName(),"hTriggerIPUtrendRich"  )==0 ||
		 strcmp(His->GetName(),"hTriggerIPUtrendShower")==0 || strcmp(His->GetName(),"hTriggerIPUtrendMatchU")==0 )
		{
		  for(Int_t i=0;i<8;i++) // "8" to include also the dummie histogram which contains the global format settings.
		    {
		      His ->getP(0,i)->GetXaxis()->SetAxisColor(0);
		      His ->getP(0,i)->GetYaxis()->SetAxisColor(0);
		      His ->getP(0,i)->GetXaxis()->SetLabelColor(0);
		      His ->getP(0,i)->GetYaxis()->SetLabelColor(0);
		      His ->getP(0,i)->GetXaxis()->SetTitleColor(0);
		      His ->getP(0,i)->GetYaxis()->SetTitleColor(0);
		    }
		  TString secnum[6] = { "0","1","2","3","4","5" };
		  His->getP(0,6)->SetTitle( (TString)His->getP(0,6)->GetTitle()+" global" );                //--- append " global" to the title of the global trendhist of each ipu
		  for(Int_t i=0;i<6;i++) // "6" to exclude the global histogram.
		    {
		      His->getP(0,i)->SetTitle( (TString)His->getP(0,i)->GetTitle()+" sector_"+secnum[i] ); //--- append sector number to non global trend hists
		    }
		}
	      //---------------------------
	      //---------------------------
	    }
	}
      //---------- else Checking for Histarray
      else if( his->InheritsFrom("HHistArray") )
	{
	  HHistArray *His = (HHistArray*)his;
	  if(His)
	    {
	      //--------------------------
	      if(strcmp(His->GetName(),"hTriggerMultDist")==0 )
		{
		  TString names[4] = { "_Rich","_Tof","_Shower","_Matching Unit" };
		  for(Int_t i=0;i<4;i++)
		    {
		      His ->getP(0,i)->GetXaxis()->SetAxisColor(0);
		      His ->getP(0,i)->GetYaxis()->SetAxisColor(0);
		      His ->getP(0,i)->GetXaxis()->SetLabelColor(0);
		      His ->getP(0,i)->GetYaxis()->SetLabelColor(0);
		      His ->getP(0,i)->GetXaxis()->SetTitleColor(0);
		      His ->getP(0,i)->GetYaxis()->SetTitleColor(0);

		      His->getP(0,i)->SetTitle( (TString)His->getP(0,i)->GetTitle()+names[i] );	  
		    }
		}
	      //--------------------------
	      if( (strcmp(His->GetName(),"hTriggerRichRingMultDist")==0)   || (strcmp(His->GetName(),"hTriggerTofHitMultDist")==0 )   ||
                  (strcmp(His->GetName(),"hTriggerShowerHitMultDist")==0 ) || (strcmp(His->GetName(),"hTriggerMatchUHitMultDist")==0 )   )
		{
		  TString secnum[6] = { "0","1","2","3","4","5" };
		  for(Int_t i=0;i<6;i++)
		    {
		      His ->getP(0,i)->GetXaxis()->SetAxisColor(0);
		      His ->getP(0,i)->GetYaxis()->SetAxisColor(0);
		      His ->getP(0,i)->GetXaxis()->SetLabelColor(0);
		      His ->getP(0,i)->GetYaxis()->SetLabelColor(0);
		      His ->getP(0,i)->GetXaxis()->SetTitleColor(0);
		      His ->getP(0,i)->GetYaxis()->SetTitleColor(0);
		
		      His->getP(0,i)->SetTitle( (TString)His->getP(0,i)->GetTitle()+"_sector_"+secnum[i] );	  
		    }
		}
	      //-------------------------
	      //-------------------------
	      //-------------------------
	    }
	}
    }
}

// used to take care of settings which have to be redone every time the array drops in. (y-axis scaling, ...)
void arrayScaling( TNamed *his )
{
  if(his)
    {
      //---------- Checking for Trendarray
      if( his->InheritsFrom("HTrendArray")  )
	{
	  HTrendArray *His = (HTrendArray*) his;
	  if(His)
	    {
	      //--------------------------
	      if(strcmp(His->GetName(),"hTriggerIPUtrendTof"   )==0 || strcmp(His->GetName(),"hTriggerIPUtrendRich"  )==0 ||
		 strcmp(His->GetName(),"hTriggerIPUtrendShower")==0 || strcmp(His->GetName(),"hTriggerIPUtrendMatchU")==0 )
		{
		  Double_t maxtemp=0, MAX=0;
		  for(Int_t i=0;i<6;i++)// "6" to exclude the global histogram.
		    {
		      maxtemp = His->getP(0,i)->GetBinContent( His->getP(0,i)->GetMaximumBin() );
		      if(MAX < maxtemp) { MAX = maxtemp; }     //--- determine y maximum of sectorwise trendhists.
		    }
		  for(Int_t i=0;i<6;i++)// "6" to exclude the global histogram.
		    {
		      His->getP(0,i)->SetMaximum(MAX*1.1);     //--- scale all y axis to y maximum+10%.
		    }
		}
	      //---------------------------
	      //---------------------------
	    }
	}
      //---------- else Checking for Histarray
      else if( his->InheritsFrom("HHistArray") )
	{
	  HHistArray *His = (HHistArray*)his;
	  if(His)
	    {
	      //-------------------------- yet empty for histarrays...
	      //--------------------------
	      //--------------------------
	      //--------------------------
	    }
	}
    }
}

void TGo4TriggerOnline::downscaled( int index )
{
  cout << "index: " << index << endl;
    
  TGo4TriggerParameter* triggerParameter = new TGo4TriggerParameter("TriggerParam","TriggerParam");
  triggerParameter->SetUseDownscaled((Bool_t)index);     
  cout << "Bool_t index" << (Bool_t)index << endl;
  
  TGo4ComSetParameter* com = new TGo4ComSetParameter("TriggerParam");
  com->SetParameter(triggerParameter);
  fxTGo4GUIRegistry->SubmitCommand(com);
}


void TGo4TriggerOnline::Reset()
{
    TGo4ResetParameter* reset = new TGo4ResetParameter("HistReset","HistReset");
    reset->SetTriggerChanged(kTRUE);
    reset->Set("Trigger",0);     
    TGo4ComSetParameter* com=new TGo4ComSetParameter("HistReset");
    com->SetParameter(reset);
    fxTGo4GUIRegistry->SubmitCommand(com);
}
