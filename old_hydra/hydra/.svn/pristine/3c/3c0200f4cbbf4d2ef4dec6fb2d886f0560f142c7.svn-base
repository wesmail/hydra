#ifndef HMDCTRACKFITTER_H
#define HMDCTRACKFITTER_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "iostream.h"
#include "hlocation.h"
#include "TH2.h"
#include "TCanvas.h"
//#include "hmdclookuptb.h"

//extern void fcn(Int_t &npar, Double_t *gin, Double_t &f, Double_t *x, Int_t iflag);

class HIterator;
class HMdcTrackFitPar;

class HMdcTrackFitter : public TObject {

 private:

  Int_t numberOfParameter;    // Number of parameters for fit procedure
  Int_t maxNMdcTimes;         // Maximum number of signals in MDC
  Int_t maxIteration;         // Maximum number of interations for fit
  Int_t i; 
  Double_t stepNorm;          // Normalisation factor for step
  Double_t xVertex, 
    yVertex, zVertex;         // Coordinates for vertex
  Double_t  particleVelocity; // Particle velocity
  Double_t *weightPar;        // Pointer to the weight array for parameters 
  Double_t *step;             // Pointer to the array with steps 
                              // for chanching of parameters
  Double_t cnst;              // Tukey constant
  Int_t NMDC1Times, NMDC2Times; // Number of fired wires in MDC1&MDC2
  Float_t * TDCTime;            // Poimter to the array with drift times
  Float_t * xLeft;              // Poimter to the array with x-coord. of left edge of wire  
  Float_t * yLeft;              // =====" " ====== y-coord. of left edge of wire
  Float_t * zLeft;              // =====" " ====== z-coord. of left edge of wire    
  Float_t * xRight;             // =====" " ====== x-coord. of right edge of wire
  Float_t * yRight;             // =====" " ====== y-coord. of right edge of wire
  Float_t * zRight;             // =====" " ====== z-coord. of right edge of wire
  Float_t * qualityWeight;      // reflect number of fiered wiers per plane in cluster.
  Float_t * dist;               // deviation of calculated and measured time.
  Float_t * weight;             // ???????????
  Float_t arriveTime;           // Time-of-Flight
  Double_t * parameter;         // Pointer to the array with parameters for functional
  Int_t iteration;              // loop variable
  Double_t   xInitPlane, yInitPlane, zInitPlane;   // coord of projection plane, same in sector coordinat system
  Double_t   distVertexMDC1, distVertexMDC2;       // distances from DMC, MDC2 to the target
  Double_t   driftVelocityMDC1, driftVelocityMDC2; // Drift velocity in MDC&MDC2   
  Double_t   pVelocity;                            // Particle Velocity
  Double_t   pVelocityErr;                         // Particle velocity error
  Double_t   xOutVertex, yOutVertex, zOutVertex;   // Final (after fitting) position of the vertex
  Double_t   xErrVertex, yErrVertex, zErrVertex;   // ====" "==== error
  Double_t   xOutPlane, yOutPlane, zOutPlane ;     // Final (after fitting) track coord. on the projection plane
  Double_t   xErrPlane, yErrPlane, zErrPlane ;     // ====" "==== error
  Double_t   funMin ;                              // value of the functional                            
  Bool_t fprint;                                   // Flag for control print-out
  TH1F *hdistvertexmdc;                            // pointer to the histogram with distances MDC2-target

