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
    Int_t getRC(Int_t,Int_t);//
    Int_t getPort(Int_t,Int_t);//
    Int_t getMod(Int_t,Int_t);//
    Int_t getCh(Int_t,Int_t);//
    Int_t isReadOut(Int_t,Int_t);// is x,y of pad with valid channel
    Int_t getCol(Int_t);//
    Int_t getRow(Int_t);//
    Int_t isValidUpi(Int_t);// is uip valid ? (4800)
    Int_t isUnConnCh(Int_t);// is uip of channel without pad ? (88)

    TVector *uiplog;//1 marks channels with pad attached to it
    TVector *padsxy;//channel is mapped to col*100 + row
    TVector *padsx;//channel is mapped to col
    TVector *padsy;//channel is mapped to row
    TVector *xyuip;//col*100+row is mapped to its channel
    TVector *xyuiplog;//read out pads are marked by 1
    TVector *uncuip;//unconnected channels are marked by 1

    Int_t i, fPadsNr,fCols,fRCs,fPorts,fModules,fChannels,fRows,fAddress1,fAddress2;
    Int_t fUnconnCh;

  ClassDef(HRichMappingPar,1) //ROOT extension

private:

    
  
 };

#endif // HRICHMAPPINGPAR_H


