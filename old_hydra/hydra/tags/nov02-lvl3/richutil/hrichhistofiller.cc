// $Id: hrichhistofiller.cc,v 1.5 2003-01-24 18:24:33 halo Exp $
// Last update by Thomas Eberl: 03/01/21 14:37:00
using namespace std;
#include "hrichhistofiller.h"
#include "hdihitmatch.h"
#include "hrichcut.h"
#include "hrichcuttracklet.h"
#include "hrichcutpair.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hrichhit.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatch.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
#include "hrichhistfac.h"
#include <TH1.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TKey.h>
#include <string.h>
#include <iostream> 
#include <iomanip>

ClassImp(HRichHistoFiller)

HRichHistoFiller::HRichHistoFiller(Text_t *name,Text_t *title, Char_t* filename,Char_t* cutfilein) :
  HReconstructor(name,title)
{
    pFileName  = filename;// output filename for histos
    pInFileName = cutfilein;//my cuts
    if (!strcmp(pInFileName,"")) pInFileName=0;
}

HRichHistoFiller::HRichHistoFiller()
{

}


HRichHistoFiller::~HRichHistoFiller(void) 
{

}


Bool_t HRichHistoFiller::init() {
        if (gHades) 
	{
	    HEvent *event=gHades->getCurrentEvent();
	    HRuntimeDb *rtdb=gHades->getRuntimeDb();
	    HSpectrometer *spec=gHades->getSetup();
	    if (event && rtdb) 
	    {
		HDetector *rich = spec->getDetector("Rich");
		if (rich) 
		{ 
		    pHitMatchCat=event->getCategory(catMatchHit);
		    if (!pHitMatchCat) 
		    {
			Error("init","No HIT MATCH category defined, check if it is swt off in calling macro !");
			return kFALSE;
		    }
		    else  pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");

 		    pHitDiMatchCat=event->getCategory(catDiMatchHit);
 		    if (!pHitDiMatchCat) 
 		    {
 			Error("init","No HIT DIMATCH category defined, check if it is swt off in calling macro !");
 			return kFALSE;
 		    }
 		    else pIterDiMatchHit = (HIterator*)getHitDiMatchCat()->MakeIterator("native");
		}
	    }
	}


	pHistArray = new TObjArray(10); // auto-expanding
	pFileOut = new TFile(pFileName,"RECREATE");
	if (pInFileName) pFileCutIn = new TFile(pInFileName);
	else pFileCutIn=0;
 	initCuts();
        kMakeSave = kFALSE;//swt to save vers of histograms in file every n evts
	nCounterProcessedNbEvents=0; //total nb of evts seen by this task
	return kTRUE;
}

void HRichHistoFiller::initCuts()
{

    // swt to activate/deactivate histogramming levels
    kDetHPairs=kTRUE;
    kTrackHPairs=kTRUE;
    kDetHTracks=kTRUE;
    
    cuts = new TObjArray(1);
    if (!pFileCutIn)
    {
	HRichCutTracklet *trackcut = new HRichCutTracklet("trackcut","default");
	cuts->Add(trackcut);
	HRichCutPair *paircut = new HRichCutPair("paircut","default");
	cuts->Add(paircut);
	cout<<"WARNING: default cuts created"<<endl;
    }
    else
    {
	TIter cutlist(pFileCutIn->GetListOfKeys());
	TKey *cuto=0;
	cout<<"File: "<<pFileCutIn->GetName()<<" opened for reading ..."<<endl;
	while ((cuto=(TKey*)cutlist()))
	{
	    cout<<"Cut loaded: "<<cuto->GetName()<<" "<<cuto->GetTitle()<<endl;
	    cuts->Add((HRichCutO*)pFileCutIn->Get(cuto->GetName()));
	}
    }

    // there is one (most recent) cut object per category
    // switch it into different states via an identifying string when applying it for selection

    trackletcut = new TString("trackcut");;
    paircut     = new TString("paircut");
    
}

