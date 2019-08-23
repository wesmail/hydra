#ifndef HMDCTRACKFITTER_H
#define HMDCTRACKFITTER_H

#include "TObject.h"
#include "hlocation.h"
#include "hmdclistcells.h"
#include "hsymmat.h"

class HMdcTrackFitPar;
class HMdcSizesCells;
class HMdcSizesCellsMod;
class HMdcSizesCellsLayer;
class HMdcCal2ParSim;
class HMdcCal2Par;
class HMdcClus;
class HMdcList12GroupCells;
class HMdcWireFit;
class HMdcClusFit;
class HCategory;
class HMdcDigitPar;

// enum FillerFlag {
//   kNoFill=0,kIsFilled=1,kHitByFit=2,kHitSegByFit=3,kHitByClus=4,kHitSegByClus=5
// };

class HMdcTrackFitInOut : public TObject {
  friend class HMdc12Fit;
  friend class HMdcTrackFitter;
  friend class HMdcTrackFitterH;
  friend class HMdcTrackFitterA;
  protected:
    Int_t version;          // =0 - old version of drift time calc.
                            // =1 - using of funct. HMdcCal2Par::calcTime(...)
                            // for recalc. distance to drift time.
    HMdcCal2ParSim* fCal2ParSim;
    HMdcCal2Par* fCal2Par;
    HMdcTrackFitPar* fitPar;     // fit parameters container
    HCategory* fCalCat;          // pointer to MdcCal1 category
    HCategory* fClusFitCat;      // cat. for debuging inform.
    HCategory* fWireFitCat;      // -/-                                  
    HLocation locClusFit;        // location for new "HMdcClusFit" object
    HLocation locWireFit;        // location for new "HMdcWireFit" object
    HLocation loc;               // location for HMdcCal1 object
    
    Bool_t isGeant;              // =kTRUE for GEANT data
    Int_t useWireOffset;         // remove signal time on wire from drift time
    HMdcDigitPar* fDigitPar;     // "phys" digitization parameters 
    Float_t signalSpeed;         // speed of signal on the wire (ns/mm)
    HMdcSizesCells* fSizesCells;        //
    HMdcSizesCellsMod* fSCModAr[6][4];  //
    Double_t cutWeight;                 // cut for Tukey weight
    
  public:
    HMdcTrackFitInOut(void) : version(1), isGeant(kFALSE), useWireOffset(0),
      signalSpeed(0.005), fSizesCells(0), cutWeight(0.01) {;}
    ~HMdcTrackFitInOut(void) {;}
    Bool_t init(void);
    Bool_t reinit(void);
    
  ClassDef(HMdcTrackFitInOut,0)
};

#define ARRSIZE 250

class HMdcTrackFitter : public TObject {
  friend class HMdc12Fit;
  protected:
    // Fit options:
    Int_t version;          // =0 - old version of drift time calc.
                            // =1 - using of funct. HMdcCal2Par::calcTime(...)
                            // for recalc. distance to drift time.
    Int_t numOfParam;       // Number of parameters for fit procedure
    Int_t maxIteration;     // Maximum number of interations for fit
    Double_t tukeyConst;    //Tukey constant
    Double_t driftVelMDC1;  // Drift velocity in MDC1
    Double_t driftVelMDC2;  // Drift velocity in MDC2
    Double_t driftVelMDC3;  // Drift velocity in MDC3
    Double_t driftVelMDC4;  // Drift velocity in MDC4

    // Pointers to parameters containers parameters and functions
    Int_t arrSize;                  // size of arraies
    HMdcTrackFitInOut* fitInOut;
    HMdcCal2ParSim* fCal2ParSim;
    HMdcCal2Par* fCal2Par;
    Int_t indClusFit;               // HMdcClusFit object index
    HMdcSizesCellsLayer* fSizeCellsLayer[ARRSIZE];
    
    // Input data:
    HMdcClus * fClst;
    
