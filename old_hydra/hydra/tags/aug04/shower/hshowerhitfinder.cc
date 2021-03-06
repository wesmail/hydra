#include "hshowerhit.h"
#include "hshowerpid.h"
#include "hshowerhitheader.h"
#include "hshowerhitfinder.h"
#include "hshowercriterium.h"

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
#include "hshowergeometry.h"
#include "hshowerpad.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"
#include "hgeomvector.h"
#include "hgeomvector2.h"
#include "hspecgeompar.h"
#include "hgeomvolume.h"
#include "showerdef.h"


ClassImp(HShowerHitFinder)

///////////////////////////////////////////////////////////////////////
//HShowerHitFinder
//
// HShowerHitFinder reconstructor searches local maxima in CAL level
//
//////////////////////////////////////////////////////////////////////

HShowerHitFinder::HShowerHitFinder(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
   fIter = NULL; 
   m_Loc.set(4, 0, 0, 0, 0);
   setFillPID();
   setSortFlag(kTRUE);

   m_pCriterium = new HShowerCriterium;
}

HShowerHitFinder::HShowerHitFinder()
{
   fIter = NULL; 
   m_Loc.set(4, 0, 0, 0, 0);
   setFillPID();
   setSortFlag(kTRUE);
   m_pCriterium = new HShowerCriterium;
}


HShowerHitFinder::~HShowerHitFinder(void) {
    if (m_pCellArr) {
      delete m_pCellArr;
    }
//    if (m_pHitFPar) delete m_pHitFPar;
    if (fIter) delete fIter;
    if (m_pCriterium) delete m_pCriterium;
}

Bool_t HShowerHitFinder::init() {
    printf("initialization of shower hit finder\n");

    TArrayI arr(4);
    TArrayI arr1(1);
    HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");

    arr[0] = pShowerDet->getMaxSectors();
    arr[1] = pShowerDet->getMaxModules();
    arr[2] = pShowerDet->getRows();
    arr[3] = pShowerDet->getColumns();
    arr1[0] = arr[0] * arr[1] * arr[2] * arr[3];

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
 
    if (IsFillPID()) {
      m_pPIDCat=gHades->getCurrentEvent()->getCategory(catShowerPID);
      if (!m_pPIDCat) {
        m_pPIDCat=pShowerDet->buildCategory(catShowerPID);
  
        if (!m_pPIDCat) return kFALSE;
        else gHades->getCurrentEvent()
                        ->addCategory(catShowerPID, m_pPIDCat, "Shower");
      }
    }
    else m_pPIDCat = NULL;

    m_pCellArr = new HRaTreeExt(m_pCalCat, &arr);
    fIter = (HIterator*)getCalCat()->MakeIterator("native"); 

    return initParameters();
}
Bool_t HShowerHitFinder::initParameters() {
    HRuntimeDb* rtdb=gHades->getRuntimeDb();

    HShowerGeometry *pGeom = (HShowerGeometry*)rtdb->
                                               getContainer("ShowerGeometry");
	if (!pGeom) {
		Error("HShowerHitFinder::initParameters","Container ShowerGeometry not created");
		return kFALSE;
	}
		setGeometry(pGeom);

    if (!pGeom) return kFALSE;

    HShowerHitFPar *pHitFPar = (HShowerHitFPar*)rtdb->
                                         getContainer("ShowerHitFinderParams");
    if (!pHitFPar) {
       pHitFPar = new HShowerHitFPar;
       rtdb->addContainer(pHitFPar);
    }
    setHitFPar(pHitFPar);
    if (!pHitFPar) return kFALSE;
    m_pCriterium->setParams(pHitFPar); 

    HSpecGeomPar *fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");

	setHSpecGeomPar(fSpecGeometry);
	if(!fSpecGeometry) return kFALSE;
	

    return kTRUE;

}

void HShowerHitFinder::setCriterium(HShowerCriterium* pCrit) {
   if (m_pCriterium) delete m_pCriterium; 
   m_pCriterium = pCrit;
}

Bool_t HShowerHitFinder::finalize() {
    return kTRUE;
}

HShowerHitFinder& HShowerHitFinder::operator=(HShowerHitFinder &c) {

  return c;
}

Int_t HShowerHitFinder::execute()
{
  HShowerCal *pCal;
  m_pCellArr->update();
 
  Int_t n = 0;
  Int_t hits = 0;

  fIter->Reset();
  while((pCal = (HShowerCal *)fIter->Next()))
  {
     m_Loc[0] = pCal->getSector();
     m_Loc[1] = pCal->getModule();
     m_Loc[2] = pCal->getRow();
     m_Loc[3] = pCal->getCol();
  
     if (lookForHit(pCal, m_Loc)) hits++;
     n++;
  }

  if (IsSortFlagSet()) {
     m_pHitCat->sort();
     if(m_pPIDCat) m_pPIDCat->sort();
  }

  return 0;
}

