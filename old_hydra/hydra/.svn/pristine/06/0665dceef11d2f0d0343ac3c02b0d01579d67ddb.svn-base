// @(#)$Id: hshowercalibraterIPU.h,v 1.2 2002-10-28 14:42:38 jaskula Exp $
//*-- Authors : Marcin Jaskula, Jerzy Pietraszko 11/10/2002
//            : code taken from HShowerCalibrator

#ifndef HShowerCalibraterIPU_H
#define HShowerCalibraterIPU_H

// -----------------------------------------------------------------------------

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hshowerraw.h"

// -----------------------------------------------------------------------------

class HCategory;
class HIterator;

// -----------------------------------------------------------------------------

class HShowerCalibraterIPU : public HReconstructor
{
public:
    HShowerCalibraterIPU();
    HShowerCalibraterIPU(Text_t *name,Text_t *title);
   ~HShowerCalibraterIPU();

    Bool_t init(void);
    Bool_t reinit(void);
    Bool_t finalize(void);
    Int_t execute(void);
    Bool_t calibrate(HShowerRaw *raw);
    HShowerCalibraterIPU &operator=(HShowerCalibraterIPU &c);

    HCategory* getRawCat(){return m_pRawCat;}
    HCategory* getCalCat(){return m_pCalCat;}
    HParSet* getCalPar(){return m_pCalPar;}

    void setRawCat(HCategory* pRawCat){m_pRawCat = pRawCat;}
    void setCalCat(HCategory* pCalCat){m_pCalCat = pCalCat;}
    void setCalPar(HParSet*);

    void initCalPar();

private:
    HLocation m_zeroLoc;
    HLocation m_loc; //Location of object being calibrated
    HCategory *m_pRawCat; //!Pointer to the raw data category
    HCategory *m_pCalCat; //!Pointer to the cal data category

    HParSet *m_pCalPar; //!Pointer to the cal data category

    HIterator *fIter;  //!Iterator for fired pads

    // -------------------------------------------------------------------------

    ClassDef(HShowerCalibraterIPU, 0) //!ROOT extension
};

// -----------------------------------------------------------------------------

#endif
