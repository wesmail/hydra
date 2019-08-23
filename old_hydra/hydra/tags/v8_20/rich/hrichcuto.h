// $Id: hrichcuto.h,v 1.1 2004-10-15 08:18:57 eberl Exp $
// Last update by Thomas Eberl: 03/01/15 16:56:19
// analogous to Dan's class in phyana

#ifndef HRICHCUTO_H
#define HRICHCUTO_H

class HRichCutO : public TNamed {

public:
  HRichCutO();
  HRichCutO(const Text_t *name, const Text_t *title);
  ~HRichCutO() {}

protected:
  Int_t fNCheck;   // number of checks
  Int_t fNGood;    // number of checks which pass cuts

ClassDef(HRichCutO,1) // Base class for cuts

};

#endif
