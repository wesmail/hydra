#ifndef HMDCGETCONTAINERS_H
#define HMDCGETCONTAINERS_H

#include "hades.h"
#include "hmdctrackddef.h"
#include "hevent.h"
#include "hcategory.h"
#include "hmdccal1sim.h"

class HRuntimeDb;
class HSpectrometer;
class HMdcLayerGeomPar;
class HMdcGeomPar;
class HMdcDetector;
class HSpecGeomPar;
class HMdcGeomStruct;
class HMdcRawStruct;
class HMdcLookupGeom;
class HKickPlane;
class HGeomCompositeVolume;
class HModGeomPar;
class HMdcLayerGeomParLay;
class HGeomTransform;
class HParSet;

class HMdcGetContainers : public TObject {
  protected:
    static HMdcGetContainers* fMdcGetContainers;
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
    HKickPlane* fKickPlane;
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
    HKickPlane* getKickPlane(Bool_t init=kFALSE);
    
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
  
    
    static Bool_t HMdcGetContainers::isGeant() {
      return (getCatMdcCal1()->getClass()!=HMdcCal1Sim::Class())?kFALSE:kTRUE;}
      
    // Categories:
    static HCategory* getCatMdcCal1(){
      return gHades->getCurrentEvent()->getCategory(catMdcCal1);}
    static HCategory* getCatMdcClus(Bool_t create=kFALSE);
    static HCategory* getCatMdcClusFit(Bool_t create=kFALSE);
    static HCategory* getCatMdcWireFit(Bool_t create=kFALSE);
    static HCategory* getCatMdcHitClus(Bool_t create=kFALSE);
                                     
    ClassDef(HMdcGetContainers,0)
};

#endif  /*!HMDCGETCONTAINERS_H*/
