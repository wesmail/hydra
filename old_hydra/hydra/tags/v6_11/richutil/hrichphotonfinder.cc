// $Id: hrichphotonfinder.cc,v 1.3 2002-09-25 16:19:53 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:43:51
//
//**********************************************************
// this class is meant to identify photon hit locations on
// the padplane 
//**********************************************************
#include "hrichphotonfinder.h"
#include "hrichphotonhit.h"
#include "hrichphotonhitheader.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hrichgeometrypar.h"
#include "hrichcalpar.h"
#include "hrichhit.h"
#include "hrichcal.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hlinearcategory.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include <TMath.h>

ClassImp(HRichPhotonFinder)

HRichPhotonFinder::HRichPhotonFinder(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
     pIterCal=NULL;
     pIterHit=NULL;
     //     pIterPhotonHit=NULL;
     fs = 6; //size of square to be cut out around hit point
     evtcounter=0;
     pCalCat=NULL;
     pHitCat=NULL;
     pPhotonHitCat=NULL;
     pGeomPar=NULL;
}

HRichPhotonFinder::~HRichPhotonFinder(void) {
    if (pIterCal) delete pIterCal;
    if (pIterHit) delete pIterHit;
    //    if (pIterPhotonHit) delete pIterPhotonHit;
    //if (mX) delete mX;
    //if (mY) delete mY;
    //if (mCh) delete mCh;
   //   if (dX) delete dX;
//      if (dY) delete dY;
//      if (dCh) delete dCh;
}

Bool_t HRichPhotonFinder::init() {

    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
	->getDetector("Rich");
    
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    // GEOMETRY PARAMETER CONTAINER
    HRichGeometryPar *pGeomPar = 
	(HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
//     if (!pGeomPar) {
// 	pGeomPar = new HRichGeometryPar;
// 	rtdb->addContainer(pGeomPar);
//     }
    setGeometryPar(pGeomPar);
    if (!getGeometryPar()) 
    {
	cout<<"ERROR in init: no pointer to geom container"
	    <<endl;
    }

    // CALIBRATION PARAMETER CONTAINER
    initCalPar();

    // RICH CAL container
    pCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    if (!pCalCat) {
	pCalCat=pRichDet->buildCategory(catRichCal);

	if (!pCalCat) return kFALSE;
	else gHades->getCurrentEvent()
		 ->addCategory(catRichCal, pCalCat, "Rich");
    }
    pIterCal = (HMatrixCatIter*)getCalCat()->MakeIterator();
    // RICH HIT container
    pHitCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    if (!pHitCat) {
	pHitCat=pRichDet->buildCategory(catRichHit);
	
      if (!pHitCat) return kFALSE;
      else gHades->getCurrentEvent()
	       ->addCategory(catRichHit, pHitCat, "Rich");
    }
    
    pIterHit = (HIterator*)getHitCat()->MakeIterator();
    
    // RICH PHOTON HIT container
    pPhotonHitCat=gHades->getCurrentEvent()->getCategory(catRichPhotonHit);
    if (!pPhotonHitCat) {
	pPhotonHitCat=pRichDet->buildCategory(catRichPhotonHit);
	
	if (!pPhotonHitCat) return kFALSE;
	else gHades->getCurrentEvent()
		 ->addCategory(catRichPhotonHit, pPhotonHitCat, "Rich");
    }
    

    // RICH PHOTON HIT HEADER container
    pPhotonHitHeaderCat=gHades->getCurrentEvent()
	->getCategory(catRichPhotonHitHeader);
    if (!pPhotonHitHeaderCat) {
	pPhotonHitHeaderCat=pRichDet->buildCategory(catRichPhotonHitHeader);
	
	if (!pPhotonHitHeaderCat) return kFALSE;
	else gHades->getCurrentEvent()
		 ->addCategory(catRichPhotonHitHeader, pPhotonHitHeaderCat
			       , "Rich");
    }
    //cout<<"end of init()"<<endl;    
    return kTRUE;
}
void HRichPhotonFinder::printCalParCell(HLocation& loc) {
    HRichCalParCell *calparcell = NULL;
    calparcell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())
	->getObject(loc);
    cout<<"SEC: "<<loc[0]<<" ROW: "<<loc[1]<<" COL: "<<loc[2]<<
	" OFFSET: "<<calparcell->getOffset()<<" SIGMA : "<<
	calparcell->getSigma()<<" SLOPE : "<<calparcell->getSlope()<<
	endl;
} 

