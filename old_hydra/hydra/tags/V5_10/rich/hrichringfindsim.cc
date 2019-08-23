// File: hrichringfindsim.cc
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 00/05/07 19:40:54
//
#include "hrichringfindsim.h"
#include "hrichanalysis.h"
#include "hrichanalysissim.h"
#include "hrichanalysispar.h"
#include "hrichhitsim.h"

ClassImp(HRichRingFindSim)


HRichRingFindSim::HRichRingFindSim(){
pRings = new HRichHitSim[10];

}

void HRichRingFindSim::CalcRingParameters(HRichAnalysisSim *showMe) {

 Int_t i,j,k,l,m,n,
       iCount, iIsPhot4, iIsPhot8, iPhot4Nr, iPhot8Nr, iPad;
 Int_t iNowX, iNowY, iShift;

 iPhot4Nr = iPhot8Nr = iPad = 0;
 iShift = pAnalysisParams->iRingMaskSize/2;

if (iRingNr == 0 && 
     pGeometryParams->getPadsPar()->getActivePadsNr() > 
     iShift*iShift*4) {

 do {
  iNowX = (Int_t)HomogenDistr(pAnalysisParams->iRingRadius, showMe->GetPadsXNr() - pAnalysisParams->iRingRadius);
  iNowY = (Int_t)HomogenDistr(pAnalysisParams->iRingRadius, showMe->GetPadsYNr() - pAnalysisParams->iRingRadius);
 } while (showMe->IsOut(iNowX,iNowY,0,0));

 for (j = 0; j < pAnalysisParams->iRingMaskSize; j++)
  for (i = 0; i < pAnalysisParams->iRingMaskSize; i++)
   if (!showMe->IsOut(iNowX,iNowY,i-iShift,j-iShift)) {
    iIsPhot4 = iIsPhot8 = 0;
    m = iNowX+i-iShift + showMe->GetPadsXNr()*(iNowY+j-iShift);
    if (showMe->GetPad(m)->getAmplitude() > 0 && 
        pAnalysisParams->iRingMask[i + (pAnalysisParams->iRingMaskSize)*j] == 1) {
     iPad++;
     for (k = -1; k < 2; k++)
      for (l = -1; l < 2; l++)
       if (((l == 0 && abs(k)) || (k == 0 && abs(l))) && !(l == 0 && k == 0) &&
           !showMe->IsOut(m,l,k) && showMe->GetPad(m+l,k)->getAmplitude() >= showMe->GetPad(m)->getAmplitude())
        iIsPhot4++;
     if (iIsPhot4 == 0) iPhot4Nr++;
     for (k = -1; k < 2; k++)
      for (l = -1; l < 2; l++)
       if (abs(l) && abs(k) && !showMe->IsOut(m,l,k) && 
           showMe->GetPad(m+l,k)->getAmplitude() >= showMe->GetPad(m)->getAmplitude())
        iIsPhot8++;
     if (iIsPhot4 == 0 && iIsPhot8 == 0) iPhot8Nr++;
    }
   }

 showMe->iFakePad = iPad;
 showMe->iFakeLocalMax4 = iPhot4Nr;
 showMe->iFakeLocalMax8 = iPhot8Nr;

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: 0 rings found:\n";
 cout << "Background parameters | fake local maxima 4 = " << showMe->iFakeLocalMax4 
      << " | fake local maxima 8 = " << showMe->iFakeLocalMax8 << "\n";
#endif

} else {

 Int_t trackCount = 0;
 for (n = 0; n < iRingNr; n++) {
  iRingTempImage.Reset();
  for (j = 0; j < pAnalysisParams->iRingMaskSize; j++) 
   for (i = 0; i < pAnalysisParams->iRingMaskSize; i++) {
    if (!showMe->IsOut(pRings[n].iRingX,pRings[n].iRingY,i-iShift,j-iShift)) {
     Int_t ampli = showMe->GetPad(pRings[n].iRingX+i-iShift, pRings[n].iRingY+j-iShift)->getAmplitude();
     pRings[n].iRingImage[i + pAnalysisParams->iRingMaskSize*j] = ampli;
      
     if (ampli) {
	 
	 pRings[n].iRingPadNTrack1[trackCount] = showMe->getPadsNTrack1(pRings[n].iRingX+i-iShift,  pRings[n].iRingY+j-iShift,showMe->secCount);
		
         pRings[n].iRingPadNTrack2[trackCount] = showMe->getPadsNTrack2(pRings[n].iRingX+i-iShift,  pRings[n].iRingY+j-iShift,showMe->secCount);

         trackCount++;
     }    
    

    } else pRings[n].iRingImage[i + pAnalysisParams->iRingMaskSize*j] = 0;
   }
 
  iPhot4Nr = iPhot8Nr = 0;
  iNowX = pRings[n].iRingX;
  iNowY = pRings[n].iRingY;
  for (j = 0; j < pAnalysisParams->iRingMaskSize; j++)
   for (i = 0; i < pAnalysisParams->iRingMaskSize; i++)
    if (!showMe->IsOut(iNowX,iNowY,i-iShift,j-iShift)) {
     iIsPhot4 = iIsPhot8 = 0;
     m = iNowX+i-iShift + showMe->GetPadsXNr()*(iNowY+j-iShift);
     if (showMe->GetPad(m)->getAmplitude() > 0 && 
         pAnalysisParams->iRingMask[i + (pAnalysisParams->iRingMaskSize)*j] == 1) {
      pRings[n].iRingPadNr++;
      pRings[n].iRingAmplitude += showMe->GetPad(m)->getAmplitude();
      // cout << "-------- pad amplitude --- " << m << " |  " << showMe->GetPad(m)->getAmplitude() << endl;
      for (k = -1; k < 2; k++)
       for (l = -1; l < 2; l++)
        if (((l == 0 && abs(k)) || (k == 0 && abs(l))) && !(l == 0 && k == 0) &&
            !showMe->IsOut(m,l,k) && showMe->GetPad(m+l,k)->getAmplitude() >= showMe->GetPad(m)->getAmplitude()) {
         iIsPhot4++;
        }
      if (iIsPhot4 == 0) {
       iPhot4Nr++;
       iRingTempImage[i + (pAnalysisParams->iRingMaskSize)*j] += 1;
      }
      for (k = -1; k < 2; k++)
       for (l = -1; l < 2; l++)
        if (abs(l) && abs(k) && !showMe->IsOut(m,l,k) && 
            showMe->GetPad(m+l,k)->getAmplitude() >= showMe->GetPad(m)->getAmplitude()) {
         iIsPhot8++;
        }
      if (iIsPhot4 == 0 && iIsPhot8 == 0) {
       iPhot8Nr++;
       iRingTempImage[i + (pAnalysisParams->iRingMaskSize)*j] += 1;
      }
     }
    }

 pRings[n].iRingLocalMax4 = iPhot4Nr;
 pRings[n].iRingLocalMax8 = iPhot8Nr;


 iCount = 0;
 iClusterSize.Reset();
 iClusterLMax4.Reset();
 iClusterLMax8.Reset();
 for (j = 0; j < iRingImageSize; j++)
  for (i = 0; i < iRingImageSize; i++)
   if (pRings[n].iRingImage[i + iRingImageSize*j] > 0) {
    iInnerCount = iInnerPhot4 = iInnerPhot8 = 0;
    CalcRingClusters(showMe, &iRingTempImage[0], n, i, j);
    if (iInnerCount) {
     iClusterSize[iCount] = iInnerCount;
     iClusterLMax4[iCount] = iInnerPhot4;
     iClusterLMax8[iCount] = iInnerPhot8;
     iCount++;
    }
   }
 pRings[n].iRingClusterNr = iCount;
 for (i = 0; i < iCount; i++) {
  pRings[n].iRingClusterSize[i] = iClusterSize[i];
  pRings[n].iRingClusterLMax4[i] = iClusterLMax4[i];
  pRings[n].iRingClusterLMax8[i] = iClusterLMax8[i]; 
 } 

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: " << iRingNr << " ring(s) found:\n";
 cout << "Ring nr " << n+1 << " fired = " << pRings[n].iRingPadNr 
      << " | x = " << pRings[n].iRingX
      << " | y = " << pRings[n].iRingY << " | quality = " << pRings[n].iRingQuality
      << " | amplitude = " << pRings[n].iRingAmplitude 
      << " | local max 4 = " << pRings[n].iRingLocalMax4 
      << " | local max 8 = " << pRings[n].iRingLocalMax8 << "\n";
 cout << " free parameter = " << pRings[n].iRingFreeParam << "\n";
 cout << "Clusters data (number of pads, local max 4, local max 8):\n";
 
 for (i = 0; i < pRings[n].iRingClusterNr; i++) {
  cout << "(" << pRings[n].iRingClusterSize[i]
       << ", " << pRings[n].iRingClusterLMax4[i]
       << ", " << pRings[n].iRingClusterLMax8[i] << ")\n"; 
 }
#endif

 } // end of loop over all rings


} // end of condition with iRingNr

} // eof CalcRin
Bool_t HRichRingFindSim::init(HRichAnalysisSim* showMe) {
 
 pAnalysisParams = showMe->getParams();
 pGeometryParams = (HRichGeometryPar*)showMe->getGeomPar();

 iInnerCount = 0;
 iInnerPhot4 = 0;
 iInnerPhot8 = 0;
 iClusterSize.Set(50);
 iClusterLMax4.Set(50);
 iClusterLMax8.Set(50);
 iRingImageSize = pAnalysisParams->iRingMaskSize;
 iRingTempImage.Set(pAnalysisParams->iRingMaskSize * pAnalysisParams->iRingMaskSize);
 iPadPlane.Set(((HRichGeometryPar*)showMe->getGeomPar())->getPadsNr());
 iRingNr = 0;
 iAlgorithm1RingNr = 0;
 iAlgorithm2RingNr = 0;

 if (pRings) delete [] pRings;
 pRings = NULL;
 if (pAlgorithm1Ring) delete [] pAlgorithm1Ring;
 if (pAlgorithm2Ring) delete [] pAlgorithm2Ring;

 if (pAnalysisParams->iHowManyFitMatrixRings > 0)
  pAlgorithm1Ring = new HRichHit[pAnalysisParams->iHowManyFitMatrixRings];
 else pAlgorithm1Ring = NULL;
 if (pAnalysisParams->iHowManyHoughTransfRings > 0)
  pAlgorithm2Ring = new HRichHit[pAnalysisParams->iHowManyHoughTransfRings];
 else pAlgorithm2Ring = NULL;

 return kTRUE;
}
void HRichRingFindSim::LookForTrack(HRichAnalysisSim *showMe){
   
   Int_t n,j,k,z;
   j = k = z = 0;
   for (n = 0; n < iRingNr; n++) {
       while(pRings[n].iRingPadNTrack1[j]){
           
           pRings[n].iRingTrack[k]=showMe->getTrack(pRings[n].iRingPadNTrack1[j]);
           pRings[n].iRingFlag[k]=showMe->getFlag(pRings[n].iRingPadNTrack1[j]);
           
           k++;
           j++;
       }
       z =pRings[n].iRingPadNTrack2[j] - pRings[n].iRingPadNTrack1[j]; 
       //cout<<"+++++++++++"<<z<<endl;   
       while (z>0){
           pRings[n].iRingTrack[k]=showMe->getTrack(pRings[n].iRingPadNTrack1[j]+1);
            pRings[n].iRingFlag[k]=showMe->getFlag(pRings[n].iRingPadNTrack1[j]+1);
	   cout<<"********"<<pRings[n].iRingFlag[k]<<"+++++++"<<k<<endl;
           k++;
           z--;
           
       }   

   }
   //cout<<"Num of pads for each ring "<<k<<endl;
}
Int_t HRichRingFindSim::Execute(HRichAnalysisSim *giveMe) {
   
Int_t i,j, iChosen;
if (giveMe->pRings) {
 giveMe->iRingNr = 0;
 delete [] giveMe->pRings;
 giveMe->pRings = NULL;
}

if (giveMe->GetLabelNr() == 0 || GetAlgorithmNr(giveMe) == 0) {
 iRingNr = 0;
 CalcRingParameters(giveMe);
  LookForTrack(giveMe);
return (giveMe->iRingNr = iRingNr);
}

iRingNr = 0;

if (pAnalysisParams->isActiveRingFindFitMatrix)
 RingFindFitMatrix(giveMe,pAnalysisParams->iMinimalFitMatrixRingQuality,
                   pAnalysisParams->iMinimalFitMatrixRingDistance,pAnalysisParams->iHowManyFitMatrixRings);

if (pAnalysisParams->isActiveRingHoughTransf)
 RingFindHoughTransf(giveMe,pAnalysisParams->iMinimalHoughTransfRingQuality,
                     pAnalysisParams->iMinimalHoughTransfRingDistance,pAnalysisParams->iHowManyHoughTransfRings);

// --- all rings found by all algorithms will be stored ---
if (GetAlgorithmNr(giveMe) == 2 && pAnalysisParams->iSuperiorAlgorithmID == 3) {
 pRings = new HRichHitSim[iAlgorithm1RingNr + iAlgorithm2RingNr];
 for (i = 0; i < iAlgorithm1RingNr; i++) {
  pRings[iRingNr].iRingX = pAlgorithm1Ring[i].iRingX;
  pRings[iRingNr].iRingY = pAlgorithm1Ring[i].iRingY;
  pRings[iRingNr].iRingQuality = pAlgorithm1Ring[i].iRingQuality;
  pRings[iRingNr].iRingAlgorithmIndex = pAlgorithm1Ring[i].iRingAlgorithmIndex;
  pRings[iRingNr].iRingFreeParam = pAlgorithm1Ring[i].iRingFreeParam;
  iRingNr++;
 }
 for (i = 0; i < iAlgorithm2RingNr; i++) {
  pRings[iRingNr].iRingX = pAlgorithm2Ring[i].iRingX;
  pRings[iRingNr].iRingY = pAlgorithm2Ring[i].iRingY;
  pRings[iRingNr].iRingQuality = pAlgorithm2Ring[i].iRingQuality;
  pRings[iRingNr].iRingAlgorithmIndex = pAlgorithm2Ring[i].iRingAlgorithmIndex;
  pRings[iRingNr].iRingFreeParam = pAlgorithm2Ring[i].iRingFreeParam;
  iRingNr++;
 }
 CalcRingParameters(giveMe);
 LookForTrack(giveMe);
 giveMe->pRings = pRings;
 pRings = NULL;
return (giveMe->iRingNr = iRingNr);
}

// --- only rings found by both algorithms ---
if (GetAlgorithmNr(giveMe) == 2) {
 pRings = new HRichHitSim[((iAlgorithm1RingNr > iAlgorithm2RingNr) ? iAlgorithm1RingNr : iAlgorithm2RingNr)];

#ifdef HRICH_DEBUGMODE
 cout << "Rings found by both algorithms matching... (maximum " 
      << ((iAlgorithm1RingNr > iAlgorithm2RingNr) ? iAlgorithm1RingNr : iAlgorithm2RingNr) << " candidates).\n";
#endif

 for (i = 0; i < iAlgorithm1RingNr; i++)
  if (pAlgorithm1Ring[i].iRingQuality > 0) {
   for (j = 0; j < iAlgorithm2RingNr; j++) {
    iChosen = 0;
    if (pAlgorithm2Ring[j].iRingQuality > 0) {
     if (CalcDistance(pAlgorithm1Ring[i],pAlgorithm2Ring[j]) <= pAnalysisParams->iRingRadius/2) {
      if (pAnalysisParams->iSuperiorAlgorithmID == 1) {
       pRings[iRingNr].iRingX = pAlgorithm1Ring[i].iRingX;
       pRings[iRingNr].iRingY = pAlgorithm1Ring[i].iRingY;
       pRings[iRingNr].iRingQuality = pAlgorithm1Ring[i].iRingQuality;
       pRings[iRingNr].iRingAlgorithmIndex = 3;
       pRings[iRingNr].iRingFreeParam = pAlgorithm1Ring[i].iRingFreeParam;
       iRingNr++;
       iChosen = 1;
      } else if (pAnalysisParams->iSuperiorAlgorithmID == 2) {
       pRings[iRingNr].iRingX = pAlgorithm2Ring[j].iRingX;
       pRings[iRingNr].iRingY = pAlgorithm2Ring[j].iRingY;
       pRings[iRingNr].iRingQuality = pAlgorithm2Ring[j].iRingQuality;
       pRings[iRingNr].iRingAlgorithmIndex = 3;
       pRings[iRingNr].iRingFreeParam = pAlgorithm2Ring[j].iRingFreeParam;
       iRingNr++;
       iChosen = 1;
      }
     }
    }
    if (iChosen) break;
   }
  }

#ifdef HRICH_DEBUGMODE
 cout << "Number of rings matched: " << iRingNr << endl;
#endif

 CalcRingParameters(giveMe);
 // cout<<" After calcringparameters"<<endl;
 LookForTrack(giveMe);
 giveMe->pRings = pRings;
 pRings = NULL;
return (giveMe->iRingNr = iRingNr);
}

// --- rings found by selected algorithm ---
if (GetAlgorithmNr(giveMe) == 1) {
 if (pAnalysisParams->isActiveRingFindFitMatrix) {
  iRingNr = iAlgorithm1RingNr;
  pRings = (HRichHitSim*)pAlgorithm1Ring;
  pAlgorithm1Ring = new HRichHit[pAnalysisParams->iHowManyFitMatrixRings];
  CalcRingParameters(giveMe);
  giveMe->pRings = pRings;
  pRings = NULL;
return (giveMe->iRingNr = iRingNr);
 }
 if (pAnalysisParams->isActiveRingHoughTransf) {
  iRingNr = iAlgorithm2RingNr;
  pRings = (HRichHitSim*)pAlgorithm2Ring;
  pAlgorithm2Ring = new HRichHitSim[pAnalysisParams->iMinimalHoughTransfRingQuality];
  CalcRingParameters(giveMe);
   LookForTrack(giveMe);
  giveMe->pRings = pRings;
  pRings = NULL;
return (giveMe->iRingNr = iRingNr);
 }
}


return (giveMe->iRingNr = 0); // this should never happen

}




