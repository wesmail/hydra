#ifndef HRICHGEOMETRYPAR_H
#define HRICHGEOMETRYPAR_H
#pragma interface

#include "hparset.h"
#include "hrichparset.h"
#include "hlocation.h"
#include "hrichframe.h"
#include "hrichpad.h"
#include "hrichwire.h"

class HParHadAsciiFileIo;

class HRichGeometryPar : public HRichParSet
{
    
public:
    
   HRichGeometryPar(const Char_t* name    = "RichGeometryParameters",
		    const Char_t* title   = "Rich Geometry Parameters",
		    const Char_t* context = "");
   virtual ~HRichGeometryPar();
    
   Int_t   getSectorsNr()            { return fSectorsNr;         }
   Int_t   getColumns()              { return fColumns;           } // pads in X
   Int_t   getRows()                 { return fRows;              } // pads in Y
   Int_t   calcCol(Int_t padnr)      { return (padnr % fColumns); }
   Int_t   calcRow(Int_t padnr)      { return (padnr / fColumns); }
   Int_t   getPadsNr()               { return (fColumns * fRows); }
   Int_t   getSectorActive(Int_t s)  { return (s >= 0 && s < 6) ? fSectorActive[s] : -1; }
   Int_t   getPadStatus(Int_t padnr) { return fPads.getPad(padnr)->getPadActive(); }
   Float_t getSectorShift()          { return fSectorShift;       }
   Float_t getDistanceWiresPads()    { return fDistanceWiresPads; }
   Float_t getSectorPhi(Int_t s)     { return (s >= 0 && s < 6) ? fSectorPhi[s] : -1; }

   void setSectorActive(Int_t s, Int_t i) { fSectorActive[s] = i;}
   void setColumns(Int_t c)               { fColumns = c;}
   void setRows(Int_t r)                  { fRows = r;   }
   void setSectorPhi(Int_t s, Float_t phi){ fSectorPhi[s] = phi; }
   void setSectorShift(Float_t shift)     { fSectorShift = shift; }
   void setLab(Int_t nLab = 1)            { fLab = nLab; }
   void setDistanceWiresPads(Float_t dis) { fDistanceWiresPads = dis; }
   Bool_t IsLab( void )                   { return fLab; }
   Int_t incrementSectorNr()               { return ++fSectorsNr; }
    
    
   virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
   virtual Bool_t writeAscii(HParHadAsciiFileIo *pHadAsciiFile);
   virtual void clear() {};
    
    
   HRichFrame*   getFramePar() { return &fFrame; }
   HRichWireTab* getWiresPar() { return &fWires; }
   HRichPadTab*  getPadsPar()  { return &fPads;  }
    
   void initParameters();
    
    
   ClassDef(HRichGeometryPar,1) //ROOT extension
	
private:
    
   Int_t   fSectorsNr;
   Int_t   fSectorActive[6];
   Int_t   fColumns;
   Int_t   fRows;
   Int_t   fLab;
   Float_t fDistanceWiresPads;
   Float_t fSectorPhi[6];
   Float_t fSectorShift; // this number corresponds to the shift in Y (lab coordinates)
                         // of the Rich volume in HGeant. The factor fSectorShift/cos(20)
                         // has to be added to the y coordinate in the detector ( pad plane)
                         // reference system in order to get the correct pad number
                         // and the correct theta. This correction takes place in the
                         // hrichdigitizer class.

   HRichFrame     fFrame;
   HRichWireTab   fWires;
   HRichPadTab    fPads;
    
    
};

#endif // HRICHGEOMETRYPAR_H
