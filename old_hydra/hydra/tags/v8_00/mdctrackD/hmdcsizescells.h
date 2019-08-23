#ifndef HMDCSIZESCELLS_H
#define HMDCSIZESCELLS_H

#include <TObject.h>
#include <TClonesArray.h>
#include <TMath.h>
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
class HMdcLayerGeomParLay;
class HGeomVolume;

class HMdcSizesCellsCell : public TObject {
  friend class HMdcSizesCellsLayer;
  friend class HMdcSizesCellsMod;
  friend class HMdcSizesCellsSec;
  friend class HMdcSizesCells;
  protected:
    Short_t        cell;  // Cell number
    HGeomVector wirePnt1; // wire geometry, 2-poins in sector coor.sys.
    HGeomVector wirePnt2; // first p. connected to readout
    Bool_t status;        // =kTRUE if wire conected to readout
    Char_t readOutSide;   // Side from which the wire is read out (first letter
                          // in Mbo name).
                          // ='1' for left, ='2' for right (for top also)
                          // ='0' for not connected wires
    Float_t xReadout;     // position of the wire point connected to readout
                          // in coor.system of wire (y=0,z=0)
    Float_t length;       // wire length
  public:
    HMdcSizesCellsCell(Int_t c) {clear(); cell=c;}
    ~HMdcSizesCellsCell(void) {;}
    const HGeomVector* getWirePoint(Int_t n) const {
        if(n<0||n>1) return 0;
        return n==0 ? &wirePnt1:&wirePnt2; }
        
    void    setStatus(Bool_t stat)     {status=stat;}
    Short_t getCell(void) const        {return cell;}
    Bool_t  getStatus(void) const      {return status;}
    Float_t getWireLength(void) const  {return length;}
    Char_t  getReadoutSide(void) const {return readOutSide;}
    Float_t getXReadout(void) const    {return xReadout;}
    void clear(void);
    
  private:

  ClassDef(HMdcSizesCellsCell,0)
};

class HMdcSizesCellsLayer : public HMdcPlane {
  friend class HMdcSizesCellsMod;
  friend class HMdcSizesCellsSec;
  friend class HMdcSizesCells;
  protected:
    Short_t              sector;            // Address:           
    Short_t              module;
    Short_t              layer;
    HMdcLayerGeomParLay* pLayerGeomParLay;  // Layer parameters
    HGeomVolume*         pGeomVol;          // Layer geometry
    Short_t              nCells;            // Number of wires in layer
    Double_t             halfCatDist;       // Half of cathodes distance
    Double_t             pitch;             // Distance between wires
    Double_t             halfPitch;         // Half of "pitch"
    Double_t             cosWireOr;         // Cosine of wire orientation
    Double_t             sinWireOr;         // Sine of wire orientation angle
    Double_t             wireOffSet;        // = (CentralWireNr()-1.)*pitch
    HGeomTransform       sysRSec;           // Transformation sector <-> layer
    Double_t             tSysRSec[12];      // - - -
    HGeomTransform       rotLaySysRSec;     // Transformation sector <->
    Double_t             tRLaySysRSec[12];  //   rotated on WireOr deg. layer
    HGeomTransform       sysRMod;           // Transformation module<->layer
    TClonesArray*        array;             // HMdcSizesCellsCell objects
    HMdcSizesCellsMod*   pToMod;            // Pointer to corresponding module
  public:
    HMdcSizesCellsLayer(HMdcSizesCellsMod* pMod, Int_t lay);
    ~HMdcSizesCellsLayer(void);
    HMdcSizesCellsCell& operator[](Int_t i) const {
        return *static_cast<HMdcSizesCellsCell*>((*array)[i]);}

    Bool_t setSecTrans(Double_t corZ=0.);
    Bool_t fillLayerCont(const Double_t* corr=0);
    HMdcLayerGeomParLay* getLayerGeomParLay(void) {return pLayerGeomParLay;}
    HGeomVolume*         getGeomolume(void)       {return pGeomVol;}

