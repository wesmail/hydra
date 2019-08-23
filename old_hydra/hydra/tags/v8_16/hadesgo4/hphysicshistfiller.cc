//*-- AUTHOR : J. Markert

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
// HPhysicsHistFiller
//
// Container class for the physics hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "hphysicshistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"

#include "hmdctrackddef.h"
#include "hmdctrackgdef.h"
#include "hmdcdef.h"
#include "haddef.h"

#include "hmdctrkcand.h"
#include "hsplinetrack.h"
#include "hmdcseg.h"
#include "htofhit.h"
#include "hshowerhittof.h"
#include "hrichhit.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>
#include <TMath.h>

// Color definitions
TArrayI HPhysicsHistFiller::colorsSector;

// Hist Pointers
HHistArray*  HPhysicsHistFiller::hphimdctof    =0;
HHistArray*  HPhysicsHistFiller::hphimdctofino =0;
HHistArray*  HPhysicsHistFiller::hphimdcrich   =0;
HHistArray2* HPhysicsHistFiller::hdedxmdc      =0;
HHistArray2* HPhysicsHistFiller::hdedxtof      =0;
HHistArray2* HPhysicsHistFiller::hdedxtofino   =0;


// Hist Addon Pointers
HMonHistAddon* HPhysicsHistFiller::hphimdctofA    =0;
HMonHistAddon* HPhysicsHistFiller::hphimdctofinoA =0;
HMonHistAddon* HPhysicsHistFiller::hphimdcrichA   =0;
HMonHistAddon* HPhysicsHistFiller::hdedxmdcA      =0;
HMonHistAddon* HPhysicsHistFiller::hdedxtofA      =0;
HMonHistAddon* HPhysicsHistFiller::hdedxtofinoA   =0;

ClassImp(HPhysicsHistFiller)

