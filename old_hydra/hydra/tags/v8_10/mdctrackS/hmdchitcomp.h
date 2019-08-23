# ifndef HMDCHITCOMP_H
# define HMDCHITCOMP_H

# include "TH1.h"
# include "TH2.h"
# include "TMatrix.h"
# include "hcategory.h"
# include "hlocation.h"
# include "hreconstructor.h"

class HMdcCal3Sim;
class HMdcHitAux;
class HMdcModuleGeometry;
class HMdcTargetGeometry;

class HMdcHitComp : public HReconstructor{
private:
  HCategory* fRecCat;                // category of reconstructed (HMdcHitAux)
  HCategory* fSimCat;               // category of AuxSim data
  HCategory* fCal3Cat;              // category of cal3 data
  HMdcModuleGeometry* geo;                 // !mdc geometry
  HMdcTargetGeometry* target;           // !target geometry
  HLocation fLoc;                    // location of the reconstructor

  Int_t nseg6, nseg5, nseg4;          // number of rec hits with 6,5,4 cells
  Int_t nsim6, nsim5, nsim4, nsim3, nsim2, nsim1;     // number of geant hits with 6,5,4,3,2,1 cells
  Int_t n6to6, n6to5, n6to4; 
  Int_t fn6to6, fn6to5, fn6to4;
  Int_t n5to5,n5to4;
  Int_t fn5to5, fn5to4;
  Int_t fn4to4;
  Int_t n4to4;         // nptoq: geant hits with p cells reconstructed with q cells
  Int_t norec6, norec5, norec4; // no reconstructed hits, which come from the simulation with 6 or 5 cells
  Int_t ghost6, ghost5, ghost4;     //number of ghosts hits with 6,5,4 cells
  Int_t mix6to5, mix6to4, mix5to4; // number of mixed hits with 5cells+1 i.e.
  Int_t fmix6to5, fmix6to4, fmix5to4;
  Int_t mix6to4plus1;              // 6cells-simulated hit -> (4+1) hit
  Int_t fmix6to4plus1;
  Int_t n5to6, n4to6, n4to5;
  Int_t fn5to6, fn4to6, fn4to5;

  TH1F* hNHits;                  // number of cells per hit (sim hit)
  TH1F** hCoord;                 // coordinate in each plane for sim hits
  TH1F** hx;                     // x distribution per plane
  TH1F** hy;                     // y distribution per plane
  TH1F** hres;                   // residues in each plane
			
  TH1F *hxSlope, *hySlope;         // xslope and y slope distribution(rec)
  TH1F *hRecHits;                 // number of cells per reconstructed hit
  TH1F *hTracks;                  // number of reconstructed hits per event
  TH1F *hChi, *hChi6, *hChi5;       // chi2, chi2 of hits with 6 or 5 cells
  TH1F *hChi6to6, *hChi6to5, *hChi5to5; // hChiptoq: chi2 of hits originally with p cells and reconstructed with q cells
  TH1F *hChiGhost6, *hChiGhost5;    // chi2 of ghosts with 6 of 5 cells
  TH1D *hProb, *hProb6, *hProb5;   // probability
  TH2F *hYX, *hXslopeX, *hYslopeY, *hXslopeYslope; 

  TH1F *hDistanceToTarget, *hDistanceToTargetWellRec, *hDistanceToTargetNoWR;
  TH1F *hDistanceToTargetOverErr, *hDistanceToTargetOverErrWellRec, *hDistanceToTargetOverErrNoWR;
  TH1F *hDistanceToZ, *hDistanceToZWellRec, *hDistanceToZNoWR;
  TH1F *hDistanceToZOverErr, *hDistanceToZOverErrWellRec, *hDistanceToZOverErrNoWR;
  TH1F *hDiffRecSimXSlopeOverErr, *hDiffRecSimYSlopeOverErr;
  TH1F *hDiffRecSimX, *hDiffRecSimY, *hDiffRecSimXSlope, *hDiffRecSimYSlope;


  Int_t fEventId;

  void setParContainers(void);
  void bookHisto(void);
  void deleteHisto(void);
  void buildHitAuxSimCat(void);
  void addCal3ToHitAux(HMdcCal3Sim* );
  Float_t distanceToTarget(HMdcHitAux* seg);
  Float_t distanceToTargetOverErr(HMdcHitAux* seg);
  Float_t distanceToZ(HMdcHitAux* seg);
  Float_t distanceToZOverErr(HMdcHitAux* seg);
  TMatrix calculateSlopes(HMdcHitAuxSim* sim, Float_t z);



public:
  HMdcHitComp(void);
  HMdcHitComp(Text_t* name, Text_t* title);
  ~HMdcHitComp(void);
  void setLoc(HLocation &location);
  void printStat(void);                   
  void printStat(Text_t* file);

  Int_t execute(void);
  Bool_t init(void);
  Bool_t finalize(void){return kTRUE;}

ClassDef(HMdcHitComp,1)
};





# endif /* !HMDCHITCOMP_H */


