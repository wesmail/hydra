#ifndef HMDC12CLUSTERS_H
#define HMDC12CLUSTERS_H

#include "TObject.h"
#include "TObjArray.h"
#include "hgeomtransform.h"
//#include "TArrayI.h"
//#include "TNtuple.h"
#include "hparset.h"
//#include "hmdcdigitgeompar.h"
//#include "hmdcgeomobj.h"
//#include "hmdcsizescells.h"

class HMdcCluster : public TObject {
protected:
  
//  Char_t numTimes[12][300];
  
  Double_t xmid;     // In coord. system of sector
  Double_t ymid;     //
  Double_t zmid;     //
  Double_t dXmid;    // Error of xmid
  Double_t dYmid;    // Error of ymid
  Double_t dZmid;    // Error of zmid (dZmid iz function from (xmid+dXmid))
  Double_t xLeft;   // Left and ut point for cluster region
  Double_t yUp;   // xLeft,yUp,zUp
  Double_t zUp;   //
  Double_t xRight;   // Right and down point for cluster region
  Double_t yDown;   // xRight,yDown,zDown
  Double_t zDown;   //
  Double_t nBinsInCluster;
  
  Float_t prDb;   //???????!!!!!!!!!!
//  Double_t zmid;

  Short_t max;
//  HGeomTransform system;
  Short_t qualityClusters[2];
  Short_t hits[12][40]; //!!!!!!!!!!!!!!!! [20];
  Short_t numhits[12];
  Short_t numTimes[12][40];  //!!!!!!!!!!!!![20]; //?????????????????
//  Int_t nclusters;
public: 
  HMdcCluster();
  HMdcCluster(Short_t max);
  ~HMdcCluster(){;}
  void print();
  void setHitToCluster(Int_t nl, Short_t ncell, Short_t nTm);

  void setCluster(Short_t numhitI[],  Short_t hitsI[][40], Short_t numTimesI[][40]);
  void setBinsWt(Double_t binswt) {nBinsInCluster=binswt;}
  void setX(Double_t x){xmid=x;}
  void setY(Double_t y){ymid=y;}
  void setZ(Double_t z){zmid=z;}
  void setErrX(Double_t dX){dXmid=dX;}
  void setErrY(Double_t dY){dYmid=dY;}
  void setErrZ(Double_t dZ){dZmid=dZ;}
  void setXright(Double_t x){xRight=x;}
  void setYdown(Double_t y){yDown=y;}
  void setZup(Double_t z){zUp=z;}
  void setXleft(Double_t x){xLeft=x;}
  void setYup(Double_t y){yUp=y;}
  void setZdown(Double_t z){zDown=z;}
  void setPrDb(Float_t pD){prDb=pD;}
  Double_t getBinsWt() {return nBinsInCluster;}  
  Double_t getX(){return xmid;}
  Double_t getY(){return ymid;}
  Double_t getZ(){return zmid;}
  Double_t getErrX(){return dXmid;}
  Double_t getErrY(){return dYmid;}
  Double_t getErrZ(){return dZmid;}
  Double_t getXright(){return xRight;}
  Double_t getYdown(){return yDown;}
  Double_t getZup(){return zUp;}
  Double_t getXleft(){return xLeft;}
  Double_t getYup(){return yUp;}
  Double_t getZdown(){return zDown;}
  Double_t getNBins(){return nBinsInCluster;}
  Float_t getPrDb(){return prDb;}
  Int_t getQuality1(){return qualityClusters[0];}
  Int_t getQuality2(){return qualityClusters[1];}
  Short_t getMaximum(){return max;}

  void calcQuality();
  
  Short_t getNumHits(Int_t n) {return numhits[n];}
  Short_t getHit(Int_t l,Int_t n) {return hits[l][n];}
  Short_t getTimes(Int_t l,Int_t n) {return numTimes[l][n];}
  
  Bool_t Test(HMdcCluster& clst, Double_t Ap, Double_t Bp, Double_t Dp,
              Double_t shX2, Double_t shY2);
  Bool_t IsSortable() const {return kTRUE;}
  Int_t Compare(TObject *obj); 
    
  ClassDef(HMdcCluster,1)
}; 

class HMdc12Clusters : public TObject {
protected:
  TObjArray *clusters;
//  Char_t numTimes[12][300];
  Int_t nClusters;
//  HGeomTransform system;

public: 
  HMdc12Clusters ();
  ~HMdc12Clusters ();
  HMdcCluster& operator[](Int_t i) {
      return *static_cast<HMdcCluster*>((*clusters)[i]);
  }
  Int_t addCluster(Short_t max);
  void clear();
  Int_t deleteCluster(Int_t nClust);
  void Sort(){clusters->Sort();}
  Int_t getNClusters(){return nClusters;}
  void calcxy();
//  void setsys(HGeomTransform &sys){system.setTransform(sys);}
//  const HGeomTransform& getTransform(){return system;}
  void print();
  
  ClassDef(HMdc12Clusters,1)
};

#endif  /*!HMDC12CLUSTERS_H*/
