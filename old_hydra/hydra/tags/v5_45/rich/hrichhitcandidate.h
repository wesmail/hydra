#ifndef HRICHHITCANDIDATE_H
#define HRICHHITCANDIDATE_H

#include "TObject.h"
#include "TArrayI.h"

class HRichHitCandidate: public TObject {

private:

 Int_t fX;
 Int_t fY;
 Int_t fA; 
 Int_t fPadLabel;
 Int_t fMaxLabel;

public:

 HRichHitCandidate();
 HRichHitCandidate(const Int_t x, const Int_t y, const Int_t a, const Int_t p, const Int_t l);
 virtual ~HRichHitCandidate();

 void Reset();

 Int_t getX() { return fX; }
 Int_t getY() { return fY; }
 Int_t getA() { return fA; }
 Int_t getPadLabel() { return fPadLabel; }
 Int_t getMaxLabel() { return fMaxLabel; }

 void setX(const Int_t x) { fX = x; }
 void setY(const Int_t y) { fY = y; }
 void setA(const Int_t a) { fA = a; }
 void setPadLabel(const Int_t p) { fPadLabel = p; }    
 void setMaxLabel(const Int_t l) { fMaxLabel = l; }    

 Bool_t  IsSortable() const { return kTRUE; }
 Int_t   Compare(TObject *obj);

 ClassDef(HRichHitCandidate,1)
};


inline Int_t  HRichHitCandidate::Compare(TObject *obj) {
   if (fA==((HRichHitCandidate*)obj)->getA()) return 0;
   return (fA > ((HRichHitCandidate*)obj)->getA()) ? 1 : -1;
}

#endif // HRICHHITCANDIDATE_H
