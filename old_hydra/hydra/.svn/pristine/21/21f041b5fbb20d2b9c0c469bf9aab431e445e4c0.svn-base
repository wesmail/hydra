// File: showerunpack.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 98/01/20 21:25:08
//

#ifndef HShowerUnpackerCalib_H
#define HShowerUnpackerCalib_H

#include "hldsubevt.h"
#include "hldunpack.h"
#include "hshowerunpacker.h"

class HShowerUnpackerCalib: public HShowerUnpacker{
public:
  HShowerUnpackerCalib();

  int getSubEvtId() const { return 301; }
  virtual Byte_t eventType(){return kCalibrationEvent;}

  ClassDef(HShowerUnpackerCalib,1)		// unpack Shower data
};

#endif /* !HShowerUnpackerCalib_H */
