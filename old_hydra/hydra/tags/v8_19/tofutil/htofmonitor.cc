#include "htofmonitor.h"
#include "htofeventdisplay.h"
#include "hades.h"
#include "htofhit.h"
#include "htofraw.h"
#include "hcategory.h"
#include "hdebug.h"
#include "tofdef.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "TH2.h"


//*-- Author : D.Vasiliev 05.07.99
//*-- Modified : 05/07/99 by D.Vasiliev

//_HADES_CLASS_DESCRIPTION 
////////////////////////
//HTofMonitor class
//
// This class is responsible of histogram filling for TOF detector
////////////////////////



HTofMonitor::HTofMonitor(void) {

  fLoc.set(3,0,0,0);

}


HTofMonitor::HTofMonitor(Text_t *name,Text_t *title) : 

  HReconstructor (name,title) {
  fLoc.set(3,0,0,0);

}


Bool_t HTofMonitor::init(void) {

  fRawCat=gHades->getCurrentEvent()->getCategory(catTofRaw);
  if (!fRawCat) {
    fRawCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofRaw);
    if (!fRawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofRaw,fRawCat,"Tof");
  }

  fHitCat=gHades->getCurrentEvent()->getCategory(catTofHit);
  if (!fHitCat) {
    fHitCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofHit);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofHit,fHitCat,"Tof");
  }

  return kTRUE;

}


