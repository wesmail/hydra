using namespace std;
#include <stdlib.h>
#include <math.h>
#include <iostream> 
#include <iomanip>
#include "hrichringfindIPUSim.h"
#include "hrichanalysisIPUSim.h"
#include "hrichanalysisIPU.h"
#include "hrichhitIPUSim.h"
#include <TMath.h>

 //////////////////////////////////////////////////////////////////////////////

ClassImp(HRichRingFindIPUSim)

//----------------------------------------------------------------------------
HRichRingFindIPUSim::HRichRingFindIPUSim() { 
 pRings = NULL;
 }
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichRingFindIPUSim::init(HRichAnalysisIPUSim* showMe) {
 HRichRingFindIPU::init(showMe);

 if (pRings) delete [] pRings;
 pRings=new HRichHitIPUSim[5000];
 showMe->pRings = pRings;

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPUSim::~HRichRingFindIPUSim() {
 if (pRings) delete [] pRings;
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPUSim::HRichRingFindIPUSim(const HRichRingFindIPUSim& source) {
 cerr << "HRichRingFindIPUSim object can not be initialized with values of another object!\n";
 cerr << "Default constructor will be called.\n";
 HRichRingFindIPUSim();
}
//============================================================================

//----------------------------------------------------------------------------
HRichRingFindIPUSim& HRichRingFindIPUSim::operator=(const HRichRingFindIPUSim& source) {
 if (this != &source) {
  cerr << "HRichRingFindIPUSim object can not be assigned!\n";
  cerr << "Default constructor will be called.\n";
  HRichRingFindIPUSim();
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichRingFindIPUSim::Execute(HRichAnalysisIPUSim *giveMe) {
  iRingNr = HRichRingFindIPU::Execute();
  for(Int_t i=0; i<iRingNr;++i) {
    pRings[i]=HRichRingFindIPU::pRings[i];
  }
  if(iRingNr>0) LookForTrack(giveMe);
  return (iRingNr);
}
//============================================================================


//----------------------------------------------------------------------------

void HRichRingFindIPUSim::LookForTrack(HRichAnalysisIPUSim *showMe){
  
// this method stores the track numbers for each ring and it stores maximally 3
// tracks  for each ring. At this stage the initial and final track positions in
// the catRichTrack array are stored. 
  
  Int_t n,j,k,z;
  Int_t ringHalfSize = 8;  
  
  for (n = 0; n < iRingNr; n++) {
    Int_t trackCount = 0;
    HRichHitIPUSim & ring=pRings[n];
    
    Int_t iNowX = ring.iRingX;
    Int_t iNowY = ring.iRingY;
    
    for (Int_t row = iNowY-ringHalfSize; row <=iNowY+ringHalfSize; ++row) {
      for (Int_t col = iNowX-ringHalfSize; col <= iNowX+ringHalfSize; ++col) {
	//     for(Int_t lnr=0; lnr < label_size; lnr++)  {
	//       Int_t row = iNowX + getLabelPattern(lnr,0); 
	//       Int_t col = iNowY + getLabelPattern(lnr,1); // this would be the IPU mask
	
	if (row>=0&&row<=95&&col>=0&&col<=95 ) { // boundaries of the pad plane 
	  if (pads[Sector][row][col]) { // pad fired
	    ring.iRingPadNTrack1[trackCount] = showMe->getPadsNTrack1(col, row, Sector);
	    ring.iRingPadNTrack2[trackCount] = showMe->getPadsNTrack2();
	    
	    trackCount++;
	  }
	}
      }
    }
    ring.iRingPadNTrack1[trackCount]=-1; // signals end of list
    ring.iRingPadNTrack2[trackCount]=-1; // signals end of list
    j = k = z = 0;
    while(ring.iRingPadNTrack1[j]>=0) {
      
      ring.iRingTrack[k]=showMe->getTrack(ring.iRingPadNTrack1[j]);
      ring.iRingFlag[k]=showMe->getFlag(ring.iRingPadNTrack1[j]);
      k++;
            
      z = ring.iRingPadNTrack2[j] - ring.iRingPadNTrack1[j]; 
      // if z > 0 then it means that there is more then 1 track for the analyzed Ring
      // In this case i loop over the other tracks and store them
      
      while (z>0){
	ring.iRingTrack[k]=showMe->getTrack(ring.iRingPadNTrack1[j]+z);
	ring.iRingFlag[k]=showMe->getFlag(ring.iRingPadNTrack1[j]+z);
        
	k++;
	z--;
      }
      j++;
    }  
    ring.iRingTrack[k]=0; // signals end of list
  }
}
//============================================================================








































