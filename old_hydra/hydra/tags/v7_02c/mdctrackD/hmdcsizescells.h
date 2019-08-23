#ifndef HMDCSIZESCELLS_H
#define HMDCSIZESCELLS_H

#include <TObject.h>
#include <TClonesArray.h>
#include "hmdcgeomobj.h"
#include "hgeomtransform.h"

class HMdcGetContainers;
class HMdcLayerGeomPar;
class HMdcGeomPar;
class HMdcDetector;
class HMdcTrap;
class HMdcGeomStruct;
class HMdcRawStruct;
class HMdcLookupRaw;
class HSpecGeomPar;

class HMdcSizesCellsCell : public TObject {
  friend class HMdcSizesCells;
  friend class HMdcSizesCellsLayer;
  protected:
    HGeomVector wireSizes[2];  // wire (2-poins), first p. connected to readout
    Bool_t status;
    Char_t readOutSide;  // Side from which the wire is read out (first letter
                         // in Mbo name).
                         // ='1' for left, ='2' for right (for top also)
                         // ='0' for not connected wires
    Float_t xReadout;    // position of the wire point connected to readout
                         // in coor.system of wire (y=0,z=0)
    Float_t length;      // wire length
  public:
    HMdcSizesCellsCell(void) {clear();}
    ~HMdcSizesCellsCell(void) {;}
    const HGeomVector* getWirePoint(Int_t n){
        return (n<0||n>1) ? 0:&wireSizes[n]; }
    void setStatus(Bool_t stat) { status=stat; }
    Bool_t getStatus(void) { return status; }
    Float_t getWireLength(void) {return length;}
    Char_t getReadoutSide(void) {return readOutSide;}
    Float_t getXReadout(void) {return xReadout;}
    void clear(void);
  ClassDef(HMdcSizesCellsCell,0)
};

class HMdcSizesCellsLayer : public HMdcPlane {
  friend class HMdcSizesCells;
  protected:
    Short_t sector;
    Short_t module;
    Short_t layer;
    Short_t nCells;
    Double_t halfCatDist;
    Double_t pitch;
    Double_t halfPitch;
    Double_t cosWireOr;
    Double_t sinWireOr;
    Double_t wireOffSet;       //= (CentralWireNr()-1.)*pitch
    HGeomTransform sysRSec;
    Double_t tfSysRSec[12];                                                  
    HGeomTransform sysRMod;
    TClonesArray* array;      // array of pointers of type HMdcSizesCellsCell
    static Double_t radToDeg;
  public:
    HMdcSizesCellsLayer(Int_t sec, Int_t mod, Int_t lay);
    ~HMdcSizesCellsLayer(void);
    HMdcSizesCellsCell& operator[](Int_t i) {
        return *static_cast<HMdcSizesCellsCell*>((*array)[i]);
    }
    Int_t getSize(void);
    const HGeomTransform* getSecTrans(void) { return &sysRSec; }
    const HGeomTransform* getModTrans(void) { return &sysRMod; }
    Bool_t setSecTrans(HGeomTransform * alignTrans=0);
    void transSecToRotLayer(HGeomVector& point);
    Double_t getCatDist(void) const {return 2.*halfCatDist;}
    Double_t getHalfCatDist(void) const {return halfCatDist;}
    Double_t getPitch(void) const {return pitch;}
    Double_t getHalfPitch(void) const {return halfPitch;}
    Double_t getCosWireOr(void) const {return cosWireOr;}
    Double_t getSinWireOr(void) const {return sinWireOr;}
    Double_t getTanWireOr(void) const {return sinWireOr/cosWireOr;}
    Double_t getWireOffSet(void) const {return wireOffSet;}
    Int_t calcCellNum(Double_t x, Double_t y) const;
    Bool_t calcCrossedCells(Double_t xP1,Double_t yP1,Double_t zP1,
        Double_t xP2,Double_t yP2,Double_t zP2,Float_t& cell1,Float_t& cell2);

