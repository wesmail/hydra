// File: richunpackold.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 97/12/11 14:31:22
//

#ifndef __HRichUnpackerOld_H
#define __HRichUnpackerOld_H

#include "hldunpack.h"
#include "hcategory.h"
#include "haddef.h"
#include "hades.h"
#include "hevent.h"

class HRichUnpackerOld: public HldUnpack {
public:
  HRichUnpackerOld(void);
  HRichUnpackerOld(HRichUnpackerOld &unpacker);
  ~HRichUnpackerOld(void);
  int getSubEvtId(void) const;
  Bool_t init(void);
  int execute(void);

private:
  int getRow(int pad);
  int getColumn(int pad);

public:
  ClassDef(HRichUnpackerOld,1)        // unpack RICH data
};

#endif /* !RICHUNPACK_H */    
