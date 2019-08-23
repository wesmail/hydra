/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/


void TGo4HadesConfig::init()
{
    fxTGo4GUIRegistry = TGo4GUIRegistry::Instance(); 	/** Get the pointer to the GUI Registry  **/

    fxTGo4HadesConfigSlots = dynamic_cast <TGo4HadesConfigSlots *> (fxTGo4GUIRegistry->GetSlotClass("TGo4HadesConfigSlots"));

    if(fxTGo4HadesConfigSlots  !=0) {
	/* if it is already there, set this GUI  as window of the slot Object */
	fxTGo4HadesConfigSlots->SetGUI(this);
    }else{
	/* if no Slot Object where found creat a new one*/
	fxTGo4HadesConfigSlots = new TGo4HadesConfigSlots ("TGo4HadesConfigSlots" , "Slot Class for Qt GUI", this);
    }
    /* Get the pointer for the Status Object */
    fxTGo4HadesConfigStatus= dynamic_cast <TGo4HadesConfigStatus *> (fxTGo4HadesConfigSlots->GetStatus());


    fxTGo4GUIRegistry->ReDirectObject("Hadesconfig",fxTGo4HadesConfigSlots, true);
    configparam=new TGo4HadesConfigParameter("configlocal","configlocal");
    detectorSetupChanged=kFALSE;

    layerLine[0][0]=s0m0;
    layerLine[0][1]=s0m1;
    layerLine[0][2]=s0m2;
    layerLine[0][3]=s0m3;
    layerLine[1][0]=s1m0;
    layerLine[1][1]=s1m1;
    layerLine[1][2]=s1m2;
    layerLine[1][3]=s1m3;
    layerLine[2][0]=s2m0;
    layerLine[2][1]=s2m1;
    layerLine[2][2]=s2m2;
    layerLine[2][3]=s2m3;
    layerLine[3][0]=s3m0;
    layerLine[3][1]=s3m1;
    layerLine[3][2]=s3m2;
    layerLine[3][3]=s3m3;
    layerLine[4][0]=s4m0;
    layerLine[4][1]=s4m1;
    layerLine[4][2]=s4m2;
    layerLine[4][3]=s4m3;
    layerLine[5][0]=s5m0;
    layerLine[5][1]=s5m1;
    layerLine[5][2]=s5m2;
    layerLine[5][3]=s5m3;

    levelLine[0]= tracklevel0;
    levelLine[1]= tracklevel1;
    levelLine[2]= tracklevel2;
    levelLine[3]= tracklevel3;
}

void TGo4HadesConfig::destroy()
{
    fxTGo4GUIRegistry->RemoveRedirectEntry("Hadesconfig",fxTGo4HadesConfigSlots);
    delete configparam;
}


void TGo4HadesConfig::closeEvent( QCloseEvent *ce )
{
    TGo4LockGuard glob;
    ce->accept();
    fxTGo4HadesConfigSlots->SetGUI(0);
}


void TGo4HadesConfig::newTreeSize()
{

}


void TGo4HadesConfig::getObj()
{
    TGo4ComGetParameter* com=new TGo4ComGetParameter("Hadesconfig");
    fxTGo4GUIRegistry->SubmitCommand(com);
    //delete com;
}

void TGo4HadesConfig::submit()
{
    if(startbox->count()==3)
    {
	HGo4OnlineParam* param=configparam->GetPar();    // get online param
	HMdcSetup* mdcset     =configparam->GetMdcSet(); // get mdc param

	configparam->SetChanged(); // set flag, that this object has been modified

	HGo4OnlineParam* copyparam=(HGo4OnlineParam*)param->Clone(); // make a copy
 	HMdcSetup* copymdcset     =(HMdcSetup*)     mdcset->Clone(); // make a copy

	writeParams(param,mdcset); // fill values from gui

	if(param->isEqual(copyparam)&&mdcset->isEqual(copymdcset))
	{   // if nothing has been changed do nothing
	    delete copyparam;
            delete copymdcset;
	}
	else if(param->isEqual(copyparam,1) && mdcset->isEqual(copymdcset))
	{   // only parameters have been changed which can be modified without reinit
	    delete copyparam;
	    delete copymdcset;

	    TGo4HadesConfigParameter* copyconfig=(TGo4HadesConfigParameter*)configparam->Clone(); // make a copy
	    copyconfig->SetName("Hadesconfig");
	    TGo4ComSetParameter* com=new TGo4ComSetParameter("Hadesconfig");
	    com->SetParameter(copyconfig);
	    fxTGo4GUIRegistry->SubmitCommand(com);
	}
	else
	{   // something important has been changed and a reinit is needed
	    delete copyparam;
	    delete copymdcset;

	    fxTGo4GUIRegistry->SubmitCommand("THStop");

	    TGo4HadesConfigParameter* copyconfig=(TGo4HadesConfigParameter*)configparam->Clone();
	    copyconfig->SetName("Hadesconfig");
	    TGo4ComSetParameter* com=new TGo4ComSetParameter("Hadesconfig");
	    com->SetParameter(copyconfig);
	    fxTGo4GUIRegistry->SubmitCommand(com);

	    fxTGo4GUIRegistry->SubmitCommand("ANInit");
	    fxTGo4GUIRegistry->SubmitCommand("THStart");
	}
    }
}

