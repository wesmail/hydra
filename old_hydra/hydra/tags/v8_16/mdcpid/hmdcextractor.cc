//*-- AUTHOR : J.Markert

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HMdcExtractor
//
// Extracts Mdc data
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdcextractor.h"
#include "hmdcextractors.h"
#include "hcategorymaker.h"
#include "hmdcfunc1.h"
#include "hmdcdef.h"
#include "hmdcpiddef.h"
#include "kickdef.h"
#include "piddef.h"
#include "hpidparticle.h"
#include "hpidcandidate.h"
#include "hpidtrackcand.h"
#include "hkicktrack.h"
#include "hmdctrackddef.h"
#include "hdebug.h"
#include "hades.h"
#include "htree.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hmdcdetector.h"
#include "hevent.h"
#include "hrecevent.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlocation.h"
#include "hmdccal1.h"
#include "hmdccal1sim.h"
#include "hmdchit.h"
#include "hmdchitsim.h"
#include "hmdcseg.h"
#include "hmdcsegsim.h"
#include "hmdctrkcand.h"
#include "hkicktrack.h"
#include "hmdcclusinf.h"
#include "hmdcclusfit.h"
#include "hmdcwirefit.h"
#include <iostream> 
#include <iomanip>
#include <math.h>
#include <stdlib.h>
#include "TFile.h"

ClassImp(HMdcExtractor)

HMdcExtractor::HMdcExtractor(void) {
  // default constructor
  initVariables();
}
HMdcExtractor::HMdcExtractor(Text_t* name,Text_t* title)
                 : HReconstructor(name,title)
  // constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments
{
  initVariables();
}
HMdcExtractor::~HMdcExtractor(void) {
  // destructor deletes the iterator

  if (itertrkcand)  delete itertrkcand;
  itertrkcand =0;
  if (iterseg)  delete iterseg;
  iterseg =0;
  if (iterkick)  delete iterkick;
  iterkick =0;
  if (iterpidpart)  delete iterpidpart;
  iterpidpart =0;

  if(event)
  {
      //event->Clear();
      //event->Delete();
      //delete event;
      //event=0;
  }

  if(trkcandext)  delete trkcandext;
  if(wireinfoext) delete wireinfoext;
  trkcandext =0;
  wireinfoext=0;

}
void HMdcExtractor::setFileNameCPR(TString namefile)
{
    if(namefile.CompareTo("")==0)
    {
	Error("setFileNameCPR()","No File name specified!");
        exit(1);
    }
    filenameCPR=namefile;
}
void HMdcExtractor::setParContainers() {
  // creates the parameter containers MdcCal2Par if it does not
  // exist and adds them to the list of parameter containers in the runtime
  // database
}
void HMdcExtractor::initVariables()
{
    event    =new HRecEvent;
    initLookupSeg();
    counter=0;
    segCat =0;
    iterseg =0;
    kickCat=0;
    iterkick=0;
    locseg.set(3,0,0,0);
    lockick.set(2,0,0);
    cpr=kTRUE;
    filenameCPR="";
    conditionCPR="OR";
    probSingle=0.9;
    cprct[0]=0;
    cprct[1]=0;
}
Bool_t HMdcExtractor::init(void) {
  // creates the MdcCal1 input and MdcCal2 output categories if they do not
  // exist and adds them to the current event
  // creates an iterator which loops over all fired cells

    setParContainers();
    trkcandCat =(HMatrixCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcTrkCand);
    if(trkcandCat) {
	itertrkcand=(HIterator *)((HCategory*)trkcandCat)->MakeIterator("native");
    }else{
        Warning("init()","No catMdcTrkCand in input!");
    }

    segCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcSeg));
    if(segCat) {
	iterseg=(HIterator *)((HCategory*)segCat)->MakeIterator("native");
    }else{
        Warning("init()","No catMdcSeg in input!");
    }
    HCategory* hitCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcHit));
    if(hitCat) {
    }else{
        Warning("init()","No catMdcHit in input!");
    }
    HCategory* clusinfCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcClusInf));
    if(clusinfCat) {
    }else{
        Warning("init()","No catMdcClusInf in input!");
    }
    kickCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catKickTrackB));
    if(kickCat) {

        iterkick=(HIterator *)((HCategory*)kickCat)->MakeIterator("native");
    }else{
        Warning("init()","No catMdcKickTrack in input!");
    }

    //--------------------pid stuff---------------------------------------------------
    pidpartCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catPidPart));
    if(pidpartCat) {

        iterpidpart=(HIterator *)((HCategory*)pidpartCat)->MakeIterator("native");
    }else{
        Warning("init()","No catPidParticle in input!");
    }
    pidcandCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catPidCandidate));
    if(pidcandCat) {

    }else{
        Warning("init()","No catPidCandidate in input!");
    }
    pidtrackCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catPidTrackCand));
    if(pidtrackCat) {

    }else{
        Warning("init()","No catPidTrackCandidate in input!");
    }
    //----------------------------------------------------------------------------------

    func1=new HMdcFunc1();
    func1->setMdcSegCat(segCat);
    func1->setMdcHitCat(hitCat);
    func1->setClusInfCat(clusinfCat);


    fActive=kTRUE;
    return kTRUE;
}


