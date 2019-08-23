#ifndef HRICHTASKSET_H
#define HRICHTASKSET_H

#include "htaskset.h"
#include "TString.h"

class HRichTaskSet : public HTaskSet
{

 public:
   HRichTaskSet(void);
   HRichTaskSet(Text_t name[], Text_t title[]);
   ~HRichTaskSet(void);

   // function to be used for derived task sets (HRichTaskSet, etc.)
   static HTask *make(const Char_t* select = "", Option_t* option = "");
   static void   parseArguments(TString s1);

 private:
   static Bool_t iSkipEvent;  // Switch to control skipping event w/o found ring in HRichAnalysis task.
                              // Can be controlled via option arguments in the make function.
   static Bool_t inoiseOn;    // Switch to control the noise simulation in HRichDigitizer task.
                              // Can be controlled via option arguments in the make function.

   ClassDef(HRichTaskSet, 1) // Set of tasks
};

#endif /* !HRICHTASKSET_H */
