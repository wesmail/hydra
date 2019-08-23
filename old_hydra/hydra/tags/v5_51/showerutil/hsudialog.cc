#include "hsudialog.h"

#include <TGLabel.h>
#include <TMath.h>
#include <TGButton.h>

#include "hsueditboxes.h"
#include "hsulistboxes.h"
#include "hsudialogframe.h"
#include "hsubuttons.h"

//______________________________________________________________________________

#define SAFE_DELETE(A)  { if(A != NULL) { delete A; A = NULL; } }

//______________________________________________________________________________

ClassImp(HSUDialog)

//______________________________________________________________________________

HSUDialog::HSUDialog(const char *psTitle) :
        TGTransientFrame(gClient->GetRoot(), gClient->GetRoot(), 10, 10)
{
UInt_t uiMaxButtonWidth = 0;

    m_pDialogLayoutHints  = NULL;

    m_pButtonsFrame       = NULL;
    m_pButtonsLayoutHints = NULL;
    m_pButtonOk           = NULL;
    m_pButtonCancel       = NULL;

    m_bResized            = kFALSE;

    m_pParams = new TObjArray();
    m_pParams->SetOwner(kTRUE);

    m_pLastList = NULL;

    m_pButtonsLayoutHints = new TGLayoutHints(kLHintsCenterY | kLHintsExpandX,
                                    3, 3, 3, 3);

        // create the buttons and their layout
    m_pButtonsFrame = new TGHorizontalFrame(this, 60, 20, kFixedWidth);

    m_pButtonOk = new TGTextButton(m_pButtonsFrame, new TGHotString("&Ok"), 1);
    m_pButtonOk->Associate(this);
    m_pButtonsFrame->AddFrame(m_pButtonOk, m_pButtonsLayoutHints);
    uiMaxButtonWidth = m_pButtonOk->GetDefaultWidth();

    m_pButtonCancel = new TGTextButton(m_pButtonsFrame,
                                    new TGHotString("&Cancel"), 0);
    m_pButtonCancel->Associate(this);
    m_pButtonsFrame->AddFrame(m_pButtonCancel, m_pButtonsLayoutHints);
    uiMaxButtonWidth = TMath::Max(uiMaxButtonWidth,
                                        m_pButtonCancel->GetDefaultWidth());

        // resize the button frame
    m_pButtonsFrame->Resize(2 * (uiMaxButtonWidth + 20), GetDefaultHeight());

    m_pDialogLayoutHints = new TGLayoutHints(kLHintsBottom | kLHintsCenterX,
                                        0, 0, 5, 5);
    AddFrame(m_pButtonsFrame, m_pDialogLayoutHints);

    m_pParamsLayoutHints = new TGLayoutHints(kLHintsTop | kLHintsCenterX,
                    4, 4, 4, 0);

        // set names
    SetWindowName(psTitle);
    SetIconName(psTitle);

    SetClassHints(psTitle, psTitle);
    SetMWMHints(kMWMDecorAll | kMWMDecorResizeH  | kMWMDecorMaximize |
                                kMWMDecorMinimize | kMWMDecorMenu,
                kMWMFuncAll  | kMWMFuncResize    | kMWMFuncMaximize |
                                kMWMFuncMinimize, kMWMInputModeless);
}

//______________________________________________________________________________

void HSUDialog::SetSize(void)
{
Int_t           iPosX, iPosY;
UInt_t          uiRootW, uiRootH;
Int_t           i, iMax;
UInt_t          uiH = 0, uiWL = 0, uiWW = 200, k;
HSUDialogFrame *pFrame;

    iMax = m_pParams->GetLast();
    for(i = 0; i <= iMax; i++)
    {
        pFrame = (HSUDialogFrame *)m_pParams->At(i);
        uiH += pFrame->GetWidgetHeight() + 4;

        if((k = pFrame->GetLabelWidth()) > uiWL)
            uiWL = k;

        if((k = pFrame->GetWidgetWidth()) > uiWW)
            uiWW = k;
    }

    for(i = 0; i <= iMax; i++)
    {
        pFrame = (HSUDialogFrame *)m_pParams->At(i);
        pFrame->SetSize(uiWL, uiWW);
    }

    Resize(uiWL + uiWW + 30, m_pButtonsFrame->GetDefaultHeight() + uiH + 10);

    MapSubwindows();

        // position in the screen center
    gVirtualX->GetWindowSize(fClient->GetRoot()->GetId(), iPosX, iPosY,
                                uiRootW, uiRootH);
    iPosX = (uiRootW - fWidth) >> 1;
    iPosY = (uiRootH - fHeight) >> 1;

    Move(iPosX, iPosY);
    SetWMPosition(iPosX, iPosY);

        // make the message box non-resizable
    SetWMSize(fWidth, fHeight);
    SetWMSizeHints(fWidth, fHeight, fWidth, fHeight, 0, 0);

    m_bResized = kTRUE;
}

//______________________________________________________________________________

HSUDialog::~HSUDialog()
{
    SAFE_DELETE(m_pButtonOk);
    SAFE_DELETE(m_pButtonCancel);
    SAFE_DELETE(m_pButtonsLayoutHints);
    SAFE_DELETE(m_pButtonsFrame);

    SAFE_DELETE(m_pDialogLayoutHints);

    SAFE_DELETE(m_pParams);

    SAFE_DELETE(m_pParamsLayoutHints);
}

//______________________________________________________________________________

void HSUDialog::CloseWindow()
{
    UnmapWindow();
}

//______________________________________________________________________________

