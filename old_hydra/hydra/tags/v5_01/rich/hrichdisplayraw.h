// File: hrichdisplayraw.h
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/06/18 16:15:39
//Modified by Thomas Eberl: 99/08/30
//
#ifndef HRICHDISPLAYRAW_H
#define HRICHDISPLAYRAW_H
#include "hreconstructor.h"
#include "hades.h"
#include "hevent.h"
#include "haddef.h"
#include <TMapFile.h>
#include <TArrayS.h>
#include <TH2.h>
#include <TObjArray.h>
//#include "hrichdispcont.h"
#include <TMatrix.h>
#include <TVector.h>
class HCategory;
class HRichDisplayRaw : public HReconstructor  {
public:
    HRichDisplayRaw();
    HRichDisplayRaw(Text_t *name,Text_t *title);
    ~HRichDisplayRaw();
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
    HCategory* getRawCat(){return pRichRaw;}
    void setRawCat(HCategory* pRawCat){pRichRaw = pRawCat;}  
    Int_t producer(void);
    TMapFile * sharedFile;
    TH2F * richHisto[6][2];
    
    HCategory * pRichRaw;
public:
    ClassDef(HRichDisplayRaw,1)        // unpack RICH data
};
/* class HRichDispSector : public TObject{ */
/* private: */
/*     int num; */
/*     int charge[2][2]; */
/* public: */
/*     HRichDispSector(int i=0) : num(i) {}; */
/*     ~HRichDispSector() { }; */
/*     void setNum(int i) {num = i;} */
/*     int  getNum() {return num;} */
/*     void setCharge(int myCharge, int col, int row) { */
/* 	charge[col][row] = myCharge; */
/*     } */
/*     int getCharge(int col,int row){ */
/* 	return charge[col][row]; */
/*     } */
/* }; */
/* class HRichNamed : public TNamed { */
/* 	TObjArray * fArray; */
/* }; */
#endif /* !HRICHDISPLAYRAW_H */












