#ifndef HSUDIALOG_H
#define HSUDIALOG_H

#include <TGFrame.h>
#include <TGString.h>
#include <TObjArray.h>

#include "hsubuttons.h"

// -----------------------------------------------------------------------------

class TGButton;
class HSUList;
class HSUDialogFrame;

// -----------------------------------------------------------------------------

class HSUDialog : public TGTransientFrame
{
protected:
    TGLayoutHints      *m_pDialogLayoutHints;
    TGLayoutHints      *m_pParamsLayoutHints;

    TGHorizontalFrame  *m_pButtonsFrame;
    TGLayoutHints      *m_pButtonsLayoutHints;
    TGButton           *m_pButtonOk;
    TGButton           *m_pButtonCancel;

    Bool_t              m_bResized;
    TObjArray          *m_pParams;
    Bool_t              m_bReturn;
    HSUList            *m_pLastList;

public:
                        HSUDialog(const char *psTitle);
    virtual            ~HSUDialog();

    Bool_t              Show(void);

    void                AddEditString(const Char_t *pName,
                                        Char_t *pStr, Int_t iSize);
    void                AddEditInt(const Char_t *pName, Int_t *piVal);
    void                AddEditFloat(const Char_t *pName, Float_t *pfVal);

    void                AddComboBox(const Char_t *pName, Int_t *piVal);
    void                AddListBox(const Char_t *pName, Int_t *piVal);

    void                AddCheckButton(const Char_t *pName, Int_t *piVal,
                                        Int_t iMask = 1);

    void                AddLine(void);
    void                AddLabel(const Char_t *pName);

    void                AddOpenFileName(const Char_t *pName, Char_t *pStr,
                                    const Char_t **paFileTypes = NULL);
    void                AddOpenFileName(const Char_t *pName, Char_t *pStr,
                                    EHSUFiles eFileType);

    void                AddSaveFileName(const Char_t *pName, Char_t *pStr,
                                    const Char_t **paFileTypes = NULL);
    void                AddSaveFileName(const Char_t *pName, Char_t *pStr,
                                    EHSUFiles eFileType);

    void                AddEntry(const Char_t *s, Int_t iId);

private:
    void                AddChildFrame(HSUDialogFrame *pFrame);
    void                SetSize(void);

    virtual void        CloseWindow();
    virtual Bool_t      ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);

    ClassDef(HSUDialog,0)  // Parameters dialog box
};

#endif //!HSUDIALOG_H
