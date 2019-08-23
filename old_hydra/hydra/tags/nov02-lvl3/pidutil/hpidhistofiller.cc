// $Id: hpidhistofiller.cc,v 1.2 2003-03-14 15:37:19 halo Exp $
// Last update by Thomas Eberl: 03/02/26 09:41:22
using namespace std;
#include "hpidhistofiller.h"
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
#include "hlinearcategory.h"
#include "hpidhistfactory.h"
#include "piddef.h"
#include "hpidcutparticle.h"
#include "hpidcutdilepton.h"
#include "hpidfl.h"
#include "hpidtrackcand.h"
#include "hpidcandidate.h"
#include "hpidparticle.h"
#include "hpiddilepton.h"
#include "htofhit.h"
#include <TH1.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TKey.h>
#include <TString.h>
#include <iostream> 
#include <iomanip>


////////////////////////////////////////////////////////////////////////////
// 
// HPidHistoFiller
//
// fill default histograms from "official" data categories
// applying cut objects
// there should be one cut object per data category
// the cut object applied should be saved in the output histogram file
// the histogram names are the default from the histogram factory +
// any user supplied string + a list of the applied cuts
// The available cuts are determined by the inner states of the category
// specific cut object.
// The general syntax of an automatically generated histogram is
// HistogramBaseName(XAXIS.YAXIS).STRING_CUT1.CUT2.CUT....
//
// Histograms are created and filled recursively (if switched on), i.e.  e.g.
// If an invariant mass spectrum is filled from a HPidDilepton,
// the HPidParticles that were used to generate the dilepton are used to fill
// particle histograms like "Mom.vs.Beta", then again the track candidates and 
// detector hits are retrieved and histograms specific for these levels are filled.
// 
// NOTA BENE: the number of created hstograms can be large when using the maximum
// depth of recursion.
//
// TO-DO list
//
// let user specify cut objects
// let user specify depth of histogramming recursion in a convenient way 
// 
//
ClassImp(HPidHistoFiller)

HPidHistoFiller::HPidHistoFiller(Text_t *name,Text_t *title, Char_t* filename,Char_t* cutfilein) :
  HReconstructor(name,title)
{
    pFileName  = filename;// output filename for histos
    pInFileName = cutfilein;//my cuts
    if (!strcmp(pInFileName,"")) pInFileName=0;
}

HPidHistoFiller::HPidHistoFiller()
{

}


HPidHistoFiller::~HPidHistoFiller(void) 
{

}


Bool_t HPidHistoFiller::init() {
        if (gHades) 
	{
	    HEvent *event=gHades->getCurrentEvent();
	    HRuntimeDb *rtdb=gHades->getRuntimeDb();
	    //HSpectrometer *spec=gHades->getSetup();
	    if (event && rtdb) 
	    {
// 		HDetector *rich = spec->getDetector("Rich");
// 		if (rich) 
// 		{ 
// 		}


                // -----------------------------------------------------------
		if((pPidParticleCat = gHades->getCurrentEvent()
		    ->getCategory(catPidPart)) == NULL
		   )
		{
		    Error("init", "No category catPidPart");
		    return kFALSE;
		}
		pIterPidParticle = (HIterator*)pPidParticleCat->MakeIterator("native");
		if (!pIterPidParticle) 
		{
		    Error("init", "No iterator for catPidPart");
		    return kFALSE;
		}
                // -----------------------------------------------------------
                // -----------------------------------------------------------
		if((pPidDileptonCat = gHades->getCurrentEvent()
		    ->getCategory(catPidDilepton)) == NULL
		   )
		{
		    Error("init", "No category catPidPart");
		    return kFALSE;
		}
		pIterPidDilepton = (HIterator*)pPidDileptonCat->MakeIterator("native");
		if (!pIterPidDilepton) 
		{
		    Error("init", "No iterator for catPidPart");
		    return kFALSE;
		}
                // -----------------------------------------------------------

	    }
	}
	

	pHistArray = new TObjArray(10); // auto-expanding
	pFileOut = new TFile(pFileName,"RECREATE");
	if (pInFileName) pFileCutIn = new TFile(pInFileName);
	else pFileCutIn=0;
 	initCuts();
	initSwitches();
        
	nCounterProcessedNbEvents=0; //total nb of evts seen by this task
	return kTRUE;
}