    HMdcSizesCells* getSizesCells(void);
    Int_t    getSize(void) const;
    Short_t  getSec(void) const         {return sector;} 
    Short_t  getMod(void) const         {return module;} 
    Short_t  getLayer(void) const       {return layer;}  
    Double_t getCatDist(void) const     {return 2.*halfCatDist;}
    Double_t getHalfCatDist(void) const {return halfCatDist;}
    Double_t getPitch(void) const       {return pitch;}
    Double_t getHalfPitch(void) const   {return halfPitch;}
    Double_t getCosWireOr(void) const   {return cosWireOr;}
    Double_t getSinWireOr(void) const   {return sinWireOr;}
    Double_t getTanWireOr(void) const   {return sinWireOr/cosWireOr;}
    Double_t getWireOffSet(void) const  {return wireOffSet;}
    const HGeomTransform* getSecTrans(void) const      {return &sysRSec;}
    const HGeomTransform* getModTrans(void) const      {return &sysRMod;}
    const HGeomTransform* getRotLaySysRSec(void) const {return &rotLaySysRSec;}
    void transSecToRotLayer(HGeomVector& point) const;
    Int_t calcCellNum(Double_t x, Double_t y) const;
    Bool_t calcCrossedCells(Double_t x1,Double_t y1,Double_t z1,
        Double_t x2,Double_t y2,Double_t z2,Float_t& cell1,Float_t& cell2) const;
    Double_t getAlpha(const HGeomVector& p1, const HGeomVector& p2) const;
    Double_t getDist(const HGeomVector& p1, const HGeomVector& p2, Int_t cell);
    inline Double_t getDist(Double_t x1, Double_t y1, Double_t z1,
        Double_t x2, Double_t y2, Double_t z2, Int_t cell) const;
    inline void getYZinWireSys(Double_t x, Double_t y, Double_t z, Int_t cell,
        Double_t& yw, Double_t& zw) const;
    inline Double_t getDistToZero(Double_t y1,Double_t z1,
      Double_t y2, Double_t z2) const;
    inline Double_t getImpactToZero(Double_t y1,Double_t z1,
      Double_t y2, Double_t z2, Double_t &alpha) const;
    Double_t getImpact(Double_t x1,Double_t y1,Double_t z1,
        Double_t x2,Double_t y2,Double_t z2,Int_t cell,Double_t &alphaPerp) const;
    inline Double_t getImpact(const HMdcLineParam& ln, Int_t cell,
        Double_t &alpha) const;
    inline Double_t getImpact(const HMdcLineParam& ln, Int_t cell,
        Double_t &alpha, Double_t &y,Double_t &z,Double_t &dy,Double_t &dz) const;
    void getImpact(const HGeomVector& p1, const HGeomVector& p2, Int_t cell,
        Double_t &alpha, Double_t &per) const;
    Bool_t getImpact(Double_t x1,Double_t y1,Double_t z1,
        Double_t x2, Double_t y2, Double_t z2,
        Int_t cell, Double_t &alphaDrDist, Double_t &driftDist) const;
    inline Bool_t getImpact(const HMdcLineParam& ln, Int_t cell,
        Double_t &alpha, Double_t &minDist) const;
    inline Bool_t getImpact(const HMdcLineParam& ln, Int_t cell,
        Double_t &alpha, Double_t &minDist,
        Double_t &y,Double_t &z,Double_t &dy,Double_t &dz) const;
    Bool_t getDriftDist(Double_t xP1,Double_t yP1,Double_t zP1,
        Double_t xP2, Double_t yP2, Double_t zP2,
        Int_t cell, Double_t &alphaDrDist, Double_t &driftDist) const;
    Double_t getXSign(Double_t x1, Double_t y1, Double_t z1, 
        Double_t x2, Double_t y2, Double_t z2, Int_t cell) const;
    Double_t getXSign(const HMdcLineParam& ln, Int_t cell) const;
    Float_t getSignPath(Double_t x1,Double_t y1,Double_t z1,
      Double_t x2, Double_t y2, Double_t z2, Int_t cell) const;
    Double_t getSignPath(const HMdcLineParam& ln, Int_t cell) const;
    Float_t getSignPath(Double_t x1,Double_t y1,Double_t z1,
      Double_t x2, Double_t y2, Double_t z2, Int_t cell, Float_t& outside) const;
    inline void transTo(Double_t& x, Double_t& y, Double_t& z) const;
    inline void transFrom(Double_t& x, Double_t& y, Double_t& z) const;
    