Bool_t HRichHistoFiller::finalize() {
     HRichUtilFunc::saveHistos(pFileOut,pHistArray);
     HRichUtilFunc::saveCuts(pFileOut,cuts);
     
     pFileOut->Close();

    return kTRUE;
}


Int_t HRichHistoFiller::execute()
{

    if (kMakeSave && nCounterProcessedNbEvents > 500000 &&
    	nCounterProcessedNbEvents%100000==0) HRichUtilFunc::saveHistos(pFileOut,pHistArray);

    // use different cuts in different selection functions and store the resulting objects in
    // TObjArrays. These can then be handed over to the actual plotting functions which
    // in turn should not do any secret cuts !

    // N.B. The user must make sure that different categories are consistent !
    // i.e. : do not plot tracklets and pairs which are not created from the same tracklet content !

    // TRACKLET part
    // select tracklets and store them in different obj array

    TObjArray* tracksubsets = new TObjArray(5);
    //tracksubsets->Add(selectTracklets("matchedGoodRing",0));

    fillTrackletHistos(tracksubsets);


    // ----------------------------------------------

    for (Int_t arr=0;arr<tracksubsets->GetLast()+1;arr++) delete ((TObjArray*)(*tracksubsets)[arr]);
    delete tracksubsets;

    // ----------------------------------------------------------------------------------

    // PAIR part
    // select pairs and store them in different obj arrays

    TObjArray* pairsubsets = new TObjArray(5);

    pairsubsets->Add(  selectPairs("unlike",0)  );
    pairsubsets->Add(  selectPairs("like+",0)   );
    pairsubsets->Add(  selectPairs("like-",0)   );
    pairsubsets->Add(  selectPairs("opangKICK>",0)  );
    pairsubsets->Add(  selectPairs("opangKICK>",(TObjArray*)pairsubsets->FindObject("unlike"))  );
    pairsubsets->Add(  selectPairs("unlike.opangKICK>",0)  );
    pairsubsets->Add(  selectPairs("unlike.2hit",0)   );
    pairsubsets->Add(  selectPairs("unlike.2hit",(TObjArray*)pairsubsets->FindObject("opangKICK>"))   );
    pairsubsets->Add(  selectPairs("unlike.1richhit",0)   );
    pairsubsets->Add(  selectPairs("unlike.1mdchit",0)   );
    fillPairHistos(pairsubsets);

   
    // -----------------------------------

    for (Int_t arr=0;arr<pairsubsets->GetLast()+1;arr++) delete ((TObjArray*)(*pairsubsets)[arr]);
    delete pairsubsets;

    Int_t kReturnValue=0;//proceed with next task

    nCounterProcessedNbEvents++;
    return kReturnValue;
    
}


TObjArray* HRichHistoFiller::selectTracklets(char* cutStateIdentifier,TObjArray *t)
{
    HRichCutTracklet *trackcut = (HRichCutTracklet*)(cuts->FindObject(trackletcut->Data()));
    TObjArray *arr = new TObjArray(5);
    arr->SetName(cutStateIdentifier);
    trackcut->switchTo(cutStateIdentifier);

    if(t)
    {
	Int_t max = t->GetLast()+1;
	TString a(t->GetName());
	a.Append(".");
	a.Append(cutStateIdentifier);
	arr->SetName(a.Data());
	for (Int_t i=0;i<max;i++)
	{
	    HHitMatch *h = ((HHitMatch*)(*t)[i]);
	    if (h && trackcut->check(h)) arr->Add(h);
	}
    }
    else
    {
	HHitMatch *h=0;
	pIterMatchHit->Reset();
	while(( h = (HHitMatch *)pIterMatchHit->Next())) 
	    if (trackcut->check(h)) arr->Add(h);
    }
    return arr;
}


