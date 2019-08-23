// $Id: hrichmon.cc,v 1.7 2002-12-12 04:08:05 eberl Exp $
// Last update by Thomas Eberl: 02/12/12 04:54:23
//
#include "hrichmon.h"
#include "hrichgeometrypar.h"
#include "hruntimedb.h"
#include "hrichpad.h"
#include "hrichraw.h"
#include "hrichhit.h"
#include "hhitmatch.h"
#include "hkicktrack.h"
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
#include "hrichutilfunc.h"
#include "hades.h"
#include "richdef.h"
#include "kickdef.h"
#include "hlinearcategory.h"
#include <TObjArray.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TVector.h>
#include <TMatrix.h>
#include <TH1.h>
#include <TTRAP.h>
#include <TBRIK.h>
#include <TTUBS.h>
#include <TRotMatrix.h>
#include <TStopwatch.h>
#include <TDatime.h>
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
    // GEOMETRY PARAMETER CONTAINER
	HRichGeometryPar *pGeomPar = 
	    (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
	setGeometryPar(pGeomPar);
	HRichPadTab *pPadsPar = ((HRichGeometryPar*)getGeometryPar())
	->getPadsPar();
	setPadTable(pPadsPar);
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

		if (pCatSwitch->Contains("LEPTON"))
		{
		    pMatchCat=gHades->getCurrentEvent()->getCategory(catMatchHit);
		    if (!pMatchCat) {
			Error("init","no track input category found");
			return kFALSE;
		    }
		    pMatchIter = (HIterator*)pMatchCat->MakeIterator();
		}

		if (pCatSwitch->Contains("TRACK"))
		{
		
		    pKickCat = gHades->getCurrentEvent()->getCategory(catKickTrack);
		    if (!pKickCat) {
			Error("init","KickTrack not found in input file");
			return kFALSE;
		    }
		    pKickIter = (HIterator *)pKickCat->MakeIterator("native");
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
    TDatime dt;
    dt.Set();
    datetext = new TText(-.97,-.97,dt.AsString());
    datetext->SetTextSize(0.05);
    //    cout<<dt.AsString()<<endl;
    return kTRUE;
    
}

Int_t HRichMon::execute()
{
    nEvtsProcessed++;
    // store pointers to annotation objects
    TObjArray circarr(5);
    TObjArray stringarr(5);
    //    cout<<nEvtsProcessed<<endl;
    // select events to be displayed
    if (selectEvt())
    {
    
	

	// reset view, prepare for new event
	makePadsInvisible();

	// show all fired pads in event with their ADC val color-coded
	showFiredPads();
	makeEvtAnnotation();
	if (pCatSwitch->Contains("HIT")) makeHitAnnotation(circarr,stringarr);
	if (pCatSwitch->Contains("TRACK")) makeTrackAnnotation(circarr,stringarr);
	if (pCatSwitch->Contains("LEPTON")) makeLepTrackAnnotation(circarr,stringarr);

	// redraw window
	updateWindow(); 
	delayUpdate(2.);
	// create EPS and GIF of window
	if (pCatSwitch->Contains("PRINT")) printWindow();
	
    }

    if (pCatSwitch->Contains("TERM")) cout<<nEvtsProcessed<<"---------------------------------------------"<<endl; 
    //waitOnKey();
    deleteAnnotation(circarr,stringarr);
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
	    
	    trap[(k*nPads)+i] = new TTRAP(name1,title1,"void",dz,0,0,tmp[0],tmp[1],tmp[2],
					  tmp[3],tmp[4],tmp[5],tmp[6],tmp[7]);
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
    //gStyle->SetOptDate(1);
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

	if (isGoodRing(r)) 
	{
	    hitcnt++;
	    kTEST = kTRUE;
	}
    }

    // there are at least two rings and one has a min quality
    if (hitcnt>=2) kTEST = kTRUE;
    else kTEST = kFALSE;
    
    
    return kTEST;
    
}