    Double_t getAlpha(const HGeomVector& p1, const HGeomVector& p2);
    Double_t getDist(const HGeomVector& p1, const HGeomVector& p2, Int_t cell);
    inline Double_t getDist(Double_t xP1, Double_t yP1, Double_t zP1,
        Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell);
    inline void getYZinWireSys(Double_t x, Double_t y, Double_t z, Int_t cell,
        Double_t& yw, Double_t& zw);
    inline Double_t getDistToZero(Double_t y1,Double_t z1,
      Double_t y2, Double_t z2);
    inline Double_t getImpactToZero(Double_t y1,Double_t z1,
      Double_t y2, Double_t z2, Double_t &alpha);
    inline Double_t getImpact(Double_t xP1,Double_t yP1,Double_t zP1,
        Double_t xP2,Double_t yP2,Double_t zP2,Int_t cell,Double_t &alphaPerp);
    void getImpact(const HGeomVector& p1, const HGeomVector& p2, Int_t cell,
        Double_t &alpha, Double_t &per);
    inline Bool_t getImpact(Double_t xP1,Double_t yP1,Double_t zP1,
        Double_t xP2, Double_t yP2, Double_t zP2,
        Int_t cell, Double_t &alphaDrDist, Double_t &driftDist);
    Bool_t getDriftDist(Double_t xP1,Double_t yP1,Double_t zP1,
        Double_t xP2, Double_t yP2, Double_t zP2,
        Int_t cell, Double_t &alphaDrDist, Double_t &driftDist);
    Double_t HMdcSizesCellsLayer::getXSign(Double_t xP1, Double_t yP1,
      Double_t zP1, Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell);
    Float_t getSignPath(Double_t xP1,Double_t yP1,Double_t zP1,
      Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell);
    Float_t getSignPath(Double_t xP1,Double_t yP1,Double_t zP1,
      Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell, Float_t& outside);
    inline void transTo(Double_t& x, Double_t& y, Double_t& z) const;
    void print(void);

  ClassDef(HMdcSizesCellsLayer,0)
};


class HMdcSizesCellsMod : public HMdcPlane {
  friend class HMdcSizesCells;
  protected:
    Short_t sector;          // Address
    Short_t module;          //
    HGeomTransform sysRSec;  // Trnsformation sector<->module
    Double_t tfSysRSec[12];
    Double_t ct[6];          // Lookup table for funct. calcMdcHit, ...
    TObjArray* array;        // array of pointers of type HMdcSizesCellsLayer
  public:
    HMdcSizesCellsMod(Int_t sec, Int_t mod);
    ~HMdcSizesCellsMod(void);
    HMdcSizesCellsLayer& operator[](Int_t i) {
        return *static_cast<HMdcSizesCellsLayer*>((*array)[i]);
      }
    Int_t getSize(void);
    Bool_t setSecTrans(const HGeomTransform& secTrans);
    const HGeomTransform* getSecTrans(void) { return &sysRSec; }
    inline void transTo(Double_t& x, Double_t& y, Double_t& z) const;
    inline void transFrom(Double_t& x, Double_t& y, Double_t& z) const;
    inline void transFromZ0(Double_t& x, Double_t& y, Double_t& z) const;
    inline void transFromZ0(Float_t& x, Float_t& y, Float_t& z) const;
    inline void calcInterTrMod(Double_t x1, Double_t y1, Double_t z1,
        Double_t x2, Double_t y2, Double_t z2,
        Double_t& x, Double_t& y) const;
    inline void calcMdcHit(Double_t x1, Double_t y1, Double_t z1,
        Double_t x2, Double_t y2, Double_t z2,
        Double_t& x, Double_t& y, Double_t& xDir, Double_t& yDir) const;
    void calcMdcHit(Double_t x1, Double_t y1, Double_t z1,
        Double_t x2, Double_t y2, Double_t z2,
        Double_t eX1, Double_t eY1, Double_t eZ1, Double_t eX2, Double_t eY2,
        Double_t dZ1dX1, Double_t dZ1dY1, Double_t dZ2dX2, Double_t dZ2dY2,
        Double_t& x, Double_t& eX, Double_t& y, Double_t& eY,
        Double_t& xDir, Double_t& eXDir, Double_t& yDir, Double_t& eYDir) const;
    const Double_t* getTfSysRSec(void) {return tfSysRSec;}
    const Double_t* getMdcHitLookupTb(void) {return ct;}
  protected:
    void setSecTrans(const HGeomTransform* secTrans);

