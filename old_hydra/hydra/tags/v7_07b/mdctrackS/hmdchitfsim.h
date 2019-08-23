# ifndef HMDCHITFSIM_H
# define HMDCHITFSIM_H


# include "hmdchitf.h"

class HMdcHitFSim : public HMdcHitF{

 public:
  HMdcHitFSim(void) : HMdcHitF(){;}
  HMdcHitFSim(Text_t* name, Text_t* title) : HMdcHitF(name,title){;}


 private:
  virtual Text_t *getCalHitClassName(void) { return "HMdcCal3Sim"; }
  void buildList(void);

ClassDef(HMdcHitFSim,1)

};

# endif          /* HMDCHITFSIM_H */


