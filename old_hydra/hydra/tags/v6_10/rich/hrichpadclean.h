// File: hrichpadclean.h
// ****************************************************************************
//
// HRichPadClean
//
// Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// Modified: 1999/12/04 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//
// ****************************************************************************


#ifndef HRICHPADCLEAN_H
#define HRICHPADCLEAN_H

#include "TObject.h"
#include "TArrayI.h"
#include "hrichanalysis.h"


// ****************************************************************************

class HRichPadClean: public TObject {

// ****************************************************************************

  private:

 Int_t iCount;
 TArrayI iTempCluster;
 void DeletePulse(HRichAnalysis *showYou, Int_t border, Int_t col, Int_t row);
 
  public:

 HRichPadClean();
 virtual ~HRichPadClean();
 
 HRichPadClean(const HRichPadClean& source);
 HRichPadClean& operator=(const HRichPadClean& source);

 Int_t CleanAlonePad(HRichAnalysis *showMe, Int_t border = 3, Int_t lowerThr = 1);
 Int_t CleanHighPulse(HRichAnalysis *showMe, Int_t border = 2, Int_t upperThr = 200);

 Int_t Execute(HRichAnalysis *giveMe);


 ClassDef(HRichPadClean,1)
};

//============================================================================


#endif // HRICHPADCLEAN_H












