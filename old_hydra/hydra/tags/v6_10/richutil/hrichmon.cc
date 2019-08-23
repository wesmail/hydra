// File: hrichmon.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/05/16 15:10:06
//
#include "hrichmon.h"
#include "hrichraw.h"
#include "hrichhit.h"
#include "hdihitmatch.h"
#include "hconstant.h"
#include "hevent.h"
#include "hrootsource.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include <iostream.h>
#include <stdio.h>
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hlinearcategory.h"
#include <TObjArray.h>
#include <TLatex.h>
#include <TVector.h>
#include <TMatrix.h>
#include <TH1.h>
#include <TTRAP.h>
#include <TBRIK.h>
#include <TTUBS.h>
#include <TRotMatrix.h>
#include <TStopwatch.h>
#include <TArc.h>
ClassImp(HRichMon)

HRichMon::HRichMon(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{

}

HRichMon::HRichMon()
{

}

HRichMon::HRichMon(Text_t *name,Text_t *title, char *swt, char* geofilename) :
  HReconstructor(name,title)
{
    // geometry file with all pad corners from CAD
    pFile  = new TFile(geofilename,"READ");
    // switch to connect cntainers
    pCatSwitch = new TString(swt);
    // zoom factor for padplanes
    zoomFactor=1.5;
    nEvtsProcessed=0;
}

HRichMon::~HRichMon(void) {

}


Bool_t HRichMon::init() {
    // What about hit  and sim ??? can all be done here ?!
    if (gHades) {
	HEvent *event=gHades->getCurrentEvent();
	HRuntimeDb *rtdb=gHades->getRuntimeDb();
	HSpectrometer *spec=gHades->getSetup();
	if (event && rtdb) {
	    HDetector *rich = spec->getDetector("Rich");
	    
	    if (rich) { //Has the user set up a RICH?
		
		if (pCatSwitch->Contains("RAW"))
		{
		    pCat=gHades->getCurrentEvent()->getCategory(catRichRaw);
		}
		else if (pCatSwitch->Contains("CAL"))
		{
		    pCat=gHades->getCurrentEvent()->getCategory(catRichCal);
		}
		
		if (!pCat) 
		{
		    Error("init","no input category found");
		    return kFALSE;
		}
		pIter = (HIterator*)pCat->MakeIterator("native");


		if (pCatSwitch->Contains("HIT"))
		{
		    pHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
		    if (!pHitCat) {
			Error("init","no hit input category found");
			return kFALSE;
		    }
		    pHitIter = (HIterator*)pHitCat->MakeIterator();
		}

		if (pCatSwitch->Contains("PAIR"))
		{
		    pPairCat=gHades->getCurrentEvent()->getCategory(catDiMatchHit);
		    if (!pPairCat) {
			Error("init","no pair input category found");
			return kFALSE;
		    }
		    pPairIter = (HIterator*)pPairCat->MakeIterator();
		}

	    }
	}
    }
    initGeoData(); //read geom file
    initWindow();  //draw detector
    el = new TEventList("rings","rings",10);
    evtnumbertext = new TText(0.4,-0.97,"Event: ");
    return kTRUE;
    
}

Int_t HRichMon::execute()
{
    nEvtsProcessed++;
    //    cout<<nEvtsProcessed<<endl;
    // select events to be displayed
    if (selectEvt())
    {
    
	

	// reset view, prepare for new event
	makePadsInvisible();

	// show all fired pads in event with their ADC val color-coded
	showFiredPads();
	makeEvtAnnotation();
	if (pCatSwitch->Contains("HIT")) makeHitAnnotation();

	// redraw window
	updateWindow(); 

	// create EPS and GIF of window
	//printWindow();
	
	// wait for input of a digit before next event    
	//      Int_t num;
	//      scanf("%d",&num);//enter integer
    }
    return kTRUE;
    // end of execute function
}

Bool_t HRichMon::finalize() {
    TFile ff("evtlist.root","RECREATE");
    ff.cd();
    el->Write();
    ff.Close();
    delete el;
    return kTRUE;
}

void HRichMon::initGeoData(){
    nPads = 4894;
    nSec = 6;
    memCharges = new (Float_t)[nSec*nPads+1];
    resetMemCharges();
    Float_t dz     = 0.5;
    TFile *geo = pFile;
    if (!geo){
	cout<<"Error opening geometry file"<<endl;
    }else{
	c1x = (TVector*) geo->Get("corner1x");
	c1y = (TVector*) geo->Get("corner1y");
	c2x = (TVector*) geo->Get("corner2x");
	c2y = (TVector*) geo->Get("corner2y");
	c3x = (TVector*) geo->Get("corner3x");
	c3y = (TVector*) geo->Get("corner3y");
	c4x = (TVector*) geo->Get("corner4x");
	c4y = (TVector*) geo->Get("corner4y");
	mapTable = (TMatrix*) geo->Get("mat");
	geo->Close();
    }
    TTRAP ** trap;
    trap = new TTRAP* [nPads*nSec];
    
    Char_t name1[20];
    Char_t title1[20];
    Char_t name2[20];
    Char_t title2[20];
    
    for (Int_t i=0; i < nPads; i++) { //create trapezoids
	Float_t tmp[8];
	tmp[0] = h1((*c1x)(i),(*c2x)(i),(*c3x)(i),(*c4x)(i));
	tmp[1] = bl1((*c2y)(i),(*c3y)(i));
	tmp[2] = tl1((*c1y)(i),(*c4y)(i));
	tmp[3] = alpha1((*c1x)(i),(*c2x)(i),(*c1y)(i),(*c2y)(i),(*c3y)(i),(*c4y)(i));
	tmp[4] = h2((*c1x)(i),(*c2x)(i),(*c3x)(i),(*c4x)(i));
	tmp[5] = bl2((*c2y)(i),(*c3y)(i));
	tmp[6] = tl2((*c1y)(i),(*c4y)(i));
	tmp[7] = alpha2((*c1x)(i),(*c2x)(i),(*c1y)(i),(*c2y)(i),(*c3y)(i),(*c4y)(i));
       
	for (Int_t k=0; k<nSec; k++){
	    sprintf (title1,"TRAP_%d_%d",i,k);
	    sprintf (name1, "TRAP_%d_%d",i,k);
	    
	    trap[(k*nPads)+i] = new TTRAP(name1,title1,"void",dz,0,0,tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7]);
	    trap[(k*nPads)+i] -> SetLineColor(k+2);
	}
    }


    TBRIK* brik0 = new TBRIK("MOTHER","MOTHER","void",0.,0.,0.);
    brik0->SetVisibility(0);
    
    Int_t noHelperVol = 2*nSec;
    node = new TNode* [nSec*nPads+noHelperVol+1];
    

    node[0] = new TNode("NODE_0","NODE_0","MOTHER");
    node[0]->cd();
    node[0]->SetVisibility(-4);
    
    TRotMatrix ** rotmat;
    rotmat = new TRotMatrix* [7];
    rotmat[0] = new TRotMatrix("rot0","rot0",90.,90.,90.,180.,0.,0.);
    rotmat[1] = new TRotMatrix("rot1","rot1",90.,150.,90.,240.,0.,0.);
    rotmat[2] = new TRotMatrix("rot2","rot2",90.,210.,90.,300.,0.,0.);
    rotmat[3] = new TRotMatrix("rot3","rot3",90.,270.,90.,0.,0.,0.);
    rotmat[4] = new TRotMatrix("rot4","rot4",90.,330.,90.,60.,0.,0.);
    rotmat[5] = new TRotMatrix("rot5","rot5",90.,30.,90.,120.,0.,0.);
    rotmat[6] = new TRotMatrix("rot6","rot6",110.,0.,90.,90.,20.,0.); // for 3d-view

    
    Char_t cSecNodeName[20];
    Char_t cRotMatName[5];
    
    for (Int_t k=0; k<nSec; k++){
	sprintf (cSecNodeName,"NODE_%d",nSec*nPads+k+1);
	sprintf (cRotMatName,"rot%d",k);
	node[nSec*nPads+k+1] = new TNode(cSecNodeName,cSecNodeName,"MOTHER",0.,0.,0.,cRotMatName);//  ????????????
    }
    
    
    TTUBS* tubs = new TTUBS("tubs","tubs","void",6.,65.,0.,-28.,28.);
    tubs->SetLineColor(0);
    
    for (Int_t k=0; k<nSec; k++){
	node[nSec*nPads+k+1] -> cd();
	sprintf (cSecNodeName,"NODE_%d",nSec*nPads+nSec+k+1);
	node[nSec*nPads+nSec+k+1] = new TNode(cSecNodeName,cSecNodeName,"tubs",0.,0.,0.,"rot6");
    }
    
    for ( Int_t k=0; k < nSec; k++ ) {
	node[nSec*nPads+k+nSec+1] -> cd();
	for (Int_t i=0; i < nPads; i++) {
	    sprintf (title2,"NODE_%d_%d",i+1,k+1);
	    sprintf (name2, "NODE_%d_%d",i+1,k+1);
	    sprintf (name1, "TRAP_%d_%d",i,k);
	    Float_t tmp1[2];
	    tmp1[0] = yd((*c1y)(i),(*c2y)(i),(*c3y)(i),(*c4y)(i));
	    tmp1[1] = xd((*c1x)(i),(*c2x)(i),(*c3x)(i),(*c4x)(i));
	    node[(k*nPads)+(i+1)] = new TNode(name2,title2,name1,tmp1[0],tmp1[1],0.);
	    node[(k*nPads)+(i+1)] -> SetVisibility(0);
	}
	
    }
       
}
void HRichMon::resetMemCharges(){
    for (int i=0;i<nSec*nPads+1;i++){
	memCharges[i] = 0;
    }
}
void HRichMon::makePadsInvisible(){
    for (Int_t i=0;i<nPads*nSec;i++){
	node[i+1] -> SetVisibility(0);
    }
}
void HRichMon::initWindow(){
    window = new TCanvas("RICH_Online_Monitor","RICH_Online_Monitor",500,550);
    window->Draw("x3d");
    window -> SetFillColor(10);
    window -> cd();
    Int_t linecolor=13;
    Int_t linestyle=3;

    Int_t sectornode0=29371;
    //  cout<<"Node 0"<<endl;
//      cout<<node[0] -> GetX()<<" "<<
//  	node[0] -> GetY()<<" "<<
//  	node[0] -> GetZ()<<" "<<endl;
//      cout<<" Sector nodes :"<<endl;
    for (Int_t sec=0;sec<6;sec++){
	node[sectornode0+sec] -> SetLineColor(linecolor);
	node[sectornode0+sec] -> SetLineStyle(linestyle);
//  	cout<<node[sectornode0+sec] -> GetX()<<" "<<
//  		node[sectornode0+sec] -> GetY()<<" "<<
//  		node[sectornode0+sec] -> GetZ()<<" "<<endl;
    }

    node[0] -> Draw();
    viewer = (TView *)  gPad->GetView();
    viewer->SetRange(-65,-65,-65,65,65,65);
    //    viewer->ShowAxis();
    viewer->Top();
    //rotateDetectorFancy();

    viewer-> RotateView(90,180);
    viewer-> ZoomView(0,zoomFactor);
    cout<<"We look in beam direction"<<endl;
    TArc *border = new TArc(0.,0.,.55*zoomFactor,0,360);
    border->SetLineWidth(2);
    border->SetLineColor(linecolor);
    border->Draw();
    window -> Modified();
    window -> Draw();
    window -> Update();
}
void HRichMon::rotateDetectorFancy()
{
    Int_t index=0;
    for (Int_t sec=0;sec<6;sec++){
	for (Int_t i=10;i<90;i++){
	    for (Int_t j=10;j<90;j++){
	    
		index = (Int_t) ( (*mapTable)( i,j ) 
			      + (sec * nPads) );
		node[index] -> SetLineColor(sec+2);
		node[index] -> SetLineWidth(1);
		node[index] -> SetVisibility(1);
		i+=10;
		j+=10;
	    }
	}
    }

    Int_t angle=0;
    while (angle<=180)
    {
	viewer-> RotateView(angle*2,angle);
	updateWindow();
	angle+=5;
    }
}