    inline void rotVectToLay(Double_t xi,Double_t yi,Double_t zi,
        Double_t& xo, Double_t& yo, Double_t& zo) const;
    inline void rotVectToRotLay(Double_t xi,Double_t yi,Double_t zi,
        Double_t& xo, Double_t& yo, Double_t& zo) const;
    inline void rotYZtoRotLay(Double_t xi,Double_t yi,Double_t zi,
        Double_t& yo, Double_t& zo) const;
    inline void rotYZtoRotLay(const HGeomVector& dir,
        Double_t& yo, Double_t& zo) const;
    inline void rotVectToRotLay(const HGeomVector& dir,
        Double_t& xo, Double_t& yo, Double_t& zo) const;
    inline Double_t transXinRotLay(Double_t xi, Double_t yi) const;
    inline Double_t transYinRotLay(Double_t xi, Double_t yi) const;
    inline void transXYinRotLay(Double_t& x, Double_t& y) const;
    void print(void) const;
  protected:
    Bool_t getParamCont(void);

  ClassDef(HMdcSizesCellsLayer,0)
};


class HMdcSizesCellsMod : public HMdcPlane {
  friend class HMdcSizesCellsLayer;
  friend class HMdcSizesCellsSec;
  friend class HMdcSizesCells;
  protected:
    Short_t sector;            // Address
    Short_t module;            //
    HGeomTransform sysRSec;    // Transformation sector<->module
    Double_t tSysRSec[12];     // ---
    Double_t ct[6];            // Lookup table for funct. calcMdcHit, ...
    TObjArray* array;          // array of pointers of type HMdcSizesCellsLayer
    HMdcSizesCellsSec* pToSec; // Pointer to corresponding sector
  public:
    HMdcSizesCellsMod(HMdcSizesCellsSec* pSec, Int_t mod);
    ~HMdcSizesCellsMod(void);
    HMdcSizesCellsLayer& operator[](Int_t i) const {
        return *static_cast<HMdcSizesCellsLayer*>((*array)[i]);}
    Bool_t setSecTrans(const HGeomTransform * alignTrans=0,
        Int_t sysOfAlignTrans=0);
    
    Int_t   getSize(void) const;
    Short_t getSec(void) const {return sector;}
    Short_t getMod(void) const {return module;}
    HMdcSizesCells* getSizesCells(void);
    const HGeomTransform* getSecTrans(void) const { return &sysRSec; }
    inline void transTo(Double_t& x, Double_t& y, Double_t& z) const;
    inline void transFrom(Double_t& x, Double_t& y, Double_t& z) const;
    inline void transFromZ0(Double_t& x, Double_t& y, Double_t& z) const;
    inline void transFromZ0(Float_t& x, Float_t& y, Float_t& z) const;
    inline void rotVectTo(Double_t xi,Double_t yi,Double_t zi,
        Double_t& xo, Double_t& yo, Double_t& zo) const;
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
    const Double_t* getTfSysRSec(void) const      {return tSysRSec;}
    const Double_t* getMdcHitLookupTb(void) const {return ct;}
  protected:

  ClassDef(HMdcSizesCellsMod,0)
};


