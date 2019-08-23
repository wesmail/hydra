// File: hrichpadclean.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : Oct. 2000 W. Koenig
//*-- Modified : Mar 12 18:50:45 CET 2005 martin.jurkovic@ph.tum
#include <stdlib.h>
#include <iostream> 
#include <iomanip>
#include "hades.h"
#include "hevent.h"
#include "hmatrixcategory.h"
#include "hrichdetector.h"
#include "hrichdirclus.h"
#include "hrichpadclean.h"
#include "hspectrometer.h"
#include "richdef.h"

// ***************************************************************************
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
//  HRichPadClean
//
//  This class cleans pad plane provided by HRichAnalysis.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichPadClean)

//----------------------------------------------------------------------------
HRichPadClean::HRichPadClean() {
  iCount = 0;
  iTempCluster.Set(100);
  minX = 200;
  maxX = 200;
  minY = 0; 
  maxY = 0;
  deltaPhi = 0;
  fRichClusCat = NULL;
}

//============================================================================

//----------------------------------------------------------------------------
HRichPadClean::~HRichPadClean() {}
//============================================================================

//----------------------------------------------------------------------------
HRichPadClean::HRichPadClean(const HRichPadClean& source) {
 iCount = source.iCount;
 iTempCluster = source.iTempCluster;
}
//============================================================================