void HRichPhotonFinder::initCalPar() {
    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
	->getDetector("Rich");
    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    pCalPar = rtdb->getContainer("RichCalPar");
    if (!pCalPar) {
	pCalPar= new HRichCalPar;
	pCalPar->SetName("RichCalPar");
	if (pCalPar) {
	    ((HRichCalPar*)pCalPar)->
		setSetup(pRichDet->getSectors(),
			 pRichDet->getRows(),
			 pRichDet->getColumns());
	    rtdb->addContainer(pCalPar);
	}
    }
}

Bool_t HRichPhotonFinder::finalize() {
  
    return kTRUE;
}

Int_t HRichPhotonFinder::execute()
{
    //cout<<"in execute()"<<endl;
    HRichHit *pHit;
    pIterHit->Reset();
    // an area of (2*fs+1)^2 is cut around the ring center
    Int_t arraysize = (2*fs+1)*(2*fs+1);
    Int_t *nX = new Int_t[arraysize];
    Int_t *nY = new Int_t[arraysize];
    Float_t *fCh = new Float_t[arraysize];
    Float_t *fSigCh = new Float_t[arraysize];
    // loop over each ring in event
    while((pHit = (HRichHit *)pIterHit->Next()))
    {
	//cout<<evtcounter<<" im hit loop"<<endl;
	Int_t nHitSector = pHit->getSector();
	Int_t nHitX = pHit->iRingX;
	Int_t nHitY = pHit->iRingY;
	HRichCal* pCal;
	for (Int_t i=0;i<arraysize;i++) nX[i]=nY[i]=0;
	for (Int_t i=0;i<arraysize;i++) fCh[i]=fSigCh[i]=0.;   
	pIterCal->Reset();
	Int_t nPadCounter=0;
	// loop over fired pads in fs*fs sized square around hit
	// and store them for loc max search
	while((pCal = (HRichCal *)pIterCal->Next()))
	{
	    // HLocation calLoc = pIterCal->getLocation();
	    // this method returns an ill HLocation, why ??
	    
	    if (pCal && nHitSector == pCal->getSector())
	   {
	       Int_t nCalRow = pCal->getRow();
	       Int_t nCalCol = pCal->getCol();
	       
	       //Float_t nCalChrg = pCal->getCharge();
	       if(TMath::Abs(nCalRow-nHitY) <= fs &&
		  TMath::Abs(nCalCol-nHitX) <= fs)
	       {

		   // pads on a square of size fs*fs around
		   // the hit point are stored
		   // the charge is stored
		   // and the charge reduced by the 3 sigma
		   // threshold in the frontends is stored
		   // these charges are used for calcImpactByGravity
		   
		   HLocation cloc;
		   cloc.set(3,0,0,0);
		   cloc.setOffset(nCalCol);
		   cloc.setIndex(1,nCalRow);
		   cloc.setIndex(0,nHitSector);
		   nX[nPadCounter] = nCalCol;
		   nY[nPadCounter] = nCalRow;
		   fCh[nPadCounter] = pCal->getCharge();
		   //		   printCalParCell(cloc);
		   fSigCh[nPadCounter] = pCal->getCharge() -
		       3*((HRichCalPar*)getCalPar())->getSigma(cloc);
		   if (fSigCh[nPadCounter]<0.) fSigCh[nPadCounter]=0.;
		   //cout<<fCh[nPadCounter]<<" "<<fSigCh[nPadCounter]<<endl;
		   nPadCounter++;
	       }
	   }
	} // endwhile cal container
	
	Int_t numberOfPads = nPadCounter;
	Int_t *nPadChSortedIdx = new Int_t[arraysize];
	// sort indices of charge array in descending order
	TMath::Sort(numberOfPads,fSigCh,nPadChSortedIdx);
	findLocMax(numberOfPads,nX,nY,fCh,fSigCh,nPadChSortedIdx,pHit);
	if (nPadChSortedIdx) delete [] nPadChSortedIdx;
	//cout<<"Number of input pads: "<<numberOfPads<<endl;
    } // endwhile hit container
    //cout<<"before delete in execute "<<evtcounter<<endl;
    if (nX) delete [] nX;
    if (nY) delete [] nY;
    if (fCh) delete [] fCh;
    if (fSigCh) delete [] fSigCh;
    //cout<<"end of execute "<<evtcounter<<endl;
    evtcounter++;
    return 0;
}