Bool_t HRichMon::isGoodRing(HRichHit *r)
{
     Bool_t kTEST = kFALSE;

    Int_t nPatMatCut = 250;
    Int_t nHouTraCut = 30;
    Int_t nRingAmplCut = 300;
    Int_t nPadNrCut = 6;
    Int_t nLocMax4Cut = 1;


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
    //    window->Print(eps.Data(),"eps");
    window->Print("/u/hrich/web-docs/online.gif","gif");

}
void HRichMon::makeEvtAnnotation()
{
    TString a("Event: ");
    a+=nEvtsProcessed;
    evtnumbertext->SetText(0.4,-0.97,a.Data());
    evtnumbertext->Draw();
    TDatime dt;
    dt.Set();
    TString d(dt.AsString());
    datetext->SetText(-0.97,-.97,d.Data());
    //    cout<<d.Data()<<endl;
    datetext->Draw();
}
void HRichMon::makeHitAnnotation(TObjArray &c,TObjArray &s)
{
    // this function draws a red circle around the determined ring center
    //
    //
    //
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
    

//     TObjArray circarr(3);
//     TObjArray stringarr(3);
    Int_t hitcnt=0;//count hits (rings)
    Int_t hitcnt2=0;//count hits (rings)
    // loop over found rings
    HRichHit *h=0;
    pHitIter->Reset();
    while((h = (HRichHit *)pHitIter->Next()))
    {
	//	if (isGoodRing(h)) //mark only selected rings
	if(kTRUE)
	{
	    //	    h->dumpToStdout();
	    Int_t hitnodeind = (Int_t) ( (*mapTable)( h->getRingCenterX(),
						      h->getRingCenterY() ) 
					 + (h->getSector() * nPads) );
	    //draw red point at the position of ring center
	    //set visibility to 1
	    node[hitnodeind] -> SetVisibility(0);
	    node[hitnodeind] -> SetLineColor(linecolor);
	    node[hitnodeind] -> SetLineWidth(3);
	    

	    hitcnt=s.GetLast()+1;
	    hitcnt++;
	    hitcnt2++;

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
	    c.Add(circ);

	    // write ring property annotation on top
	    TString hitstring("hit ");
	    hitstring+=hitcnt2;
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

	    if (pCatSwitch->Contains("TERM"))
	    {
		cout<<hitstring.Data()<<endl;
	    }
	    else
	    {
		hittext = new TLatex(-0.95,0.925-(hitcnt-1)*0.06,
				     hitstring.Data());
		hittext->SetTextSize(0.0278884);
		hittext->SetLineWidth(2);
		s.Add(hittext);
		hittext->Draw();
	    }

	}//end if isGoodRing
	 
    }//end while over hit cat

    //    updateWindow();
    // delete annotations before next event
    //delayUpdate(1.);
    // wait for input of a digit before next event    
    Int_t num;
//     cout<<"enter a digit:";
//     scanf("%d",&num);//enter integer

}
void HRichMon::makeLepTrackAnnotation(TObjArray &c,TObjArray &s)
{
    // this function draws a pink circle around the track entry point 
    //
    //
    //
    Float_t deg2rad=1./HConstant::rad2deg();
    // padplane is tilted by 20 degrees w/ respect to lab x-y-plane
    Float_t proj20 = TMath::Cos(20.*deg2rad);
    // radius for red circle around found ring scales with det. view
    // 0.06 is value for ring in NDC coords (0,0 in center, normalized to 1)
    Float_t circrad=.02 * zoomFactor;
    Float_t offset=0.04;// padplane shift from center !
    TArc *circ=0;//circle around ring
    TLatex *hittext=0;//text indicating hit coords
    Int_t linecolor = 6; //pink ! mark hit with this color

    Float_t zoomUnit = 0.55;//corresponds to a zoomfactor of 1.0 
    Int_t degPerSec = 60;
    

//     TObjArray circarr(3);
//     TObjArray stringarr(3);
    Int_t hitcnt=0;//count hits (tracks)

    // loop over found tracks
    Int_t nTrkCnt = 0;
    HHitMatch *h=0;
    pMatchIter->Reset();
    while((h = (HHitMatch *)pMatchIter->Next()))
    {
	if(kTRUE)
	{
	    nTrkCnt++;
	    // we have a (theta,phi) coord from the kick track
	    // and need to find the pad which would be the 
	    // center of the ring formed by the lepton 
	    // that caused the track.
// 	    Int_t sec =  h->getSector();
// 	    if (sec==5) sec=-1;
// 	    Float_t offset = (sec+1)*degPerSec;
// 	    Float_t tphi = h->getMdcPhi() - offset;
// 	    cout<<"kick track phi: "<<tphi<<endl;
	    Float_t tphi   = h->getMdcPhi();
	    Float_t ttheta = h->getMdcTheta();
	    //cout<<ttheta<<" "<<tphi<<endl;
	    Int_t padX=0;
	    Int_t padY=0;
	    getPadXYforThetaPhi(ttheta,tphi,padX,padY,h->getSector());
	    //cout<<"padX: "<<padX<<"  padY: "<<padY<<endl;
	    Int_t hitnodeind = 0;
	    if (padX>-1 && padY>-1)
	    {
		hitnodeind = (Int_t) ( (*mapTable)( padX,
						    padY ) 
				       + (h->getSector() * nPads) );
	    }
	    if (hitnodeind==0) continue;
	    //draw point at the position of track
	    //set visibility to 1
	    node[hitnodeind] -> SetVisibility(0);
	    node[hitnodeind] -> SetLineColor(linecolor);
	    node[hitnodeind] -> SetLineWidth(3);

	    hitcnt=s.GetLast()+1;
	    hitcnt++;
	    
	    Int_t nSec=h->getSector();
	    if (nSec==0) nSec=6;//plane azimuthal coords go anti-clockwise
	    nSec=(nSec-1)*degPerSec;//one sector covers 60 degrees

	    // distance r from center for polar coords
	    // 0.55 corresponds to a zoomfactor of 1.0 

	    Float_t r = zoomUnit*zoomFactor*(h->getMdcTheta()/90. + 
					     offset*zoomFactor)*proj20; 

	    Float_t phi = 180. - h->getMdcPhi();
	    if (phi<0) phi = 360. + phi;
	    phi = phi*deg2rad;
	    
	    // create pink circle around selected track
	    circ = new TArc(r*TMath::Cos(phi),r*TMath::Sin(phi),circrad,0,360);
	    circ->SetLineWidth(2);
	    circ->SetLineColor(linecolor);
	    circ->Draw();
	    c.Add(circ);

	    // write ring property annotation on top
 	    TString hitstring("trk ");
 	    hitstring+=nTrkCnt;
 	    hitstring+=":  ";
 	    hitstring+=h->getSector();
 	    hitstring+=",";
	    hitstring+=padX;
 	    hitstring+=",";
 	    hitstring+=padY;
  	    hitstring+=" *** p:";
	    hitstring+=TMath::Floor(h->getKickMom());
 	    hitstring+=", beta:";
	    hitstring+=h->getKickBeta();
//  	    hitstring+=",";
//  	    hitstring+=h->getKickCharge();
//  	    hitstring+=" *** P:";
//  	    hitstring+=h->getKickBeta();
//  	    hitstring+=",H:";
//  	    hitstring+=h->getKickQual();
//  	    hitstring+=",A:";
//  	    hitstring+=h->getKickPull();
	    if (pCatSwitch->Contains("TERM"))
	    {
		cout<<hitstring.Data()<<endl;
	    }
	    else
	    {
		hittext = new TLatex(-0.95,0.925-(hitcnt-1)*0.06,
				     hitstring.Data());
		hittext->SetTextSize(0.0278884);
		hittext->SetLineWidth(2);
		s.Add(hittext);
		hittext->Draw();
	    }
	}//end if 
 
    }//end while over hit cat
    

}
void HRichMon::makeTrackAnnotation(TObjArray &c,TObjArray &s)
{
    // this function draws a black circle around the track entry point 
    //
    //
    //
    Float_t deg2rad=1./HConstant::rad2deg();
    // padplane is tilted by 20 degrees w/ respect to lab x-y-plane
    Float_t proj20 = TMath::Cos(20.*deg2rad);
    // radius for red circle around found ring scales with det. view
    // 0.06 is value for ring in NDC coords (0,0 in center, normalized to 1)
    Float_t circrad=.02 * zoomFactor;
    Float_t offset=0.04;// padplane shift from center !
    TArc *circ=0;//circle around ring
    TLatex *hittext=0;//text indicating hit coords
    Int_t linecolor = 12; //dark grey to mark kicktrack position

    Float_t zoomUnit = 0.55;//corresponds to a zoomfactor of 1.0 
    Int_t degPerSec = 60;
    

    Int_t hitcnt=0;//count hits (tracks)

    // loop over found tracks
    Int_t nTrkCnt = 0;
    HKickTrack *k=0;
    pKickIter->Reset();
    while((k = (HKickTrack *)pKickIter->Next()))
    {
	if(kTRUE)
	{
	    nTrkCnt++;
	    // we have a (theta,phi) coord from the kick track
	    // and need to find the pad which would be the 
	    // center of the ring formed by the lepton 
	    // that caused the track.
// 	    Int_t sec =  h->getSector();
// 	    if (sec==5) sec=-1;
// 	    Float_t offset = (sec+1)*degPerSec;
// 	    Float_t tphi = h->getMdcPhi() - offset;
// 	    cout<<"kick track phi: "<<tphi<<endl;
	    Float_t trackTheta,trackPhi;
	    trackTheta=trackPhi=0.;
	    HRichUtilFunc::rotateTrackToLab(k->getSector(),k->getTheta(),
			     k->getPhi(),trackTheta,trackPhi);
	    //	    cout<<"kick theta:"<<trackTheta<<"  kick phi:"<<trackPhi<<endl;
	    Int_t padX=0;
	    Int_t padY=0;
	    getPadXYforThetaPhi(trackTheta,trackPhi,padX,padY,k->getSector());
	    Int_t hitnodeind = 0;
	    if (padX>-1 && padY>-1)
	    {
		hitnodeind = (Int_t) ( (*mapTable)( padX,
						    padY ) 
				       + (k->getSector() * nPads) );
	    }
	    if (hitnodeind==0) continue;

	    //draw point at the position of track
	    //set visibility to 1
	    node[hitnodeind] -> SetVisibility(0);
	    node[hitnodeind] -> SetLineColor(linecolor);
	    node[hitnodeind] -> SetLineWidth(3);

	    hitcnt=s.GetLast()+1;
	    hitcnt++;
	    
	    Int_t nSec=k->getSector();
	    if (nSec==0) nSec=6;//plane azimuthal coords go anti-clockwise
	    nSec=(nSec-1)*degPerSec;//one sector covers 60 degrees

	    // distance r from center for polar coords
	    // 0.55 corresponds to a zoomfactor of 1.0 

	    Float_t r = zoomUnit*zoomFactor*(trackTheta/90. + 
					     offset*zoomFactor)*proj20; 

	    Float_t phi = 180. - trackPhi;
	    if (phi<0) phi = 360. + phi;
	    phi = phi*deg2rad;
	    
	    // create black circle around selected track
	    if (k->getBeta()>.95) linecolor=5;
	    circ = new TArc(r*TMath::Cos(phi),r*TMath::Sin(phi),circrad,0,360);
	    circ->SetLineWidth(2);
	    circ->SetLineColor(linecolor);
	    circ->Draw();
	    c.Add(circ);

	    // write ring property annotation on top
 	    TString hitstring("trk ");
 	    hitstring+=nTrkCnt;
 	    hitstring+=":  ";
 	    hitstring+=k->getSector();
 	    hitstring+=",";
	    hitstring+=padX;
 	    hitstring+=",";
 	    hitstring+=padY;
  	    hitstring+=" *** p:";
	    hitstring+=TMath::Floor(k->getP());
 	    hitstring+=", beta:";
	    hitstring+=k->getBeta();
//  	    hitstring+=",";
//  	    hitstring+=h->getKickCharge();
//  	    hitstring+=" *** P:";
//  	    hitstring+=h->getKickBeta();
//  	    hitstring+=",H:";
//  	    hitstring+=h->getKickQual();
//  	    hitstring+=",A:";
//  	    hitstring+=h->getKickPull();
	    if (pCatSwitch->Contains("TERM"))
	    {
		//		cout<<hitstring.Data()<<endl;
	    }
	    else
	    {
		hittext = new TLatex(-0.95,0.925-(hitcnt-1)*0.06,
				     hitstring.Data());
		hittext->SetTextSize(0.0278884);
		hittext->SetLineWidth(2);
		s.Add(hittext);
		//		hittext->Draw();
	    }
	}//end if 
 
    }//end while over hit cat
    

}