//----------------------------------------------------------------------------
HRichPadClean& HRichPadClean::operator=(const HRichPadClean& source) {
 if (this != &source) {
  iCount = source.iCount;
  iTempCluster = source.iTempCluster;
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichPadClean::CleanAlonePad(HRichAnalysis *showMe, Int_t border, Int_t lowerThr) {
    // this method deletes all single pad under threshold (lowerThr).
    // A single pad is defined as pad whose first fired neighbour
    // has at least a distance of "border" from the pad (4 pads at the moment).

  Int_t k,l;
  iCount = 0;

  if (!border) {
    cerr << "Argument \'border\' in function CleanAlonePad must be larger than 0!\n";
    exit(1);
  }

#ifdef HRICH_DEBUGMODE
  cout << "RICH DEBUG MODE: CleanAlonePad cleans the following pads: \n";
#endif
 
  HRichPadSignal * pad;
  Int_t offset;
  Int_t maxCols = showMe->GetPadsXNr();
  Int_t maxRows = showMe->GetPadsYNr();

  for(Int_t row=0;row<maxRows;++row) {
    Int_t leftBorder=showMe->pLeftBorder[row];
    Int_t rightBorder=showMe->pRightBorder[row];
    pad = showMe->GetPad(leftBorder+row*maxCols);
    for(Int_t col=leftBorder; col<=rightBorder;++col) {
      if (pad->getAmplitude() > 0) {
	for (k = row-border; k <= row+border; ++k) {
	  offset = k*maxCols;
	  for (l = col-border; l <= col+border; ++l)
	    if (!(l == col && k == row))
	      if (!showMe->IsOut(l,k))
		if (showMe->GetPad(l+offset)->getAmplitude() > 0) goto nextPad;
	}

	if (pad->getAmplitude() < lowerThr) {
	  pad->setAmplitude(0);
	  iCount++;

#ifdef HRICH_DEBUGMODE
	  cout << " (" << colStart+border << "," << rowStart+border << ")... ";
#endif

	}
      }
    nextPad:
      ++pad;
    }
  }

#ifdef HRICH_DEBUGMODE
  if (!iCount) cout << "None. \n";
  else cout << "\nRICH DEBUG MODE: Total number of pads cleaned by "
    "CleanAlonePad = " << iCount << "\n";
#endif

  return iCount;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichPadClean::DeletePulse(HRichAnalysis* showYou,
                                Int_t          border, 
                                Int_t          col, 
                                Int_t          row) {
// this method deletes a given pad and all the neighbours within
//  a distance of "border"( 4 pads).

   Int_t maxCols = showYou->GetPadsXNr();
   chargeTot += showYou->GetPad(col+maxCols*row)->getAmplitude();

#ifdef HRICH_DEBUGMODE
   cout << " charge  " << showYou->GetPad(col + maxCols * row)->getAmplitude()
        << "  chargeTot  " <<chargeTot << endl;
#endif

   showYou->GetPad(col + maxCols * row)->setAmplitude(0);

#ifdef HRICH_DEBUGMODE
   cout << ((HRichGeometryPar*)showYou->getGeomPar())->getPadsPar()->getPad(col,row)
        << "  first pad  with the second method" << endl;
   cout << "  ==================== it is assignment time!========= " << endl;
   cout << " minX " <<   minX    << " maxY  " <<   maxY    << "  col  " << col << " minY  "
        <<   minY   << " row   " <<   row     << " maxY  " << maxY << endl;
#endif

   minX = minX >= col ? col:minX;
   minY = minY >= row ? row:minY;
   maxX = maxX <= col ? col:maxX;
   maxY = maxY <= row ? row:maxY;

#ifdef HRICH_DEBUGMODE
   cout << "  the harder you try the dumber you look " << endl;
   cout << " minX " << minX << " maxX " << maxX << "  col  " << col
        << " minY " << minY << " row  " << row  << " maxY  " << maxY<<endl;
#endif

   iCount++;

#ifdef HRICH_DEBUGMODE
   cout << " (" << colStart+border << "," << rowStart+border << ")... ";
#endif

   Int_t j;

   for (Int_t i = row-border; i <= row+border; ++i) 
   {
      Int_t offset = maxCols*i;
      for (j = col-border; j <= col+border; ++j)
         if (!showYou->IsOut(j,i))
            if(!(i == row && j == col))
               if (showYou->GetPad(j + offset)->getAmplitude() > 0)
                  DeletePulse(showYou, border, j, i);
   }

   return;
}

//============================================================================

//----------------------------------------------------------------------------
Int_t 
HRichPadClean::CleanHighPulse(HRichAnalysis* showMe, 
                              Int_t          border, 
                              Int_t          upperThr) {
//
// this method deletes all the pads above upper threshold (upperThr)
// and the cluster connected to each pad.
//

#ifdef HRICH_DEBUGMODE
   cout << "RICH DEBUG MODE: DeletePulse cleans the following pads: \n";
#endif

   Int_t maxRows      = showMe->GetPadsYNr();
   Int_t maxCols      = showMe->GetPadsXNr();
   Int_t iCountBefore = 0;
   Int_t nCleaned     = 0;

   iCount = 0;

   for( Int_t row = 0; row < maxRows; ++row )
   {
      Int_t offset = row * maxCols;
      Int_t rightBorder = showMe->pRightBorder[row];
      for( Int_t col = showMe->pLeftBorder[row]; col <= rightBorder; ++col) {
         if ( showMe->GetPad(col+offset)->getAmplitude() > upperThr )
         {
            chargeTot = 0; // fix MJ
            minX = 200;
            maxX = 0;
            minY = 200; 
            maxY = 0;
            deltaPhi = 0;
            iCountBefore = iCount;
            DeletePulse(showMe, border, col, row);
            if ( (iCount - iCountBefore) > 0 && nCleaned < 100 )
            {
               iTempCluster[nCleaned++] = iCount - iCountBefore;

               // Store the cluster and its parameters to the HRichDirClus category
               HRichDirClus* clus = NULL;;
               HLocation loc;
               loc.set(1, 0);
               clus = static_cast<HRichDirClus*>( static_cast<HMatrixCategory*>(fRichClusCat)->getNewSlot(loc) );
   
               if( NULL == clus ) 
                  Error("CleanHighPulse","Error no slot free in HRichDirClus !!!!!!!!!!!  ");
               else 
               {
                  clus = new(clus) HRichDirClus;
                  clus->setSector(showMe->GetActiveSector());
                  clus->setXYDim(TMath::Abs(minX - maxX),
                                 TMath::Abs(minY - maxY));
                  clus->setTotalCharge(chargeTot);
                  clus->setNrPad(iCount - iCountBefore);
                  clus->setPhiDiff(calculateDPhi(showMe, minX, minY, maxX, maxY));
                  Float_t theta = -1;
                  if ( NULL != ((HRichGeometryPar*)showMe->getGeomPar())->getPadsPar()->getPad( ((Int_t)(maxX + minX)/2), ((Int_t)(maxY + minY)/2)) )
                     theta = ((HRichGeometryPar*)showMe->getGeomPar())->getPadsPar()->getPad( ((Int_t)(maxX + minX)/2), ((Int_t)(maxY + minY)/2))->getTheta();
                  clus->setTheta( theta );
               }
            }
         }
      }
   }

   showMe->iClusterCleanedNr = nCleaned;
   showMe->iClustersCleaned.Set(nCleaned);

   for (Int_t i = 0; i < nCleaned; i++)
      showMe->iClustersCleaned[i] = iTempCluster[i];

#ifdef HRICH_DEBUGMODE
   if (!iCount) cout << "None. \n" << "RICH DEBUG MODE: Total number "
                     << "of pads cleaned by CleanHighPulse = 0\n";
   else cout << "\nRICH DEBUG MODE: Total number of pads cleaned "
             << "by CleanHighPulse = " << iCount << "\n";
#endif

   return iCount;
}
//============================================================================

Bool_t HRichPadClean::init() {
  fRichClusCat = gHades->getCurrentEvent()->getCategory(catRichDirClus);
  if (!fRichClusCat) {
    fRichClusCat=gHades->getSetup()->getDetector("Rich")->buildCategory(catRichDirClus);
    if (!fRichClusCat) cout<<"catRichDirClus could not be created"<<endl;
    else gHades->getCurrentEvent()->addCategory(catRichDirClus,fRichClusCat,"Rich");
  }
  return kTRUE;
}

//----------------------------------------------------------------------------
Int_t HRichPadClean::Execute(HRichAnalysis *giveMe) {

  if(giveMe->iPadFiredNr >2) {
    Int_t iReducedNr = 0;
    HRichAnalysisPar *analParams = giveMe->getParams(); 
    //cout << "anal params" << analParams << endl;
    if (analParams->isActiveCleanAlonePad) 
      iReducedNr += CleanAlonePad(giveMe,analParams->iCleanAlonePadBorder,
				  analParams->iCleanAlonePadLowerThreshold);
    //cout << "alas!" << endl;
    if (analParams->isActiveCleanHighPulse) 
      iReducedNr += CleanHighPulse(giveMe,analParams->iCleanHighPulseBorder,
				   analParams->iCleanHighPulseUpperThreshold);
    return (iReducedNr);
  } else return giveMe->iPadFiredNr; //either 0, 1 or 2
}

//============================================================================
Float_t HRichPadClean::calculateDPhi(HRichAnalysis *showMe, Int_t xmin, Int_t ymin, Int_t xmax, Int_t ymax){
  Float_t phi1 = -1;
  Float_t phi2 = -1;

#ifdef HRICH_DEBUGMODE
  cout <<" xmin " <<xmin<<" ymin   "<< ymin<<" xmax  "<<xmax <<"   ymax "<<ymax<<endl;
#endif
  
  Int_t maxCols = showMe->GetPadsXNr();

#ifdef HRICH_DEBUGMODE
  cout <<((HRichGeometryPar*)showMe->getGeomPar())->getPadsPar()->getPad(xmin,ymin)<<"  first pad  "<<endl;
  cout <<((HRichGeometryPar*)showMe->getGeomPar())->getPadsPar()->getPad(xmin+maxCols*ymin)<<"  first pad  "<<endl;
  cout <<((HRichGeometryPar*)showMe->getGeomPar())->getPadsPar()->getPad(xmax+maxCols*ymax)<<"  second pad  "<<endl;
  cout << showMe->GetPad(xmin+maxCols*ymin)<<" first pad, second method  "<<endl; 
#endif
   
  if( showMe->GetPad(xmin+maxCols*ymin) )
    phi1 = ((HRichGeometryPar*)showMe->getGeomPar())->getPadsPar()->getPad(xmin+maxCols*ymin)->getPhi(showMe->GetActiveSector());
  if ( showMe->GetPad(xmax+maxCols*ymax) )
    phi2 = ((HRichGeometryPar*)showMe->getGeomPar())->getPadsPar()->getPad(xmax+maxCols*ymax)->getPhi(showMe->GetActiveSector());


#ifdef HRICH_DEBUGMODE
  cout<<"  delta Phi "<<TMath::Abs(phi1-phi2)<<endl;
#endif


  if ( -1 == phi1 && -1 == phi2 )
     return -1;
  
  return TMath::Abs(phi1-phi2);
}
//============================================================================

