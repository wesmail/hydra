#ifndef HMDCFILLTIME1HISTS_H
#define HMDCFILLTIME1HISTS_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "TRandom.h"

class HCategory;
class HIterator;
class HMdcLookupGeom;
class HMdcLookupRaw;
class TH2F;
class TDirectory;


class HMdcFillTime1Hists : public HReconstructor {
protected:
  HCategory* calCat;       //! pointer to the cal data
  Char_t *fNameRootHists;   // file name of root output file

  TH2F *hmdctime_tdc  [6][4][16];
  TH2F *hmdctime_layer[6][4][6];

  HLocation loc;           //! location for new object.
  HIterator* iter;         //! iterator on cal1 data.

  HMdcLookupGeom* lookup;  //! lookup table for mapping
  HMdcLookupRaw* lookupRaw;//! lookup table for mapping

public:
  HMdcFillTime1Hists(void);
  HMdcFillTime1Hists(const Text_t* name,const Text_t* title);
  ~HMdcFillTime1Hists(void);
  void setOutputRoot (const Char_t*);
  Bool_t init(void);
  TDirectory *Mkdir(TDirectory *,const Char_t *, Int_t, Int_t p=1);
  void setParContainers(void);
  Bool_t finalize();
  Int_t execute();
  ClassDef(HMdcFillTime1Hists,0) // fills different hists for cal1 data
};

#endif /* !HMDCFILLTIME1HISTS_H */