void HRichMon::deleteAnnotation(TObjArray &c,TObjArray &s)
{
    for (Int_t i=0;i<(c.GetLast()+1);i++)
	delete (TArc*)c[i];
    for (Int_t i=0;i<(s.GetLast()+1);i++)
	delete (TLatex*)s[i];
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
}
Bool_t HRichMon::selectEvt()
{

    Bool_t kTEST = kTRUE;
    if (pCatSwitch->Contains("HIT") && pCatSwitch->Contains("PAIR"))
    {
	if ( testHits() && testPairs() ) kTEST=kTRUE;
	else kTEST=kFALSE;
    }
    if (kTEST && pCatSwitch->Contains("CAL") ) 
    {
	el->Enter(nEvtsProcessed-1);//account for start event
    }
    return kTRUE;
}

Int_t HRichMon::getPadXforPhi(Float_t p,Int_t ystart,Int_t sec)
{
    HRichPadTab *tab = getPadTable();
    Int_t x0 = -1;
    Float_t dtmin = 2.;
    if (tab)
    {
	for (Int_t x=0;x<92;x++)
	{
	    HRichPad *pPadtmp = tab->getPad(x,ystart);
	    if (pPadtmp)
	    {
		Float_t dt = TMath::Abs(pPadtmp->getPhi(sec)-p);
		if (dt<=dtmin) {x0 = x; dtmin=dt;}
	    }
	}
    }
    
    return x0;

}

