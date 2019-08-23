#include <iostream.h>
#include "hades.h"
#include "hparhadasciifileio.h"
#include "hspectrometer.h"
#include "hrichparset.h"
#include "hrichdetector.h"
#include "hrichmappingpar.h"

ClassImp(HRichMappingPar)

HRichMappingPar::HRichMappingPar() {
  SetName("RichMappingParameters");
}

HRichMappingPar::~HRichMappingPar()
{ 
}



//----------------------------------------------------------------------------
Bool_t HRichMappingPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

  Int_t  fCols2, fRows2, fRCs2, fPorts2, fModules2, fChannels2;

  using namespace HadAsciiAuxiliaryFunctions;
  if (!pHadAsciiFile) return kFALSE;

  Bool_t status = kTRUE;


 try {

  HAsciiRaw &mydata = *pHadAsciiFile->GetRawAscii();

  mydata.SetActiveSection("Rich Mapping Parameters Setup");

  mydata.SetReadKeyTypes('i','i','i','i','i','i','i');
  mydata.ReadRawLine(0,&fPadsNr,&fRows,&fCols,&fRCs,&fPorts,&fModules,&fChannels);
  fAddress1 = (fRCs-1)*10000 + (fPorts-1)*1000 + (fModules-1)*100 + fChannels;
  fAddress2 = fCols*100 + fRows;
  uiplog =   new TVector(fAddress1);
  padsxy =   new TVector(fAddress1);
  padsx  =   new TVector(fAddress1);
  padsy  =   new TVector(fAddress1);
  xyuip  =   new TVector(fAddress2);
  xyuiplog = new TVector(fAddress2);
  for (i=0;i<fAddress1;i++){
      (*uiplog)(i) = 0;
      (*padsxy)(i) = 0;
      (*padsx)(i)  = 0;
      (*padsy)(i)  = 0;
  }
  for (i=0;i<fAddress2;i++){
      (*xyuip)(i)    = 0;
      (*xyuiplog)(i) = 0;
  } 
  mydata.SetActiveSection("Rich Mapping Parameters");
  mydata.SetReadKeyTypes('i','i','i','i','i','i');

   Int_t line = 0;
   for(i = 0; i < fPadsNr; i++) {
    mydata.ReadRawLine(line, &fCols2, &fRows2, &fRCs2,&fPorts2,&fModules2,&fChannels2);
    fAddress1 = fRCs2*10000 + fPorts2*1000 + fModules2*100 + fChannels2;
    fAddress2 = fCols2*100 + fRows2;    
    (*uiplog)(fAddress1)   = 1;
    (*padsxy)(fAddress1)   = fCols2*100 + fRows2;
    (*padsx)(fAddress1)    = fCols2;
    (*padsy)(fAddress1)    = fRows2;
    (*xyuip)(fAddress2)    = fAddress1;
    (*xyuiplog)(fAddress2) = 1;

    line++;
   }
  

 } // eof try block

 catch (Bool_t ret) {
  status = ret;
 }
 catch (...) {
  status = kFALSE;
  ErrorMsg(2,"HRichMappingPar::initAscii",1,"Unidentified exception catched.");
 }

 if (status)
  ErrorMsg(0,"HRichMappingPar::initAscii",
   1,"Container \'RichMappingPar\' has been read from ASCII file.");

 return status;
 }
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichMappingPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {

   Int_t tmp, rc, port, mod, ch;


 using namespace HadAsciiAuxiliaryFunctions;
 if (!pHadAsciiFile) return kFALSE;

 Bool_t status = kTRUE;


 try {

  HAsciiRaw &mydata = *pHadAsciiFile->GetRawAscii();

  mydata.WriteSection("Rich Mapping Parameters Setup");
  mydata.WriteKeywords(7, "MaxPads", "MaxRows", "MaxCols", "MaxRCs", "MaxPorts", "MaxModules", "MaxChannels");
  mydata.SetSeparator('\t');
  mydata.SetWriteKeyTypes('i', 'i', 'i','i', 'i', 'i','i');
  mydata.WriteRawLine(fPadsNr,fRows,fCols,fRCs,fPorts,fModules,fChannels);

  mydata.WriteSection("Rich Mapping Parameters");
   mydata.WriteKeywords(6, "Col","Row","RC","Port","Module","Channel");
   mydata.SetWriteKeyTypes('i','i','i','i','i','i');
   fAddress1 = (fRCs-1)*10000 + (fPorts-1)*1000 + (fModules-1)*100 + fChannels-1;

   for(i = 0; i < fAddress1; i++) 
     if ( (*uiplog)(i)) {
       
       rc = i/10000;
       tmp = i % 10000;
       port = tmp/1000;
       tmp = tmp % 1000;
       mod = tmp/100;
       ch = tmp % 100;

      mydata.WriteRawLine((Int_t)(*padsx)(i),(Int_t)(*padsy)(i),(Int_t) rc, (Int_t) port, (Int_t) mod, (Int_t) ch );
     }
 
  
 } // eof try block

 catch (Bool_t ret) {
  status = ret;
 }
 catch (...) {
  status = kFALSE;
  ErrorMsg(2,"HRichMappingPar::writeAscii",1,"Unidentified exception catched.");
 }

 if (status)
  ErrorMsg(0,"HRichMappingPar::writeAscii",
   1,"Container \'RichMappingPar\' has been written to ASCII file.");

 return status;
}
//============================================================================





