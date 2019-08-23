// File: hrichringfindsim.cc
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 03/04/18 14:20:20
// Updated Oct. 2000 by Wolfgang Koenig
#include "hrichringfindsim.h"
#include "hrichanalysis.h"
#include "hrichanalysissim.h"
#include "hrichanalysispar.h"
#include "hrichhitsim.h"

ClassImp(HRichRingFindSim)


HRichRingFindSim::HRichRingFindSim(){
  pRings = NULL;
}

HRichRingFindSim::~HRichRingFindSim(){
  if (pRings) delete [] pRings;
}

Bool_t HRichRingFindSim::init(HRichAnalysisSim* showMe) {

  HRichRingFind::init(showMe);

  if (pRings) delete [] pRings;

  Int_t maxRings = 0;
  if (pAnalysisParams->isActiveRingFindFitMatrix)
    maxRings += pAnalysisParams->iHowManyFitMatrixRings;
  if (pAnalysisParams->isActiveRingHoughTransf)
    maxRings += pAnalysisParams->iHowManyHoughTransfRings;
  pRings=new HRichHitSim[maxRings];
  showMe->pRings = pRings;

  return kTRUE;
}

void HRichRingFindSim::LookForTrack(HRichAnalysisSim *showMe){
  
// this method stores the track numbers for each ring and it stores maximally 3
// tracks  for each ring. At this stage the initial and final track positions in
// the catRichTrack array are stored. 

   Int_t n,j,k,z;
   Int_t ringHalfSize = iRingImageSize/2;
  
  
   for (n = 0; n < iRingNr; n++) {
     Int_t trackCount = 0;
     HRichHitSim & ring=pRings[n];

     Int_t iNowX = ring.iRingX;
     Int_t iNowY = ring.iRingY;
     for (Int_t row = iNowY-ringHalfSize; row <=iNowY+ringHalfSize; ++row) {
       for (Int_t col = iNowX-ringHalfSize; col <= iNowX+ringHalfSize; ++col) {
         if (!showMe->IsOut(col, row)) {
           if (showMe->GetPad(col, row)->getAmplitude()) {
             ring.iRingPadNTrack1[trackCount] = showMe->getPadsNTrack1(col, row, showMe->secCount);
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
       

       //if z > 0 then it means that there is more then 1 track for the analyzed Ring
       // In this case even the second [third] track is stored.
	   
       for (Int_t tmp = 1; tmp <= z; tmp++) {
	 ring.iRingTrack[k]=showMe->getTrack(ring.iRingPadNTrack1[j] + tmp);
	 ring.iRingFlag[k]=showMe->getFlag(ring.iRingPadNTrack1[j] + tmp);
	 k++;
       }   

       j++;
     }

     ring.iRingTrack[k]=0; // signals end of list
   }

}

Int_t HRichRingFindSim::Execute(HRichAnalysisSim *giveMe) {
 
  iRingNr=HRichRingFind::Execute(giveMe);
  for(Int_t i=0; i<iRingNr;++i) {
    // cout<<HRichRingFind::pRings[i].iRingX<<", copied: ";
    pRings[i]=HRichRingFind::pRings[i];
    //cout<<"INSIDE HRichRingFindSim : "<<pRings[i].iRingX<<", "
    //    <<pRings[i].iRingY<<endl;
  }

  if(iRingNr>0) LookForTrack(giveMe);
  return (iRingNr);

}

















