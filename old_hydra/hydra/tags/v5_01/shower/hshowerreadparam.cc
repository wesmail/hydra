// File: hbasereadpar.cc
//
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include <iomanip.h>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include "hshowerreadparam.h"
#include "hshowercalpar.h"
#include "hades.h"
#include "hdebug.h"
#include "hcategory.h"
#include "hlocation.h"

ClassImp(HShowerReadParam)

HShowerReadParam::HShowerReadParam() : HBaseReadParam()
{
  setCalPar(NULL);
  setSetup(6, 2);

  strcpy(m_szCalibTreeName, "ShowerCalibData");
}


void HShowerReadParam::setSetup(Int_t nSectors, Int_t nModules) {
  m_nSectors = nSectors;
  m_nModules = nModules;
}

HShowerCalParCell* HShowerReadParam::getCell(int sec, int mod, int row, int col)
{
   HShowerCalParCell *pCell;
   HLocation *pLoc = new HLocation;

   pLoc->setNIndex(4);
   pLoc->setIndex(0, sec);
   pLoc->setIndex(1, mod);
   pLoc->setIndex(2, row);
   pLoc->setIndex(3, col);

 
   pCell = (HShowerCalParCell*) ((HShowerCalPar*)getCalPar())
                                   ->getSlot(*pLoc);
   if (pCell) 
      pCell = new(pCell) HShowerCalParCell;

   delete pLoc;
    
   return pCell; 
}

int HShowerReadParam::readParams()
{
   struct calpar
   {
     Float_t fSlope;
     Float_t fOffset;
   };

   struct calpar calibPar[m_nSectors][m_nModules];  //6 sectors, 2 modules
   Char_t branchname[20];
 
   Int_t nSec, nMod, nRow, nCol, nCur;
   Stat_t nEntries;

   HShowerCalParCell* pCell;
   TFile *f = new TFile(getFileName());
   TTree *pTree = (TTree*)f->Get(m_szCalibTreeName);
   TBranch *br[m_nSectors];
   TBranch *b;
   for(nSec = 0; nSec < m_nSectors; nSec++) {
      sprintf(branchname, "Sector%d", nSec);
      b = br[nSec] = pTree->GetBranch(branchname);
      br[nSec]->SetAddress(calibPar[nSec]);
   } 
 
   nEntries = pTree->GetEntries();
   for(nCur = 0; nCur < nEntries; nCur++) {
      pTree->GetEvent();
 
      nRow = nCur % 32;
      nCol = nCur / 32;
    
      for(nSec = 0; nSec < m_nSectors; nSec++) 
         for(nMod = 0; nMod < m_nModules; nMod++) {
            pCell = getCell(nSec, nMod, nRow, nCol);
            pCell->setSlope(calibPar[nSec][nMod].fSlope);
            pCell->setOffset(-1.0 * calibPar[nSec][nMod].fOffset);
      }

   }

   return kTRUE;
}

