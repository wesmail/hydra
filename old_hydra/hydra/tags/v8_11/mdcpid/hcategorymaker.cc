//*-- AUTHOR : J.Markert

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
//HCategoryMaker
//
//  class for creating categories in an common way
//
/////////////////////////////////////////////////////////////
using namespace std;
#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include "hcategorymaker.h"
#include "hmdccal1.h"
#include "hmdchit.h"
#include "hmdcclusinf.h"
#include "hmdcseg.h"
#include "hmdcclusfit.h"
#include "hmdcwirefit.h"
#include "hmdctrkcand.h"
#include "hkicktrack.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "hmdcdef.h"
#include "hmdcpiddef.h"
#include "kickdef.h"
#include "hades.h"
#include "hevent.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hiterator.h"
#include "TClass.h"

ClassImp(HCategoryMaker)

HCategoryMaker::HCategoryMaker(const char* name, const char* title)
    :TNamed(name,title)
{
    // constructor
}

HCategoryMaker::~HCategoryMaker() {
  // destructor
}

HCategory* HCategoryMaker::buildLinearCategory(Text_t* className) {
    if (strcmp(className,"HMdcRawEventHeader")==0)
    {
	Int_t size=6*4*5*2; //sector*module*roc*10
	HLinearCategory* category = new HLinearCategory(className,size);
	if (!category) return NULL;
	if (strcmp(className,"HMdcRawEventHeader")==0)
	{
	    category->setPersistency(kFALSE);
	}
	return category;

    } else if(strcmp(className,"HMdcClusInf")==0) {

	HLinearCategory* category = new HLinearCategory(className,4000);
	return category;

    } else if(strcmp(className,"HMdcClusFit")==0) {

	HLinearCategory* category = new HLinearCategory(className,4000);
	return category;

    } else if(strcmp(className,"HMdcWireFit")==0) {

	HLinearCategory* category = new HLinearCategory(className,40000);
	return category;

    } else {
	cout<<"HCategoryMaker::buildLinearCategory() for "<<className<<" not implemented"<<endl;
	return 0;
    }
}


