#ifndef HDETPARASCIIFILEIO_H
#define HDETPARASCIIFILEIO_H

#include <fstream.h>
#include "hdetpario.h"
#include "TArrayI.h"
#include "hgeomtransform.h"

class HDetGeomPar;
class HGeomVolume;
class HGeomShapes;

class HDetParAsciiFileIo : public HDetParIo {
protected:
  fstream* pFile;  //! pointer to ascii file
  Bool_t readLabPositions(const Text_t*,HDetGeomPar*,Int_t*);
  Bool_t readVolumes(const Text_t*,HDetGeomPar*);
  void readTransform(HGeomTransform&);
  Bool_t readVolume(HGeomVolume*,HGeomShapes*,Text_t*);
  void writeTransform(const HGeomTransform&);
  void writeVolume(HGeomVolume*,HGeomShapes*);
public:
  HDetParAsciiFileIo(fstream* f);
  ~HDetParAsciiFileIo() {}
  Bool_t read(HDetGeomPar*,Int_t*);
  Int_t writeFile(HDetGeomPar*);

  ClassDef(HDetParAsciiFileIo,0) // Class for detector parameter I/O from ascii file
};

#endif  /* !HDETPARASCIIFILEIO_H */
