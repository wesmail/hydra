#ifndef HMETAMATCH_H
#define HMETAMATCH_H

#include "hreconstructor.h"
#include "TString.h"
#include "TObject.h"
{
 private:
  Short_t seg1Ind; //index of first segment
  Short_t seg2Ind;//index of second segment
  Short_t metaInd;//index from META
  Int_t system;//Shower or tof 
 public:
  HMetaMatch();
  ~HMetaMatch();
  void setSeg1Ind(Short_t ind1){seg1Ind=ind1;}
  void setSeg1Ind(Short_t ind1){seg1Ind=ind1;}
}
