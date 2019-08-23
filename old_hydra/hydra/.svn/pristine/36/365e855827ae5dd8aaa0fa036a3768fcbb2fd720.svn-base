//*-- AUTHOR : Ilse Koenig
//*-- Created : 09/05/2005
//*-- Changed : 19.7.2005 B. Spruck

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//
//  HHodoDetector
//
//  Detector class for the Pion Hodoscopes
//
/////////////////////////////////////////////////////////////

using namespace std;
#include "hhododetector.h"
#include "hododef.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hades.h"
#include "hevent.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hparasciifileio.h"
#include "hdetpario.h"
#include "hhodoparrootfileio.h"
#include "hhodoparasciifileio.h"
#include "TClass.h"
#include "TArrayI.h"
#include <iostream> 
#include <iomanip>

ClassImp(HHodoDetector)

HHodoDetector::HHodoDetector(void) : HDetector("Hodo","The Pion Hodoscopes") {
  // constructor
  // sets the name, the number of modules and the maximum number of fibers per module
  fName="Hodo";
  maxSectors=-1;
  maxModules=10;
  maxComponents=128;
  modules=new TArrayI(maxModules);
}

HHodoDetector::~HHodoDetector(void) {
  // destructor
  delete modules;
  modules=0;
}

HCategory* HHodoDetector::buildLinearCategory(Text_t *classname,Float_t fillRate) {
  // builds a linear category for the Hodoscopes
  HLinearCategory* category=new HLinearCategory(classname,(Int_t)(maxModules*maxComponents*fillRate));
  return category;
}

HCategory* HHodoDetector::buildMatrixCategory(Text_t *className,Float_t fillRate) {
  // builds a matrix category for the Hodoscopes (2 levels)
  Int_t maxMod=getMaxModInSetup();
  if (maxMod==0) return 0;
  Int_t* sizes=new Int_t[2];
  sizes[0]=maxMod;
  sizes[1]=maxComponents;
  HMatrixCategory* category = new HMatrixCategory(className,2,sizes,fillRate);
  delete [] sizes;
  return category;
}

HCategory* HHodoDetector::buildCategory(Cat_t cat)
{// get  the category if existing
 // built and add if not existing
 // return pointer or zero
   HCategory *pcat;
   pcat=gHades->getCurrentEvent()->getCategory(cat);
   if( pcat) return(pcat);// already existing

   // builds the categories for the Hodoscopes
   switch (cat) {
      case catHodoRaw:
         pcat=buildMatrixCategory("HHodoRaw",0.5);
         break;
      case catHodoCal:
         pcat=buildMatrixCategory("HHodoCal",0.5);
         break;
      case catHodoHit:
         pcat=buildLinearCategory("HHodoHit",0.1);
         break;
      default:
         return NULL;
   }
   if(pcat) gHades->getCurrentEvent()->addCategory(cat,pcat,"Hodo");
   return(pcat);
}

void HHodoDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters needed by the Hodoscopes
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HHodoParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HHodoParRootFileIo* p=new HHodoParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HHodoParAsciiFileIo* p=new HHodoParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }
}

Bool_t HHodoDetector::write(HParIo* output) {
  // writes the setup of the Hodoscopes to the output
  HDetParIo* out=output->getDetParIo("HHodoParIo");
  if (out) return out->write(this);
  return kFALSE;
}

Int_t HHodoDetector::getMaxModInSetup(void) {
  // Why so complicated?
  // return(maxModules);
  // might be much easier...
  Int_t maxMod=-1;
  for(Int_t i=0;i<maxModules;i++) {
    // if (modules->At(i)) maxMod=(i>maxMod)?i:maxMod;// i is always >maxMod
    if (modules->At(i)) maxMod=i;
  }
  maxMod++;
  return maxMod;
}
