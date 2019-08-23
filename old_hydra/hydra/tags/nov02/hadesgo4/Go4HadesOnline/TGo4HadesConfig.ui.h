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
	HGo4OnlineParam* param=configparam->GetPar();
	configparam->SetChanged();

	HGo4OnlineParam* copyparam=(HGo4OnlineParam*)param->Clone();
	writeParams(param);

	if(param->isEqual(copyparam))
	{
	    delete copyparam;
	}
	else if(param->isEqual(copyparam,1))
	{
	    delete copyparam;
	    TGo4HadesConfigParameter* copyconfig=(TGo4HadesConfigParameter*)configparam->Clone();
	    copyconfig->SetName("Hadesconfig");
	    TGo4ComSetParameter* com=new TGo4ComSetParameter("Hadesconfig");
	    com->SetParameter(copyconfig);
	    fxTGo4GUIRegistry->SubmitCommand(com);
	}
	else
	{
	    delete copyparam;
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
  HGo4OnlineParam* param=configparam->GetPar();

  nLoopspin          ->setValue(param->getNLoop());
  sleepspin          ->setValue(param->getSleep());
  maxratespin        ->setValue(param->getMaxRate());

  triggerrefreshspin ->setValue(param->getTriggerRefresh());
  startrefreshspin   ->setValue(param->getStartRefresh());
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
  mdcbox             ->setCurrentItem(param->getMdcMaxLevel()-1);
  tofbox             ->setCurrentItem(param->getTofMaxLevel()-1);
  tofinobox          ->setCurrentItem(param->getTofinoMaxLevel()-1);
  showerbox          ->setCurrentItem(param->getShowerMaxLevel()-1);

  startbox  ->removeItem(3);
  richbox   ->removeItem(3);
  mdcbox    ->removeItem(3);
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


void TGo4HadesConfig::writeParams( HGo4OnlineParam * p )
{
    p->setNLoop(nLoopspin->value());
    p->setSleep(sleepspin->value());
    p->setMaxRate(maxratespin->value());

    p->setTriggerRefresh(triggerrefreshspin->value());
    p->setStartRefresh  (startrefreshspin->value());
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
    p->setMdcMaxLevel   (mdcbox->currentItem()+1);

    p->setTof           (tofcheck->isChecked());
    p->setTofMaxLevel   (tofbox->currentItem()+1);

    p->setTofino        (tofinocheck->isChecked());
    p->setTofinoMaxLevel(tofinobox->currentItem()+1);

    p->setShowerTofino  (showertofinocheck->isChecked());

    p->setShower        (showercheck->isChecked());
    p->setShowerMaxLevel(showerbox->currentItem()+1);
}
