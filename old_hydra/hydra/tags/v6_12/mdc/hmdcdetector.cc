//*-- AUTHOR : Ilse Koenig
//*-- Modified : 05/09/2002 by P. Zumbruch
//*-- Modified : 11/12/2001 by I. Koenig
//*-- Modified : 21/02/2000 by R. Holzmann
//*-- Modified : 27/05/99   by I. Koenig

/////////////////////////////////////////////////////////////
//HMdcDetector
//
//  class for detector Mdc
//  stores the actual setup in a linear array of 24 modules;
//
/////////////////////////////////////////////////////////////

#include <stdio.h>
#include "hmdcdetector.h"
#include "hmdcdef.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hdetpario.h"
#include "hmdcparrootfileio.h"
#include "hparasciifileio.h"
#include "hmdcparasciifileio.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hmessagemgr.h"
#include "TClass.h"

ClassImp(HMdcDetector)

HMdcDetector::HMdcDetector() {
  // constructor
  fName="Mdc";
  maxModules=4;
  maxComponents=6;
  modules= new TArrayI(24);
}

HMdcDetector::~HMdcDetector() {
  // destructor
  delete modules;
  modules=0;
}

Bool_t HMdcDetector::init(void) {
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  HParSet* pg=rtdb->getContainer("MdcGeomStruct");
  if (!pg->init()) return kFALSE;
  return kTRUE;
}

void HMdcDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Mdc
  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
    io->setDetParIo("HMdcParIo");
    return;
  }
  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HMdcParRootFileIo* p=new HMdcParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
    io->setDetParIo(p);
  }
  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HMdcParAsciiFileIo* p=new HMdcParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }
}


Bool_t HMdcDetector::write(HParIo* output) {
  // writes the Mdc setup to output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return kFALSE;
}


HCategory* HMdcDetector::buildLinearCategory(Text_t* className) {
  if (strcmp(className,"HMdcRawEventHeader")==0) {
    Int_t size=6*4*5*2; //sector*module*roc*10
    HLinearCategory* category = new HLinearCategory(className,size);
    if (!category) return NULL;
    if (strcmp(className,"HMdcRawEventHeader")==0)
      {
	category->setPersistency(kFALSE);
      }
    return category;
  } else {
      gHades->getMsg()->warning(10,DET_MDC,GetName()
				,"buildLinearCategory for %s not implemented"
				,className);
    return 0;

  }
}


HCategory* HMdcDetector::buildMatrixCategory(Text_t* className,Float_t fillRate) {
  TArrayI* ind=0;
  Int_t nSizes;
  Int_t* sizes=0;
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  if (!rtdb) return NULL;

  if (strcmp(className,"HMdcRaw")==0) 
    {
      ind=new TArrayI(4);
      HMdcRawStruct* p;
      if (!(p = (HMdcRawStruct*) rtdb->getContainer("MdcRawStruct"))) return NULL;
      p->getMaxIndices(ind);
      nSizes=ind->GetSize();
      sizes=new Int_t[nSizes];
      for (Int_t i=0;i<nSizes;i++) sizes[i]=ind->At(i)+1;
    } 
  else if (strcmp(className,"HMdcDataWord")==0) 
    {
      ind=new TArrayI(4);
      HMdcRawStruct* p;
      if (!(p = (HMdcRawStruct*) rtdb->getContainer("MdcRawStruct"))) return NULL;
      p->getMaxIndices(ind);
      nSizes=ind->GetSize();
      sizes=new Int_t[nSizes+1];
      for (Int_t i=0;i<nSizes;i++) sizes[i]=ind->At(i)+1;

      sizes[nSizes]=8;
      nSizes++;
    } 
  else 
    {
      ind=new TArrayI(4);
      HMdcGeomStruct* p;
      if (!(p = (HMdcGeomStruct*) rtdb->getContainer("MdcGeomStruct"))) return NULL;
      p->getMaxIndices(ind);
      
      if (   strcmp(className,"HMdcCal1")==0
          || strcmp(className,"HMdcCal2")==0
          || strcmp(className,"HMdcGeantCell")==0
          || strcmp(className,"HMdcCal1Sim")==0
          || strcmp(className,"HMdcCal2Sim")==0) 
      {
	nSizes=ind->GetSize();
	sizes=new Int_t[nSizes];
	for (Int_t i=0;i<nSizes;i++) sizes[i]=ind->At(i)+1;
      } 
      else  
	{
	  nSizes=ind->GetSize()-1;
	  sizes=new Int_t[nSizes];
	  sizes[nSizes-1] = 600;
	  
	  if (strcmp(className, "HMdcSeg")==0 || strcmp(className, "HMdcSegSim")==0) 
	    {
	      sizes[0] = ind->At(0) + 1;
	      sizes[1] = (ind->At(1) / 2) + 1;
	    } 
	  else if (strcmp(className, "HMdcHit")==0 || strcmp(className, "HMdcHitSim")==0) 
	    {
	      for (Int_t i=0;i<nSizes-1;i++) sizes[i]=ind->At(i)+1;
	    } 
	  else 
	    {
	      Error("buildMatrixCategory","Class %s not supported",className);
	      return NULL;
	    }
	}
    }

  HMatrixCategory* category = new HMatrixCategory(className,nSizes,sizes,fillRate);
 
  if (ind) delete ind;
  delete [] sizes;

  if (strcmp(className,"HMdcDataWord")==0)
    {
      category->setPersistency(kFALSE);
    }

  return category;
}

HCategory* HMdcDetector::buildCategory(Cat_t cat) {
  // builds the default categories (here MatrixCategories)
  switch (cat) 
    {
    case catMdcRaw :            return buildMatrixCategory("HMdcRaw",0.5);
    case catMdcCal1 :           return buildMatrixCategory("HMdcCal1",0.5);
    case catMdcCal2 :           return buildMatrixCategory("HMdcCal2",0.5);
    case catMdcGeantCell :      return buildMatrixCategory("HMdcGeantCell",0.5);
    case catMdcHit :            return buildMatrixCategory("HMdcHit",0.5);
    case catMdcSeg :            return buildMatrixCategory("HMdcSeg",0.5); 
    case catMdcRawEventHeader : return buildLinearCategory("HMdcRawEventHeader");
    case catMdcDataWord :       return buildMatrixCategory("HMdcDataWord",0.1);
    default : return NULL;
    }
}
