using namespace std;
#include "hrichIPUremakeSim.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hmatrixcatiter.h"
#include "triggerinfodef.h"
#include "htriggerdetector.h"
#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"
#include "hlocmaxfilterSim.h"
#include "hrichhitIPUSim.h"
#include "hmessagemgr.h"

#include <iostream>
#include <iomanip>

Bool_t HRichIPURemakeSim::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb)
{
  // Adds to the runtime database "rtdb" the containers needed by the MU Emulation
  //spec is used to get information of the spectrometer setup.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns kTRUE if all parameter containers are found, otherwise kTRUE;
  if (!fRichIPUParThr)
    {
      fRichIPUParThr=(HRichIPUParThresholds *)rtdb->getContainer("RichIPUParThresholds");
      if (fRichIPUParThr)
        {
          INFO_msg(10,HMessageMgr::DET_TRIGGER,"RichIPU Parameters Thresholds initialized");
        }
      else
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"RichIPU Parameters Thresholds Initialization failed!");
          return kFALSE;
        }
    }

  fRichIPUParLmax=(HRichIPUParLocMax *)rtdb->getContainer("RichIPUParLocMax");
  if (fRichIPUParLmax)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"RichIPU Parameters LocMax initialized");
    }
  else
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"RichIPU Parameters LocMax Initialization failed!           ");      ;
      return kFALSE;
    }
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichIPURemakeSim::HRichIPURemakeSim(void)
{
  // Default constructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  pRich= NULL;
  iterRich=NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichIPURemakeSim::HRichIPURemakeSim(Text_t *name,Text_t *title)
              : HReconstructor(name, title)
{
  // Default constructor
  //
  // Input parameters:
  //   Text_t *name
  //     Name of the object
  //   Text_t *title
  //     Title of the object
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  pRich= NULL;
  iterRich=NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichIPURemakeSim::~HRichIPURemakeSim(void)
{ 
  // Destructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  // none
  if (iterRich)
    {
      delete iterRich;
      iterRich=NULL;
    }
}

//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichIPURemakeSim::init(void)
{
  // Initialize everything.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Retuurns kFALSE if a category or a parameter container can not ber found,
  //   kTRUE otherwise.
  SEPERATOR_msg("-",60);
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"INIT of HRichIPURemakeSim");
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");

  // initialize parameters
  if (pTrigDet)
    {
      if(!initParContainer(spec,rtdb))
        {
          return kFALSE;
        }
    }

  // allocate input categories
  pRich=gHades->getCurrentEvent()->getCategory(catHardRichHit);
  if (!pRich)
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"Category HardRichHit not found!");
      return kFALSE;
    }
  iterRich = (HIterator*)pRich->MakeIterator("native");
  return kTRUE;
}

//============================================================================

//----------------------------------------------------------------------------
Int_t HRichIPURemakeSim::execute(void)
{
  // Do the job!
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns 0 allways.
  prepareInput();
  HLocMaxFilterSim locmaxfilter;
  pRich->filter(locmaxfilter);
  return 0;
}
//============================================================================


//----------------------------------------------------------------------------
void HRichIPURemakeSim::prepareInput(void)
{ 
  //Coppy needed data from input categories into internal datastructures.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  HRichHitIPUSim   *pMURich=NULL;
  Int_t sector;
  Int_t yRichHitIPU, xRichHitIPU;
  Int_t RingsumPattern[6][96][96];
  for (Int_t s=0;s<6;s++)
    {
      for (Int_t r=0;r<96;r++)
        {
          for(Int_t c=0;c<96;c++)
            {
              RingsumPattern[s][r][c]= 0;
            }
        }
    }

  iterRich->Reset();
  while ((pMURich = (HRichHitIPUSim *)iterRich->Next()) != 0)
    {
      sector = pMURich->getSector();

      yRichHitIPU = pMURich->getY();
      xRichHitIPU = pMURich->getX();
      RingsumPattern[sector][xRichHitIPU][yRichHitIPU]=pMURich->getQuality();

    }
  iterRich->Reset();
  while ((pMURich = (HRichHitIPUSim *)iterRich->Next()) != 0 )
    {
      Int_t fulfill=0;
      sector = pMURich->getSector();
      xRichHitIPU = pMURich->getX();
      yRichHitIPU = pMURich->getY();

      if( (pMURich->getQuality()    > fRichIPUParThr->getThresholds(0,sector)) &&
          (pMURich->getVetoQuality()< -fRichIPUParThr->getThresholds(1,sector)))
        {

          Int_t CurrentSum = pMURich->getQuality();

          if  (fRichIPUParLmax->getLocMaxFlag()==1)
            {
              if ( (fRichIPUParLmax->getLocMax()==4)||(fRichIPUParLmax->getLocMax()==8) )
                {
                  if ( CurrentSum  >= RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU] &&
                      CurrentSum  >= RingsumPattern[sector][xRichHitIPU][yRichHitIPU+1] &&
                      CurrentSum  >  RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU] &&
                      CurrentSum  >  RingsumPattern[sector][xRichHitIPU][yRichHitIPU-1] )
                    {

                      if (fRichIPUParLmax->getLocMax()==8)
                        {
                          if ( CurrentSum  >= RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU+1] &&
                              CurrentSum  >= RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU+1] &&
                              CurrentSum  >  RingsumPattern[sector][xRichHitIPU-1][yRichHitIPU-1] &&
                              CurrentSum  >  RingsumPattern[sector][xRichHitIPU+1][yRichHitIPU-1] )
                            {
                              fulfill=1;
                            }
                        }
                      else 
                        {
                          fulfill=1;	    
                        }
                    }
                }
              else
                {
                  cout << " wrong parameter value!!!" << endl;
                }
            }
          else
            {
              fulfill=1;
            }

          if (fulfill==0)
            {
              pMURich->setX(9999);    // x==9999 used as flag for filtering out 
            }
        }
      else
        {
          pMURich->setX(9999);    // x==9999 used as flag for filtering out 
        }
    }
}

//----------------------------------------------------------------------------
Bool_t HRichIPURemakeSim::finalize()
{
  // Finalize the job
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Return allways kTRUE.
  return kTRUE;
}
//============================================================================

ClassImp(HRichIPURemakeSim)
