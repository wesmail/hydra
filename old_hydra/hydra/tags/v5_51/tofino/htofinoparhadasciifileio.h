#ifndef HTofinoParHadAsciiFileIo_h
#define HTofinoParHadAsciiFileIo_h

#include <fstream.h>

#include "hdetpario.h"
#include "hdetparhadasciifileio.h"
#include "TObject.h"
#include "TFile.h"
#include "TArrayI.h"

class HTofinoCalPar;
class HTofinoShowerMap;

class HTofinoParHadAsciiFileIo : public HDetParHadAsciiFileIo {
public:
  HTofinoParHadAsciiFileIo(HParHadAsciiFileIo* f);
  ~HTofinoParHadAsciiFileIo() {}

  // calls special read-function for each container type
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HTofinoCalPar* pPar,Int_t* set);
  Bool_t read(HTofinoShowerMap* pPar,Int_t* set);

  // calls special update-function for each container type
  Int_t write(HParSet*);

  Bool_t writeFile(HTofinoCalPar* pPar);
  Bool_t writeFile(HTofinoShowerMap* pPar);
  // writes the setup information to file
  Bool_t write(HDetector*);

  ClassDef(HTofinoParHadAsciiFileIo,0)  // Tofino parameter IO from ascii file
};

#endif  /* !HSHOWERPARHADASCIIFILEIO_H */
