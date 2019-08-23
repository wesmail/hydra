#ifndef HPIDALGSTANDCUTS_H
#define HPIDALGSTANDCUTS_H

#include "hpidalgorithm.h"
#include "hpidhitdata.h"
#include "hpidtrackdata.h"
#include "hshowerhitfpar.h"
#include "hparset.h"
#include "hpidhistogramscont.h"
#include "hpidtrackcand.h"
#include "hpidtrackcandsim.h"
#include "hpidalgstandcutspar.h"

#include "TNtuple.h"
#include "TFile.h"
#include "TString.h"
#include "TF1.h"

// -----------------------------------------------------------------------------

class HParSet;
class HPidAlgStandCuts : public HPidAlgorithm
{
public:

	HPidAlgStandCuts(TString outFileName="");
	HPidAlgStandCuts(Float_t fWeight,TString outFileName="");
   ~HPidAlgStandCuts(void) {}

    // -------------------------------------------------------------------------

    virtual Bool_t      init(void);
    virtual Bool_t      reinit(void);
    virtual Bool_t      finalize(void);

    virtual Bool_t      calculatePDF(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);
    virtual Bool_t      calculateCL(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);

    virtual void        print(void) const;

  
	void setPidParams(HPidAlgStandCutsPar *pP)  { pParams = pP;};
	const HPidAlgStandCutsPar* getPidParams(void) const {return pParams; };
	
	Bool_t isLepInRich(Int_t iPid,HPidHitData *pHitData);
	Bool_t isLepInTofino(Int_t iPid,HPidHitData *pHitData, HPidTrackData *pTrackData);
	Bool_t isLepInTof(Int_t iPid,HPidHitData *pHitData, HPidTrackData *pTrackData);
	Bool_t isLepInShower(Int_t iPid,HPidHitData *pHitData, HPidTrackData *pTrackData);
	Bool_t isLepInMdc(Int_t iPid,HPidHitData *pHitData, HPidTrackData *pTrackData);
    // -------------------------------------------------------------------------
	
	void setOutFileName(TString outFile) { sOutFileName = outFile; };
	TString  getOutFileName(void) 		 { return sOutFileName;    };

	Bool_t openOutFile(void);
	Bool_t buildOutNtuple(void);

	Bool_t fillOutNtuple(Int_t iPid, HPidTrackCandSim *pTrack);
	Bool_t writeOutFile(void);

    // -------------------------------------------------------------------------
	void clear(void);
    // -------------------------------------------------------------------------

    ClassDef(HPidAlgStandCuts, 0)  // Algorithm PID for the standard cuts 

private:
                          
	// hard cut parameters
	HPidAlgStandCutsPar *pParams;

    // -------------------------------------------------------------------------
	
	TString sOutFileName;   // name of the output file with ntuple
	TFile   *pOutFile;      // output file 
	TNtuple *pNtuple;       // control ntuple

	TF1* pMdcDiscriminator;  // Function used to decide upon leptonic id based upon energy loss in mdc
    // -------------------------------------------------------------------------

	Bool_t  bSim;      		// check experiment or simulation 
	Int_t   iSimEventFlag;  // simulation event structure flag   

    // -------------------------------------------------------------------------
	
	Bool_t bRich; 	  // lepton in Rich  
	Bool_t bMdc; 	  // lepton in Mdc  
	Bool_t bShower;   // lepton in Shower
	Bool_t bTofino;   // lepton in Tofino
	Bool_t bTof;      // lepton in Tof

    // -------------------------------------------------------------------------

};

// -----------------------------------------------------------------------------

#endif //HPidAlgStandCuts_H
