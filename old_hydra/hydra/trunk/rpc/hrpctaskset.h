#ifndef HRPCTASKSET_H
#define HRPCTASKSET_H

#include "htaskset.h"

class HRpcTaskSet : public HTaskSet {

public:
  HRpcTaskSet(void); // : HTaskSet() {}   //HRpcTaskSet(void);
  HRpcTaskSet(const Text_t name[],const Text_t title[]); // : HTaskSet(name,title) {} //HRpcTaskSet(Text_t name[],Text_t title[]);
  ~HRpcTaskSet(void); // {} //~HRpcTaskSet(void);

  
 // function to be used for derived task sets (HRpcTaskSet, etc.)
  HTask *make(const Char_t* select="",const Option_t* option="");

  ClassDef(HRpcTaskSet,1) // Set of tasks
};

#endif /* !HRPCTASKSET_H */