  ClassDef(HMdcSizesCellsMod,0)
};


class HMdcSizesCellsSec : public TObject {
  friend class HMdcSizesCells;
  protected:
    Short_t sector;
    TObjArray* array;             // HMdcSizesCellsMod array
    HGeomTransform sysRLab;
    Bool_t* mdcStatSec;           // kTRUE - mdc exist
    
    HGeomVector target[3];  // [0] First point of target in sector coor.sys.
                            // [1] Middle point of target in sector coor.sys.
                            // [2] Last point of target in sector coor.sys.
  public:
    HMdcSizesCellsSec(Int_t sec, Bool_t* mdcStSec);
    ~HMdcSizesCellsSec(void);
    HMdcSizesCellsMod& operator[](Int_t i) {
      return *static_cast<HMdcSizesCellsMod*>((*array)[i]);
    }
    Int_t getSize(void);
    const HGeomTransform* getLabTrans(void) { return &sysRLab; }
    Bool_t modStatus(Int_t m) const {
      if(m<0 || m>3) return kFALSE;
      return mdcStatSec[m];
    }
    const HGeomVector& getTargetFirstPoint(void) const {return target[0];}
    const HGeomVector& getTargetMiddlePoint(void) const {return target[1];}
    const HGeomVector& getTargetLastPoint(void) const {return target[2];}
    void calcRZToTargLine(Double_t x1,Double_t y1,Double_t z1, 
        Double_t x2,Double_t y2,Double_t z2,  Double_t &zm,Double_t &r0) const;

  ClassDef(HMdcSizesCellsSec,0)
};

