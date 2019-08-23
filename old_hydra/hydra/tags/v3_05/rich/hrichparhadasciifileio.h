#ifndef HRICHPARHADASCIIFILEIO_H
#define HRICHPARHADASCIIFILEIO_H

#include <fstream.h>

#include "hdetpario.h"
#include "hdetparhadasciifileio.h"
#include "TObject.h"
#include "TFile.h"
#include "TArrayI.h"

class HRichGeometry;
class HRichDigiDetPar;
class HRichHitFPar;
class HRichCalPar;
class HRichAnalysisPar;

class HRichParHadAsciiFileIo : public HDetParHadAsciiFileIo {
public:
  HRichParHadAsciiFileIo(HParHadAsciiFileIo* f);
  ~HRichParHadAsciiFileIo() {}

  // calls special read-function for each container type
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HRichGeometry* pPar,Int_t* set);
  Bool_t read(HRichDigiDetPar* pPar,Int_t* set);
  Bool_t read(HRichHitFPar* pPar,Int_t* set);
  Bool_t read(HRichCalPar* pPar,Int_t* set);
  Bool_t read(HRichAnalysisPar* pPar,Int_t* set);

  // calls special update-function for each container type
  Int_t write(HParSet*);

  Bool_t writeFile(HRichGeometry* pPar);
  Bool_t writeFile(HRichDigiDetPar* pPar);
  Bool_t writeFile(HRichHitFPar* pPar);
  Bool_t writeFile(HRichCalPar* pPar);
  Bool_t writeFile(HRichAnalysisPar* pPar);
  // writes the setup information to file
  Bool_t write(HDetector*);

  ClassDef(HRichParHadAsciiFileIo,0) 
};

#endif  // HRICHPARHADASCIIFILEIO_H
