#include "hshowerhit.h"
#include "hshowerhitheader.h"
#include "hshowerhitfinder.h"

#include "TArrayI.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hshowerdetector.h"
#include "hratreeext.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hshowercal.h"
#include "hshowerhitfpar.h"
#include "hdebug.h"
#include "hades.h"
#include "showerdef.h"


ClassImp(HShowerHitFinder)

HShowerHitFinder::HShowerHitFinder(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
  m_pHitFPar = new HShowerHitFPar;
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  rtdb->addContainer(m_pHitFPar);
}

HShowerHitFinder::HShowerHitFinder()
{
  m_pHitFPar = new HShowerHitFPar;
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  rtdb->addContainer(m_pHitFPar);
}


HShowerHitFinder::~HShowerHitFinder(void) {
    if (m_pCellArr) {
      delete m_pCellArr;
    }
    if (m_pHitFPar) delete m_pHitFPar;
}

Bool_t HShowerHitFinder::init() {
    TArrayI arr(4);
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");

    pShowerDet->getRows();
    arr[0] = pShowerDet->getMaxSectors();
    arr[1] = pShowerDet->getMaxModules();
    arr[2] = pShowerDet->getRows();
    arr[3] = pShowerDet->getColumns();

    m_pCalCat=gHades->getCurrentEvent()->getCategory(catShowerCal);
    if (!m_pCalCat) {
      m_pCalCat=pShowerDet->buildCategory(catShowerCal);

      if (!m_pCalCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catShowerCal, m_pCalCat, "Shower");
    }


    m_pHitCat=gHades->getCurrentEvent()->getCategory(catShowerHit);
    if (!m_pHitCat) {
      m_pHitCat=pShowerDet->buildCategory(catShowerHit);

      if (!m_pHitCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catShowerHit, m_pHitCat, "Shower");
    }

    m_pHitHdrCat=gHades->getCurrentEvent()->getCategory(catShowerHitHdr);
    if (!m_pHitHdrCat) {
      m_pHitHdrCat=pShowerDet->buildCategory(catShowerHitHdr);

      if (!m_pHitHdrCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catShowerHitHdr, m_pHitHdrCat, "Shower");
    }

    m_pCellArr = new HRaTreeExt(m_pCalCat, &arr);

    return kTRUE;
}

Bool_t HShowerHitFinder::finalize() {
    return kTRUE;
}

HShowerHitFinder& HShowerHitFinder::operator=(HShowerHitFinder &c) {

  return c;
}

Int_t HShowerHitFinder::execute()
{
  HIterator *iter;
  HShowerCal *pCal;
  HLocation loc;

  m_pCellArr->update();
 
  iter = (HIterator*)getCalCat()->MakeIterator(); 
  Int_t n = 0;
  Int_t hits = 0;

  while((pCal = (HShowerCal *)iter->Next()))
  {
     loc = iter->getLocation();
     if (lookForHit(loc)) hits++;
     n++;
  }

  return 1;
}

Bool_t HShowerHitFinder::lookForHit(HLocation& fLoc) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerHitFinder::execute");
  gDebuger->message("Cal cat points to %p",fCalCat);
#endif
  HShowerCal *cal=NULL;
  HShowerHit *hit=NULL;
  Bool_t isHit = kFALSE;

  cal=(HShowerCal *)m_pCalCat->getObject(fLoc);

  if (cal && isLocalMax(fLoc)) {
    Float_t fCharge;

    hit=(HShowerHit *)m_pHitCat->getNewSlot(fLoc);
    if (hit!=NULL) {
      hit=new(hit) HShowerHit;

      fillSums(hit, fLoc);
      calcCoord(hit, fLoc);
       
      fCharge = cal->getChannel();
      hit->setCharge(fCharge);
      hit->setSector(cal->getSector());
      hit->setModule(cal->getModule());
      hit->setRow(cal->getRow());
      hit->setCol(cal->getCol());
      hit->setEventNr(cal->getEventNr());

      isHit = kTRUE;
      updateLocalMax(fLoc);
   }
  }
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HShowerHitFinder::execute");
#endif
  updateFiredCells(fLoc);
  return isHit;
}

