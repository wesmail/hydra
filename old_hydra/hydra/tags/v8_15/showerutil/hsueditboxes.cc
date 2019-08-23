#pragma implementation

// -----------------------------------------------------------------------------

#include "hsueditboxes.h"

// -----------------------------------------------------------------------------

ClassImp(HSUEditString);
ClassImp(HSUEditInt);
ClassImp(HSUEditFloat);

// -----------------------------------------------------------------------------
//
// HSUEditString
//
// -----------------------------------------------------------------------------

HSUEditString::HSUEditString(Char_t *pBuffer, Int_t iBufferSize,
                    const TGWindow* pParent, Int_t iId,
                    ETextJustification  eJustification)
                    : TGTextEntry(pParent, pBuffer, iId)
{
    if(pBuffer == NULL)
        Error("HSUEditString", "pBuffer == NULL");

    if(iBufferSize <= 0)
        Error("HSUEditString", "iBufferSize <= 0");

    m_pBuffer     = pBuffer;
    m_iBufferSize = (iBufferSize > 0) ? iBufferSize : 0;
    m_pTString    = NULL;

    SetAlignment(eJustification);
    SetMaxLength(m_iBufferSize);
}

// -----------------------------------------------------------------------------

HSUEditString::HSUEditString(TString *pStr,
                    const TGWindow* pParent, Int_t iId,
                    ETextJustification  eJustification)
                    : TGTextEntry(pParent, pStr->Data(), iId)
{
    m_pTString    = pStr;
    m_iBufferSize = -1;
    m_pBuffer     = NULL;

    SetAlignment(eJustification);
}

// -----------------------------------------------------------------------------

void HSUEditString::Reset(void)
{
    if(m_pBuffer != NULL)
        SetText((m_iBufferSize >= 0) ? m_pBuffer : m_pTString->Data());
}

// -----------------------------------------------------------------------------

void HSUEditString::SetData(void)
{
int         iSize;
const char *pText;

    if(m_iBufferSize < 0)
    {
        if(m_pTString == NULL)
        {
            Error("SetData", "m_pTString == NULL");
            return;
        }

        *m_pTString = GetText();

        return;
    }

    if(m_pBuffer == NULL)
    {
        Error("SetData", "m_pBuffer == NULL");
        return;
    }

    pText = GetText();
    if((iSize = strlen(pText)) < m_iBufferSize)
    {
        strcpy(m_pBuffer, pText);
        return;
    }

    strncpy(m_pBuffer, pText, m_iBufferSize - 1);
    m_pBuffer[m_iBufferSize - 1] = '\0';
}

// -----------------------------------------------------------------------------
//
// HSUEditInt
//
// -----------------------------------------------------------------------------

HSUEditInt::HSUEditInt(Int_t *piVal,
                    const TGWindow* pParent, Int_t iId,
                    ETextJustification  eJustification)
                    : TGTextEntry(pParent, "", iId)
{
    if(piVal == NULL)
        Error("HSUEditInt", "piVal == NULL");

    m_piVal = piVal;
    SetAlignment(eJustification);
}

// -----------------------------------------------------------------------------

void HSUEditInt::Reset(void)
{
char s[100];

    sprintf(s, "%d", *m_piVal);
    SetText(s);
}

// -----------------------------------------------------------------------------

void HSUEditInt::SetData(void)
{
Int_t i;

    if(sscanf(GetText(), "%d", &i) != 1)
    {
        Warning("SetData", "Wrong numerical value: not changed !");
        return;
    }

    if(m_piVal != NULL)
        *m_piVal = i;
    else
        Error("HSUEditInt", "piVal == NULL");
}

// -----------------------------------------------------------------------------
//
// HSUEditFloat
//
// -----------------------------------------------------------------------------

HSUEditFloat::HSUEditFloat(Float_t *pfVal,
                    const TGWindow* pParent, Int_t iId,
                    ETextJustification  eJustification)
                    : TGTextEntry(pParent, "", iId)
{
    if(pfVal == NULL)
        Error("HSUEditFloat", "pfVal == NULL");

    m_pfVal = pfVal;
    SetAlignment(eJustification);
}

// -----------------------------------------------------------------------------

void HSUEditFloat::Reset(void)
{
char s[100];

    sprintf(s, "%g", *m_pfVal);
    SetText(s);
}

// -----------------------------------------------------------------------------

void HSUEditFloat::SetData(void)
{
Float_t f;

    if(sscanf(GetText(), "%g", &f) != 1)
    {
        Warning("SetData", "Wrong numerical value: not changed !");
        return;
    }

    if(m_pfVal != NULL)
        *m_pfVal = f;
    else
        Error("HSUEditFloat", "pfVal == NULL");
}

// -----------------------------------------------------------------------------
