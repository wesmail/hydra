#ifndef HSPLINETRACK_H
#define HSPLINETRACK_H
#include "TObject.h"
#include "hbasetrack.h"
class HSplineTrack : public HBaseTrack 
{
private:
   Double_t tofdist;//Distance from target to META hit
   Double_t betaCalc;
public:
   HSplineTrack();
   ~HSplineTrack();
   void setTofDist(Double_t TOFDIST) {tofdist=TOFDIST;}
   void setBetaCalc(Double_t BETACALC){betaCalc=BETACALC;}
   Double_t getBetaCalc(){return betaCalc;}
   Double_t getTofDist(){return tofdist;}
   ClassDef (HSplineTrack,1);
};
#endif


  
