#ifndef HMDCGETCONTAINERS_H
#define HMDCGETCONTAINERS_H

#include "hruntimedb.h"
#include "hades.h"
#include "hmdctrackddef.h"
#include "hmdcdetector.h"
#include "hspectrometer.h"
#include "hmdclayergeompar.h"
#include "hmdcgeompar.h"
#include "hspecgeompar.h"
#include "hgeomcompositevolume.h"
#include "hmdcgeomstruct.h"
#include "hmdcrawstruct.h"
#include "hmdclookupgeom.h"
#include "hkickplane.h"
#include "hcategory.h"
#include "hevent.h"
#include "hmdccal1sim.h"

class HMdcGetContainers : public HParSet {
  protected:
    HRuntimeDb* rtdb;
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
  public:
    HMdcGetContainers();
    ~HMdcGetContainers(){;}
//    static const HMdcGetContainers* fMdcGetContainers; //???
    static HMdcGetContainers* getObject();
    Bool_t init(HParIo* input,Int_t* set) {return kTRUE;}
    
    HMdcLayerGeomPar* getMdcLayerGeomPar(Bool_t init=kTRUE);
    HMdcGeomPar* getMdcGeomPar(Bool_t init=kTRUE);
    HMdcDetector* getMdcDetector();
    HSpecGeomPar* getSpecGeomPar(Bool_t init=kTRUE);
    HMdcGeomStruct* getMdcGeomStruct(Bool_t init=kTRUE);
    HMdcRawStruct* getMdcRawStruct(Bool_t init=kTRUE);
    HMdcLookupGeom* getMdcLookupGeom(Bool_t init=kTRUE);
    HKickPlane* getKickPlane(Bool_t init=kTRUE);
    
    // For MDC only:
    HGeomCompositeVolume* getGeomCompositeVolume(Int_t mod, Bool_t init=kTRUE);
    HMdcLayerGeomParLay* getMdcLayerGeomParLay(Int_t sec,Int_t mod,Int_t lay,
                                               Bool_t init=kTRUE);
  
    static HCategory * getCatMdcCal1(){
      return gHades->getCurrentEvent()->getCategory(catMdcCal1);}
    static Bool_t HMdcGetContainers::isGeant() {
      return (getCatMdcCal1()->getClass()!=HMdcCal1Sim::Class())?kFALSE:kTRUE;}
    static HCategory * getCatMdcClus(Bool_t create=kFALSE);
                                     
    ClassDef(HMdcGetContainers,0)
};

#endif  /*!HMDCGETCONTAINERS_H*/
