// $Id: hrichchernovringfitter.cc,v 1.5 2008-05-09 16:16:32 halo Exp $
// Last update by Thomas Eberl: 03/04/02 16:55:48
//
// Class HRichChernovRingFitter
// implementation of fit routine 
// shamelessly adapted from: Thomas Ullrich, Dec 1999, STAR Collaboration
// ***************************************************************************
// *
// * Description:
// *
// * Fast fitting routine using an iterative linear regression 
// * method (ILRM). Reference: N.I.Chernov, G.A.Ososkov, Computer  
// * Physics Communication 33 (1984) 329-333.                   
// *
// ***************************************************************************
// FIXME:
// *** apply pad cleaning algorithm before fitting (single pads w/ a distance)
// *** subtract contributions from a second nearby ring before fitting
// *** implement functional minimization with distance-from-center weighted coords
// *** improve padpattern->photon  model, the "de-digitization"
// *** the routine is fast, but the extraction of the fired pads into a suitable
//     interface structure for the fitting routing is too slow !!!
// ***************************************************************************
#include "hrichchernovringfitter.h"
#include "hrichphotonhit.h"
#include "hrichphotonhitheader.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hrichhit.h"
#include "hrichhitfit.h"
#include "hrichcal.h"
#include "hrichpad.h"
#include "hrichgeometrypar.h"
#include "hlinearcategory.h"

ClassImp(HRichChernovRingFitter)

HRichChernovRingFitter::HRichChernovRingFitter(Text_t *name,Text_t *title,Bool_t kPhoton) :
  HReconstructor(name,title)
{
     kPhotonFinder = kPhoton;
     pIterCal=NULL;
     pIterHit=NULL;
     pIterPhotonHit=NULL;
     fs = 6; //half size of square to be cut out around hit point
     arraysize = (2*fs+1)*(2*fs+1);
     evtcounter=0;
     pCalCat=NULL;
     pHitCat=NULL;
     pHitFitCat=NULL;
     pPhotonHitCat=NULL;
     pGeomPar=NULL;
}

HRichChernovRingFitter::~HRichChernovRingFitter(void) 
{
   if (pIterCal) delete pIterCal;
   if (pIterHit) delete pIterHit;
   if (pIterPhotonHit) delete pIterPhotonHit;
}

