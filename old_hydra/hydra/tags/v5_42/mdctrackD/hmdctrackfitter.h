#ifndef HMDCTRACKFITTER_H
#define HMDCTRACKFITTER_H

#include "TObject.h"

class HMdcTrackFitPar;
class TStopwatch;
class HMdcSizesCellsLayer;
class HMdcCal2ParSim;

class HMdcTrackFitter : public TObject {
  private:
    Int_t version;      // =0 - old version of drift time calc.
                        // =1 - using of funct. HMdcCal2Par::calcTime(...)
                        // for recalc. distance to drift time.
    Int_t numberOfParameter;    // Number of parameters for fit procedure
    Int_t maxIteration;         // Maximum number of interations for fit
    Double_t stepNorm;          // Normalisation factor for step
    Double_t xVertex,
      yVertex, zVertex;         // Coordinates for vertex
    Double_t weightPar[6];      // Array with weight for parameters
    Double_t step[6];           // Array with steps
                                // for chanching of parameters
    Double_t cnst;              // Tukey constant
    Int_t segment;              // Number of segment 1 for MDC1&MDC2 2 for MDC3&MDC4
    Int_t sector;               // Number of sector
    Int_t* lLayer;              // list of layers
    Int_t* lModule;             // list of modules (0-3)
    Int_t* lCell;               // list of cells
    Int_t nMDC1Times, nMDC2Times; // Number of fired wires in MDC1&MDC2
    HMdcSizesCellsLayer** fSizeCellsLayer;
    Char_t * hitStatus;           // hitStatus[hit]=kTRUE -use this hit in fit
    Float_t * TDCTime;            // Poimter to the array with drift times
    Float_t * xLeft;              // Poimter to the array with x-coord. of left edge of wire
    Float_t * yLeft;              // =====" " ====== y-coord. of left edge of wire
    Float_t * zLeft;              // =====" " ====== z-coord. of left edge of wire
    Float_t * xRight;             // =====" " ====== x-coord. of right edge of wire
    Float_t * yRight;             // =====" " ====== y-coord. of right edge of wire
    Float_t * zRight;             // =====" " ====== z-coord. of right edge of wire
    Float_t * qualityWeight;      // reflect number of fiered wiers per plane in cluster.
    Float_t dist[250];            // deviation of calculated and measured time.
    Float_t distWire[250];        // dist. to wires
    Float_t weight[250];          // ???????????

    Float_t arriveTime;           // Time-of-Flight
    Double_t parameter[6];        // Pointer to the array with parameters for functional
    Int_t iteration;              // loop variable
    Double_t   xInitPlane, yInitPlane, zInitPlane;   // coord of projection plane, same in sector coordinat system
    Double_t   distVertexMDC1, distVertexMDC2;       // distances from DMC, MDC2 to the target
    Double_t   distVertexMDC3, distVertexMDC4;       // distances from DMC, MDC2 to the target
    Double_t   driftVelocityMDC1, driftVelocityMDC2; // Drift velocity in MDC1&MDC2
    Double_t   driftVelocityMDC3, driftVelocityMDC4; // Drift velocity in MDC3&MDC4
    Double_t   pVelocity;                            // Particle Velocity
    Double_t   pVelocityErr;                         // Particle velocity error
    Double_t   xOutVertex, yOutVertex, zOutVertex;   // Final (after fitting) position of the vertex
    Double_t   xErrVertex, yErrVertex, zErrVertex;   // ====" "==== error
    Double_t   xOutPlane, yOutPlane, zOutPlane ;     // Final (after fitting) track coord. on the projection plane
    Double_t   xErrPlane, yErrPlane, zErrPlane ;     // ====" "==== error
    Double_t   funMin ;                              // value of the functional
    Bool_t fprint;                                   // Flag for control print-out
    TStopwatch* timer;
    Double_t functional, newFunctional;

    // Parameters for comparison fit. and cal. times
    HMdcTrackFitPar* fitPar;     // Pointer to fitting container
    HMdcCal2ParSim* fCal2ParSim;