Bool_t HRichPhotonFinder::findLocMax(Int_t numberOfPads,Int_t *x,
				     Int_t *y,Float_t *c,Float_t *cs,
				     Int_t* sortidx,
				     HRichHit *pHit)
{
    // loop over sorted local maxima in descending order
    
    Int_t nPhotonCounter = 0;
    Int_t xr       = pHit->iRingX;
    Int_t yr       = pHit->iRingY;
    Float_t phir   = pHit->getPhi();
    Float_t thetar = pHit->getTheta();
    Int_t sec      = pHit->getSector();
    //  cout<<"Evt: "<<evtcounter<<" Ring x: "<<xr<<" Ring y: "
//  	<<yr<<" Sector: "<<sec<<" Nb of pads: "<<numberOfPads<<endl;
    for (Int_t j=0;j<numberOfPads;j++)
    {
	// current local max under inspection
	Int_t nLocalMaxX = x[sortidx[j]];
	Int_t nLocalMaxY = y[sortidx[j]];
//  	cout<<"loc max x: "<<nLocalMaxX
//  	    <<" loc max y: "<<nLocalMaxY
//  	    <<" loc max chrg: "<<c[sortidx[j]]
//  	    <<" loc max sig chrg: "<<cs[sortidx[j]]
//  	    <<endl;
	if (! (isLocalMaxOnBorder(nLocalMaxX,nLocalMaxY,xr,yr)) ){
	    if ( isLocalMaxOfFive(x,y,cs,sortidx[j]) )
	    {
		if (isPhoton())
		{
		    Float_t *fPhotonParams = 
			calcImpactByGravity(x,y,c,cs,sortidx[j]);
		        HLocation loc;
			loc.set(1,sec);
			HRichPhotonHit *photon = (HRichPhotonHit*)
			    ((HLinearCategory*)getPhotonHitCat())
			    ->getNewSlot(loc);
			if(photon)
			{
			    photon = new (photon) HRichPhotonHit;
			    photon->setX(fPhotonParams[0]);
			    photon->setY(fPhotonParams[1]);
			    photon->setCharge(fPhotonParams[2]);
			    photon->setSector(sec);
			    photon->setRingX(xr);
			    photon->setRingY(yr);
			    setThetaPhi(photon);
			    nPhotonCounter++;
			}
			
		}//endif isPhoton 
		else {}
	    }//endif isLocalMaxOfFive 
	    else {}
	}//endif isLocalMaxOnBorder  
	else {}
    } //end loop over pads
    //cout<<"Evt: "<<evtcounter<<" - Number of photons found "
    //<<nPhotonCounter<<endl;
    if (nPhotonCounter >= 3) //otherwise it makes no sense trying to fit later
    {
	
	HLocation loc;
	loc.set(1,sec);
	HRichPhotonHitHeader *phhitheader = (HRichPhotonHitHeader*)
	    ((HLinearCategory*)getPhotonHitHeaderCat())
	    ->getNewSlot(loc);
			if(phhitheader)
			{
			  phhitheader = new (phhitheader) HRichPhotonHitHeader;
			  phhitheader->setSector(sec);
			  phhitheader->setRingX(xr);
			  phhitheader->setRingY(yr);
			  phhitheader->setRingPhi(phir);
			  phhitheader->setRingTheta(thetar);
			  phhitheader->setNbPhotons(nPhotonCounter);
			}
        return kTRUE;
    } else return kFALSE;
}

Bool_t HRichPhotonFinder::isLocalMaxOnBorder(Int_t xmax, Int_t ymax, Int_t xr,Int_t yr){
    // is the local max candidate on the border of the search area ?
    Int_t xshifted = xmax-(xr-fs);
    Int_t yshifted = ymax-(yr-fs);
    //cout<<"xshifted :"<<xshifted<<" yshifted: "<<yshifted<<endl;
    if ( xshifted>0 && xshifted<fs+fs && 
         yshifted>1 && yshifted<fs+fs    )
    {
	return kFALSE;
    }else{
	return kTRUE;
    }
}