Int_t HMdcExtractor::execute(void) {

    if(counter==0)
    {
	HCategoryMaker::initCategory(event,catMdcCal1     ,"HMdcCal1"     ,"Mdc");
	HCategoryMaker::initCategory(event,catMdcHit      ,"HMdcHit"      ,"Mdc");
	HCategoryMaker::initCategory(event,catMdcSeg      ,"HMdcSeg"      ,"Mdc");
	HCategoryMaker::initCategory(event,catMdcTrkCand  ,"HMdcTrkCand"  ,"Mdc");
	HCategoryMaker::initCategory(event,catMdcClusInf  ,"HMdcClusInf"  ,"Mdc");
	HCategoryMaker::initCategory(event,catMdcClusFit  ,"HMdcClusFit"  ,"Mdc");
	HCategoryMaker::initCategory(event,catMdcWireFit  ,"HMdcWireFit"  ,"Mdc");
	HCategoryMaker::initCategory(event,catMdcWireInfo ,"HMdcWireInfo" ,"Mdc");
	HCategoryMaker::initCategory(event,catKickTrack   ,"HKickTrack"   ,"");

	trkcandext =new HMdcTrkCandExtractor(event);
	trkcandext ->setDataLevels("cal,hit,clusinf,clusfit,wirefit,kick");
	wireinfoext=new HMdcWireInfoExtractor(event);
	wireinfoext->setDataLevels("seg,hit,clusinf,clusfit,wirefit,kick");

    }
    if(counter%100==0)
    {
	cout<<"event "<<counter<<endl;
	if(cpr)cout<<"singles: "<<cprct[0]<<" , doubles: "<<cprct[1]<<endl;
    }
    counter++;

    if(iterkick&&itertrkcand&&iterpidpart)
    {
	event->Clear();

	HPidParticle*  pidpart;
        HPidCandidate* pidcand;
        HPidTrackCand* pidtrack;

	HKickTrack*  kick;
	HMdcSeg*     seg;
	HMdcTrkCand* trkcand;

	Int_t pidids[10][4];
	for(Int_t i=0;i<10;i++)
	{
	    for(Int_t j=0;j<4;j++)
	    {
		pidids[i][j]=-1;
	    }
	}
	resetLookupSeg();

	Int_t indpart,indcand,indkick,segindex;

	iterpidpart->Reset();
        while ((pidpart=(HPidParticle *)iterpidpart->Next())!=0)
	{
            indpart    =pidpart ->getPidCandidateIndex();
	    pidcand    =(HPidCandidate*)pidcandCat ->getObject(indpart);
            indcand    =pidcand ->getTrackCandIndex();
            pidtrack   =(HPidTrackCand*)pidtrackCat->getObject(indcand);
	    indkick    =pidtrack->getKickTrackId();

	    Int_t ntimeskick =pidtrack->getKickTrackCount();
            Int_t ntimesseg  =pidtrack->getInnerMdcNumber();
            
	    kick=(HKickTrack*)kickCat->getObject(indkick);
	    kick->setRingId(-3);

	    locseg[0]=kick->getSector();
	    locseg[1]=0;
	    locseg[2]=kick->getSegment();

	    seg     =(HMdcSeg*)segCat->getObject(locseg);
	    segindex=segCat->getIndex(locseg);

	    Int_t found=0;
	    if(ntimeskick==1&&ntimesseg==1)
	    { // only if segment and kicktrack was used one time

		itertrkcand->Reset();
		while ((trkcand=(HMdcTrkCand *)itertrkcand->Next())!=0)
		{
                    
		    Int_t inner  =trkcand->getSeg1Ind();
		    //Int_t outer  =trkcand->getSeg2Ind();
		    Int_t nmatch =trkcand->getNCandForSeg1();
		    if(nmatch>=0&&nmatch<2)
		    { // only if no or one matched outer segment
			if(inner==segindex)
			{
			    //if(registerSeg(segindex)==1&&found<10)
			    //{

				if(cpr)
				{
                                    Bool_t isSingle=kFALSE;
				    isSingle=func1->passCprPdfCuts(pidpart,probSingle,(Char_t *)filenameCPR.Data(),1,conditionCPR.Data());
				    // checking wheather HPidParticle pass a probability to be a single
				    // mode_rs =0 for simulation data
				    // mode_rs =1 for real data
				    if(isSingle){kick->setRingId(1);cprct[0]++;}
				    else        {kick->setRingId(2);cprct[1]++;}
				}

                                /*
				pidids[found][0]=indpart;
				pidids[found][1]=indcand;
				pidids[found][2]=indkick;
				pidids[found][3]=segindex;
                                */
				found++;
		        	trkcandext->extract(kick,trkcand,0);
			    //}
			}
		    }
		}
	    }

	}

	HCategoryMaker::copyMdcCal1Cat   (gHades->getCurrentEvent()->getCategory(catMdcCal1)   ,event->getCategory(catMdcCal1));
	HCategoryMaker::copyMdcHitCat    (gHades->getCurrentEvent()->getCategory(catMdcHit)    ,event->getCategory(catMdcHit));
	HCategoryMaker::copyMdcClusInfCat(gHades->getCurrentEvent()->getCategory(catMdcClusInf),event->getCategory(catMdcClusInf));
	HCategoryMaker::copyMdcSegCat    (gHades->getCurrentEvent()->getCategory(catMdcSeg)    ,event->getCategory(catMdcSeg));
	HCategoryMaker::copyMdcClusFitCat(gHades->getCurrentEvent()->getCategory(catMdcClusFit),event->getCategory(catMdcClusFit));
	HCategoryMaker::copyMdcWireFitCat(gHades->getCurrentEvent()->getCategory(catMdcWireFit),event->getCategory(catMdcWireFit));
	HCategoryMaker::copyMdcTrkCandCat(gHades->getCurrentEvent()->getCategory(catMdcTrkCand),event->getCategory(catMdcTrkCand));
	HCategoryMaker::copyKickTrackCat (gHades->getCurrentEvent()->getCategory(catKickTrack) ,event->getCategory(catKickTrack));

	/*
	iterseg->Reset();
        Int_t indseg;
	while ((seg=(HMdcSeg *)iterseg->Next())!=0)
	{
            indseg=segCat->getIndex(seg);

	    for(Int_t i=0;i<10;i++)
	    {
		pidids[i][0]=indpart;
		pidids[i][1]=indcand;
		pidids[i][2]=indkick;
		pidids[i][3]=segindex;

		if(inseg==segindex)
		{

                      event->getCategory()


		}

	    }
	}
         */

	/*
	 //-----------------filling wireinfo----------------------------
	 HIterator* iterkickcp=0;
	 HMatrixCategory* kickcatcp=0;

	 kickcatcp=(HMatrixCategory*)event->getCategory(catKickTrack);
	 if(kickcatcp)iterkickcp=(HIterator *)((HCategory*)kickcatcp)->MakeIterator("native");

	 iterkickcp->Reset();
	 while ((kick=(HKickTrack *)iterkickcp->Next())!=0)
	 {
	 //wireinfoext->extract(kick,0);
	 }
	    delete iterkickcp;
	    //-----------------end filling wireinfo------------------------
	*/

	event->Clear();
	trkcandext->clear();
    }
    return 0;
}
Bool_t HMdcExtractor::finalize()
{
    return kTRUE;
}
