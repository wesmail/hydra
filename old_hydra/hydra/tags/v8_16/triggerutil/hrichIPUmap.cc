//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/19/2005

#include "hrichIPUmap.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "triggerinfodef.h"
#include "hruntimedb.h"
#include "hrichgeometrypar.h"
#include "hrichhitIPU.h"
#include "hmessagemgr.h"

//----------------------------------------------------------------------------
HRichIPUMap::HRichIPUMap(void)
{
  // Default constructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  //Return code:
  //   none
  pRich= NULL;
  iterRich=NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichIPUMap::HRichIPUMap(Text_t *name,Text_t *title)
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
  //Return code:
  //   none
  pRich= NULL;
  iterRich=NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichIPUMap::~HRichIPUMap(void)
{
  // Destructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  //Return code:
  //   none
  if (pRich)
    {
      delete pRich;
      pRich= NULL;
    }

  if (iterRich)
    {
      delete iterRich;
      iterRich=NULL;
    }
}

//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichIPUMap::init(void)
{
  // Initalisation of the parameters
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  //Return code:
  //   none
  SEPERATOR_msg("-",60);
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"INIT of HRichIPUMap()");
  HRuntimeDb *rtdb = gHades->getRuntimeDb();

  // allocate input categories
  pRich=gHades->getCurrentEvent()->getCategory(catHardRichHit);
  if (!pRich)
    {
      return kFALSE;
    }
  iterRich = (HIterator*)pRich->MakeIterator("native");

  if(rtdb)
    {
      HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
      setGeomPar(pGeomPar);
      if (pGeomPar == NULL)
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"RICH geometry parameters not found!");
          return kFALSE;
        }
    }
  SEPERATOR_msg("-",60);
  return kTRUE;
}

//============================================================================


//----------------------------------------------------------------------------
Int_t HRichIPUMap::execute(void)
{
  // Execute the job.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  //Return code:
  //   Returns allways 0.
  HRichHitIPU   *pMURich=NULL;
  Int_t yRichHitIPU, xRichHitIPU, nSec;

  iterRich->Reset();
  while ((pMURich = (HRichHitIPU *)iterRich->Next()) != 0)
    {
      //cout << "BEFORE X " << pMURich->getX() << "  Y " << pMURich->getY() << endl;
      nSec = pMURich->getSector();
      yRichHitIPU = pMURich->getY();
      xRichHitIPU = pMURich->getX();

      pMURich->setX(yRichHitIPU);
      pMURich->setY(xRichHitIPU);
      //cout << "AFTER  X " << pMURich->getX() << "  Y " << pMURich->getY() << endl; 

      // mapping according to RICH 
      HRichPad * pad = ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->getPad(pMURich->getX(),pMURich->getY());

      pMURich->setPhi(pad->getPhi(nSec));
      pMURich->setTheta(pad->getTheta());
      //cout << "theta " << pMURich->getTheta() << "  phi " << pMURich->getPhi() << endl; 
    }
  return 0;
}
//============================================================================


//----------------------------------------------------------------------------
Bool_t HRichIPUMap::finalize(void)
{
  // Finalize the job
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  //Return code:
  //   Returns allways kTRUE;
  return kTRUE;
}
//============================================================================

ClassImp(HRichIPUMap)
