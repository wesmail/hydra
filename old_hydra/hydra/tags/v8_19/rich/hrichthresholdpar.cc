//*-- Author : Tassilo Christ 
//First implementation: 19.07.2002
using namespace std;
#include <iostream> 
#include <iomanip>
#include "hades.h"
#include "hparhadasciifileio.h"
#include "hspectrometer.h"
#include "hrichparset.h"
#include "hrichdetector.h"
#include "hrichthresholdpar.h"

// ***************************************************************************
//_HADES_CLASS_DESCRIPTION 
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
HRichThresholdPar::HRichThresholdPar(const Char_t* name,const Char_t* title,
                         const Char_t* context)
                         : HRichParSet(name,title,context) {
  Int_t i;
  for(i=0;i<6;i++){
    fActiveSectors[i]=0;
  }			   
}

HRichThresholdPar::~HRichThresholdPar()
{}

Bool_t HRichThresholdPar::defaultInit()
{
  HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
  if (!pRichDet) return kFALSE;

  fNSectors = pRichDet->getSectors();
  
  for (Int_t k=0;k<6;k++){
    if (pRichDet->getModule(k,0)){
      fActiveSectors[k]=1;
    }
    fCalParVers[k] = -1;
    fShift[k]=fMultiply_offset[k]=fMultiply_sigma[k] = -1.0;
  }
  printf("Rich threshold par default initialization finished\n");
  return kTRUE;
}

void HRichThresholdPar::printThresholdPar()
{
  for (Int_t i = 0;i<6; i++){
    cout << "**********************************************" << endl;
    cout << "Threshold parameters for the analysis in sector "<< i <<"!"<<endl;
    cout << "cal par version: " << fCalParVers[i] << endl;
    cout << "shift: " << fShift[i] << endl;
    cout << "multiply_offset: " << fMultiply_offset[i] << endl;
    cout << "multiply_sigma: " << fMultiply_sigma[i] << endl;
    cout << "**********************************************" << endl;
  }
}

Bool_t HRichThresholdPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
  using namespace HadAsciiAuxiliaryFunctions;
  if (!pHadAsciiFile) return kFALSE;
  Bool_t status = kTRUE;
  Char_t sectname[60];

  try {
    
    HAsciiRaw &mydata = *pHadAsciiFile->GetRawAscii();
    
    mydata.SetSeparator('\t');
    mydata.WriteSection("Rich Threshold Parameters Setup");

    mydata.WriteKeywords(1, "nSectors");
    mydata.SetWriteKeyTypes('i');
    mydata.WriteRawLine(fNSectors);

    for(Int_t sect = 0; sect < fNSectors; sect++) {
      sprintf(sectname, "Rich Threshold Parameters: Sector %d", sect);
      mydata.WriteSection(sectname);
      mydata.WriteKeywords(4, "CalParVersion", "Shift", "Multiply_offset", "Multiply_sigma");
      mydata.SetWriteKeyTypes('i', 'f', 'f', 'f');
      mydata.WriteRawLine(fCalParVers[sect], fShift[sect], fMultiply_offset[sect], fMultiply_sigma[sect]);
      
    }  
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
  Char_t sectname[60];
 try {

  HAsciiRaw &mydata = *pHadAsciiFile->GetRawAscii();
  cout << "got raw ascii" << endl;
  mydata.SetActiveSection("Rich Threshold Parameters Setup");
  cout << "Active Section OK" << endl;
  
  mydata.SetReadKeyTypes('i');
  mydata.ReadRawLine(0,&fNSectors);
  cout << "number of detected sectors: " << fNSectors;
  for(Int_t sect = 0; sect < fNSectors; sect++) {
    sprintf(sectname, "Rich Threshold Parameters: Sector %d", sect);
    mydata.SetActiveSection(sectname);
    mydata.SetReadKeyTypes('i', 'f', 'f', 'f');
    mydata.ReadRawLine(0,&fCalParVers[sect], &fShift[sect], &fMultiply_offset[sect], &fMultiply_sigma[sect]);   
  }  
 
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

void HRichThresholdPar::setShift(Int_t pSector,Float_t pShift){fShift[pSector]=pShift;}
void HRichThresholdPar::setMultiply_offset(Int_t pSector, Float_t pMultiply_offset){fMultiply_offset[pSector]=pMultiply_offset;}
void HRichThresholdPar::setMultiply_sigma(Int_t pSector, Float_t pMultiply_sigma){fMultiply_sigma[pSector]=pMultiply_sigma;}
void HRichThresholdPar::setCalParVers(Int_t pSector, Int_t pCalParVers){fCalParVers[pSector]=pCalParVers;}


