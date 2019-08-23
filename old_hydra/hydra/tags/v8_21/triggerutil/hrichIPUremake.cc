//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/18/2005

#include "hrichIPUremake.h"
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
#include "hmessagemgr.h"
#include "hlocmaxfilter.h"
#include "hrichhitIPU.h"

Bool_t HRichIPURemake::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb)
{
  // Adds to the runtime database "rtdb" the containers needed by the MU Emulation
  //spec is used to get information of the spectrometer setup.
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

          ERROR_msg(HMessageMgr::DET_TRIGGER,"RichIPU Parameters LocMax Initialization failed!");
          return kFALSE;
        }
  return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichIPURemake::HRichIPURemake(void)
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
HRichIPURemake::HRichIPURemake(const Text_t *name,const Text_t *title)
              : HReconstructor(name, title)
{
  // Default constructor
  //
  // Input parameters:
  //   Text_t *name
  //      Name of the object
  //   Text_t *title
  //      Title of the object
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
HRichIPURemake::~HRichIPURemake(void)
{
  // Destructor
  // Delete the iterators
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  if (iterRich)
    {
      delete iterRich;
      iterRich=NULL;
    }
}

//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichIPURemake::init(void)
{
  SEPERATOR_msg("-",60);
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"INIT of HRichIPURemake");
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HTriggerDetector * pTrigDet = (HTriggerDetector*)gHades->getSetup()->getDetector("Trigger");

  // initialize parameters
  if (pTrigDet)
    if(!initParContainer(spec,rtdb))
      {
        return kFALSE;
      }

  // allocate input categories
  if((pRich=gHades->getCurrentEvent()->getCategory(catHardRichHit))==NULL) return kFALSE;
  iterRich = (HIterator*)pRich->MakeIterator("native");

  SEPERATOR_msg("-",60);
  return kTRUE;
}

//============================================================================


//----------------------------------------------------------------------------
Int_t HRichIPURemake::execute(void)
{
  // Execute the job.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Allways 0.
  prepareInput();
  HLocMaxFilter locmaxfilter;
  pRich->filter(locmaxfilter);
  return 0;
}
//============================================================================


//----------------------------------------------------------------------------
void HRichIPURemake::prepareInput(void)
{
  // Get all needed data from the category into the internal datastructures.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  HRichHitIPU   *pMURich=NULL;
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
  while ((pMURich = (HRichHitIPU *)iterRich->Next()) != 0)
    {
      sector = pMURich->getSector();

      yRichHitIPU = pMURich->getY();
      xRichHitIPU = pMURich->getX();
      RingsumPattern[sector][xRichHitIPU][yRichHitIPU]=pMURich->getQuality();
    }

  iterRich->Reset();
  while ((pMURich = (HRichHitIPU *)iterRich->Next()) != 0 )
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
                  INFO_msg(10,HMessageMgr::DET_TRIGGER,"wrong parameter value!!!");
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
Bool_t HRichIPURemake::finalize(void)
{
  // Finalize the job.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns allways kTRUE;
  return kTRUE;
}
//============================================================================

ClassImp(HRichIPURemake)