void HPidHistoFiller::initSwitches()
{
    //swt to save vers of histograms in file every n evts
    kMakeSave              = kFALSE;
    // swt to activate/deactivate histogramming levels
    kDetHDileptons         = kTRUE;
    kParticleHDileptons    = kTRUE;
    kDetHParticles         = kTRUE;
}

void HPidHistoFiller::initCuts()
{

    cuts = new TObjArray(1);
    if (!pFileCutIn)
    {
	// if no file opened, create default cuts
	HPidCutParticle *pparticlecut = new HPidCutParticle("particlecut","default");
	cuts->Add(pparticlecut);
	HPidCutDilepton *pdileptoncut = new HPidCutDilepton("dileptoncut","default");
	cuts->Add(pdileptoncut);
	Warning("initCuts", "default cuts created");
    }
    else
    {
	// iterate over cuts in file and load the one with the highest cycle number
	// of each kind
	TIter cutlist(pFileCutIn->GetListOfKeys());
	TKey *cuto=0;
	cout<<"File: "<<pFileCutIn->GetName()<<" opened for reading ..."<<endl;
	while ((cuto=(TKey*)cutlist()))
	{
	    cout<<"Cut loaded: "<<cuto->GetName()<<" "<<cuto->GetTitle()<<endl;
	    cuts->Add((HPidCutO*)pFileCutIn->Get(cuto->GetName()));
	}
    }

    // there is one (most recent) cut object per category
    // switch it into different states via an identifying string when applying it for selection

    particlecut = new TString("particlecut");
    dileptoncut = new TString("dileptoncut");
    
}

Bool_t HPidHistoFiller::finalize() 
{
    // save histograms and cuts into same file
    HPidFL::saveHistos(pFileOut,pHistArray);
    HPidFL::saveCuts(pFileOut,cuts);
    
    pFileOut->Close();
    
    return kTRUE;
}


Int_t HPidHistoFiller::execute()
{

    if (kMakeSave && nCounterProcessedNbEvents > 500000 &&
    	nCounterProcessedNbEvents%100000==0) HPidFL::saveHistos(pFileOut,pHistArray);

    // use different cuts in different selection functions and store the resulting objects in
    // TObjArrays. These can then be handed over to the actual plotting functions which
    // in turn should not do any secret cuts !

    // N.B. The user must make sure that different categories are consistent !
    // i.e. : do not plot particles and dileptons which are not created from the same particle content !


    // 

    // HPidParticle part
    // select particles and store them in different obj array

    TObjArray* pidparticlesubsets = new TObjArray(5);
    pidparticlesubsets->Add(   selectPidParticles("leptonbeta",0)   );
    fillPidParticleHistos(pidparticlesubsets);

    // ----------------------------------------------

    for (Int_t arr=0;arr<pidparticlesubsets->GetLast()+1;arr++) delete ((TObjArray*)(*pidparticlesubsets)[arr]);
    delete pidparticlesubsets;

    // ----------------------------------------------------------------------------------

    // HPidDilepton part
    // select dileptons and store them in different obj arrays

    TObjArray* piddileptonsubsets = new TObjArray(5);

    piddileptonsubsets->Add(  selectPidDileptons("unlike",0)  );
    fillPidDileptonHistos(piddileptonsubsets);

   
    // -----------------------------------

    for (Int_t arr=0;arr<piddileptonsubsets->GetLast()+1;arr++) delete ((TObjArray*)(*piddileptonsubsets)[arr]);
    delete piddileptonsubsets;

    Int_t kReturnValue=0;//proceed with next task

    nCounterProcessedNbEvents++;
    return kReturnValue;
    
}