class HMdcSizesCellsSec : public TObject {
  friend class HMdcSizesCellsMod;
  friend class HMdcSizesCells;
  protected:
    Short_t sector;          // Address                                         
    TObjArray* array;        // HMdcSizesCellsMod array                         
    HGeomTransform sysRLab;  // Transformation sector<->lab.sys.     
    Bool_t* mdcStatSec;      // kTRUE - mdc exist 
    Int_t numTargets;        // number of targets
    HGeomVector* targets;    // targets in coor.sys. of sector
    HGeomVector targ3p[3];   // [0] First point of target in sector coor.sys.
                             // [1] Middle point of target in sector coor.sys.
                             // [2] Last point of target in sector coor.sys.
    HMdcSizesCells* pToSC;   // Pointer to HMdcSizesCells object
  public:
    HMdcSizesCellsSec(HMdcSizesCells* pSC, Int_t sec);
    ~HMdcSizesCellsSec(void);
    HMdcSizesCellsMod& operator[](Int_t i) const {
      return *static_cast<HMdcSizesCellsMod*>((*array)[i]);}
    Int_t getSize(void) const;
    const HGeomTransform* getLabTrans(void) const {return &sysRLab;}
    Bool_t modStatus(Int_t m) const {
      if(m<0 || m>3) return kFALSE;
      return mdcStatSec[m];
    }
    HMdcSizesCells* getSizesCells(void);
    const HGeomVector& getTargetFirstPoint(void) const  {return targ3p[0];}
    const HGeomVector& getTargetMiddlePoint(void) const {return targ3p[1];}
    const HGeomVector& getTargetLastPoint(void) const   {return targ3p[2];}
    void calcRZToTargLine(Double_t x1,Double_t y1,Double_t z1, 
        Double_t x2,Double_t y2,Double_t z2,  Double_t &zm,Double_t &r0) const;
    Int_t getNumOfTargets(void) const {return numTargets;}
    HGeomVector* getTargets(void) {return targets;}
    HGeomVector* getTarget(Int_t i) {
      return (i>=0 && i<numTargets) ? &(targets[i]):0;}

  ClassDef(HMdcSizesCellsSec,0)
};

class HMdcSizesCells : public TObject {
  friend class HMdcSizesCellsSec;
  friend class HMdcSizesCellsMod;
  friend class HMdcSizesCellsLayer;
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
    Int_t numTargets;                // number of targets
    HGeomVector* targets;            // targets
    HGeomVector targ3p[3];           // [0] First point of target in lab.sys.
                                     // [2] Middle point of target in lab.sys.
                                     // [3] Last point of targ3p in lab.sys.
  public:
    static HMdcSizesCells* getObject(void);
    static HMdcSizesCells* getExObject(void);
    static void deleteCont(void);
    HMdcSizesCellsSec& operator[](Int_t i) const {
      return *static_cast<HMdcSizesCellsSec*>((*array)[i]);}
    Int_t  getSize(void) const;
    Bool_t initContainer(void);
    Bool_t hasChanged(void) const {return changed;}
    void   clear(void);
    Bool_t getCellVol(Int_t sec,Int_t mod,Int_t lay,Int_t cell,
        HMdcTrap& volCell,Double_t sizeFactor=-1.) const;
    Char_t findReadOutSide(Int_t s,Int_t m,Int_t l,Int_t c) const;
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
    Bool_t fillModCont(Int_t sec, Int_t mod,
		       HGeomTransform * alignTrans=0, Int_t sysOfAlignTrans=0);
    Bool_t fillModCont(Int_t sec, Int_t mod, Double_t * corr);

    const HGeomVector& getTargetFirstPoint(void) const  {return targ3p[0];}
    const HGeomVector& getTargetMiddlePoint(void) const {return targ3p[1];}
    const HGeomVector& getTargetLastPoint(void) const   {return targ3p[2];}
    void calcRZToTargLine(Double_t x1,Double_t y1,Double_t z1, 
        Double_t x2,Double_t y2,Double_t z2,  Double_t &zm,Double_t &r0) const;
    static void calcRZToLineXY(Double_t x1, Double_t y1, Double_t z1, 
                               Double_t x2, Double_t y2, Double_t z2, 
        Double_t xBeam, Double_t yBeam, Double_t &zm, Double_t &r0);
    static void setTransform(Double_t* par, HGeomTransform& trans);
    Int_t getNumOfTargets(void) const {return numTargets;}
    HGeomVector* getTargets(void) {return targets;}
    static void copyTransfToArr(const HGeomTransform& trans, Double_t* arr);
  protected:
    HMdcSizesCells(void);
    ~HMdcSizesCells(void);
    Bool_t fillCont(Int_t sec);
    Bool_t fillTargetPos(HGeomVector* targetShift=0);

