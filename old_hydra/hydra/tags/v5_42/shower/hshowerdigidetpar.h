#ifndef HShowerDigiDetPar_H
#define HShowerDigiDetPar_H
#pragma interface

#include "hparset.h"
#include "hshowerparset.h"
#include "hlocation.h"
#include "hshowerpad.h"
#include "hshowerwire.h"
#include "hshowerframe.h"

class HParHadAsciiFileIo;

//
// v. 2.0
// MJ Wed Aug  1 12:26:21 CEST 2001
//

//------------------------------------------------------------------------------

#define POLY_DEGREE 4

//------------------------------------------------------------------------------

class HShowerDigiDetPar : public HShowerParSet
{
public:

    HShowerDigiDetPar();
   ~HShowerDigiDetPar();

    //--------------------------------------------------------------------------

    Float_t     getThickDet()           { return fThickDet;             }
    Int_t       getMatrixRange()        { return nMatrixRange;          }
    Float_t     getBoxSize()            { return fBoxSize;              }
    Float_t     getChargeSlope()        { return fChargeSlope;          }
    Float_t     getPlaneDist()          { return fPlaneDist;            }
    Int_t       getPadRange()           { return nPadRange;             }
    Float_t     getPadThreshold()       { return fPadThreshold;         }
    Float_t     getUpdatePadThreshold() { return fUpdatePadThreshold;   }
    Float_t     getMasterOffset()       { return fMasterOffset;         }

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

    virtual void    clear();
    virtual Bool_t  initAscii(HParHadAsciiFileIo  *pHadAsciiFile);
    virtual Bool_t  writeAscii(HParHadAsciiFileIo *pHadAsciiFile);

    Float_t         calcCharge(Float_t fCharge, Float_t fDist, Float_t fXd,
                                Float_t fYd, Float_t fXu, Float_t fYu);
    void            initChargeMatrix();

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

    //--------------------------------------------------------------------------

    ClassDef(HShowerDigiDetPar, 2) //ROOT extension
};
#endif
