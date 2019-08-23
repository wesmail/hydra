// File: hrichdirclus.cc
// ****************************************************************************
//
// HRichDirClus
//
// Author: Author: Laura Fabbietti <Laura.Fabbietti@ph.tum.de>
// Modified: 2005/03/11 by Martin Jurkovic <martin.jurkovic@ph.tum.de>
//
// ****************************************************************************

using namespace std;
#include "hrichdirclus.h"
ClassImp(HRichDirClus)
  
HRichDirClus::HRichDirClus(){
  xDim = 0.;
  yDim = 0.;
  sector = 0 ;
  totalCharge = 0.;
  nrPad = 0 ;
  phiDiff = 0.;
}
HRichDirClus::~HRichDirClus(){
}