void TGo4HadesConfig::UpdateParam( TGo4Parameter * par )
{
  configparam->UpdateFrom(par);
  HGo4OnlineParam* param=configparam->GetPar();    // get online param
  HMdcSetup* mdcset     =configparam->GetMdcSet(); // get mdc param

  nLoopspin          ->setValue(param->getNLoop());
  sleepspin          ->setValue(param->getSleep());
  maxratespin        ->setValue(param->getMaxRate());

  triggerrefreshspin ->setValue(param->getTriggerRefresh());
  startrefreshspin   ->setValue(param->getStartRefresh());
  richrefreshspin    ->setValue(param->getRichRefresh());
  richODrefreshspin  ->setValue(param->getRichODRefresh());
  mdcrefreshspin     ->setValue(param->getMdcRefresh());
  tofrefreshspin     ->setValue(param->getTofRefresh());
  tofinorefreshspin  ->setValue(param->getTofinoRefresh());
  showerrefreshspin  ->setValue(param->getShowerRefresh());

  triggercheck       ->setChecked((bool)param->getTrigger());
  startcheck         ->setChecked((bool)param->getStart());
  richcheck          ->setChecked((bool)param->getRich());
  mdccheck           ->setChecked((bool)param->getMdc());
  tofcheck           ->setChecked((bool)param->getTof());
  tofinocheck        ->setChecked((bool)param->getTofino());
  showercheck        ->setChecked((bool)param->getShower());
  showertofinocheck  ->setChecked((bool)param->getShowerTofino());

  startbox           ->setCurrentItem(param->getStartMaxLevel()-1);
  richbox            ->setCurrentItem(param->getRichMaxLevel()-1);
  tofbox             ->setCurrentItem(param->getTofMaxLevel()-1);
  tofinobox          ->setCurrentItem(param->getTofinoMaxLevel()-1);
  showerbox          ->setCurrentItem(param->getShowerMaxLevel()-1);

  // setting mdc
  mdcbox             ->setCurrentItem(mdcset        ->getMdcCommonSet()     ->getAnalysisLevel());
  caltimecutsselect  ->setCurrentItem(mdcset        ->getMdcCalibrater1Set()->getTimeCutFlagCal1());
  calmodeselect      ->setCurrentItem((mdcset       ->getMdcCalibrater1Set()->getModeFlagCal1())-1);
  trackmagnetselect  ->setCurrentItem((Int_t)(mdcset->getMdcTrackFinderSet()->getIsCoilOff()));
  trackmodeselect    ->setCurrentItem(mdcset        ->getMdcTrackFinderSet()->getTypeClustFinder());
  Int_t Layerslocal[24];
  mdcset->getMdcTrackFinderSet()->getNLayers(&Layerslocal[0]);
  for(Int_t s=0;s<6;s++){
      for(Int_t m=0;m<4;m++){
	  writeLineEdit(layerLine[s][m],Layerslocal[s*4+m]);
      }
  }
  Int_t Levellocal[4];
  mdcset->getMdcTrackFinderSet()->getNLevel(&Levellocal[0]);
  for(Int_t m=0;m<4;m++){
      writeLineEdit(levelLine[m],Levellocal[m]);
  }
  segment1->setValue(mdcset->getMdcClusterToHitSet()->getMdcForSeg1());
  segment2->setValue(mdcset->getMdcClusterToHitSet()->getMdcForSeg2());
  fittermodeselect->setCurrentItem(mdcset->getMdc12FitSet()->getFitIndep());

  startbox  ->removeItem(3);
  richbox   ->removeItem(3);
  mdcbox    ->removeItem(5);
  tofinobox ->removeItem(2);
  tofbox    ->removeItem(3);
  showerbox ->removeItem(3);

}


