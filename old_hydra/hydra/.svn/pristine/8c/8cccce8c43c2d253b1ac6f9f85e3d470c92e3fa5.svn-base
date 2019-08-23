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

  mydata.SetReadKeyTypes('i','i','i','i','i','i','i','i');
  mydata.ReadRawLine(0,&fPadsNr,&fRows,&fCols,&fRCs,&fPorts,&fModules,&fChannels,&fUnconnCh);
  fAddress1 = (fRCs-1)*10000 + (fPorts-1)*1000 + (fModules-1)*100 + fChannels;
  fAddress2 = fCols*100 + fRows;
  uiplog =   new TVector(fAddress1);
  padsxy =   new TVector(fAddress1);
  padsx  =   new TVector(fAddress1);
  padsy  =   new TVector(fAddress1);
  xyuip  =   new TVector(fAddress2);
  xyuiplog = new TVector(fAddress2);
  uncuip = new TVector(fAddress1);
  for (i=0;i<fAddress1;i++){
      (*uiplog)(i) = 0;
      (*padsxy)(i) = 0;
      (*padsx)(i)  = 0;
      (*padsy)(i)  = 0;
      (*uncuip)(i) = 0;
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
  mydata.SetActiveSection("Rich Unconnected Channels");
  mydata.SetReadKeyTypes('i','i','i','i');
  line = 0;
  for (i=0; i<fUnconnCh; i++){
   mydata.ReadRawLine(line, &fRCs2,&fPorts2,&fModules2,&fChannels2);
   fAddress1 = fRCs2*10000 + fPorts2*1000 + fModules2*100 + fChannels2;
   (*uncuip)(fAddress1) = 1;
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
  mydata.WriteKeywords(8, "MaxPads", "MaxRows", "MaxCols", "MaxRCs", "MaxPorts", "MaxModules", "MaxChannels", "UnconnCh");
  mydata.SetSeparator('\t');
  mydata.SetWriteKeyTypes('i', 'i', 'i','i', 'i', 'i','i','i');
  mydata.WriteRawLine(fPadsNr,fRows,fCols,fRCs,fPorts,fModules,fChannels,fUnconnCh);

  mydata.WriteSection("Rich Mapping Parameters");
   mydata.WriteKeywords(6, "Col","Row","RC","Port","Module","Channel");
   mydata.SetWriteKeyTypes('i','i','i','i','i','i');
   fAddress1 = (fRCs-1)*10000 + (fPorts-1)*1000 + (fModules-1)*100 + fChannels;

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
 
   mydata.WriteSection("Rich Unconnected Channels");
   mydata.WriteKeywords(4, "RC","Port","Module","Channel");
   mydata.SetWriteKeyTypes('i','i','i','i');
   for(i = 0; i < fAddress1; i++) 
     if ( (*uncuip)(i)) {
	 rc = i/10000;
       tmp = i % 10000;
       port = tmp/1000;
       tmp = tmp % 1000;
       mod = tmp/100;
       ch = tmp % 100;
       mydata.WriteRawLine(rc,port,mod,ch);
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

//----------------------------------------------------------------------------
Int_t HRichMappingPar::getRC(Int_t x,Int_t y) {
    
    //    Int_t rc = ((Int_t)(*xyuip)(x*100+y))/10000;
    return ((Int_t)(*xyuip)(x*100+y))/10000;
}

Int_t HRichMappingPar::getPort(Int_t x,Int_t y) {
    
    //    Int_t port = (((Int_t) (*xyuip)(x*100+y))%10000)/1000;
    return (((Int_t) (*xyuip)(x*100+y))%10000)/1000;
}

Int_t HRichMappingPar::getMod(Int_t x,Int_t y) {
    
    // Int_t mod = ((((Int_t) (*xyuip)(x*100+y))%10000)%1000)/100;
    return ((((Int_t) (*xyuip)(x*100+y))%10000)%1000)/100;
}

Int_t HRichMappingPar::getCh(Int_t x,Int_t y) {
    
    //    Int_t ch = ((((Int_t) (*xyuip)(x*100+y))%10000)%1000)%100;
    return ((((Int_t) (*xyuip)(x*100+y))%10000)%1000)%100;
}

Int_t HRichMappingPar::isReadOut(Int_t x, Int_t y){
 
    return (Int_t)(*xyuiplog)(x*100+y);
}

Int_t HRichMappingPar::isConn(Int_t Address){

    return (Int_t)(*uiplog)(Address);
}

Int_t HRichMappingPar::getCol(Int_t Address){

    return (Int_t)(*padsx)(Address); 
}

Int_t HRichMappingPar::getRow(Int_t Address){

    return (Int_t)(*padsy)(Address); 
}