class HMdcSizesCells : public TObject {
  protected:
    static HMdcSizesCells* fMdcSizesCells;
    HMdcGetContainers* fGetCont;
    HMdcLayerGeomPar* fLayerGeomPar;
    Int_t verLayerGeomPar[3];
    HMdcGeomPar* fGeomPar;
    Int_t verGeomPar[3];
    HMdcDetector* fMdcDet;
    Bool_t mdcStatus[6][4];          // kTRUE - mdc exist
    Int_t nModsSeg[6][2];            // number of mdc per segment
    HMdcGeomStruct* fMdcGeomStruct;
    HMdcLookupRaw* fMdcLookupRaw;
    Int_t verLookupRaw[3];
    HMdcRawStruct* fMdcRawStruct;
    HSpecGeomPar* fSpecGeomPar;
    TObjArray* array;                // array of pointers to HMdcSizesCellsSec
    Bool_t  changed;
    HGeomVector target[3];  // [0] First point of target in lab.sys.
                            // [2] Middle point of target in lab.sys.
                            // [3] Last point of target in lab.sys.
  public:
    static HMdcSizesCells* getObject(void);
    static HMdcSizesCells* getExObject(void);
    static void deleteCont(void);
    HMdcSizesCellsSec& operator[](Int_t i) {
      return *static_cast<HMdcSizesCellsSec*>((*array)[i]);
    }
    Int_t  getSize(void);
    Bool_t initContainer(void);
    Bool_t hasChanged(void) {return changed;}
    void   clear(void);
    Bool_t getCellVol(Int_t sec,Int_t mod,Int_t lay,Int_t cell,
        HMdcTrap& volCell);
    Char_t findReadOutSide(Int_t s,Int_t m,Int_t l,Int_t c);
    inline static void calcMdcSeg(Double_t x1,Double_t y1,Double_t z1, 
                                Double_t x2, Double_t y2, Double_t z2,
        Double_t &zm, Double_t &r0, Double_t &theta, Double_t &phi);
    static void calcMdcSeg(Double_t x1, Double_t y1, Double_t z1, 
                           Double_t x2, Double_t y2, Double_t z2,
        Double_t eX1, Double_t eY1, Double_t eZ1, Double_t eX2, Double_t eY2,
        Double_t dZ1dX1, Double_t dZ1dY1, Double_t dZ2dX2, Double_t dZ2dY2,
        Double_t& zm, Double_t& eZm, Double_t& r0,  Double_t& eR0,
        Double_t& theta, Double_t& eTheta, Double_t& phi, Double_t& ePhi);
    Bool_t modStatus(Int_t s, Int_t m) const {
      if(s<0 || s>5 || m<0 || m>3) return kFALSE;
      return mdcStatus[s][m];
    }
    Int_t nModInSeg(Int_t s, Int_t seg) const {
      if(s<0 || s>5 || seg<0 || seg>1) return kFALSE;
      return nModsSeg[s][seg];
    }
    Bool_t fillModCont(Int_t sec, Int_t mod, HGeomTransform * alignTrans=0);
    const HGeomVector& getTargetFirstPoint(void) const {return target[0];}
    const HGeomVector& getTargetMiddlePoint(void) const {return target[1];}
    const HGeomVector& getTargetLastPoint(void) const {return target[2];}
    void calcRZToTargLine(Double_t x1,Double_t y1,Double_t z1, 
        Double_t x2,Double_t y2,Double_t z2,  Double_t &zm,Double_t &r0) const;
    static void calcRZToLineXY(Double_t x1, Double_t y1, Double_t z1, 
                               Double_t x2, Double_t y2, Double_t z2, 
        Double_t xBeam, Double_t yBeam, Double_t &zm, Double_t &r0);
  protected:
    HMdcSizesCells(void);
    ~HMdcSizesCells(void);
    Bool_t fillCont(Int_t sec);
    Bool_t fillTargetPos(void);

  ClassDef(HMdcSizesCells,0)
};

//----------------------- Inlines ------------------------------
inline void HMdcSizesCellsLayer::transTo(Double_t& x, Double_t& y, Double_t& z) const {
  // transform. point x,y,z from sector coor.sys. to layer coor.sys.
  Double_t pmtx=x-tfSysRSec[ 9];
  Double_t pmty=y-tfSysRSec[10];
  Double_t pmtz=z-tfSysRSec[11];
  x=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  y=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  z=tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
}

inline Double_t HMdcSizesCellsLayer::getDist(Double_t xP1,Double_t yP1,Double_t zP1,
    Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell) {
  // calc.  the minimal distance from line xP1,yP1,zP1-xP2,yP2,zP2  to wire.
  // xP1,yP1,zP1,xP2,yP2,zP2 - in sector coor.sys.
  Double_t yWire=cell*pitch-wireOffSet;
  Double_t pmtx,pmty,pmtz;
  pmtx=xP1-tfSysRSec[ 9];
  pmty=yP1-tfSysRSec[10];
  pmtz=zP1-tfSysRSec[11];
  Double_t x1,y1,z1,x2,y2,z2;
  x1=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  y1=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  z1=tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
  y1 = y1*cosWireOr-x1*sinWireOr - yWire;
  pmtx=xP2-tfSysRSec[ 9];
  pmty=yP2-tfSysRSec[10];
  pmtz=zP2-tfSysRSec[11];
  x2=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  y2=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  z2=tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
  y2 = y2*cosWireOr-x2*sinWireOr - yWire;

  Double_t dz=z1-z2;
  Double_t dy=y1-y2;
  Double_t lng=1./sqrt(dz*dz+dy*dy); //(dz*dz+dy*dy)=0 if tr.paral. to wire,only
  return fabs((z1*y2-z2*y1)*lng);
}

