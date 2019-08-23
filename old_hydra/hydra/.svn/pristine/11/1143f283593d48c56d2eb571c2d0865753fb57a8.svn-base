// $Id: hrichpca.cc,v 1.6 2008-05-09 16:16:33 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:43:21
//
using namespace std;
#include "hrichpca.h"
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
#include "hhitmatch.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TH1.h>
ClassImp(HRichPCA)

HRichPCA::HRichPCA(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{

}

HRichPCA::HRichPCA()
{

}

HRichPCA::HRichPCA(Text_t *name,Text_t *title, Char_t* filename) :
  HReconstructor(name,title)
{
    pFile  = new TFile(filename,"RECREATE");
    // output file for diagnostic histos
}

HRichPCA::~HRichPCA(void) {

}


Bool_t HRichPCA::init() {
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
	nNbVariables = 6;// number of properties
	principal = new TPrincipal(nNbVariables,"ND");
	pFile = new TFile("pcah.root","RECREATE");
	h3 = new TH3D("h3","h3",100,0,1500,100,0,100,20,0,20);
	h3t = new TH3D("h3t","h3t",40,-2,2,100,-5,5,200,-10,10);
	h2 = new TH2D("h2","h2",100,0,1500,100,0,100);
	}
	return kTRUE;

}

Int_t HRichPCA::execute()
{
    HHitMatch *h=0;
    pIterMatchHit->Reset();
    Int_t nEnt = (getHitMatchCat())->getEntries();
    Int_t *nRichInd = new Int_t[nEnt];
    for (Int_t i=0;i<nEnt;i++) nRichInd[i]=-2;
    while(( h= (HHitMatch *)pIterMatchHit->Next())) 
    {
	Int_t richind = h->getRichInd();
	//h->dumpToStdout();
	
	// CUTS on Tracks
	if (HRichCut::isNewIndex(richind,nRichInd,nEnt) &&
	    HRichCut::isFullRichMdcMetaTracklet(h) &&
	    HRichCut::isRMThetaDiff(h,2.)               ) 

	{
	    //cout<<"rich index: "<<richind<<endl;
	    Double_t * data = new Double_t[nNbVariables];
//  	    data[0] = h->getRichTheta();
//  	    data[1] = h->getRichPhi();
//  	    data[2] = h->getMdcTheta();
//  	    data[3] = h->getMdcPhi();
	    data[2] = h->getRingAmplitude();
	    data[1] = h->getRingPadNr();
	    data[0] = h->getRingLocalMax4();
  	    data[3] = h->getRingPatMat();
  	    data[4] = h->getRingHouTra();
    	    data[5] = h->getRadius();
	    //    	    data[6]= h->getMdcClusterSize();
	    //	    data[11]= h->getMdcClusterHit();
//    	    for (Int_t k=0;k<nNbVariables;k++)
//    	    {
//    		cout<<data[k]<<" ";
//    	    }
//    	    cout<<endl;

	    h3->Fill(data[0],data[1],data[2]);
	    principal->AddRow(data);
	    delete [] data;
	}
    }
    delete [] nRichInd;
    //cout<<"*** end of evt ***"<<endl;
    return kTRUE;
    // end of execute function
}

Bool_t HRichPCA::finalize() {
    principal->MakePrincipals();
    principal->Print("MSVE");
    //principal->Test();
    principal->MakeHistograms();
    TList *listofhistos    = principal->GetHistograms();
    TVectorD eigenvalues((TVectorD)(*principal->GetEigenValues()));
    TMatrixD covmatrix((TMatrixD)(*principal->GetCovarianceMatrix()));
    TVectorD meanvalues((TVectorD)(*principal->GetMeanValues()));
    TMatrixD eigenvectors((TMatrixD)(*principal->GetEigenVectors()));
    TVectorD sigmas((TVectorD)(*principal->GetSigmas()));
    TVectorD userdata((TVectorD)(*principal->GetUserData()));
    //Int_t k=0; // unused
    //const Double_t* usrdata=0; // unused
    Double_t * out = new Double_t[nNbVariables];
    Double_t *out2 = new Double_t[nNbVariables-1];
//      while ((usrdata=principal->GetRow(k)))
//      {
//  	k++;
//  	principal->X2P(usrdata,out);
//  	h3t->Fill(out[0],out[1],out[2]);
//  	principal->P2X(out,out2,nNbVariables-1);
//  	cout<<k-1<<"  "<<TMath::Abs(out2[0]-usrdata[0])/usrdata[0]
//  	         <<"  "<<TMath::Abs(out2[1]-usrdata[1])/usrdata[1]<<endl;
//  	h2->Fill(out2[0],out2[1]);
	
//      }
    delete [] out;
    delete [] out2;
    //    cout<<"bis hierher 1"<<endl;
    pFile->cd();
    //    cout<<"bis hierher 2"<<endl;
    
      TH1 *h=0;
      TIter next(listofhistos);
      //    cout<<"bis hierher 3"<<endl;
      next.Reset();
      //    cout<<"bis hierher 4"<<endl;

      while (( h = (TH1 *) next() )) h->Write();
//      eigenvalues.Write();
//      covmatrix.Write();
//      meanvalues.Write();
//      eigenvectors.Write();
//      sigmas.Write();
//      userdata.Write();
//      h2->Write();
//      h3->Write();
//      h3t->Write();
    //  h2->DrawCopy();
//      h3->DrawCopy();
//      h3t->DrawCopy();
    pFile->Close(); 
//      principal->MakeCode();
    //    TBrowser* b = new TBrowser("principalBrowser", principal);
    //delete principal;
    return kTRUE;
}

