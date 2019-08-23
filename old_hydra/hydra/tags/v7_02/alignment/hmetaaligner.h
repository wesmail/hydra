#ifndef HMETAALIGNER_H
#define HMETAALIGNER_H

// -----------------------------------------------------------------------------

#include <TROOT.h>
#include <TNtuple.h>
#include "hruntimedb.h"
#include "hgeomrotation.h"
#include "hgeomvector.h"

// -----------------------------------------------------------------------------

#define kMETA_ALIGNER_MATRICS             8
#define kMETA_ALIGNER_PARAMS              6

#define kMATA_ALIGN_DEFAULT_STEP          0.01
#define kMATA_ALIGN_DEFAULT_ERROR         0.0001
#define kMATA_ALIGN_DEFAULT_STEPS       500.0
#define kMATA_ALIGN_DEFAULT_TUKAY_CONST   6.0

// -----------------------------------------------------------------------------

class HMetaAligner : public TObject
{
public:
    Int_t            m_iSector;
    Int_t            m_iSystem;         // detector type: 0: shower, 1: tof

    Int_t            m_iVerbose;        // printing level
    Bool_t           m_bInitOK;
    Bool_t           m_bNormalize;      // normalize chi^2 to the entries
    Bool_t           m_bEqualSigmas;    // set all sigmas to 11.0

    Int_t            m_iMaxEntries;     // entries in the input file
    Int_t            m_iEntries;        // entries to calculate

    Double_t         m_dStartParams[kMETA_ALIGNER_PARAMS];// start parameters
    Double_t         m_dParams[kMETA_ALIGNER_PARAMS];     // calculated values
    Double_t         m_dSteps[kMETA_ALIGNER_PARAMS];      // step value of param
    Double_t         m_dStartErrors[kMETA_ALIGNER_PARAMS];// start errors
    Double_t         m_dErrors[kMETA_ALIGNER_PARAMS];     // calculated errors
    Bool_t           m_bFixed[kMETA_ALIGNER_PARAMS];      // if fixed

    Double_t         m_dError;          // error value for MINUIT
    Double_t         m_dMigSteps;       // max. number of steps
    Bool_t           m_bImprove;        // inprove the results

    Bool_t           m_bUseTukay;       // use weights
    Double_t         m_dTukayConst;     // const for weights
    Double_t         m_dTukayConst_2;   // m_dTukayConst ^ 2

    Bool_t           m_bChangeWeights;

    Double_t         m_dLastChi_2;      // last value of chi^2
    Int_t            m_iStatus;         // status of the fit
    Int_t            m_iHits;           // number of hits for the detector
    Int_t            m_iLastThrown;     // number of hits with weight = 0
    Int_t            m_iCalls;          // number of iterations

    Double_t         m_dChi2XSum;       // chi^2 for X distance
    Double_t         m_dChi2YSum;       // chi^2 for Y distance

    Double_t         m_dWeightChi2X;    // weight for chi^2 X
    Double_t         m_dWeightChi2Y;    // weight for chi^2 Y

protected:
    TFile           *m_pTupleFile;      // file with input data
    TNtuple         *m_pTuple;          // pointer to NTuple

    HRuntimeDb      *m_pRTDB;           // Runtime dB
    HGeomRotation    m_arRI[kMETA_ALIGNER_MATRICS]; // inv modules' matrices
    HGeomVector      m_avT[kMETA_ALIGNER_MATRICS];  // translations vectors

    Float_t          m_fXc;             // data from NTuple
    Float_t          m_fYc;
    Float_t          m_fZc;
    Float_t          m_fAlphaX;
    Float_t          m_fAlphaY;
    Float_t          m_fAlphaZ;
    Float_t          m_fXo;
    Float_t          m_fYo;
    Float_t          m_fSigXo;
    Float_t          m_fSigYo;
    Float_t          m_fSec;
    Float_t          m_fMod;
    Float_t          m_fSys;

    Int_t            m_iMethod;         // MINUIT method id

    Double_t         m_dPrevSigma;      // params when weights const changes
    Double_t         m_dWeightsSum0;
    Double_t         m_dWeightsSum1;

    // -------------------------------------------------------------------------

public:
             HMetaAligner(const char *pFileName, Int_t iSector,
                        Int_t iSystem, const char *pParamFile, Int_t iRunId);

            ~HMetaAligner(void);

    void     setValue(Int_t iParam, Double_t dVal = 0.0f);
    void     setStep(Int_t iParam, Double_t dStep = kMATA_ALIGN_DEFAULT_STEP);
    void     fixParam(Int_t iParam, Bool_t bFix = kTRUE);
    void     releaseParam(Int_t iParam);
    void     releaseAll(void);
    void     fixAll(void);
    void     reversFixed(void);
    void     setError(Double_t d = kMATA_ALIGN_DEFAULT_ERROR);
    void     setMigRad(Double_t d = kMATA_ALIGN_DEFAULT_ERROR);
    void     setTukayConst(Double_t d = kMATA_ALIGN_DEFAULT_TUKAY_CONST);
    void     setNormalize(Bool_t bSet = kTRUE);
    void     setImprove(Bool_t bSet = kTRUE);
    void     setEntries(Int_t i = 0);
    void     copyResultsToStart(void);

    virtual Bool_t runFit(Bool_t bUseTukay = kTRUE,
                        const char *pOut = "out.root", Int_t iMethod = 0);
    virtual Bool_t contFit(Bool_t bUseTukay = kTRUE,
                        const char *pOut = "out.root", Int_t iMethod = 0);
    virtual void minAlign(Int_t &npar, Double_t *gin, Double_t &f,
                            Double_t *par, Int_t iflag);

    void     print(const char *pFileName = NULL) const;
    void     printLine(const char *pFileName = NULL) const;
    void     printHtmlRow(const char *pFileName = NULL) const;
    void     writeResults(const char *pFileName);

    // -------------------------------------------------------------------------

private:

    void     init(void);
    Bool_t   loadParameters(const char *pParamFileName, Int_t iRunId);
    Bool_t   openNTuple(const char *pFileName);
    Bool_t   getShowerGeom(const char *pParamFile, Int_t iRunId);
    Bool_t   getTofGeom(const char *pParamFile, Int_t iRunId);

    virtual Double_t funcResiduals(HGeomVector *pvDT,
                            HGeomRotation *prDRot = NULL);

    // -------------------------------------------------------------------------

    Double_t m_dDX;
    Double_t m_dDY;
    Double_t m_dChi2;
    Double_t m_dChi2W;
    Double_t m_dChi2X;
    Double_t m_dChi2Y;

    // -------------------------------------------------------------------------

    ClassDef(HMetaAligner, 0)
};

// -----------------------------------------------------------------------------

#endif // HMETAALIGNER_H
