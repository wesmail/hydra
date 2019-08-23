// File: hrichdirclus.h
// ****************************************************************************
//
// HRichDirClus
//
// Author: Author: Laura Fabbietti <Laura.Fabbietti@ph.tum.de>
// Modified: 2005/03/11 by martin Jurkovic <martin.jurkovic@ph.tum.de>
//
// ----------------------------------------------------------------------------
//
// This class stores information about the clusters in the RICH 
// detector like cluster size, total charge accumulated in the
// cluster and number of fired pads in the cluster. 
//
// ****************************************************************************

#ifndef HRICHDIRCLUS_H
#define HRICHDIRCLUS_H

#include <TObject.h> 
#include <TArrayI.h> 

class HRichDirClus: public TObject {
public:
  HRichDirClus();
  virtual ~HRichDirClus();

  void setXYDim(Float_t x,Float_t y){xDim= x; yDim = y;}
  void setSector(int sec){sector = sec;}
  void setTotalCharge(float charge){totalCharge=charge;}
  void setNrPad(int padnr){nrPad=padnr;}
  void setPhiDiff(Float_t phi){phiDiff=phi;}

  Float_t getXDim(){return xDim;}
  Float_t getYDim(){return yDim;}
  Int_t getSector(){return sector;}
  Float_t getTotalCharge(){return totalCharge;}
  Int_t getNrPad(){return nrPad;}
  Float_t getPhiDiff(){return phiDiff;}

private:
  Float_t xDim, yDim;
  Int_t sector;
  Float_t totalCharge;
  Int_t nrPad;
  Float_t phiDiff;


  ClassDef(HRichDirClus,1)
};
#endif // HRICHDIRCLUS_H