inline void HMdcSizesCellsLayer::getYZinWireSys(Double_t x, Double_t y,
      Double_t z, Int_t cell, Double_t& yw, Double_t& zw) {
  // Output: yw,zw - in wire coor.sys.
  // Wire coor.sys. is line y=z=0, Y - along layer
  // x,y,z - in sector coor.sys.
  Double_t yWire=cell*pitch-wireOffSet;
  Double_t pmtx,pmty,pmtz;
  pmtx=x-tfSysRSec[ 9];
  pmty=y-tfSysRSec[10];
  pmtz=z-tfSysRSec[11];
  Double_t xw;
  xw=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  yw=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  zw=tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
  yw = yw*cosWireOr-xw*sinWireOr - yWire;
}

inline Double_t HMdcSizesCellsLayer::getDistToZero(Double_t y1,Double_t z1,
    Double_t y2, Double_t z2) {
  // calc. the minimal distance from line (y1,z1) - (y2,z2)  to poin
  // y=z=0 in plane y-z.
  Double_t dz=z1-z2;
  Double_t dy=y1-y2;
  Double_t lng=1./sqrt(dz*dz+dy*dy); //(dz*dz+dy*dy)=0 if tr.paral. to wire,only
  return fabs((z1*y2-z2*y1)*lng);
}

inline Double_t HMdcSizesCellsLayer::getImpactToZero(Double_t y1,Double_t z1,
    Double_t y2, Double_t z2, Double_t &alpha) {
  // calc. the angle alpha (in degree) between line (y1,z1) - (y2,z2)
  // in Y-Z plane and Y axis and the minimal distance from this line
  // to poin y=z=0 in plane y-z.
  Double_t dz=z1-z2;
  Double_t dy=y1-y2;
  alpha=atan2(fabs(dz),fabs(dy))*radToDeg;
  Double_t lng=1./sqrt(dz*dz+dy*dy); //(dz*dz+dy*dy)=0 if tr.paral. to wire,only
  return fabs((z1*y2-z2*y1)*lng);
}

inline Double_t HMdcSizesCellsLayer::getImpact(Double_t xP1,Double_t yP1,Double_t zP1,
    Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell, Double_t &alpha) {
  // calc. the angle alpha (in degree) between projection of line
  // xP1,yP1,zP1-xP2,yP2,zP2 on the Y-Z plane and Y axis in coor.sys.
  // of wires and the minimal distance from line xP1,yP1,zP1-xP2,yP2,zP2
  // to wire.
  // xP1,yP1,zP1,xP2,yP2,zP2 - in sector coor.sys.
  Double_t yWire=cell*pitch-wireOffSet;
  Double_t pmtx,pmty,pmtz;
  pmtx=xP1-tfSysRSec[ 9];
  pmty=yP1-tfSysRSec[10];
  pmtz=zP1-tfSysRSec[11];
  Double_t x1,y1,z1,x2,y2,z2;
  x1=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  y1=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  z1=tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
  y1 = y1*cosWireOr-x1*sinWireOr - yWire;
  pmtx=xP2-tfSysRSec[ 9];
  pmty=yP2-tfSysRSec[10];
  pmtz=zP2-tfSysRSec[11];
  x2=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  y2=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  z2=tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
  y2 = y2*cosWireOr-x2*sinWireOr - yWire;

  Double_t dz=z1-z2;
  Double_t dy=y1-y2;
  alpha=atan2(fabs(dz),fabs(dy))*radToDeg;
  Double_t lng=1./sqrt(dz*dz+dy*dy); //(dz*dz+dy*dy)=0 if tr.paral. to wire,only
  return fabs((z1*y2-z2*y1)*lng);
}

