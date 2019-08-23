//$Id: hrichcal.h,v 1.9 2003-01-24 18:00:54 halo Exp $
// Last update by Thomas Eberl: 02/10/21 14:09:10
//

#ifndef HRICHCAL_H
#define HRICHCAL_H
#pragma interface
using namespace std;
#include "hlocateddataobject.h"
#include <iostream> 
#include <iomanip>

class HRichCal : public HLocatedDataObject {

public:

    HRichCal() : fCharge(0.0F),fSector(-1),fRow(-1),fCol(-1){}
    HRichCal(const Float_t q) :
        fCharge(q),fSector(-1),fRow(-1),fCol(-1) {}
    
    virtual ~HRichCal() {}
    
        Int_t clear() {
	    fCharge = 0.0F;
	    fSector = -1;
	    fRow = fCol = -1;
	    return 1;
	}
    
    Float_t getCharge() const { return fCharge; }
    Float_t addCharge(Float_t q) { return fCharge += q; }
    void setCharge(Float_t q) { fCharge = q; }
    friend ostream& operator<< (ostream& output, HRichCal& element);
    
    Int_t getSector() { return fSector; }
    Int_t getRow() { return fRow; }
    Int_t getCol() { return fCol; }
    Int_t getAddress() {
	return 10000 * (fSector ? fSector : 6) + 100 * fRow + fCol; }
    
    void setSector(Int_t s) { fSector = s; }
    void setRow(Int_t r) { fRow = r; }
    void setCol(Int_t c) { fCol = c; }
    
    void setEventNr(Int_t lNr) { fEventNr = lNr; }
    Int_t getEventNr() { return fEventNr; }
    
    Int_t getNLocationIndex() { return 3; }
    Int_t getLocationIndex(Int_t i);
    // the class is sortable by the charge
    Bool_t IsSortable() const {return kTRUE;}
    Int_t  Compare(const TObject *obj) const {
       // HRichTrack is sorted by the pad charge in decreasing order
       if (fCharge == ((HRichCal*)obj)->fCharge) return 0;
       else if (fCharge < ((HRichCal*)obj)->fCharge) return 1;
            else return -1;
    }
    void dumpToStdout();

private:

        Int_t fEventNr;
        Float_t fCharge; 
        short fSector;
        char fRow;
        char fCol; 

        ClassDef(HRichCal,3) //Rich cal data
};

#endif

