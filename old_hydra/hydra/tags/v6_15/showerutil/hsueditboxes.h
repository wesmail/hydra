#ifndef HSUEDITBOXES_H
#define HSUEDITBOXES_H
#pragma interface

// -----------------------------------------------------------------------------

#include <TGTextEntry.h>
#include "hsuactivewidget.h"

// -----------------------------------------------------------------------------

class HSUEditString : public TGTextEntry, public HSUActiveWidget
{
public:
                HSUEditString(Char_t *pBuffer, Int_t iBufferSize,
                            const TGWindow* pParent, Int_t iId,
                            ETextJustification  eJustification = kTextLeft);

                HSUEditString(TString *pStr,
                            const TGWindow* pParent, Int_t iId,
                            ETextJustification  eJustification = kTextLeft);

    void        Reset(void);
    void        SetData(void);

    // -------------------------------------------------------------------------

private:

    Char_t     *m_pBuffer;
    TString    *m_pTString;
    Int_t       m_iBufferSize;

    ClassDef(HSUEditString, 0)
};

// -----------------------------------------------------------------------------

class HSUEditInt : public TGTextEntry, public HSUActiveWidget
{
public:
                HSUEditInt(Int_t *piVal,
                            const TGWindow* pParent, Int_t iId,
                            ETextJustification  eJustification = kTextLeft);

    void        Reset(void);
    void        SetData(void);

    // -------------------------------------------------------------------------

private:

    Int_t      *m_piVal;

    ClassDef(HSUEditInt, 0)
};

// -----------------------------------------------------------------------------

class HSUEditFloat : public TGTextEntry, public HSUActiveWidget
{
public:
                HSUEditFloat(Float_t *pfVal,
                            const TGWindow* pParent, Int_t iId,
                            ETextJustification  eJustification = kTextLeft);

    void        Reset(void);
    void        SetData(void);

    // -------------------------------------------------------------------------

private:

    Float_t    *m_pfVal;

    ClassDef(HSUEditFloat, 0)
};

#endif //!HSUEDITBOXES_H
