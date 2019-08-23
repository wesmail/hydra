#ifndef HDETEVTDISPLAY_H
#define HDETEVTDISPLAY_H

#include <TNamed.h>

class HGeomTransform;
class TNode;
 
class HDetEvtDisplay : public TNamed {
public:
  HDetEvtDisplay(void) {}   
  virtual ~HDetEvtDisplay(void) {}
  virtual Bool_t init(void)=0;
  virtual Bool_t reinit(void)=0;
  virtual Int_t execute()=0;     
  virtual void clear(void)=0;
  virtual Bool_t createGeom(TNode*,const HGeomTransform*,const HGeomTransform*,
                            Int_t nSec=-1)=0;  
  virtual void cleanup(void)=0;
  virtual Bool_t needsChange(void)=0;   
  ClassDef(HDetEvtDisplay,0) // base class for the detector event display
};

#endif  /* !HDETEVTDISPLAY_H */