Bool_t HSUDialog::Show(void)
{
HSUActiveWidget *pAW;

    if( ! m_bResized)
        SetSize();

    for(Int_t i = 0; i <= m_pParams->GetLast(); i++)
    {
        if((pAW = ((HSUDialogFrame *)m_pParams->At(i))->GetActiveWidget())
                    != NULL)
        {
            pAW->Reset();
        }
    }

    m_bReturn = kFALSE;

    MapWindow();

    fClient->WaitForUnmap(this);

    return m_bReturn;
}

//______________________________________________________________________________

Bool_t HSUDialog::ProcessMessage(Long_t lMsg, Long_t lParam1, Long_t)
{
    switch(GET_MSG(lMsg))
    {
        case kC_COMMAND:
            switch(GET_SUBMSG(lMsg))
            {
                case kCM_BUTTON:
                    if(lParam1 != 0)
                    {
                    HSUActiveWidget *pAW;

                        for(Int_t i = 0; i <= m_pParams->GetLast(); i++)
                        {
                            if((pAW = ((HSUDialogFrame *)m_pParams->At(i))
                                        ->GetActiveWidget()) != NULL)
                            {
                                pAW->SetData();
                            }
                        }

                        m_bReturn = kTRUE;
                    }

                    UnmapWindow();
                    break;

                default:
                    break;
            }

            break;

        default:
            break;
    }

    return kTRUE;
}

//______________________________________________________________________________

void HSUDialog::AddChildFrame(HSUDialogFrame *pFrame)
{
    m_pParams->Add(pFrame);
    AddFrame(pFrame, m_pParamsLayoutHints);
    m_bResized = kFALSE;
}

//______________________________________________________________________________

void HSUDialog::AddEditString(const Char_t *pName, Char_t *pStr, Int_t iSize)
{
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUEditString  *pEdit;

    pEdit = new HSUEditString(pStr, iSize, pFrame, m_pParams->GetLast() + 1);
    pFrame->Init(pName, pEdit, pEdit);
    pEdit->Associate(this);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddEditInt(const Char_t *pName, Int_t *piVal)
{
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUEditInt     *pEdit;

    pEdit = new HSUEditInt(piVal, pFrame, m_pParams->GetLast() + 1);
    pFrame->Init(pName, pEdit, pEdit);
    pEdit->Associate(this);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddEditFloat(const Char_t *pName, Float_t *pfVal)
{
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUEditFloat   *pEdit;

    pEdit = new HSUEditFloat(pfVal, pFrame, m_pParams->GetLast() + 1);
    pFrame->Init(pName, pEdit, pEdit);
    pEdit->Associate(this);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddComboBox(const Char_t *pName, Int_t *piVal)
{
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUComboBox    *pBox;

    pBox = new HSUComboBox(piVal, pFrame, m_pParams->GetLast() + 1);
    pFrame->Init(pName, pBox, pBox);
    pBox->Associate(this);
    AddChildFrame(pFrame);

    m_pLastList = pBox;
}

//______________________________________________________________________________

void HSUDialog::AddListBox(const Char_t *pName, Int_t *piVal)
{
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUListBox     *pBox;

    pBox = new HSUListBox(piVal, pFrame, m_pParams->GetLast() + 1);
    pFrame->Init(pName, pBox, pBox);
    pBox->Associate(this);
    AddChildFrame(pFrame);

    m_pLastList = pBox;
}

//______________________________________________________________________________

void HSUDialog::AddCheckButton(const Char_t *pName, Int_t *piVal, Int_t iMask)
{
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUCheckButton *pButton;

    pButton = new HSUCheckButton(pName, piVal, iMask, pFrame,
                            m_pParams->GetLast() + 1);
    pFrame->Init(NULL, pButton, pButton);
    pButton->Associate(this);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddLine(void)
{
HSUDialogFrame    *pFrame = new HSUDialogFrame(this);
HSUHorizontalLine *pLine;

    pLine = new HSUHorizontalLine(pFrame);
    pFrame->Init(NULL, pLine, pLine);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddLabel(const Char_t *pName)
{
HSUDialogFrame    *pFrame = new HSUDialogFrame(this);

    pFrame->Init(pName, NULL, NULL);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddOpenFileName(const Char_t *pName, Char_t *pStr,
                                const Char_t **paFileTypes)
{
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUFileOpen    *pFileBox;

    pFileBox = new HSUFileOpen(pStr, kFDOpen, paFileTypes, pFrame);
    pFrame->Init(pName, pFileBox, pFileBox);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddOpenFileName(const Char_t *pName, Char_t *pStr,
                                EHSUFiles eFileType)
{
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUFileOpen    *pFileBox;

    pFileBox = new HSUFileOpen(pStr, kFDOpen, eFileType, pFrame);
    pFrame->Init(pName, pFileBox, pFileBox);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddSaveFileName(const Char_t *pName, Char_t *pStr,
                                const Char_t **paFileTypes)
{
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUFileOpen    *pFileBox;

    pFileBox = new HSUFileOpen(pStr, kFDSave, paFileTypes, pFrame);
    pFrame->Init(pName, pFileBox, pFileBox);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddSaveFileName(const Char_t *pName, Char_t *pStr,
                                EHSUFiles eFileType)
{
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUFileOpen    *pFileBox;

    pFileBox = new HSUFileOpen(pStr, kFDSave, eFileType, pFrame);
    pFrame->Init(pName, pFileBox, pFileBox);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddEntry(const Char_t *s, Int_t iId)
{
    if(m_pLastList == NULL)
    {
        Error("AddEntry", "No ListBox or ComboBox in the dialog");
        return;
    }

    m_pLastList->Add(s, iId);
}