    Int_t segment;                      // segment
    Int_t sector;                       // sector
    HMdcList12GroupCells listCells;     // list cells in cluster
    HMdcList12GroupCells listCellsOrg;  // list no fitted cells in cluster
    UChar_t lLayer[ARRSIZE];            // list of layers
    UChar_t lModule[ARRSIZE];           // list of modules
    UChar_t lCell[ARRSIZE];             // list of cells
    UChar_t timeIndx[ARRSIZE];          // Times list: =1->time1, =2->time2 in HMdcCal1
    Double_t tdcTime[ARRSIZE];          // drift_times-time_offset
    Double_t tdcTimeCal1[ARRSIZE];      // drift times from Cal1
    Float_t timeOffset[ARRSIZE];        // time offset (signal_path/signal_speed)
    Double_t errTdcTime[ARRSIZE];       // Array with drift times errors
    Double_t oneDErrTdcTime2[ARRSIZE];  // 1./(errTdcTime[ARRSIZE]^2)
//     Double_t xLeft[ARRSIZE];      // Array with x-coord. of left edge of wire
//     Double_t yLeft[ARRSIZE];      // Array with y-coord. of left edge of wire
//     Double_t zLeft[ARRSIZE];      // Array with z-coord. of left edge of wire
//     Double_t xRight[ARRSIZE];     // Array with x-coord. of right edge of wire
//     Double_t yRight[ARRSIZE];     // Array with y-coord. of right edge of wire
//     Double_t zRight[ARRSIZE];     // Array with z-coord. of right edge of wire
    Char_t  hitStatus[ARRSIZE];         // Hits which have status=1 will used this in fit only
                                        // =0 this hit was fited (for dubl.tr.in cl. only)
                                        // -1 may by will fited (-/-) Not implement yet !!!
    Int_t nClTimes;                     // Number of hits(fired wires) in cluster
    Int_t nMdcTimes[2];                 // Num. of hits in first and second MDC
    Int_t firstTime;                    // Star loop from this value
    Int_t lastTime;                     // End loop
    Int_t nMDC1Times, nMDC2Times;       // Number of hits in MDC1&MDC2
    Int_t indFWireInLay[12];            // first wire in layer index =-1 - no w.in lay.
    Int_t indLWireInLay[12];            // last wire in layer index
    Double_t xVertex,yVertex,zVertex;   // Coordinates for vertex
    Double_t xInitPlane,yInitPlane,
        zInitPlane;                     // Track coord on the proj.plane
    HMdcSizesCellsLayer* firstPl;       // plane for first track point
    HMdcSizesCellsLayer* secondPl;      // plane for second track point
//     Double_t parA1,parB1,parD1;   // first layer plane: parA1*x+parB1*y+z=parD1
//     Double_t parA2,parB2,parD2;   // last layer plane: parA2*x+parB2*y+z=parD2

    // Input-output data
    Double_t qualityWeight[ARRSIZE];    //  now == 1 ???

    // Output data
    Int_t iteration;                               // Loop variable
    Double_t funMin ;                              // Value of the functional
    Int_t numOfGoodWires;                          // num.of wires in fit
    Double_t chi2perDF;                            // =funMin/(numOfGoodWires-numOfParam+1)
    Double_t arriveTime;                           // Arrive time
    Double_t xOutVertex, yOutVertex, zOutVertex;   // Final pos. of the vertex
    Double_t xOutPlane, yOutPlane, zOutPlane;      // Tr. coord. on the proj.pl.
    HSymMat4 errMatr;                              // Matrix of errors 
    Double_t errTimeOffset;                         // error in time offset

    // Options:
    Bool_t fprint;                                 // Flag for control print-out

    // Fit variables:
    Double_t functional, newFunctional;
    Double_t parameter[6];         // array with parameters for functional
    Double_t drTime[ARRSIZE];      // calcul. drift time
    Double_t dev[ARRSIZE];         // deviation of calculated and measured time.
    Double_t chi2[ARRSIZE];        // deviation^2
    Double_t distWire[ARRSIZE];    // dist. to wires
    Double_t alphaDrDist[ARRSIZE]; // angles of min.dist
    Bool_t inCell[ARRSIZE];        // =kTRUE - tr.cross cell
    Double_t weight[ARRSIZE];      // weight array
    
//    FillerFlag filler;
    Int_t fitStatus;               // =kTRUE if cluster fit accepted 
    Int_t segIndex;                // HMdcSeg index

  public:
    HMdcTrackFitter(HMdcTrackFitInOut* fIO) :
        version(0), arrSize(ARRSIZE), firstTime(0), fprint(kFALSE) {
      fitInOut=fIO;
      init();
    }
    virtual ~HMdcTrackFitter(void) {}
    void setPrintFlag(Bool_t prnt) {fprint=prnt;}
    void setVersion(Int_t ver) {version=ver;}
    virtual Int_t execute(Double_t threshold, Int_t iter=0) {return 0;}
    virtual Bool_t finalize(void) {return kTRUE;}
    Bool_t fillListHits(HMdcList12GroupCells& lCells);

    // Input data:
    void setAddress(Int_t sec, Int_t seg) {sector=sec; segment=seg;}