  ClassDef(HMdcSizesCells,0)
};

//----------------------- Inlines ------------------------------
inline void HMdcSizesCellsLayer::transTo(Double_t& x, Double_t& y, Double_t& z) const {
  // transform. point x,y,z from sector coor.sys. to layer coor.sys.
  rotVectToLay(x-tSysRSec[9],y-tSysRSec[10],z-tSysRSec[11],x,y,z);
}

inline void HMdcSizesCellsLayer::transFrom(Double_t& x, Double_t& y, Double_t& z) const {
  // transform. point x,y,z from layer coor.sys. to sector coor.sys.
  Double_t pmtx=x;
  Double_t pmty=y;
  Double_t pmtz=z;
  x=tSysRSec[0]*pmtx+tSysRSec[1]*pmty+tSysRSec[2]*pmtz+tSysRSec[ 9];
  y=tSysRSec[3]*pmtx+tSysRSec[4]*pmty+tSysRSec[5]*pmtz+tSysRSec[10];
  z=tSysRSec[6]*pmtx+tSysRSec[7]*pmty+tSysRSec[8]*pmtz+tSysRSec[11];
}

inline Double_t HMdcSizesCellsLayer::getDist(
    Double_t x1, Double_t y1, Double_t z1,
    Double_t x2, Double_t y2, Double_t z2, Int_t cell) const {
  // calc.  the minimal distance from line x1,y1,z1-x2,y2,z2  to wire.
  // x1,y1,z1,x2,y2,z2 - in sector coor.sys.
  Double_t y,z,dy,dz;
  rotYZtoRotLay(x1-tRLaySysRSec[9],y1-tRLaySysRSec[10],z1-tRLaySysRSec[11],y,z);
  y += wireOffSet - cell*pitch;
  rotYZtoRotLay(x2-x1,y2-y1,z2-z1,dy,dz);
  return fabs((y*dz-z*dy)/sqrt(dz*dz+dy*dy));
}

inline void HMdcSizesCellsLayer::getYZinWireSys(Double_t x, Double_t y,
      Double_t z, Int_t cell, Double_t& yw, Double_t& zw) const {
  // Output: yw,zw - in wire coor.sys.
  // Wire coor.sys. is line y=z=0, Y - along layer
  // x,y,z - in sector coor.sys.
  rotYZtoRotLay(x-tRLaySysRSec[9],y-tRLaySysRSec[10],z-tRLaySysRSec[11],yw,zw);
  yw += wireOffSet - cell*pitch;
}

inline Double_t HMdcSizesCellsLayer::getDistToZero(Double_t y1,Double_t z1,
    Double_t y2, Double_t z2) const {
  // calc. the minimal distance from line (y1,z1) - (y2,z2)  to poin
  // y=z=0 in plane y-z.
  Double_t dz=z1-z2;
  Double_t dy=y1-y2;
  Double_t lng=1./sqrt(dz*dz+dy*dy); //(dz*dz+dy*dy)=0 if tr.paral. to wire,only
  return fabs((z1*y2-z2*y1)*lng);
}

inline Double_t HMdcSizesCellsLayer::getImpactToZero(Double_t y1,Double_t z1,
    Double_t y2, Double_t z2, Double_t &alpha) const {
  // calc. the angle alpha (in degree) between line (y1,z1) - (y2,z2)
  // in Y-Z plane and Y axis and the minimal distance from this line
  // to poin y=z=0 in plane y-z.
  Double_t dz=z1-z2;
  Double_t dy=y1-y2;
  alpha=atan2(fabs(dz),fabs(dy))*TMath::RadToDeg();
  Double_t lng=1./sqrt(dz*dz+dy*dy); //(dz*dz+dy*dy)=0 if tr.paral. to wire,only
  return fabs((z1*y2-z2*y1)*lng);
}