TObjArray* HPidHistoFiller::selectPidParticles(char* cutStateIdentifier,TObjArray *t)
{
    HPidCutParticle *pparticlecut = (HPidCutParticle*)(cuts->FindObject(particlecut->Data()));
    TObjArray *arr = new TObjArray(5);
    arr->SetName(cutStateIdentifier);
    pparticlecut->switchTo(cutStateIdentifier);

    if(t)
    {
	Int_t max = t->GetLast()+1;
	TString a(t->GetName());
	a.Append(".");
	a.Append(cutStateIdentifier);
	arr->SetName(a.Data());
	for (Int_t i=0;i<max;i++)
	{
	    HPidParticle *h = ((HPidParticle*)(*t)[i]);
	    if (h && pparticlecut->check(h)) arr->Add(h);
	}
    }
    else
    {
	HPidParticle *h=0;
	pIterPidParticle->Reset();
	while(( h = (HPidParticle *)pIterPidParticle->Next())) 
	    if (pparticlecut->check(h)) arr->Add(h);
    }
    return arr;
}


TObjArray* HPidHistoFiller::selectPidDileptons(char* cutStateIdentifier,TObjArray *t)
{
    HPidCutDilepton *pdileptoncut = (HPidCutDilepton*)(cuts->FindObject(dileptoncut->Data()));
    TObjArray *arr = new TObjArray(5);
    arr->SetName(cutStateIdentifier);
    pdileptoncut->switchTo(cutStateIdentifier);

    if(t)
    {
	Int_t max = t->GetLast()+1;
	TString a(t->GetName());
	a.Append(".");
	a.Append(cutStateIdentifier);
	arr->SetName(a.Data());
	for (Int_t i=0;i<max;i++)
	{
	    HPidDilepton *h = ((HPidDilepton*)(*t)[i]);
	    if (h && pdileptoncut->check(h)) arr->Add(h);
	}
    }
    else
    {
	HPidDilepton *h=0;
	pIterPidDilepton->Reset();
	while(( h = (HPidDilepton *)pIterPidDilepton->Next())) 
	    if (pdileptoncut->check(h)) arr->Add(h);
    }
    return arr;

}


void HPidHistoFiller::fillPidParticleHistos(TObjArray *subsets)
{
    for (Int_t arr=0;arr<subsets->GetLast()+1;arr++) 
    {
	TObjArray* t = ((TObjArray*)(*subsets)[arr]);
	if (!fillParticleHistosFromParticles(t,t->GetName())) Error("execute","fillParticleHistosFromParticles() failed");
    }
}

void HPidHistoFiller::fillPidDileptonHistos(TObjArray *subsets)
{
    for (Int_t arr=0;arr<subsets->GetLast()+1;arr++) 
    {
	TObjArray* t = ((TObjArray*)(*subsets)[arr]);
	if (!fillDileptonHistosFromDileptons(t,t->GetName())) Error("execute","fillDileptonHistosFromDileptons() failed");
    }
}

