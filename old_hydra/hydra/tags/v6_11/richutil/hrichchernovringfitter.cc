// $Id: hrichchernovringfitter.cc,v 1.3 2002-09-25 16:19:52 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:29:44
//
// Class HRichChernovRingFitter
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
     fs = 6; //size of square to be cut out around hit point
     arraysize = (2*fs+1)*(2*fs+1);
     evtcounter=0;
     pCalCat=NULL;
     pHitCat=NULL;
     pHitFitCat=NULL;
     pPhotonHitCat=NULL;
     pGeomPar=NULL;
}

HRichChernovRingFitter::~HRichChernovRingFitter(void) {
   if (pIterCal) delete pIterCal;
   if (pIterHit) delete pIterHit;
   if (pIterPhotonHit) delete pIterPhotonHit;
   //if (mX) delete mX;
   //if (mY) delete mY;
   //if (mCh) delete mCh;

}

Bool_t HRichChernovRingFitter::init() {
    //cout<<" in  HRichChernovRingFitter init"<<endl;
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
	//	pCalCat=pRichDet->buildCategory(catRichCal);

//  	if (!pCalCat) return kFALSE;
//  	else gHades->getCurrentEvent()
//  		 ->addCategory(catRichCal, pCalCat, "Rich");
//      }
    pIterCal = (HIterator*)getCalCat()->MakeIterator("native");
    cout<<"this was cal"<<endl;
    // RICH HIT container
    pHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    if (!pHitCat) {
      pHitCat=pRichDet->buildCategory(catRichHit);

      if (!pHitCat) return kFALSE;
      else gHades->getCurrentEvent()
	       ->addCategory(catRichHit, pHitCat, "Rich");
    }
    
    //pIterHit = (HMatrixCatIter*)getHitCat()->MakeIterator();
    pIterHit = (HIterator*)getHitCat()->MakeIterator();
        cout<<"this was hit"<<endl;

    // RICH HIT FIT container
    pHitFitCat=gHades->getCurrentEvent()->getCategory(catRichHitFit);
    if (!pHitFitCat) {
      pHitFitCat=pRichDet->buildCategory(catRichHitFit);

      if (!pHitFitCat) return kFALSE;
      else gHades->getCurrentEvent()
	       ->addCategory(catRichHitFit, pHitFitCat, "Rich");
    }
        cout<<"this was fit"<<endl;

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

    // init helper arrays for temp storage of cut out pads
    
    //cout<<"end of init()"<<endl;
    mX = new TArrayD(10);
    mY = new TArrayD(10);
    mCh= new TArrayD(10);
    return kTRUE;
}

Bool_t HRichChernovRingFitter::finalize() {
  
    return kTRUE;
}


Int_t HRichChernovRingFitter::execute()
{
    //cout<<"in HRichChernovRingFitter execute()"<<endl;
    if (kPhotonFinder){
	if(!fitFoundPhotons()) return kFALSE;
    }else{
	if(!fitFiredPads()) return kFALSE;
    }
    evtcounter++;
    //cout<<"end of execute() :"<<evtcounter<<endl;
    return 0;
}

