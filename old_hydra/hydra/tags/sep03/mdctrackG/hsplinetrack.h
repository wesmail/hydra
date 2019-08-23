#ifndef HSPLINETRACK_H
#define HSPLINETRACK_H
#include "TObject.h"
#include "hbasetrack.h"
class HSplineTrack : public HBaseTrack 
{
 private:
  //Double_t p;//momentum from spline method;
  Double_t dist[4]; //distances to corresponding chumbers in mm;
  Short_t IndTrkCand;
 // Int_t charge;//sign of charge;
//  Short_t seg1Ind;//the same from segsmatch;
 // Short_t seg2Ind;//the same from segsmatch;
 public:
  HSplineTrack();
  ~HSplineTrack();
  void setTrkCandInd(Short_t INDTRKCAND){IndTrkCand=INDTRKCAND;}
  Short_t getTrkCandInd() {return IndTrkCand;}
 // void SetMomentum(Double_t momentum){p=momentum;}
 // void setCharge(Int_t sighn){charge=sighn;}
 // void setSeg1Ind(Short_t ind1){seg1Ind=ind1;}
 // void setSeg2Ind(Short_t ind2){seg2Ind=ind2;}
 //
 // Double_t getMomentum() {return p;}
 // Float_t getCharge() {return charge;}
 // Short_t getSeg1Ind() {return seg1Ind;}
 /// Short_t getSeg2Ind() {return seg2Ind;}
  ClassDef (HSplineTrack,1);
};
#endif
  

  