Int_t HPidHistoFiller::fillDetHistosFromParticles(TObjArray* particleArray,const Char_t *cNameParticleSample)
{
    Int_t ret_val=1;
    Int_t max = particleArray->GetLast()+1;
    Int_t *rings = new Int_t[max];
    Int_t *tofs = new Int_t[max];
    for (Int_t j=0;j<max;j++) rings[j]=tofs[j]=-2;

    TString samplename(particleArray->GetName());
    if (!samplename.Contains(cNameParticleSample)) appendName(samplename,cNameParticleSample);

    for (Int_t i=0;i<max;i++)
    {
	HPidParticle *h = ((HPidParticle*)(*particleArray)[i]);

	HPidCandidate *c = (HPidCandidate*) h -> getPidCandidate();

	HPidTrackCand *t = (HPidTrackCand*) c -> getTrackCand();
	
	HRichHit *r = t -> getRichHit();
	// RICH
	Bool_t isNewRing = HPidFL::isNewIndex((Int_t)t->getRingId(),rings,max);
	if (isNewRing)
	{
	    TString sector("sec");
	    sector += (Int_t) t -> getSector();
	    appendName(samplename,sector.Data());
		
	    fillTH1F(HPidHistFactory::getHRingRadiusBasename(),samplename.Data(),r->getRadius());
	    fillTH2F(HPidHistFactory::getHRingRadiusThetaBasename(),samplename.Data(),r->getTheta(),r->getRadius());
		
	    fillTH1F(HPidHistFactory::getHRingAmplitudeBasename(),samplename.Data(),r->getRingAmplitude());
	    fillTH2F(HPidHistFactory::getHRingAmplitudeThetaBasename(),samplename.Data(),r->getTheta(),r->getRingAmplitude());
		
	    fillTH1F(HPidHistFactory::getHRingPadNrBasename(),samplename.Data(),r->getRingPadNr());
	    fillTH2F(HPidHistFactory::getHRingPadNrThetaBasename(),samplename.Data(),r->getTheta(),r->getRingPadNr());
		
	    fillTH1F(HPidHistFactory::getHRingLocMax4Basename(),samplename.Data(),r->getRingLocalMax4());
	    fillTH2F(HPidHistFactory::getHRingLocMax4ThetaBasename(),samplename.Data(),r->getTheta(),r->getRingLocalMax4());
		
	    fillTH1F(HPidHistFactory::getHRingPMBasename(),samplename.Data(),r->getRingPatMat());
	    fillTH2F(HPidHistFactory::getHRingPMThetaBasename(),samplename.Data(),r->getTheta(),r->getRingPatMat());
		
	    fillTH1F(HPidHistFactory::getHRingHTBasename(),samplename.Data(),r->getRingHouTra());
	    fillTH2F(HPidHistFactory::getHRingHTThetaBasename(),samplename.Data(),r->getTheta(),r->getRingHouTra());

 	    fillTH1F(HPidHistFactory::getHRingCentroidBasename(),samplename.Data(),r->getCentroid());
	    fillTH2F(HPidHistFactory::getHRingCentroidThetaBasename(),samplename.Data(),r->getTheta(),r->getCentroid());

	    Float_t avChrg = ((Float_t)r->getRingAmplitude()) / ((Float_t)r->getRingPadNr());
	    fillTH1F(HPidHistFactory::getHRingAvChrgBasename(),samplename.Data(),1.);
	    fillTH2F(HPidHistFactory::getHRingAvChrgThetaBasename(),samplename.Data(),r->getTheta(),avChrg);

	    fillTH2F(HPidHistFactory::getHRingAvChrgLocMax4Basename(),samplename.Data(),avChrg,r->getRingLocalMax4());
	    fillTH2F(HPidHistFactory::getHRingAvChrgCentroidBasename(),samplename.Data(),avChrg,r->getCentroid());
	    fillTH2F(HPidHistFactory::getHRingAvChrgPatMatBasename(),samplename.Data(),avChrg,r->getRingPatMat());
	    fillTH2F(HPidHistFactory::getHRingAvChrgPadNrBasename(),samplename.Data(),avChrg,r->getRingPadNr());
	    fillTH2F(HPidHistFactory::getHRingLocMax4CentroidBasename(),samplename.Data(),r->getRingLocalMax4(),r->getCentroid());
	    fillTH2F(HPidHistFactory::getHRingLocMax4PatMatBasename(),samplename.Data(),r->getRingLocalMax4(),r->getRingPatMat());
	    fillTH2F(HPidHistFactory::getHRingLocMax4PadNrBasename(),samplename.Data(),r->getRingLocalMax4(),r->getRingPadNr());
	    fillTH2F(HPidHistFactory::getHRingCentroidPatMatBasename(),samplename.Data(),r->getCentroid(),r->getRingPatMat());
	    fillTH2F(HPidHistFactory::getHRingCentroidPadNrBasename(),samplename.Data(),r->getCentroid(),r->getRingPadNr());
	    fillTH2F(HPidHistFactory::getHRingPatMatPadNrBasename(),samplename.Data(),r->getRingPatMat(),r->getRingPadNr());



	    fillTH2F(HPidHistFactory::getHRingXYBasename(),samplename.Data(),r->getRingCenterX(),r->getRingCenterY());
	}
	

	// build new name
	samplename="";
	samplename=particleArray->GetName();
	if (!samplename.Contains(cNameParticleSample)) appendName(samplename,cNameParticleSample);

	HTofHit *tof = 0;
	if (t->getSystem()==1) tof = t -> getTofHit();
	{
	    //TOF
	    Bool_t isNewTof = HPidFL::isNewIndex((Int_t)t->getMetaId(),tofs,max);
	    if (isNewTof)
	    {
		//		cout<<HPidHistFactory::getHTofBasename()<<" ///  "<<samplename.Data()<<endl;
		//	fillTH1F(HPidHistFactory::getHTofBasename(),samplename.Data(),tof->getTof());
	    }
	
	}
    }

    delete [] rings;
    delete [] tofs;

    return ret_val;
}

