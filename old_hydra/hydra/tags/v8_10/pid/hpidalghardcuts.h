#ifndef HPIDALGHARDCUTS_H
#define HPIDALGHARDCUTS_H

#include "hpidalgorithm.h"
#include "hpidhitdata.h"
#include "hpidtrackdata.h"
#include "hparset.h"
#include "hpidhistogramscont.h"
#include "hpidtrackcand.h"
#include "hpidtrackcandsim.h"
#include "hpidalgstandcutspar.h"
#include "hpidalghardcutspar.h"

#include "TNtuple.h"
#include "TFile.h"
#include "TString.h"
#include "TCutG.h"

// -----------------------------------------------------------------------------

class HPidAlgHardCuts : public HPidAlgorithm

{
public:

  HPidAlgHardCuts(TString InParamFile, TString outFileName="");
  HPidAlgHardCuts(Float_t fWeight, TString InParamFile, TString outFileName="");
   ~HPidAlgHardCuts(void) {}

    // -------------------------------------------------------------------------

    virtual Bool_t      init(void);
    virtual Bool_t      reinit(void);
    virtual Bool_t      finalize(void);

    virtual Bool_t      calculatePDF(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);
    virtual Bool_t      calculateCL(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);

    virtual void        print(void) const;

    // -------------------------------------------------------------------------
    Bool_t isLepInRich(Int_t iPid, HPidHitData *pHitData);
    Bool_t isProton(Int_t iPid, HPidTrackData *pTrackData, Int_t iSelectedMomAlg);
    Bool_t isPiP(Int_t iPid, HPidTrackData *pTrackData, Int_t iSelectedMomAlg);
    Bool_t isPiM(Int_t iPid, HPidTrackData *pTrackData, Int_t iSelectedMomAlg);
     
    Int_t getPid(HPidTrackCand *pTrack, Int_t nAlg);
    
    // -------------------------------------------------------------------------
    void setOutput(TString fname) { sOutFileName = fname;}; 
    void setSimulation() { bSim = kTRUE;};
    
    Bool_t openOutFile(void);
    Bool_t buildOutNtuple(void);
    
    Bool_t fillOutNtuple(Int_t iPid, HPidTrackCandSim *pTrack);
    Bool_t writeOutFile(void);
    
    ClassDef(HPidAlgHardCuts, 0)  // Algorithm PID for the hard cuts 

private:
    
    // hard cut parameters
    HPidAlgStandCutsPar *pParams;
    HPidAlgHardCutsPar *pParams2;
    
    TString sInFileName;            // input file for the graphical cut
    TString sOutFileName;           // output file for debug ntuple
    TFile   *pOutFile;              // output file 
    TNtuple *pNtuple;               // control ntuple
    
    Bool_t  bSim;      	            // check experiment or simulation 
    
    TCutG *GCut_8;                  // Graphical cut for pi+
    TCutG *GCut_9;                  // Graphical cut for pi-
    TCutG *GCut_14;                 // Graphical cut for p
    
    Bool_t bCut8;
    Bool_t bCut9;
    Bool_t bCut14;
    
};

// -----------------------------------------------------------------------------

#endif //HPidAlgHardCuts_H
