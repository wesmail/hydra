#ifndef HTofinoCalParCell_H
#define HTofinoCalParCell_H
#pragma interface

#include "hdataobject.h"

class HTofinoCalParCell : public HDataObject {
public:
    HTofinoCalParCell();
//  initialization could be used e.g. to clear the data element
    HTofinoCalParCell(Float_t fSlope);
    ~HTofinoCalParCell() {}

    Int_t reset();
    Float_t getSlope() {return m_fSlope;}
    Float_t getDeltaSlope() {return m_fDeltaSlope;}
    Float_t getCoordPosition() {return m_fCoordPos;}
    void getDistanceParams(Float_t* dist);

    void setSlope(Float_t fSlope) {m_fSlope = fSlope;}
    void setDeltaSlope(Float_t fDeltaSlope) {m_fDeltaSlope = fDeltaSlope;}
    void setCoordPosition(Float_t fCoordPos) {m_fCoordPos = fCoordPos;}
    void setParams(Float_t fSlope);
    void setDeltaParams(Float_t fDeltaSlope);
    void setDistanceParams(Float_t dpar1, Float_t dpar2, Float_t dpar3);
    void setDistanceParams(Float_t* disttab);

    Float_t calcDistance(Float_t row);
    Float_t calcDistPMT(Float_t y);

    ClassDef(HTofinoCalParCell,1) //Tofino calibration parameters per cell

private:
    Float_t m_fSlope;        // slope parameter for  calibration of the cell
    Float_t m_fDeltaSlope;   // error in slope parameter 
    Float_t m_fDistParam[3]; // parameters for distance calculation
    Float_t m_fCoordPos;     // y position of the coordinate system in the cell
};

#endif
