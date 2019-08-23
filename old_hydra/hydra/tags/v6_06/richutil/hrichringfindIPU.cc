#include <stdlib.h>
#include <math.h>
#include <iostream.h>
#include "hrichringfindIPU.h"
#include "hrichanalysisIPU.h"
#include <TMath.h>

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////

ClassImp(HRichRingFindIPU)

//----------------------------------------------------------------------------
HRichRingFindIPU::HRichRingFindIPU() { 
 iRingNr = 0;
 pRings = NULL;
 }
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFindIPU::init(HRichAnalysisIPU* showMe) {

 if (pRings) delete [] pRings;
 pRings=new HRichHitIPU[5000]; //5000 non vuol dire un cazzo
 showMe->pRings = pRings; 

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPU::~HRichRingFindIPU() {
 if (pRings) delete [] pRings;

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
Int_t HRichRingFindIPU::Execute() {

// this matrix should be put in the parameters when they will exist!!!
int labelpattern[32][2] = {{0,4},
                             {1,4},
                             {2,4},
                             {2,3},
                             {3,3},
                             {3,2},
                             {4,2},
                             {4,1},
                             {4,0},
                             {4,-1},
                             {4,-2},
                             {2,-3},
                             {3,-3},
                             {3,-2},
                             {2,-4},
                             {1,-4},
                             {0,-4},
                             {-1,-4},
                             {-2,-4},
                             {-2,-3},
                             {-3,-3},
                             {-3,-2},
                             {-4,-2},
                             {-4,-1},
                             {-4,0},
                             {-4,1},
                             {-4,2},
                             {-2,3},
                             {-3,3},
                             {-3,2},
                             {-2,4},
                             {-1,4}}; 

  // initialisation of the variables to 0
 iRingNr = 0;
 lr = lc = 0;
 for (lr=0; lr<96;lr++) 
   for (lc=0; lc<96;lc++)
     label[lr][lc]=0;


 // loop over the pad plane
 for(int r=0; r<96; r++)  {
   for(int c=0; c<96; c++)  {
     if(pads[Sector][r][c]) { 

//***************************** Labeling ************************************
//   potential ring centers all the pads lying on a ring around any fired pad, 
//   weighted by the number of times they have been found

       for(Int_t lnr=0; lnr < 24; lnr++)  {
	 lr = r + labelpattern[lnr][0];
	 lc = c + labelpattern[lnr][1]; 
	 //	 cout << "lr: " << lr << "   lc: " << lc << endl;
	 if (lr>=0&&lr<=95&&lc>=0&&lc<=95 ) { // boundaries of the padplane
	   label[lr][lc]++;
//  	   cout << "label[" << lr << "][" << lc << "]:    " 
//  		<< label[lr][lc] << endl;
	 }

       }
     }
   }
 }

//***************************** Ring Finder **********************************
 RingSearch();
 MaxSearch();

 return (iRingNr);
}
//============================================================================


//----------------------------------------------------------------------------
void HRichRingFindIPU::ZeroArray() {

  // the number of sectors, rows and cols must be taken from somewhere else!!!!
   for(Int_t i=0; i<6; i++)
     for(Int_t j=0; j<96; j++)
       for(Int_t k=0; k<96; k++)
	 pads[i][j][k] = 0;

}
//============================================================================


//----------------------------------------------------------------------------
//************************* Ring Recognition **********************************
// A pattern (patterCoord) with a ring region and a veto region is applied
// to each pad found in the labeling process.
// A threshold (iRegionThreshold) is set for each of the two regions.

void HRichRingFindIPU::RingSearch(){
  Int_t iNumberOfRegions = 2;
  Int_t iGroupsInRegion[iNumberOfRegions];
  Int_t px, py;
  Int_t iPadsInGroup;
  Int_t iNumberOfValidRegions;

  Int_t iNumberOfGroups = 29;

  Int_t  iRegionThresholds[iNumberOfRegions][6];

//// OLD PATTERNCOORD
//    Int_t patternCoord[29][5][2]=
//                                 {{{4,0},{-4,-1},{-4,0},{-4,1},{-4,2}},
//  				{{4,0},{-4,1},{-4,2},{-3,2},{-3,3}},
//  				{{4,0},{-3,2},{-3,3},{-2,3},{-2,4}},
//  				{{4,0},{-2,3},{-2,4},{-1,4},{0,4}},
//  				{{4,0},{4,1},{4,0},{4,-1},{4,-2}},
//  				{{4,0},{4,-1},{4,-2},{3,-2},{3,-3}},
//  				{{4,0},{3,-2},{3,-3},{2,-3},{2,-4}},
//  				{{4,0},{2,-3},{2,-4},{1,-4},{0,-4}},
//  				{{4,0},{-1,4},{0,4},{1,4},{2,4}},
//  				{{4,0},{1,4},{2,4},{2,3},{3,3}},
//  				{{4,0},{2,3},{3,3},{3,2},{4,2}},
//  				{{4,0},{3,2},{4,2},{4,1},{4,0}},
//  				{{4,0},{1,-4},{0,-4},{-1,-4},{-2,-4}},         
//  				{{4,0},{-1,-4},{-2,-4},{-2,-3},{-3,-3}},      
//  				{{4,0},{-2,-3},{-3,-3},{-3,-2},{-4,-2}},
//  				{{4,0},{-3,-2},{-4,-2},{-4,-1},{-4,0}},
//  				{{3,1},{0,2},{0,1},{1,1},{0,0}},
//  				{{3,1},{2,0},{1,0},{1,-1},{0,0}},
//  				{{3,1},{0,-2},{0,-1},{-1,-1},{0,0}},
//  				{{3,1},{-2,0},{-1,0},{-1,1},{0,0}},
//  				{{4,1},{-6,0},{6,0},{0,6},{0,-6}},
//  				{{4,1},{-6,1},{6,-1},{-1,-6},{1,6}},
//  				{{4,1},{-6,2},{-2,-6},{6,-2},{2,6}},
//  				{{4,1},{-6,3},{-3,-6},{6,-3},{3,6}},
//  				{{4,1},{-5,4},{-4,-5},{5,-4},{4,5}},
//  				{{4,1},{-4,5},{-5,-4},{4,-5},{5,4}},
//  				{{4,1},{-3,6},{-6,-3},{3,-6},{6,3}},
//  				{{4,1},{-2,6},{-6,-2},{2,-6},{6,2}},
//  				{{4,1},{-1,6},{-6,-1},{1,-6},{6,1}}  };                                                                                        

///// NEW PATTERNCOORD
      Int_t patternCoord[29][5][2]=
                               {{{4,0},{-4,-1},{-4,0},{-4,1},{-4,2}},
				{{4,0},{-4,1},{-4,2},{-3,2},{-3,3}},
				{{4,0},{-3,2},{-3,3},{-2,3},{-2,4}},
				{{4,0},{-2,3},{-2,4},{-1,4},{0,4}},
				{{4,0},{4,1},{4,0},{4,-1},{4,-2}},
				{{4,0},{4,-1},{4,-2},{3,-2},{3,-3}},
				{{4,0},{3,-2},{3,-3},{2,-3},{2,-4}},
				{{4,0},{2,-3},{2,-4},{1,-4},{0,-4}},
				{{4,0},{-1,4},{0,4},{1,4},{2,4}},
				{{4,0},{1,4},{2,4},{2,3},{3,3}},
				{{4,0},{2,3},{3,3},{3,2},{4,2}},
				{{4,0},{3,2},{4,2},{4,1},{4,0}},
				{{4,0},{1,-4},{0,-4},{-1,-4},{-2,-4}},         
				{{4,0},{-1,-4},{-2,-4},{-2,-3},{-3,-3}},      
				{{4,0},{-2,-3},{-3,-3},{-3,-2},{-4,-2}},
				{{4,0},{-3,-2},{-4,-2},{-4,-1},{-4,0}},
				{{4,1},{-2,0},{0,2},{0,-2},{2,0}},
				{{4,1},{-1,-1},{-1,1},{1,-1},{1,1}},
				{{4,1},{-1,0},{0,-1},{0,1},{1,0}},
				{{0,1},{0,0},{0,0},{0,0},{0,0}},
				{{4,1},{-6,0},{6,0},{0,6},{0,-6}},
				{{4,1},{-6,1},{6,-1},{-1,-6},{1,6}},
				{{4,1},{-6,2},{-2,-6},{6,-2},{2,6}},
				{{4,1},{-6,3},{-3,-6},{6,-3},{3,6}},
				{{4,1},{-5,4},{-4,-5},{5,-4},{4,5}},
				{{4,1},{-4,5},{-5,-4},{4,-5},{5,4}},
				{{4,1},{-3,6},{-6,-3},{3,-6},{6,3}},
				{{4,1},{-2,6},{-6,-2},{2,-6},{6,2}},
				{{4,1},{-1,6},{-6,-1},{1,-6},{6,1}}  }; 




    iRegionThresholds[0][0] = 8;
    iRegionThresholds[1][0] = -4;
    iRegionThresholds[0][1] = 8;
    iRegionThresholds[1][1] = -4;
    iRegionThresholds[0][2] = 12;
    iRegionThresholds[1][2] = -3;
    iRegionThresholds[0][3] = 99;
    iRegionThresholds[1][3] = -1;
    iRegionThresholds[0][4] = 8;
    iRegionThresholds[1][4] = -4;
    iRegionThresholds[0][5] = 8;
    iRegionThresholds[1][5] = -4;

 // iRegionThresholds[2] ={8,-4};
 
// initialisation of centers to 0 
  for(Int_t j=0; j<96; j++)
    for(Int_t k=0; k<96; k++)
      centers[j][k] = 0;                                                                                       
  // cout << "----------------" << endl;
  
  // loop over potential ring centers
  for(Int_t lr=0; lr<96; lr++)  {
    for(Int_t lc=0; lc<96; lc++)  {

      if(label[lr][lc] >3) { 
	//	cout << lr << "   " << lc << "  " << label[lr][lc] << endl;
	
	// initialisation of variables to 0
	iNumberOfValidRegions = 0;
	for (Int_t j=0; j< iNumberOfRegions; j++){
	  iGroupsInRegion[j]=0;
	}
	
	// loop over groups
	for (Int_t i=0; i< iNumberOfGroups; i++){
	  iPadsInGroup =0;
	// loop over pads in group
	  for(Int_t j=1; j<=patternCoord[i][0][0]; j++){
	    px = lr + patternCoord[i][j][0];
	    py = lc + patternCoord[i][j][1];
	    if(px>=0 && px <=95 && py>=0 && py<=95){ // boundaries of pad plane
	      //   cout << "px: " << px << "   py: " << py << endl;
	      if (pads[Sector][px][py]) {
		iPadsInGroup++;
	      }
	    }
	  } // end of loop over pads in group
	  if ( iPadsInGroup > 0 ){
	    // cout << "px: " << px << "   py: " << py << "  pad fired in group " << i << ": " << iPadsInGroup << endl;
	    // cout << "Incremented group " << i << endl;
	  }
	  // cout << "---*---" << endl;

	  if ( iPadsInGroup > 0 )  {    // at least one pad fired (logical OR)
	    iGroupsInRegion[patternCoord[i][0][1]]++;
	    //cout << "Inc region " << patternCoord[i][0][1] << " in group " << i <<endl;
	    //   cout << "-----*-----" << endl;
	  }
	} // end of loop over groups

	for (Int_t j=0; j< iNumberOfRegions; j++){
	  if (  (iRegionThresholds[j][Sector] >= 0 && iGroupsInRegion[j] > iRegionThresholds[j][Sector]) || 
                (iRegionThresholds[j][Sector] < 0 && iGroupsInRegion[j] < -iRegionThresholds[j][Sector]) ) {
	    iNumberOfValidRegions++;
	    //  if (j==0)
	    //cout << "Valid Region " << j << ": " << iGroupsInRegion[j] << "Thr: " << iRegionThresholds[j] << endl;
	  }
	}
	//cout << "number of valid regions: " << iNumberOfValidRegions << endl;
	//cout << "----------*----------" << endl;
	
	if ( iNumberOfValidRegions ==  iNumberOfRegions ) {
	  // We  have found a ring !!!!
	  centers[lr][lc] = iGroupsInRegion[0];
	  //  cout << "centers[" << lr << "][" << lc << "]: " << centers[lr][lc] << "    groups in veto region: " << iGroupsInRegion[1] << endl;
	}
      }
    }
  }
  
}
//============================================================================


//----------------------------------------------------------------------------
//************************* Local Max Search *********************************
// This algo is likely to identify one or more neighbouring pads
// as the center of a potential ring candidate.
// Therefore, a local maximum search over 8 directly neighbouring
// pads is performed to determine the center.   

void HRichRingFindIPU::MaxSearch(){
  // loop over found ring centers
  for(Int_t lr=0; lr<96; lr++)  {
    for(Int_t lc=0; lc<96; lc++)  {
      if(centers[lr][lc] >0) { 
	// cout << lr << "   " << lc << endl;

	// search over 8 neighbouring pads
	// 4 neighbouring pads used in the hardware
	if( centers[lr][lc] >= centers[lr+1][lc] &&
	    centers[lr][lc] >= centers[lr][lc+1] &&
	    centers[lr][lc] >  centers[lr-1][lc] &&
	    centers[lr][lc] >  centers[lr][lc-1])
//  	    centers[lr][lc] >= centers[lr+1][lc+1] &&
//  	    centers[lr][lc] >= centers[lr-1][lc+1] &&
//  	    centers[lr][lc] >  centers[lr-1][lc-1] &&
//  	    centers[lr][lc] >  centers[lr+1][lc-1] )
	  {
 	  
	  // fill the ring container
	  HRichHitIPU & ring = pRings[iRingNr];      
	  iRingNr++;
	  ring.iRingX = lr; 
	  ring.iRingY = lc;
	  ring.iRingQuality = centers[lr][lc];
	}
	
      }
    }
  }
}
//============================================================================













