Bool_t HRichPhotonFinder::isLocalMaxOfFive(Int_t *x, Int_t *y, Float_t *c, Int_t max){
    // max is the index in x,y,c of the loc max candidate
    //cout<<"inside function isLocalMaxOfFive "<<xmax<<" "<<ymax<<endl;
    
    Float_t fChargeCenter = c[max];
    Float_t fChargeUpper  = 0.;
    Float_t fChargeLower  = 0.;
    Float_t fChargeRight  = 0.;
    Float_t fChargeLeft   = 0.;
    for (Int_t i=0;i<(2*fs+1)*(2*fs+1);i++)
    {
	if (x[i] != 0 && y[i] != 0)
	{
	    if( x[i] == x[max]   && y[i] == y[max]+1 ) fChargeUpper=c[i]; 
	    if( x[i] == x[max]   && y[i] == y[max]-1 ) fChargeLower=c[i]; 
	    if( x[i] == x[max]+1 && y[i] == y[max]   ) fChargeRight=c[i]; 
	    if( x[i] == x[max]-1 && y[i] == y[max]   ) fChargeLeft =c[i]; 
	}

    }
    //  cout<<fChargeCenter<<" "<<fChargeUpper<<" "<<fChargeLower
//  	<<" "<<fChargeRight<<" "<<fChargeLeft<<endl;
    if (fChargeCenter > fChargeUpper &&
	fChargeCenter > fChargeLower &&
	fChargeCenter > fChargeRight &&
	fChargeCenter > fChargeLeft     ){
	return kTRUE;
    }else{
	return kFALSE;
    }
}

//void HRichPhotonFinder::subtractThreshold(){
    // in order to improve the spatial resolution when constructing
    // the photon impact point by wheighted sums, one should
    // subtract the threshold that was set in the electronics when taking data.
    // This increases the pulse height ratios contributing to the averaging.
    // cf.: Schneider, R.; Diploma Thesis 1991; TU Muenchen; unpublished
    //  Float_t fChOffset = 1.0; // change this later to real calculated sigmas
//      for (Int_t i=0;i<fieldsize;i++){
//  	for (Int_t j=0;j<fieldsize;j++){
//  	    fSearchArea[i][j] = fSearchArea[i][j] - fChOffset;
//  	}
//      }
//}

Float_t* HRichPhotonFinder::calcImpactByGravity(Int_t *x, Int_t *y, Float_t *c,Float_t *cs, Int_t max){

    Float_t *fPhotonParams = new Float_t[5];
    Float_t fChargeCenter = cs[max];
    Float_t fChrgSum      = c[max];
    Float_t fChargeUpper  = 0.;
    Float_t fChargeLower  = 0.;
    Float_t fChargeRight  = 0.;
    Float_t fChargeLeft   = 0.;

    for (Int_t i=0;i<(2*fs+1)*(2*fs+1);i++)
    {
	if (x[i] != 0 && y[i] != 0)
	{
	    if( x[i] == x[max]   && y[i] == y[max]+1 ) fChargeUpper=cs[i];
	    if( x[i] == x[max]   && y[i] == y[max]+1 ) fChrgSum   += c[i];
	    if( x[i] == x[max]   && y[i] == y[max]-1 ) fChargeLower=cs[i];
	    if( x[i] == x[max]   && y[i] == y[max]-1 ) fChrgSum   += c[i];
	    if( x[i] == x[max]+1 && y[i] == y[max]   ) fChargeRight=cs[i];
	    if( x[i] == x[max]+1 && y[i] == y[max]   ) fChrgSum   += c[i];
	    if( x[i] == x[max]-1 && y[i] == y[max]   ) fChargeLeft =cs[i]; 
	    if( x[i] == x[max]-1 && y[i] == y[max]   ) fChrgSum   += c[i];
	}

    }


    // x coord of photon impact
    fPhotonParams[0] = 
	( x[max]*fChargeCenter +
	  (x[max]-1)*fChargeLeft+
	  (x[max]+1)*fChargeRight) / 
	( fChargeCenter + fChargeRight + fChargeLeft );
    // y coord of photon impact
    fPhotonParams[1] =
	( y[max]*fChargeCenter +
	  (y[max]-1)*fChargeLower+
	  (y[max]+1)*fChargeUpper) / 
	( fChargeCenter + fChargeLower + fChargeUpper );
    // summed charge produced by photon
    fPhotonParams[2] = fChrgSum;

    return fPhotonParams;
}
void HRichPhotonFinder::dumpPhotonParameters()
{

}

