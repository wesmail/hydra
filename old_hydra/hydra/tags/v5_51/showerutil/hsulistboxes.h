#ifndef HSULISTBOXES
#define HSULISTBOXES
#pragma interface

// -----------------------------------------------------------------------------

#include <TGComboBox.h>
#include <TGListBox.h>
#include "hsuactivewidget.h"

// -----------------------------------------------------------------------------

class HSUList : public HSUActiveWidget
{
public:
    virtual void Add(const Char_t *s, Int_t iId) = 0;

    ClassDef(HSUList, 0)
};

// -----------------------------------------------------------------------------

class HSUComboBox : public TGComboBox, public HSUList
{
public:
                HSUComboBox(Int_t *piVal, const TGWindow* pParent, Int_t iId);

    void        Reset(void);
    void        SetData(void);
    void        Add(const Char_t *s, Int_t iId);

    // -------------------------------------------------------------------------

private:

    Int_t      *m_piVal;

    ClassDef(HSUComboBox, 0)
};

// -----------------------------------------------------------------------------

class HSUListBox : public TGListBox, public HSUList
{
public:
                HSUListBox(Int_t *piVal, const TGWindow* pParent, Int_t iId);

    void        Reset(void);
    void        SetData(void);
    void        Add(const Char_t *s, Int_t iId);

    // -------------------------------------------------------------------------

private:
    Int_t      *m_piVal;

    Int_t       m_iElements;

    ClassDef(HSUListBox, 0)
};

#endif //!HSULISTBOXES
