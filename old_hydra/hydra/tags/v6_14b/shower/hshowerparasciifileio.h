#ifndef HSHOWERPARASCIIFILEIO_H
#define HSHOWERPARASCIIFILEIO_H
using namespace std;
#include <fstream> 

#include "hdetpario.h"
#include "hdetparasciifileio.h"
#include "TObject.h"
#include "TFile.h"
#include "TArrayI.h"

class HShowerParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HShowerParAsciiFileIo(fstream* f);
  ~HShowerParAsciiFileIo() {}

  // calls special read-function for each container type
  Bool_t init(HParSet*,Int_t*);

  // calls special update-function for each container type
  Int_t write(HParSet*);

  // writes the setup information to file
  Bool_t write(HDetector*);

  ClassDef(HShowerParAsciiFileIo,0) 
};

#endif  /* !HSHOWERPARASCIIFILEIO_H */