Bool_t HRichChernovRingFitter::init() 
{
    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");

    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    HRichGeometryPar *pGeomPar = 
	(HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
    setGeometryPar(pGeomPar);
    if (getGeometryPar()==NULL) 
    {
	cout<<"ERROR in init: no pointer to geom container"
	    <<endl;
    }

    // RICH CAL container
    pCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    if (!pCalCat) {
	cout<<"no valid CAL category on file"<<endl;
	return kFALSE;
    }
    pIterCal = (HIterator*)getCalCat()->MakeIterator("native");

    // RICH HIT container
    pHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    if (!pHitCat) 
    {
	cout<<"no valid HIT category on file"<<endl;
	return kFALSE;
    }
    pIterHit = (HIterator*)getHitCat()->MakeIterator();

    // RICH HIT FIT container
    pHitFitCat=gHades->getCurrentEvent()->getCategory(catRichHitFit);
    if (!pHitFitCat) 
    {
	pHitFitCat=pRichDet->buildCategory(catRichHitFit);
	
	if (!pHitFitCat) return kFALSE;
	else gHades->getCurrentEvent()
		 ->addCategory(catRichHitFit, pHitFitCat, "Rich");
    }

    if (kPhotonFinder){
	// RICH PHOTON HITHEADER container
	pPhotonHitHeaderCat=gHades->getCurrentEvent()
	    ->getCategory(catRichPhotonHitHeader);
	if (!pPhotonHitHeaderCat) {
	    pPhotonHitHeaderCat=pRichDet
		->buildCategory(catRichPhotonHitHeader);
	    
	    if (!pPhotonHitHeaderCat) return kFALSE;
	    else gHades->getCurrentEvent()
		     ->addCategory(catRichPhotonHitHeader, 
				   pPhotonHitHeaderCat, "Rich");
	}
	
	pIterPhotonHitHeader = (HIterator*)getPhotonHitHeaderCat()
	                       ->MakeIterator("native");

	// RICH PHOTON HIT container
	pPhotonHitCat=gHades->getCurrentEvent()->getCategory(catRichPhotonHit);
	if (!pPhotonHitCat) {
	    pPhotonHitCat=pRichDet->buildCategory(catRichPhotonHit);
	    
	    if (!pPhotonHitCat) return kFALSE;
	    else gHades->getCurrentEvent()
		     ->addCategory(catRichPhotonHit, pPhotonHitCat, "Rich");
	}
	
	pIterPhotonHit = (HIterator*)getPhotonHitCat()->MakeIterator("native");
    }

    return kTRUE;
}

Bool_t HRichChernovRingFitter::finalize() 
{
    return kTRUE;
}


Int_t HRichChernovRingFitter::execute()
{
    if (kPhotonFinder){
	// search and use photon impact coords for fit
	if(!fitFoundPhotons()) return kFALSE;
    }else{
	// use fired pad coords for fit
	if(!fitFiredPads()) return kFALSE;
    }
    evtcounter++;
    return 0;
}

// Bool_t HRichChernovRingFitter::fitFoundPhotons()
// {
//     // this function is not yet checked !!!
//     HRichPhotonHitHeader *pPhotonHitHeader;
//     pIterPhotonHitHeader->Reset();
//     Double_t *dX = new Double_t[arraysize];
//     Double_t *dY = new Double_t[arraysize];
//     Double_t *dCh = new Double_t[arraysize];
//     while(    (pPhotonHitHeader = (HRichPhotonHitHeader *)
// 	       pIterPhotonHitHeader->Next())
// 	 )
//     {
// 	//cout<<"in photon hit header loop"<<endl;
// 	Int_t nHeaderRingX       = pPhotonHitHeader->getRingX();
// 	Int_t nHeaderRingY       = pPhotonHitHeader->getRingY();
// 	Float_t fHeaderRingPhi   = pPhotonHitHeader->getRingPhi();
// 	Float_t fHeaderRingTheta = pPhotonHitHeader->getRingTheta();
// 	Int_t nHeaderSector      = pPhotonHitHeader->getSector();
// 	Int_t nHeaderNbPhot      = pPhotonHitHeader->getNbPhotons();
// 	//cout<<"ring X: "<<nHeaderRingX<<" ring Y: "<<nHeaderRingY<<endl;
// 	HRichPhotonHit *pPhotonHit;
// 	pIterPhotonHit->Reset();
// 	Int_t nHitCounter=0;
// 	while( (pPhotonHit = (HRichPhotonHit *)pIterPhotonHit->Next()) )
// 	{
// 	    //cout<<"in photon hit header loop"<<endl;
// 	    Int_t nHitRingX = pPhotonHit->getRingX();
// 	    Int_t nHitRingY = pPhotonHit->getRingY();
// 	    Int_t nHitSector = pPhotonHit->getSector();
// 	    if (nHitSector==nHeaderSector &&
// 		nHitRingX ==nHeaderRingX &&
// 		nHitRingY ==nHeaderRingY)
// 	    {
// 		dX[nHitCounter]=pPhotonHit->getPhi();
// 		dY[nHitCounter]=pPhotonHit->getTheta();
// 		dCh[nHitCounter]=pPhotonHit->getCharge();
// 		nHitCounter++;    
// 	    }

// 	} //end while photon hits
// 	if (nHitCounter!=nHeaderNbPhot) cout<<"Error:photon nb mismatch"<<endl;
// 	//if (!mX) mX = new TArrayD(nHitCounter,compress(dX,nHitCounter));
// 	mX->Set(nHitCounter,compress(dX,nHitCounter)); 
	
// 	//if (!mY) mY = new TArrayD(nHitCounter,compress(dY,nHitCounter));
// 	mY->Set(nHitCounter,compress(dY,nHitCounter));
	
// 	//if (!mCh) mCh = new TArrayD(nHitCounter,compress(dCh,nHitCounter));
// 	mCh->Set(nHitCounter,compress(dCh,nHitCounter));

// 	clearFitParams();
// 	// ===================================================================
// 	if (fit())  // <------ here the actual fitting algorithm is called !!!
// 	// ===================================================================
// 	{//check if fit is reasonable
	    
// 	    if (! ((dFittedCenterX-fHeaderRingPhi) < 2. &&
// 		(dFittedCenterY-fHeaderRingTheta) < 2. )  )
// 			unvalidFitParams();
// 	}else{unvalidFitParams();}
// 	switchXtoPhi();
// 	Int_t ind=-1;
// 	storeFitinHRichHitFit(nHeaderSector,nHitCounter,ind);
// 	//dumpFitParameters(nHeaderSector,nHeaderRingX,nHeaderRingY,-1.);
	
//     } // end while photon hit header
//     if (dX) delete [] dX; 
//     if (dY) delete [] dY;
//     if (dCh) delete [] dCh;
//     return kTRUE;
// }

void HRichChernovRingFitter::switchXtoPhi()
{
    dFittedCenterPhi=dFittedCenterX;
    dFittedCenterTheta=dFittedCenterY;
    dFittedCenterX=-1;
    dFittedCenterY=-1;
}


Bool_t HRichChernovRingFitter::fitFiredPads2()
{
    // alternative implementation which is slower, astonishingly 
    HRichHit *pHit;
    pIterHit->Reset();
    while((pHit = (HRichHit *)pIterHit->Next()))
    {
	Double_t *dX = new Double_t[arraysize];
	Double_t *dY = new Double_t[arraysize];
	Double_t *dCh = new Double_t[arraysize];
	for (Int_t ii=0;ii<arraysize;ii++) dX[ii]=dY[ii]=dCh[ii]=-1.;
	Int_t nPadCounter = 0;
	//cout<<"start ring ************************** "<<endl;
	// center of ring as found by the ring finder
	Int_t nHitSector = pHit->getSector();
	Int_t nHitX = pHit->iRingX;
	Int_t nHitY = pHit->iRingY;
	Int_t startcol=0;
	if (nHitX-fs>=0) startcol = nHitX-fs;
	else startcol = 0;
	Int_t startrow=0;
	if (nHitY-fs>=0) startrow = nHitY-fs;
	else startrow = 0;
	Int_t endcol=0;
	if (nHitX+fs<=92) endcol = nHitX+fs;
	else endcol = 92;
	Int_t endrow=0;
	if (nHitY+fs<=92) endrow = nHitY+fs;
	else endrow = 92;

	for (Int_t i=startcol;i<=endcol;i++)
	{
	    for (Int_t j=startrow;j<=endrow;j++)
	    {
		HLocation loc;
		loc.set(3,0,0,0);
		loc.setOffset(i);
		loc.setIndex(1,j);
		loc.setIndex(0,nHitSector);
		HRichCal *c = (HRichCal*)((HMatrixCategory*)getCalCat())->getObject(loc);
		if (c)
		{
		    dX[nPadCounter] = i; 
		    dY[nPadCounter] = j; 
		    dCh[nPadCounter] = c->getCharge();
		    //cout<<"col: "<<i<<"  row: "<<j<<"   ch: "<<c->getCharge()<<endl;
		    nPadCounter++;
		}
	    }
	}
	//cout<<"nr of pads: "<<nPadCounter<<endl;
	Double_t *dnewX = compress(dX,nPadCounter);
	Double_t *dnewY = compress(dY,nPadCounter);
	Double_t *dnewCh = compress(dCh,nPadCounter);
	TArrayD x(nPadCounter,dnewX);
	TArrayD y(nPadCounter,dnewY);
	TArrayD c(nPadCounter,dnewCh);
	delete [] dX;
	delete [] dY;
	delete [] dCh;
	delete [] dnewX;
	delete [] dnewY;
	delete [] dnewCh;
	clearFitParams();
	if (nPadCounter>3) {if (fit(x,y)) calcThetaAndPhi(pHit);}
	//dumpFitParameters(nHitSector,nHitX,nHitY,pHit->getRadius());
	//cout<<"end ring ************************** "<<endl;
    }
    
    return kTRUE;
}
Bool_t HRichChernovRingFitter::fitFiredPads()
{
    // What to do with hits on the padplane border ?
    // They are not fitted correctly !
    HRichHit *pHit;
    pIterHit->Reset();
    
    // loop over each ring in event
    while((pHit = (HRichHit *)pIterHit->Next()))
    {
	// store pad coords of ring
	Double_t *dX = new Double_t[arraysize];
	Double_t *dY = new Double_t[arraysize];
	Double_t *dCh = new Double_t[arraysize];
	for (Int_t i=0;i<arraysize;i++) dX[i]=dY[i]=dCh[i]=-1.;;   

	// hit data
	Int_t nHitSector = pHit->getSector();
	Int_t nHitX = pHit->iRingX;
	Int_t nHitY = pHit->iRingY;
	
	// fired pad data
	HRichCal* pCal;
	pIterCal->Reset();
	Int_t nPadCounter=0;

	// loop over fired pads in arraysize sized square around hit
	// and store them for fitting
	while((pCal = (HRichCal *)pIterCal->Next()))
	{
	   if (nHitSector == pCal->getSector())
	   {
	       Int_t nCalRow = pCal->getRow();
	       Int_t nCalCol = pCal->getCol();
	       Float_t nCalChrg = pCal->getCharge();

	       if(TMath::Abs(nCalRow-nHitY) <= fs &&
		  TMath::Abs(nCalCol-nHitX) <= fs)
	       {
		   dX[nPadCounter] = nCalCol;
		   dY[nPadCounter] = nCalRow;
		   dCh[nPadCounter] = nCalChrg;
		   nPadCounter++;
	       }
	   }
	} // endwhile cal container
	
	// resize array according to nr of found pads in square 
	Double_t *dnewX = compress(dX,nPadCounter);
	Double_t *dnewY = compress(dY,nPadCounter);
	Double_t *dnewCh = compress(dCh,nPadCounter);
	// use ROOT arrays as interface to fit routine
	// (historical reasons, potentially slow ... )
	TArrayD x(nPadCounter,dnewX);
	TArrayD y(nPadCounter,dnewY);
	TArrayD c(nPadCounter,dnewCh);
	// free mem of used helper arrays
	if (dX) delete [] dX;
	if (dY) delete [] dY;
	if (dCh) delete [] dCh;
	if (dnewX) delete [] dnewX;
	if (dnewY) delete [] dnewY;
	if (dnewCh) delete [] dnewCh;
	
	// reset params filled by fitting routine
	clearFitParams();
	// ===================================================================
	// do not fit rings with less than 4 pads
	if (nPadCounter>3 && fit(x,y))  // <------ here the actual fitting algorithm is called !!!
	// ===================================================================
	{//check if fit is reasonable
	    
	    if ((dFittedCenterX-nHitX) < 2. &&
		(dFittedCenterY-nHitY) < 2. )
	    {
		// calculate theta and phi by linear interpolation of pad centers
		calcThetaAndPhi(pHit);
	    } 
	    else // fit was too far off
	    {
		//cout<<"ERROR: Fitted center too far off !"<<endl;
		// set the fit params to a invalid default
		unvalidFitParams();
	    }
		
	}
	else 
	{
	    //cout<<"ERROR: Fit unsuccessful !"<<endl;
	    unvalidFitParams();
	}
	Int_t ind = getHitCat()->getIndex(pHit);
	storeFitinHRichHitFit(pHit->getSector(),nPadCounter,ind);
	//dumpFitParameters(nHitSector,nHitX,nHitY,pHit->fRingRadius);
    } //endwhile hit container
    return kTRUE;
}

void HRichChernovRingFitter::storeFitinHRichHitFit(Int_t s, Int_t n,Int_t i)
{
    HLocation loc;
    loc.set(1,s);
    HRichHitFit *hitfit = (HRichHitFit*)((HLinearCategory*)getHitFitCat())
	->getNewSlot(loc);
    if (hitfit!=NULL)
    {
	hitfit->Reset();
	hitfit = new(hitfit) HRichHitFit;
	hitfit->setHitIndex(i);
	hitfit->setSector(s);
	hitfit->setNbCoords(n);
	hitfit->setFitRadius(getFitRadius());
	hitfit->setFitCenterX(getFitCenterX());
	hitfit->setFitCenterY(getFitCenterY());
	hitfit->setFitVar(getFitVar());
	hitfit->setFitCenterTheta(getFitCenterTheta());
	hitfit->setFitCenterPhi(getFitCenterPhi());
    }
}

void HRichChernovRingFitter::clearFitParams()
{
    dFittedRadius = 0.;
    dFittedCenterX = 0.;
    dFittedCenterY = 0.;
    dFitVariance = 0.;
    dFittedCenterTheta = 0.;
    dFittedCenterPhi = 0.;
}

void HRichChernovRingFitter::unvalidFitParams()
{
    dFittedRadius = -1.;
    dFittedCenterX = -1.;
    dFittedCenterY = -1.;
    dFitVariance = -1.;
    dFittedCenterTheta = -1.;
    dFittedCenterPhi = -1.;
}

Double_t* HRichChernovRingFitter::compress(Double_t* arr,Int_t nPadCounter)
{
    Int_t nNonZeroElementCounter=0;
    Double_t* arr2=0;
    for (Int_t i=0;i<arraysize;i++)
    {
	if (arr[i] != -1) nNonZeroElementCounter++;
    }
    if (nNonZeroElementCounter==nPadCounter) // redundant check
    {
	arr2 = new Double_t[nNonZeroElementCounter];
	for (Int_t j=0;j<nNonZeroElementCounter;j++)
	{
	    arr2[j]=arr[j];
	}
    }
    else 
    {
	cout<<"cnt eles : "<<nNonZeroElementCounter<<"   giv eles: "<<nPadCounter<<endl;
	Error("compress","inconsistency");
    }
    return arr2;
}

// Bool_t HRichChernovRingFitter::fit()
// {
//     // needs coord arrays as data members
//     Int_t i;
//     Double_t xx, yy, xx2, yy2;
//     Double_t f, g, h, p, q, t, g0, g02, a, b, c, d;
//     Double_t xroot, ff, fp, xd, yd, g1;
//     Double_t dx, dy, dradius2, xnom;
    
//     Double_t xgravity = 0.0;
//     Double_t ygravity = 0.0;
//     Double_t x2 = 0.0;
//     Double_t y2 = 0.0;
//     Double_t xy = 0.0;
//     Double_t xx2y2 = 0.0;
//     Double_t yx2y2 = 0.0;
//     Double_t x2y22 = 0.0;
//     Double_t radius2 = 0.0;

//     mRC = 0;
    
//     Int_t npoints = mX->GetSize();
    
//     if (npoints <= 3) {
// 	mRC = 1;
// 	//cout<<"Error: less than 4 points for fit !!"<<endl;
// 	return kFALSE;
//     }
   
//     // arithmetic mean of points
//     xgravity = mX->GetSum() / npoints;
//     ygravity = mY->GetSum() / npoints;
        
//     for (i=0; i<npoints; i++) {
// 	xx  = (*mX)[i]-xgravity; // coords relative to the mean 
// 	yy  = (*mY)[i]-ygravity;
// 	xx2 = xx*xx; // calculate Gauss bracket values for normal equations
// 	yy2 = yy*yy;
// 	x2  += xx2;
// 	y2  += yy2;
// 	xy  += xx*yy;
// 	xx2y2 += xx*(xx2+yy2);
// 	yx2y2 += yy*(xx2+yy2);
// 	x2y22 += (xx2+yy2)*(xx2+yy2);
//     }
//     if (xy == 0.) {
// 	mRC = 2;
// 	//cout<<"Error: too many points on a line for fitting"<<endl;
// 	return kFALSE;
//     }
//     // calculate coefficients for 4th order equation 
//     f = (3.*x2+y2)/npoints;
//     g = (x2+3.*y2)/npoints;
//     h = 2*xy/npoints;
//     p = xx2y2/npoints;
//     q = yx2y2/npoints;
//     t = x2y22/npoints;
//     g0 = (x2+y2)/npoints;
//     g02 = g0*g0;
//     a = -4.0;
//     b = (f*g-t-h*h)/g02;
//     c = (t*(f+g)-2.*(p*p+q*q))/(g02*g0);
//     d = (t*(h*h-f*g)+2.*(p*p*g+q*q*f)-4.*p*q*h)/(g02*g02);
//     xroot = 1.0; // solve 4th order equation by Newton method
//     for (i=0; i<5; i++) {
// 	ff = (((xroot+a)*xroot+b)*xroot+c)*xroot+d;
// 	fp = ((4.*xroot+3.*a)*xroot+2.*b)*xroot+c;
// 	xroot -= ff/fp;
//     }
//     g1 = xroot*g0;
//     xnom = (g-g1)*(f-g1)-h*h;
//     if (xnom == 0.) {
// 	mRC = 3;
// 	//cout<<"Error: denominator zero !"<<endl;
// 	return kFALSE;
//     }
//     yd = (q*(f-g1)-h*p)/xnom;
//     xnom = f-g1;
//     if (xnom == 0.) {
// 	mRC = 4;
// 	//cout<<"Error: denominator zero !"<<endl;
// 	return kFALSE;
//     }
//     xd = (p-h*yd )/xnom;
    
//     radius2 = xd*xd+yd*yd+g1;
//     dFittedCenterX = xd+xgravity;
//     dFittedCenterY = yd+ygravity;
    
//     for (i=0; i<npoints; i++) {
// 	dx = (*mX)[i]-(dFittedCenterX);
// 	dy = (*mY)[i]-(dFittedCenterY);
// 	dradius2 = dx*dx+dy*dy;
// 	dFitVariance += dradius2+radius2-2.*sqrt(dradius2*radius2);
//     }
//     dFitVariance /= npoints-3.0;
    
//     dFittedRadius  = sqrt(radius2);
//     Bool_t kRC = kTRUE;
    
//     return kRC;
// }
Bool_t HRichChernovRingFitter::fit(TArrayD &mXc, TArrayD &mYc)
{
    Int_t i;
    Double_t xx, yy, xx2, yy2;
    Double_t f, g, h, p, q, t, g0, g02, a, b, c, d;
    Double_t xroot, ff, fp, xd, yd, g1;
    Double_t dx, dy, dradius2, xnom;
    
    Double_t xgravity = 0.0;
    Double_t ygravity = 0.0;
    Double_t x2 = 0.0;
    Double_t y2 = 0.0;
    Double_t xy = 0.0;
    Double_t xx2y2 = 0.0;
    Double_t yx2y2 = 0.0;
    Double_t x2y22 = 0.0;
    Double_t radius2 = 0.0;

    mRC = 0;
    
    Int_t npoints = mXc.GetSize();
    
    if (npoints <= 3) {
	mRC = 1;
	//cout<<"Error: less than 4 points for fit !!"<<endl;
	return kFALSE;
    }
   
    // arithmetic mean of points
    xgravity = mXc.GetSum() / npoints;
    ygravity = mYc.GetSum() / npoints;
        
    for (i=0; i<npoints; i++) {
	xx  = mXc[i]-xgravity; // coords relative to the mean 
	yy  = mYc[i]-ygravity;
	xx2 = xx*xx; // calculate Gauss bracket values for normal equations
	yy2 = yy*yy;
	x2  += xx2;
	y2  += yy2;
	xy  += xx*yy;
	xx2y2 += xx*(xx2+yy2);
	yx2y2 += yy*(xx2+yy2);
	x2y22 += (xx2+yy2)*(xx2+yy2);
    }
    if (xy == 0.) {
	mRC = 2;
	//cout<<"Error: too many points on a line for fitting"<<endl;
	return kFALSE;
    }
    // calculate coefficients for 4th order equation 
    f = (3.*x2+y2)/npoints;
    g = (x2+3.*y2)/npoints;
    h = 2*xy/npoints;
    p = xx2y2/npoints;
    q = yx2y2/npoints;
    t = x2y22/npoints;
    g0 = (x2+y2)/npoints;
    g02 = g0*g0;
    a = -4.0;
    b = (f*g-t-h*h)/g02;
    c = (t*(f+g)-2.*(p*p+q*q))/(g02*g0);
    d = (t*(h*h-f*g)+2.*(p*p*g+q*q*f)-4.*p*q*h)/(g02*g02);
    xroot = 1.0; // solve 4th order equation by Newton method
    for (i=0; i<5; i++) {
	ff = (((xroot+a)*xroot+b)*xroot+c)*xroot+d;
	fp = ((4.*xroot+3.*a)*xroot+2.*b)*xroot+c;
	xroot -= ff/fp;
    }
    g1 = xroot*g0;
    xnom = (g-g1)*(f-g1)-h*h;
    if (xnom == 0.) {
	mRC = 3;
	//cout<<"Error: denominator zero !"<<endl;
	return kFALSE;
    }
    yd = (q*(f-g1)-h*p)/xnom;
    xnom = f-g1;
    if (xnom == 0.) {
	mRC = 4;
	//cout<<"Error: denominator zero !"<<endl;
	return kFALSE;
    }
    xd = (p-h*yd )/xnom;
    
    radius2 = xd*xd+yd*yd+g1;
    dFittedCenterX = xd+xgravity;
    dFittedCenterY = yd+ygravity;
    
    for (i=0; i<npoints; i++) {
	dx = mXc[i]-(dFittedCenterX);
	dy = mYc[i]-(dFittedCenterY);
	dradius2 = dx*dx+dy*dy;
	dFitVariance += dradius2+radius2-2.*sqrt(dradius2*radius2);
    }
    dFitVariance /= npoints-3.0;
    
    dFittedRadius  = sqrt(radius2);
    Bool_t kRC = kTRUE;
    
    return kRC;
}

void HRichChernovRingFitter::dumpFitParameters(Int_t sec,Int_t x,Int_t y,Float_t r)
{
    HRichPadTab *pPadsPar = ((HRichGeometryPar*)getGeometryPar())->getPadsPar();
    if (pPadsPar)
    {
	HRichPad *pPad0 = pPadsPar->getPad(x,y);
	if (pPad0)
	{
	    Float_t theta = pPad0->getTheta();
	    Float_t phi   = pPad0->getPhi(sec);
	    cout<<"EVENT NUMBER :"<<evtcounter<<endl;
	    cout<<"Rich Hit> x:"<<x<<"      y:"<<y<<"      rad:"<<r
		<<" theta:"<<theta<<"  phi:"<<phi<<"  sec:"<<sec<<endl;
	    cout<<"     Fit> x:"<<dFittedCenterX<<" y:"<<dFittedCenterY
		<<" rad:"<<dFittedRadius<<" theta:"<<dFittedCenterTheta
		<<" phi:"<<dFittedCenterPhi
		<<" var:"<<dFitVariance
		<<endl;
	} else cout<<"ERROR in HRichChernovRingFitter::dumpFitParameters"<<
		   " : no pointer to pad"<<endl;
    } else cout<<"ERROR in HRichChernovRingFitter::dumpFitParameters"<<
		   " : no pointer to RTDB"<<endl;
}

void HRichChernovRingFitter::calcThetaAndPhi(HRichHit *pHit)
{
    // interpolate the theta and phi values from the RTDB
    // which are for the respective pad centers
    // in order to retrieve the correct angles for the hit point
    //       4
    //    2  0  3
    //       1
    // if a pad cannot be retrieved from the container
    // do not interpolate
    HRichPadTab *pPadsPar = ((HRichGeometryPar*)getGeometryPar())->getPadsPar();
    Int_t nPadX0 = (Int_t) TMath::Floor(dFittedCenterX);
    Int_t nPadY0 = (Int_t) TMath::Floor(dFittedCenterY);
    if (pPadsPar)
    {
	Float_t fThetaOfPadCenter0 = 0;
	Float_t fPhiOfPadCenter0   = 0;
	HRichPad *pPad0 = pPadsPar->getPad(nPadX0,nPadY0);
	if (pPad0)
	{
	    fThetaOfPadCenter0 = pPad0->getTheta();
	    fPhiOfPadCenter0   = pPad0->getPhi(pHit->getSector());
	} else {
	    return;
// 	    cout<<"ERROR: no pointer to pad 0"<<endl;
// 	    dumpFitParameters(pHit->getSector(),pHit->iRingX,pHit->iRingY,
// 			      pHit->fRingRadius);
// 	    cout<<" PadX: "<<nPadX0<<" PadY: "<<nPadY0
// 		<<" FitX: "<<dFittedCenterX<<" FitY: "<<dFittedCenterY<<endl;
	}

	
	Float_t fThetaOfPadCenter1 = 0;
	//Float_t fPhiOfPadCenter1 = 0;
	HRichPad *pPad1 = pPadsPar->getPad(nPadX0,nPadY0-1);
	if (pPad1)
	{
	    fThetaOfPadCenter1 = pPad1->getTheta();
	    //fPhiOfPadCenter1   = pPad1->getPhi(sector);//for later use
	} else {
	    return;
// 	    cout<<"ERROR: no pointer to pad 1"<<endl;
// 	    dumpFitParameters(pHit->getSector(),pHit->iRingX,pHit->iRingY,
// 			      pHit->fRingRadius);
// 	    cout<<" PadX: "<<nPadX0<<" PadY: "<<nPadY0-1
// 		<<" FitX: "<<dFittedCenterX<<" FitY: "<<dFittedCenterY<<endl;
	}


	//Float_t fThetaOfPadCenter2 = 0;
	Float_t fPhiOfPadCenter2 = 0;
	HRichPad *pPad2 = pPadsPar->getPad(nPadX0-1,nPadY0);
	if (pPad2)
	{
	    //fThetaOfPadCenter2 = pPad2->getTheta();//for later use
	    fPhiOfPadCenter2   = pPad2->getPhi(pHit->getSector());
	} else {
	    return;
// 	    cout<<"ERROR: no pointer to pad 2"<<endl;
// 	    dumpFitParameters(pHit->getSector(),pHit->iRingX,pHit->iRingY,
// 			      pHit->fRingRadius);
// 	    cout<<" PadX: "<<nPadX0-1<<" PadY: "<<nPadY0
// 		<<" FitX: "<<dFittedCenterX<<" FitY: "<<dFittedCenterY<<endl;
	}


	//Float_t fThetaOfPadCenter3 = 0;
	Float_t fPhiOfPadCenter3 = 0;
	HRichPad *pPad3 = pPadsPar->getPad(nPadX0+1,nPadY0);
	if (pPad3)
	{
	    // fThetaOfPadCenter3 = pPad3->getTheta();//for later use
	    fPhiOfPadCenter3   = pPad3->getPhi(pHit->getSector());
	} else {
	    return;
// 	    cout<<"ERROR: no pointer to pad 3"<<endl;
// 	    dumpFitParameters(pHit->getSector(),pHit->iRingX,pHit->iRingY,
// 			      pHit->fRingRadius);
// 	    cout<<" PadX: "<<nPadX0+1<<" PadY: "<<nPadY0
// 		<<" FitX: "<<dFittedCenterX<<" FitY: "<<dFittedCenterY<<endl;
	} 

 
	Float_t fThetaOfPadCenter4 = 0;
	//Float_t fPhiOfPadCenter4   = 0;
	HRichPad *pPad4 = pPadsPar->getPad(nPadX0,nPadY0+1);
	if (pPad4)
	{
	    fThetaOfPadCenter4 = pPad4->getTheta();
	    //fPhiOfPadCenter4   = pPad4->getPhi(sector);//for later use
	} else {
	    return;
// 	    cout<<"ERROR: no pointer to pad 4"<<endl;
// 	    dumpFitParameters(pHit->getSector(),pHit->iRingX,pHit->iRingY,
// 			      pHit->fRingRadius);
// 	    cout<<" PadX: "<<nPadX0<<" PadY: "<<nPadY0+1
// 		<<" FitX: "<<dFittedCenterX<<" FitY: "<<dFittedCenterY<<endl;
	} 



	// phi interpolation
	//	cout<<"phi interpolation"<<endl;
	if (dFittedCenterX-nPadX0 == 0.5) dFittedCenterPhi=fPhiOfPadCenter0;
	if (dFittedCenterX-nPadX0 > 0.5)
	{
	    dFittedCenterPhi = (dFittedCenterX-0.5-nPadX0)*
		(fPhiOfPadCenter3-fPhiOfPadCenter0) + fPhiOfPadCenter0;
	}
	if (dFittedCenterX-nPadX0 < 0.5)
	{
	    dFittedCenterPhi = (dFittedCenterX+0.5-nPadX0)*
		(fPhiOfPadCenter0-fPhiOfPadCenter2) + fPhiOfPadCenter2;
	}



	// theta interpolation
	//cout<<"theta interpolation"<<endl;
	if (dFittedCenterY-nPadY0 == 0.5) dFittedCenterTheta=fThetaOfPadCenter0;
	if (dFittedCenterY-nPadY0 > 0.5)
	{
	    dFittedCenterTheta = (dFittedCenterY-0.5-nPadY0)*
		(fThetaOfPadCenter4-fThetaOfPadCenter0) + fThetaOfPadCenter0;   
	}
	if (dFittedCenterY-nPadY0 < 0.5)
	{
	    dFittedCenterTheta = (dFittedCenterY+0.5-nPadY0)*
		(fThetaOfPadCenter0-fThetaOfPadCenter1) + fThetaOfPadCenter1;   
	}


	// FIXME: if e.g. theta of 3 and 0 are not equal, one has to calc
	// the difference and add to dFittedCenterTheta according to the phi !!
	// but this should be a very small effect
    }
    else cout<<"ERROR: no pointer to RTDB"<<endl;
    
}

