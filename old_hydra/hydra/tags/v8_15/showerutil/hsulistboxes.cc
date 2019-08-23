#pragma implementation

// -----------------------------------------------------------------------------

#include "hsulistboxes.h"
#include "TGScrollBar.h"

// -----------------------------------------------------------------------------

ClassImp(HSUList);
ClassImp(HSUComboBox);
ClassImp(HSUListBox);

// -----------------------------------------------------------------------------
//
// HSUComboBox
//
// -----------------------------------------------------------------------------

HSUComboBox::HSUComboBox(Int_t *piVal, const TGWindow* pParent, Int_t iId)
                    : TGComboBox(pParent, iId)
{
    if(piVal == NULL)
        Error("HSUComboBox", "piVal == NULL");

    //SetHeight(2 * GetHeight());   // bug in Root ???
    SetHeight(fDDButton->GetDefaultHeight() + 2 * fBorderWidth);

    m_piVal = piVal;
}

// -----------------------------------------------------------------------------

void HSUComboBox::Reset(void)
{
    if(m_piVal == NULL)
        Error("Reset", "m_piVal == NULL");
    else
        Select(*m_piVal);
}

// -----------------------------------------------------------------------------

void HSUComboBox::SetData(void)
{
    if(m_piVal == NULL)
        Error("SetData", "m_piVal == NULL");
    else
        *m_piVal = GetSelected();
}

// -----------------------------------------------------------------------------

void HSUComboBox::Add(const Char_t *s, Int_t iId)
{
    AddEntry(s, iId);
}

// -----------------------------------------------------------------------------
//
// HSUListBox
//
// -----------------------------------------------------------------------------

HSUListBox::HSUListBox(Int_t *piVal, const TGWindow* pParent, Int_t iId)
                    : TGListBox(pParent, iId)
{
    if(piVal == NULL)
        Error("HSUListBox", "piVal == NULL");

    IntegralHeight(kTRUE);

    m_piVal = piVal;

    m_iElements = 0;
}

// -----------------------------------------------------------------------------

void HSUListBox::Reset(void)
{
    if(m_piVal == NULL)
        Error("Reset", "m_piVal == NULL");
    else
        Select(*m_piVal);
}

// -----------------------------------------------------------------------------

void HSUListBox::SetData(void)
{
    if(m_piVal == NULL)
        Error("SetData", "m_piVal == NULL");
    else
        *m_piVal = GetSelected();
}

// -----------------------------------------------------------------------------

void HSUListBox::Add(const Char_t *s, Int_t iId)
{
    AddEntry(s, iId);
    m_iElements++;

    if((m_iElements <= 4) && (fItemVsize > 0))
        SetHeight(m_iElements * fItemVsize + 2 * fBorderWidth);
}
