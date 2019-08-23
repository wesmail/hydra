// File: richunpack.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 97/12/11 14:31:22
//

#ifndef __HRichUnpacker_H
#define __HRichUnpacker_H

#include "hldunpack.h"
#include "hcategory.h"
#include "haddef.h"
#include "hades.h"
#include "hevent.h"
#include "richdef.h"

class HRichUnpacker: public HldUnpack {
public:
  HRichUnpacker(void);
  HRichUnpacker(HRichUnpacker &unpacker);
  ~HRichUnpacker(void);
  int getSubEvtId(void) const;
  Bool_t init(void);
  int execute(void);

  int fEventNr;

private:
  int getRow(int pad);
  int getColumn(int pad);

public:
  ClassDef(HRichUnpacker,1)        // unpack RICH data
};

#endif /* !RICHUNPACK_H */    