void HShowerHitFinder::fillSums(HShowerHit* hit, HLocation &fLoc) {
  HLocation fTmpLoc;
  int nModules = ((HShowerHitFPar*)getHitFPar())->getModules();
  float sum, var;
  int cs;

  fTmpLoc = fLoc;
  for(fTmpLoc[1] = 0; fTmpLoc[1] < nModules; fTmpLoc[1]++) {
     sum = calculateSum(fTmpLoc, 1, &cs);
     var = calculateVar(fTmpLoc, 1, sum/9.0);
     hit->setSum(fTmpLoc[1], sum);      
     hit->setVar(fTmpLoc[1], var);      
     hit->setClusterSize(fTmpLoc[1], cs);
  }

  calculateSum(fLoc, 1, &cs);
  if (cs > 1) updateClusters(fLoc);

  if (nModules == 3) {
     fTmpLoc[1] = 2;
     hit->setSum25(calculateSum(fTmpLoc, 2));
  }

  hit->updateCalc();
}

void HShowerHitFinder::calcCoord(HShowerHit* hit, HLocation &fLoc) {
  float fX, fY, fRow, fCol;
  float fXCoeff[2], fYCoeff[2];
  float fXOffset, fYOffset;

  fXCoeff[0] = -51.2;
  fXCoeff[1] = 3.2 ;
  fXOffset = -1.0;

  fYCoeff[0] = 2.25;
  fYCoeff[1] = -4.5;
  fYOffset = 19.0;

  fRow = fLoc[2];
  fCol = fLoc[3];
  
  fX =  fXCoeff[1] * (fCol - fXOffset ) + fXCoeff[0];
  fY =  fYCoeff[1] * (fRow - fYOffset ) + fYCoeff[0];  

  hit->setXY(fX, fY);
}

Bool_t HShowerHitFinder::isLocalMax(HLocation &fLoc) {
  HShowerCal* pCell;
  HShowerCal* pRefCell = (HShowerCal*)m_pCellArr->getObject(fLoc);
  float charge, charge1;
  int rowL, rowU, colL, colR;

  int row = fLoc[2];
  int col = fLoc[3];
  HLocation fTmpLoc;

  ((HShowerHitFPar*)getHitFPar())->getRowBord(fLoc[0], fLoc[1], &rowL, &rowU);
  ((HShowerHitFPar*)getHitFPar())->getColBord(fLoc[0], fLoc[1], &colL, &colR);

  if ((row < rowL + 1) ||  (row >  rowU - 1))
     return 0;

  if ((col < colL + 1) ||  (col >  colR - 1))
     return 0;

  fTmpLoc = fLoc;

  charge = pRefCell->getChannel();

  for(fTmpLoc[2] = row - 1; fTmpLoc[2] <= row + 1; fTmpLoc[2]++)
    for(fTmpLoc[3] = col - 1; fTmpLoc[3] <= col + 1; fTmpLoc[3]++)
    {
        pCell =  (HShowerCal*)m_pCellArr->getObject(fTmpLoc);

        if (pCell->isLocalMax() != 0)
          return 0;

        charge1 =  pCell->getChannel();
        if(charge < charge1)
          return 0;

    }

  pRefCell->setLocalMax();
  return 1;
}

