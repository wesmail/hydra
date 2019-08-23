// File: HShowerReadParam.h
//
//

#ifndef HShowerReadParam_H
#define HShowerReadParam_H

class HShowerReadParam;

#include "hshowercalpar.h"
#include "hshowercalparcell.h"
#include "hbasereadparam.h"

class HShowerReadParam: public HBaseReadParam
{
public:
  HShowerReadParam();

  virtual int readParams();
  void setSetup(int nSector, Int_t m_nModules);

private:
  Int_t m_nSectors;
  Int_t m_nModules;

  Char_t m_szCalibTreeName[60]; 

  HShowerCalParCell* getCell(int, int, int, int);

  ClassDef(HShowerReadParam, 1)	
};

#endif /* !HShowerReadParam_H */
