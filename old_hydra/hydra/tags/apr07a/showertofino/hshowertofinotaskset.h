#ifndef HShowerTofinoTaskSet_H
#define HShowerTofinoTaskSet_H

#include "htaskset.h"
#include "TString.h"

class HShowerTofinoTaskSet : public HTaskSet {
private:
  	
static  Int_t ianalysisSimOrReal; // counter for arguments of sim/real 	
static  Int_t ianalysisLevel;     // counter for arguments of analysis level 	
static  Int_t imagnet;            // counter for arguments of magnet 	

static  Int_t analysisSimOrReal; // analysis sim/real	
static  Int_t analysisLevel;	 // analysis level leprecogn=1 
static  Bool_t isCoilOff;        // kTRUE=magnet off, kFALSE=magnet on 
static  Bool_t isSimulation;     // kTRUE=simulation , kFALSE=real 
  
		

public:
  HShowerTofinoTaskSet(void);
  HShowerTofinoTaskSet(Text_t name[],Text_t title[]);
  ~HShowerTofinoTaskSet(void);

 // functions to set analysis
   void setAnalysisSimOrReal(Option_t*);
   void setAnalysisLevel(Option_t*);
   void passArguments(TString s1);
   void printOptions(void);	   

 // function to be used for derived task sets (HShowerTofinoTaskSet, etc.)
  HTask *make(const char *select="", Option_t *option="");

  ClassDef(HShowerTofinoTaskSet,1) // Set of tasks
};

#endif /* !HShowerTofinoTaskSet_H */