void TGo4HadesConfig::checkConsistencyShowerTofino(bool)
{
    if(tofinobox->count()==2 || showerbox->count()==3 )
    {
	if(showertofinocheck->isChecked())// called from showertofino
	{
	    if(!tofinocheck->isChecked())      tofinocheck   ->setChecked(true);
	    if(tofinobox->currentItem()!=1)    tofinobox     ->setCurrentItem(1);
	    if(!showercheck->isChecked())      showercheck   ->setChecked(true);
	    if(showerbox->currentItem()!=2)    showerbox     ->setCurrentItem(2);
	    if(!showertofinocheck->isChecked())showertofinocheck->setChecked(true);
	}
    }
}
void TGo4HadesConfig::checkConsistencyTofino()
{
    if(tofinobox->count()==2 || showerbox->count()==3 )
    {
	if(!tofinocheck->isChecked()
	   || tofinobox->currentItem()<1)// called from tofino
	{
	    if(showertofinocheck->isChecked())showertofinocheck->setChecked(false);
	}
    }
}
void TGo4HadesConfig::checkConsistencyShower()
{
    if(tofinobox->count()==2 || showerbox->count()==3 )
    {
	if(!showercheck->isChecked()
	   || showerbox->currentItem()<2)// called from shower
	{
	    if(showertofinocheck->isChecked())showertofinocheck->setChecked(false);
	}
    }
}


void TGo4HadesConfig::writeParams( HGo4OnlineParam * p, HMdcSetup * p2 )
{
    p->setNLoop(nLoopspin->value());
    p->setSleep(sleepspin->value());
    p->setMaxRate(maxratespin->value());

    p->setTriggerRefresh(triggerrefreshspin->value());
    p->setStartRefresh  (startrefreshspin->value());
    p->setRichRefresh   (richrefreshspin->value());
    p->setRichODRefresh (richODrefreshspin->value());
    p->setMdcRefresh    (mdcrefreshspin->value());
    p->setTofRefresh    (tofrefreshspin->value());
    p->setTofinoRefresh (tofinorefreshspin->value());
    p->setShowerRefresh (showerrefreshspin->value());

    p->setTrigger       (triggercheck->isChecked());

    p->setStart         (startcheck->isChecked());
    p->setStartMaxLevel (startbox->currentItem()+1);

    p->setRich          (richcheck->isChecked());
    p->setRichMaxLevel  (richbox->currentItem()+1);

    p->setMdc           (mdccheck->isChecked());
    p2->getMdcCommonSet()     ->setAnalysisLevel     (mdcbox             ->currentItem());
    p2->getMdcCalibrater1Set()->setTimeCutFlagCal1   (caltimecutsselect  ->currentItem());
    p2->getMdcCalibrater1Set()->setModeFlagCal1      (calmodeselect      ->currentItem()+1);
    p2->getMdcTrackFinderSet()->setIsCoilOff((Bool_t)(trackmagnetselect  ->currentItem()));
    p2->getMdcTrackFinderSet()->setTypeClustFinder   (trackmodeselect    ->currentItem());

    Int_t Layerslocal[24];
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    Layerslocal[s*4+m]=readLineEdit(layerLine[s][m]);
	}
    }
    p2->getMdcTrackFinderSet()->setNLayers(&Layerslocal[0]);

    Int_t Levellocal[4];
    for(Int_t m=0;m<4;m++){
	Levellocal[m]=readLineEdit(levelLine[m]);
    }
    p2->getMdcTrackFinderSet() ->setNLevel    (&Levellocal[0]);
    p2->getMdcClusterToHitSet()->setMdcForSeg1(segment1->value());
    p2->getMdcClusterToHitSet()->setMdcForSeg2(segment2->value());
    p2->getMdc12FitSet()       ->setFitIndep  (fittermodeselect->currentItem());

    p->setTof           (tofcheck->isChecked());
    p->setTofMaxLevel   (tofbox->currentItem()+1);

    p->setTofino        (tofinocheck->isChecked());
    p->setTofinoMaxLevel(tofinobox->currentItem()+1);

    p->setShowerTofino  (showertofinocheck->isChecked());

    p->setShower        (showercheck->isChecked());
    p->setShowerMaxLevel(showerbox->currentItem()+1);
}

Int_t TGo4HadesConfig::readLineEdit(QLineEdit* line)
{
	QString tmp=line->text();
	return tmp.toInt();
}

void TGo4HadesConfig::checkLineEdit()
{
    Int_t myval;
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    myval=readLineEdit(layerLine[s][m]);
            if(myval<1|| myval>6)writeLineEdit(layerLine[s][m],6);
	}
    }
}

void TGo4HadesConfig::writeLineEdit(QLineEdit* line,Int_t myval)
{
    	QString tmp;
        char  tmp1[10];
	sprintf(tmp1,"%i",myval);
        tmp=tmp1;
	line->setText(tmp);
}