TObjArray* HRichHistoFiller::selectPairs(char* cutStateIdentifier,TObjArray *t)
{
    HRichCutPair *paircuto = (HRichCutPair*)(cuts->FindObject(paircut->Data()));
    TObjArray *arr = new TObjArray(5);
    arr->SetName(cutStateIdentifier);
    paircuto->switchTo(cutStateIdentifier);

    if(t)
    {
	Int_t max = t->GetLast()+1;
	TString a(t->GetName());
	a.Append(".");
	a.Append(cutStateIdentifier);
	arr->SetName(a.Data());
	for (Int_t i=0;i<max;i++)
	{
	    HDiHitMatch *h = ((HDiHitMatch*)(*t)[i]);
	    if (h && paircuto->check(h)) arr->Add(h);
	}
    }
    else
    {
	HDiHitMatch *h=0;
	pIterDiMatchHit->Reset();
	while(( h = (HDiHitMatch *)pIterDiMatchHit->Next())) 
	    if (paircuto->check(h)) arr->Add(h);
    }
    return arr;

}


void HRichHistoFiller::fillTrackletHistos(TObjArray *subsets)
{
    for (Int_t arr=0;arr<subsets->GetLast()+1;arr++) 
    {
	TObjArray* t = ((TObjArray*)(*subsets)[arr]);
	if (!fillTrackHistosFromTracklets(t,t->GetName())) Error("execute","fillTrackHistosFromTracklets() failed");
    }
}

void HRichHistoFiller::fillPairHistos(TObjArray *subsets)
{
    for (Int_t arr=0;arr<subsets->GetLast()+1;arr++) 
    {
	TObjArray* t = ((TObjArray*)(*subsets)[arr]);
	if (!fillPairHistosFromPairs(t,t->GetName())) Error("execute","fillPairHistosFromPairs() failed");
    }
}

