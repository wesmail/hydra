#ifndef HGO4PARASCIIFILEIO_H
#define HGO4PARASCIIFILEIO_H
using namespace std;
#include <fstream> 
#include "hdetparasciifileio.h"

class HParSet;
/** Class for Ascii file Io of all containers of HadesGo4
*/
class HGo4ParAsciiFileIo : public HDetParAsciiFileIo {
public:
  HGo4ParAsciiFileIo(fstream*);
  ~HGo4ParAsciiFileIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  ClassDef(HGo4ParAsciiFileIo,0) // Class for HadesGo4 parameter I/O from Ascii files
};

#endif  /* !HGO4PARASCIIFILEIO_H */



