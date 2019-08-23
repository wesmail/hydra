#ifndef HShowerCalPar_H
#define HShowerCalPar_H
#pragma interface

#include "hparset.h"
#include "hshowerparset.h"
#include "hlocation.h"
#include "hobjtable.h"
#include "hshowercalparcell.h"

class HParHadAsciiFileIo;

// -----------------------------------------------------------------------------

enum enum_HSCPM_Var
{
    HSCPV_SLOPE = 0,
    HSCPV_OFFSET,
    HSCPV_THRESHOLD,
    HSCPV_GAIN,
    HSCPV_MAX
};

enum enum_HSCPM_Mod
{
    HSCPM_SET = 0,
    HSCPM_ADD,
    HSCPM_MULT
};

// -----------------------------------------------------------------------------

class HShowerCalPar : public HShowerParSet
{
public:
    HShowerCalPar(const char* name="ShowerCalPar",
                  const char* title="Calibration parameters for Shower",
                  const char* context="");
   ~HShowerCalPar();

    void    setSetup(Int_t nSectors, Int_t nModules,
                        Int_t nRows, Int_t nColumns);
    Int_t   getNSectors()   { return m_nSectors; }
    Int_t   getNModules()   { return m_nModules; }
    Int_t   getNRows()      { return m_nRows;    }
    Int_t   getNColumns()   { return m_nColumns; }


    void    setCellClassName(Char_t* pszName);
    Char_t* getCellClassName();

    HShowerCalParCell*& getSlot(HLocation &loc);
    HShowerCalParCell*  getObject(HLocation &loc);

    Float_t getOffset(HLocation &loc);
    Float_t getSlope(HLocation &loc);
    Float_t getThreshold(HLocation &loc);
    Float_t getGain(HLocation &loc);

    void    setOffset(HLocation &loc, Float_t fOffset);
    void    setSlope(HLocation &loc, Float_t fSlope);
    void    setThreshold(HLocation &loc, Float_t fThreshold);
    void    setGain(HLocation &loc, Float_t fGain);

    virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
    virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);

    virtual Bool_t defaultInit();

    void    Print();

    void    setWriteForElectronic(Bool_t bWrite)
                                    { m_bWriteForElectronic = bWrite; }
    Bool_t  getWriteForElectronic(void)
                                    { return m_bWriteForElectronic;   }

    void    setSlopesToValue(Float_t fSlope)
							{ changeValues(HSCPV_SLOPE, HSCPM_SET, fSlope); }
    void    addValueToSlopes(Float_t fAdd)
							{ changeValues(HSCPV_SLOPE, HSCPM_ADD, fAdd); }
    void    multiplySlopesByValue(Float_t fM)
							{ changeValues(HSCPV_SLOPE, HSCPM_MULT, fM); }

    void    setOffsetsToValue(Float_t fOffset)
							{ changeValues(HSCPV_OFFSET, HSCPM_SET, fOffset);}
    void    addValueToOffsets(Float_t fAdd)
							{ changeValues(HSCPV_OFFSET, HSCPM_ADD, fAdd); }
    void    multiplyOffsetsByValue(Float_t fM)
							{ changeValues(HSCPV_OFFSET, HSCPM_MULT, fM); }

    void    setThresholdsToValue(Float_t fThreshold)
							{ changeValues(HSCPV_THRESHOLD, HSCPM_SET,
                                                fThreshold); }
    void    addValueToThresholds(Float_t fAdd)
							{ changeValues(HSCPV_THRESHOLD, HSCPM_ADD, fAdd);}
    void    multiplyThresholdsByValue(Float_t fM)
							{ changeValues(HSCPV_THRESHOLD, HSCPM_MULT, fM); }
    void    setGainsToValue(Float_t fGain)
							{ changeValues(HSCPV_GAIN, HSCPM_SET, fGain); }
    void    addValueToGains(Float_t fAdd)
							{ changeValues(HSCPV_GAIN, HSCPM_ADD, fAdd); }
    void    multiplyGainsByValue(Float_t fM)
							{ changeValues(HSCPV_GAIN, HSCPM_MULT, fM); }

private:
    void    changeValues(int iVar, int iOpr, Float_t f);

    ClassDef(HShowerCalPar, 2)  //ROOT extension

private:
    Int_t   m_nSectors;         //sizes of calibration structure
    Int_t   m_nModules;
    Int_t   m_nRows;
    Int_t   m_nColumns;

    Bool_t  m_bWriteForElectronic;//!

    Char_t  m_szClassName[40];  //name of class used for keeping data for
                                //calibration of one pad

    HObjTable m_ParamsTable;    //structure which contains all object
                                //for calibration of one pad
};

#endif
