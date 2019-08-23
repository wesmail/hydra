#ifndef HMDCTRACKFITTER_H
#define HMDCTRACKFITTER_H

#include "TObject.h"

class HMdcTrackFitPar;
class HMdcSizesCellsLayer;
class HMdcCal2ParSim;

#define ARRSIZE 250

class HMdcTrackFitter : public TObject {
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
    Int_t arrSize;                // size of arraies
    HMdcCal2ParSim* fCal2ParSim;
    HMdcTrackFitPar* fitPar;     // Pointer to fitting container
    HMdcSizesCellsLayer* fSizeCellsLayer[ARRSIZE];
     // Input data:
    Int_t segment;                // segment
    Int_t sector;                 // sector
    Int_t lLayer[ARRSIZE];        // list of layers
    Int_t lModule[ARRSIZE];       // list of modules
    Int_t lCell[ARRSIZE];         // list of cells
    Int_t timeIndx[ARRSIZE];      // =1->time1, =2->time2 in HMdcCal1
    Double_t tdcTime[ARRSIZE];    // Array with drift times
    Double_t xLeft[ARRSIZE];      // Array with x-coord. of left edge of wire
    Double_t yLeft[ARRSIZE];      // Array with y-coord. of left edge of wire
    Double_t zLeft[ARRSIZE];      // Array with z-coord. of left edge of wire
    Double_t xRight[ARRSIZE];     // Array with x-coord. of right edge of wire
    Double_t yRight[ARRSIZE];     // Array with y-coord. of right edge of wire
    Double_t zRight[ARRSIZE];     // Array with z-coord. of right edge of wire
    Char_t  hitStatus[ARRSIZE];   // hitStatus[hit]=kTRUE -use this hit in fit
    Int_t nClTimes;               // Number of hits(fired wires) in cluster
    Int_t firstTime;              // Star loop from this value
    Int_t nMDC1Times, nMDC2Times; // Number of hits in MDC1&MDC2
    Double_t xVertex,yVertex,
        zVertex;                  // Coordinates for vertex
    Double_t xInitPlane,
        yInitPlane, zInitPlane;   // Track coord on the proj.plane
    Double_t parA,parB,parD;      // param. of proj.plane: parA*x+parB*y+z=parD

    // Input-output data
    Double_t qualityWeight[ARRSIZE];   // ??? reflect num.of hits/layer in cluster.

    // Output data
    Int_t iteration;                               // Loop variable
    Double_t funMin ;                              // Value of the functional
    Double_t arriveTime;                           // Arrive time
    Double_t arriveTimeErr;                        // Arrive time error
    Double_t xOutVertex, yOutVertex, zOutVertex;   // Final pos. of the vertex
    Double_t xErrVertex, yErrVertex, zErrVertex;   // ====" "==== error
    Double_t xOutPlane, yOutPlane, zOutPlane ;     // Tr. coord. on the proj.pl.
    Double_t xErrPlane, yErrPlane, zErrPlane ;     // ====" "==== error

    // Options:
    Bool_t fprint;                                 // Flag for control print-out

    // Fit variables:
    Double_t functional, newFunctional;
    Double_t parameter[6];         // array with parameters for functional
    Double_t fullTime[ARRSIZE];        //
    Double_t dev[ARRSIZE];         // deviation of calculated and measured time.
    Double_t distWire[ARRSIZE];    // dist. to wires
    Double_t alphaDrDist[ARRSIZE]; // angles of min.dist
    Bool_t inCell[ARRSIZE];        // =kTRUE - tr.cross cell
    Double_t weight[ARRSIZE];      // ???????????
    
    Int_t hitSt[ARRSIZE];

  public:
    HMdcTrackFitter(void) : 
      version(0), arrSize(ARRSIZE), firstTime(0), fprint(kFALSE) {;}
    virtual ~HMdcTrackFitter(void) {;}
    void setPrintFlag(Bool_t prnt) { fprint=prnt; }
    void setVersion(Int_t ver) { version=ver; }
    virtual Bool_t init(void);
    virtual Bool_t reinit(void);
    virtual Int_t execute(Double_t threshold, Int_t iter=0) {return 0;}
    virtual Bool_t finalize(void) {return kTRUE;}
    
