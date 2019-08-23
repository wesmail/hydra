#ifndef HShowerGeometry_H
#define HShowerGeometry_H

#include "hdetgeompar.h"
#include "hgeomvector2.h"
#include "hparset.h"
#include "hshowerparset.h"
#include "hlocation.h"
#include "hshowerpad.h"
#include "hshowerwire.h"
#include "hshowerframe.h"
#include "hshowersectorgeom.h"
#include "hshowermodulegeom.h"
class HGeomVolume;
class HShowerGeometry: public HDetGeomPar{
public:
	HShowerGeometry(const char* name="ShowerGeometry",
					const char* title="Shower geometry parameters",
					const char* context="GeomProduction");
	~HShowerGeometry();					
	Bool_t init(HParIo*,Int_t*);
	Int_t write(HParIo* output);
	Int_t getSecNum(const TString&);
	Int_t getModNumInMod(const TString&);
	Int_t getModNumInComp(const TString&);
	Int_t getCompNum(const TString&) {return 0;}
	
	Int_t getSectors(){return m_nSectors;}
  Int_t getModules(){return m_nModules;}
  Int_t getRows(){return m_nRows;}
  Int_t getColumns(){return m_nColumns;}
  void setLab(Int_t nLab = 1){m_nLab = nLab;}
  Bool_t IsLab(){return m_nLab;}
	virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
	virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);
	virtual void clear();

  HShowerFrame* getFrame(Int_t n){return &frames[n];}
  HShowerWireTab* getWireTab(Int_t nMod){return &wires[nMod];}
  HShowerPadTab* getPadParam(Int_t nMod){return &pads[nMod];}
  void attachTables();

  void getLocalCoord(HLocation& loc, HGeomVector& v);
  void getLabCoord(HLocation& loc, HGeomVector2& v);
  void getSphereCoord(HLocation& loc, HGeomVector2& v, HGeomVector *pvTarget = NULL);

  const HGeomTransform&  getTransform(Int_t sect);
  const HGeomTransform&  getTransform(Int_t sect, Int_t mod);

  void translModules();
  void rotateSectors();
  Bool_t transToLab();
	Bool_t transPadToLab();
	Bool_t updateDetGeom();
private:
	
	Bool_t initDetGeom();
	Bool_t writeModGeomParAscii(
			HParHadAsciiFileIo* pHadAsciiFile);
	Bool_t writeCompositeVolAscii(
			HParHadAsciiFileIo* pHadAsciiFile);
	Bool_t writeGeomVolAscii(
			HParHadAsciiFileIo* pHadAsciiFile,HGeomVolume *p);
	Bool_t initModGeomParAscii(
			HParHadAsciiFileIo* pHadAsciiFile);
	Bool_t initCompositeVolAscii(
			HParHadAsciiFileIo* pHadAsciiFile);
	Bool_t initGeomVolAscii(
			HParHadAsciiFileIo* pHadAsciiFile,HGeomVolume *p);
public:
	Int_t m_nSectors;           //number of sectors  
  Int_t m_nModules;           //number of modules
  Int_t m_nRows;              //number of rows
  Int_t m_nColumns;           //number of columns
  Int_t m_nLab;         //determinate if lab coordinates have been calculated       

  HShowerSectorGeom sectors[6]; //definitions of coordinates in lab

  HShowerFrame frames[3];   //definitions of local coordinates of frames
  HShowerWireTab wires[3];  //definitions of local coordinates of wires
  HShowerPadTab pads[3];    //definitions of local coordinates of pads

	
ClassDef(HShowerGeometry,2)//Class for PreShower geometry container	
};
#endif /* !HShowerGeometry_H */
