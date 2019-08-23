#ifndef HMDCGETCONTAINERS_H
#define HMDCGETCONTAINERS_H

#include "hades.h"
#include "hmdctrackddef.h"
#include "hevent.h"
#include "hcategory.h"
#include "hmdccal1sim.h"
#include "hmdcclussim.h"
#include "hmdchitsim.h"
#include "hmdcsegsim.h"
#include "TString.h"

class HRuntimeDb;
class HSpectrometer;
class HMdcLayerGeomPar;
class HMdcGeomPar;
class HMdcDetector;
class HSpecGeomPar;
class HMdcGeomStruct;
class HMdcRawStruct;
class HMdcLookupGeom;
class HKickPlane2;
class HGeomCompositeVolume;
class HModGeomPar;
class HMdcLayerGeomParLay;
class HGeomTransform;
class HParSet;

class HMdcGetContainers : public TObject {
  protected:
    static HMdcGetContainers* fMdcGetContainers;
    static HCategory* pMdcCal1Cat;
    static HCategory* pGeantKineCat;
    static HCategory* pGeantMdcCat;
    HRuntimeDb* rtdb;
    Bool_t isRtdb;
    HSpectrometer* spectr;
    HMdcLayerGeomPar* fMdcLayerGeomPar;
    Bool_t isMdcLayerGeomPar;
    HMdcGeomPar* fMdcGeomPar;
    Bool_t isMdcGeomPar;
    HMdcDetector* fMdcDetector;
    Bool_t isMdcDetector;
    HSpecGeomPar* fSpecGeomPar;
    Bool_t isSpecGeomPar;
    HMdcGeomStruct* fMdcGeomStruct;
    Bool_t isMdcGeomStruct;
    HMdcRawStruct* fMdcRawStruct;
    Bool_t isMdcRawStruct;
    HMdcLookupGeom* fMdcLookupGeom;
    Bool_t isMdcLookupGeom;
    HKickPlane2* fKickPlane;
    Bool_t isKickPlane;
    
    Bool_t initCont(HParSet *par);
    HMdcGetContainers();
    ~HMdcGetContainers() {;}
  public:
    static Bool_t isInited(HParSet *par);
    static HMdcGetContainers* getObject();
    static void deleteCont();
    
    HMdcLayerGeomPar* getMdcLayerGeomPar(Bool_t init=kFALSE);
    HMdcGeomPar* getMdcGeomPar(Bool_t init=kFALSE);
    HMdcDetector* getMdcDetector();
    HSpecGeomPar* getSpecGeomPar(Bool_t init=kFALSE);
    HMdcGeomStruct* getMdcGeomStruct(Bool_t init=kFALSE);
    HMdcRawStruct* getMdcRawStruct(Bool_t init=kFALSE);
    HMdcLookupGeom* getMdcLookupGeom(Bool_t init=kFALSE);
    HKickPlane2* getKickPlane(Bool_t init=kFALSE);
    
    HModGeomPar* getModGeomPar(Int_t sec, Int_t mod, Bool_t init=kFALSE);
    HGeomCompositeVolume* getGeomCompositeVolume(Int_t mod, Bool_t init=kFALSE);
    HMdcLayerGeomParLay* getMdcLayerGeomParLay(Int_t sec,Int_t mod,Int_t lay,
                                               Bool_t init=kFALSE);
    // Transformations:
    const HGeomTransform& getLabTransSec(Int_t sec, Bool_t init=kFALSE);
    Bool_t getLabTransSec(HGeomTransform& trans, Int_t sec, Bool_t init=kFALSE);
    const HGeomTransform& getLabTransMod(Int_t sec,Int_t mod,Bool_t init=kFALSE);
    Bool_t getLabTransMod(HGeomTransform& trans, Int_t sec, Int_t mod, 
        Bool_t init=kFALSE);
    Bool_t getLabTransLayer(HGeomTransform& trans, Int_t sec,Int_t mod, 
        Int_t lay, Bool_t init=kFALSE);
    Bool_t getSecTransMod(HGeomTransform& trans, Int_t sec, Int_t mod, 
        Bool_t init=kFALSE);
    Bool_t getSecTransLayer(HGeomTransform& trans, Int_t sec,Int_t mod, 
        Int_t lay, Bool_t init=kFALSE);
    const HGeomTransform& getModTransLayer(Int_t mod, Int_t lay, 
        Bool_t init=kFALSE);
    Bool_t getModTransLayer(HGeomTransform& trans, Int_t mod, Int_t lay,
        Bool_t init=kFALSE);
    Bool_t getModTransLayerZ0(HGeomTransform& trans, Int_t mod, Int_t lay,
        Bool_t init=kFALSE);
    Bool_t getLabTransLayerZ0(HGeomTransform& trans, Int_t sec,Int_t mod, 
        Int_t lay, Bool_t init=kFALSE);
    Bool_t getSecTransLayerZ0(HGeomTransform& trans, Int_t sec,Int_t mod, 
        Int_t lay, Bool_t init=kFALSE);
    Bool_t getSecTransCathLayerZ0(HGeomTransform& trans1,HGeomTransform& trans2,
        Int_t sec,Int_t mod, Int_t lay, Bool_t init=kFALSE);
    
    static Bool_t isGeant(void);
      
    // Categories:
    static void setCatMdcCal1(HCategory* cat=0);
    static void setCatGeantKine(HCategory* cat=0);
    static void setCatGeantMdc (HCategory* cat=0);
    inline static HCategory* getCatMdcCal1(void);
    inline static HCategory* getCatGeantKine(void);
    inline static HCategory* getCatGeantMdc(void);

    static HCategory* getCatMdcClus(Bool_t create=kFALSE);
    static HCategory* getCatMdcClusFit(Bool_t create=kFALSE);
    static HCategory* getCatMdcWireFit(Bool_t create=kFALSE);
    static HCategory* getCatMdcClusInf(Bool_t create=kFALSE);
    static HCategory* getCatMdcTrkCand(Bool_t create=kFALSE);
    
    static void getFileName(TString& fileName);
    static void getFileNameWInd(TString& fileName);
    static void getRunId(TString& runId);
    static void getEventDate(TString& eventDate);
    static void getEventTime(TString& eventTime);
                                     
    ClassDef(HMdcGetContainers,0)
};


inline HCategory* HMdcGetContainers::getCatMdcCal1(void)   {
  if(pMdcCal1Cat==0) setCatMdcCal1();
  return pMdcCal1Cat;
}
  
inline HCategory* HMdcGetContainers::getCatGeantKine(void) {
  if(pGeantKineCat==0) setCatGeantKine();
  return pGeantKineCat;
}

inline HCategory* HMdcGetContainers::getCatGeantMdc (void) {
  if(pGeantMdcCat==0) setCatGeantMdc();
  return pGeantMdcCat;
}

#endif  /*!HMDCGETCONTAINERS_H*/
