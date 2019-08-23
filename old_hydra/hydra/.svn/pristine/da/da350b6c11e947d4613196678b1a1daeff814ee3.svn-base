#ifndef HMDCWIREDATA_H
#define HMDCWIREDATA_H

#include <TObject.h>
#include "hsymmat.h"
#include "hmdcsizescells.h"
#include <TMatrixD.h>
#include "hmdclistgroupcells.h"
#include "hlocation.h"

class HMdcCal1;
class HMdcCal2Par;
class HMdcTrackParam;
class HMdcWireFit;
class HMdcTrackFitInOut;
class HMdcClus;
class HMdcCal1Sim;
class HMdcClusFit;
class HMdcEvntListCellsAndTimes;
class HGeantMdc;
class HMdcDriftTimePar;
class HMdcDriftTimeParBin;
 
class HMdcWireData : public TObject {
  // One wire data for track fitter:
  protected:
    // Input data:
    Int_t  sector;             // MDC sector
    Int_t  module;             // MDC module
    Int_t  layer;              // MDC layer
    Int_t  cell;               // MDC cell
    Int_t  timeIndx;           // =1->time1, =2->time2 in HMdcCal1
    Double_t tdcTimeCal1;      // drift time from HMdcCal1
    HMdcCal1* fMdcCal1;        // pointer to the corresponding HMdcCal1 object

    // Fit var. and output data:
    Double_t signalTimeOffset; // signal time offset (signal_path/signal_speed)
    Double_t tdcTime;          // = tdcTimeCal1 - signal_time_offset
    Double_t errTdcTime;       // drift time error
    Double_t driftTime;        // calculated drift time
    Double_t dev0;             // = driftTime - tdcTime
    Double_t dev;              // deviation = driftTime + time_offset - tdcTime
    Double_t chi2;             // deviation^2 = (dev/errTdcTime)^2
    Double_t distance;         // distance from track to wire
    Double_t alpha;            // impact angle of track (see HMdcCal2ParSim)
    Bool_t   inCell;           // =kTRUE - track cross this cell
    Double_t weight;           // weight of this hit in fit
    Bool_t   isWireUsed;       // kTRUE if wires is used on final fit step
    Char_t   hitStatus;        // =1 hit is used in fit
                               // =0 hit passed fit already before this fit
                               // -1 hit excluded from this fit but can be used
                               //    in next fit
    Short_t sequentialIndex;   // this object index in array
    Double_t* grad;            // pointer to grad in HMdcWireArr
    TMatrixD* grad2;           // pointer to grad2 in HMdcWireArr
    TMatrixD* matrH;           // pointer to matrH in HMdcWireArr
    
    // Lookup table for calc. of derivatives:
    Double_t y;                // track parameters in wires coor. system.
    Double_t z;                //
    Double_t dirY;             //
    Double_t dirZ;             //
    Double_t dYdP[4];          // dY/dX1,dY/dY1,dY/dX2,dY/dY2
    Double_t dZdP[4];          // dZ/dX1,dZ/dY1,dZ/dX2,dZ/dY2
    Double_t dDirYdP[4];       // dDirY/dX1,dDirY/dY1,dDirY/dX2,dDirY/dY2
    Double_t dDirZdP[4];       // dDirZ/dX1,dDirZ/dY1,dDirZ/dX2,dDirZ/dY2
    Double_t cd2DsdP2[4];      // coeff. for d2Dist/dPar(x1,y1,x2,y2)
    Double_t cd2DsdPkdPl[6];   // coeff. for d2Dist/dPkdPl, dVk/dPkdPl and
    Double_t cdVkdPl[6];       // dUk/dPkdPl. [i] is element in sym.matrix (4,4)
    Double_t cdUkdPl[6];       // [0-5] => (1,0),(2,0),(2,1),(3,0),(3,1),(3,2)

    // Variables for time optimization:
    Double_t oneDErrTdcTime2;  // 1./(errTdcTime^2)
    Double_t wtNorm;           // = weight/errTdcTime^2

    // Variables for derivativatives calculation:
    Double_t driftTimeDer;     // calculated drift time
    Double_t devDer;           // deviation

    // Variables for errors calculation:
    Double_t dTdPar[4];        // [4] for 4 parameters of track

    HMdcSizesCellsLayer* pSCLayer;      // HMdcSizesCells[sector][module][layer]
    HMdcDriftTimeParBin* pDrTimeParBin;
    static HMdcDriftTimePar* pDrTimePar;
    static HMdcCal2Par*    pCal2Par;
    
