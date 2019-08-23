// $Id: hrichcal.h,v 1.12 2008-11-25 11:02:27 jurkovic Exp $
// Last update by Thomas Eberl: 02/10/21 14:09:10
//

#ifndef HRICHCAL_H
#define HRICHCAL_H
#pragma interface

using namespace std;

#include "hlocateddataobject.h"

#include <iostream> 
#include <iomanip>

class HRichCal : public HLocatedDataObject 
{
private:
   Float_t fCharge; 
   Int_t   fSector;
   Int_t   fRow;
   Int_t   fCol; 
   Int_t   fEventNr;
   Bool_t  isCleanedSingle;
   Bool_t  isCleanedHigh;

public:

   HRichCal() :
     fCharge(0.0),
     fSector(-1),
     fRow(-1),
     fCol(-1),
     fEventNr(-1),
     isCleanedSingle(0),
     isCleanedHigh(0) {}

   HRichCal(const Float_t q) :
     fCharge(q),
     fSector(-1),
     fRow(-1),
     fCol(-1),
     fEventNr(-1),
     isCleanedSingle(0),
     isCleanedHigh(0) {}

   virtual ~HRichCal() {}
    
   Int_t clear()
   {
     fCharge         = 0.0;
     fSector         = -1;
     fRow            = -1;
     fCol            = -1;
     fEventNr        = -1;
     isCleanedSingle = 0;
     isCleanedHigh   = 0;
     return 1;
   }

   ///////////////////////////////////////////////////////////////////
   // GETTERS
   Float_t getCharge(void)         const { return fCharge; }
   Int_t   getSector(void)         const { return fSector; }
   Int_t   getRow(void)            const { return fRow; }
   Int_t   getCol(void)            const { return fCol; }
   Int_t   getNLocationIndex(void) const { return 3; }
   Int_t   getEventNr(void)        const { return fEventNr; }
   Int_t   getAddress(void)        const { return 10000 * (fSector ? fSector : 6) + 100 * fRow + fCol; }
   // the class is sortable by the charge
   Bool_t  IsSortable(void)        const { return kTRUE; }
   Bool_t  getIsCleanedSingle(void)const { return isCleanedSingle; }
   Bool_t  getIsCleanedHigh(void)  const { return isCleanedHigh; }
   inline Int_t getLocationIndex(Int_t i);

   ///////////////////////////////////////////////////////////////////
   // SETTERS
   void setCharge(Float_t q)         { fCharge         = q; }
   void setSector(Int_t s)           { fSector         = s; }
   void setRow(Int_t r)              { fRow            = r; }
   void setCol(Int_t c)              { fCol            = c; }
   void setIsCleanedSingle(Bool_t a) { isCleanedSingle = a; }
   void setIsCleanedHigh(Bool_t b)   { isCleanedHigh   = b; }
   void setEventNr(Int_t e)          { fEventNr        = e; }

   // HRichTrack is sorted by the pad charge in decreasing order
   Int_t  Compare(const TObject *obj) const 
   {
     if ( fCharge == ((HRichCal*)obj)->fCharge) return 0;
     if ( fCharge <  ((HRichCal*)obj)->fCharge) return 1;
     else return -1;
   }

   void    dumpToStdout();
   Float_t addCharge(Float_t q) { return fCharge += q; }
   friend  ostream& operator << (ostream& output, HRichCal& element);

   ClassDef(HRichCal, 4) //Rich cal data
};

inline Int_t HRichCal::getLocationIndex(Int_t i) {
  switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getRow();    break;
  case 2 : return getCol();    break;
  }
  return -1;
}


#endif