    UChar_t* getListModules(void) {return lModule;}
    UChar_t* getListLayers(void) {return lLayer;}
    UChar_t* getListCells(void) {return lCell;}

    void setLastTime(Int_t last) {
        lastTime=(last<nClTimes) ? lastTime=last:nClTimes;}
    void setFirstTime(Int_t first) {firstTime = first;}
    void setNMDC1Times( Int_t nT ) {nMDC1Times = nT;}
    void setNMDC2Times( Int_t nT ) {nMDC2Times = nT;}

    HMdcSizesCellsLayer** getSizeCellsLayerArr(void) {return fSizeCellsLayer;}

    Char_t*  getHitStatusArr(void)  {return hitStatus;}
    UChar_t* getTimeIndxArr(void)  {return timeIndx;}
    Double_t* getTDCTimeArr(void) {return tdcTime;}
//     Double_t* getXLeftArr(void) {return xLeft;}
//     Double_t* getYLeftArr(void) {return yLeft;}
//     Double_t* getZLeftArr(void) {return zLeft;}
//     Double_t* getXRightArr(void) {return xRight;}
//     Double_t* getYRightArr(void) {return yRight;}
//     Double_t* getZRightArr(void) {return zRight;}
    Double_t* getDeviationArr(void) {return dev;}
    Double_t* getDistWireArr(void) {return distWire;}
    Double_t* getAlphaDrDistArr(void) {return alphaDrDist;}
    Bool_t* getInCellFlag(void) {return inCell;}
    Double_t* getWeightArr(void) {return weight;}
    Double_t* getDriftTimeArr(void) {return drTime;}


    void setTrackPar(Double_t x1,Double_t y1,Double_t z1,
                     Double_t x2,Double_t y2,Double_t z2);

    Double_t* getQualityWeightArr(void) {return qualityWeight;}

    // driftVelMDC is geted from param. file, this func. for testing
    void setDriftVelocityMDC1(Float_t  v) {driftVelMDC1 = Double_t(v);}
    void setDriftVelocityMDC2(Float_t  v) {driftVelMDC2 = Double_t(v);}
    void setDriftVelocityMDC3(Float_t  v) {driftVelMDC3 = Double_t(v);}
    void setDriftVelocityMDC4(Float_t  v) {driftVelMDC4 = Double_t(v);}

    // Fit final parameters:
    Int_t getNumOfParam(void) const {return numOfParam;}
    Double_t getFunMin(void) const {return funMin;}
    Double_t getChi2(void) const {return chi2perDF;}
    Double_t getXOutVertex(void) const {return xOutVertex;}
    Double_t getYOutVertex(void) const {return yOutVertex;}
    Double_t getZOutVertex(void) const {return zOutVertex;}
    Double_t getXOutPlane(void) const {return xOutPlane;}
    Double_t getYOutPlane(void) const {return yOutPlane;}
    Double_t getZOutPlane(void) const {return zOutPlane;}
    Double_t getArriveTime(void) const {return arriveTime;}
    Int_t getIterationNumber(void) const {return iteration;}

    Double_t getDeviation(Int_t nT) const {return  dev[nT];}
    Double_t getChi2(Int_t nT) const {return  chi2[nT];}
    Double_t getDistWire(Int_t nT) const {return distWire[nT];}
    Double_t getTdcTime(Int_t nT) const {return tdcTime[nT];}
    Double_t getTdcTimeErr(Int_t nT) const {return errTdcTime[nT];}
    Double_t getWeight(Int_t nT) const {return weight[nT];}
    virtual Double_t getFunctional(void) const {return -1.;} // not fast way !
    Int_t getNMDC1Times(void) const {return nMDC1Times;}
    Int_t getNMDC2Times(void) const {return nMDC2Times;}

    Double_t testFitResult(void);
    Bool_t fillClusFitCont(void);
    Bool_t fillWireFitCont(HMdcWireFit* fWireFit, Int_t hit) const;
    Int_t unsetHits(void);
    Bool_t fitCluster(Double_t threshold, Int_t fittingMod=-1);
    void calcMinFunctional(void);
    void setFittingTimesList(Int_t iMod=-1);
    Float_t getParErr(Int_t i) const { 
      return (i<0||i>=numOfParam) ? 0.:sqrt(errMatr.getElement(i,i));}
    HSymMat4& getErrMatr(void) {return errMatr;}

  protected:
    void init(void);
    void calcTdcErrors(Double_t* par);

  ClassDef(HMdcTrackFitter,0)
};

#undef ARRSIZE

#endif