inline void HMdcSizesCellsLayer::rotVectToLay(Double_t xi,Double_t yi,Double_t zi,
    Double_t& xo, Double_t& yo, Double_t& zo) const {
  // Input:  xi,yi,zi - sector coor.sys.
  // Output: xo,yo,zo - layer coor.sys.
  xo=tSysRSec[0]*xi+tSysRSec[3]*yi+tSysRSec[6]*zi;
  yo=tSysRSec[1]*xi+tSysRSec[4]*yi+tSysRSec[7]*zi;
  zo=tSysRSec[2]*xi+tSysRSec[5]*yi+tSysRSec[8]*zi;
}

inline void HMdcSizesCellsLayer::rotVectToRotLay(
    Double_t xi,Double_t yi,Double_t zi,
    Double_t& xo, Double_t& yo, Double_t& zo) const {
  // Input:  xi,yi,zi - sector coor.sys.
  // Output: xo,yo,zo - rotated layer coor.sys.
  xo=tRLaySysRSec[0]*xi+tRLaySysRSec[3]*yi+tRLaySysRSec[6]*zi;
  yo=tRLaySysRSec[1]*xi+tRLaySysRSec[4]*yi+tRLaySysRSec[7]*zi;
  zo=tRLaySysRSec[2]*xi+tRLaySysRSec[5]*yi+tRLaySysRSec[8]*zi;
}

inline void HMdcSizesCellsLayer::rotVectToRotLay(const HGeomVector& d,
    Double_t& xo, Double_t& yo, Double_t& zo) const {
  // Input:  xi,yi,zi - sector coor.sys.
  // Output: yo,zo - rotated layer coor.sys.
  xo=tRLaySysRSec[0]*d.getX()+tRLaySysRSec[3]*d.getY()+tRLaySysRSec[6]*d.getZ();
  yo=tRLaySysRSec[1]*d.getX()+tRLaySysRSec[4]*d.getY()+tRLaySysRSec[7]*d.getZ();
  zo=tRLaySysRSec[2]*d.getX()+tRLaySysRSec[5]*d.getY()+tRLaySysRSec[8]*d.getZ();
}

inline void HMdcSizesCellsLayer::rotYZtoRotLay(
    Double_t xi,Double_t yi,Double_t zi,Double_t& yo, Double_t& zo) const {
  // Input:  xi,yi,zi - sector coor.sys.
  // Output: yo,zo - rotated layer coor.sys.
  yo=tRLaySysRSec[1]*xi+tRLaySysRSec[4]*yi+tRLaySysRSec[7]*zi;
  zo=tRLaySysRSec[2]*xi+tRLaySysRSec[5]*yi+tRLaySysRSec[8]*zi;
}

inline void HMdcSizesCellsLayer::rotYZtoRotLay(const HGeomVector& d,
    Double_t& yo, Double_t& zo) const {
  // Input:  xi,yi,zi - sector coor.sys.
  // Output: yo,zo - rotated layer coor.sys.
  yo=tRLaySysRSec[1]*d.getX()+tRLaySysRSec[4]*d.getY()+tRLaySysRSec[7]*d.getZ();
  zo=tRLaySysRSec[2]*d.getX()+tRLaySysRSec[5]*d.getY()+tRLaySysRSec[8]*d.getZ();
}

inline Double_t HMdcSizesCellsLayer::transXinRotLay(Double_t xi, Double_t yi)
    const {
  // Input:  xi,yi - layer coor.sys.
  // Return X in  rotated layer coor.sys.
  return xi*cosWireOr+yi*sinWireOr;
}

inline Double_t HMdcSizesCellsLayer::transYinRotLay(Double_t xi, Double_t yi)
    const {
  // Input:  xi,yi - layer coor.sys.
  // Return Y in  rotated layer coor.sys.
  return yi*cosWireOr-xi*sinWireOr;
}

inline void HMdcSizesCellsLayer::transXYinRotLay(Double_t& x, Double_t& y)
    const {
  // Input:  xi,yi - layer coor.sys.
  // Return X,Y in  rotated layer coor.sys.
  Double_t xt=x;
  x=xt*cosWireOr+y*sinWireOr;
  y=y*cosWireOr-xt*sinWireOr;
}