Bool_t HRichChernovRingFitter::fitFoundPhotons()
{
    //cout<<"in fitFoundPhotons()"<<endl;
    HRichPhotonHitHeader *pPhotonHitHeader;
    pIterPhotonHitHeader->Reset();
    Double_t *dX = new Double_t[arraysize];
    Double_t *dY = new Double_t[arraysize];
    Double_t *dCh = new Double_t[arraysize];
    while(    (pPhotonHitHeader = (HRichPhotonHitHeader *)
	       pIterPhotonHitHeader->Next())
	 )
    {
	//cout<<"in photon hit header loop"<<endl;
	Int_t nHeaderRingX       = pPhotonHitHeader->getRingX();
	Int_t nHeaderRingY       = pPhotonHitHeader->getRingY();
	Float_t fHeaderRingPhi   = pPhotonHitHeader->getRingPhi();
	Float_t fHeaderRingTheta = pPhotonHitHeader->getRingTheta();
	Int_t nHeaderSector      = pPhotonHitHeader->getSector();
	Int_t nHeaderNbPhot      = pPhotonHitHeader->getNbPhotons();
	//cout<<"ring X: "<<nHeaderRingX<<" ring Y: "<<nHeaderRingY<<endl;
	HRichPhotonHit *pPhotonHit;
	pIterPhotonHit->Reset();
	Int_t nHitCounter=0;
	while( (pPhotonHit = (HRichPhotonHit *)pIterPhotonHit->Next()) )
	{
	    //cout<<"in photon hit header loop"<<endl;
	    Int_t nHitRingX = pPhotonHit->getRingX();
	    Int_t nHitRingY = pPhotonHit->getRingY();
	    Int_t nHitSector = pPhotonHit->getSector();
	    if (nHitSector==nHeaderSector &&
		nHitRingX ==nHeaderRingX &&
		nHitRingY ==nHeaderRingY)
	    {
		dX[nHitCounter]=pPhotonHit->getPhi();
		dY[nHitCounter]=pPhotonHit->getTheta();
		dCh[nHitCounter]=pPhotonHit->getCharge();
		nHitCounter++;    
	    }

	} //end while photon hits
	if (nHitCounter!=nHeaderNbPhot) cout<<"Error:photon nb mismatch"<<endl;
	//if (!mX) mX = new TArrayD(nHitCounter,compress(dX,nHitCounter));
	mX->Set(nHitCounter,compress(dX,nHitCounter)); 
	
	//if (!mY) mY = new TArrayD(nHitCounter,compress(dY,nHitCounter));
	mY->Set(nHitCounter,compress(dY,nHitCounter));
	
	//if (!mCh) mCh = new TArrayD(nHitCounter,compress(dCh,nHitCounter));
	mCh->Set(nHitCounter,compress(dCh,nHitCounter));

	clearFitParams();
	// ===================================================================
	if (fit())  // <------ here the actual fitting algorithm is called !!!
	// ===================================================================
	{//check if fit is reasonable
	    
	    if (! ((dFittedCenterX-fHeaderRingPhi) < 2. &&
		(dFittedCenterY-fHeaderRingTheta) < 2. )  )
			unvalidFitParams();
	}else{unvalidFitParams();}
	switchXtoPhi();
	Int_t ind=-1;
	storeFitinHRichHitFit(nHeaderSector,nHitCounter,ind);
	//dumpFitParameters(nHeaderSector,nHeaderRingX,nHeaderRingY,-1.);
	
    } // end while photon hit header
    if (dX) delete [] dX; 
    if (dY) delete [] dY;
    if (dCh) delete [] dCh;
    return kTRUE;
}

void HRichChernovRingFitter::switchXtoPhi()
{
    dFittedCenterPhi=dFittedCenterX;
    dFittedCenterTheta=dFittedCenterY;
    dFittedCenterX=-1;
    dFittedCenterY=-1;
}

Bool_t HRichChernovRingFitter::fitFiredPads()
{
    // What to do with hits on the padplane border ?
    // They are not fitted correctly !
    HRichHit *pHit;
    pIterHit->Reset();
    Double_t *dX = new Double_t[arraysize];
    Double_t *dY = new Double_t[arraysize];
    Double_t *dCh = new Double_t[arraysize];
    // loop over each ring in event
    while((pHit = (HRichHit *)pIterHit->Next()))
    {
	
	Int_t nHitSector = pHit->getSector();
	Int_t nHitX = pHit->iRingX;
	Int_t nHitY = pHit->iRingY;
	HRichCal* pCal;
	for (Int_t i=0;i<arraysize;i++) dX[i]=dY[i]=dCh[i]=0;   
	pIterCal->Reset();
	//cout<<pIterCal<<endl;
	Int_t nPadCounter=0;
	// loop over fired pads in arraysize sized square around hit
	// and store them for fitting
	while((pCal = (HRichCal *)pIterCal->Next()))
	{
	    //pCal->dumpToStdout();
	   if (nHitSector == pCal->getSector())
	   {
	       Int_t nCalRow = pCal->getRow();
	       Int_t nCalCol = pCal->getCol();
	       Float_t nCalChrg = pCal->getCharge();
	       //pCal->dumpToStdout();
	       if(TMath::Abs(nCalRow-nHitY) <= fs &&
		  TMath::Abs(nCalCol-nHitX) <= fs)
	       {
		   dX[nPadCounter] = nCalCol;
		   dY[nPadCounter] = nCalRow;
		   dCh[nPadCounter] = nCalChrg;
		   //pCal->dumpToStdout();
		   nPadCounter++;
	       }
	   }
	} // endwhile cal container

	//if (!mX && dblx) mX = new TArrayD(nPadCounter,dblx);
	mX->Set(nPadCounter,compress(dX,nPadCounter)); 
	
	//if (!mY) mY = new TArrayD(nPadCounter,compress(dY,nPadCounter));
	mY->Set(nPadCounter,compress(dY,nPadCounter));
	
	//if (!mCh) mCh = new TArrayD(nPadCounter,compress(dCh,nPadCounter));
	mCh->Set(nPadCounter,compress(dCh,nPadCounter));

	
	
	clearFitParams();
	// ===================================================================
	if (fit())  // <------ here the actual fitting algorithm is called !!!
	// ===================================================================
	{//check if fit is reasonable
	    
	    if ((dFittedCenterX-nHitX) < 2. &&
		(dFittedCenterY-nHitY) < 2. )
	    {
		calcThetaAndPhi(pHit);
	    } 
	    else
	    {
		//cout<<"ERROR: Fitted center too far off !"<<endl;
		//dumpFitParameters(nHitSector,nHitX,nHitY,pHit->fRingRadius);
		unvalidFitParams();
	    }
		
	}
	else 
	{
	    //cout<<"ERROR: Fit unsuccessful !"<<endl;
	    //dumpFitParameters(nHitSector,nHitX,nHitY,pHit->fRingRadius);
	    unvalidFitParams();
	}
	Int_t ind = getHitCat()->getIndex(pHit);
	storeFitinHRichHitFit(pHit->getSector(),nPadCounter,ind);
	//dumpFitParameters(nHitSector,nHitX,nHitY,pHit->fRingRadius);
    } //endwhile hit container
    if (dX) delete [] dX; 
    if (dY) delete [] dY;
    if (dCh) delete [] dCh;
    return kTRUE;
}

