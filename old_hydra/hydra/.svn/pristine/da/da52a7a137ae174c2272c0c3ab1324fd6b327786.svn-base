#ifndef HSPLINETRACK_H
#define HSPLINETRACK_H
#include "TObject.h"
#include "hbasetrack.h"
class HSplineTrack : public HBaseTrack 
{
private:
   Double_t tofdist;//Distance from target to META hit
   Float_t qSpline;
public:
   HSplineTrack();
   virtual  ~HSplineTrack();
   void setTofDist(Double_t TOFDIST) {tofdist=TOFDIST;}
   Double_t getTofDist(){return tofdist;}
   void setQSpline(Float_t QS) {qSpline=QS;}
   Float_t getQSpline() {return qSpline;}
   ClassDef (HSplineTrack,2);
};
#endif


  
