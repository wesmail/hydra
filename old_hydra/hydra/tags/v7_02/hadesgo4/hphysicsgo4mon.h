#ifndef HPHYSICSGO4MON_H
#define HPHYSICSGO4MON_H

#include "hreconstructor.h"
#include "TString.h"

class HCategory;
class HIterator;
class HPhysicsHistBooker;

/** Physics online monitoring task.
 * The task gets the pointer to the categories and
 * loops over them calling the fill functions of
 * HPhysicsHistFiller.
*/
class HPhysicsGo4Mon : public HReconstructor {
protected:

  //########################### COMMON VARIABLES ##########################################

  // pointer to categories and iterators
  HCategory* MdcTrkCandCat;
  HCategory* MdcSegCat;
  HCategory* TofHitCat;
  HCategory* ShowerTofHitCat;
  HCategory* KickTrackCat;
  HCategory* SplineTrackCat;
  HCategory* MetaMatchCat;
  HCategory* RichHitCat;
  HIterator* iterMdcTrkCand;
  HIterator* iterMdcSeg;
  HIterator* iterTofHit;
  HIterator* iterShowerTofHit;
  HIterator* iterKickTrack;
  HIterator* iterSplineTrack;
  HIterator* iterMetaMatch;
  HIterator* iterRichHit;

  // pointer to used parameter containers
  static HPhysicsHistBooker* physicsHistbooker;//!< pointer to HPhysicsHistBooker
  /// init the variable of the task
  void   initVariables();
  /// get the pointers to the categories and create the iterators
  Bool_t getCategories();
  /// get the pointers to the needed Containers
  void   setParContainers(void);
public:
  HPhysicsGo4Mon(void);
  HPhysicsGo4Mon(Text_t* name,Text_t* title);
  ~HPhysicsGo4Mon(void);
  /// prints the status of the task to the screen
  void   printStatus();
  /// init the task
  Bool_t init(void);
  /// reinit the task
  Bool_t reinit(void);
  /// execute function called in event loop
  Int_t  execute();
  /// finalize function of the task
  Bool_t  finalize();
  ClassDef(HPhysicsGo4Mon,0) // fills different hists
};

#endif /* !HPHYSICSGO4MON_H */
