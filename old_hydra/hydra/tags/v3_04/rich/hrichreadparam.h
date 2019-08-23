// File: HRichReadParam.h

#ifndef HRICHREADPARAM_H
#define HRICHREADPARAM_H

class HRichReadParam;

#include "hrichcalpar.h"
#include "hrichcalparcell.h"
#include "hbasereadparam.h"

class HRichReadParam: public HBaseReadParam
{
public:
  HRichReadParam();

  virtual int readParams();
  void setSetup(int nSector);

private:
  Int_t m_nSectors;

  Char_t m_szCalibTreeName[60]; 

  HRichCalParCell* getCell(int, int, int);

  ClassDef(HRichReadParam, 1)	
};

#endif // HRICHREADPARAM_H 