  public:
    HMdcTrackFitter(Bool_t prnt=kFALSE);
    ~HMdcTrackFitter(void);
    void setPrintFlag(Bool_t prnt);
    void setVersion(Int_t ver) { version=ver; }
    //   HMdcTrackFitter(Int_t N1, Int_t N2);
    Bool_t init(void);
    Bool_t reinit(void);
    Int_t execute(Float_t threshold, Int_t iter=0);
    Bool_t finalize(void);
    Double_t calculationOfFunctional(Int_t iflag);
    Double_t deviation(Double_t xVertex, Double_t yVertex, Double_t zVertex,
		       Double_t xPlane, Double_t yPlane, Double_t zPlane,
		       Double_t particleVelocity, Int_t iflag);
    void setAddress(Int_t sec, Int_t seg) {sector=sec; segment=seg;}
    void setListModules(Int_t* lMod)  { lModule=lMod; }
    void setListLayers(Int_t* lLay)  { lLayer=lLay; }
    void setListCells(Int_t* lCells)  { lCell=lCells; }
    void setNMDC1Times( Int_t nT )    { nMDC1Times = nT; }
    void setNMDC2Times( Int_t nT )    { nMDC2Times = nT; }
    Int_t getNMDC1Times()    { return nMDC1Times; }
    Int_t getNMDC2Times()    { return nMDC2Times; }
    Bool_t belongHitTrack( Int_t nT )    { return((dist[nT] < 10) ? kTRUE : kFALSE); }
    Float_t getDeviation( Int_t nT )    { return dist[nT]; }
    Float_t getDistWire( Int_t nT )    { return distWire[nT]; }
    Float_t getTDCTime( Int_t nT )    { return TDCTime[nT]; }
    Float_t getWeight( Int_t nT )    { return weight[nT]; }
    void setSizeCellsLayer(HMdcSizesCellsLayer** fSCLayer)
        { fSizeCellsLayer=fSCLayer; }
    void setHitStatus (Char_t * hSt)  { hitStatus=hSt; }
    void setTDCTime   ( Float_t * t ) { TDCTime    = t; }
    void setXLeft     ( Float_t * x ) { xLeft      = x; }
    void setYLeft     ( Float_t * y ) { yLeft      = y; }
    void setZLeft     ( Float_t * z ) { zLeft      = z; }
    void setXRight    ( Float_t * x ) { xRight     = x; }
    void setYRight    ( Float_t * y ) { yRight     = y; }
    void setZRight    ( Float_t * z ) { zRight     = z; }
    void setXVertex    ( Double_t x ) { xVertex     = x; }
    void setYVertex    ( Double_t y ) { yVertex     = y; }
    void setZVertex    ( Double_t z ) { zVertex     = z; }
    void setQualityWeight    ( Float_t * qw ) { qualityWeight = qw; }
    void setXInitPlane ( Float_t  x ) { xInitPlane  = x; }
    void setYInitPlane ( Float_t  y ) { yInitPlane  = y; }
    void setZInitPlane ( Float_t  z ) { zInitPlane  = z; }
    void setDriftVelocityMDC1 ( Float_t  v ) { driftVelocityMDC1 = double(v); }
    void setDriftVelocityMDC2 ( Float_t  v ) { driftVelocityMDC2 = double(v); }
    void setDriftVelocityMDC3 ( Float_t  v ) { driftVelocityMDC3 = double(v); }
    void setDriftVelocityMDC4 ( Float_t  v ) { driftVelocityMDC4 = double(v); }
    void setDistVertexMDC1 ( Double_t  v ) { distVertexMDC1 = v; }
    void setDistVertexMDC2 ( Double_t  v ) { distVertexMDC2 = v; }
    void setDistVertexMDC3 ( Double_t  v ) { distVertexMDC3 = v; }
    void setDistVertexMDC4 ( Double_t  v ) { distVertexMDC4 = v; }
    Double_t getXOutPlane() { return xOutPlane; }
    Double_t getYOutPlane() { return yOutPlane; }
    Double_t getZOutPlane() { return zOutPlane; }
    Double_t getXOutVertex() { return xOutVertex; }
    Double_t getYOutVertex() { return yOutVertex; }
    Double_t getZOutVertex() { return zOutVertex; }
    Double_t getPVelocity() { return pVelocity; }
    Double_t getDistVertexMDC1() { return distVertexMDC1; }
    Double_t getDistVertexMDC2() { return distVertexMDC2; }
    Double_t getDistVertexMDC3() { return distVertexMDC3; }
    Double_t getDistVertexMDC4() { return distVertexMDC4; }
    Double_t getFunMin() { return funMin; }
    Float_t getArrTime() { return arriveTime; }
    Int_t getIterationNumber() { return iteration; }
    Double_t getFunctional(void);
    void printParam(Char_t* title) const;

  private:
    void setParam(void);
    void fillOutput(void);
    void printResult(Char_t* status) const;

    ClassDef(HMdcTrackFitter,0)
};

#endif