Int_t HTofMonitor::execute(void) {

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HTofMonitor::execute");
#endif

  if(!gTofDisplay) return 1;

  HTofRaw *raw=NULL;
  HTofHit *hit=NULL;

  Int_t i;
  Int_t j;

  Int_t mult3L, mult6L, multAllL;
  Int_t mult3R, mult6R, multAllR;
  Int_t mult3Tot, mult6Tot, multAllTot;

  HTofHisto *fTofHisto;


  mult3L = mult3R = mult3Tot = 0;
  mult6L = mult6R = mult6Tot = 0;
  multAllL = multAllR = multAllTot = 0;

  for(i=0; i<384; i++) {

    fTofHisto = (HTofHisto*) gTofDisplay->getHistoCont()->At(i);
    if(fTofHisto) {

      j = i%64;
      fLoc[0] = i/64; //sector
      fLoc[1] = j/8;  //module
      fLoc[2] = j%8;  //rod



      raw = (HTofRaw*) fRawCat->getObject(fLoc);
      hit = (HTofHit*) fHitCat->getObject(fLoc);

      if(raw) {

	//      cout << "sector = "<<fLoc[0]<<"  mod = "<<fLoc[1]<<
	//"  rod = "<<fLoc[2]<<endl;

	if(raw->getLeftTime() || raw->getRightTime()) { 


	  if(raw->getLeftTime()) {
	    if(i>=128 && i<192) {
	      mult3L++;
	      ((TH1F*) gTofDisplay->getHistoCont()->At(400))
		->Fill((Float_t)(i-127));
	    }
	    if(i>=320 && i<384) {
	      mult6L++;
	      ((TH1F*) gTofDisplay->getHistoCont()->At(403))
		->Fill((Float_t)(i-319));
	    }
	    multAllL++;
	  }

	  if(raw->getRightTime()) {
	    if(i>=128 && i<192) {
	      mult3R++;
	      ((TH1F*) gTofDisplay->getHistoCont()->At(401))
		->Fill((Float_t)(i-127));
	    }
	    if(i>=320 && i<384) {
	      mult6R++;
	      ((TH1F*) gTofDisplay->getHistoCont()->At(404))
		->Fill((Float_t)(i-319));
	    }
	    multAllR++;
	  }

	  if(raw->getLeftTime() && raw->getRightTime()) {
	    if(i>=128 && i<192) {
	      mult3Tot++;
	      ((TH1F*) gTofDisplay->getHistoCont()->At(402))
		->Fill((Float_t)(i-127));
	    }
	    if(i>=320 && i<384) {
	      mult6Tot++;
	      ((TH1F*) gTofDisplay->getHistoCont()->At(405))
		->Fill((Float_t)(i-319));
	    }
	    multAllTot++;
	  }

	  if(raw->getLeftTime()) fTofHisto->left->Fill(raw->getLeftTime());
	  if(raw->getRightTime()) fTofHisto->right->Fill(raw->getRightTime());

	  if(raw->getLeftTime() && raw->getRightTime()) {
	    fTofHisto->halfs->Fill((Float_t)(0.5*(raw->getLeftTime() +
						  raw->getRightTime())));
	    fTofHisto->halfd->Fill((Float_t)(0.5*(raw->getLeftTime() -
						  raw->getRightTime())));
	  }

	  //	  fTofHisto->lr->Fill(raw->getRightTime(),raw->getLeftTime());

	  ((TH2F*)gTofDisplay->getHistoCont()->At(384))
	    ->Fill(raw->getRightTime(),raw->getLeftTime());
	  ((TH2F*)gTofDisplay->getHistoCont()->At(387))
	    ->Fill(raw->getLeftTime(),(Float_t)(i+1));
	  ((TH2F*)gTofDisplay->getHistoCont()->At(390))
	    ->Fill(raw->getRightTime(),(Float_t)(i+1));
	}
      }
      if(hit && raw) {
	if(raw->getLeftTime() && raw->getRightTime()) { 

	  fTofHisto->tof->Fill(hit->getTof());
	  fTofHisto->xpos->Fill(hit->getXpos());
	  //	  fTofHisto->lt->Fill(hit->getTof(),raw->getLeftTime());
	  //	  fTofHisto->lx->Fill(hit->getXpos(),raw->getLeftTime());
	  //	  fTofHisto->rt->Fill(hit->getTof(),raw->getRightTime());
	  //	  fTofHisto->rx->Fill(hit->getXpos(),raw->getRightTime());
	  //	  fTofHisto->tx->Fill(hit->getXpos(),hit->getTof());


	  ((TH2F*)gTofDisplay->getHistoCont()->At(385))
	    ->Fill(hit->getTof(),raw->getLeftTime());
	  ((TH2F*)gTofDisplay->getHistoCont()->At(386))
	    ->Fill(hit->getXpos(),raw->getLeftTime());
	  ((TH2F*)gTofDisplay->getHistoCont()->At(388))
	    ->Fill(hit->getTof(),raw->getRightTime());
	  ((TH2F*)gTofDisplay->getHistoCont()->At(389))
	    ->Fill(hit->getXpos(),raw->getRightTime());
	  ((TH2F*)gTofDisplay->getHistoCont()->At(391))
	    ->Fill(hit->getXpos(),hit->getTof());
	  ((TH2F*)gTofDisplay->getHistoCont()->At(392))
	    ->Fill(hit->getTof(),(Float_t)(i+1));
	  ((TH2F*)gTofDisplay->getHistoCont()->At(393))
	    ->Fill(hit->getXpos(),(Float_t)(i+1));
	}
      }
    }
  } 

  ((TH1F*) gTofDisplay->getHistoCont()->At(394))->Fill((Float_t)mult3L);
  ((TH1F*) gTofDisplay->getHistoCont()->At(395))->Fill((Float_t)mult3R);
  ((TH1F*) gTofDisplay->getHistoCont()->At(396))->Fill((Float_t)mult3Tot);
  ((TH1F*) gTofDisplay->getHistoCont()->At(397))->Fill((Float_t)mult6L);
  ((TH1F*) gTofDisplay->getHistoCont()->At(398))->Fill((Float_t)mult6R);
  ((TH1F*) gTofDisplay->getHistoCont()->At(399))->Fill((Float_t)mult6Tot);
  ((TH1F*) gTofDisplay->getHistoCont()->At(406))->Fill((Float_t)multAllL);
  ((TH1F*) gTofDisplay->getHistoCont()->At(407))->Fill((Float_t)multAllR);
  ((TH1F*) gTofDisplay->getHistoCont()->At(408))->Fill((Float_t)multAllTot);
  

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HTofMonitor::execute");
#endif

  return 0;
}


ClassImp(HTofMonitor)











