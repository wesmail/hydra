#ifndef HSUPROGRESS_H
#define HSUPROGRESS_H
#pragma interface

/* ******************************** */
/*                                  */
/*   Some progress bar functions    */
/*                                  */
/*                  MJ. 27.05.2001  */
/* ******************************** */

/* ******************************** */

#include <TStopwatch.h>

/* ******************************** */

class HSUProgress
{
public:

                HSUProgress();
                HSUProgress(int iMax);
    virtual    ~HSUProgress();

    /* **************************** */

    void        SetMaxValue(Int_t i)        { m_iMax = i;           }
    Int_t       GetMaxValue(void)           { return m_iMax;        }

    void        Reset(void);
    void        Restart(void);
    Int_t       GetLast(void)               { return m_iLast;       }
    void        Next(int iSteps = 1);
    void        Final(void);

    void        SetSpinOn(Bool_t b)         { m_bSpin = b;          }
    Bool_t      GetSpinOn(void)             { return m_bSpin;       }
    void        SetSpinPer(Float_t f);
    void        SetSpinEvents(Int_t i);
    const char* GetSpinChars(void)          { return m_psSpinChars; }
    void        SetSpinChars(const char *pStr);
    void        SetSpinChars(int iType);
    float       GetSpinReal(void);

    void        SetProgressOn(Bool_t b)     { m_bProgress = b;      }
    Bool_t      GetProgressOn(void)         { return m_bProgress;   }
    void        SetProgressPer(Float_t f);
    void        SetProgressEvents(Int_t i);
    char        GetProgressChar(void)       { return m_cProgressChar; }
    void        SetProgressChar(char c)     { m_cProgressChar = c;  }
    float       GetProgressReal(void);

    void        SetTimerOn(Bool_t b)        { m_bTimer = b;         }
    Bool_t      GetTimerOn(void)            { return m_bTimer;      }
    void        SetTimerPer(Float_t f);
    void        SetTimerEvents(Int_t i);
    float       GetTimerReal(void);

    void        SetFullTimerOn(Bool_t b)    { m_bFullTimer = b;     }
    Bool_t      GetFullTimerOn(void)        { return m_bFullTimer;  }

    /* **************************** */

private:

    Int_t       m_iMax;
    Int_t       m_iLast;

    Bool_t      m_bSpin;
    Bool_t      m_bSpinInPer;
    Float_t     m_fSpinSet;
    Float_t     m_fSpinReal;
    TString     m_psSpinChars;
    Int_t       m_iSpinLen;

    Bool_t      m_bProgress;
    Bool_t      m_bProgressInPer;
    Float_t     m_fProgressSet;
    Float_t     m_fProgressReal;
    char        m_cProgressChar;

    Bool_t      m_bTimer;
    Bool_t      m_bTimerInPer;
    Float_t     m_fTimerSet;
    Float_t     m_fTimerReal;

    Bool_t      m_bFullTimer;

    TStopwatch  tTimer;

    /* **************************** */

    ClassDef(HSUProgress, 1) // MProgress class
};

/* ******************************** */

#endif //!HSUPROGRESS_H
