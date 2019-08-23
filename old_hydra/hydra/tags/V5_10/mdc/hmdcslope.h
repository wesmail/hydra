#ifndef HMDSLOPE_H
#define HMDSLOPE_H

#include "hreconstructor.h"

typedef Int_t   MyInt;
typedef Float_t MyFloat;

class TFile;
class HCategory;
class HIterator;
class HMdcCalPar;
class HMdcCalParRaw;

typedef MyFloat MyFloatField[6][4][16][96];
typedef MyInt   MyIntField  [6][4][16][96];

class HMdcSlope : public HReconstructor {

protected:
    HCategory     *rawCat;     //! pointer to the raw data
    HCategory     *calCat;     //! pointer to the calibrated data
    HIterator     *iter;       //! iterator on raw data
    HMdcCalParRaw *calparraw;  //! calibration parameters on raw level
    
    Char_t *fNameRoot;         //! file name of root output file

    MyFloatField *avgSlope;
    MyFloatField *avgSlopeErr;
    MyIntField   *nEvt;

    Int_t binError;
    
    TFile *file;

    TH1F  *hSlope;
    TH1F  *hTime;

public:
    HMdcSlope();
    HMdcSlope(Text_t* name,Text_t* title);
    ~HMdcSlope();

    Bool_t init();
    Bool_t finalize();
    Int_t  execute();


//    void setOutputAscii(Char_t*);
    void setOutputRoot (Char_t*);

protected:
    void calc(Int_t, Int_t, Int_t, Int_t, Int_t, Int_t, Float_t, Float_t*, Float_t*);
 
public: // This has to be placed at the end (--> root docs)
    ClassDef(HMdcSlope, 0) //! Class calculates slopes and errors for TDC channels from internal calibration.
};

#endif /* !HMDCSLOPE_H */