void HRichPhotonFinder::setThetaPhi(HRichPhotonHit *photon)
{
    // interpolate the theta and phi values from the RTDB
    // which are for the respective pad centers
    // in order to retrieve the correct angles for the hit point
    //       4
    //    2  0  3
    //       1
    //cout<<"in setThetaPhi()"<<endl;
    HRichPadTab *pPadsPar = ((HRichGeometryPar*)getGeometryPar())
	                     ->getPadsPar();
    Float_t fPhotonX = photon->getX();
    Float_t fPhotonY = photon->getY();
    Int_t nPadX0 = (Int_t) TMath::Floor(fPhotonX);
    Int_t nPadY0 = (Int_t) TMath::Floor(fPhotonY);
    Float_t fPhotonPhi   = 0.;
    Float_t fPhotonTheta = 0.;
    if (pPadsPar)
    {
	Float_t fThetaOfPadCenter0 = 0;
	Float_t fPhiOfPadCenter0   = 0;
	HRichPad *pPad0 = pPadsPar->getPad(nPadX0,nPadY0);
	if (pPad0)
	{
	    fThetaOfPadCenter0 = pPad0->getTheta();
	    fPhiOfPadCenter0   = pPad0->getPhi(photon->getSector());
	} else {
	    cout<<"ERROR: no pointer to pad 0"<<endl;
	    cout<<" PadX: "<<nPadX0<<" PadY: "<<nPadY0
		<<" photon X: "<<fPhotonX
		<<" photon Y: "<<fPhotonY<<endl;
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
	    cout<<" PadX: "<<nPadX0<<" PadY: "<<nPadY0-1
		<<" photon X: "<<fPhotonX
		<<" photon Y: "<<fPhotonY<<endl;
	}


	//Float_t fThetaOfPadCenter2 = 0;
	Float_t fPhiOfPadCenter2 = 0;
	HRichPad *pPad2 = pPadsPar->getPad(nPadX0-1,nPadY0);
	if (pPad2)
	{
	    //fThetaOfPadCenter2 = pPad2->getTheta();//for later use
	    fPhiOfPadCenter2   = pPad2->getPhi(photon->getSector());
	} else {
	    cout<<"ERROR: no pointer to pad 2"<<endl;
	    cout<<" PadX: "<<nPadX0-1<<" PadY: "<<nPadY0-1
		<<" photon X: "<<fPhotonX
		<<" photon Y: "<<fPhotonY<<endl;

	}


	//Float_t fThetaOfPadCenter3 = 0;
	Float_t fPhiOfPadCenter3 = 0;
	HRichPad *pPad3 = pPadsPar->getPad(nPadX0+1,nPadY0);
	if (pPad3)
	{
	    // fThetaOfPadCenter3 = pPad3->getTheta();//for later use
	    fPhiOfPadCenter3   = pPad3->getPhi(photon->getSector());
	} else {
	    cout<<"ERROR: no pointer to pad 3"<<endl;
	    cout<<" PadX: "<<nPadX0+1<<" PadY: "<<nPadY0
		<<" photon X: "<<fPhotonX
		<<" photon Y: "<<fPhotonY<<endl;
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
	    cout<<" PadX: "<<nPadX0<<" PadY: "<<nPadY0+1
		<<" photon X: "<<fPhotonX
		<<" photon Y: "<<fPhotonY<<endl;
	} 



	// phi interpolation
	//	cout<<"phi interpolation"<<endl;
	
	if (fPhotonX-nPadX0 == 0.5) fPhotonPhi=fPhiOfPadCenter0;
	if (fPhotonX-nPadX0 > 0.5)
	{
	    fPhotonPhi = (fPhotonX-0.5-nPadX0)*
		(fPhiOfPadCenter3-fPhiOfPadCenter0) + fPhiOfPadCenter0;
	}
	if (fPhotonX-nPadX0 < 0.5)
	{
	    fPhotonPhi = (fPhotonX+0.5-nPadX0)*
		(fPhiOfPadCenter0-fPhiOfPadCenter2) + fPhiOfPadCenter2;
	}



	// theta interpolation
	//cout<<"theta interpolation"<<endl;
	if (fPhotonY-nPadY0 == 0.5) fPhotonTheta=fThetaOfPadCenter0;
	if (fPhotonY-nPadY0 > 0.5)
	{
	    fPhotonTheta = (fPhotonY-0.5-nPadY0)*
		(fThetaOfPadCenter4-fThetaOfPadCenter0) + fThetaOfPadCenter0;   
	}
	if (fPhotonY-nPadY0 < 0.5)
	{
	    fPhotonTheta = (fPhotonY+0.5-nPadY0)*
		(fThetaOfPadCenter0-fThetaOfPadCenter1) + fThetaOfPadCenter1;   
	}


	// FIXME: if e.g. theta of 3 and 0 are not equal, one has to calc
	// the difference and add to dFittedCenterTheta according to the phi !!
	// but this should be a very small effect
    }
    else cout<<"ERROR: no pointer to RTDB"<<endl;

    photon->setTheta(fPhotonTheta);
    photon->setPhi(fPhotonPhi);
}

