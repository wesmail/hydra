#ifndef HCATEGORYMAKER_H
#define HCATEGORYMAKER_H

#include "TNamed.h"
#include "haddef.h"
#include "TArrayI.h"

class HCategory;
class HEvent;

class HCategoryMaker : public TNamed {
public:
  HCategoryMaker(const char* name="CategoryMaker", const char* title="CategoryMaker");
  ~HCategoryMaker();

  static HCategory* buildLinearCategory(Text_t* className);
  static HCategory* buildMatrixCategory(Text_t* className,Float_t fillRate);
  static HCategory* buildCategory(Cat_t cat);
  static HCategory* initCategory(HEvent* event=0,Cat_t cat=-1, TString catname="", TString detector="");
  static Int_t copyMdcCal1Cat   (HCategory* targetcat,HCategory* cpCat);
  static Int_t copyMdcHitCat    (HCategory* targetcat,HCategory* cpCat);
  static Int_t copyMdcSegCat    (HCategory* targetcat,HCategory* cpCat);
  static Int_t copyMdcClusInfCat(HCategory* targetcat,HCategory* cpCat);
  static Int_t copyMdcClusFitCat(HCategory* targetcat,HCategory* cpCat);
  static Int_t copyMdcWireFitCat(HCategory* targetcat,HCategory* cpCat);
  static Int_t copyMdcTrkCandCat(HCategory* targetcat,HCategory* cpCat);
  static Int_t copyKickTrackCat (HCategory* targetcat,HCategory* cpCat);

  ClassDef(HCategoryMaker,1) // category makerclass
};

#endif  /* !HCATEGORYMAKER_H */
                                                            