Bool_t HRichMon::testHits()
{
    Bool_t kTEST = kFALSE;
    //select evt or skip it

    HRichHit *r=0;
    pHitIter->Reset();
    Int_t hitcnt=0;//count hits (rings)
    // loop over found rings
    while((r = (HRichHit *)pHitIter->Next()))
    {
	hitcnt++;
	if (isGoodRing(r)) kTEST = kTRUE;
    }

    // there are at least two rings and one has a min quality
    if (hitcnt>=2 && kTEST) kTEST = kTRUE;
    else kTEST = kFALSE;
    
    
    return kTEST;
    
}

Bool_t HRichMon::isGoodRing(HRichHit *r)
{
     Bool_t kTEST = kFALSE;

    Int_t nPatMatCut = 500;
    Int_t nHouTraCut = 200;
    Int_t nRingAmplCut = 500;
    Int_t nPadNrCut = 8;
    Int_t nLocMax4Cut = 6;


	if(r->getRingPatMat() >= nPatMatCut &&
	   r->getRingHouTra()>= nHouTraCut &&
	   r->getRingAmplitude() >= nRingAmplCut &&
	   r->getRingPadNr() >= nPadNrCut &&
	   r->getRingLocalMax4()>= nLocMax4Cut) 
	{
	    kTEST = kTRUE;
	    
	}
    
    return kTEST;
}