Int_t HPidHistoFiller::fillParticleHistosFromParticles(TObjArray* particleArray,const Char_t *cNameParticleSample)
{
    // function takes array with selected particles and fills fixed histograms.
    // function makes sure that particle is not used twice and that components
    // of a particle are not used twice


 Int_t ret_val=1;
 Int_t max = particleArray->GetLast()+1;
 // define helper arrays
 Int_t *pidcand=new Int_t[max];
 // init helper arrays
 for (Int_t j=0;j<max;j++) pidcand[j]=-2;

 
    for (Int_t i=0;i<max;i++)
    {
	HPidParticle *hi = ((HPidParticle*)(*particleArray)[i]);


	Int_t particleind=(Int_t)hi->getPidCandidateId();
	Bool_t kNewParticle=HPidFL::isNewIndex(particleind,pidcand,max);
	if(kNewParticle)
	{

	    TString samplename=particleArray->GetName();
	    if (!samplename.Contains(cNameParticleSample)) appendName(samplename,cNameParticleSample);
	    //
	    fillTH2F(HPidHistFactory::getHMomChrgvsBetaBasename(),samplename.Data(),
		     hi->P()*hi->getCharge(),hi->getBetaExp());
	    //
	    fillTH2F(HPidHistFactory::getHMassvsBetaBasename(),samplename.Data(),
		     hi->getMassExp(),hi->getBetaExp());
	    //
	    fillTH2F(HPidHistFactory::getHMassChrgvsBetaBasename(),samplename.Data(),
		     hi->getMassExp()*hi->getCharge(),hi->getBetaExp());
	    //
	    fillTH1F(HPidHistFactory::getHMassBasename(),samplename.Data(),
		     hi->getMassExp());

	}
    }

    delete [] pidcand;

    if (kDetHParticles) if (!fillDetHistosFromParticles(particleArray,particleArray->GetName())) Error("execute","fillDetHistosFromParticles() failed");
	
    return ret_val;
}

