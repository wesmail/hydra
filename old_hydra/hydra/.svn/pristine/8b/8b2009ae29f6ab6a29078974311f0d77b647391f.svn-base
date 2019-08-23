using namespace std;
#include <stdlib.h>
#include <math.h>
#include <iostream> 
#include <iomanip>
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hrichringfindIPU.h"
#include "hrichanalysisIPU.h"
#include "hrichIPUparpattern.h"
#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"
#include <TMath.h>


//////////////////////////////////////////////////////////////////////////////

ClassImp(HRichRingFindIPU)
//----------------------------------------------------------------------------

 const char* HRichRingFindIPU::checkParamContext(void) {
  // Arbitrary choice to get the context from fRichIPUParThr!
  if (fRichIPUParThr) {    
    return fRichIPUParThr->getParamContext();
  } else {
    return NULL;
  }
}
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFindIPU::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb) {
  // Adds to the runtime database "rtdb" the containers needed by the RICH IPU
  //spec is used to get information of the spectrometer setup.
  fRichIPUParPat=(HRichIPUParPattern *)rtdb->getContainer("RichIPUParPattern");
  if (fRichIPUParPat) {
    cout << "RichIPU Parameters Pattern initialized" << endl;
  } else {
    cout << "******  RichIPU Parameters Pattern Initialization failed!! *********" << endl;
  }

  fRichIPUParThr=(HRichIPUParThresholds *)rtdb->getContainer("RichIPUParThresholds");
  if (fRichIPUParThr) {
    cout << "RichIPU Parameters Thresholds initialized" << endl;
  } else {
    cout << "******  RichIPU Parameters Thresholds Initialization failed!! *********" << endl;
  }

  fRichIPUParLmax=(HRichIPUParLocMax *)rtdb->getContainer("RichIPUParLocMax");
  if (fRichIPUParLmax) {
    cout << "RichIPU Parameters LocMax initialized" << endl;
  } else {
    cout << "******  RichIPU Parameters LocMax Initialization failed!! *********" << endl;
  }
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPU::HRichRingFindIPU() { 
 iRingNr = 0;
 pRings = NULL;
 }
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFindIPU::init(HRichAnalysisIPU* showMe) {
  cout << "-------------- INIT of HRichRingFindIPU() ------------------- " << endl;
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  // initialize parameters
    if (pRichDet)
      initParContainer(spec,rtdb);
  
 if (pRings) delete [] pRings;
 pRings=new HRichHitIPU[5000]; 
 showMe->pRings = pRings; 

 flag =0;
 version =0;
 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPU::~HRichRingFindIPU() {
 if (pRings) delete [] pRings;
 pRings = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPU::HRichRingFindIPU(const HRichRingFindIPU& source) {
 cerr << "HRichRingFindIPU object can not be initialized with values of another object!\n";
 cerr << "Default constructor will be called.\n";
 HRichRingFindIPU();
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPU& HRichRingFindIPU::operator=(const HRichRingFindIPU& source) {
 if (this != &source) {
  cerr << "HRichRingFindIPU object can not be assigned!\n";
  cerr << "Default constructor will be called.\n";
  HRichRingFindIPU();
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichRingFindIPU::makeLabelPattern() {

// --- derivation of the label mask from the pattern search mask --- 
// the labelling mask corresponds to the ring region of the pattern 
// search mask, where each pad is counted only once
   
  Int_t nn=0;
  for (Int_t xx=0; xx < fRichIPUParPat->getNGroups(); xx++) { // loop over groups
    if (fRichIPUParPat->getPattern(xx,0,1)==0)  { // ring region
      for (Int_t yy=1; yy <= fRichIPUParPat->getPattern(xx,0,0); yy++) { // loop over group lenght
	Int_t flag2 =0;
	for (Int_t zz=0; zz < fRichIPUParPat->getNPadDimensions(); zz++) { // loop over x,y
	  if(flag2==0) {
	    nn++;
	    flag2=1;
	  }
	  if ( (nn>=0) && (nn<65) && (zz>=0) && (zz<2) ) {
	    setLabelPattern(nn-1,zz,fRichIPUParPat->getPattern(xx,yy,zz)); // each pad should enter twice
	  } else {
	    cout << "ARRAY OUT OF SIZE (you never should read this!!) " << endl;
	  }
	}	  
      }
    }
  }

  label_size=0;
  for (Int_t ii=0; ii<nn; ii++){
    Int_t flag4=0;
    for (Int_t k=0; k<fRichIPUParPat->getNPadDimensions(); k++) {
      Int_t flag3=0;
      for (Int_t j=1; j<=ii; j++ ){
	// if the pad is already labelled	
	if (  (( (getLabelPattern(ii,0)) == (getLabelPattern(ii-j,0))) &&  
	       ( (getLabelPattern(ii,1)) == (getLabelPattern(ii-j,1))) )  ) {
	  flag3=1;
	} 
	
      } // j loop
      if (flag3==0){
	// i write the pad into the label patter only the first time it appears 
	// (when flag3 = 0)
	if(flag4==0) {
	  label_size++;
	  flag4=1;
	}
	setLabelPattern(label_size-1,k,getLabelPattern(ii,k));
      }
    } // k loop
  } // ii loop
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFindIPU::Execute() {
  // i make the label pattern once and forever (only when flag = 0)
  if (flag ==0) {
    makeLabelPattern();
    flag=1;
  }
  
  // initialisation of the variables to 0
 iRingNr = 0;
 lr = lc = 0;
 for (lr=0; lr<96;lr++) 
   for (lc=0; lc<96;lc++)
     label[lr][lc]=0;


 // loop over the pad plane
 for(int r=0; r<96; r++)  {
   for(int c=0; c<96; c++)  {
     if(pads[Sector][r][c])   { 

//----------------------------- Labeling -------------------------------------
// potential ring centers are all the pads lying on a ring around any fired pad, 
// weighted by the number of times they have been found

       for(Int_t lnr=0; lnr < label_size; lnr++)  {
	 lr = r + getLabelPattern(lnr,0); 
	 lc = c + getLabelPattern(lnr,1); 
	 if (lr>=0&&lr<=95&&lc>=0&&lc<=95 ) { // boundaries of the padplane
	   label[lr][lc]++;
	 }
       }
     }
   }
 }
//----------------------------- Ring Finder -----------------------------------
 RingSearch();
 MaxSearch();
 
 return (iRingNr);
}
//============================================================================


//----------------------------------------------------------------------------
void HRichRingFindIPU::ZeroArray() {

   for(Int_t i=0; i<6; i++)
     for(Int_t j=0; j<96; j++)
       for(Int_t k=0; k<96; k++)
	 pads[i][j][k] = 0;
   
}
//============================================================================

//----------------------------------------------------------------------------
//------------------------- Ring Recognition ---------------------------------
// A pattern (patterCoord) with a ring region and a veto region is applied
// to each pad found in the labeling process (potential ring center).
// The ring and veto regions of the pattern consist of groups.
// In each group is enough if 1 pad is fired ("logical OR") to consider 
// the group fired.
// The groups in each region are summed up.
// A threshold (fRichIPUParThr->getThresholds(re,sec)) is set independently 
// for each of the two regions.

void HRichRingFindIPU::RingSearch(){
  // version 1: local maximum search on ringsum for ALL pads 
  //             (used in hardware until beginning of 04)
  // version 2: local maximum search on ringsum for pads over threshold
  // version 3: combined thresholds
  version =2;

  Int_t iNumberOfGroups = fRichIPUParPat->getNGroups();
  Int_t iNumberOfRegions = 2;

  Int_t iGroupsInRegion[iNumberOfRegions];
  Int_t px, py;
  Int_t iPadsInGroup;
  Int_t iTotPads;
  Int_t iNumberOfValidRegions;

  Int_t patternHit[iNumberOfGroups];

// initialisation of centers to 0 
    for(Int_t j=0; j<96; j++) {
      for(Int_t k=0; k<96; k++) { 
	centers[j][k] = 0; 
	ringsum[j][k] = 0;
	vetosum[j][k] = 0;
	nPads[j][k] = 0;
      }
    }
  

  // loop over potential ring centers
  for(Int_t lr=0; lr<96; lr++)  {
    for(Int_t lc=0; lc<96; lc++)  {

      if(label[lr][lc] >0) {  // labelling condition

	// initialisation of variables to 0
	iNumberOfValidRegions = 0;

	for (Int_t j=0; j< iNumberOfRegions; j++){
	  iGroupsInRegion[j]=0;
	}	
	iTotPads = 0;

	// loop over groups
	for (Int_t i=0; i< iNumberOfGroups; i++){
	  patternHit[i] = 0;
	  iPadsInGroup = 0;

	// loop over pads in group
	  for(Int_t j=1; j<= fRichIPUParPat->getPattern(i,0,0) ; j++){

	    px = lr + fRichIPUParPat->getPattern(i,j,0);  
	    py = lc - fRichIPUParPat->getPattern(i,j,1);
	    if(px>=0 && px <=95 && py>=0 && py<=95){ // boundaries of pad plane

	      if (pads[Sector][px][py]) { // if pad is fired

		iPadsInGroup++; 
		if ( fRichIPUParPat->getPattern(i,0,1)  == 0 ) // only pads in groups of ring region
		  iTotPads++;                                  // are counted to get Npads
	      }
	    }
	  } // end of loop over pads in group
	 
	  if ( iPadsInGroup > 0 )  {    // at least one pad fired (logical OR)
	    iGroupsInRegion[fRichIPUParPat->getPattern(i,0,1)]++;
	    patternHit[i] = 1;
	  }
	} // end of loop over groups
	
	//cout << "RING FIND: thresholds " << fRichIPUParThr->getThresholds(0,Sector) << " " << fRichIPUParThr->getThresholds(1,Sector) << endl;
	if (version==1) {
	    for (Int_t j=0; j< iNumberOfRegions; j++){
		// here thresholds are applied independently for ring and veto region
		if (  (fRichIPUParThr->getThresholds(j,Sector)) >= 0 
		      && iGroupsInRegion[j] > (fRichIPUParThr->getThresholds(j,Sector)) || 
		      (fRichIPUParThr->getThresholds(j,Sector)) < 0 
		      && iGroupsInRegion[j] < -(fRichIPUParThr->getThresholds(j,Sector)) ) {
		    iNumberOfValidRegions++;
		}
	    }
	    ringsum[lr][lc]  = iGroupsInRegion[0];
	    vetosum[lr][lc]  = iGroupsInRegion[1];

	    if ( iNumberOfValidRegions ==  iNumberOfRegions ) {  // We  have found a ring candidate!!!!
		centers[lr][lc] = 1;
		nPads[lr][lc] = iTotPads/2;
	    }
	}
	
	else if (version==2) {
	    for (Int_t j=0; j< iNumberOfRegions; j++){
		// here thresholds are applied independently for ring and veto region
		if (  (fRichIPUParThr->getThresholds(j,Sector)) >= 0 
		      && iGroupsInRegion[j] > (fRichIPUParThr->getThresholds(j,Sector)) || 
		      (fRichIPUParThr->getThresholds(j,Sector)) < 0 
		      && iGroupsInRegion[j] < -(fRichIPUParThr->getThresholds(j,Sector)) ) {
		    iNumberOfValidRegions++;
		}
	    }
	    
	    if ( iNumberOfValidRegions ==  iNumberOfRegions ) {  //We  have found a ring candidate!!!!
		centers[lr][lc] = 1;
		nPads[lr][lc] = iTotPads/2;
		ringsum[lr][lc]  = iGroupsInRegion[0];
		vetosum[lr][lc]  = iGroupsInRegion[1];
	    }
	}
	
	else if (version==3) {
	    iNumberOfValidRegions=1;
	    // here thresholds are applied together for ring and veto region
	    if (iGroupsInRegion[0]-iGroupsInRegion[1] > fRichIPUParThr->getThresholds(0,Sector)) {
		iNumberOfValidRegions++;
	    }
	 
	    if ( iNumberOfValidRegions ==  iNumberOfRegions ) {  // We  have found a ring candidate!!!!
		centers[lr][lc] = 1;
		nPads[lr][lc] = iTotPads/2;
		ringsum[lr][lc]  = iGroupsInRegion[0];
		vetosum[lr][lc]  = iGroupsInRegion[1];
	    }
	}
	

      }
    }
  }
  
}
//============================================================================


//----------------------------------------------------------------------------
//------------------------- Local Max Search ---------------------------------
// The ring search algo is likely to identify one or more neighbouring pads
// as the center of a potential ring candidate.
// Therefore, a local maximum search over 4/ 8 directly neighbouring
// pads is performed to determine the center.   

void HRichRingFindIPU::MaxSearch(){
  // loop over found ring centers

  for(Int_t lr=0; lr<96; lr++)  {
    for(Int_t lc=0; lc<96; lc++)  {
      if(centers[lr][lc] >0) { 

	// search over 4 or 8 neighbouring pads
	Int_t fulfill=0;
	if  (fRichIPUParLmax->getLocMaxFlag()==1) {
	  if ( (fRichIPUParLmax->getLocMax()==4)||(fRichIPUParLmax->getLocMax()==8) )  {
	    
	    if(   ((version==1)&&
		   ( ringsum[lr][lc] >= ringsum[lr+1][lc] &&
		     ringsum[lr][lc] >= ringsum[lr][lc+1] &&
		     ringsum[lr][lc] >  ringsum[lr-1][lc] &&
		     ringsum[lr][lc] >  ringsum[lr][lc-1]   ) ) ||
		  ((version==2)&&
		   ( ringsum[lr][lc] > ringsum[lr+1][lc] &&
		     ringsum[lr][lc] >= ringsum[lr][lc+1] &&
		     ringsum[lr][lc] >=  ringsum[lr-1][lc] &&
		     ringsum[lr][lc] >  ringsum[lr][lc-1]   ) ) ){
	      
	      if (fRichIPUParLmax->getLocMax()==8) {
		if( ringsum[lr][lc] >= ringsum[lr+1][lc+1] &&
		    ringsum[lr][lc] >= ringsum[lr-1][lc+1] &&
		    ringsum[lr][lc] >  ringsum[lr-1][lc-1] &&
		    ringsum[lr][lc] >  ringsum[lr+1][lc-1] ) {
		  fulfill=1;
		}
	      } else {
		fulfill=1;	    
	      }	      
	    }
	  } else {
	    cout << " wrong parameter value!!!" << endl;
	  }
	} else {
	  fulfill=1;
	}
	
	if ( fulfill==1 ) { // fill the ring container

	  HRichHitIPU & ring = pRings[iRingNr];
	  if ( iRingNr < 4999 ) {
	    iRingNr++;
	  } else {
	    cout << "iRingNr reached MAXIMUM!!!" << endl;
	  }
	  // now i store the ring info
 	  ring.iRingX = lc; 
 	  ring.iRingY = lr;
 	  ring.iRingQuality = ringsum[lr][lc];
 	  ring.iVetoQuality = vetosum[lr][lc];
 	  ring.nPads = nPads[lr][lc];
	}	
      }
    }
  }
}
//============================================================================











































