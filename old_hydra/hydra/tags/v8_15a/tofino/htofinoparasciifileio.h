#ifndef HTOFINOPARASCIIFILEIO_H
#define HTOFINOPARASCIIFILEIO_H
using namespace std;
#include <fstream> 

#include "TObject.h"
#include "TArrayI.h"
#include "hdetparasciifileio.h"

class HTofinoDetector;
class HParSet;

class HTofinoParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HTofinoParAsciiFileIo(fstream*);
  ~HTofinoParAsciiFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  template<class T> Bool_t read(T*, Int_t*, Bool_t needsClear=kFALSE);
  template<class T> Int_t writeFile2(T*);
  ClassDef(HTofinoParAsciiFileIo,0) // Class for TOFINO parameter I/O from Ascii files
};

#endif  /* !HTOFINOPARASCIIFILEIO_H */







