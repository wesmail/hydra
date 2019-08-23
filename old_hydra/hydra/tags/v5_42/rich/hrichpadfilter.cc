# include "hdebug.h"
# include "hrichpadfilter.h"
#include <fstream.h>

ClassImp(HRichPadFilter)

//*-- Author : Laura Fabbietti
//*-- Date: 01/02/01
//*-- Last modified : 01/02/01


    ///////////////////////////////////////////////////
    // 
    // HRichPadFilter is a HFilter to reduce the size
    // of the HRichCalSim container. After the cut-off
    // threshold for each pad is applied all the pad
    // that are under threshold are deleted.
    //
    //////////////////////////////////////////////////

 Bool_t HRichPadFilter :: check(TObject* hit){
  // returns kTRUE if the pad has not be deleted. 
  // otherwise, returns kFALSE

      if (((HRichCalSim*)hit)->getCharge()!=0) return kTRUE;
      return kFALSE;


}