HCategory* HCategoryMaker::buildMatrixCategory(Text_t* className,Float_t fillRate) {
  TArrayI* ind=0;
  Int_t nSizes=0;
  Int_t* sizes=0;
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  if (!rtdb) return NULL;

  if(strcmp(className,"HKickTrack")==0)
  {
      nSizes=2;
      sizes=new Int_t[2];
      sizes[0]=6;
      sizes[1]=100;
  }
  else if (strcmp(className,"HMdcRaw")==0)
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
	     || strcmp(className,"HMdcWireInfo")==0
	     || strcmp(className,"HMdcCal1Sim")==0
	     || strcmp(className,"HMdcCal2Sim")==0)
      {
	  nSizes=ind->GetSize();
	  sizes=new Int_t[nSizes];
	  for (Int_t i=0;i<nSizes;i++) sizes[i]=ind->At(i)+1;
      }
      else if (strcmp(className, "HMdcTrkCand")==0)
      {
	  nSizes=2;
	  sizes=new Int_t[nSizes];
	  sizes[0] = ind->At(0) + 1;
	  sizes[1] = 600;
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
	      cout<<"HCategoryMaker::buildMatrixCategory() : Class "<<className<<" not supported"<<endl;
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

HCategory* HCategoryMaker::buildCategory(Cat_t cat) {
  // builds the default categories (here MatrixCategories)
  switch (cat) 
    {
    case catMdcRaw :            return buildMatrixCategory("HMdcRaw",0.5);
    case catMdcCal1 :           return buildMatrixCategory("HMdcCal1",0.5);
    case catMdcCal2 :           return buildMatrixCategory("HMdcCal2",0.5);
    case catMdcGeantCell :      return buildMatrixCategory("HMdcGeantCell",0.5);
    case catMdcHit :            return buildMatrixCategory("HMdcHit",0.5);
    case catMdcSeg :            return buildMatrixCategory("HMdcSeg",0.5);
    case catMdcTrkCand :        return buildMatrixCategory("HMdcTrkCand",0.5);
    case catMdcRawEventHeader : return buildLinearCategory("HMdcRawEventHeader");
    case catMdcDataWord :       return buildMatrixCategory("HMdcDataWord",0.1);
    case catMdcClusInf :        return buildLinearCategory("HMdcClusInf");
    case catMdcClusFit :        return buildLinearCategory("HMdcClusFit");
    case catMdcWireFit :        return buildLinearCategory("HMdcWireFit");
    case catMdcWireInfo :       return buildMatrixCategory("HMdcWireInfo",0.5);
    case catKickTrack:          return buildMatrixCategory("HKickTrack",0.5);
    default : return NULL;
    }
}
HCategory* HCategoryMaker::initCategory(HEvent* event,Cat_t cat, TString catname, TString detector)
{
  // inits the container cat 
  // looks first whether it already exists
  // otherwise it is created
  // catname is used for Error handling
  if(event==0)
  {
      cout<<"HCategoryMaker::initCategory() : ZERO pointer received for event!"<<endl;
      exit(1);
  }

  HCategory * category = 0;
  
  category = (HCategory*)(event->getCategory(cat));
  if (!category)
  {
      if(catname.CompareTo("HKickTrack")==0)
      {
          HCategory* kick = HCategoryMaker::HCategoryMaker::buildCategory(cat);
          if (kick)
          {
              event->addCategory(catKickTrack,kick,"Tracks");
              category=(HCategory*)kick;
          }
          else
          {
              cout<<"HCategoryMaker::initCategory() :  can't create or get category "<<catname.Data()<<endl;
              return NULL;
          }
      }
      else
      {
          category=(HCategory*)HCategoryMaker::buildCategory(cat);

          if (!category)
          {
              cout<<"HCategoryMaker::initCategory() : can't create or get category "<<catname.Data()<<endl;
              return NULL;
          }

          if (!(event->addCategory(cat,category,detector.Data())) )
          {
              cout<<"HCategoryMaker::initCategory() : can't add category "<< catname.Data()<<" to detector "<<detector.Data()<<endl;
              return NULL;
          }
      }
  }
  return category;
}
Int_t HCategoryMaker::copyMdcCal1Cat   (HCategory* targetCat,HCategory* cpCat)
{
    if(!targetCat||!cpCat) return 0;
    targetCat->Clear();
    HIterator* iter=(HIterator*)cpCat->MakeIterator();

    HMdcCal1* cal1cp;
    HMdcCal1* cal1;

    Int_t index,index2;
    iter->Reset();
    while((cal1cp=(HMdcCal1*)iter->Next())!=0)
    {
	HLocation& loc=iter->getLocation();
        cal1=(HMdcCal1*)targetCat->getSlot(loc,&index);
	index2=cpCat->getIndex(cal1cp);
	new (cal1) HMdcCal1(*cal1cp);
    }
    delete iter;

    return 1;
}
Int_t HCategoryMaker::copyMdcHitCat    (HCategory* targetCat,HCategory* cpCat)
{
    if(!targetCat||!cpCat) return 0;
    targetCat->Clear();
    HIterator* iter=(HIterator*)cpCat->MakeIterator();

    HMdcHit* hitcp;
    HMdcHit* hit;

    Int_t index,index2;
    iter->Reset();
    while((hitcp=(HMdcHit*)iter->Next())!=0)
    {
	HLocation& loc=iter->getLocation();
        hit=(HMdcHit*)targetCat->getSlot(loc,&index);
	index2=cpCat->getIndex(hitcp);
	new (hit) HMdcHit(*hitcp);
    }
    delete iter;
     return 1;
}
Int_t HCategoryMaker::copyMdcSegCat    (HCategory* targetCat,HCategory* cpCat)
{
    if(!targetCat||!cpCat) return 0;
    targetCat->Clear();
    HIterator* iter=(HIterator*)cpCat->MakeIterator();

    HMdcSeg* segcp;
    HMdcSeg* seg;

    Int_t index,index2;
    iter->Reset();
    while((segcp=(HMdcSeg*)iter->Next())!=0)
    {
	HLocation& loc=iter->getLocation();
        seg=(HMdcSeg*)targetCat->getSlot(loc,&index);
	index2=cpCat->getIndex(segcp);
	new (seg) HMdcSeg(*segcp);
    }
    delete iter;
    return 1;
}
Int_t HCategoryMaker::copyMdcClusInfCat(HCategory* targetCat,HCategory* cpCat)
{
    if(!targetCat||!cpCat) return 0;
    targetCat->Clear();
    HIterator* iter=(HIterator*)cpCat->MakeIterator();

    HMdcClusInf* clusinfcp;
    HMdcClusInf* clusinf;

    Int_t index,index2;
    iter->Reset();
    while((clusinfcp=(HMdcClusInf*)iter->Next())!=0)
    {
	HLocation& loc=iter->getLocation();
        clusinf=(HMdcClusInf*)targetCat->getSlot(loc,&index);
	index2=cpCat->getIndex(clusinfcp);
	new (clusinf) HMdcClusInf(*clusinfcp);
    }
    delete iter;
    return 1;
}
Int_t HCategoryMaker::copyMdcClusFitCat(HCategory* targetCat,HCategory* cpCat)
{
    if(!targetCat||!cpCat) return 0;
    targetCat->Clear();
    HIterator* iter=(HIterator*)cpCat->MakeIterator();

    HMdcClusFit* clusfitcp;
    HMdcClusFit* clusfit;

    Int_t index,index2;
    iter->Reset();
    while((clusfitcp=(HMdcClusFit*)iter->Next())!=0)
    {
	HLocation& loc=iter->getLocation();
        clusfit=(HMdcClusFit*)targetCat->getSlot(loc,&index);
	index2=cpCat->getIndex(clusfitcp);
	new (clusfit) HMdcClusFit(*clusfitcp);
    }
    delete iter;
    return 1;
}
Int_t HCategoryMaker::copyMdcWireFitCat(HCategory* targetCat,HCategory* cpCat)
{
    if(!targetCat||!cpCat) return 0;
    targetCat->Clear();
    HIterator* iter=(HIterator*)cpCat->MakeIterator();

    HMdcWireFit* wirefitcp;
    HMdcWireFit* wirefit;

    Int_t index,index2;
    iter->Reset();
    while((wirefitcp=(HMdcWireFit*)iter->Next())!=0)
    {
	HLocation& loc=iter->getLocation();
        wirefit=(HMdcWireFit*)targetCat->getSlot(loc,&index);
	index2=cpCat->getIndex(wirefitcp);
	new (wirefit) HMdcWireFit(*wirefitcp);
    }
    delete iter;
    return 1;
}
Int_t HCategoryMaker::copyMdcTrkCandCat(HCategory* targetCat,HCategory* cpCat)
{
    if(!targetCat||!cpCat) return 0;
    targetCat->Clear();
    HIterator* iter=(HIterator*)cpCat->MakeIterator();

    HMdcTrkCand* trkcandcp;
    HMdcTrkCand* trkcand;

    Int_t index,index2;
    iter->Reset();
    while((trkcandcp=(HMdcTrkCand*)iter->Next())!=0)
    {
	HLocation& loc=iter->getLocation();
        trkcand=(HMdcTrkCand*)targetCat->getSlot(loc,&index);
	index2=cpCat->getIndex(trkcandcp);
	new (trkcand) HMdcTrkCand(*trkcandcp);
    }
    delete iter;
    return 1;
}
Int_t HCategoryMaker::copyKickTrackCat(HCategory* targetCat,HCategory* cpCat)
{
    if(!targetCat||!cpCat) return 0;
    targetCat->Clear();
    HIterator* iter=(HIterator*)cpCat->MakeIterator();

    HKickTrack* trkcp;
    HKickTrack* trk;

    Int_t index,index2;
    iter->Reset();
    while((trkcp=(HKickTrack*)iter->Next())!=0)
    {
	HLocation& loc=iter->getLocation();
	trk=(HKickTrack*)targetCat->getSlot(loc,&index);
	index2=cpCat->getIndex(trkcp);
	//new (trk) HKickTrack(*trkcp,1);
        new (trk) HKickTrack(*trkcp);
    }
    delete iter;
    return 1;
}




