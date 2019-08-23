//*-- AUTHOR Bjoern Spruck
//*-- created : 24.03.06

#ifndef HHODOCALIBRATER_H
#define HHODOCALIBRATER_H

#include "hreconstructor.h"
#include "hlocation.h"

class HCategory;
class HIterator;
class HHodoCalPar;

class HHodoCalibrater : public HReconstructor {
protected:
  HCategory *rawCat;          //! pointer to the raw data
  HCategory *calCat;          //! pointer to the cal data
  HIterator *iter;            //! iterator on raw data.
  HLocation loc;              //! location for new cal object
  HHodoCalPar* calpar;        //! calibration parameters

public:
  HHodoCalibrater(void);
  HHodoCalibrater(Text_t* name,Text_t* title);
  ~HHodoCalibrater(void);
  Bool_t init(void);
  Bool_t finalize(void) { return kTRUE; }
  Int_t execute(void);
public:
  ClassDef(HHodoCalibrater,0) // Calibrater raw->cal for HODO data
};

#endif /* !HHODOCALIBRATER_H */
