HPhysicsHistFiller::HPhysicsHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsSector.Set(6);
}
HPhysicsHistFiller::~HPhysicsHistFiller()
{
}
void   HPhysicsHistFiller::putPhysicsParams(HParamList* l,TList* histpool)
{
    // Puts all params of HPhysicsHistFiller to the parameter list of
    // HParamList (which ist used by the io);

       Text_t mytemp[500];
       l->add("colorsSector"           ,colorsSector);
       hphimdctofA->getText(mytemp);
       l->add("hphimdctof"             ,mytemp,'s',500);
       hphimdctofinoA->getText(mytemp);
       l->add("hphimdctofino"          ,mytemp,'s',500);
       hphimdcrichA->getText(mytemp);
       l->add("hphimdcrich"            ,mytemp,'s',500);
       hdedxmdcA->getText(mytemp);
       l->add("hdedxmdc"               ,mytemp,'s',500);
       hdedxtofA->getText(mytemp);
       l->add("hdedxtof"               ,mytemp,'s',500);
       hdedxtofinoA->getText(mytemp);
       l->add("hdedxtofino"            ,mytemp,'s',500);

}
Bool_t HPhysicsHistFiller::getPhysicsParams(HParamList* l,TList* histpool,TList* activeHists)
{
    if(!(l->fill("colorsSector",&colorsSector))) return kFALSE;
    Text_t mytemp[500];
    if(!(l->fill("hphimdctof"   ,mytemp,500)))   return kFALSE;
    else
    {
	histpool->Add(hphimdctofA=new HMonHistAddon(mytemp));
	if(hphimdctofA->getActive()==1)
	{
	    activeHists->Add(hphimdctof=new HHistArray(*hphimdctofA));
	    for(Int_t i=0;i<6;i++)
	    {
		hphimdctof->getP(0,i)->SetLineColor(colorsSector[i]);
	    }
	}
    }
    if(!(l->fill("hphimdctofino",mytemp,500)))   return kFALSE;
    else
    {
	histpool->Add(hphimdctofinoA=new HMonHistAddon(mytemp));
	if(hphimdctofinoA->getActive()==1)
	{
	    activeHists->Add(hphimdctofino=new HHistArray(*hphimdctofinoA));
	    for(Int_t i=0;i<6;i++)
	    {
		hphimdctofino->getP(0,i)->SetLineColor(colorsSector[i]);
	    }
	}
    }
    if(!(l->fill("hphimdcrich"  ,mytemp,500)))   return kFALSE;
    else
    {
	histpool->Add(hphimdcrichA=new HMonHistAddon(mytemp));
	if(hphimdcrichA->getActive()==1)
	{
	    activeHists->Add(hphimdcrich=new HHistArray(*hphimdcrichA));
	    for(Int_t i=0;i<6;i++)
	    {
		hphimdcrich->getP(0,i)->SetLineColor(colorsSector[i]);
	    }
	}
    }
    if(!(l->fill("hdedxmdc"     ,mytemp,500)))   return kFALSE;
    else
    {
	histpool->Add(hdedxmdcA=new HMonHistAddon(mytemp));
	if(hdedxmdcA->getActive()==1)
	{
	    activeHists->Add(hdedxmdc=new HHistArray2(*hdedxmdcA));
	}
    }
    if(!(l->fill("hdedxtof"     ,mytemp,500)))   return kFALSE;
    else
    {
	histpool->Add(hdedxtofA=new HMonHistAddon(mytemp));
	if(hdedxtofA->getActive()==1)
	{
	    activeHists->Add(hdedxtof=new HHistArray2(*hdedxtofA));
	}
    }
    if(!(l->fill("hdedxtofino"  ,mytemp,500)))   return kFALSE;
    else
    {
	histpool->Add(hdedxtofinoA=new HMonHistAddon(mytemp));
	if(hdedxtofinoA->getActive()==1)
	{
	    activeHists->Add(hdedxtofino=new HHistArray2(*hdedxtofinoA));
	}
    }

    return kTRUE;
}
void HPhysicsHistFiller::fillMdcDeDx(HSplineTrack* spline,HMdcTrkCand* trk)
{
    Int_t s,charge;
    Float_t p,dedx1,dedx2;
    p     =spline->getP();
    charge=spline->getPolarity();
    s     =trk   ->getSec();
    dedx1 =trk   ->getdedxInnerSeg();
    dedx2 =trk   ->getdedxOuterSeg();

    if(hdedxmdc)
    {
	hdedxmdc->getP(s,0)->Fill((Axis_t)p*charge,(Axis_t)dedx1);
 	hdedxmdc->getP(s,1)->Fill((Axis_t)p*charge,(Axis_t)dedx2);
    }
}
void HPhysicsHistFiller::fillTofDeDx(HSplineTrack* spline,HTofHit* tofhit)
{
    Int_t s,charge;
    Float_t p,dedx;
    p     =spline->getP();
    charge=spline->getPolarity();
    s     =spline->getSector();
    dedx  =tofhit->getEdep();

    if(hdedxtof)
    {
	hdedxtof->getP(0,s)->Fill((Axis_t)p*charge,(Axis_t)dedx);
    }

}
void HPhysicsHistFiller::fillTofinoDeDx(HSplineTrack* spline,HShowerHitTof* tofhit)
{
    Int_t s,charge;
    Float_t p,dedx;
    p     =spline->getP();
    charge=spline->getPolarity();
    s     =spline->getSector();
    dedx  =tofhit->getADC();

    if(hdedxtofino)
    {
	hdedxtofino->getP(0,s)->Fill((Axis_t)p*charge,(Axis_t)dedx);
    }

}
void HPhysicsHistFiller::fillMdcTofPhi(HMdcSeg* seg,HTofHit* tofhit)
{
    Int_t sseg,stof,io;
    io   =seg->getIOSeg();
    sseg =seg   ->getSec();
    stof =tofhit->getSector();

    if(io==0&&sseg==stof)
    {
	Float_t phiseg;
	Float_t phitof;
	(sseg<5) ? phiseg=(seg->getPhi()*TMath::RadToDeg())+sseg*60.: phiseg =(seg->getPhi()*TMath::RadToDeg())-60.;
	tofhit->getPhi(phitof);
	if(hphimdctof)
	{
	    hphimdctof->getP(0,sseg)->Fill((Axis_t)(phiseg-phitof));
	}

    }

}
void HPhysicsHistFiller::fillMdcTofinoPhi(HMdcSeg* seg,HShowerHitTof* tofhit)
{
    Int_t sseg,stof,io;
    io   =seg->getIOSeg();
    sseg =seg   ->getSec();
    stof =tofhit->getSector();

    if(io==0&&sseg==stof)
    {
	Float_t phitof,thetatof,rtof,phiseg;
	tofhit->getSphereCoord(&rtof,&phitof,&thetatof);

	(sseg<5) ? phiseg=(seg->getPhi()*TMath::RadToDeg())+sseg*60.: phiseg =(seg->getPhi()*TMath::RadToDeg())-60.;
	if(hphimdctofino)
	{
	    hphimdctofino->getP(0,sseg)->Fill((Axis_t)(phiseg-phitof));
	}

    }

}
void HPhysicsHistFiller::fillMdcRichPhi(HMdcSeg* seg,HRichHit* richhit)
{
    Int_t sseg,srich,io;
    io   =seg->getIOSeg();
    sseg =seg   ->getSec();
    srich =richhit->getSector();

    if(io==0&&sseg==srich)
    {
	Float_t phirich,phiseg;
	phirich=richhit->getPhi();

	(sseg<5) ? phiseg=(seg->getPhi()*TMath::RadToDeg())+sseg*60.: phiseg =(seg->getPhi()*TMath::RadToDeg())-60.;
	if(hphimdcrich)
	{
	    hphimdcrich->getP(0,sseg)->Fill((Axis_t)(phiseg-phirich));
	}

    }
}

