#ifndef HRICHPARASCIIFILEIO_H
#define HRICHPARASCIIFILEIO_H

#include <fstream.h>

#include "hdetpario.h"
#include "hdetparasciifileio.h"
#include "TObject.h"
#include "TFile.h"
#include "TArrayI.h"

class HRichParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HRichParAsciiFileIo(fstream* f);
  ~HRichParAsciiFileIo() {}

  // calls special read-function for each container type
  Bool_t init(HParSet*,Int_t*);

  // calls special update-function for each container type
  Int_t write(HParSet*);

  // writes the setup information to file
  Bool_t write(HDetector*);

  ClassDef(HRichParAsciiFileIo,0) 
};

#endif  // HRICHPARASCIIFILEIO_H 
