#ifndef HRICHLABEL_H
#define HRICHLABEL_H

#include "TObject.h"


class HRichLabel: public TObject {

  public:

 char iLeftX;
 char iRightX;
 char iLowerY;
 char iUpperY;

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

