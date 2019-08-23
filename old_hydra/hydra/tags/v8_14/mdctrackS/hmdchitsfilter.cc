using namespace std;
# include "hdebug.h"
# include "hmdchitsfilter.h"
#include <fstream> 


ClassImp(HMdcHitsFilter)

//*-- Author : Beatriz Fuentes Arenaz
//*-- Date: 24/2/99
//*-- Last modified : 24/2/99
//*-- Copyright : GENP (Univ. Santiago de Compostela)

//_HADES_CLASS_DESCRIPTION 
  //////////////////////////////////////////////////////////////////
  //
  // HMdcHitsFilter is an HFilter to reduce the size of the 
  // segments category after the HMdcHitAuxF::find function is 
  // executed. In this function, some segments must be deleted after
  // they are added to the category of segments. This segments are
  // marked setting the toBeDeleted flag to a non-zero value. This
  // filter deletes this segments from the segments category.
  //
  //////////////////////////////////////////////////////////////////


  Bool_t HMdcHitsFilter :: check(TObject* hit){
  // returns kTRUE if the segment has not be deleted. 
  // otherwise, returns kFALSE

#if DEBUG_LEVEL>2
      gDebuger->enterFunc("HMdcHitsFilter::check");
#endif

      if (((HMdcHitAux*)hit)->isDel()==0) return kTRUE;
      return kFALSE;

#if DEBUG_LEVEL>2
      gDebuger->leaveFunc("HMdcHitsFilter::check");
#endif
}
