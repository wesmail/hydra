#ifndef HSPECPARASCIIFILEIO_H
#define HSPECPARASCIIFILEIO_H

#include "hdetparasciifileio.h"

class HSpecGeomPar;
class HScsPar;

class HSpecParAsciiFileIo : public HDetParAsciiFileIo {
protected:
public:
  HSpecParAsciiFileIo(fstream* f);
  ~HSpecParAsciiFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  Bool_t read(HSpecGeomPar*,Int_t*);
  Int_t writeFile(HSpecGeomPar*);
  Int_t writeFile(HScsPar*);

  ClassDef(HSpecParAsciiFileIo,0) // Class for spectrometer parameter I/O from ascii file
};

#endif  /* !HSPECPARASCIIFILEIO_H */
