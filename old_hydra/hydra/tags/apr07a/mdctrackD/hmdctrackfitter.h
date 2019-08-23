#ifndef HMDCTRACKFITTER_H
#define HMDCTRACKFITTER_H

#include <cmath>
#include "TObject.h"
#include "hlocation.h"
#include "hmdclistgroupcells.h"
#include "hmdctrackparam.h"
#include "hmdcwiredata.h"
#include "hmdcdrifttimepar.h"

class HMdcTrackFitPar;
class HMdcSizesCells;
class HMdcSizesCellsMod;
class HMdcSizesCellsLayer;
class HMdcCal2ParSim;
class HMdcCal2Par;
class HMdcClus;
class HMdcWireFit;
class HMdcClusFit;
class HCategory;
class HMdcDigitPar;
class HMdcEvntListCellsAndTimes;

class HMdcTrackFitInOut : public TObject {
  friend class HMdc12Fit;
  friend class HMdcTrackFitter;
  friend class HMdcTrackFitterA;
  friend class HMdcTrackFitterB;
  protected:
    Int_t version;          // =0 - old version of drift time calc.
                            // =1 - using of funct. HMdcCal2Par::calcTime(...)
                            // for recalc. distance to drift time.
    HMdcDriftTimePar driftTimePar;
    HMdcCal2ParSim*  fCal2ParSim;
    HMdcCal2Par*     fCal2Par;
    HMdcTrackFitPar* fitPar;      // fit parameters container
    HCategory*    fCalCat;        // pointer to MdcCal1 category
    HCategory*    fClusFitCat;    // cat. for debuging inform.
    HCategory*    fWireFitCat;    // -/-
    HCategory*    fGeantKineCat;  // pointer to HGeantKine category
    HCategory*    fGeantMdcCat;   // pointer to HGeantMdc category
    HLocation     locClusFit;     // location for new "HMdcClusFit" object
    HLocation     locWireFit;     // location for new "HMdcWireFit" object
    HLocation     loc;            // location for HMdcCal1 object

    Bool_t        geantFlag;      // =kTRUE for GEANT data
    Int_t         wireOffsetFlag; // remove signal time on wire from drift time
    HMdcDigitPar* fDigitPar;      // "phys" digitization parameters
    Float_t       signalSpeed;    // speed of signal on the wire (ns/mm)
    Bool_t        fprint;         // flag for control print-out
    Int_t         printStartEv;   // start to print from this event number
    Int_t         nEventPrint;    // print this number of events (def=1000000000)
    HMdcSizesCells*    fSizesCells;     //
    HMdcSizesCellsMod* fSCModAr[6][4];  //

    // Track fitter cuts and parameters:
    Double_t cutWeight;         // cut for Tukey weight
    Int_t    tofFlag;           // =1 - calculate tof for each module
                                //     Don't use tofFlag=1 !!!
                                // =2 - calculate tof for each segment
                                // =3 - calculate one tof for all modules
    Double_t minTimeOffset;     // cut on minimal value of time offset
    Double_t maxTimeOffset;     // cut on maximal value of time offset
    Int_t    minCellsNum;       // cut on the min.number of wires passed fit
    Double_t totalChi2Cut;      // cut on value of functional
    Double_t chi2PerNdfCut;     // cut on value of functional/NDF
    Bool_t   chi2CutFlag;       // if kTRUE - functional<totalChi2Cut
                                // else functional/NDF<chi2PerNdfCut

    // Tukey weights constants:
    Bool_t useTukeyFlag;        // flag for using or not Tukey weights.
    Double_t cnWs;    // if     (chi2<cnWs*sig2) wt=(1-(chi2/(cn4s*sig2))^2)^2
    Double_t cn2s;    // else if(chi2<cn2s*sig2) wt=(1- chi2/(cn2s*sig2)   )^2
    Double_t cn4s;    // else wt=0.
    Double_t minSig2;           // minimal value of sugma^2
    Int_t    maxNFilterIter;    // max. number of iterations in wires filtering
    Double_t minWeight;         // weight=(weight<minWeight) ? 0.:1.;
    Double_t maxChi2;           // weight=(chi2[time]>maxChi2) ? 0.:1.;
    Bool_t   doTargScan;        // HMdcTrackFitterB use this flag

