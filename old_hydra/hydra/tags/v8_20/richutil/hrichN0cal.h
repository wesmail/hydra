// File: hrichN0cal.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 02/08/21 20:35:41
//
#ifndef HRICHN0CAL_H
#define HRICHN0CAL_H
#include "hreconstructor.h"
#include "hlocation.h"
#include <stdlib.h>
#include "TH1.h"
#include "TFile.h"

class HCategory;
class HIterator;

class HRichN0Cal : public  HReconstructor {
public:
    HRichN0Cal(Text_t *name, Text_t *title,Text_t *outName,Char_t *typ);   
    ~HRichN0Cal();
    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    Int_t filterSector(Int_t sec,Float_t multSec);
    HCategory* getLocalCat(){ return fLocalCat;}
    HCategory* getLocalHrCat(){ return fLocalCatHr;}
    ClassDef(HRichN0Cal, 1)
	
protected:
    Int_t secFlag[6];//!
    Int_t cutSecD[6];//!
    Int_t cutSecU[6];//!
    Int_t secOk;//!
    Float_t slopeBG[4];//!
    Float_t offSetBG[4];//!    
    Float_t ring1ThMin,ring1ThMax,ring2ThMin,ring2ThMax;
    TFile * outFile;//!
    TH1F*** histo;//!
    TH1F ** thetaH;//!
    Int_t countEvt[6];//!
    HCategory *fLocalCat; //!
    HCategory *fLocalCatHr; //!
    HIterator *fIter1; //!
    HIterator * fIter2; //!
};
#endif /* !HRICHN0CAL_H */