void HRichChernovRingFitter::storeFitinHRichHitFit(Int_t s, Int_t n,Int_t i)
{
    //cout<<"in storeFitinHRichHitFit()"<<endl;
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

//  void HRichChernovRingFitter::storeFitParamsInHRichHit(HRichHit *hit)
//  {
//      //cout<<"in storeFitParamsInRichHit()"<<endl;
//      hit->setFitRadius(getFitRadius());
//      hit->setFitCenterX(getFitCenterX());
//      hit->setFitCenterY(getFitCenterY());
//      hit->setFitVar(getFitVar());
//      hit->setFitCenterTheta(getFitCenterTheta());
//      hit->setFitCenterPhi(getFitCenterPhi());
    
//  }

void HRichChernovRingFitter::clearFitParams()
{
    //cout<<"in clearFitParams()"<<endl;
    dFittedRadius = 0.;
    dFittedCenterX = 0.;
    dFittedCenterY = 0.;
    dFitVariance = 0.;
    dFittedCenterTheta = 0.;
    dFittedCenterPhi = 0.;
}

void HRichChernovRingFitter::unvalidFitParams()
{
    //cout<<"in clearFitParams()"<<endl;
    dFittedRadius = -1.;
    dFittedCenterX = -1.;
    dFittedCenterY = -1.;
    dFitVariance = -1.;
    dFittedCenterTheta = -1.;
    dFittedCenterPhi = -1.;
}

Double_t* HRichChernovRingFitter::compress(Double_t* arr,Int_t nPadCounter)
{
    //cout<<"in compress()"<<endl;
    Int_t nNonZeroElementCounter=0;
    Double_t* arr2=0;
    for (Int_t i=0;i<arraysize;i++)
    {
	if (arr[i] != 0) nNonZeroElementCounter++;
    }
    if (nNonZeroElementCounter==nPadCounter) // redundant check
    {
	arr2 = new Double_t[nNonZeroElementCounter];
	for (Int_t j=0;j<nNonZeroElementCounter;j++)
	{
	    arr2[j]=arr[j];
	}
    }
    else Error("compress","inconsistency");

    return arr2;
}

Bool_t HRichChernovRingFitter::fit()
{
    //cout<<"in fit()"<<endl;
    int i;
    double xx, yy, xx2, yy2;
    double f, g, h, p, q, t, g0, g02, a, b, c, d;
    double xroot, ff, fp, xd, yd, g1;
    double dx, dy, dradius2, xnom;
    
    double xgravity = 0.0;
    double ygravity = 0.0;
    double x2 = 0.0;
    double y2 = 0.0;
    double xy = 0.0;
    double xx2y2 = 0.0;
    double yx2y2 = 0.0;
    double x2y22 = 0.0;
    double radius2 = 0.0;

    mRC = 0;
    
    int npoints = mX->GetSize();
    
    if (npoints <= 3) {
	mRC = 1;
	cout<<"Error: less than 4 points for fit !!"<<endl;
	return kFALSE;
    }
   
    // arithmetic mean of points
    xgravity = mX->GetSum() / npoints;
    ygravity = mY->GetSum() / npoints;
        
    for (i=0; i<npoints; i++) {
	xx  = (*mX)[i]-xgravity; // coords relative to the mean 
	yy  = (*mY)[i]-ygravity;
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
	cout<<"Error: two many points on a line for fitting"<<endl;
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
	cout<<"Error: denominator zero !"<<endl;
	return kFALSE;
    }
    yd = (q*(f-g1)-h*p)/xnom;
    xnom = f-g1;
    if (xnom == 0.) {
	mRC = 4;
	cout<<"Error: denominator zero !"<<endl;
	return kFALSE;
    }
    xd = (p-h*yd )/xnom;
    
    radius2 = xd*xd+yd*yd+g1;
    dFittedCenterX = xd+xgravity;
    dFittedCenterY = yd+ygravity;
    
    for (i=0; i<npoints; i++) {
	dx = (*mX)[i]-(dFittedCenterX);
	dy = (*mY)[i]-(dFittedCenterY);
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
    //cout<<"in calcThetaAndPhi()"<<endl
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
	    cout<<"ERROR: no pointer to pad 0"<<endl;
	    dumpFitParameters(pHit->getSector(),pHit->iRingX,pHit->iRingY,
			      pHit->fRingRadius);
	    cout<<" PadX: "<<nPadX0<<" PadY: "<<nPadY0
		<<" FitX: "<<dFittedCenterX<<" FitY: "<<dFittedCenterY<<endl;
	}

	
	Float_t fThetaOfPadCenter1 = 0;
	//Float_t fPhiOfPadCenter1 = 0;
	HRichPad *pPad1 = pPadsPar->getPad(nPadX0,nPadY0-1);
	if (pPad1)
	{
	    fThetaOfPadCenter1 = pPad1->getTheta();
	    //fPhiOfPadCenter1   = pPad1->getPhi(sector);//for later use
	} else {
	    cout<<"ERROR: no pointer to pad 1"<<endl;
	    dumpFitParameters(pHit->getSector(),pHit->iRingX,pHit->iRingY,
			      pHit->fRingRadius);
	    cout<<" PadX: "<<nPadX0<<" PadY: "<<nPadY0-1
		<<" FitX: "<<dFittedCenterX<<" FitY: "<<dFittedCenterY<<endl;
	}


	//Float_t fThetaOfPadCenter2 = 0;
	Float_t fPhiOfPadCenter2 = 0;
	HRichPad *pPad2 = pPadsPar->getPad(nPadX0-1,nPadY0);
	if (pPad2)
	{
	    //fThetaOfPadCenter2 = pPad2->getTheta();//for later use
	    fPhiOfPadCenter2   = pPad2->getPhi(pHit->getSector());
	} else {
	    cout<<"ERROR: no pointer to pad 2"<<endl;
	    dumpFitParameters(pHit->getSector(),pHit->iRingX,pHit->iRingY,
			      pHit->fRingRadius);
	    cout<<" PadX: "<<nPadX0-1<<" PadY: "<<nPadY0
		<<" FitX: "<<dFittedCenterX<<" FitY: "<<dFittedCenterY<<endl;
	}


	//Float_t fThetaOfPadCenter3 = 0;
	Float_t fPhiOfPadCenter3 = 0;
	HRichPad *pPad3 = pPadsPar->getPad(nPadX0+1,nPadY0);
	if (pPad3)
	{
	    // fThetaOfPadCenter3 = pPad3->getTheta();//for later use
	    fPhiOfPadCenter3   = pPad3->getPhi(pHit->getSector());
	} else {
	    cout<<"ERROR: no pointer to pad 3"<<endl;
	    dumpFitParameters(pHit->getSector(),pHit->iRingX,pHit->iRingY,
			      pHit->fRingRadius);
	    cout<<" PadX: "<<nPadX0+1<<" PadY: "<<nPadY0
		<<" FitX: "<<dFittedCenterX<<" FitY: "<<dFittedCenterY<<endl;
	} 

 
	Float_t fThetaOfPadCenter4 = 0;
	//Float_t fPhiOfPadCenter4   = 0;
	HRichPad *pPad4 = pPadsPar->getPad(nPadX0,nPadY0+1);
	if (pPad4)
	{
	    fThetaOfPadCenter4 = pPad4->getTheta();
	    //fPhiOfPadCenter4   = pPad4->getPhi(sector);//for later use
	} else {
	    cout<<"ERROR: no pointer to pad 4"<<endl;
	    dumpFitParameters(pHit->getSector(),pHit->iRingX,pHit->iRingY,
			      pHit->fRingRadius);
	    cout<<" PadX: "<<nPadX0<<" PadY: "<<nPadY0+1
		<<" FitX: "<<dFittedCenterX<<" FitY: "<<dFittedCenterY<<endl;
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