Int_t HRichHistoFiller::fillDetHistosFromTracklets(TObjArray* trackArray,const Char_t *cNameTrackSample)
{
    Int_t ret_val=1;
    Int_t max = trackArray->GetLast()+1;
    Int_t *rings = new Int_t[max];
    Int_t *tofs = new Int_t[max];
    for (Int_t j=0;j<max;j++) rings[j]=tofs[j]=-2;

    TString samplename(trackArray->GetName());
    if (!samplename.Contains(cNameTrackSample)) appendName(samplename,cNameTrackSample);

    for (Int_t i=0;i<max;i++)
    {
	HHitMatch *h = ((HHitMatch*)(*trackArray)[i]);

	// RICH
	Bool_t isNewRing = HRichCut::isNewIndex(h->getRichInd(),rings,max);
	if (isNewRing)
	{
	    TString sector("sec");
	    sector+=h->getSector();
	    appendName(samplename,sector.Data());
		
	    fillTH1F(HRichHistFac::getHRingRadiusBasename(),samplename.Data(),h->getRadius());
	    fillTH2F(HRichHistFac::getHRingRadiusThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRadius());
		
	    fillTH1F(HRichHistFac::getHRingAmplitudeBasename(),samplename.Data(),h->getRingAmplitude());
	    fillTH2F(HRichHistFac::getHRingAmplitudeThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRingAmplitude());
		
	    fillTH1F(HRichHistFac::getHRingPadNrBasename(),samplename.Data(),h->getRingPadNr());
	    fillTH2F(HRichHistFac::getHRingPadNrThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRingPadNr());
		
	    fillTH1F(HRichHistFac::getHRingLocMax4Basename(),samplename.Data(),h->getRingLocalMax4());
	    fillTH2F(HRichHistFac::getHRingLocMax4ThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRingLocalMax4());
		
	    fillTH1F(HRichHistFac::getHRingPMBasename(),samplename.Data(),h->getRingPatMat());
	    fillTH2F(HRichHistFac::getHRingPMThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRingPatMat());
		
	    fillTH1F(HRichHistFac::getHRingHTBasename(),samplename.Data(),h->getRingHouTra());
	    fillTH2F(HRichHistFac::getHRingHTThetaBasename(),samplename.Data(),h->getRichTheta(),h->getRingHouTra());

 	    fillTH1F(HRichHistFac::getHRingCentroidBasename(),samplename.Data(),h->getCentroid());
	    fillTH2F(HRichHistFac::getHRingCentroidThetaBasename(),samplename.Data(),h->getRichTheta(),h->getCentroid());

	    Float_t avChrg = ((Float_t)h->getRingAmplitude()) / ((Float_t)h->getRingPadNr());
	    fillTH1F(HRichHistFac::getHRingAvChrgBasename(),samplename.Data(),1.);
	    fillTH2F(HRichHistFac::getHRingAvChrgThetaBasename(),samplename.Data(),h->getRichTheta(),avChrg);

	    fillTH2F(HRichHistFac::getHRingAvChrgLocMax4Basename(),samplename.Data(),avChrg,h->getRingLocalMax4());
	    fillTH2F(HRichHistFac::getHRingAvChrgCentroidBasename(),samplename.Data(),avChrg,h->getCentroid());
	    fillTH2F(HRichHistFac::getHRingAvChrgPatMatBasename(),samplename.Data(),avChrg,h->getRingPatMat());
	    fillTH2F(HRichHistFac::getHRingAvChrgPadNrBasename(),samplename.Data(),avChrg,h->getRingPadNr());
	    fillTH2F(HRichHistFac::getHRingLocMax4CentroidBasename(),samplename.Data(),h->getRingLocalMax4(),h->getCentroid());
	    fillTH2F(HRichHistFac::getHRingLocMax4PatMatBasename(),samplename.Data(),h->getRingLocalMax4(),h->getRingPatMat());
	    fillTH2F(HRichHistFac::getHRingLocMax4PadNrBasename(),samplename.Data(),h->getRingLocalMax4(),h->getRingPadNr());
	    fillTH2F(HRichHistFac::getHRingCentroidPatMatBasename(),samplename.Data(),h->getCentroid(),h->getRingPatMat());
	    fillTH2F(HRichHistFac::getHRingCentroidPadNrBasename(),samplename.Data(),h->getCentroid(),h->getRingPadNr());
	    fillTH2F(HRichHistFac::getHRingPatMatPadNrBasename(),samplename.Data(),h->getRingPatMat(),h->getRingPadNr());



	    fillTH2F(HRichHistFac::getHRingXYBasename(),samplename.Data(),h->getRingCenterX(),h->getRingCenterY());
	}
	

	// build new name
	samplename="";
	samplename=trackArray->GetName();
	if (!samplename.Contains(cNameTrackSample)) appendName(samplename,cNameTrackSample);

	//TOF
	Bool_t isNewTof = HRichCut::isNewIndex(h->getTofInd(),tofs,max);
	if (isNewTof)
	{
	    fillTH1F(HRichHistFac::getHTofBasename(),samplename.Data(),h->getTofTof());
	}
	
	
    }

    delete [] rings;
    delete [] tofs;

    return ret_val;
}

