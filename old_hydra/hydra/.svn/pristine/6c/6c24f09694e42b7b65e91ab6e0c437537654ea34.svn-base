#ifndef HSUBUTTONS_H
#define HSUBUTTONS_H
#pragma interface

// -----------------------------------------------------------------------------

#include <TGFrame.h>
#include <TGButton.h>
#include <TGSplitter.h>
#include <TGTextBuffer.h>
#include <TGTextEntry.h>
#include <TGFileDialog.h>
#include <TGPicture.h>
#include <TG3DLine.h>

#include "hsuactivewidget.h"

// -----------------------------------------------------------------------------

class HSUCheckButton : public TGCheckButton, public HSUActiveWidget
{
public:
                HSUCheckButton(const Char_t *pName, Int_t *piVal, Int_t iMask,
                        const TGWindow *pParent, Int_t iId);

    void        Reset(void);
    void        SetData(void);

private:
    Int_t      *m_piVal;        // Where put the result
    Int_t       m_iMask;        // Mask of the result

    ClassDef(HSUCheckButton, 0) // Check button for ShowerUtil GUI
};

// -----------------------------------------------------------------------------

class HSUHorizontalLine : public TGHorizontal3DLine, public HSUActiveWidget
{
public:
                HSUHorizontalLine(const TGWindow *pParent)
                        : TGHorizontal3DLine(pParent)
                                {}

    void        Reset(void)     {}
    void        SetData(void)   {}

    ClassDef(HSUHorizontalLine, 0)  // Horizontal line for Shower Util GUI
};

// -----------------------------------------------------------------------------

class HSUFileOpen : public TGHorizontalFrame, public HSUActiveWidget
{
public:

    enum EHSUFiles {
       kFilesMacro = BIT(0),    // *.C    pattern
       kFilesHLD   = BIT(1),    // *.hld  pattern
       kFilesRoot  = BIT(2),    // *.root pattern
       kFilesPS    = BIT(3),    // *.ps   pattern
       kFilesEPS   = BIT(4),    // *.eps  pattern
       kFilesGif   = BIT(5),    // *.gif  pattern
       kFilesAll   = BIT(6)     // *      pattern
    };

    // -------------------------------------------------------------------------
                        HSUFileOpen(Char_t *pFileName, EFileDialogMode eMode,
                            const Char_t **pFileTypes, const TGWindow *pParent);
                        HSUFileOpen(TString *pFileName, EFileDialogMode eMode,
                            const Char_t **pFileTypes, const TGWindow *pParent);

                        HSUFileOpen(Char_t *pFileName, EFileDialogMode eMode,
                            UInt_t uiFileTypes, const TGWindow *pParent);
                        HSUFileOpen(TString *pFileName, EFileDialogMode eMode,
                            UInt_t uiFileTypes, const TGWindow *pParent);

                       ~HSUFileOpen(void);

    void                Reset(void);
    void                SetData(void);

    virtual TGDimension GetDefaultSize() const;
    virtual void        Resize(UInt_t w, UInt_t h);

    // -------------------------------------------------------------------------

private:
    TGTextEntry        *m_pTextField;   // text part
    TGPictureButton    *m_pButton;
    const TGPicture    *m_pPicture;

    EFileDialogMode     m_eMode;

    Char_t             *m_pFileName;
    TString            *m_pTStrFileName;
    const Char_t        **m_pFileTypes;
    Bool_t              m_bDeleteFileTypes;

    void                Init(EFileDialogMode eMode, const Char_t **pFileTypes);
    void                InitFilesType(UInt_t uiFileTypes);
    virtual Bool_t      ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
    void                OpenFileDialog(void);

    ClassDef(HSUFileOpen, 0)
};

// -----------------------------------------------------------------------------

#endif //!HSUBUTTONS_H