Int_t HRichMon::getPadYforTheta(Float_t t,Int_t xstart)
{
    HRichPadTab *tab = getPadTable();
    Int_t y0 = -1;
    Float_t dtmin = 2.;
    if (tab)
    {
	for (Int_t y=0;y<92;y++)
	{
	    HRichPad *pPadtmp = tab->getPad(xstart,y);
	    if (pPadtmp)
	    {
		Float_t dt = TMath::Abs(pPadtmp->getTheta()-t);
		if (dt<=dtmin) {y0 = y; dtmin=dt;}
	    }
	}
    }
    
    return y0;

}

void HRichMon::getPadXYforThetaPhi(Float_t t,Float_t p,Int_t &x,Int_t &y,Int_t sec)
{
    // this routine is pretty dumb, but we have a second to calculate :-)
    //take a row in the middle of the padplane, search the phi
    Int_t x0 = getPadXforPhi(p,45,sec);
    Int_t y0 = -1;
    //take a column at phi, search the pad with the closest theta
    if (x0>-1) y0 = getPadYforTheta(t,x0);
    else Error("getPadXYforThetaPhi","invalid pad x calculated");
    if (y0==-1) Error("getPadXYforThetaPhi","invalid pad y calculated");
    // due to the curvature of the rows, search on ...
    // use the found pad for theta and calc the closest pad for phi
    //
    Int_t xtmp=-1;
    Int_t ytmp=-1;
    Int_t nn=0;
    while (nn<10 && (xtmp!=x0 || ytmp!=y0))
    {
	nn++;
	xtmp=x0;ytmp=y0;
	x0 = getPadXforPhi(p,y0,sec);
	y0 = getPadYforTheta(t,x0);
	//	cout<<"x0:"<<x0<<" - y0:"<<y0<<endl;
    }
    x=x0;
    y=y0;
}

void HRichMon::waitOnKey()
{
        Int_t num;
	scanf("%d",&num);//enter integer
}
