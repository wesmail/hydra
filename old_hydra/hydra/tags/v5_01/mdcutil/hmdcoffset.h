#ifndef HMDCOFFSET_H
#define HMDCOFFSET_H

#include "hreconstructor.h"

#define HEX_NAN 0x7fc00000

typedef Int_t   MyInt;
typedef Float_t MyFloat;
typedef TH1S    MyHist;

class TFile;
class HCategory;
class HIterator;
class HMdcCalPar;
class HMdcCalParRaw;
class HMdcLookupGeom;

typedef MyInt MyField[6][4][16][96][2048];

class HMdcOffset : public HReconstructor {
protected:
    static const Int_t nbin;         //! Number of bins ih the histogramms
    static const Int_t nbinm1;       //! nbin - 1
    static const Int_t nbinp1;       //! nbin + 1
    static const Int_t nSubEvents;   //! The max. number of objects per event

    HCategory     *rawCat;           //! pointer to the raw data
    HIterator     *iter;             //! iterator on raw data
    HMdcCalParRaw *calparraw;        //! pointer to HMdcCalParRaw container

    Bool_t isPulserFile;             //! flag for a external calibration file (pulser)
    
    Char_t *fNameAscii;              //! file name of ascii output file
    Char_t *fNameRoot;               //! file name of root output file

    Float_t minfitthreshold;         //! minimum threshold for linear fit (y-range)
    Float_t maxfitthreshold;         //! maximum threshold for linear fit (y-range)

    Int_t offsetfitNoise;            //! offset of the fit range for the subtraction of the noise (start at yequalzero going to the left)
    Int_t  widthfitNoise;            //! width of the fit range

    Int_t rangeGauss;                //! fit range of the gaussian fit around calculated offset

    MyField *hreverse;               //! array for the drift-time (Time1) to fill in eventloop
    MyField *hintegral;              //! array for the integrated drift-time

    MyHist *hinv;                    //!  drift-time histograms to hold collected data
    MyHist *hint;                    //!  integrated drift-time histograms
    
    MyFloat yequalzero;              //! intersection point of the first  fit and the x-axis
    MyFloat crosspointX;             //! x-value of the intersection point of the two linear fits->offset

    MyFloat fitpar0;                 //! first fit parameter of the first linear fit
    MyFloat fitpar0error;            //! error of the first fit parameter of the first linear fit
    MyFloat fitpar1;                 //! second fit parameter of the first linear fit
    MyFloat fitpar1error;            //! error of the second fit parameter of the first linear fit

    MyFloat fitparNoise0;            //! first fit parameter of the second linear fit
    MyFloat fitparNoise0error;       //! error of the first fit parameter of the second linear fit
    MyFloat fitparNoise1;            //! second fit parameter of the second linear fit
    MyFloat fitparNoise1error;       //! error of the second fit parameter of the second linear fit
    Float_t totalsigma;              //! sigma of the offset calculated from the sigmas of the two linear fits


    MyFloat fitGaussMean;            //! mean of the gaussian fit
    MyFloat fitGaussSigma;           //! sigma of the gaussian fit

public:
    HMdcOffset();
    HMdcOffset(Text_t* name,Text_t* title);
    ~HMdcOffset();
    
    Bool_t init();
    Bool_t finalize();
    Int_t  execute();

    void setOutputAscii(Char_t*);
    void setOutputRoot (Char_t*);

    void setPulserFile()          {isPulserFile=kTRUE;}
    
    void setNoiseOffset(Int_t i)    {offsetfitNoise =i;}
    void setNoiseWidth (Int_t i)    {widthfitNoise  =i;}
    void setThresholdMin(Float_t f) {minfitthreshold=f;}
    void setThresholdMax(Float_t f) {maxfitthreshold=f;}
    void setNoise(Int_t o, Int_t w)   {offsetfitNoise=o; widthfitNoise=w;}
    void setThreshold(Float_t min, Float_t max) {minfitthreshold=min; maxfitthreshold=max;}
    void setRangeGauss(Int_t i)     {rangeGauss=i;}
    
protected:
    Bool_t isNaN(MyFloat f) {return *((long int*)&f)==HEX_NAN;}
    void setDefault();
    ofstream *openAsciiFile();

    TDirectory *Mkdir(TDirectory *, Char_t *, Int_t, Int_t p=1);
    void createHist(TFile*,Int_t, Int_t, Int_t, Int_t);
    void fillHist  (Int_t, Int_t, Int_t, Int_t);
    Int_t fitHist   (Int_t, Int_t, Int_t, Int_t);
    void writeAscii(ofstream&, Int_t, Int_t, Int_t, Int_t);
    void writeHist (TFile*);
    void deleteHist();

public: // This has to be placed at the end (--> root dox)
    ClassDef(HMdcOffset, 0)  //! class for the calculation of the offsets per tdc channel
};

#endif /* !HMDCOFFSET_H */

