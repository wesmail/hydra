#ifndef HShowerGReader_H
#define HShowerGReader_H

#include "hgeantreader.h"
#include "heventheader.h"
#include "hcategory.h"

class shower_tuple {
public:
  Int_t idevt;
  Int_t nhit;
  Float_t xhit[100],yhit[100];
  Float_t thetahit[100],phihit[100];
  Float_t eloshit[100], betahit[100], momhit[100];
  Int_t dethit[100];
};

class HShowerGReader : public HGeantReader {
private:
  HCategory *fHitCat; //GEANT Dir Hit category

  Int_t fEventId; //Event number.

  void fillHit(shower_tuple *shower, int sect);

  shower_tuple *shower;
  TTree *tr;

public:
  HShowerGReader(void);
  ~HShowerGReader(void);
  Bool_t execute(void);
  Bool_t init(void);

  ClassDef(HShowerGReader,0) //GEANT reader for RICH data
};

#endif
