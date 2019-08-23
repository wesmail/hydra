#ifndef __HShowerSectorGeom_H
#define __HShowerSectorGeom_H
#pragma interface

#include <TObject.h>
#include <TObjArray.h>
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

    void setSectorID(int id){m_nSectorID = id;}
    int getSectorID(){return m_nSectorID;}

    void setAngleRot(Float_t angle){angleRot = angle;}
    Float_t getAngleRot(){return angleRot;}

    const HGeomTransform& getTransform(){return transform;}
    void setTransform(const HGeomTransform& t){transform = t;}

    void translateModules();
    void rotateSector();

    void transformSector(HGeomTransform& tt);

    ClassDef(HShowerSectorGeom, 1)

public:
  int m_nModules;
  float  angleRot;   //m_nSectorID * 60 degree
  HGeomTransform transform;

private:
  int m_nSectorID;

  TObjArray m_ModuleArr;  //table of geometry of modules
};

#endif
