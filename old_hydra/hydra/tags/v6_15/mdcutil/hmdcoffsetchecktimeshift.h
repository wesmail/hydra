#ifndef HMDCOFFSETCHECKTIMESHIFT_H
#define HMDCOFFSETCHECKTIMESHIFT_H

#include "hreconstructor.h"
#include "hstartdef.h"

#define HEX_NAN 0x7fc00000
// tetetstetetetettete
typedef Int_t   MyInt;
typedef Float_t MyFloat;
typedef TH1F    MyHist;

class TFile;
class HCategory;
class HIterator;
class HMdcCalPar;
class HMdcCalParRaw;
class HMdcLookupGeom;
class HMdcTimeCut;

//typedef MyInt MyField[6][4][16][96][2048];
//typedef MyInt MyFieldMbo[6][4][16][20][2048];
  typedef MyInt       MyFieldMdc[6][4][20][50][2048];

  //  typedef MyInt       MyFieldMdcSingle[2048];


class HMdcOffsetCheckTimeShift : public HReconstructor {
protected:
    static const Int_t nbin;               // Number of bins ih the histogramms
    static const Int_t nbinm1;             // nbin - 1
    static const Int_t nbinp1;             // nbin + 1
    static const Int_t nSubEvents;         // The max. number of objects per event
    HCategory     *rawCat;                 // pointer to the raw data
    HCategory* calStartCat;       //! pointer to the cal data
    HIterator     *iter;                   // iterator on raw data
    HIterator* iter_start;         //! iterator on start cal data.
    HMdcCalParRaw *calparraw;              // pointer to HMdcCalParRaw container
    HMdcTimeCut *timecut;              // pointer to HMdcTimeCut container
    HMdcLookupGeom *lookupgeom;              // pointer to HMdcLookupGeom container

    
    Int_t eventcounter;    // counts number of calculated events of all files
    Int_t filecounter;     // counts the number of files
    Int_t skipcount;       // counts the number of events
    Int_t numberofevents;  // counts the number of events which are collected in each step
    Int_t step;            // counts the number of step of stepsize in same file
    Int_t stepsize;        // number of events between the hists in same file
    Bool_t isPulserFile;   // flag for a external calibration file (pulser)
    Bool_t noStart;        // flag if starttime should not be used
    Bool_t useTimeCut;     // flag if time cuts should be perormed.
    Char_t filenames[50][200];// Array to store the names of the anaylyzed files
    Char_t *fNameAsciiOffset;                    // file name of ascii output file
    Char_t *fNameRootOffset;                     // file name of root output file

    Float_t minfitthreshold;               // minimum threshold for linear fit (y-range)
    Float_t maxfitthreshold;               // maximum threshold for linear fit (y-range)

    Int_t offsetfitNoise;                  // offset of the fit range for the subtraction of the noise (start at yequalzero going to the left)
    Int_t  widthfitNoise;                  // width of the fit range

    Int_t rangeGauss;                      // fit range of the gaussian fit around calculated offset

    MyFieldMdc *hreverse;                     // array for the drift-time (Time1) to be filled in eventloop
    MyFieldMdc *hintegral;                    // array for the integrated drift-time

    MyHist *hinv;                          //  drift-time histograms to hold collected data
    MyHist *hint;                          //  integrated drift-time histograms
    
    MyFloat yequalzero;                    // intersection point of the first  fit and the x-axis
    MyFloat crosspointX;                   // x-value of the intersection point of the two linear fits->offset

    MyFloat fitpar0;                       // first fit parameter of the first linear fit
    MyFloat fitpar0error;                  // error of the first fit parameter of the first linear fit
    MyFloat fitpar1;                       // second fit parameter of the first linear fit
    MyFloat fitpar1error;                  // error of the second fit parameter of the first linear fit

    MyFloat fitparNoise0;                  // first fit parameter of the second linear fit
    MyFloat fitparNoise0error;             // error of the first fit parameter of the second linear fit
    MyFloat fitparNoise1;                  // second fit parameter of the second linear fit
    MyFloat fitparNoise1error;             // error of the second fit parameter of the second linear fit
    MyFloat totalsigma;                    // sigma of the offset calculated from the sigmas of the two linear fits


    MyFloat fitGaussMean;                  // mean of the gaussian fit
    MyFloat fitGaussSigma;                 // sigma of the gaussian fit

public:
    HMdcOffsetCheckTimeShift();
    HMdcOffsetCheckTimeShift(Text_t* name,Text_t* title);
    ~HMdcOffsetCheckTimeShift();
    
    Bool_t init();
    Bool_t reinit();
    Bool_t finalize();
    Int_t  execute();

    void setOutputAscii(Char_t*);
    void setOutputRoot (Char_t*);

    void setPulserFile()          {isPulserFile=kTRUE;}
    void setNoStart()             {noStart=kTRUE;}
    void setTimeCut(Bool_t cut)   {useTimeCut=cut;}

    void setStepSize(Int_t i)     {stepsize =i;}
    void setNumberOfEvents(Int_t i)     {numberofevents =i;}
   
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
    Float_t getstarttime();
    void writeAscii(ofstream&, Int_t, Int_t, Int_t, Int_t);
    void writeHist (TFile*);
    void deleteHist();

public: // This has to be placed at the end (--> root dox)
    ClassDef(HMdcOffsetCheckTimeShift, 0)  // class for the calculation of the offsets per tdc channel
};

#endif /* !HMDCOFFSETCHECKTIMESHIFT_H */

