// $Id: hrichpcasim.cc,v 1.7 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:43:34
//
using namespace std;
#include "hrichpcasim.h"
#include "hrichcut.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include <iostream> 
#include <iomanip>
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hhitmatchsim.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
#include "TVectorD.h"
#include "TMatrixD.h"
#include "TH1.h"
ClassImp(HRichPCASim)

HRichPCASim::HRichPCASim(const Text_t *name,const Text_t *title) :
  HReconstructor(name,title)
{

}

HRichPCASim::HRichPCASim()
{

}

HRichPCASim::HRichPCASim(const Text_t *name,const Text_t *title, const Char_t* filename) :
  HReconstructor(name,title)
{
    pFile  = new TFile(filename,"RECREATE");
    // output file for diagnostic histos
}

HRichPCASim::~HRichPCASim(void) {

}


Bool_t HRichPCASim::init() {
        if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
	    HDetector *rich = spec->getDetector("Rich");
	    
	    if (rich) { //Has the user set up a RICH?

		pHitMatchCat=event->getCategory(catMatchHit);
		if (!pHitMatchCat) 
		{
			Error("init","No HIT MATCH category defined");
			return kFALSE;
		}
				
		pIterMatchHit = (HIterator*)getHitMatchCat()->MakeIterator("native");
		

	    }
	}
	nNbVariables = 7;// number of properties
	principal = new TPrincipal(nNbVariables,"ND");
	pFile = new TFile("pcasimh.root","RECREATE");
	h3 = new TH3D("h3","h3",100,0,1500,100,0,100,20,0,20);
	}
	return kTRUE;

}

Int_t HRichPCASim::execute()
{
    HHitMatchSim *h=0;
    pIterMatchHit->Reset();
    Int_t nEnt = (getHitMatchCat())->getEntries();
    Int_t *nRichInd = new Int_t[nEnt];
    for (Int_t i=0;i<nEnt;i++) nRichInd[i]=-2;
    while(( h= (HHitMatchSim *)pIterMatchHit->Next())) 
    {
	Int_t richind = h->getRichInd();
	//h->dumpToStdout();
	
	// CUTS on Tracks
	if (HRichCut::isNewIndex(richind,nRichInd,nEnt) &&
	    HRichCut::isFullRichMdcMetaTracklet(h) &&
	    HRichCut::isRMThetaDiff(h,2.)               ) 

	{
	    //h->dumpToStdout();
	    //	    cout<<"rich index: "<<richind<<endl;
	    Double_t * data = new Double_t[nNbVariables];
//    	    data[7] = h->getRichTheta();
//    	    data[8] = h->getRichPhi();
//    	    data[9] = h->getMdcTheta();
//    	    data[10] = h->getMdcPhi();
	    data[1] = h->getRingAmplitude();
	    data[2] = h->getRingPadNr();
	    data[0] = h->getRingLocalMax4();
  	    data[3] = h->getRingPatMat();
  	    data[4] = h->getRingHouTra();
  	    data[5] = h->getRadius();
  	    data[6]= h->getMdcClusterSize();
//  	    for (Int_t k=0;k<nNbVariables;k++)
//  	    {
//  		cout<<data[k]<<" ";
//  	    }
//  	    cout<<endl;
	    h3->Fill(data[0],data[1],data[2]);
//    	    data[7]  = 3.3; 
//    	    data[8]  = 2.2;
//    	    data[9]  = 1.1;
//    	    data[10] = 0.75;
//  	    data[0]  = 6.6;
//  	    data[1]  = 4.4;
//  	    data[2]  = 3.21;
//  	    data[3]  = 3.14;
//  	    data[4]  = 25.5678;
//  	    data[5]  = 676.345345;
//  	    data[6]  = 2.;
	    //	    data[11]= h->getMdcClusterHit();
	    principal->AddRow(data);
	    delete [] data;
	}
    }
    delete [] nRichInd;
    //    cout<<"*** end of evt ***"<<endl;
    return kTRUE;
    // end of execute function
}

Bool_t HRichPCASim::finalize() {
    principal->MakePrincipals();
    principal->Print("MSEV");
    //principal->Test();
    principal->MakeHistograms();
    TList *listofhistos    = principal->GetHistograms();
    TVectorD eigenvalues((TVectorD)(*principal->GetEigenValues()));
    TMatrixD covmatrix((TMatrixD)(*principal->GetCovarianceMatrix()));
    TVectorD meanvalues((TVectorD)(*principal->GetMeanValues()));
    TMatrixD eigenvectors((TMatrixD)(*principal->GetEigenVectors()));
    TVectorD sigmas((TVectorD)(*principal->GetSigmas()));
    TVectorD userdata((TVectorD)(*principal->GetUserData()));
    pFile->cd();
    h3->Write();
    TH1 *h=0;
    TIter next(listofhistos);
    next.Reset();
    while (( h = (TH1 *) next() )) h->Write();
    eigenvalues.Write();
    covmatrix.Write();
    meanvalues.Write();
    eigenvectors.Write();
    sigmas.Write();
    userdata.Write();
    pFile->Close(); 
    principal->MakeCode();
    //      TBrowser* b = new TBrowser("principalBrowser", principal);
    //delete principal;
    return kTRUE;
}