    // Input data:
    void setAddress(Int_t sec, Int_t seg) {sector=sec; segment=seg;}
    
    Int_t* getListModules(void) {return lModule;}
    Int_t* getListLayers(void) {return lLayer;}
    Int_t* getListCells(void) {return lCell;}
    
    void setNTimes(Int_t nT) { nClTimes = nT; }
    void setFirstTimes(Int_t nT) { firstTime = nT; }
    void setNMDC1Times( Int_t nT ) { nMDC1Times = nT; }
    void setNMDC2Times( Int_t nT ) { nMDC2Times = nT; }
    
    HMdcSizesCellsLayer** getSizeCellsLayerArr(void) {return fSizeCellsLayer;}
    
    Char_t*  getHitStatusArr(void)  {return hitStatus;}
    Int_t* getTimeIndxArr(void)  {return timeIndx;}
    Double_t* getTDCTimeArr(void) {return tdcTime;}
    Double_t* getXLeftArr(void) {return xLeft;}
    Double_t* getYLeftArr(void) {return yLeft;}
    Double_t* getZLeftArr(void) {return zLeft;}
    Double_t* getXRightArr(void) {return xRight;}
    Double_t* getYRightArr(void) {return yRight;}
    Double_t* getZRightArr(void) {return zRight;}
    Double_t* getDeviationArr(void) {return dev;}
    Double_t* getDistWireArr(void) {return distWire;}
    Double_t* getAlphaDrDistArr(void) {return alphaDrDist;}
    Bool_t* getInCellFlag(void) {return inCell;}
    Double_t* getWeightArr(void) {return weight;}
    Double_t* getFullTime(void) {return fullTime;}
    
    
    
    void setXVertex    ( Double_t x ) { xVertex     = x; }
    void setYVertex    ( Double_t y ) { yVertex     = y; }
    void setZVertex    ( Double_t z ) { zVertex     = z; }
    
    Double_t* getQualityWeightArr(void) {return qualityWeight;}
    
    void setXInitPlane ( Double_t  x ) { xInitPlane  = x; }
    void setYInitPlane ( Double_t  y ) { yInitPlane  = y; }
    void setZInitPlane ( Double_t  z ) { zInitPlane  = z; }
    void setParPrPlane (Double_t a, Double_t b, Double_t d)
        {parA=a; parB=b; parD=d;}
    
    // driftVelMDC is geted from param. file, this func. for testing
    void setDriftVelocityMDC1 ( Float_t  v ) { driftVelMDC1 = Double_t(v); }
    void setDriftVelocityMDC2 ( Float_t  v ) { driftVelMDC2 = Double_t(v); }
    void setDriftVelocityMDC3 ( Float_t  v ) { driftVelMDC3 = Double_t(v); }
    void setDriftVelocityMDC4 ( Float_t  v ) { driftVelMDC4 = Double_t(v); }
    
    // Fit final parameters: 
    Int_t getNumOfParam() { return numOfParam; }
    Double_t getFunMin() { return funMin; }
    Double_t getXOutVertex() { return xOutVertex; }
    Double_t getYOutVertex() { return yOutVertex; }
    Double_t getZOutVertex() { return zOutVertex; }
    Double_t getXOutPlane() { return xOutPlane; }
    Double_t getYOutPlane() { return yOutPlane; }
    Double_t getZOutPlane() { return zOutPlane; }
    Double_t getArriveTime() { return arriveTime; }
    Int_t getIterationNumber() { return iteration; } 
    
    Double_t getDeviation( Int_t nT ) { return dev[nT]; }
    Double_t getDistWire( Int_t nT ) { return distWire[nT]; }
    Double_t getTDCTime( Int_t nT ) { return tdcTime[nT]; }
    Double_t getWeight( Int_t nT ) { return weight[nT]; }    
    virtual Double_t getFunctional(void) {return -1.;} // not fast way !
    Int_t getNMDC1Times() { return nMDC1Times; }
    Int_t getNMDC2Times() { return nMDC2Times; }

    ClassDef(HMdcTrackFitter,0)
};
    
#undef ARRSIZE

#endif