Int_t HRichHistoFiller::fillTrackHistosFromTracklets(TObjArray* trackArray,const Char_t *cNameTrackSample)
{
    // function takes array with selected tracks and fills fixed histograms.
    // function makes sure that track is not used twice and that components
    // of a track are not used twice
    // N.B.: if track was created using the COMPLEX MODE, a check for the unique nature
    // of a kick track is not sufficient, as there will be tracks that have no kicktrack

 Int_t ret_val=1;
 Int_t max = trackArray->GetLast()+1;
 // define helper arrays
 // for deflection angle histo
 Int_t *tupt = new Int_t[max];
 Int_t *tups = new Int_t[max];
 // for mom vs beta histo
 Int_t *kicks=new Int_t[max];
 // init helper arrays
 for (Int_t j=0;j<max;j++) tupt[j]=tups[j]=kicks[j]=-2;

 
    for (Int_t i=0;i<max;i++)
    {
	HHitMatch *hi = ((HHitMatch*)(*trackArray)[i]);



	// fill deflection angle using each mdc-meta combi only once
	TString samplename(trackArray->GetName());
	appendName(samplename,"MDC_META");
	if (!samplename.Contains(cNameTrackSample)) appendName(samplename,cNameTrackSample);
	Int_t mdcidx=hi->getMdcInd();
	Int_t metaidx=-1;
	Int_t *arr=0;
	if (hi->getTofInd()>=0) {metaidx=hi->getTofInd();arr=tupt;}
	else if (hi->getShowInd()>=0) {metaidx=hi->getShowInd();arr=tups;}
	Bool_t kNewDeflAngle = HRichCut::isNew2Tuple(mdcidx,metaidx,arr,max);
	if (kNewDeflAngle) fillTH1F(HRichHistFac::getHDeflAngleBasename(),samplename.Data(),
				    HRichUtilFunc::getDeflectionAngle(hi));

	
	// fill kick track histos for different kicktracks
	Int_t trackind=hi->getKickInd();
	Bool_t kNewTrack=HRichCut::isNewIndex(trackind,kicks,max);
	if(kNewTrack)
	{
	    samplename="";
	    samplename=trackArray->GetName();
	    if (!samplename.Contains(cNameTrackSample)) appendName(samplename,cNameTrackSample);
	    //
	    fillTH2F(HRichHistFac::getHMomChrgvsBetaBasename(),samplename.Data(),
		     hi->getKickMom()*hi->getKickCharge(),hi->getKickBeta());
	    //
	    fillTH2F(HRichHistFac::getHMassvsBetaBasename(),samplename.Data(),
		     hi->getKickMass(),hi->getKickBeta());
	    //
	    fillTH2F(HRichHistFac::getHMassChrgvsBetaBasename(),samplename.Data(),
		     hi->getKickMass()*hi->getKickCharge(),hi->getKickBeta());
	    //
	    fillTH1F(HRichHistFac::getHMassBasename(),samplename.Data(),
		     hi->getKickMass());
	    //
	    // use a MDC-META combi only once, even if two kicktracks are different,cf. above !
	    if (kNewDeflAngle) fillTH2F(HRichHistFac::getHDeflAnglevsMassBasename(),samplename.Data(),
				      HRichUtilFunc::getDeflectionAngle(hi),hi->getKickMass());
	    //
	    if (kNewDeflAngle) fillTH2F(HRichHistFac::getHDeflAnglevsBetaBasename(),samplename.Data(),
				      HRichUtilFunc::getDeflectionAngle(hi),hi->getKickBeta());

	}
    }
    delete [] tupt;
    delete [] tups;
    delete [] kicks;

    if (kDetHTracks) if (!fillDetHistosFromTracklets(trackArray,trackArray->GetName())) Error("execute","fillDetHistosFromTracklets() failed");
	
    return ret_val;
}

Int_t HRichHistoFiller::fillTrackHistosFromPairs(TObjArray *pairs,const Char_t *cNamePairSample)
{
    Int_t ret_val=1;
    Int_t max = pairs->GetLast()+1;
    Int_t *nNewTrack=new Int_t[max];
    for (Int_t i=0;i<max;i++) nNewTrack[i]=-2;
    TObjArray *tracks = new TObjArray(5);
    TString samplename(cNamePairSample);
    tracks->SetName(samplename.Data());
    for (Int_t i=0;i<max;i++)
    {

	HDiHitMatch *h = ((HDiHitMatch*)(*pairs)[i]);
	
   	if (HRichCut::isNewIndex(h->getIndTrk1(),nNewTrack,max)) 
	    tracks->Add((HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk1()));
   	if (HRichCut::isNewIndex(h->getIndTrk2(),nNewTrack,max)) 
	    tracks->Add((HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk2()));

    }

    if (kTrackHPairs) 
    {
	TObjArray *t = selectTracklets("matchedGoodRing",tracks);
	fillTrackHistosFromTracklets(t,cNamePairSample);
	delete t;
    }
    if (kTrackHPairs) fillTrackHistosFromTracklets(tracks,cNamePairSample);
    if (kDetHPairs)   fillDetHistosFromTracklets(tracks,cNamePairSample);
    delete tracks;
    delete nNewTrack;

    return ret_val;
}

