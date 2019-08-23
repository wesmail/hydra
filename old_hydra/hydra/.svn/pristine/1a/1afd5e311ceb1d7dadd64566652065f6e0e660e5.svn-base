#ifndef __HShowerSectorGeom_H
#define __HShowerSectorGeom_H
#pragma interface

#include "TObject.h"
#include "TObjArray.h"
#include "hshowerparset.h"
#include "hgeomtransform.h"

class HShowerModuleGeom;

class HShowerSectorGeom: public HShowerParSet
{
  public:
    HShowerSectorGeom();
    virtual ~HShowerSectorGeom();

    void reset();
    virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
    virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);


//    Int_t isOut(Float_t x, Float_t y);

    HShowerModuleGeom* getModule(Int_t n);
    Int_t addModule(HShowerModuleGeom*);
    Int_t setModule(HShowerModuleGeom* pCorner, Int_t n); 

    void setModules(Int_t n){m_nModules = n;}
    Int_t getModules(){return m_nModules;}

    void setSectorID(Int_t id){m_nSectorID = id;}
    Int_t getSectorID(){return m_nSectorID;}

    void setAngleRot(Float_t angle){angleRot = angle;}
    Float_t getAngleRot(){return angleRot;}

    const HGeomTransform& getTransform(){return transform;}
    void setTransform(const HGeomTransform& t){transform = t;}

    void translateModules();
    void rotateSector();

    void transformSector(HGeomTransform& tt);

    ClassDef(HShowerSectorGeom, 1)

public:
  Int_t m_nModules;    //number of modules in sector
  Float_t  angleRot;   //m_nSectorID * 60 degree
  HGeomTransform transform; //transformation data

private:
  Int_t m_nSectorID;  //identification of sector

  TObjArray m_ModuleArr;  //table of lab coord of modules
};

#endif