Float_t HShowerHitFinder::calculateVar(HLocation &fLoc, int nRange, float avg)
{
  int rowL, rowU, colL, colR;

  int row = fLoc[2];
  int col = fLoc[3];
  ((HShowerHitFPar*)getHitFPar())->getRowBord(fLoc[0], fLoc[1], &rowL, &rowU);
  ((HShowerHitFPar*)getHitFPar())->getColBord(fLoc[0], fLoc[1], &colL, &colR);

  if ((row < rowL + nRange) ||  (row >  rowU - nRange))
     return 0.0;

  if ((col < colL + nRange) ||  (col >  colR - nRange))
     return 0.0;

  float fDiff, fVar = 0;
  HLocation fTmpLoc;
  fTmpLoc = fLoc;

  HShowerCal* pCell;

  for(fTmpLoc[2] = row - nRange; fTmpLoc[2] <= row + nRange; fTmpLoc[2]++)
    for(fTmpLoc[3] = col - nRange; fTmpLoc[3] <= col + nRange; fTmpLoc[3]++)
    {
        pCell =  (HShowerCal*)m_pCellArr->getObject(fTmpLoc);
        fDiff +=  (pCell->getChannel() - avg);
        fVar += fDiff * fDiff;
    }

  return fVar;
}

Float_t HShowerHitFinder::calculateSum(HLocation &fLoc, int nRange, int* pncs)
{
  int nThreshold = ((HShowerHitFPar*)getHitFPar())->getThreshold();
  int rowL, rowU, colL, colR;

  int row = fLoc[2];
  int col = fLoc[3];
  ((HShowerHitFPar*)getHitFPar())->getRowBord(fLoc[0], fLoc[1], &rowL, &rowU);
  ((HShowerHitFPar*)getHitFPar())->getColBord(fLoc[0], fLoc[1], &colL, &colR);

  if ((row < rowL + nRange) ||  (row >  rowU - nRange))
     return 0.0;

  if ((col < colL + nRange) ||  (col >  colR - nRange))
     return 0.0;

  float fSum = 0;
  HLocation fTmpLoc;
  fTmpLoc = fLoc;

  int cs = 0;
  HShowerCal* pCell;

  for(fTmpLoc[2] = row - nRange; fTmpLoc[2] <= row + nRange; fTmpLoc[2]++)
    for(fTmpLoc[3] = col - nRange; fTmpLoc[3] <= col + nRange; fTmpLoc[3]++)
    {
        pCell =  (HShowerCal*)m_pCellArr->getObject(fTmpLoc);
        fSum +=  pCell->getChannel();
        if (pCell->getChannel() > nThreshold)
          cs++;
    }

  if (pncs)
    *pncs = cs;

  return fSum;
}

HShowerHitHeader* HShowerHitFinder::getHitHeader(HLocation &fLoc) {
  HLocation fTmpLoc;
  
  fTmpLoc.set(2, fLoc.getIndex(0), fLoc.getIndex(1));
  fTmpLoc[0] = fLoc[0];
  fTmpLoc[1] = fLoc[1];

  HShowerHitHeader *pHitHdr = (HShowerHitHeader*)
                 ((HMatrixCategory*)m_pHitHdrCat)->getObject(fTmpLoc);
  if (!pHitHdr) {
     pHitHdr = (HShowerHitHeader*)((HMatrixCategory*)m_pHitHdrCat)
                                                    ->getSlot(fTmpLoc);
     if (pHitHdr != NULL) {
        pHitHdr = new(pHitHdr) HShowerHitHeader;
        pHitHdr->setSector(fTmpLoc[0]);
        pHitHdr->setModule(fTmpLoc[1]);
     }

  }

  return pHitHdr; 
}

void HShowerHitFinder::updateClusters(HLocation &fLoc) {
   HShowerHitHeader *pHitHdr = getHitHeader(fLoc);
   if (pHitHdr)
      pHitHdr->incClusters();
}

void HShowerHitFinder::updateLocalMax(HLocation &fLoc) {
   HShowerHitHeader *pHitHdr = getHitHeader(fLoc);
   if (pHitHdr)
      pHitHdr->incLocalMax();
}

void HShowerHitFinder::updateFiredCells(HLocation &fLoc) {
   HShowerHitHeader *pHitHdr = getHitHeader(fLoc);
   if (pHitHdr)
      pHitHdr->incFiredCells();
}