Int_t HPidHistoFiller::fillParticleHistosFromDileptons(TObjArray *dileptons,const Char_t *cNameDileptonSample)
{
    Int_t ret_val=1;
    Int_t max = dileptons->GetLast()+1;
    Int_t *nNewParticle=new Int_t[max];
    for (Int_t i=0;i<max;i++) nNewParticle[i]=-2;
    TObjArray *particles = new TObjArray(5);
    TString samplename(cNameDileptonSample);
    particles->SetName(samplename.Data());
    for (Int_t i=0;i<max;i++)
    {

	HPidDilepton *h = ((HPidDilepton*)(*dileptons)[i]);
	
   	if (HPidFL::isNewIndex((Int_t)h->getParticleId1(),nNewParticle,max)) 
	    particles->Add((HPidParticle*) h -> getParticle1());
   	if (HPidFL::isNewIndex((Int_t)h->getParticleId2(),nNewParticle,max)) 
	    particles->Add((HPidParticle*) h -> getParticle2());

    }

    if (kParticleHDileptons) 
    {
	TObjArray *t = selectPidParticles("leptonbeta",particles);
	fillParticleHistosFromParticles(t,cNameDileptonSample);
	delete t;
    }
    if (kParticleHDileptons) fillParticleHistosFromParticles(particles,cNameDileptonSample);
    if (kDetHDileptons)   fillDetHistosFromParticles(particles,cNameDileptonSample);
    delete particles;
    delete nNewParticle;

    return ret_val;
}

Int_t HPidHistoFiller::fillDileptonHistosFromDileptons(TObjArray *t,const Char_t *cNameDileptonSample)
{
    Int_t ret_val=1;
    Int_t max = t->GetLast()+1;
    // helper arrays to determine unique nature of index tuples
    Int_t *nParticle2Tuple=new Int_t[max];

    for (Int_t i=0;i<max;i++) nParticle2Tuple[i]=-2;

    TString addtohistname("");
    for (Int_t i=0;i<max;i++)
    {
	HPidDilepton *h = ((HPidDilepton*)(*t)[i]);
	// check whether the particle index combination was used before in this evt
	Bool_t isNewDilepton = HPidFL::isNew2Tuple((Int_t)h->getParticleId1(),(Int_t)h->getParticleId2(),nParticle2Tuple,max);

	if (isNewDilepton)
	{
	    setHistNameAddition(addtohistname,"",cNameDileptonSample);
	    fillTH1F(HPidHistFactory::getHOpeningAngleBasename(),addtohistname.Data(),h->getOpeningAngleDeg());

	    setHistNameAddition(addtohistname,"",cNameDileptonSample);
	    fillTH1F(HPidHistFactory::getHInvMassBasename(),addtohistname.Data(),h->M());

	}	    
    }

    delete [] nParticle2Tuple;
    if (kParticleHDileptons) if (!fillParticleHistosFromDileptons(t,t->GetName())) Error("execute","fillParticleHistosFromDileptons() failed");
    
    return ret_val;
}

void HPidHistoFiller::fillTH1F(const char *basename,const char *addtohistname,Float_t val)
{
    TString base(basename);
    base.Append(addtohistname);
    TH1F *h = (TH1F*) pHistArray->FindObject(base.Data());
    if (!h)
    {
	h = (TH1F*) HPidHistFactory::getHistoByBaseName(basename,addtohistname);
	pHistArray->Add(h);			
    }
    h->Fill(val);
}
void HPidHistoFiller::fillTH2F(const char *basename,const char *addtohistname,Float_t val1,Float_t val2)
{
    TString base(basename);
    base.Append(addtohistname);
    TH2F *h = (TH2F*) pHistArray->FindObject(base.Data());
    if (!h)
    {
	h = (TH2F*) HPidHistFactory::getHistoByBaseName(basename,addtohistname);
	if (h) pHistArray->Add(h);
    }
    h->Fill(val1,val2);
}
void HPidHistoFiller::setHistNameAddition(TString &str,const Char_t* histname,const Char_t* samplename)
{
    str="";
    str.Append(histname);
    str.Append(".");
    str.Append(samplename);
}
void HPidHistoFiller::appendName(TString &s,const char*c)
{
    s.Append(".");
    s.Append(c);
}
void HPidHistoFiller::prependName(TString &s,const char*c)
{
    s.Prepend(c);
}
