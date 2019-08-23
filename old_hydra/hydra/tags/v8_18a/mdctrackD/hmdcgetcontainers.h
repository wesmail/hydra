#ifndef HMDCGETCONTAINERS_H
#define HMDCGETCONTAINERS_H

#include "hades.h"
#include "hmdctrackddef.h"
#include "hcategory.h"
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
class HMdcLayerCorrPar;
class HParSet;

class HMdcGetContainers : public TObject {
  protected:
    static HMdcGetContainers* fMdcGetContainers;

    HCategory*        pMdcCal1Cat;
    HCategory*        pGeantKineCat;
    HCategory*        pGeantMdcCat;
    HCategory*        pMdcClusCat;
    HCategory*        pMdcClusFitCat;
    HCategory*        pMdcWireFitCat;
    HCategory*        pMdcClusInfCat;
    HCategory*        pMdcTrkCandCat;
    
    HRuntimeDb*       rtdb;
    Bool_t            isRtdb;
    HSpectrometer*    spectr;
    HMdcLayerGeomPar* fMdcLayerGeomPar;
    Bool_t            isMdcLayerGeomPar;
    HMdcGeomPar*      fMdcGeomPar;
    Bool_t            isMdcGeomPar;
    HMdcDetector*     fMdcDetector;
    Bool_t            isMdcDetector;
    HSpecGeomPar*     fSpecGeomPar;
    Bool_t            isSpecGeomPar;
    HMdcGeomStruct*   fMdcGeomStruct;
    Bool_t            isMdcGeomStruct;
    HMdcRawStruct*    fMdcRawStruct;
    Bool_t            isMdcRawStruct;
    HMdcLookupGeom*   fMdcLookupGeom;
    Bool_t            isMdcLookupGeom;
    HKickPlane2*      fKickPlane;
    Bool_t            isKickPlane;
    HMdcLayerCorrPar* fMdcLayerCorrPar;
    Bool_t            isMdcLayerCorrPar;
    
Bool_t useOldGeometry;
    
  public:
    static Bool_t isInited(HParSet *par);
    static HMdcGetContainers* getObject();
    static void deleteCont();
    
    // Parameters HRuntimeDb:
    HMdcLayerGeomPar*     getMdcLayerGeomPar(Bool_t init=kFALSE);
    HMdcLayerCorrPar*     getMdcLayerCorrPar(Bool_t init=kFALSE);
    HMdcGeomPar*          getMdcGeomPar(Bool_t init=kFALSE);
    HMdcDetector*         getMdcDetector();
    HSpecGeomPar*         getSpecGeomPar(Bool_t init=kFALSE);
    HMdcGeomStruct*       getMdcGeomStruct(Bool_t init=kFALSE);
    HMdcRawStruct*        getMdcRawStruct(Bool_t init=kFALSE);
    HMdcLookupGeom*       getMdcLookupGeom(Bool_t init=kFALSE);
    HKickPlane2*          getKickPlane(Bool_t init=kFALSE);
    HModGeomPar*          getModGeomPar(Int_t sec,Int_t mod,Bool_t init=kFALSE);
    HGeomCompositeVolume* getGeomCompositeVolume(Int_t mod, Bool_t init=kFALSE);
    HMdcLayerGeomParLay*  getMdcLayerGeomParLay(Int_t sec,Int_t mod,Int_t lay,
                                               Bool_t init=kFALSE);
    // Transformations:
    const HGeomTransform& getLabTransSec(Int_t sec, Bool_t init=kFALSE);
    Bool_t   getLabTransSec(HGeomTransform& trans,Int_t sec,Bool_t init=kFALSE);
    const HGeomTransform& getLabTransMod(Int_t sec,Int_t mod,
                                         Bool_t init=kFALSE);
    Bool_t   getLabTransMod(HGeomTransform& trans, Int_t sec, Int_t mod, 
                            Bool_t init=kFALSE);
    Bool_t   getSecTransMod(HGeomTransform& trans, Int_t sec, Int_t mod, 
                            Bool_t init=kFALSE);
    Bool_t   getLabTransLayer(HGeomTransform& trans, Int_t sec,Int_t mod,
                              Int_t lay, Bool_t init=kFALSE);
    Bool_t   getSecTransLayer(HGeomTransform& trans, Int_t sec,Int_t mod, 
                              Int_t lay, Bool_t init=kFALSE);
    const HGeomTransform& getModTransLayer(Int_t sec, Int_t mod, Int_t lay,
                                           Bool_t init=kFALSE);
    Bool_t   getSecTransCathLayer(HGeomTransform& trans1,HGeomTransform& trans2,
                                  Int_t sec, Int_t mod, Int_t lay,
                                  Bool_t init=kFALSE);
    Bool_t   getLabTransLayerZ0(HGeomTransform& trans, Int_t sec,Int_t mod, 
                                Int_t lay, Bool_t init=kFALSE){return
                                getLabTransLayer(trans,sec,mod,init);}
    Double_t getSensWireThick(Int_t mod, Int_t lay);
    Bool_t   getModTransGeantLayer(HGeomTransform& trans, Int_t sec,
                                   Int_t mod,Int_t lay);
    Bool_t   getLabTransGeantLayer(HGeomTransform& trans, Int_t sec,
                                   Int_t mod, Int_t lay);
    Bool_t   getSecTransGeantLayer(HGeomTransform& trans, Int_t sec,Int_t mod, 
                                   Int_t lay);
     
    // Categories:
    HCategory*    getCatMdcCal1(void);
    HCategory*    getCatGeantKine(void);
    HCategory*    getCatGeantMdc(void);
    HCategory*    getCatMdcClus(Bool_t create=kFALSE);
    HCategory*    getCatMdcClusFit(Bool_t create=kFALSE);
    HCategory*    getCatMdcWireFit(Bool_t create=kFALSE);
    HCategory*    getCatMdcClusInf(Bool_t create=kFALSE);
    HCategory*    getCatMdcTrkCand(Bool_t create=kFALSE);
    
    //  For using in macro without Hades object:
    void          setCatMdcCal1(HCategory* cat)    {pMdcCal1Cat = cat;}
    void          setCatGeantKine(HCategory* cat)  {pGeantKineCat = cat;}
    void          setCatGeantMdc(HCategory* cat)   {pGeantMdcCat = cat;}
    void          setCatMdcClus(HCategory* cat)    {pMdcClusCat = cat;}
    void          setCatMdcClusFit(HCategory* cat) {pMdcClusFitCat = cat;}
    void          setCatMdcWireFit(HCategory* cat) {pMdcWireFitCat = cat;}
    void          setCatMdcClusInf(HCategory* cat) {pMdcClusInfCat = cat;}
    void          setCatMdcTrkCand(HCategory* cat) {pMdcTrkCandCat = cat;}
    
    // Others:
    static Bool_t isGeant(void) {return getObject()->isGeantObj();}
    static void   getFileName(TString& fileName);
    static void   getFileNameWInd(TString& fileName);
    static void   getRunId(TString& runId);
    static void   getEventDate(TString& eventDate);
    static void   getEventTime(TString& eventTime);
    
void setUseOldGeometry(void) {useOldGeometry=kTRUE;}
    
  private:
    Bool_t isGeantObj(void);
    Bool_t initCont(HParSet *par);
    HMdcGetContainers();
    ~HMdcGetContainers(){}
                                         
    ClassDef(HMdcGetContainers,0)
};

#endif  /*!HMDCGETCONTAINERS_H*/