Bool_t HRichMon::testPairs()
{
    Bool_t kTEST = kFALSE;
    HDiHitMatch *h=0;
    pPairIter->Reset();
    while(( h= (HDiHitMatch *)pPairIter->Next())) 
    {
	
	if (h->getOpangKICK()> 10.) 
	{
	    //	    cout<<h->getOpangKICK()<<endl;
	    kTEST=kTRUE;

	}

    }

    return kTEST;
}


void HRichMon::printWindow()
{
    TString base("Event_");
    base+=nEvtsProcessed;
    TString eps = base;
    TString gif = base;
    eps+=".eps";
    gif+=".gif";
    window->Print(eps.Data(),"eps");
    window->Print(gif.Data(),"gif");

}
void HRichMon::makeEvtAnnotation()
{
    TString a("Event: ");
    a+=nEvtsProcessed;
    evtnumbertext->SetText(0.4,-0.97,a.Data());
    evtnumbertext->Draw();
}
void HRichMon::makeHitAnnotation()
{

    Float_t deg2rad=1./HConstant::rad2deg();
    // padplane is tilted by 20 degrees w/ respect to lab x-y-plane
    Float_t proj20 = TMath::Cos(20.*deg2rad);
    // radius for red circle around found ring scales with det. view
    // 0.06 is value for ring in NDC coords (0,0 in center, normalized to 1)
    Float_t circrad=.06 * zoomFactor;
    Float_t offset=0.04;// padplane shift from center !
    TArc *circ=0;//circle around ring
    TLatex *hittext=0;//text indicating hit coords
    Int_t linecolor = 2; //red ! mark hit with this color

    Float_t zoomUnit = 0.55;//corresponds to a zoomfactor of 1.0 
    Int_t degPerSec = 60;
    

    TObjArray circarr(3);
    TObjArray stringarr(3);
    Int_t hitcnt=0;//count hits (rings)

    // loop over found rings
    HRichHit *h=0;
    pHitIter->Reset();
    while((h = (HRichHit *)pHitIter->Next()))
    {
	if (isGoodRing(h)) //mark only selected rings
	{
	    Int_t hitnodeind = (Int_t) ( (*mapTable)( h->getRingCenterX(),
						      h->getRingCenterY() ) 
					 + (h->getSector() * nPads) );
	    //draw red point at the position of ring center
	    //set visibility to 1
	    node[hitnodeind] -> SetVisibility(0);
	    node[hitnodeind] -> SetLineColor(linecolor);
	    node[hitnodeind] -> SetLineWidth(3);
	    
	    hitcnt++;
	    Int_t nSec=h->getSector();
	    if (nSec==0) nSec=6;//plane polar coords go anti-clockwise
	    nSec=(nSec-1)*degPerSec;//one sector covers 60 degrees

	    // distance r from center for polar coords
	    // 0.55 corresponds to a zoomfactor of 1.0 

	    Float_t r = zoomUnit*zoomFactor*(h->getTheta()/90. + 
					     offset*zoomFactor)*proj20; 

	    Float_t phi = 180. - h->getPhi();
	    if (phi<0) phi = 360. + phi;
	    phi = phi*deg2rad;
	    
	    // create red circle around selected ring
	    circ = new TArc(r*TMath::Cos(phi),r*TMath::Sin(phi),circrad,0,360);
	    circ->SetLineWidth(2);
	    circ->SetLineColor(linecolor);
	    circ->Draw();
	    circarr.Add(circ);

	    // write ring property annotation on top
	    TString hitstring("hit ");
	    hitstring+=hitcnt;
	    hitstring+=":  ";
	    hitstring+=h->getSector();
	    hitstring+=",";
	    hitstring+=h->getRingCenterX();
	    hitstring+=",";
	    hitstring+=h->getRingCenterY();
	    hitstring+=" *** P:";
	    hitstring+=h->getRingPatMat();
	    hitstring+=",H:";
	    hitstring+=h->getRingHouTra();
	    hitstring+=",A:";
	    hitstring+=h->getRingAmplitude();
	    hitstring+=",N:";
	    hitstring+=h->getRingPadNr();
	    hitstring+=",M:";
	    hitstring+=h->getRingLocalMax4();

	    hittext = new TLatex(-0.95,0.925-(hitcnt-1)*0.06,
				 hitstring.Data());
	    hittext->SetTextSize(0.0278884);
	    hittext->SetLineWidth(2);
	    stringarr.Add(hittext);
	    hittext->Draw();
	}//end if isGoodRing
	 
    }//end while over hit cat
    
    updateWindow();
    // delete annotations before next event
    delayUpdate(1.);
    for (Int_t i=0;i<(circarr.GetLast()+1);i++)
	delete (TArc*)circarr[i];
    for (Int_t i=0;i<(stringarr.GetLast()+1);i++)
	delete (TLatex*)stringarr[i];
}