inline Bool_t HMdcSizesCellsLayer::getImpact(Double_t xP1,Double_t yP1,Double_t zP1,
    Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell,
    Double_t &alpha, Double_t &minDist) {
  // return kTRUE if line xP1,yP1,zP1-xP2,yP2,zP2 intersect cell or kFALSE
  // if not intersect and calc. the angle alpha (in degree) between projection
  // of the line  xP1,yP1,zP1-xP2,yP2,zP2 on the Y-Z plane and Y axis in coor.
  // sys. of wires and the minimal distance from line xP1,yP1,zP1-xP2,yP2,zP2
  // to wire.
  // xP1,yP1,zP1,xP2,yP2,zP2 - in sector coor.sys.
  Double_t yWire=cell*pitch-wireOffSet;
  Double_t pmtx=xP1-tfSysRSec[ 9];
  Double_t pmty=yP1-tfSysRSec[10];
  Double_t pmtz=zP1-tfSysRSec[11];
  Double_t x1,y1,z1,x2,y2,z2;
  x1=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  y1=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  z1=tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
  y1 = y1*cosWireOr-x1*sinWireOr - yWire; // to coor.sys. of wire
  pmtx=xP2-tfSysRSec[ 9];
  pmty=yP2-tfSysRSec[10];
  pmtz=zP2-tfSysRSec[11];
  x2=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  y2=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  z2=tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
  y2 = y2*cosWireOr-x2*sinWireOr - yWire; // to coor.sys. of wire

  Double_t dz=fabs(z1-z2);
  Double_t dy=fabs(y1-y2);
  Double_t lng=1./sqrt(dz*dz+dy*dy);
  Double_t yDist = fabs(z1*y2-z2*y1); // abs(Ywire-Ycross_track)=yDist/dz
  minDist = yDist*lng;
  alpha=atan2(dz,dy)*radToDeg;
  if(minDist * dz*lng > halfPitch) {
    if(dy==0.0) return kFALSE;
    if((yDist-halfPitch*dz)/dy > halfCatDist)  return kFALSE;
  } else if(minDist*dy*lng > halfCatDist &&    // dy*lng = cos(alpha)
      (yDist-halfCatDist*dy)/dz > halfPitch) return kFALSE;
  return kTRUE;
}

inline void HMdcSizesCellsMod::transTo(Double_t& x, Double_t& y, Double_t& z) const {
  // transform. point x,y,z from sector coor.sys. to mdc coor.sys.
  Double_t pmtx=x-tfSysRSec[ 9];
  Double_t pmty=y-tfSysRSec[10];
  Double_t pmtz=z-tfSysRSec[11];
  x=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  y=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  z=tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
}

inline void HMdcSizesCellsMod::transFrom(Double_t& x, Double_t& y, Double_t& z) const {
  // transform. point x,y,z from mdc coor.sys. to sector coor.sys.
  Double_t pmtx=x;
  Double_t pmty=y;
  Double_t pmtz=z;
  x=tfSysRSec[0]*pmtx+tfSysRSec[1]*pmty+tfSysRSec[2]*pmtz+tfSysRSec[ 9];
  y=tfSysRSec[3]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[5]*pmtz+tfSysRSec[10];
  z=tfSysRSec[6]*pmtx+tfSysRSec[7]*pmty+tfSysRSec[8]*pmtz+tfSysRSec[11];
}

inline void HMdcSizesCellsMod::transFromZ0(Double_t& x,Double_t& y,Double_t& z) const {
  // transform. point x,y on the mdc plane (z=0) from mdc coor.sys. 
  // to sector coor.sys.
  Double_t pmtx=x;
  Double_t pmty=y;
  x=tfSysRSec[0]*pmtx+tfSysRSec[1]*pmty+tfSysRSec[ 9];
  y=tfSysRSec[3]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[10];
  z=tfSysRSec[6]*pmtx+tfSysRSec[7]*pmty+tfSysRSec[11];
}

inline void HMdcSizesCellsMod::transFromZ0(Float_t& x,Float_t& y,Float_t& z) const {
  // transform. point x,y on the mdc plane (z=0) from mdc coor.sys. 
  // to sector coor.sys.
  Double_t pmtx=x;
  Double_t pmty=y;
  x=tfSysRSec[0]*pmtx+tfSysRSec[1]*pmty+tfSysRSec[ 9];
  y=tfSysRSec[3]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[10];
  z=tfSysRSec[6]*pmtx+tfSysRSec[7]*pmty+tfSysRSec[11];
}

