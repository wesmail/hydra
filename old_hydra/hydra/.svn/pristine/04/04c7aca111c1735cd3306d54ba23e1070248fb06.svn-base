#ifndef HMDCPARASCIIO_H
#define HMDCPARASCIIO_H

#include <fstream.h>
#include "hdetpario.h"
#include "TFile.h"
#include "TArrayI.h"

class HMdcDetector;
class HMdcRawStruct;
class HMdcGeomStruct;
class HMdcMboReadout;
class HMdcCalPar;
class HMdcLookupGeom;
class HMdcLookupRaw;
class HMdcDigitPar;
class HMdcDigitGeomPar;

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

  // read/write of MdcMboReadout container (readout addresses of the MDC
  // motherboards)
  Bool_t read(HMdcMboReadout*,Int_t*);
  Int_t writeFile(HMdcMboReadout*);

  // read/write calibration parameters and fill MdcCalPar container
  Bool_t read(HMdcCalPar*,Int_t*);
  Int_t writeFile(HMdcCalPar*);

  // read/write lookup table from raw to cal1 level
  Bool_t read(HMdcLookupGeom*,Int_t*);
  Int_t writeFile(HMdcLookupGeom*);

  // write lookup table from cal1 to raw level
  // no read function
  Int_t writeFile(HMdcLookupRaw*);

  // read/write digitization parameters and fill MdcDigitPar container
  Bool_t read(HMdcDigitPar*,Int_t*);
  Int_t writeFile(HMdcDigitPar*);

  // read/write geometry parameters for digitization  and fill
  // MdcDigitGeomPar container
  Bool_t read(HMdcDigitGeomPar*,Int_t*);
  Int_t writeFile(HMdcDigitGeomPar*);

  ClassDef(HMdcParAsciIo,0) // Class for MDC parameter I/O from Asci files
};

#endif  /* !HMDCPARASCIIO_H */
