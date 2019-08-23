//*-- Author : Laura Fabbietti
//////////////////////////////////////////////////////////////////////////
//________________________________________________________________________
//
// HShowerHitFinderSim
// this class is the exact clone of HShowerHitFinder, for the simulated data.
// In the catShowerCal container HShowerCal is replaced by HSHowerCalSim, and
// in the catShowerHit container HShowerHit is replaced by HSHowerHitSim.
//
////////////////////////////////////////////////////////////////////////// 

#include "hshowerhitsim.h"
#include "hshowerpid.h"
#include "hshowerhitheader.h"
#include "hshowerhitfindersim.h"

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
#include "hshowercalsim.h"
#include "hshowerhitfpar.h"
#include "hshowergeometry.h"
#include "hshowerpad.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"
#include "showerdef.h"

ClassImp(HShowerHitFinderSim)

Bool_t HShowerHitFinderSim::init() {
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
       m_pCalCat=pShowerDet->buildLinearCat("HShowerCalSim");
       if (!m_pCalCat) return kFALSE;
       else gHades->getCurrentEvent()
                  ->addCategory(catShowerCal, m_pCalCat, "Shower");
    }

    m_pHitCat=gHades->getCurrentEvent()->getCategory(catShowerHit);
    if (!m_pHitCat) {
       m_pHitCat=pShowerDet->buildLinearCat("HShowerHitSim");

       if (!m_pHitCat) return kFALSE;
       else gHades->getCurrentEvent()
                  ->addCategory(catShowerHit, m_pHitCat, "Shower");
    }

    m_pPIDCat=gHades->getCurrentEvent()->getCategory(catShowerPID);
    if (!m_pPIDCat) {
       m_pPIDCat=pShowerDet->buildCategory(catShowerPID);

       if (!m_pPIDCat) return kFALSE;
       else gHades->getCurrentEvent()
                  ->addCategory(catShowerPID, m_pPIDCat, "Shower");
    }

    m_pHitHdrCat=gHades->getCurrentEvent()->getCategory(catShowerHitHdr);
    if (!m_pHitHdrCat) {
       m_pHitHdrCat=pShowerDet->buildCategory(catShowerHitHdr);

       if (!m_pHitHdrCat) return kFALSE;
       else gHades->getCurrentEvent()
                  ->addCategory(catShowerHitHdr, m_pHitHdrCat, "Shower");
    }

    m_pCellArr = new HRaTreeExt(m_pCalCat, &arr);
    fIter = (HIterator*)getCalCat()->MakeIterator("native"); 

    return initParameters();
}

Int_t HShowerHitFinderSim::execute()
{
//  cout<<"executing hit finder"<<endl;
  HShowerCalSim *pCalSim;
  m_pCellArr->update();
 
  Int_t n = 0;
  Int_t hits = 0;

  fIter->Reset();
  while((pCalSim = (HShowerCalSim *)fIter->Next()))
  {
     m_Loc[0] = pCalSim->getSector();
     m_Loc[1] = pCalSim->getModule();
     m_Loc[2] = pCalSim->getRow();
     m_Loc[3] = pCalSim->getCol();
  
     if (lookForHit(pCalSim, m_Loc)) hits++;
     n++;
  }

  return 0;
}

Bool_t HShowerHitFinderSim::lookForHit(HShowerCalSim* cal, HLocation& fLoc) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerHitFinder::execute");
  gDebuger->message("Cal cat points to %p",fCalCat);
#endif

//  cout<<"executing lookForHit"<<endl;
  HShowerHitSim *hitSim=NULL;
  HShowerPID *pid=NULL;
  Bool_t isHit = kFALSE;

  if (cal && isLocalMax(fLoc)) {
    Float_t fCharge;
    hitSim=(HShowerHitSim *)m_pHitCat->getNewSlot(fLoc);
    if (hitSim) {
      hitSim=new(hitSim) HShowerHitSim;

      fillSums(hitSim, fLoc);
      calcCoord(hitSim, fLoc);
       
      fCharge = cal->getChannel();
      hitSim->setCharge(fCharge);
      hitSim->setSector(cal->getSector());
      hitSim->setModule(cal->getModule());
      hitSim->setRow(cal->getRow());
      hitSim->setCol(cal->getCol());
      hitSim->setEventNr(cal->getEventNr());
 
 //*******************************
      hitSim->setNTrack1( cal->getNTrack1());
      hitSim->setNTrack2( cal->getNTrack2());
 //*******************************
 
      if (hitSim->getShower() > 0) {
          pid=(HShowerPID *)m_pPIDCat->getNewSlot(fLoc);
          if (pid) {
            pid=new(pid) HShowerPID;
            fillPID(hitSim, pid);
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

void HShowerHitFinderSim::fillPID(HShowerHitSim* hitSim, HShowerPID* pid) {
   float fX, fY, fZ;

   pid->setCharge(hitSim->getCharge());
   pid->setSector(hitSim->getSector());
   pid->setModule(hitSim->getModule());
   pid->setRow(hitSim->getRow());
   pid->setCol(hitSim->getCol());
   pid->setEventNr(hitSim->getEventNr());
   pid->setShower(hitSim->getShower());

   hitSim->getLabXYZ(&fX, &fY, &fZ);
   pid->setXYZ(fX, fY, fZ);
}

void HShowerHitFinderSim::fillSums(HShowerHitSim* hitSim, HLocation &fLoc) {
  HLocation fTmpLoc;
  int nModules = ((HShowerHitFPar*)getHitFPar())->getModules();
  float sum, var;
  int cs;

  fTmpLoc = fLoc;
  for(fTmpLoc[1] = 0; fTmpLoc[1] < nModules; fTmpLoc[1]++) {
     sum = calculateSum(fTmpLoc, 1, &cs);
     var = calculateVar(fTmpLoc, 1, sum/9.0);
     hitSim->setSum(fTmpLoc[1], sum);      
     hitSim->setVar(fTmpLoc[1], var);      
     hitSim->setClusterSize(fTmpLoc[1], cs);
  }

  calculateSum(fLoc, 1, &cs);
  if (cs > 1) updateClusters(fLoc);

  if (nModules == 3) {
     fTmpLoc[1] = 2;
     hitSim->setSum25(calculateSum(fTmpLoc, 2));
  }

  hitSim->updateCalc();
}

void HShowerHitFinderSim::calcCoord(HShowerHitSim* hitSim, HLocation &fLoc) {
  HGeomVector vLocalCoord;
  HGeomVector vLabCoord;

  ((HShowerGeometry*)getGeometry())->getLocalCoord(fLoc, vLocalCoord);
  ((HShowerGeometry*)getGeometry())->getLabCoord(fLoc, vLabCoord);

  hitSim->setXY(vLocalCoord.getX(), vLocalCoord.getY());
  hitSim->setLabXYZ(vLabCoord.getX(), vLabCoord.getY(), vLabCoord.getZ());
}









