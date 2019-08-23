# ifndef HMDCHITAUX_H
# define HMDCHITAUX_H

# include <fstream.h>
# include "hsymmat.h"
# include "hrecobject.h"

class HMdcCal3;

class HMdcHitAux : public HRecObject{
protected:                             
  Float_t myPoint[3];             // three coordinates of the point
  Float_t mySlope[2];             // two slopes of the segment: x-z, y-z
  Float_t myChi;                  // chi-square of the fit
  Int_t myNumberOfHits;           // number of hits in the segment      
  HMdcCal3* cal[6];             // pointers to the calibrated hits that 
                                 // belong to the segment
  HSymMat5 cov;                  // covariance matrix of parameters
  Int_t toBeDeleted;
  Int_t isUsed;

public:
  HMdcHitAux(void);
  HMdcHitAux(HMdcHitAux &copy);

  ~HMdcHitAux(void);
  void clear(void);
  void addCal(HMdcCal3* data);
  void addCal(HMdcCal3* data, Int_t pos);
  void printCal();
  void printCal(ostream &file);
  HMdcCal3* getCal(Int_t planeNumber){return cal[planeNumber];}
  void print(ostream &file);
  void print(void);
  void markHits(void);
  void unMarkHits(void);
  
  Float_t getPointX(void){return myPoint[0];}
  Float_t getPointY(void){return myPoint[1];}
  Float_t getPointZ(void){return myPoint[2];}
  Float_t getSlopeX(void){return mySlope[0];}
  Float_t getSlopeY(void){return mySlope[1];}
  Float_t getChi(void) {return myChi;}
  Float_t getXError(void){return cov.getErr(0);}
  Float_t getYError(void){return cov.getErr(1);}
  Float_t getErrorSlope1(void) {return cov.getErr(2);}
  Float_t getErrorSlope2(void) {return cov.getErr(3);}
  Float_t getCovElement(Int_t i, Int_t j){ return cov.getElement(i,j);}
  Int_t isDel(void){return toBeDeleted;}


  Int_t getNumHits(void){return myNumberOfHits;}
  void operator=(HMdcHitAux *other);
  void operator=(HMdcHitAux &other);

  void setPointX(Float_t x){myPoint[0]=x;}
  void setPointY(Float_t y){myPoint[1]=y;}
  void setPointZ(Float_t z){myPoint[2]=z;}
  void setPoint(Float_t x, Float_t y, Float_t z);
  void setSlopeX(Float_t xSlope){mySlope[0]=xSlope;}
  void setSlopeY(Float_t ySlope){mySlope[1]=ySlope;}
  void setSlope(Float_t xSlope, Float_t ySlope);
  void setErrorX(Float_t xError) { cov.setErr(0,xError);}
  void setErrorY(Float_t yError) { cov.setErr(1,yError);}
  void setError(Float_t xError, Float_t yError);
  void setErrorSlopeX(Float_t error){ cov.setErr(2,error);}
  void setErrorSlopeY(Float_t error){ cov.setErr(3,error);}
  void setErrorSlope(Float_t xerror, Float_t yerror);
  void setCovElement(Int_t i, Int_t j, Float_t ele) {cov.setElement(i,j,ele);}
  void setChi(Float_t chiSq){myChi=chiSq;}
  void setHits(Int_t nHits){myNumberOfHits=nHits;}
  void setDel(void){toBeDeleted=1;}
  void notDel(void){toBeDeleted=0;}
  void used(void){isUsed=1;}
  void notUsed(void){isUsed=0;}
  Int_t getIsUsed(void){return isUsed;}

  ClassDef(HMdcHitAux,1)
};


# endif /* !HMDCHITAUX_H */