    Int_t geantTrackNum;       // geant track number
    Float_t tof;               // geant time offset 
  public:
    HMdcWireData(void);
    ~HMdcWireData(void){}
    void setGradP(Double_t* gr,TMatrixD* gr2,TMatrixD* mH) 
        {grad=gr; grad2=gr2; matrH=mH;}
    void setCell(Int_t s, Int_t m, Int_t l, Int_t c, Int_t it, Float_t t);
    void setCell(HMdcCal1* cal1, Int_t it,Bool_t isGeant=kFALSE);
    void setNegTdcTimeTo0(void);
    void setSignalTimeOffset(Double_t offset);
    void setWeightTo1or0(Double_t maxChi2, Double_t minWeight);
    void setWeightAndWtNorm(Double_t wt) {weight=wt; wtNorm=wt*oneDErrTdcTime2;}
    void setHitStatus(Int_t st)          {hitStatus=st;}
    void setErrTdcTime(Double_t err)     {errTdcTime=err;}
    void setSequentialIndex(Short_t i)   {sequentialIndex=i;}
    void setUnitWeight(void);
    void setSizesCellsLayer(HMdcSizesCellsLayer* fscl) {pSCLayer=fscl;}
    static void setCal2Par(HMdcCal2Par* c2par)         {pCal2Par=c2par;}
    static void setDriftTimePar(HMdcDriftTimePar* par) {pDrTimePar=par;}

    void getAddress(Int_t& s, Int_t& m, Int_t& l, Int_t& c, Int_t& it);
    Int_t     getSector(void) const           {return sector;}
    Int_t     getModule(void) const           {return module;}
    Int_t     getLayer(void) const            {return layer;}
    Int_t     getCell(void) const             {return cell;}
    Int_t     getTimeIndx(void) const         {return timeIndx;}
    Double_t  getTdcTimeCal1(void) const      {return tdcTimeCal1;}
    Double_t  getSignalTimeOffset(void) const {return signalTimeOffset;}
    Double_t  getTdcTime(void) const          {return tdcTime;}
    Double_t  getErrTdcTime(void) const       {return errTdcTime;}
    Double_t  getDrTime(void) const           {return driftTime;}
    Double_t  getDev(void) const              {return dev;}
    Double_t  getChi2(void) const             {return chi2;}
    Double_t  getDistance(void) const         {return distance;}
    Double_t  getAlpha(void) const            {return alpha;}
    Bool_t    isInCell(void) const            {return inCell;}
    Double_t  getWeight(void) const           {return weight;}
    Char_t    getHitStatus(void) const        {return hitStatus;}
    Double_t  getOneDErrTdcTime2(void) const  {return oneDErrTdcTime2;}
    Double_t  getWtNorm(void) const           {return wtNorm;}
    Double_t  getDrTimeDer(void) const        {return driftTimeDer;}
    Short_t   getSequentialIndex(void) const  {return sequentialIndex;}
    Int_t     getGeantTrackNum(void) const    {return geantTrackNum;}
    Bool_t    getIsWireUsed(void)             {return isWireUsed;}
    HMdcSizesCellsLayer* getSCLayer(void) const {return pSCLayer;}

    Int_t getSequentialLayNum(void) const {return layer+module*6;}
    void getLocation(HLocation& loc) const;

    void calcTdcErrors(HMdcTrackParam& par);
    Bool_t isPassedFit(void) const {
        return (hitStatus==1 && weight>0.1) ? kTRUE:kFALSE;}
    void calcDriftTime(HMdcTrackParam& par);
    void calcDriftTimeAndErr(HMdcTrackParam& par);
    void calcDriftTimeAndInCell(HMdcTrackParam& par);
    void calcDriftTimeAndErrAndInCell(HMdcTrackParam& par);
    void calcDriftTimeForDeriv(HMdcTrackParam& par);
    void recalcFunctional(HMdcTrackParam& par);
    void calcFunctional(HMdcTrackParam& par);
    void calcFunctionalForDer(HMdcTrackParam& par);
    void calcDevAndFunct(HMdcTrackParam& par);
    void calcAnalytDeriv(HMdcTrackParam& par,Int_t flag=0);
    void fillLookupTableForDer(HMdcTrackParam& par);
    Bool_t removeIfWeightLess(Double_t wtCut, HMdcList24GroupCells& list);
    Int_t unsetFittedHit(HMdcList24GroupCells& list) const;
    void fillWireFitCont(HMdcWireFit* fWireFit) const;
    void fillWireFitSimCont(HMdcWireFit* fWireFit, Int_t trackNum) const;
    void removeThisWire(HMdcList24GroupCells& list);
    Bool_t removeOneTimeIfItDoubleTime(HMdcWireData* time1,
        HMdcList24GroupCells& list);
    Bool_t removeIfOneDistOutCell(HMdcWireData* wire2,
        HMdcList24GroupCells& list);
    void calcDriftTimeAndFunct(HMdcTrackParam& par);
    void initDTdPar(Int_t par) {if(hitStatus==1) dTdPar[par]=driftTimeDer;}
    void calcDTdPar(Int_t par, Double_t oneDv2Step);
    void printIfDeleted(void) const;
    void subtractWireOffset(HMdcTrackParam& par, Double_t sigSpeed);
    void setHitStatM1toP1(void) {if(hitStatus==-1) hitStatus=1;}
    void addToTOffsetErr(HMdcTrackParam& par);
    void printTime(Int_t iflag, HMdcTrackParam& par, Bool_t isGeant=kFALSE);
    void setTukeyWeight(Double_t cwSig,Double_t c2Sig,Double_t c4Sig,
        Double_t& sumWt, Double_t& funct);
    void setTukeyWeight(Double_t cwSig,Double_t c2Sig,Double_t c4Sig);
    Bool_t isAddressEq(Int_t s, Int_t m, Int_t l, Int_t c);
// Bool_t testTukeyWeight(Double_t cwSig,Double_t c2Sig, Double_t c4Sig,
//     Double_t maxChi2, Double_t minWeight);
Bool_t testWeight1or0(Double_t maxChi2, Double_t minWeight);