  // Parameters for comparison fit. and cal. times
  Int_t myEvtNum;               // from me, just to culculate the number of clusters
  TFile *file;                  // File for storing histograms
  TCanvas *c1, *c2;             // Canvases for online dysplaing cal-fit time difference
 // Histograms for cal-fit time difference
  TH1F *hCalFit2_44, *hCalFit2_45, *hCalFit2_46, *hCalFit2_47, *hCalFit2_48, 
    *hCalFit2_49, *hCalFit2_50, *hCalFit2_51, *hCalFit2_52, *hCalFit2_53;
  TH1F *hCalFit5_44, *hCalFit5_45, *hCalFit5_46, *hCalFit5_47, *hCalFit5_48, 
    *hCalFit5_49, *hCalFit5_50, *hCalFit5_51, *hCalFit5_52, *hCalFit5_53;
  TH2F *hcalfit_2dim;
  Int_t flag_cell;             // Number of the celected fired cell in cluster 
  Int_t lay, cell;             // Layer and cell number for celected wire
  Double_t time_cal[100];      // Array for keeping cal. time
  HMdcTrackFitPar* fitPar;     // Pointer to fitting container
  
 public:
  
  HMdcTrackFitter(void);
  ~HMdcTrackFitter(void);
  void setFlag(int i){flag_cell=i;}
  void setLayCell(int i, int j){lay=i, cell=j;}
  //   HMdcTrackFitter(Int_t N1, Int_t N2);
  Bool_t init(void);
  void setParContainers();      
  Int_t execute(Float_t threshold);   
  Bool_t finalize(void);
  Double_t calculationOfFunctional(Int_t iflag);
  Double_t deviation(Double_t xVertex, Double_t yVertex, Double_t zVertex,
		     Double_t xPlane, Double_t yPlane, Double_t zPlane,
		     Double_t particleVelocity, Int_t iflag);
  void setNMDC1Times( Int_t nT )    { NMDC1Times = nT; };
  void setNMDC2Times( Int_t nT )    { NMDC2Times = nT; };
  Int_t getNMDC1Times()    { return NMDC1Times; };
  Int_t getNMDC2Times()    { return NMDC2Times; };
  Bool_t belongHitTrack( Int_t nT )    { return((dist[nT] < 10) ? kTRUE : kFALSE); };
  Float_t getDeviation( Int_t nT )    { return dist[nT]; };
  Float_t getWeight( Int_t nT )    { return weight[nT]; };   
  void setTDCTime   ( Float_t * t ) { TDCTime    = t ;}
  void setXLeft     ( Float_t * x ) { xLeft      = x; }
  void setYLeft     ( Float_t * y ) { yLeft      = y; }
  void setZLeft     ( Float_t * z ) { zLeft      = z; }
  void setXRight    ( Float_t * x ) { xRight     = x; }
  void setYRight    ( Float_t * y ) { yRight     = y; }
  void setZRight    ( Float_t * z ) { zRight     = z; }
  void setQualityWeight    ( Float_t * qw ) { qualityWeight = qw; }
  void setXInitPlane ( Float_t  x ) { xInitPlane  = x; }
  void setYInitPlane ( Float_t  y ) { yInitPlane  = y; }
  void setZInitPlane ( Float_t  z ) { zInitPlane  = z; }
  void setDriftVelocityMDC1 ( Float_t  v ) { driftVelocityMDC1 = double(v); }
  void setDriftVelocityMDC2 ( Float_t  v ) { driftVelocityMDC2 = double(v); }
  void setDistVertexMDC1 ( Double_t  v ) { distVertexMDC1 = v; }
  void setDistVertexMDC2 ( Double_t  v ) { distVertexMDC2 = v; }
  Double_t getXOutPlane() { return xOutPlane; }
  Double_t getYOutPlane() { return yOutPlane; }
  Double_t getZOutPlane() { return zOutPlane; }
  Double_t getXOutVertex() { return xOutVertex; }
  Double_t getYOutVertex() { return yOutVertex; }
  Double_t getZOutVertex() { return zOutVertex; }
  Double_t getPVelocity() { return pVelocity; }
  Double_t getDistVertexMDC1() { return distVertexMDC1; }
  Double_t getDistVertexMDC2() { return distVertexMDC2; }
  Double_t getFunMin() { return funMin; }
  Float_t getArrTime() { return arriveTime; }
  Int_t getIterationNumber() { return iteration; }
};
// supply missing math functions
inline double abs(double a)
{
  return (a < 0)? -a : a;
}
#endif
