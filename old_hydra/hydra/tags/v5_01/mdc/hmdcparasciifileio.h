#ifndef HMDCPARASCIIFILEIO_H
#define HMDCPARASCIIFILEIO_H

#include <fstream.h>

#include "TObject.h"
#include "TArrayI.h"
#include "hdetparasciifileio.h"

class HMdcRawStruct;
class HMdcGeomStruct;
class HMdcMboReadout;
class HMdcCalPar;
class HMdcLookupGeom;
class HMdcLookupRaw;
class HMdcDigitPar;
class HMdcLayerGeomPar;
class HMdcCalParRaw;
class HParSet;

class HMdcParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HMdcParAsciiFileIo(fstream*);
  ~HMdcParAsciiFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  template<class T> Bool_t read(T*, Int_t*);
  template<class T> Int_t writeFile3(T*);
  template<class T> Int_t writeFile4(T*);
  ClassDef(HMdcParAsciiFileIo,0) // Class for MDC parameter I/O from Ascii files
};

#endif  /* !HMDCPARASCIIFILEIO_H */