  private:
    void setInitVal(void);

  ClassDef(HMdcWireData,0) // One wire data for track piece fitter
};

class HMdcWiresArr : public TObject {
  protected:
    HMdcWireData* wires;                // Array of HMdcWireData objects
    Int_t         arraySize;            // Size of "wires" array
    Int_t         nDriftTimes;          // Number of drift times in track
    // Input data:
    HMdcClus* fClst1;                   //
    HMdcClus* fClst2;                   // is used for sector track (magnet off)
    Int_t     segment;                  // segment (0,1 or 3)
    Int_t     sector;                   // sector
    Float_t   xClst;                    // cluster position on
    Float_t   yClst;                    // the project plane
    Float_t   zClst;

    HMdcTrackFitInOut*   fitInOut;
    HMdcList24GroupCells lInputCells;   // sells list for fitting (input)
    HMdcList24GroupCells lOutputCells;  // cells list passed fit
    Int_t nMdcTimes[4];                 // Num. of hits in MDC's
    Int_t firstTime;
    Int_t lastTime;
    HMdcWireData* firstWire;
    HMdcWireData* lastWire;
    Int_t nModsInFit;                   // number of modules in this fit

    Int_t    numOfGoodWires;            // number of wires passed fit
    Double_t sigma2;                    // last sigma^2 of chi2 distr. which was
                                        // used for weights calc.

    HLocation locCal1;                  // location of HMdcCal1 object
    HMdcWireData* firstWireInLay[24];   // first wire in layer
    HMdcWireData* lastWireInLay[24];    // last wire in layer
    Bool_t fprint;                      // debuging print flag

    Double_t grad[4];                   // Gradient of downhill direction
    Double_t agrad;
    TMatrixD grad2;
    TMatrixD matrH;
    
    // For GEANT events for filling HMdcClusFitSim containers:
    Int_t trackNum[200];                // track numbers
    Int_t numWiresClus[200];            // num. of wires in cluster for each tr.
    Int_t numWiresFit[200];             // num. of wires passed fit for each tr.

  public:
    HMdcWiresArr(void);
    ~HMdcWiresArr(void);
    void setNumDriftTimes(Int_t nDrTm);
    void setPrintFlag(Bool_t flag=kTRUE) {fprint=flag;}
    void setXYZClust(Float_t x, Float_t y, Float_t z) {
      xClst=x; yClst=y; zClst=z;}

    HMdcWireData* getWiresArray(void) {return wires;}
    HMdcWireData* getWireData(Int_t n) {
        return (n>=0 && n<nDriftTimes) ? &(wires[n]):0;}
    HMdcWireData& operator[] (Int_t n) {return wires[n];}

    Int_t getSector(void) const       {return sector;}
    Int_t getSegment(void) const      {return segment;}
    Int_t getNDriftTimes(void) const  {return nDriftTimes;}
    Int_t getFirstTimeInd(void) const {return firstTime;}
    Int_t getLastTimeInd(void)  const {return lastTime;}
    Int_t getNumOfGoodWires(void)     {return numOfGoodWires;}
    Int_t getNModulesInFit(void)      {return  nModsInFit;}
    HMdcWireData* getFirstWire(void)  {return firstWire;}
    HMdcWireData* getLastWire(void)   {return lastWire;}
    HMdcClus*     getClust1(void)     {return fClst1;}
    HMdcClus*     getClust2(void)     {return fClst2;}
    HMdcList24GroupCells& getOutputListCells(void)    {return lOutputCells;}
    HMdcList24GroupCells& getInputListCells(void)     {return lInputCells;}

