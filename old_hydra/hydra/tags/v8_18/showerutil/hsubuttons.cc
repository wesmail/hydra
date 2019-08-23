#pragma implementation

// -----------------------------------------------------------------------------

#include "hsubuttons.h"
#include <TSystem.h>

// -----------------------------------------------------------------------------
// *-- Author : Marcin Jaskula
// -----------------------------------------------------------------------------

ClassImp(HSUCheckButton);
ClassImp(HSUHorizontalLine);
ClassImp(HSUFileOpen);

// -----------------------------------------------------------------------------

TGLayoutHints g_FileOpenEditHints(kLHintsLeft    | kLHintsCenterY);
TGLayoutHints g_FileOpenButtonHints(kLHintsRight | kLHintsCenterY);

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A)  { if(A != NULL) { delete A; A = NULL; }}
#define OPEN_FILE_SPACE 5

// -----------------------------------------------------------------------------

static const Char_t *gOpenAsTypes[] =
{
    "Macro files", "*.C",
    "HLD files",   "*.hld",
    "ROOT files",  "*.root",
    "PostScript",  "*.ps",
    "Encapsulated PostScript", "*.eps",
    "Gif files",   "*.gif",
    "All files",   "*",
     0, 0
};

// -----------------------------------------------------------------------------
//
// HSUCheckButton
//
// -----------------------------------------------------------------------------

HSUCheckButton::HSUCheckButton(const Char_t *pName, Int_t *piVal, Int_t iMask,
                            const TGWindow* pParent, Int_t iId)
                            : TGCheckButton(pParent, pName, iId)
{
    if(piVal == NULL)
        Error("HSUCheckButton", "piVal == NULL");

    m_piVal = piVal;
    m_iMask = iMask;
}

// -----------------------------------------------------------------------------

void HSUCheckButton::Reset(void)
{
    if(m_piVal == NULL)
        Error("Reset", "m_piVal == NULL");
    else
    {
        if((*m_piVal & m_iMask) != 0)
        {
            //PSetState(kButtonDown);
            PSetState(kButtonDown,kFALSE);
            fPrevState = fState;        // error in ROOT 225-03
        }
    }
}

// -----------------------------------------------------------------------------

void HSUCheckButton::SetData(void)
{
    if(m_piVal == NULL)
        Error("SetData", "m_piVal == NULL");
    else
    {
        if(fState == kButtonDown)
            *m_piVal |= m_iMask;
        else
            *m_piVal &= ~m_iMask;
    }
}

// -----------------------------------------------------------------------------
//
// HSUFileOpen
//
// -----------------------------------------------------------------------------

HSUFileOpen::HSUFileOpen(Char_t *pFileName, EFileDialogMode eMode,
                        const Char_t **pFileTypes, const TGWindow *pParent)
                        : TGHorizontalFrame(pParent, 10, 10)
{
    m_pTStrFileName = NULL;
    if((m_pFileName = pFileName) == NULL)
        Error("HSUFileOpen", "pFileName == NULL");

    m_bDeleteFileTypes = kFALSE;
    Init(eMode, pFileTypes);
}

// -----------------------------------------------------------------------------

HSUFileOpen::HSUFileOpen(TString *pFileName, EFileDialogMode eMode,
                        const Char_t **pFileTypes, const TGWindow *pParent)
                        : TGHorizontalFrame(pParent, 10, 10)
{
    m_pFileName = NULL;

    if((m_pTStrFileName = pFileName) == NULL)
        Error("HSUFileOpen", "pFileName == NULL");

    m_bDeleteFileTypes = kFALSE;
    Init(eMode, pFileTypes);
}

// -----------------------------------------------------------------------------

HSUFileOpen::HSUFileOpen(Char_t *pFileName, EFileDialogMode eMode,
                        UInt_t uiFileTypes, const TGWindow *pParent)
                        : TGHorizontalFrame(pParent, 10, 10)
{
    m_pTStrFileName = NULL;
    if((m_pFileName = pFileName) == NULL)
        Error("HSUFileOpen", "pFileName == NULL");

    InitFilesType(uiFileTypes);
    Init(eMode, m_pFileTypes);
}

// -----------------------------------------------------------------------------

HSUFileOpen::HSUFileOpen(TString *pFileName, EFileDialogMode eMode,
                        UInt_t uiFileTypes, const TGWindow *pParent)
                        : TGHorizontalFrame(pParent, 10, 10)
{
    m_pFileName = NULL;
    if((m_pTStrFileName = pFileName) == NULL)
        Error("HSUFileOpen", "pFileName == NULL");

    InitFilesType(uiFileTypes);
    Init(eMode, m_pFileTypes);
}

// -----------------------------------------------------------------------------

