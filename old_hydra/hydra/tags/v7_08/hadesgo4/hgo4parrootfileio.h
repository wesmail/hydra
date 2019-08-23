#ifndef HGO4PARROOTFILEIO_H
#define HGO4PARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TArrayI.h"

/** Class for GO4 parameter I/O from ROOT file.
*/
class HGo4ParRootFileIo : public HDetParRootFileIo {
protected:
    TArrayI *initModuleTypes;
public:
  HGo4ParRootFileIo(HParRootFile* f);
  ~HGo4ParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  ClassDef(HGo4ParRootFileIo,0) // Class for GO4 parameter I/O from ROOT file
};

#endif  /*!HGO4PARROOTFILEIO_H*/







