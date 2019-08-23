#ifndef HMDCCONNECTEDWIRES_H
#define HMDCCONNECTEDWIRES_H

#include "hmdclookupgeom.h"

class HParIo;
class HMdcMboReadout;

class HMdcConnectedWires : public HMdcLookupGeom {
protected:
  HMdcLookupGeom* lookup;   //! pointer to lookup container
  HMdcMboReadout* readout;  //! pointer to readout container
public:
  HMdcConnectedWires(const Char_t* name="MdcConnectedWires",
                     const Char_t* title=
                       "Mdc lookup table from cal1 to raw for the connected wires",
                     const Char_t* context="");
  ~HMdcConnectedWires() {}
  Bool_t init(HParIo* input,Int_t* set);
  ClassDef(HMdcConnectedWires,1) // Wires connected to readout controllers
};

#endif /* !HMDCCONNECTEDWIRES_H */
 


