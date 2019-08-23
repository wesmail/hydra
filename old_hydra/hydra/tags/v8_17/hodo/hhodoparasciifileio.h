#ifndef HHODOPARASCIIFILEIO_H
#define HHODOPARASCIIFILEIO_H

using namespace std;
#include <fstream> 

#include "TObject.h"
#include "TArrayI.h"
#include "hdetparasciifileio.h"

class HHodoDetector;
class HParSet;

class HHodoParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HHodoParAsciiFileIo(fstream*);
  ~HHodoParAsciiFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
private:
  template<class T> Bool_t read(T*, Int_t*, Bool_t needsClear=kFALSE);
  template<class T> Int_t writeFile3(T*);
  template<class T> Int_t writeFile2(T*);
  template<class T> Int_t writeFile1(T*);

public:
  ClassDef(HHodoParAsciiFileIo,0) // Parameter I/O from ASCII file for the Pion Hodoscopes
};

#endif  /* !HHODOPARASCIIFILEIO_H */







