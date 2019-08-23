// File: HBaseReadParam.h 
//
//

#ifndef HBaseReadParam_H
#define HBaseReadParam_H

#include "TError.h"
#include "hparset.h"

class HBaseReadParam : public TObject
{
public:
  HBaseReadParam();

  virtual int readParams() {return 0;}
 
  Char_t* getFileName();
  void setFileName(char* pszName);
  void setCalPar(HParSet*pCalPar);
  HParSet *getCalPar();

protected:
  HParSet* m_pCalPar;

private:
  Char_t m_szFileName[40];


ClassDef(HBaseReadParam, 0)	// base class for reading calibration parameters
};

#endif /* !HBaseReadParam_H */
