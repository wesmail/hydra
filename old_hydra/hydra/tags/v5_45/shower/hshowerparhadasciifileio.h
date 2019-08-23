#ifndef HSHOWERPARHADASCIIFILEIO_H
#define HSHOWERPARHADASCIIFILEIO_H

#include <fstream.h>

#include "hdetpario.h"
#include "hdetparhadasciifileio.h"
#include "TObject.h"
#include "TFile.h"
#include "TArrayI.h"

class HShowerGeometry;
class HShowerDigiDetPar;
class HShowerHitFPar;
class HShowerCalPar;
class HShowerHist;
class HShowerUnpackParam;

class HShowerParHadAsciiFileIo : public HDetParHadAsciiFileIo {
public:
  HShowerParHadAsciiFileIo(HParHadAsciiFileIo* f);
  ~HShowerParHadAsciiFileIo() {}

  // calls special read-function for each container type
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HShowerGeometry* pPar,Int_t* set);
  Bool_t read(HShowerDigiDetPar* pPar,Int_t* set);
  Bool_t read(HShowerHitFPar* pPar,Int_t* set);
  Bool_t read(HShowerCalPar* pPar,Int_t* set);
  Bool_t read(HShowerHist* pPar,Int_t* set);
  Bool_t read(HShowerUnpackParam* pPar,Int_t* set);

  // calls special update-function for each container type
  Int_t write(HParSet*);

  Bool_t writeFile(HShowerGeometry* pPar);
  Bool_t writeFile(HShowerDigiDetPar* pPar);
  Bool_t writeFile(HShowerHitFPar* pPar);
  Bool_t writeFile(HShowerCalPar* pPar);
  Bool_t writeFile(HShowerUnpackParam* pPar);
  // writes the setup information to file
  Bool_t write(HDetector*);

  ClassDef(HShowerParHadAsciiFileIo,0) 
};

#endif  /* !HSHOWERPARHADASCIIFILEIO_H */
