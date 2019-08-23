// File: hrichpadlabel.h
// ****************************************************************************
//
// HRichRaw
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//
// ****************************************************************************


#ifndef HRICHPADLABEL_H
#define HRICHPADLABEL_H

#include "TObject.h"
#include "TArrayI.h"
#include "hrichanalysis.h"

// ****************************************************************************

class HRichPadLabel: public TObject {

// ****************************************************************************

  private:

 Int_t iGraphArray;
 TArrayI iLabelGraphConnection;
 
  public:

 HRichPadLabel();
 HRichPadLabel(Int_t padsx, Int_t padsy);
 virtual ~HRichPadLabel();
 
 HRichPadLabel(const HRichPadLabel& source);
 HRichPadLabel& operator=(const HRichPadLabel& source);

 Int_t LabelPads(HRichAnalysis *showMe, Int_t padDistance);
 Int_t Execute(HRichAnalysis *giveMe);
 void Reset();


 ClassDef(HRichPadLabel,1)
};

//============================================================================


#endif // HRICHPADLABEL_H
