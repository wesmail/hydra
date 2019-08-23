#ifndef HRICHLABEL_H
#define HRICHLABEL_H

#include "TObject.h"


class HRichLabel: public TObject {

  public:

 Int_t iLeftX;
 Int_t iRightX;
 Int_t iLowerY;
 Int_t iUpperY;

 Int_t iSignature;
 Int_t iFiredPadsNr;
 Int_t iLabeledPadsNr;
 static Int_t iLabelNr;

 HRichLabel();
 virtual ~HRichLabel();

 HRichLabel(const HRichLabel& source);
 HRichLabel& operator=(const HRichLabel& source);

 void Reset();

 ClassDef(HRichLabel,1)
};


#endif // HRICHLABEL_H

