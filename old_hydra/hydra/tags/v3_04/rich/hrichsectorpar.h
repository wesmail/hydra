#ifndef HRICHSECTORPAR_H
#define HRICHSECTORPAR_H
#pragma interface

#include <TObject.h>
#include <TObjArray.h>
#include "hrichparset.h"
#include "hgeomtransform.h"


class HRichSectorPar: public HRichParSet
{

  public:

    HRichSectorPar();
    virtual ~HRichSectorPar();

    void reset();
    virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);


//    Int_t isOut(Float_t x, Float_t y);

    void setSectorId(Int_t id) { fSectorId = id; }
    Int_t getSectorId() { return fSectorId; }

    Float_t getRotationAngle() { return fRotationAngle; }
    void setRotationAngle(Float_t angle) { fRotationAngle = angle; }

    const HGeomTransform& getTransform() { return transform; }
    void setTransform(const HGeomTransform& t) { transform = t; }

    void transformSector(HGeomTransform& tt);
    // void transformModule(HGeomTransform& t);
    void transformVector(HGeomTransform& t);

    // void translateModules();
    void rotateSector();


public:

  HGeomTransform transform;

  Float_t fRotationAngle;
  Float_t fRotationMatrix[9];
  Float_t fSectorCentre[3];

private:

  Int_t fSectorId;


 ClassDef(HRichSectorPar, 1)
};

#endif // HRICHSECTORPAR_H