Int_t HRichHistoFiller::fillPairHistosFromPairs(TObjArray *t,const Char_t *cNamePairSample)
{
    Int_t ret_val=1;
    Int_t max = t->GetLast()+1;
    // helper arrays to determine unique nature of index tuples
    Int_t *nTrack2Tuple=new Int_t[max];

    for (Int_t i=0;i<max;i++) nTrack2Tuple[i]=-2;

    TString addtohistname("");
    for (Int_t i=0;i<max;i++)
    {
	HDiHitMatch *h = ((HDiHitMatch*)(*t)[i]);

	Bool_t isNewPair = HRichCut::isNew2Tuple(h->getIndTrk1(),h->getIndTrk2(),nTrack2Tuple,max);

	if (isNewPair)
	{
	    setHistNameAddition(addtohistname,"KICK",cNamePairSample);
	    fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),h->getOpangKICK());

	    setHistNameAddition(addtohistname,"MDC",cNamePairSample);
	    fillTH1F(HRichHistFac::getHOpeningAngleBasename(),addtohistname.Data(),h->getOpangMDC());

	    setHistNameAddition(addtohistname,"",cNamePairSample);
	    fillTH1F(HRichHistFac::getHInvMassBasename(),addtohistname.Data(),h->getInvMass());

	    // Kienle's histo
	    HHitMatch *hi = (HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk1());
	    HHitMatch *hj = (HHitMatch*)getHitMatchCat()->getObject(h->getIndTrk2());
	    TLorentzVector* vi = ((TLorentzVector*)hi->getLVec());
	    TLorentzVector* vj = ((TLorentzVector*)hj->getLVec());
	    Float_t psum = (vi->Vect()+vj->Vect()).Mag();
	    Float_t esum = sqrt((vi->Vect()).Mag()*(vi->Vect()).Mag() + 0.511*0.511)+
		sqrt((vj->Vect()).Mag()*(vj->Vect()).Mag() + 0.511*0.511);
	    fillTH2F(HRichHistFac::getHMomvsEnergyBasename(),addtohistname.Data(),psum,esum);

	}	    
    }

    delete [] nTrack2Tuple;
    if (kTrackHPairs) if (!fillTrackHistosFromPairs(t,t->GetName())) Error("execute","fillTrackHistosFromPairs() failed");
    
    return ret_val;
}

void HRichHistoFiller::fillTH1F(const char *basename,const char *addtohistname,Float_t val)
{
    TString base(basename);
    base.Append(addtohistname);
    TH1F *h = (TH1F*) pHistArray->FindObject(base.Data());
    if (!h)
    {
	h = (TH1F*) HRichHistFac::getHistoByBaseName(basename,addtohistname);
	pHistArray->Add(h);			
    }
    h->Fill(val);
}
void HRichHistoFiller::fillTH2F(const char *basename,const char *addtohistname,Float_t val1,Float_t val2)
{
    TString base(basename);
    base.Append(addtohistname);
    TH2F *h = (TH2F*) pHistArray->FindObject(base.Data());
    if (!h)
    {
	h = (TH2F*) HRichHistFac::getHistoByBaseName(basename,addtohistname);
	if (h) pHistArray->Add(h);
    }
    h->Fill(val1,val2);
}
void HRichHistoFiller::setHistNameAddition(TString &str,const Char_t* histname,const Char_t* samplename)
{
    str="";
    str.Append(histname);
    str.Append(".");
    str.Append(samplename);
}
void HRichHistoFiller::appendName(TString &s,const char*c)
{
    s.Append(".");
    s.Append(c);
}
void HRichHistoFiller::prependName(TString &s,const char*c)
{
    s.Prepend(c);
}
