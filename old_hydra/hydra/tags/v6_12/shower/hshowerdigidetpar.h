#ifndef HShowerDigiDetPar_H
#define HShowerDigiDetPar_H
#pragma interface

#include <TH1.h>
#include <TH2.h>
#include "hparset.h"
#include "hshowerparset.h"
#include "hlocation.h"
#include "hshowerpad.h"
#include "hshowerwire.h"
#include "hshowerframe.h"

class HParHadAsciiFileIo;

//
// v. 2.1
// MJ Thu Nov 29 22:30:16 CET 2001
//

//------------------------------------------------------------------------------

#define POLY_DEGREE 4

//------------------------------------------------------------------------------

class HShowerDigiDetPar : public HShowerParSet
{
public:

    HShowerDigiDetPar(const char* name="ShowerDigiPar",
                      const char* title="Digitisation parameters for Shower",
                      const char* context="ShowerStandardDigiDet");
   ~HShowerDigiDetPar();

    //--------------------------------------------------------------------------

    Float_t     getThickDet() const     { return fThickDet;             }
    Int_t       getMatrixRange() const  { return nMatrixRange;          }
    Float_t     getBoxSize() const      { return fBoxSize;              }
    Float_t     getChargeSlope() const  { return fChargeSlope;          }
    Float_t     getPlaneDist() const    { return fPlaneDist;            }
    Int_t       getPadRange() const     { return nPadRange;             }
    Float_t     getPadThreshold() const { return fPadThreshold;         }
    Float_t     getUpdatePadThreshold() const { return fUpdatePadThreshold; }
    Float_t     getMasterOffset() const { return fMasterOffset;         }

    const Float_t* getChargeMatrix()    { return pfChargeMatrix;        }
    const Float_t* getMeanParams()      { return afMeanParams;          }
    const Float_t* getSigmaParams()     { return afSigmaParams;         }
    const Float_t* getEffParams()       { return afEffParams;           }

    //--------------------------------------------------------------------------

    void setThickDet(Float_t fThick)    { fThickDet    = fThick;        }
    void setBoxSize(Float_t fSize)      { fBoxSize     = fSize;         }
    void setChargeSlope(Float_t fSlope) { fChargeSlope = fSlope;        }
    void setPlaneDist(Float_t fDist)    { fPlaneDist   = fDist;         }
    void setPadRange(Int_t nRange)      { nPadRange    = nRange;        }
    void setPadThreshold(Float_t fThreshold)
                                        { fPadThreshold = fThreshold;   }
    void setUpdatePadThreshold(Float_t fThreshold)
                                        { fUpdatePadThreshold = fThreshold; }
    void setMasterOffset(Float_t fMastOffset)
                                        { fMasterOffset = fMastOffset;  }

    void setChargeMatrix(Int_t nRange, const Float_t *pMatrix = NULL);
    void setMeanParams(const Float_t *pParams);
    void setSigmaParams(const Float_t *pParams);
    void setEffParams(const Float_t *pParams);

    //--------------------------------------------------------------------------

    const TH1F*     getEfficiencyHist(void) const { return phEff;       }
    const TH2F*     getQvBHist(void) const        { return ph2QvB;      }


   	void setEfficiencyHist(TH1F *h1) { phEff=h1;    }
    void setQvBHist(TH2F *h2)        { ph2QvB=h2;  	}
    //--------------------------------------------------------------------------

    virtual void    clear(void);
    virtual Bool_t  initAscii(HParHadAsciiFileIo  *pHadAsciiFile);
    virtual Bool_t  writeAscii(HParHadAsciiFileIo *pHadAsciiFile);

    Float_t         calcCharge(Float_t fCharge, Float_t fDist, Float_t fXd,
                                Float_t fYd, Float_t fXu, Float_t fYu);
    void            initChargeMatrix();

    //--------------------------------------------------------------------------

    Float_t         getEfficiency(Float_t fBeta) const;
    Bool_t          checkEfficiency(Float_t fBeta) const;
    Float_t         getCharge(Float_t fBeta) const;
    Bool_t          initSumVerBetaHistograms(void);

    //--------------------------------------------------------------------------

private:


    //--------------------------------------------------------------------------

private:
    Float_t  fThickDet;           // thickness of gas chamber
    Int_t    nMatrixRange;        // size of pfChargeMatrix = 2*nMatrixRange+1
    Float_t  fBoxSize;            // size of element of charge density matrix
    Float_t  fChargeSlope;        // charge distribution slope parameter
    Float_t  fPlaneDist;          // plane cathod to sense wires plane distance
    Int_t    nPadRange;           // range of integration area
    Float_t  fPadThreshold;       // charge threshold for a pad
    Float_t  fUpdatePadThreshold; // charge threshold for track updating
    Float_t  fMasterOffset;       // offset for HShowerCopy

    Float_t *pfChargeMatrix;      // charge density matrix

        // fit parameters
    Float_t  afMeanParams[POLY_DEGREE];  // mean of landau distribution (beta)
    Float_t  afSigmaParams[POLY_DEGREE]; // sigma of landau distribution (beta)
    Float_t  afEffParams[POLY_DEGREE];   // efficiency fit (beta)

    TH1F      *phEff;             // efficiency(beta) histogram
    TH2F      *ph2QvB;            // Q(beta) propability histogram

    TObjArray *pArrayQvB;         // array of histograms
    Double_t   dQvSmin;
    Double_t   dQvSmax;
    Int_t      iQvSbins;
    Double_t   d1_QvSDiff;
    Double_t   dQvSOneBin;

    //--------------------------------------------------------------------------

    ClassDef(HShowerDigiDetPar, 3) //ROOT extension
};
#endif