inline void HMdcSizesCellsMod::calcInterTrMod(Double_t x1, Double_t y1, Double_t z1,
      Double_t x2, Double_t y2, Double_t z2,
      Double_t& x, Double_t& y) const {
  Double_t dX=x2-x1;
  Double_t dY=y2-y1;
  Double_t dZ=z2-z1;
  Double_t del=1/(parA*dX+parB*dY+dZ);
  Double_t xt=(dX*(parD-z1-parB*y1)+x1*(parB*dY+dZ))*del-tfSysRSec[ 9];
  Double_t yt=(dY*(parD-z1-parA*x1)+y1*(parA*dX+dZ))*del-tfSysRSec[10];
  x=ct[0]*xt+ct[1]*yt+ct[2];
  y=ct[3]*xt+ct[4]*yt+ct[5];
}

inline void HMdcSizesCellsMod::calcMdcHit(Double_t x1, Double_t y1, Double_t z1,
      Double_t x2, Double_t y2, Double_t z2,
      Double_t& x, Double_t& y, Double_t& xDir, Double_t& yDir) const {
  // Calcul. a cross of the line with plane MDC (parA*x+parB*y+c*z=parD),
  // transform. this point to mdc coor.sys. (z=0) and calc.
  // hit direction in mdc coor.sys.
  Double_t dX=x2-x1;
  Double_t dY=y2-y1;
  Double_t dZ=z2-z1;
  Double_t del=1/(parA*dX+parB*dY+dZ);
  Double_t xt=(dX*(parD-z1-parB*y1)+x1*(parB*dY+dZ))*del-tfSysRSec[ 9];
  Double_t yt=(dY*(parD-z1-parA*x1)+y1*(parA*dX+dZ))*del-tfSysRSec[10];
  x=ct[0]*xt+ct[1]*yt+ct[2];
  y=ct[3]*xt+ct[4]*yt+ct[5];
  //---Hit direction----------------------------------------------------
  Double_t length=1/sqrt(dX*dX+dY*dY+dZ*dZ);
  xDir=(tfSysRSec[0]*dX+tfSysRSec[3]*dY+tfSysRSec[6]*dZ)*length; // unit vector
  yDir=(tfSysRSec[1]*dX+tfSysRSec[4]*dY+tfSysRSec[7]*dZ)*length;
}

inline void HMdcSizesCells::calcMdcSeg(Double_t x1, Double_t y1, Double_t z1, 
                                Double_t x2, Double_t y2, Double_t z2, 
    Double_t &zm, Double_t &r0, Double_t &theta, Double_t &phi) {
  // Input and output are in sector coor.sys.
  // theta=atan(sqrt(dX*dX+dY*dY)/dZ);  phi=atan(dY/dX)
  // zm= z1 - cos(theta)/sin(theta) * (x1*cos(phi)+y1*sin(phi))
  // r0=y1*cos(phi)-x1*sin(phi)
  //
  // If (x1,y1,z1)=(x2,y2,z2) dZ will eq.1.! 
  Double_t dX=x2-x1;
  Double_t dY=y2-y1;
  Double_t dZ=z2-z1;
  Double_t lenXY=sqrt(dX*dX+dY*dY);
  if(lenXY<2.E-5) {
    dX =x2 * 1.E-5/sqrt(x2*x2+y2*y2);
    dY =y2 * 1.E-5/sqrt(x2*x2+y2*y2);
    lenXY=1.E-5;            //dX*dX+dY*dY;
    dZ=1.;
  }
  dX /= lenXY;
  dY /= lenXY;
  dZ /= lenXY;
  phi=atan2(dY,dX);
  theta=atan2(1.,dZ);
  zm=z1-dZ*(x1*dX+y1*dY);
  r0=y1*dX-x1*dY;
}

#endif  /*!HMDCSIZESCELLS_H*/
