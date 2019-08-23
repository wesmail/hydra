// File: hrichcleaner.cc
// ***************************************************************************

//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : Oct. 2000 W. Koenig

#include <iostream.h>
#include "hrichcleaner.h"
#include "hrichdetector.h"
#include "hrichpad.h"
#include "hrichpadsignal.h"
#include "hrichgeometrypar.h"
#include "hrichpadfilter.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hades.h"
#include "hrichcal.h"
#include "hrichcalsim.h"
#include "hspectrometer.h"
#include "richdef.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hiterator.h"
// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichCleaner
//
//  This class cleans pad plane and full cal container
//  with the cleaned up pad plane.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


ClassImp(HRichCleaner)

//----------------------------------------------------------------------------
HRichCleaner::HRichCleaner(Text_t *name,Text_t *title)
	: HReconstructor(name, title) {
 iCount = 0;
 iActiveSector =0;
 isDirectHit =0;
}
//============================================================================

//----------------------------------------------------------------------------
HRichCleaner::~HRichCleaner() {}
//============================================================================

//----------------------------------------------------------------------------
HRichCleaner::HRichCleaner(const HRichCleaner& source) {
 iCount = source.iCount;
 
}
//============================================================================
Bool_t HRichCleaner::init(){
    HRichDetector * pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
    
    m_pCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    if (!m_pCalCat) {
	m_pCalCat=pRichDet->buildCategory(catRichCal);
	
	if (!m_pCalCat) return kFALSE;
	else gHades->getCurrentEvent()
		 ->addCategory(catRichCal, m_pCalCat, "Rich");
    }
    fIter = (HIterator*)m_pCalCat->MakeIterator("native");

    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    HRichAnalysisPar *pAnalysisPar = (HRichAnalysisPar*)rtdb->
	getContainer("RichAnalysisParameters");
    if (pAnalysisPar == NULL) {
	pAnalysisPar = new HRichAnalysisPar;
	rtdb->addContainer(pAnalysisPar);
    }
    setAnalysisPar(pAnalysisPar);
    if (pAnalysisPar == NULL) return kFALSE;
    
    HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->
	getContainer("RichGeometryParameters");
    
    if (pGeomPar == NULL) {
	pGeomPar = new HRichGeometryPar;
	rtdb->addContainer(pGeomPar);
    }
    setGeomPar(pGeomPar);
    if (pGeomPar == NULL) return kFALSE;
    
    
    return kTRUE;
}
Bool_t HRichCleaner::reinit() {
    initParameters();
    
    iPadActive.Set(maxCols*maxRows);
    for (int i=0 ; i<maxCols*maxRows; i++)
	if (((HRichGeometryPar*)fpGeomPar)->getPadsPar()->getPad(i)->getPadActive() >0)
	    iPadActive[i] = 1; else iPadActive[i] = 0;
    
    Int_t i,j,k,m,n;
    
    Int_t iMatrixSurface=0, iPartSurface=0;
    Int_t iMaskSize = ((HRichAnalysisPar*)fpAnalysisPar)->iRingMaskSize;
    Int_t iMaskSizeSquared = iMaskSize*iMaskSize;
    for (k = 0; k < iMaskSizeSquared; k++)
	if (((HRichAnalysisPar*)fpAnalysisPar)->iRingMask[k] == 1) iMatrixSurface++;
    
    
    for (j = 0; j < maxRows; j++)
	for (i = 0; i < maxCols; i++)   {
        iPartSurface = 0;
        for (k = 0; k < iMaskSizeSquared; k++) {
	    m = (k % iMaskSize) - iMaskSize/2;
	    n = (k / iMaskSize) - iMaskSize/2;
	    if (!IsOut(i,j,m,n))
		if (((HRichAnalysisPar*)fpAnalysisPar)->iRingMask[k] == 1) iPartSurface++;
        }
        ((HRichGeometryPar*)fpGeomPar)->getPadsPar()->
          getPad(i,j)->setAmplitFraction((Float_t)iPartSurface/(Float_t)iMatrixSurface);
      }
    return kTRUE;
}
Bool_t HRichCleaner::initParameters() {
    HRichGeometryPar *pGeomPar = getGeometryPar();                                  
    
   
  
   
    maxFiredTotalPads = 3000; // upper limit of number of fired pads
    maxFiredSectorPads =1000; // hardwired, needs to be set by an access function
    
    maxCols = pGeomPar->getColumns();
    maxRows = pGeomPar->getRows();
    //  maxPads = ((HRichGeometryPar*)fpGeomPar)->getPadsNr();
    if(pLeftBorder) delete [] pLeftBorder;
    pLeftBorder = new short[maxRows];
    if(pRightBorder) delete [] pRightBorder;
    pRightBorder = new short[maxRows];
    
    for (int row=0;row<maxRows;++row) {
	Int_t col=0;
	Int_t padOffset = row*maxCols;
	while(!pGeomPar->fPads.getPad(col+padOffset)->getPadActive() && col<maxCols) ++col;
	if(col==maxCols) {
      maxRows=row;
      break;
	}
	pLeftBorder[row]=col;
	while(pGeomPar->fPads.getPad(col+padOffset)->getPadActive() && col<maxCols) ++col;
	pRightBorder[row]=col-1;
//    cout<<"row="<<row<<" min,max="<<pLeftBorder[row]<<", "<<pRightBorder[row]<<"\n";
    }
    maxPads=maxRows*maxCols;
    
  //  cout<<"maxCols="<<maxCols<<", maxRows="<<maxRows<<", maxPads="<<maxPads<<endl;

  // now creating pads array

    Int_t i;
    pPads = new HRichPadSignal * [6];
    for (Int_t j = 0; j < 6; j++) {
	pSectorPads = pPads[j] = new HRichPadSignal[maxPads];
	for (i = 0; i < maxPads; pSectorPads[i++].clear());
    }
    pSectorPads=pPads[iActiveSector];
    
  return kTRUE;
}
//----------------------------------------------------------------------------
HRichCleaner& HRichCleaner::operator=(const HRichCleaner& source) {
 if (this != &source) {
  iCount = source.iCount;
 
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichCleaner::SetActiveSector(Int_t sectornr) {
 if (sectornr == iActiveSector) return;
 if (sectornr > 5 || sectornr < 0) {
  cerr << "Error in <HRichAnalysis::SetActiveSector>: "
       << "Sector number = " << sectornr << " out of range (0..5)!\n";
  return;
 } else if (((HRichGeometryPar*)fpGeomPar)->getSector(sectornr) == 0) {
  cerr << "Error in <HRichAnalysis::SetActiveSector>: "
       << "Sector number  = " << sectornr << " is not present (and cannot be active)!\n";
  return;
 } else {
  iActiveSector = sectornr;
  pSectorPads=pPads[sectornr];
 } 
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichCleaner::SetNextSector() {
 Int_t i = iActiveSector;
 while (i < 6) {
  i++;
  if (((HRichGeometryPar*)fpGeomPar)->getSector(i) > 0) {
	  pSectorPads=pPads[i];
	  return (iActiveSector = i);
  }
 }
 cerr << "No more sectors (last sector = " << iActiveSector << ")!\n";
return iActiveSector;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichCleaner::CleanAlonePad( Int_t border, Int_t lowerThr) {
    // this method deletes all single pad under threshold (lowerThr).
    // A single pad is defined as pad whose first fired neighbour
    // has at least a distance of "border" from the pad (4 pads at the moment).

Int_t k,l;


if (!border) {
  cerr << "Argument \'border\' in function CleanAlonePad must be larger than 0!\n";
  //  exit(1);
}

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: CleanAlonePad cleans the following pads: \n";
#endif
 
 HRichPadSignal * pad;
 Int_t offset;
 // cout<< "cleaning pad alone "<<endl;
 for(int row=0;row<maxRows;++row) {
  Int_t leftBorder=pLeftBorder[row];
  Int_t rightBorder=pRightBorder[row];
  pad = (HRichPadSignal*)(&pSectorPads[leftBorder+row*maxCols]);
  for(int col=leftBorder; col<=rightBorder;++col) {
   if (pad->getAmplitude() > 0) {
    for (k = row-border; k <= row+border; ++k) {
      offset = k*maxCols;
      for (l = col-border; l <= col+border; ++l)
        if (!(l == col && k == row))
	    if (!IsOut(l,k)){
		
		//cout<< "address "<<(HRichPadSignal*)(&pSectorPads[l+offset])<<endl;
            if (((HRichPadSignal*)(&pSectorPads[l+offset]))->getAmplitude() > 0){
		//		cout<<" pad charge "<<((HRichPadSignal*)
		//		 (&pSectorPads[l+offset]))->getAmplitude()<<endl;
		goto nextPad;
} 
	    }
    }

    if (pad->getAmplitude() < lowerThr) {
      pad->setAmplitude(0);
     

#ifdef HRICH_DEBUGMODE
      // cout << " (" << colStart+border << "," << rowStart+border << ")... ";
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

return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichCleaner::IsOut(Int_t x, Int_t y, Int_t dx, Int_t dy) {
 if ((x+dx) >= 0 && (x+dx) < maxCols &&
     (y+dy) >= 0 && (y+dy) < maxRows &&
     iPadActive[x+dx + maxCols*(y+dy)]) {
  return 0; }
 else {
  return 1; }
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichCleaner::IsOut(Int_t nowPad, Int_t dx, Int_t dy) {
 if (nowPad <= 0) return 1;
 Int_t x = nowPad % maxCols;
 Int_t y = nowPad / maxCols;

 if ((x+dx) >= 0 && (x+dx) < maxCols &&
     (y+dy) >= 0 && (y+dy) < maxRows &&
     iPadActive[nowPad+dx + maxCols*dy]) {
  return 0; }
 else {
  return 1; }
}
//============================================================================

//----------------------------------------------------------------------------
void HRichCleaner::DeletePulse(Int_t border, Int_t col, Int_t row) {
    // this method deletes a given pad and all the neighbours within
    //  a distance of "border"( 4 pads).
    //Int_t maxCols = showYou->GetPadsXNr();
    //cout<< " deleting  at col "<<col<< " row " <<row<<" poor ampitude "<<((HRichPadSignal*)(&pSectorPads[col+maxCols*row]))->getAmplitude()<<endl;
 ((HRichPadSignal*)(&pSectorPads[col+maxCols*row]))->setAmplitude(0);

 

#ifdef HRICH_DEBUGMODE
 // cout << " (" << colStart+border << "," << rowStart+border << ")... ";
#endif

 Int_t j;

 for (Int_t i = row-border; i <= row+border; ++i) {
  Int_t offset = maxCols*i;
  for (j = col-border; j <= col+border; ++j)
   if (!IsOut(j,i))
    if(!(i == row && j == col))
     if ( ((HRichPadSignal*)(&pSectorPads[j + offset]))->getAmplitude() > 0)
      DeletePulse(border, j, i);
 }
 return;
}

//============================================================================

//----------------------------------------------------------------------------
Int_t HRichCleaner::CleanHighPulse( Int_t border, Int_t upperThr) {
    //
    // this method deletes all the pads above upper threshold (upperThr)
    // and the cluster connected to each pad.
    //
 
#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: DeletePulse cleans the following pads: \n";
#endif
 
;

 for(int row=0; row<maxRows; ++row) {
  Int_t offset=row*maxCols;
  Int_t rightBorder=pRightBorder[row];
  for(int col=pLeftBorder[row]; col<=rightBorder; ++col) {
   if (((HRichPadSignal*)(&pSectorPads[col+offset]))->getAmplitude() > upperThr) {
       isDirectHit =1;
      
       DeletePulse(border,col, row);
   
   }
  }
 }


#ifdef HRICH_DEBUGMODE
 if (!iCount) cout << "None. \n" << "RICH DEBUG MODE: Total number "
                      "of pads cleaned by CleanHighPulse = 0\n";
 else cout << "\nRICH DEBUG MODE: Total number of pads cleaned "
              "by CleanHighPulse = " << iCount << "\n";
#endif

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HRichCleaner::execute() {
    HRichCal *pCal;
    Int_t i, j, ampl;
    Int_t sec, padnr;
    isDirectHit =0;
    iCount++;
    if(iCount%500==0) cout<<" proceeding evt :"<<iCount<<endl;
    //cout<<" proceeding evt cleaner :"<<iCount<<endl;
    //Bool_t kSecTest=kFALSE; 
    // Reset();
    
    Int_t iPadFiredNr = 0;
  

    for (i = 0; i < 6; i++) {
	if (fPadFired[i] > 0) {
	    fPadFired[i] = 0;
	    HRichPadSignal * pSecPads=pPads[i];
	    for (j = 0; j < maxPads; pSecPads[j++].clear());
	}
    }
    
    //    Int_t iCutOffThresh=((HRichAnalysisPar*)fpAnalysisPar)->iCutOffThresheold;
    fIter->Reset();
    
    while((pCal = (HRichCal *)fIter->Next())) {
	

	//if ( (ampl = (Int_t)pCal->getCharge()) > iCutOffThresh) {
	if ( (ampl = (Int_t)pCal->getCharge()) > 0){
	    sec = pCal->getSector();
	    fPadFired[sec]++;
	    padnr = pCal->getCol() + pCal->getRow()*maxCols;
	    pPads[sec][padnr].setAmplitude(ampl);
	}
      // remember that columns are x and rows are y

  }

    Int_t fPadFiredTot=0;
    for(i=6;i--;fPadFiredTot+=fPadFired[i]);
   
    
    if( fPadFiredTot > maxFiredTotalPads) {
	cout<<"Analysis of event skipped : too many pads fired (threshold : "<<maxFiredTotalPads<<" )"<<endl;
	return 0;
    }
    for (i=0; i<6; i++)
	if (fPadFired[i] >= maxFiredSectorPads) {
	    Warning("execute","To many fired pads in sector %i. %i/%i",i,fPadFired[i],maxFiredSectorPads);
	    return 0;
	}
    
    // **************************************************************
    
    // ------- loop over sectors --- begin ---
   
    for(i = 0; i < 6; i++) 
	if (((HRichGeometryPar*)fpGeomPar)->getSector(i) > 0) {
	    
	    //cout<< " cleaning sector  "<<i<<endl;
	    iPadFiredNr = 0;
	    SetActiveSector(i);
	    iPadFiredNr = fPadFired[i];
	    //	    cout<<"iPadFiredNr  "<<	iPadFiredNr<<endl;   
	    if(iPadFiredNr >2) {
		Int_t iReducedNr = 0;
		
		//	cout<<" iCleanAlonePadBorder "<< getParams()->iCleanAlonePadBorder<<endl;

		// remove me
		if (getParams()->isActiveCleanAlonePad) 
				    iReducedNr += CleanAlonePad(getParams()->iCleanAlonePadBorder,
								getParams()->iCleanAlonePadLowerThreshold);
		// remove me

		if (getParams()->isActiveCleanHighPulse) 
		    iReducedNr += CleanHighPulse(getParams()->iCleanHighPulseBorder,
						 getParams()->iCleanHighPulseUpperThreshold);
		
	    }
	    
	    //the pads that have been cleaned up are deleted from the ca container
	    if(isDirectHit>0) return kSkipEvent;
	    fIter->Reset();
	    
	    while((pCal = (HRichCal *)fIter->Next())) {
	
		Int_t row = pCal->getRow();
		Int_t  padOffset =  row*maxCols;
		Int_t col = pCal->getCol();
		if(pCal->getSector()==i){
		    if (((HRichPadSignal*)(&pSectorPads[col+padOffset]))->getAmplitude()==0) {
			//cout<<" col "<<col<< "  row "<<row<<endl;
			pCal->setCharge(0);
		    }
		    // if ( (ampl = (Int_t)pCal->getCharge()) > 0){
		}  
		// remember that columns are x and rows are y
		
	    }

	    HRichPadFilter padFilter;
	    ((HMatrixCategory*)getCalCat())->filter(padFilter);
   }
    // cout<<" end of the shitty cleaner "<<endl;
    return 0;
}
//============================================================================