inline Double_t HMdcSizesCellsLayer::getImpact(const HMdcLineParam& ln,
    Int_t cell,Double_t &alpha) const {
  // calc. the angle alpha (in degree) between projection of line
  // "ln" on the Y-Z plane and Y axis in coor.sys.
  // of wires and the minimal distance from "ln" to wire.
  // "ln" - in sector coor.sys.
  Double_t y,z,dy,dz;
  return getImpact(ln,cell,alpha,y,z,dy,dz);
}

inline Double_t HMdcSizesCellsLayer::getImpact(const HMdcLineParam& ln,
    Int_t cell,
    Double_t &alpha, Double_t &y,Double_t &z,Double_t &dy,Double_t &dz) const {
  // calc. the angle alpha (in degree) between projection of line
  // "ln" on the Y-Z plane and Y axis in coor.sys.
  // of wires and the minimal distance from "ln" to wire.
  // "ln" - in sector coor.sys.
  rotYZtoRotLay(ln.x1()-tRLaySysRSec[9],ln.y1()-tRLaySysRSec[10],
      ln.z1()-tRLaySysRSec[11],y,z);
  y += wireOffSet - cell*pitch;
  rotYZtoRotLay(ln.getDir(),dy,dz);
  alpha=atan2(fabs(dz),fabs(dy))*TMath::RadToDeg();
  return fabs((y*dz-z*dy)/sqrt(dz*dz+dy*dy));
}

inline Bool_t HMdcSizesCellsLayer::getImpact(const HMdcLineParam& ln,Int_t cell,
    Double_t &alpha, Double_t &minDist) const {
  // return kTRUE if line x1,y1,z1-x2,y2,z2 intersect cell or kFALSE
  // if not intersect and calc. the angle alpha (in degree) between projection
  // of the line  x1,y1,z1-x2,y2,z2 on the Y-Z plane and Y axis in coor.
  // sys. of wires and the minimal distance from line x1,y1,z1-x2,y2,z2
  // to wire.
  // x1,y1,z1,x2,y2,z2 - in sector coor.sys.
  Double_t y,z,dy,dz;
  return getImpact(ln,cell,alpha,minDist,y,z,dy,dz);
}

inline Bool_t HMdcSizesCellsLayer::getImpact(const HMdcLineParam& ln,Int_t cell,
    Double_t &alpha, Double_t &minDist,
    Double_t &y,Double_t &z,Double_t &dy,Double_t &dz) const {
  // return kTRUE if line x1,y1,z1-x2,y2,z2 intersect cell or kFALSE
  // if not intersect and calc. the angle alpha (in degree) between projection
  // of the line  x1,y1,z1-x2,y2,z2 on the Y-Z plane and Y axis in coor.
  // sys. of wires and the minimal distance from line x1,y1,z1-x2,y2,z2
  // to wire.
  // x1,y1,z1,x2,y2,z2 - in sector coor.sys.
  rotYZtoRotLay(ln.x1()-tRLaySysRSec[9],ln.y1()-tRLaySysRSec[10],
      ln.z1()-tRLaySysRSec[11],y,z);
  y += wireOffSet - cell*pitch;
  rotYZtoRotLay(ln.getDir(),dy,dz);
  
  Double_t lng=1./sqrt(dz*dz+dy*dy);
  Double_t yDist = fabs(y*dz-z*dy); // abs(Ywire-Ycross_track)=yDist/dz
  minDist = yDist*lng;
  Double_t dza=fabs(dz);
  Double_t dya=fabs(dy);
  alpha=atan2(dza,dya)*TMath::RadToDeg();
  if(minDist*dza*lng > halfPitch) {
    if(dya==0.0) return kFALSE;
    if((yDist-halfPitch*dza)/dya > halfCatDist)  return kFALSE;
  } else if(minDist*dya*lng > halfCatDist &&    // dya*lng = cos(alpha)
      (yDist-halfCatDist*dya)/dza > halfPitch) return kFALSE;
  return kTRUE;
}

inline void HMdcSizesCellsMod::transTo(Double_t& x, Double_t& y, Double_t& z)
    const {
  // transform. point x,y,z from sector coor.sys. to mdc coor.sys.
  rotVectTo(x-tSysRSec[ 9],y-tSysRSec[10],z-tSysRSec[11],x,y,z);
}

