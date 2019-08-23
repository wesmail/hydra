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

  void setXYDim(Float_t x, Float_t y) {xDim = x; yDim = y;}
  void setSector(Int_t sec)           {sector = sec;}
  void setTotalCharge(Float_t charge) {totalCharge=charge;}
  void setNrPad(Int_t padnr)          {nrPad=padnr;}
  void setPhiDiff(Float_t phi)        {phiDiff=phi;}
  void setTheta(Float_t T)            {theta = T;}

  Int_t   getSector()       {return sector;}
  Int_t   getNrPad()        {return nrPad;}
  Float_t getXDim()         {return xDim;}
  Float_t getYDim()         {return yDim;}
  Float_t getTotalCharge()  {return totalCharge;}
  Float_t getPhiDiff()      {return phiDiff;}
  Float_t getTheta()        {return theta;}

private:
  Int_t   sector;
  Int_t   nrPad;
  Float_t xDim;
  Float_t yDim;
  Float_t totalCharge;
  Float_t phiDiff;
  Float_t theta;


  ClassDef(HRichDirClus,2)
};
#endif // HRICHDIRCLUS_H
