// File: hrichdisplaycal.h
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/06/18 16:15:39
//Modified by Thomas Eberl: 99/08/30
//
#ifndef HRICHDISPLAYCAL_H
#define HRICHDISPLAYCAL_H
#include "hreconstructor.h"
#include "hades.h"
#include "hevent.h"
#include "haddef.h"
#include "TMapFile.h"
#include "TArrayS.h"
#include "TH2.h"
#include "TObjArray.h"
//#include "hrichdispcont.h"
#include "TMatrix.h"
#include "TVector.h"
class HCategory;
class HRichDisplayCal : public HReconstructor  {
public:
    HRichDisplayCal();
    HRichDisplayCal(const Text_t *name,const Text_t *title);
    ~HRichDisplayCal();
    Bool_t init(void);
    Bool_t finalize(void);
    Int_t execute(void);
    TObjArray * myArray;
    TObjArray a;
    TMatrix * pMat;
private:
    Int_t ncounter;
    TVector* counter;
    Int_t sectorNum;
    Bool_t display(HLocation &refLoc);
    HCategory* getCalCat(){return pRichCal;}
    void setCalCat(HCategory* pCalCat){pRichCal = pCalCat;}  
    Int_t producer(void);
    TMapFile * sharedFile;
    TH2F * richHisto[6][2];
    
    HCategory * pRichCal;//!
public:
    ClassDef(HRichDisplayCal,1)        // unpack RICH data
};
#endif /* !HRICHDISPLAYCAL_H */












