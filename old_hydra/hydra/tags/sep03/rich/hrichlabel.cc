#include "hrichlabel.h"

ClassImp(HRichLabel)


Int_t HRichLabel::iLabelNr = 0;

HRichLabel::HRichLabel() {
 iLeftX = 127;
 iRightX = -1;
 iLowerY = 127;
 iUpperY = -1;
 iSignature = 0;
 iFiredPadsNr = 0;
 iLabeledPadsNr = 0;
}


HRichLabel::~HRichLabel() {}


HRichLabel::HRichLabel(const HRichLabel& source) {
 iLeftX = source.iLeftX;
 iRightX = source.iRightX;
 iLowerY = source.iLowerY;
 iUpperY = source.iUpperY; 
 iSignature = source.iSignature;
 iFiredPadsNr = source.iFiredPadsNr;
 iLabeledPadsNr = source.iLabeledPadsNr;
}


HRichLabel& HRichLabel::operator=(const HRichLabel& source) {
 if (this != &source) {
  iLeftX = source.iLeftX;
  iRightX = source.iRightX;
  iLowerY = source.iLowerY;
  iUpperY = source.iUpperY;
  iSignature = source.iSignature;
  iFiredPadsNr = source.iFiredPadsNr;
  iLabeledPadsNr = source.iLabeledPadsNr;
 }

return *this;
}


void HRichLabel::Reset() {
 iSignature = 0;
 iFiredPadsNr = 0;
 iLabeledPadsNr = 0;
 if (iLabelNr) iLabelNr--;
}