    Double_t minTOffsetIter;    // time offset cut for intermediate iterations

    // Fit parameters for derivatives calc.:
    Double_t funCt1;           // if functional < funCt1 stepD1 will used
    Double_t stepD1;
    Double_t funCt2;           // else if functional < funCt2 stepD2 will used
    Double_t stepD2;
    Double_t stepD3;           // else stepD3 will used

  public:
    HMdcTrackFitInOut(void);
    ~HMdcTrackFitInOut(void) {}
    Bool_t init(void);
    Bool_t reinit(void);
    void   setDefaultFitParam(void);
    void   setModuleTof(void)               {tofFlag=1;} //Don't use this method
    void   setSegmetTof(void)               {tofFlag=2;}
    void   setSectorTof(void)               {tofFlag=3;}
    Int_t  useWireOffset(void) const        {return wireOffsetFlag;}
    Bool_t isGeant(void)                    {return geantFlag;}
    void   setUseTukeyFlag(Bool_t flag)     {useTukeyFlag=flag;}
    void   setTukeyConstants(Double_t cw,Double_t c2,Double_t c4);
    void   setTukeyWtMinSigma(Double_t sig) {minSig2=sig*sig;}
    void   setMaxNumFilterIter(Int_t ni)    {maxNFilterIter=ni;}
    void   setMinTOffsetIter(Double_t mt)   {minTOffsetIter=mt;}
    void   setMinWeight(Double_t mw)        {minWeight=mw;}
    void   setMaxMaxChi2(Double_t mc)       {maxChi2=mc;}
    void   setFunCut1(Double_t fc1)         {funCt1=fc1;}
    void   setStepD1(Double_t sd1)          {stepD1=sd1;}
    void   setFunCut2(Double_t fc2)         {funCt2=fc2;}
    void   setStepD2(Double_t sd2)          {stepD2=sd2;}
    void   setStepD3(Double_t sd3)          {stepD3=sd3;}
    void   setTargetScanFlag(Bool_t fl)     {doTargScan=fl;}
    void   setPrintFlag(Bool_t prnt)        {fprint=prnt;}
    void   setPrintFlag(Int_t currentEvent);
    void   setNEventsPrint(Int_t start,Int_t nev);
    void   setChi2PerNdfCut(Double_t cut=50.); 
    void   setTotalChi2Cut(Double_t cut=300.);

    HMdcWireFit* getNewWireFitSlot(Int_t* indWireFit);
    HMdcClusFit* getNewClusFitSlot(Int_t* indClusFit);
    void getTukeyConstants(Double_t& cw,Double_t& c2,Double_t& c4) const;
    HMdcSizesCellsMod** getSCellsModArr(Int_t s) {return fSCModAr[s];}
    HMdcSizesCells* getMdcSizesCells(void)   {return fSizesCells;}
    HCategory* getWireFitCat(void)           {return fWireFitCat;}
    HCategory* getMdcCal1Cat(void)           {return fCalCat;}
    HCategory* getGeantKineCat(void)         {return fGeantKineCat;}
    HCategory* getGeantMdcCat(void)          {return fGeantMdcCat;}
    HMdcDriftTimePar& getDriftTimePar(void)  {return driftTimePar;}
    Double_t getWeightCut(void) const        {return cutWeight;}
    Int_t    getTofFlag(void) const          {return tofFlag;}
    Float_t  getSignalSpeed(void) const      {return signalSpeed;}
    Int_t    getDrTimeCalcVer(void) const    {return version;}
    Bool_t   getTukeyFlag(void) const        {return useTukeyFlag;}
    Bool_t   useTukey(void) const            {return useTukeyFlag;}
    Double_t getTukeyConstWs(void) const     {return cnWs;}
    Double_t getTukeyConst2s(void) const     {return cn2s;}
    Double_t getTukeyConst4s(void) const     {return cn4s;}
    Double_t getTukeyWtMinSigma(void) const  {return sqrt(minSig2);}
    Double_t getTukeyWtMinSigma2(void) const {return minSig2;}
    Int_t    maxNumFilterIter(void) const    {return maxNFilterIter;}
    Double_t getMinTOffsetIter(void) const   {return minTOffsetIter;}
    Double_t getMinWeight(void) const        {return minWeight;}
    Double_t getMaxChi2(void) const          {return maxChi2;}
    Double_t getFunCut1(void) const          {return  funCt1;}
    Double_t getStepD1(void) const           {return  stepD1;}
    Double_t getFunCut2(void) const          {return  funCt2;}
    Double_t getStepD2(void) const           {return  stepD2;}
    Double_t getStepD3(void) const           {return  stepD3;}
    Double_t getStepDer(Double_t funct) const;
    Bool_t   getPrintFlag(void) const        {return fprint;}
    Int_t    getPrintStartEvent(void) const  {return printStartEv;}
    Int_t    getNEventPrint(void) const      {return nEventPrint;}