    Bool_t fillListHits(HMdcClus* fCl1, HMdcClus* fCl2);
    Bool_t fillListHits(HMdcEvntListCellsAndTimes* store,
        HMdcClus* fCl1, HMdcClus* fCl2);
    Bool_t    fillListHits(HMdcEvntListCellsAndTimes* store);
    void      fillLookupTableForDer(HMdcTrackParam& par);
    void      setTrackFitInOut(HMdcTrackFitInOut* pfio) {fitInOut=pfio;}
    Double_t  testFitResult(void);
    void      subtractWireOffset(HMdcTrackParam& par);
    void      setHitStatM1toP1(void);
    void      switchOff(Int_t sec, Int_t mod, Int_t lay=-1, Int_t cell=-1);
    Int_t     unsetHits(void);
    Int_t     getNCellsInInput(Int_t mod=-1);
    Int_t     getNCellsInOutput(Int_t mod=-1);
    void      fillClusFitAndWireFit(HMdcClusFit* fClusFit);
    void      fillClusFitSim(HMdcClusFit* fClusFit,HMdcTrackParam& par);
    Bool_t    calcNGoodWiresAndChi2(HMdcTrackParam& par);
    void      calcTdcErrorsAndFunct(HMdcTrackParam& par, Int_t iflag=0);
    Bool_t    setRegionOfWires(Int_t mod);
    Double_t  valueOfFunctional(HMdcTrackParam& par, Int_t iflag=0);
    Double_t  valueOfFunctAndErr(HMdcTrackParam& par, Int_t iflag=0);
    Double_t  functForDeriv(HMdcTrackParam& par, Int_t iflag=0);
    void      calcTimeOffsetsErr(HMdcTrackParam& par);
    Double_t  getSumOfWeights(void);
    void      printTimes(Int_t iflag, HMdcTrackParam& par);
    void      setWeightsTo1or0(HMdcTrackParam& par, Int_t iflag=0);
    Int_t     getNWiresInFit(void);
    Bool_t    filterOfHits(HMdcTrackParam& par, Int_t iflag=0);
    Bool_t    filterOfHitsV2(HMdcTrackParam& par, Int_t iflag=0);
    TMatrixD& getGrad2Matr(void) {return grad2;}
    TMatrixD& getMatrH(void) {return matrH;}
    void      setSizeGrad2Matr(HMdcTrackParam& par);
    void      calcSecondDeriv(HMdcTrackParam& par);
    void      calculateErrors(HMdcTrackParam& par);
    void      calcDerivatives(HMdcTrackParam& par,Int_t iflag);
    void      calcAnalyticDerivatives1(HMdcTrackParam& par);
    void      calcAnalyticDerivatives2(HMdcTrackParam& par);
    void      calcErrorsAnalyt(HMdcTrackParam& par);
    Double_t  calcAGradAnalyt(HMdcTrackParam& par);
    Double_t* getGrad(void) {return grad;}
    Double_t  getAGrad(void) {return agrad;}
    void      setUnitWeights(void);
    void      filterOfHitsConstSig(HMdcTrackParam& par, Double_t sig);
    Float_t   getXClst(void) {return xClst;}
    Float_t   getYClst(void) {return yClst;}
    Float_t   getZClst(void) {return zClst;}
Bool_t testTukeyWeights(HMdcTrackParam& par);
void setInitWeghts(HMdcTrackParam& par);

  private:
    void     recalcFunctional(HMdcTrackParam& par, Int_t iflag=0);
    void     calcFunctional(HMdcTrackParam& par, Int_t iflag);
    void     setListCells(HMdcClus* fCl1, HMdcClus* fCl2);
    void     setListCells(HMdcEvntListCellsAndTimes* store);
    void     setTukeyWeights(Double_t sigma2);
    Double_t setTukeyWeightsAndCalcSigma2(Double_t sigma2);
    void     calcDriftTime(HMdcTrackParam& par, Int_t iflag);
    void     calcDriftTimeAndErr(HMdcTrackParam& par, Int_t iflag);
    void     calcErrs(HMdcTrackParam& par);
    void     initDTdPar(Int_t k);
    void     calcDTdPar(Int_t k, Double_t oneDv2StepD);
    void     setWeightsTo1or0(Double_t maxChi2, Double_t minWeight);
    void     getGeantHitPoint(HMdcWireData* w, HGeantMdc* hit,
        Double_t& x,Double_t& y,Double_t& z);

  ClassDef(HMdcWiresArr,0) // Array of HMdcWireData objects
};

#endif
