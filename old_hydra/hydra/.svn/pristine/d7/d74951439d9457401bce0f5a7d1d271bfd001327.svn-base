// File: hbasereadpar.cc

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <iomanip.h>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include "hrichreadparam.h"
#include "hrichcalpar.h"
#include "hades.h"
#include "hdebug.h"
#include "hcategory.h"
#include "hlocation.h"

ClassImp(HRichReadParam)

HRichReadParam::HRichReadParam() : HBaseReadParam()
{
  setCalPar(NULL);
  setSetup(6);

  strcpy(m_szCalibTreeName, "RichCalibData");
}


void HRichReadParam::setSetup(Int_t nSectors) {
  m_nSectors = nSectors;
}

HRichCalParCell* HRichReadParam::getCell(int sec, int row, int col)
{
   HRichCalParCell *pCell;
   HLocation *pLoc = new HLocation;

   pLoc->setNIndex(3);
   pLoc->setIndex(0, sec);
   pLoc->setIndex(1, row);
   pLoc->setIndex(2, col);

 
   pCell = (HRichCalParCell*) ((HRichCalPar*)getCalPar())
                                   ->getSlot(*pLoc);
   if (pCell) 
      pCell = new(pCell) HRichCalParCell;

   delete pLoc;
    
   return pCell; 
}

int HRichReadParam::readParams()
{
   struct calpar
   {
     Float_t fSlope;
     Float_t fOffset;
   };

   struct calpar calibPar[m_nSectors];  // 6 sectors
   Char_t branchname[20];
 
   Int_t nSec, nRow, nCol, nCur;
   Stat_t nEntries;

   HRichCalParCell* pCell;
   TFile *f = new TFile(getFileName());
   TTree *pTree = (TTree*)f->Get(m_szCalibTreeName);
   TBranch *br[m_nSectors];
   TBranch *b;
   for(nSec = 0; nSec < m_nSectors; nSec++) {
      sprintf(branchname, "Sector%d", nSec);
      b = br[nSec] = pTree->GetBranch(branchname);
      br[nSec]->SetAddress(calibPar);
   } 
 
   nEntries = pTree->GetEntries();
   for(nCur = 0; nCur < nEntries; nCur++) {
      pTree->GetEvent(nCur);
 
      nRow = nCur / 32;
      nCol = nCur % 32;
   
      for(nSec = 0; nSec < m_nSectors; nSec++) {
            pCell = getCell(nSec, nRow, nCol);
            pCell->setSlope(calibPar[nSec].fSlope);
            pCell->setOffset(-1.0 * calibPar[nSec].fOffset);
         }
      

   }

   return kTRUE;
}

