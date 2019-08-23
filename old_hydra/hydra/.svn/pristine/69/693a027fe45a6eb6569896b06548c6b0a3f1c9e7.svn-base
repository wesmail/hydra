//*-- Author : RICH team member
// ***************************************************************************
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
//  HRichDetector
//
//  
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

#pragma implementation

#include "TClass.h"

#include "hades.h"
#include "hcategory.h"
#include "hdetpario.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hparasciifileio.h"
#include "hparhadasciifileio.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hrichanalysis.h"
#include "hrichdetector.h"
#include "hrichparasciifileio.h"
#include "hrichparhadasciifileio.h"
#include "hrichparrootfileio.h"
#include "hrichparset.h"
#include "htaskset.h"
#include "richdef.h"

#include <iomanip>
#include <iostream> 

using namespace std;

ClassImp(HRichDetector)

HRichDetector::HRichDetector(void) : HDetector("Rich", "The RICH detector")
{

   fGeometry  = NULL;
   maxModules = 1;
   maxColumns = 96;
   maxRows    = 96;
   modules    = new TArrayI(getMaxSectors());

}

HRichDetector::~HRichDetector(void)
{
}


HTask*
HRichDetector::buildTask(const Text_t* name,const Option_t* opt)
{
   if ( NULL != name && 0 == strcmp(name, "default") )
      name = NULL;

   HTaskSet* pDetTasks = new HTaskSet("Rich", "List of Rich tasks");

   if ( NULL == name )
   {  //default list of tasks
      pDetTasks->add(new HRichAnalysis("rich.ana", "Ring finder", kFALSE));
      return pDetTasks;
   }

   return HDetector::buildTask(name, opt);
}


HCategory*
HRichDetector::buildLinearCat(const Text_t* classname, Int_t size)
{
   HLinearCategory* category = NULL;

   category = new HLinearCategory(classname, size);
   return category;
}


HCategory*
HRichDetector::buildMatrixCat(const Text_t* classname,
                              Float_t fillRate)
{
   Int_t            maxSector = 0;
   Int_t            i;
   Int_t            sizes[3];
   HMatrixCategory* category = NULL;

   for (i = 0; i < 6; i++)
      if (getModule(i, 0) > 0)
         maxSector++; 

   sizes[0] = maxSector; 
   sizes[1] = sizes[2] = 96;

   category = new HMatrixCategory(classname, 3, sizes, fillRate);
   return category;
}


HCategory*
HRichDetector::buildSimMatrixCat(const Text_t* classname,
                                 Float_t fillRate,
                                 Int_t   size       )
{
 
   HMatrixCategory *category = NULL;
   Int_t ini[2];

   ini[0] = 6;
   ini[1] = size;

   category = new HMatrixCategory(classname, 2, ini, fillRate);
   return category;
}


Int_t
HRichDetector::getSectors()
{
   Int_t sect;
   getMaxValues(&sect);

   return sect;
}


void
HRichDetector::getMaxValues(Int_t* pMaxSector)
{
   Int_t i;
   Int_t maxSector = 0;

   for ( i = 0; i < 6; ++i)
      if (getModule(i, 0) > 0)
         maxSector = i + 1;
   *pMaxSector = maxSector; 
}

HCategory*
HRichDetector::buildCategory(Cat_t cat)
{
   switch (cat) 
   {
      case catRichRaw             : return buildMatrixCat("HRichRaw",1);
      case catRichCal             : return buildMatrixCat("HRichCal",1);
      case catRichCalTemp         : return buildMatrixCat("HRichCalMax",1);
      case catRichHit             : return buildLinearCat("HRichHit");
      case catRichHitSim          : return buildLinearCat("HRichHitSim");
      case catRichPID             : return buildLinearCat("HRichPID");
      case catRichTrack           : return buildLinearCat("HRichTrack");
      case catRichPhotonHit       : return buildLinearCat("HRichPhotonHit");//et
      case catRichPhotonHitHeader : return buildLinearCat("HRichPhotonHitHeader");//et
      case catRichLocalMaxHeader  : return buildLinearCat("HRichLocalMaxHeader");
      case catRichHitFit          : return buildLinearCat("HRichHitFit");//et
      case catRichDirClus         : return buildMatrixCat("HRichDirClus",0.5);
      case catRichLocal           : return buildMatrixCat("HRichLocal",1);//
      case catRichPhotClus        : return buildLinearCat("HPhotonCluster");
      case catRichHitHdr          : return buildLinearCat("HRichHitHeader");
      case catRichGeantRaw        : return buildSimMatrixCat("HGeantRichPhoton",1,200);
      case catRichGeantRaw+1      : return buildSimMatrixCat("HGeantRichDirect",1,200);
      case catRichGeantRaw+2      : return buildSimMatrixCat("HGeantRichMirror",1,50);
      case catMatchHit            : return buildLinearCat("HHitMatch");//et
      case catDiMatchHit          : return buildLinearCat("HDiHitMatch");//et
      case catMatchHitHeader      : return buildLinearCat("HHitMatchHeader");//et
//////////////////////////////////////////////////////////////////////////
//       case catHardRichHit         : return buildLinearCat("HRichHitIPU");//et
//       case catRichGeantRaw        : return buildLinearCat("HRichGeantCherHit");
//       case catRichGeantRaw+1      : return buildLinearCat("HRichGeantDirHit");
//       case catRichGeantWire       : return buildLinearCat("HRichGeantWire");
//////////////////////////////////////////////////////////////////////////
      default: return NULL;
   }
}


void
HRichDetector::activateParIo(HParIo* io)
{
// Initialization according to Ilse's scheme
// activates the input/output class for the parameters
// needed by the Rich


   if ( 0 == strncmp(io->IsA()->GetName(),"HOraIo", strlen("HOraIo")) )
   {
      io->setDetParIo("HRichParIo");
      return;
   }

   if ( 0 == strncmp(io->IsA()->GetName(), "HParRootFileIo", strlen("HParRootFileIo")) )
   {
      HRichParRootFileIo* p = NULL;
      p = new HRichParRootFileIo( static_cast<HParRootFileIo*>(io)->getParRootFile() );
      io->setDetParIo(p);
   }

   if ( 0 == strncmp(io->IsA()->GetName(), "HParAsciiFileIo", strlen("HParAsciiFileIo")) )
   {
      HRichParAsciiFileIo* p = NULL;
      p = new HRichParAsciiFileIo( static_cast<HParAsciiFileIo*>(io)->getFile() );
      io->setDetParIo(p);
   }

   if ( 0 == strncmp(io->IsA()->GetName(), "HParHadAsciiFileIo", strlen("HParHadAsciiFileIo")) )
   {
      HRichParHadAsciiFileIo* p = NULL;
      p = new HRichParHadAsciiFileIo( static_cast<HParHadAsciiFileIo*>(io)->getHadAscii() );
      io->setDetParIo(p);
   }

}


Bool_t
HRichDetector::write(HParIo* output)
{
// Writes the Rich setup to output

   HDetParIo* out = NULL;
   out  = output->getDetParIo("HRichParIo");
   if ( NULL != out )
      return out->write(this);
   return kFALSE;
}