  ClassDef(HMdcTrackFitInOut,0) // Service class for for Dubna track piece fitters
};

class HMdcTrackFitter : public TObject {
  friend class HMdc12Fit;
  friend class HMdcTrackFinder;
  protected:
    HMdcTrackFitInOut* fitInOut;
    HMdcCal2ParSim* fCal2ParSim;
    HMdcCal2Par* fCal2Par;
    Int_t indClusFit;            // HMdcClusFit object index
    HMdcWiresArr wires;          // array of wires for fit
    HMdcTrackParam initParam;    // initial track parameters
    HMdcTrackParam finalParam;   // final track parameters
    Int_t  iteration;             // counter of iterations

    Int_t  maxIteration;         // maximum number of interations for fit
    Int_t  tofFlag;              // see HMdcTrackFitInOut
    Bool_t fprint;               // flag for control print-out
    Char_t exitFlag;             // flag of track fitter exit (=1,2,3,4,5)
    Bool_t fitStatus;            // =kTRUE if cluster fit accepted
    Int_t  segIndex;             // HMdcSeg index

  public:
    HMdcTrackFitter(HMdcTrackFitInOut* fIO);
    virtual ~HMdcTrackFitter(void) {}
    virtual Int_t minimize(Int_t iter=0) {return 0;}
    Bool_t fillListHits(HMdcClus* clus1,HMdcClus* clus2);
    Bool_t fillListHits(HMdcEvntListCellsAndTimes* store,
                        HMdcClus* clus1,HMdcClus* clus2);
    Bool_t fillListHits(HMdcEvntListCellsAndTimes* store);

    void setPrintFlag(Bool_t prnt);
    void setModuleTof(void)  {tofFlag=1;}  // calculate tof for each module
    void setSegmentTof(void) {tofFlag=2;}  // calculate tof for each segment
    void setSectorTof(void)  {tofFlag=3;}  // calculate one tof for all modules

    // Input data:
    Bool_t setClustAndFill(HMdcClus* fClst1, HMdcClus* fClst2=0);
    Bool_t setClustAndFill(HMdcEvntListCellsAndTimes* store,
			   Double_t x1, Double_t y1, Double_t z1,
			   Double_t x2, Double_t y2, Double_t z2);

    // Fit final parameters:
    Int_t    getNumOfParam(void) const      {return finalParam.getNumParam();}
    Double_t getFunMin(void) const          {return finalParam.functional();}
    Double_t getChi2(void) const            {return finalParam.getChi2();}
    Int_t    getIterationNumber(void) const {return iteration;}
    Char_t   getExitFlag(void) const        {return exitFlag;}
    Bool_t   getFitStatus(void) const       {return fitStatus;}
    HMdcTrackParam* getFinalParam(void)     {return &finalParam;}

    HMdcWireData* getWiresList(void)        {return wires.getWiresArray();}
    HMdcWiresArr& getWiresArr(void)         {return wires;}
    HMdcWireData& getWire(Int_t n)          {return wires[n];}
    virtual Double_t getFunctional(void) const {return -1.;} 
    void fillOutput();

    Bool_t fillClusFitCont(void);
    Bool_t fitCluster(Int_t fittingMod=-1);
    void   calcMinFunctional(void);
    void   setRegionOfWires(Int_t mod=-1);
    void   switchOffWires(Int_t sec, Int_t mod, Int_t lay=-1, Int_t cell=-1)
               {wires.switchOff( sec, mod, lay, cell);}

  protected:
    void init(void);
    void setPlanes(void);
    Bool_t testChi2Cut(void);

  ClassDef(HMdcTrackFitter,0) // Base class for Dubna track piece fitter
};

#endif
