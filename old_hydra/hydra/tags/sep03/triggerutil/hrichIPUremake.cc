using namespace std;
#include "hrichIPUremake.h"
#include "hdebug.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hmatrixcatiter.h"
#include "triggerinfodef.h"
#include "htriggerdetector.h"
#include "hlocation.h"
#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"

#include <iostream>          
#include <iomanip>

void HRichIPURemake::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb) {
  // Adds to the runtime database "rtdb" the containers needed by the MU Emulation
  //spec is used to get information of the spectrometer setup.
  if (!fRichIPUParThr) {
    fRichIPUParThr=(HRichIPUParThresholds *)rtdb->getContainer("RichIPUParThresholds");
    if (fRichIPUParThr) {
      cout << "RichIPU Parameters Thresholds initialized" << endl;
    } else {
      cout << "******  RichIPU Parameters Thresholds Initialization failed!! *********" << endl;
    }
  }  

  fRichIPUParLmax=(HRichIPUParLocMax *)rtdb->getContainer("RichIPUParLocMax");
  if (fRichIPUParLmax) {
    cout << "RichIPU Parameters LocMax initialized" << endl;
  } else {
    cout << "******  RichIPU Parameters LocMax Initialization failed!! *********" << endl;
  }

}
//============================================================================

//----------------------------------------------------------------------------
HRichIPURemake::HRichIPURemake() { 

  pRich= NULL;
  iterRich=NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichIPURemake::HRichIPURemake(Text_t *name,Text_t *title)
               : HReconstructor(name, title) {

  pRich= NULL;
  iterRich=NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichIPURemake::~HRichIPURemake() { 
  // set everything to 0
  // especially the iterators

  if (pRich) delete pRich;
  pRich= NULL;
  
  if (iterRich) delete iterRich;
  iterRich=NULL;
}

//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichIPURemake::init(){
  cout << "-------------- INIT of HRichIPURemake() ------------------- " << endl;
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");

  // initialize parameters
  if (pTrigDet)
    initParContainer(spec,rtdb);
  
  // allocate input categories
  pRich=gHades->getCurrentEvent()->getCategory(catHardRichHit);
  if (!pRich) {
    return kFALSE;
  }
  else gHades->getCurrentEvent()
	 ->addCategory(catHardRichHit, pRich, "Rich");  
  iterRich = (HIterator*)pRich->MakeIterator("native");
 
  return kTRUE;
}

//============================================================================


//----------------------------------------------------------------------------
Int_t HRichIPURemake::execute() { 
  prepareInput();
  HLocMaxFilter locmaxfilter; 
  pRich->filter(locmaxfilter);
  return 0;
}
//============================================================================


//----------------------------------------------------------------------------
void HRichIPURemake::prepareInput() { 
  HRichHitIPU   *pMURich=NULL;
  Int_t sector;
  Int_t yRichHitIPU, xRichHitIPU;
  Int_t RingsumPattern[6][96][96];
  for (Int_t s=0;s<6;s++) 
    for (Int_t r=0;r<96;r++)
      for(Int_t c=0;c<96;c++)
	RingsumPattern[s][r][c]= 0;

  iterRich->Reset();
  while ((pMURich = (HRichHitIPU *)iterRich->Next()) != 0) {
    sector = pMURich->getSector();

    if( (pMURich->getQuality()    > fRichIPUParThr->getThresholds(0,sector)) &&
	(pMURich->getVetoQuality()< -fRichIPUParThr->getThresholds(1,sector))) {
      yRichHitIPU = pMURich->getY();
      xRichHitIPU = pMURich->getX();
      RingsumPattern[sector][xRichHitIPU][yRichHitIPU]=pMURich->getQuality();
    }
  }
  iterRich->Reset();
  while ((pMURich = (HRichHitIPU *)iterRich->Next()) != 0 ) {
    Int_t fulfill=0;
    sector = pMURich->getSector();
    xRichHitIPU = pMURich->getX();
    yRichHitIPU = pMURich->getY();
    if (RingsumPattern[sector][xRichHitIPU][yRichHitIPU] ){
      Int_t CurrentSum = pMURich->getQuality();

      if  (fRichIPUParLmax->getLocMaxFlag()==1) {
	if ( (fRichIPUParLmax->getLocMax()==4)||(fRichIPUParLmax->getLocMax()==8) )  {
	  if ( CurrentSum  >= RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU] &&
	       CurrentSum  >= RingsumPattern[sector][xRichHitIPU][yRichHitIPU+1] &&
	       CurrentSum  >  RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU] &&
	       CurrentSum  >  RingsumPattern[sector][xRichHitIPU][yRichHitIPU-1] ) {
	    
	    if (fRichIPUParLmax->getLocMax()==8) {
	      if ( CurrentSum  >= RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU+1] &&
		   CurrentSum  >= RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU+1] &&
		   CurrentSum  >  RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU-1] &&
		   CurrentSum  >  RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU-1] ) {
		fulfill=1;
	      }
	    } else {
	      fulfill=1;	    
	    }	      
	  }
	} else {
	  cout << " wrong parameter value!!!" << endl;
	}
      } else {
	fulfill=1;
      }

      if (fulfill==0) {
	  pMURich->setX(9999);    // x==9999 used as flag for filtering out 
      }	  
    } else {
	pMURich->setX(9999);    // x==9999 used as flag for filtering out 
    }
  }

}

//----------------------------------------------------------------------------
Bool_t HRichIPURemake::finalize(){
 return kTRUE;
}
//============================================================================

ClassImp(HRichIPURemake)





  





