#ifndef HRichGReader_H
#define HRichGReader_H

#include "hgeantreader.h"
#include "heventheader.h"
#include "hcategory.h"

class rich_tuple {
public:
  Int_t idevt;
  Int_t ncer;
  Float_t xcer[10000],ycer[10000],ecer[10000],trklength[10000];
  Int_t mediumnum[10000],statflag[10000],partrkcer[10000],parentcer[10000];
  Int_t procescer[10000];
  Int_t nhit;
  Float_t xhit[10000],yhit[10000],zhit[10000];
  Int_t parthit[10000];
  Float_t trklenhit[10000],momhit[10000],thetahit[10000],phihit[10000];
  Float_t eloshit[10000],tlenhit[10000];
  Int_t mirhit[10000],trkhit[10000],parenthit[10000],proceshit[10000];
};

class HRichGReader : public HGeantReader {
private:
  HCategory *fDirHitCat; //GEANT Dir Hit category
  HCategory *fCherHitCat; //GEANT Cher Hit category

  Int_t fEventId; //Event number.

  void fillDirHit(rich_tuple *rich);
  void fillCherHit(rich_tuple *rich);
  
  rich_tuple *rich;
  TTree *tr;

public:
  HRichGReader(void);
  ~HRichGReader(void);
  Bool_t execute(void);
  Bool_t init(void);
  ClassDef(HRichGReader,0) //GEANT reader for RICH data
};

#endif
