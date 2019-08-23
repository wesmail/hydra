#ifndef HRICHCALPAR_H
#define HRICHCALPAR_H

#include "hrichparset.h"
#include "hlocation.h"
#include "hobjtable.h"

class HParHadAsciiFileIo;
class HRichCalParCell;

class HRichCalPar : public HRichParSet {

public:

  HRichCalPar(const Char_t* name    = "RichCalPar",
              const Char_t* title   = "Rich Calibration Parameters",
              const Char_t* context = "RichPrimaryCalibration");
  ~HRichCalPar();


  // getters
  Int_t   getNSectors()             {return fSectors;} 
  Int_t   getNRows()                {return fRows;} 
  Int_t   getNColumns()             {return fColumns;} 
  Char_t* getCellClassName() {return m_szClassName;}

  HRichCalParCell* getSlot(HLocation &loc);
  HRichCalParCell* getObject(HLocation &loc);


  // setters
  void setCellClassName(const Char_t* pszName);

  //----------------------------------------------------------
  void clear();  
  void defaultInit();
  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);

  void PrintCalPar();

  ClassDef(HRichCalPar,2) //ROOT extension

//----------------------------------------------------------
private:
  Int_t fSectors;
  Int_t fRows;
  Int_t fColumns;

  Char_t m_szClassName[40];

  HObjTable m_ParamsTable;

};

#endif // HRICHCALPAR_H
