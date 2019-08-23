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
#include "hsuactivewidget.h"

// -----------------------------------------------------------------------------

enum EHSUFiles {
   kFilesMacro = BIT(0),
   kFilesHLD   = BIT(1),
   kFilesRoot  = BIT(2),
   kFilesPS    = BIT(3),
   kFilesEPS   = BIT(4),
   kFilesGif   = BIT(5),
   kFilesAll   = BIT(6)
};

// -----------------------------------------------------------------------------

class HSUCheckButton : public TGCheckButton, public HSUActiveWidget
{
public:
                HSUCheckButton(const char *pName, Int_t *piVal, Int_t iMask,
                        const TGWindow *pParent, Int_t iId);

    void        Reset(void);
    void        SetData(void);

private:
    Int_t      *m_piVal;
    Int_t       m_iMask;

    ClassDef(HSUCheckButton, 0)
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

    ClassDef(HSUHorizontalLine, 0)
};

// -----------------------------------------------------------------------------

class HSUFileOpen : public TGHorizontalFrame, public HSUActiveWidget
{
public:
                        HSUFileOpen(char *pFileName, EFileDialogMode eMode,
                            const char **pFileTypes, const TGWindow *pParent);

                        HSUFileOpen(char *pFileName, EFileDialogMode eMode,
                            UInt_t uiFileTypes, const TGWindow *pParent);

                       ~HSUFileOpen(void);

    void                Reset(void);
    void                SetData(void);

    virtual TGDimension GetDefaultSize() const;
    virtual void        Resize(UInt_t w, UInt_t h);

    // -------------------------------------------------------------------------

private:
    TGTextEntry        *m_pTextField;
    TGPictureButton    *m_pButton;
    const TGPicture    *m_pPicture;

    EFileDialogMode     m_eMode;

    Char_t             *m_pFileName;
    const char        **m_pFileTypes;
    Bool_t              m_bDeleteFileTypes;

    void                Init(char *pFileName, EFileDialogMode eMode,
                                    const char **pFileTypes);
    virtual Bool_t      ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2);
    void                OpenFileDialog(void);

    ClassDef(HSUFileOpen, 0)
};

// -----------------------------------------------------------------------------

#endif //!HSUBUTTONS_H
