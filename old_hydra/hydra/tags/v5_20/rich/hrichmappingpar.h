#ifndef HRICHMAPPINGPAR_H
#define HRICHMAPPINGPAR_H

#include <TVector.h>
#include "hparset.h"
#include "hrichparset.h"
//#include "hlocation.h"

class HParHadAsciiFileIo;

class HRichMappingPar : public HRichParSet {

public:

  HRichMappingPar();
  virtual ~HRichMappingPar();

  virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
  virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);


  ClassDef(HRichMappingPar,1) //ROOT extension

private:

   TVector *uiplog;
   TVector *padsxy;
   TVector *padsx;
   TVector *padsy;
   TVector *xyuip;
   TVector *xyuiplog;
  
  Int_t i, fPadsNr,fCols,fRCs,fPorts,fModules,fChannels,fRows,fAddress1,fAddress2;
 };

#endif // HRICHMAPPINGPAR_H


