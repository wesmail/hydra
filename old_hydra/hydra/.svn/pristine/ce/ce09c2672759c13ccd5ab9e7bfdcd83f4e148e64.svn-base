//*-- Author : Tassilo Christ 
//First implementation: 19.07.2002

#include <iostream.h>
#include "hades.h"
#include "hparhadasciifileio.h"
#include "hspectrometer.h"
#include "hrichparset.h"
#include "hrichdetector.h"
#include "hrichthresholdpar.h"

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
//  HRichThresholdPar
//
//  Parameter container for threshold data.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

ClassImp(HRichThresholdPar)

//----------------------------------------------------------------------------
HRichThresholdPar::HRichThresholdPar(const char* name,const char* title,
                         const char* context)
                         : HRichParSet(name,title,context) {
}

HRichThresholdPar::~HRichThresholdPar()
{}

Bool_t HRichThresholdPar::defaultInit()
{
  fOffset = -1;
  fMultiply_mean = -1;
  fMultiply_sigma = -1;
  return kTRUE;
}

void HRichThresholdPar::printThresholdPar()
{
  cout << "**********************************************" << endl;
  cout << "Threshold parameters for the analysis!"<<endl;
  cout << "offset: " << fOffset << endl;
  cout << "multiply_mean: " << fMultiply_mean << endl;
  cout << "multiply_sigma: " << fMultiply_sigma << endl;
  cout << "**********************************************" << endl;
}

Bool_t HRichThresholdPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
  using namespace HadAsciiAuxiliaryFunctions;
  if (!pHadAsciiFile) return kFALSE;
  Bool_t status = kTRUE;


  try {
    
    HAsciiRaw &mydata = *pHadAsciiFile->GetRawAscii();
    
    mydata.SetSeparator('\t');
    mydata.WriteSection("Rich Threshold Parameters Setup");
    mydata.WriteKeywords(3, "Offset", "Multiply_mean", "Multiply_sigma");
    mydata.SetWriteKeyTypes('f', 'f', 'f');
    mydata.WriteRawLine(fOffset, fMultiply_mean, fMultiply_sigma);
  }// eof try block

  catch (Bool_t ret) {
    status = ret;
  }
  catch (...) {
    status = kFALSE;
    ErrorMsg(2,"HRichThresholdPar::writeAscii",1,"Unidentified exception catched.");
  }
  
  if (status)
    ErrorMsg(0,"HRichThresholdPar::writeAscii",
	     1,"Container \'RichThresholdPar\' has been written to ASCII file.");
  
  return status;
}

Bool_t HRichThresholdPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
  using namespace HadAsciiAuxiliaryFunctions;
  if (!pHadAsciiFile) return kFALSE;
  Bool_t status = kTRUE;
 
 try {

  HAsciiRaw &mydata = *pHadAsciiFile->GetRawAscii();

  mydata.SetActiveSection("Rich Threshold Parameters Setup");

  mydata.SetReadKeyTypes('f', 'f', 'f');

  mydata.ReadRawLine(0, &fOffset, &fMultiply_mean, &fMultiply_sigma);
 
 } // eof try block

 catch (Bool_t ret) {
   status = ret;
 }
 catch (...) {
   status = kFALSE;
   ErrorMsg(2,"HRichThresholdPar::initAscii",1,"Unidentified exception catched.");
 }
 
 if (status)
   ErrorMsg(0,"HRichThresholdPar::initAscii",
   1,"Container \'RichThresholdPar\' has been read from ASCII file.");
  return status; 
}

void HRichThresholdPar::setOffset(Float_t pOffset){fOffset=pOffset;}
void HRichThresholdPar::setMultiply_mean(Float_t pMultiply_mean){fMultiply_mean=pMultiply_mean;}
void HRichThresholdPar::setMultiply_sigma(Float_t pMultiply_sigma){fMultiply_sigma=pMultiply_sigma;}
