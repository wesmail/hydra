//*-- Autor: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/18/2005

#ifndef HRICHIPUREMAKE_H
#define HRICHIPUREMAKE_H

#include "hreconstructor.h"

class HCategory;
class HIterator;
class HRichIPUParThresholds;
class HRichIPUParLocMax;
class HSpectrometer;
class HRuntimeDb;
class HLocMaxFilter;
// ***************************************************************************

class HRichIPURemake : public HReconstructor {

// ***************************************************************************
protected:
  HCategory *pRich;                        //!
  HRichIPUParThresholds *fRichIPUParThr;  //!
  HRichIPUParLocMax *fRichIPUParLmax;     //!
  HIterator * iterRich ;                  //!

private:
    Bool_t initParContainer(HSpectrometer *,HRuntimeDb *);

public:
    HRichIPURemake(void);
    HRichIPURemake(const Text_t *name,const Text_t *title);
    ~HRichIPURemake(void);

    Bool_t init(void);
    Bool_t finalize(void);
    Int_t execute(void);
    void prepareInput(void);

    ClassDef(HRichIPURemake,1)
  };

//============================================================================

#endif  // HRICHIPUREMAKE_H
