#ifndef HMDCPARASCIIO_H
#define HMDCPARASCIIO_H

#include <fstream.h>
#include "hdetpario.h"
#include "TFile.h"
#include "TArrayI.h"

class HMdcDetector;
class HMdcRawStruct;
class HMdcGeomStruct;
class HMdcCalPar;
class HMdcLookupGeom;

class HMdcParAsciIo : public HDetParIo {
protected:
  fstream* pFile;  // pointer to file
public:
  HMdcParAsciIo(fstream* f);
  ~HMdcParAsciIo() {}

  // calls special read-function for each container type
  Bool_t init(HParSet*,Int_t*);

  // calls special update-function for each container type
  Int_t write(HParSet*);

  // writes the setup information to file
  Bool_t write(HDetector*);

  // read/write number of TDC channels and fills MdcRawStruct container
  Bool_t read(HMdcRawStruct*,Int_t*);
  Int_t writeFile(HMdcRawStruct*);

  // read/write number of cells and fills MdcGeomStruct container
  Bool_t read(HMdcGeomStruct*,Int_t*);
  Int_t writeFile(HMdcGeomStruct*);

  // read/write calibration parameters and fill MdcCalPar container
  Bool_t read(HMdcCalPar*,Int_t*);
  Int_t writeFile(HMdcCalPar*);

  // read/write lookup table from raw to cal1 level
  Bool_t read(HMdcLookupGeom*,Int_t*);
  Int_t writeFile(HMdcLookupGeom*);

  ClassDef(HMdcParAsciIo,0) // Class for MDC parameter I/O from Asci files
};

#endif  /* !HMDCPARASCIIO_H */