Bool_t HShowerHitFinder::lookForHit(HShowerCal* cal, HLocation& fLoc) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerHitFinder::execute");
  gDebuger->message("Cal cat points to %p",cal);
#endif
  HShowerHit *hit=NULL;
  HShowerPID *pid=NULL;
  Bool_t isHit = kFALSE;

  if (cal && isLocalMax(fLoc)) {
    Float_t fCharge, fShower = 0.0;
    Int_t ret = 0;

    hit=(HShowerHit *)m_pHitCat->getNewSlot(fLoc);
    if (hit) {
      hit=new(hit) HShowerHit;

      fillSums(hit, fLoc);
      calcCoord(hit, fLoc);
       
      fCharge = cal->getCharge();

      hit->setCharge(fCharge);
      hit->setSector(cal->getSector());
      hit->setModule(cal->getModule());
      hit->setRow(cal->getRow());
      hit->setCol(cal->getCol());
 
      if (IsSortFlagSet()) hit->calcAddress();   

      fShower = m_pCriterium->showerCriterium(hit, ret, 
                             (HShowerHitFPar*)m_pHitFPar);

      hit->setShower(fShower);

      if (m_pPIDCat && fShower>0) {
          pid=(HShowerPID *)m_pPIDCat->getNewSlot(fLoc);
          if (pid) {
            pid=new(pid) HShowerPID;
            fillPID(hit, pid);
          }
      }

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

void HShowerHitFinder::fillPID(HShowerHit* hit, HShowerPID* pid) {
   Float_t fX, fY, fZ;
   Float_t fR, fPhi, fTheta;

   pid->setCharge(hit->getCharge());
   pid->setSector(hit->getSector());
   pid->setModule(hit->getModule());
   pid->setRow(hit->getRow());
   pid->setCol(hit->getCol());
   pid->setAddress(hit->getAddress());
   pid->setShower(hit->getShower());

   hit->getLabXYZ(&fX, &fY, &fZ);
   pid->setXYZ(fX, fY, fZ);
   hit->getSphereCoord(&fR, &fPhi, &fTheta);
   pid->setSphereCoord(fR, fPhi, fTheta);

//   if (IsSortFlagSet()) 
//       getPIDCat()->sort();
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
  HGeomVector vLocalCoord;
  HGeomVector2 vLabCoord;
  
  HGeomVector targetPos = m_pHSpecGeomPar->getTarget(0)->getTransform().getTransVector();
 
  ((HShowerGeometry*)getGeometry())->getLocalCoord(fLoc, vLocalCoord);
  ((HShowerGeometry*)getGeometry())->getLabCoord(fLoc, vLabCoord);
  ((HShowerGeometry*)getGeometry())->getSphereCoord(fLoc, vLabCoord, &targetPos);

  hit->setXY(vLocalCoord.getX(), vLocalCoord.getY());
  hit->setLabXYZ(vLabCoord.getX(), vLabCoord.getY(), vLabCoord.getZ());
  hit->setSphereCoord(vLabCoord.getRad(), vLabCoord.getPhi(), vLabCoord.getTheta());
}

Bool_t HShowerHitFinder::isLocalMax(HLocation &fLoc) {
  HShowerCal* pCell;
  HShowerCal* pRefCell = (HShowerCal*)m_pCellArr->getObject(fLoc);
  float charge, charge1;
  int rowL, rowU, colL, colR, iRow1, iRow2, iCol1, iCol2;;

  int row = fLoc[2];
  int col = fLoc[3];
  HLocation fTmpLoc;

  ((HShowerHitFPar*)getHitFPar())->getRowBord(fLoc[0], fLoc[1], &rowL, &rowU);
  ((HShowerHitFPar*)getHitFPar())->getColBord(fLoc[0], fLoc[1], &colL, &colR);
/*
  if ((row < rowL + 1) ||  (row >  rowU - 1))
     return 0;

  if ((col < colL + 1) ||  (col >  colR - 1))
     return 0;
*/
   if((row < rowL) || (row > rowU) || (col < colL) || (col > colR))
	        return 0;

   if((iRow1 = row - 1) < rowL)
      iRow1 = rowL;

   if((iRow2 = row + 1) > rowU)
      iRow2 = rowU;

   if((iCol1 = col - 1) < colL)
      iCol1 = colL;

   if((iCol2 = col + 1) > colR)
      iCol2 = colR;


  fTmpLoc = fLoc;

  charge = pRefCell->getCharge();

  //for(fTmpLoc[2] = row - 1; fTmpLoc[2] <= row + 1; fTmpLoc[2]++)
  //  for(fTmpLoc[3] = col - 1; fTmpLoc[3] <= col + 1; fTmpLoc[3]++)
  for(fTmpLoc[2] = iRow1; fTmpLoc[2] <= iRow2; fTmpLoc[2]++)
    for(fTmpLoc[3] = iCol1; fTmpLoc[3] <= iCol2; fTmpLoc[3]++)
    {
        if((fTmpLoc[2] == row) && (fTmpLoc[3] == col))
        continue;

        pCell =  (HShowerCal*)m_pCellArr->getObject(fTmpLoc);

        if (pCell->isLocalMax() != 0)
          return 0;

        charge1 =  pCell->getCharge();
        if(charge < charge1)
          return 0;

    }

  pRefCell->setLocalMax();
  return 1;
}

Float_t HShowerHitFinder::calculateVar(HLocation &fLoc, int nRange, float avg)
{
  int rowL, rowU, colL, colR, iRow1, iRow2, iCol1, iCol2;

  int row = fLoc[2];
  int col = fLoc[3];
  ((HShowerHitFPar*)getHitFPar())->getRowBord(fLoc[0], fLoc[1], &rowL, &rowU);
  ((HShowerHitFPar*)getHitFPar())->getColBord(fLoc[0], fLoc[1], &colL, &colR);
/*
  if ((row < rowL + nRange) ||  (row >  rowU - nRange))
     return 0.0;

  if ((col < colL + nRange) ||  (col >  colR - nRange))
     return 0.0;
*/

  if((row < rowL) || (row > rowU) || (col < colL) || (col > colR))
     return 0.0f;

  if((iRow1 = row - nRange) < rowL)
     iRow1 = rowL;

  if((iRow2 = row + nRange) > rowU)
     iRow2 = rowU;

  if((iCol1 = col - nRange) < colL)
     iCol1 = colL;

  if((iCol2 = col + nRange) > colR)
     iCol2 = colR;
  
  float fDiff = 0.0, fVar = 0.0;
  HLocation fTmpLoc;
  fTmpLoc = fLoc;

  HShowerCal* pCell;

//  for(fTmpLoc[2] = row - nRange; fTmpLoc[2] <= row + nRange; fTmpLoc[2]++)
//    for(fTmpLoc[3] = col - nRange; fTmpLoc[3] <= col + nRange; fTmpLoc[3]++)
    for(fTmpLoc[2] = iRow1; fTmpLoc[2] <= iRow2; fTmpLoc[2]++)
    for(fTmpLoc[3] = iCol1; fTmpLoc[3] <= iCol2; fTmpLoc[3]++)
    {
        pCell =  (HShowerCal*)m_pCellArr->getObject(fTmpLoc);
        fDiff +=  (pCell->getCharge() - avg);
        fVar += fDiff * fDiff;
    }

  return fVar;
}

Float_t HShowerHitFinder::calculateSum(HLocation &fLoc, int nRange, int* pncs)
{
  int nThreshold = ((HShowerHitFPar*)getHitFPar())->getThreshold();
  int rowL, rowU, colL, colR,iRow1, iRow2, iCol1, iCol2;

  int row = fLoc[2];
  int col = fLoc[3];
  ((HShowerHitFPar*)getHitFPar())->getRowBord(fLoc[0], fLoc[1], &rowL, &rowU);
  ((HShowerHitFPar*)getHitFPar())->getColBord(fLoc[0], fLoc[1], &colL, &colR);
/*
  if ((row < rowL + nRange) ||  (row >  rowU - nRange))
     return 0.0;

  if ((col < colL + nRange) ||  (col >  colR - nRange))
     return 0.0;
*/
 

   if((row < rowL) || (row > rowU) || (col < colL) || (col > colR))
     return 0.0f;

  if((iRow1 = row - nRange) < rowL)
     iRow1 = rowL;

  if((iRow2 = row + nRange) > rowU)
     iRow2 = rowU;

  if((iCol1 = col - nRange) < colL)
     iCol1 = colL;

  if((iCol2 = col + nRange) > colR)
     iCol2 = colR;

  float fSum = 0;
  HLocation fTmpLoc;
  fTmpLoc = fLoc;

  int cs = 0;
  HShowerCal* pCell;

//  for(fTmpLoc[2] = row - nRange; fTmpLoc[2] <= row + nRange; fTmpLoc[2]++)
//   for(fTmpLoc[3] = col - nRange; fTmpLoc[3] <= col + nRange; fTmpLoc[3]++)
    for(fTmpLoc[2] = iRow1; fTmpLoc[2] <= iRow2; fTmpLoc[2]++)
    for(fTmpLoc[3] = iCol1; fTmpLoc[3] <= iCol2; fTmpLoc[3]++)

    {
        pCell =  (HShowerCal*)m_pCellArr->getObject(fTmpLoc);
        fSum +=  pCell->getCharge();
        if (pCell->getCharge() > nThreshold)
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