void HRichMon::showFiredPads()
{

    // loop over raw or cal container
    HRichRaw *pObj =NULL;
    pIter->Reset();
    Int_t index = 0;
    while((pObj = (HRichRaw *)pIter->Next())) 
    {
	// use look-up table from geom file to find 
	// pad in detector display
	index = (Int_t) ( (*mapTable)( pObj->getCol(),pObj->getRow() ) 
			  + (pObj->getSector() * nPads) );
	// make it visible and set color according to ADC value
	node[index] -> SetVisibility(1);
	Int_t col = TMath::Nint( pObj->getCharge()*0.5 + 50);
	if (col>100) col=99;
	node[index] -> SetLineColor(col);
	node[index] -> SetLineWidth(1);
	
    }
}


void HRichMon::updateWindow()
{
    // redraw window content
    
    window->cd();
    window -> Modified();
    window -> Update();
    
}
void HRichMon::delayUpdate(Float_t secs)
{
    // how many secs an event should be displayed
    TStopwatch w;

    //wait until event was shown  secsforevt real-time secs
    while (w.RealTime()<secs) w.Continue();
	
	// wait for input of a digit before next event    
	//      Int_t num;
	//      scanf("%d",&num);//enter integer

}
Bool_t HRichMon::selectEvt()
{

    Bool_t kTEST = kFALSE;
    if (pCatSwitch->Contains("HIT") && pCatSwitch->Contains("PAIR"))
    {
	if ( testHits() && testPairs() ) kTEST=kTRUE;
    }
    if (kTEST && pCatSwitch->Contains("CAL") ) 
    {
	el->Enter(nEvtsProcessed-1);//account for start event
    }
    return kTEST;
}