inline void HMdcSizesCellsMod::rotVectTo(Double_t xi,Double_t yi,Double_t zi,
    Double_t& xo, Double_t& yo, Double_t& zo) const {
  // Input:  xi,yi,zi - sector coor.sys.
  // Output: xo,yo,zo - module coor.sys.
  xo=tSysRSec[0]*xi+tSysRSec[3]*yi+tSysRSec[6]*zi;
  yo=tSysRSec[1]*xi+tSysRSec[4]*yi+tSysRSec[7]*zi;
  zo=tSysRSec[2]*xi+tSysRSec[5]*yi+tSysRSec[8]*zi;
}

inline void HMdcSizesCellsMod::transFrom(Double_t& x, Double_t& y, Double_t& z) const {
  // transform. point x,y,z from mdc coor.sys. to sector coor.sys.
  Double_t pmtx=x;
  Double_t pmty=y;
  Double_t pmtz=z;
  x=tSysRSec[0]*pmtx+tSysRSec[1]*pmty+tSysRSec[2]*pmtz+tSysRSec[ 9];
  y=tSysRSec[3]*pmtx+tSysRSec[4]*pmty+tSysRSec[5]*pmtz+tSysRSec[10];
  z=tSysRSec[6]*pmtx+tSysRSec[7]*pmty+tSysRSec[8]*pmtz+tSysRSec[11];
}

inline void HMdcSizesCellsMod::transFromZ0(Double_t& x,Double_t& y,Double_t& z) const {
  // transform. point x,y on the mdc plane (z=0) from mdc coor.sys. 
  // to sector coor.sys.
  Double_t pmtx=x;
  Double_t pmty=y;
  x=tSysRSec[0]*pmtx+tSysRSec[1]*pmty+tSysRSec[ 9];
  y=tSysRSec[3]*pmtx+tSysRSec[4]*pmty+tSysRSec[10];
  z=tSysRSec[6]*pmtx+tSysRSec[7]*pmty+tSysRSec[11];
}

inline void HMdcSizesCellsMod::transFromZ0(Float_t& x,Float_t& y,Float_t& z) const {
  // transform. point x,y on the mdc plane (z=0) from mdc coor.sys. 
  // to sector coor.sys.
  Double_t pmtx=x;
  Double_t pmty=y;
  x=tSysRSec[0]*pmtx+tSysRSec[1]*pmty+tSysRSec[ 9];
  y=tSysRSec[3]*pmtx+tSysRSec[4]*pmty+tSysRSec[10];
  z=tSysRSec[6]*pmtx+tSysRSec[7]*pmty+tSysRSec[11];
}

inline void HMdcSizesCellsMod::calcInterTrMod(Double_t x1, Double_t y1, Double_t z1,
      Double_t x2, Double_t y2, Double_t z2,
      Double_t& x, Double_t& y) const {
  Double_t dX=x2-x1;
  Double_t dY=y2-y1;
  Double_t dZ=z2-z1;
  Double_t del=1/(parA*dX+parB*dY+dZ);
  Double_t xt=(dX*(parD-z1-parB*y1)+x1*(parB*dY+dZ))*del-tSysRSec[ 9];
  Double_t yt=(dY*(parD-z1-parA*x1)+y1*(parA*dX+dZ))*del-tSysRSec[10];
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
  Double_t xt=(dX*(parD-z1-parB*y1)+x1*(parB*dY+dZ))*del-tSysRSec[ 9];
  Double_t yt=(dY*(parD-z1-parA*x1)+y1*(parA*dX+dZ))*del-tSysRSec[10];
  x=ct[0]*xt+ct[1]*yt+ct[2];
  y=ct[3]*xt+ct[4]*yt+ct[5];
  //---Hit direction----------------------------------------------------
  Double_t length=1/sqrt(dX*dX+dY*dY+dZ*dZ);
  xDir=(tSysRSec[0]*dX+tSysRSec[3]*dY+tSysRSec[6]*dZ)*length; // unit vector
  yDir=(tSysRSec[1]*dX+tSysRSec[4]*dY+tSysRSec[7]*dZ)*length;
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
