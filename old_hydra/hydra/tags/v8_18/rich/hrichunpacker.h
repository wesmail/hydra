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
  Int_t getSubEvtId(void) const;
  Bool_t init(void);
  Int_t execute(void);

  Int_t fEventNr;

private:
  Int_t getRow(Int_t pad);
  Int_t getColumn(Int_t pad);

public:
  ClassDef(HRichUnpacker,1)        // unpack RICH data
};

#endif /* !RICHUNPACK_H */    