void HSUFileOpen::InitFilesType(UInt_t uiFileTypes)
{
Int_t i, iMax, k;

    iMax = sizeof(gOpenAsTypes) / (2 * sizeof(const Char_t *));
    m_pFileTypes = new const Char_t* [2 * iMax];
    for(i = 0; i < 2 * iMax; i++)
        m_pFileTypes[i] = NULL;

    for(k = 0, i = 0; i < iMax - 1; i++)
    {
        if((uiFileTypes & (1 << i)) == 0)
            continue;

        m_pFileTypes[k++] = gOpenAsTypes[2 * i];
        m_pFileTypes[k++] = gOpenAsTypes[2 * i + 1];
    }

    if(k == 0)
    {
        m_pFileTypes[k++] = gOpenAsTypes[2 * iMax - 4];
        m_pFileTypes[k++] = gOpenAsTypes[2 * iMax - 3];
    }

    m_pFileTypes[k++] = gOpenAsTypes[2 * iMax - 2];
    m_pFileTypes[k++] = gOpenAsTypes[2 * iMax - 1];

    m_bDeleteFileTypes = kTRUE;
}

// -----------------------------------------------------------------------------

void HSUFileOpen::Init(EFileDialogMode eMode, const Char_t **pFileTypes)
{
static const Char_t *pIconName = "folder_t.xpm";

    m_eMode      = eMode;
    m_pFileTypes = pFileTypes;

    m_pTextField = new TGTextEntry(this, (m_pTStrFileName != NULL) ?
                m_pTStrFileName->Data() : m_pFileName);
    AddFrame(m_pTextField, &g_FileOpenEditHints);

    m_pTextField->SetWidth(10 * m_pTextField->GetDefaultHeight());

    if((m_pPicture = fClient->GetPicture(pIconName)) == NULL)
        Error("HSUFileOpen", "%s not found !\n", pIconName);

    m_pButton = new TGPictureButton(this, m_pPicture, 1);
    AddFrame(m_pButton, &g_FileOpenButtonHints);
}

// -----------------------------------------------------------------------------

HSUFileOpen::~HSUFileOpen(void)
{
    SAFE_DELETE(m_pTextField);
    SAFE_DELETE(m_pButton);

    if(m_pPicture != NULL)
    {
        fClient->FreePicture(m_pPicture);
        m_pPicture = NULL;
    }

    if((m_bDeleteFileTypes) && (m_pFileTypes != NULL))
    {
        delete [] m_pFileTypes;
        m_pFileTypes = NULL;
    }
}

// -----------------------------------------------------------------------------

void HSUFileOpen::Reset(void)
{
    if(m_pTStrFileName != NULL)
        m_pTextField->SetText(m_pTStrFileName->Data());
    else
    {
        if(m_pFileName != NULL)
            m_pTextField->SetText(m_pFileName);
    }
}

// -----------------------------------------------------------------------------

void HSUFileOpen::SetData(void)
{
    if(m_pTStrFileName != NULL)
    {
        *m_pTStrFileName = m_pTextField->GetText();
        return;
    }

    if(m_pFileName == NULL)
    {
        Error("SetData", "m_pFileName == NULL");
        return;
    }

    strcpy(m_pFileName, m_pTextField->GetText());
}

// -----------------------------------------------------------------------------

TGDimension HSUFileOpen::GetDefaultSize() const
{
Int_t iWidth  = m_pTextField->GetDefaultWidth();
Int_t iHeight = m_pTextField->GetDefaultHeight();
Int_t i;

    if((i = m_pButton->GetDefaultHeight()) > iHeight)
        iHeight = i;

    iWidth += m_pButton->GetDefaultWidth() + OPEN_FILE_SPACE;

    return TGDimension(iWidth, iHeight);
}

// -----------------------------------------------------------------------------

void HSUFileOpen::Resize(UInt_t w, UInt_t h)
{
    SetWidth(w);
    SetHeight(h);
    m_pButton->Resize(m_pButton->GetDefaultSize());
    m_pTextField->Resize(w - OPEN_FILE_SPACE - m_pButton->GetDefaultWidth(), h);
}

// -----------------------------------------------------------------------------

Bool_t HSUFileOpen::ProcessMessage(Long_t lMsg, Long_t lParam1, Long_t)
{
    switch(GET_MSG(lMsg))
    {
        case kC_COMMAND:
            switch(GET_SUBMSG(lMsg))
            {
                case kCM_BUTTON:
                    if(lParam1 != 0)
                        OpenFileDialog();

                    break;

                default:
                    break;
            }

        default:
            break;
    }

    return kTRUE;
}

// -----------------------------------------------------------------------------

void HSUFileOpen::OpenFileDialog(void)
{
TGFileInfo    fInfo;
Char_t          sFile[256];
const Char_t   *pDirName;
void         *pDir;

    strcpy(sFile, m_pTextField->GetText());

    fInfo.fFilename  = sFile;
    pDirName = gSystem->DirName(sFile);

    if((pDir = gSystem->OpenDirectory(pDirName)) == NULL)
    {
        Warning("OpenFileDialog", "Directory: \"%s\" doesn't exisit", pDirName);
        fInfo.fIniDir = StrDup(".");
    }
    else
    {
        fInfo.fIniDir = StrDup(pDirName);
        gSystem->FreeDirectory(pDir);
    }

    fInfo.fFileTypes = ((m_pFileTypes == NULL)
                        ? gOpenAsTypes : m_pFileTypes);

    new TGFileDialog(fClient->GetRoot(), this, m_eMode, &fInfo);

    if(fInfo.fFilename != NULL)
        m_pTextField->SetText(fInfo.fFilename);
}
