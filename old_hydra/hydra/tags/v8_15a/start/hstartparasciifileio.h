#ifndef HSTARTPARASCIIFILEIO_H
#define HSTARTPARASCIIFILEIO_H
using namespace std;
#include <fstream> 

#include "TObject.h"
#include "TArrayI.h"
#include "hdetparasciifileio.h"

class HStartDetector;
class HParSet;

class HStartParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HStartParAsciiFileIo(fstream*);
  ~HStartParAsciiFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  template<class T> Bool_t read(T*, Int_t*, Bool_t needsClear=kFALSE);
  template<class T> Bool_t readStartModule(T*, Int_t*, Bool_t needsClear=kFALSE);
  template<class T> Int_t writeFile2(T*);
  ClassDef(HStartParAsciiFileIo,0) // Class for START parameter I/O from Ascii files
};

#endif  /* !HSTARTPARASCIIFILEIO_H */







