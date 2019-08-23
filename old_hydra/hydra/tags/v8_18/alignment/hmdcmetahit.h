# ifndef HMDCMETAHIT_H
# define HMDCMETAHIT_H

# include "TObject.h"
# include "hmdchit.h"
# include "hgeomvector.h"

class HMdcMetaHit : public TObject {

 protected:    
  HMdcHit fHitA;      //  Hit in Module A
  HMdcHit fHitB;      //  Hit in Module B
  HMdcHit fHitC;      //  Hit in Module C
  HMdcHit fHitD;      //  Hit in Module D
  HGeomVector fLocalHitA;  //  Local position of the Meta Hit
  HGeomVector fLocalHitB;  //  Local position of the Meta Hit
  HGeomVector fNum;     //  X: Detector TOF(0...7) or SHOWER(8)
                       //  Y: Number of MDCs
                       //  Z: Number of META Hits
  //Int_t fDetector;     //! Detector TOF(0...7) or SHOWER(8)
  //Int_t fMDCs;         //!  Number of MDCs
  //Int_t fMetaHits;    //! Number of META Hits
    
 public:

  HMdcMetaHit(void);    
  HMdcMetaHit(Int_t nMDCs);      
  HMdcMetaHit(const HMdcMetaHit &other);
  HMdcMetaHit(Int_t nDetec, HGeomVector* hitMA,
	      Int_t nMDCs, HMdcHit* hitA, HMdcHit* hitB, 
	      HMdcHit* hitC=NULL, HMdcHit* hitD=NULL); 
  HMdcMetaHit(Int_t nDetec, HGeomVector* hitMA, HGeomVector* hitMB,
	      Int_t nMDCs, HMdcHit* hitA, HMdcHit* hitB, 
	      HMdcHit* hitC=NULL, HMdcHit* hitD=NULL); 
  ~HMdcMetaHit(void); 

  void setMdcHitA(HMdcHit* hitA);
  void setMdcHitB(HMdcHit* hitB);
  void setMdcHitC(HMdcHit* hitC);
  void setMdcHitD(HMdcHit* hitD);
  void setLocalHitA(HGeomVector* vec){fLocalHitA=*vec;}
  void setLocalHitB(HGeomVector* vec){fLocalHitB=*vec;}
  void setDetector(Int_t det){fNum.setX(det);}
  void setNumberOfMDCs(Int_t nMDCs){fNum.setY(nMDCs);}
  void setNumberOfMetaHits(Int_t nNMHs){fNum.setZ(nNMHs);}
  void print(void);
 
  HMdcHit* getMdcHitA(){return &fHitA;}
  HMdcHit* getMdcHitB(){return &fHitB;}
  HMdcHit* getMdcHitC(){return &fHitC;}
  HMdcHit* getMdcHitD(){return &fHitD;}
  HGeomVector* getLocalHitA(){return &fLocalHitA;}
  HGeomVector* getLocalHitB(){return &fLocalHitB;}
  Int_t getDetector(){return (Int_t)fNum.getX();}
  Int_t getNumberOfMdcs(){return (Int_t)fNum.getY();}
  Int_t getNumberOfMetaHits(){return (Int_t)fNum.getZ();}

  ClassDef(HMdcMetaHit,1)  // MDC - META hit for alignment
};



# endif /* !HMDCMETAHIT_H */
