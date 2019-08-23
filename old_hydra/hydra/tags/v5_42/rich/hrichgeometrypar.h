#ifndef HRICHGEOMETRYPAR_H
#define HRICHGEOMETRYPAR_H
#pragma interface

#include "hparset.h"
#include "hrichparset.h"
#include "hlocation.h"
#include "hrichpad.h"
#include "hrichwire.h"
#include "hrichframe.h"
#include "hrichsectorpar.h"

class HParHadAsciiFileIo;

class HRichGeometryPar : public HRichParSet {
    
public:
    
    HRichGeometryPar();
    virtual ~HRichGeometryPar();
    
    Int_t getSectorsNr();
    Int_t getSector(Int_t sec);
    Int_t getColumns(); // pads in X
    Int_t getRows(); // pads in Y
    Int_t calcCol(Int_t padnr) { return (padnr % fColumns); }
    Int_t calcRow(Int_t padnr) { return (padnr / fColumns); }
    Int_t getPadsNr();
    Int_t getPadStatus(Int_t padnr);
    Float_t getDistWiresPads() { return fDistanceWiresPads; }
    Float_t getSectorShift() { return fSectorShift; }
    
    void setLab(Int_t nLab = 1) { fLab = nLab; }
    Bool_t IsLab() { return fLab; }
    
    
    virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
    virtual Bool_t writeAscii(HParHadAsciiFileIo *pHadAsciiFile);
    virtual void clear();
    
    
    HRichFrame* getFramePar() { return &fFrame; }
    HRichWireTab* getWiresPar() { return &fWires; }
    HRichPadTab* getPadsPar() { return &fPads; }
    
    void initParameters();
    
    
    ClassDef(HRichGeometryPar,1) //ROOT extension
	
	public:
    
    Int_t fSectorsNr;
    Int_t fSectorActive[6];
    Int_t fColumns;
    Int_t fRows;
    Int_t fLab;
    Float_t fDistanceWiresPads;
    Float_t fSectorShift; // this number corresponds to the shift in Y (lab coordinates) of the Rich volume in HGeant. The factor fSectorShift/cos(20) has to be added to the y coordinate in the detector ( pad plane) reference system in order to get the correct pad number and the correct theta. This correction takes place in the hrichdigitizer class.
    
    HRichSectorPar fSectors[6];
    HRichFrame fFrame;
    HRichWireTab fWires;
    HRichPadTab fPads;
    
    
};

#endif // HRICHGEOMETRYPAR_H
