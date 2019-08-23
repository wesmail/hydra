#pragma implementation

/* ******************************** */
/*                                  */
/*   Some progress bar functions    */
/*                                  */
/*                  MJ. 27.05.2001  */
/* ******************************** */

#include <Rtypes.h>
#include <TString.h>
#include <stdio.h>
#include "hsuprogress.h"

ClassImp(HSUProgress)

/* ******************************** */

static const char* g_pctHSUProgressConst[] = { ".oOo", "-\\|/" };

/* ******************************** */

HSUProgress::HSUProgress()
{
    Reset();
}

/* ******************************** */

HSUProgress::HSUProgress(int iMax)
{
    Reset();
    m_iMax = iMax;
}

/* ******************************** */

void HSUProgress::Reset()
{
    m_iMax           = 100;
    m_iLast          = -1;

    m_bSpin          = kTRUE;
    m_bSpinInPer     = kFALSE;
    m_fSpinSet       = 0.1f;
    m_fSpinReal      = 0.0f;
    m_psSpinChars    = g_pctHSUProgressConst[0];
    m_iSpinLen       = strlen(m_psSpinChars);

    m_bProgress      = kTRUE;
    m_bProgressInPer = kTRUE;
    m_fProgressSet   = 1.0f;
    m_fProgressReal  = 0.0f;
    m_cProgressChar  = '#';

    m_bTimer         = kTRUE;
    m_bTimerInPer    = kTRUE;
    m_fTimerSet      = 10.0f;
    m_fTimerReal     = 0.0f;

    m_bFullTimer     = kTRUE;

    tTimer.Reset();
}

/* ******************************** */

HSUProgress::~HSUProgress()
{
}

/* ******************************** */

void HSUProgress::Restart(void)
{
    m_iLast = -1;
}

/* ******************************** */

void HSUProgress::SetSpinPer(Float_t f)
{
    m_bSpinInPer = kTRUE;
    m_fSpinSet = (f <= 0.0f) ? 0.01f : f;
}

/* ******************************** */

void HSUProgress::SetSpinEvents(Int_t i)
{
    m_bSpinInPer = kFALSE;
    m_fSpinSet = (i <= 0) ? 1.0f : (Float_t)i;
}

/* ******************************** */

void HSUProgress::SetSpinChars(const char *pStr)
{
    m_psSpinChars = pStr;
    m_iSpinLen    = strlen(m_psSpinChars);
}

/* ******************************** */

void HSUProgress::SetSpinChars(int iType)
{
    if((iType >= 0) && (iType < (int)sizeof(g_pctHSUProgressConst)))
        m_psSpinChars = g_pctHSUProgressConst[iType];

    m_iSpinLen = strlen(m_psSpinChars);
}

/* ******************************** */

float HSUProgress::GetSpinReal(void)
{
    m_fSpinReal = (m_bSpinInPer)
                    ? m_fSpinSet : (100.0f * m_fSpinSet / m_iMax);

    return m_fSpinReal;
}

/* ******************************** */

void HSUProgress::SetProgressPer(Float_t f)
{
    m_bProgressInPer = kTRUE;
    m_fProgressSet = (f <= 0.0f) ? 0.01f : f;
}

/* ******************************** */

void HSUProgress::SetProgressEvents(Int_t i)
{
    m_bProgressInPer = kFALSE;
    m_fProgressSet = (i <= 0) ? 1.0f : (Float_t)i;
}

/* ******************************** */

float HSUProgress::GetProgressReal(void)
{
    m_fProgressReal = (m_bProgressInPer)
                    ? m_fProgressSet : (100.0f * m_fProgressSet / m_iMax);

    return m_fProgressReal;
}

/* ******************************** */

void HSUProgress::SetTimerPer(Float_t f)
{
    m_bTimerInPer = kTRUE;
    m_fTimerSet = (f <= 0.0f) ? 0.01f : f;
}

/* ******************************** */

void HSUProgress::SetTimerEvents(Int_t i)
{
    m_bTimerInPer = kFALSE;
    m_fTimerSet = (i <= 0) ? 1.0f : (Float_t)i;
}


/* ******************************** */

float HSUProgress::GetTimerReal(void)
{
    m_fTimerReal = (m_bTimerInPer)
                    ? m_fTimerSet : (100.0f * m_fTimerSet / m_iMax);

    return m_fTimerReal;
}

/* ******************************** */

void HSUProgress::Next(int iSteps)
{
Float_t fPrev;
Float_t fCur;
Bool_t  iChangeSpin     = 0;
Bool_t  bChangeSpin     = kFALSE;
Bool_t  iChangeProgress = 0;
Bool_t  bChangeTimer    = kFALSE;
int     i;

    if((m_iMax <= 0) || (m_iLast >= m_iMax))
    {
        m_iLast += iSteps;
        printf("Progress out of range: %d / %d\n", m_iLast, m_iMax);
        return;
    }

    if(m_iLast < 0)
    {
        GetSpinReal();
        GetProgressReal();
        GetTimerReal();

        fPrev   = 0.0f;
        m_iLast = iSteps;
        fCur    = (100.0f * m_iLast) / m_iMax;
        bChangeSpin = m_bSpin;
        tTimer.Start(kTRUE);
    }
    else
    {
        fPrev    = (100.0f * m_iLast) / m_iMax;
        m_iLast += iSteps;
        fCur     = (100.0f * m_iLast) / m_iMax;
    }

    if(m_bSpin)
    {
        iChangeSpin = (int)(fCur / m_fSpinReal);
        if((int)(fPrev / m_fSpinReal) != iChangeSpin)
            bChangeSpin = kTRUE;
    }

    if(m_bProgress)
    {
        iChangeProgress = (int)(fCur / m_fProgressReal)
                            - (int)(fPrev / m_fProgressReal);
    }

    if(m_bTimer)
    {
        if((int)(fPrev / m_fTimerReal) != (int)(fCur / m_fTimerReal))
            bChangeTimer = kTRUE;
    }

    if((m_bSpin) && ((iChangeProgress > 0) || (bChangeTimer)))
        bChangeSpin = kTRUE;

        // clear spin
    if((bChangeSpin != 0) && (fPrev != 0.0f))
        printf("\b");

        // write progress
    for(i = 0; i < iChangeProgress; i++)
        printf("%c", m_cProgressChar);

        // write the timer info
    if(bChangeTimer)
    {
        printf(" %3d %% done in %8.2f s; ~ %8.2f s left;  %8.2f evts/s\n",
                (int)fCur, tTimer.RealTime(),
                (100.0f - fCur) * tTimer.RealTime() / fCur,
                ((double)m_iLast / tTimer.RealTime()));

        tTimer.Continue();
    }

        // write the spin or the total time
    if(m_iLast < m_iMax)
    {
        if(bChangeSpin)
            printf("%c", m_psSpinChars[iChangeSpin % m_iSpinLen]);
    }
    else
    {
        if(m_bFullTimer)
            Final();
    }

    if((bChangeSpin) || (iChangeProgress > 0) || (bChangeTimer))
        fflush(stdout);
}

/* ******************************** */

void HSUProgress::Final(void)
{
    if(m_iLast <= m_iMax)
    {
        printf("\n%d events done in %8.2f s; %8.2f evts/s\n",
            m_iLast, tTimer.RealTime(), ((double)m_iLast / tTimer.RealTime()));
    }

    m_iLast = m_iMax + 1;
}
