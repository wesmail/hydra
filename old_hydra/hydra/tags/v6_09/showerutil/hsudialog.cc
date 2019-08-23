#include "hsudialog.h"

#include <TGLabel.h>
#include <TMath.h>
#include <TGButton.h>

#include "hsueditboxes.h"
#include "hsulistboxes.h"
#include "hsudialogframe.h"
#include "hsubuttons.h"

// -----------------------------------------------------------------------------

#ifndef SAFE_DELETE
#define SAFE_DELETE(A)  { if(A != NULL) { delete A; A = NULL; } }
#endif

// -----------------------------------------------------------------------------
// *-- Author : Marcin Jaskula
// -----------------------------------------------------------------------------

ClassImp(HSUDialog)

//______________________________________________________________________________
// Main dialog for all widgets in ShowerUtil library.
// It gives an easy interface for making a simple dialog which lets
// modify some numerical and text variables inside a macro. 
// It also supports disk search for files.
//
// A dialog:
// Begin_Html<img src="../gif/dialog.gif">End_Html
// is produce by a simple code:
//
// void dialogExample()
// {
// HSUDialog *pdFile = new HSUDialog("A name of dialog");
// const int  ciStringLength = 1000;
// Char_t     fileName[ciStringLength];
// Int_t      iInt =17;
// Float_t    fFloat;
// Char_t     string[1000];
// Int_t      bCheckButton = 1;
// Int_t      bComboBox = 1;
// TString    sStr = "A string in TString";
// TString    sFileStr = "test.root";
//
//     strcpy(string, "Some start value");
//
//     pdFile->AddOpenFileName("Input file", fileName);
//     pdFile->AddOpenFileName("Output file",&sFileStr,HSUFileOpen::kFilesRoot);
//     pdFile->AddLine();
//
//     pdFile->AddLabel("Insert some parameters");
//     pdFile->AddEditInt("Some integer value", &iInt);
//     pdFile->AddEditFloat("and some float", &fFloat);
//     pdFile->AddEditString("and string as char*", string, ciStringLength);
//     pdFile->AddEditString("or &TString", &sStr);
//     pdFile->AddLine();
//
//     pdFile->AddLabel("Select something");
//     pdFile->AddCheckButton("Do something ?", &bCheckButton);
//     pdFile->AddComboBox("What to do ?", &bComboBox);
//     pdFile->AddEntry("Option 1", 1);
//     pdFile->AddEntry("Second option", 2);
//     pdFile->AddEntry("10th", 10);
//
//     pdFile->AddLine();
//
//     if( ! pdFile->Show())
//     {
//         printf("\nCanceled !!!\n\n");
//         return;
//     }
// }
//
// The widgets are organized in one column in the order of adding them
// to the dialg box.
// All default values are taken from the corresponding variables.
// When "OK" button is clicked all variables get new parameters defined
// by the values in widgets parameters. Otherwise the values in
// the variables are not changed.
//
// There are 5 types of widget supported by the class:
// 1. Edit widget - where the user has opportunity to write the value
// 2. Disk search - as 1. + disk browser
// 3. Check box - allows to modify bits in an integer
// 4. Combo box - allows to select one value from many
// 5. Separator - a horizontal line

HSUDialog::HSUDialog(const char *psTitle) :
        TGTransientFrame(gClient->GetRoot(), gClient->GetRoot(), 10, 10)
{
// Constructor. The parameter is the window name (on the bar)
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

HSUDialog::~HSUDialog()
{
// Destroy the dialog and all widgets
    SAFE_DELETE(m_pButtonOk);
    SAFE_DELETE(m_pButtonCancel);
    SAFE_DELETE(m_pButtonsLayoutHints);
    SAFE_DELETE(m_pButtonsFrame);

    SAFE_DELETE(m_pDialogLayoutHints);

    SAFE_DELETE(m_pParams);

    SAFE_DELETE(m_pParamsLayoutHints);
}

//______________________________________________________________________________

void HSUDialog::SetSize(void)
{
// Recalculate size of all widgets and the dialog
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

void HSUDialog::CloseWindow()
{
// Close window. Show may be called after this to make the window appear
// on the screen
    UnmapWindow();
}

//______________________________________________________________________________

Bool_t HSUDialog::Show(void)
{
// Show the window on the screen.
// The function returns if one of the buttons: "OK", "Cancel" is clicked.
// When the "OK" is clicked then all values from widgets are written
// to the corresponding variables and kTRUE is returned.
// Otherwise the values in the variables are not changed and kFALSE is returned.
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
// Private function to interact with the user's activity
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
// Add a new widget to the dialog widgets list
    m_pParams->Add(pFrame);
    AddFrame(pFrame, m_pParamsLayoutHints);
    m_bResized = kFALSE;
}

//______________________________________________________________________________

void HSUDialog::AddEditString(const Char_t *pName, Char_t *pStr, Int_t iSize)
{
// Add an edit box for a string. The parameters are:
// pName - the name of the edit box in the dialog
// pStr  - pointer to variable from which the start value is taken
//         and to which the new value will be copy
// iSize - maximum length of the string to copy
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUEditString  *pEdit;

    pEdit = new HSUEditString(pStr, iSize, pFrame, m_pParams->GetLast() + 1);
    pFrame->Init(pName, pEdit, pEdit);
    pEdit->Associate(this);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddEditString(const Char_t *pName, TString *pStr)
{
// As a function above but the second parameter is a TString
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUEditString  *pEdit;

    pEdit = new HSUEditString(pStr, pFrame, m_pParams->GetLast() + 1);
    pFrame->Init(pName, pEdit, pEdit);
    pEdit->Associate(this);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddEditInt(const Char_t *pName, Int_t *piVal)
{
// Add an edit box for an integer variable
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
// Add an edit box for a float variable
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
// Add a combo box. The second parameter define a variable for collecting
// the value corresponding to the option selected by the user.
// To add an option to the combo box use HSUDialog::AddEntry method.
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
// As a function above, but list box is created (few option visible at once).
// It doesn't look fine for less then 5 parameters (investigation needed !!!)
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
// Add a check button. The last parameter define the mask of bits which
// will be set if the check box is checked. It lets to modify different
// bits in one integer by few check boxes.
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
// Add a separator - horizontal line
HSUDialogFrame    *pFrame = new HSUDialogFrame(this);
HSUHorizontalLine *pLine;

    pLine = new HSUHorizontalLine(pFrame);
    pFrame->Init(NULL, pLine, pLine);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddLabel(const Char_t *pName)
{
// Add an label
HSUDialogFrame    *pFrame = new HSUDialogFrame(this);

    pFrame->Init(pName, NULL, NULL);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddOpenFileName(const Char_t *pName, Char_t *pStr,
                                const Char_t **paFileTypes)
{
// Add a widget for a name of a file. It allows to write the name down
// or select it by the root browser (run by a small button on the right).
// For the root browser the last parameter is used to define
// the set of file types which should be searched by the browser.
//
// It must be an array of char* like:
//   {
//     user_friendly_name1, pattern1,
//     user_friendly_name2, pattern2,
//     ....
//     0, 0
//   }
//
// e.g:
//   {
//     "Macro files", "*.C",
//     "HLD files",   "*.hld",
//     0, 0
//   }
//
// Two last elements in the array MUST be NULL.
//
// pStr must be a pointer to a string at least 255 B long !
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUFileOpen    *pFileBox;

    pFileBox = new HSUFileOpen(pStr, kFDOpen, paFileTypes, pFrame);
    pFrame->Init(pName, pFileBox, pFileBox);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddOpenFileName(const Char_t *pName, TString *pStr,
                                const Char_t **paFileTypes)
{
// Like the previous function but work with TString instead of char*
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUFileOpen    *pFileBox;

    pFileBox = new HSUFileOpen(pStr, kFDOpen, paFileTypes, pFrame);
    pFrame->Init(pName, pFileBox, pFileBox);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddOpenFileName(const Char_t *pName, Char_t *pStr,
                                HSUFileOpen::EHSUFiles eFileType)
{
// Like the previous function by the type of interesting file may be
// chosen by some defines (use bit operator | to select few)
// 0x01 - "Macro files", "*.C"
// 0x02 - "HLD files",   "*.hld"
// 0x04 - "ROOT files",  "*.root"
// 0x08 - "PostScript",  "*.ps"
// 0x10 - "Encapsulated PostScript", "*.eps"
// 0x20 - "Gif files",   "*.gif"
// 0x40 - "All files",   "*"
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUFileOpen    *pFileBox;

    pFileBox = new HSUFileOpen(pStr, kFDOpen, eFileType, pFrame);
    pFrame->Init(pName, pFileBox, pFileBox);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddOpenFileName(const Char_t *pName, TString *pStr,
                                HSUFileOpen::EHSUFiles eFileType)
{
// Like a previous function but TString* is the second parameter
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
// Like HSUDialog::AddOpenFileName but a different type
// of the root browser is called
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUFileOpen    *pFileBox;

    pFileBox = new HSUFileOpen(pStr, kFDSave, paFileTypes, pFrame);
    pFrame->Init(pName, pFileBox, pFileBox);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddSaveFileName(const Char_t *pName, TString *pStr,
                                const Char_t **paFileTypes)
{
// See HSUDialog::AddOpenFileName and HSUDialog::AddSaveFileName
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUFileOpen    *pFileBox;

    pFileBox = new HSUFileOpen(pStr, kFDSave, paFileTypes, pFrame);
    pFrame->Init(pName, pFileBox, pFileBox);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddSaveFileName(const Char_t *pName, Char_t *pStr,
                                HSUFileOpen::EHSUFiles eFileType)
{
// See HSUDialog::AddOpenFileName and HSUDialog::AddSaveFileName
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUFileOpen    *pFileBox;

    pFileBox = new HSUFileOpen(pStr, kFDSave, eFileType, pFrame);
    pFrame->Init(pName, pFileBox, pFileBox);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddSaveFileName(const Char_t *pName, TString *pStr,
                                HSUFileOpen::EHSUFiles eFileType)
{
// See HSUDialog::AddOpenFileName and HSUDialog::AddSaveFileName
HSUDialogFrame *pFrame = new HSUDialogFrame(this);
HSUFileOpen    *pFileBox;

    pFileBox = new HSUFileOpen(pStr, kFDSave, eFileType, pFrame);
    pFrame->Init(pName, pFileBox, pFileBox);
    AddChildFrame(pFrame);
}

//______________________________________________________________________________

void HSUDialog::AddEntry(const Char_t *s, Int_t iId)
{
// Add next entry to the PREVIOUS combo box or list box added to the dialog.
// The parameters define the sting in the display and a value which will
// be copy to the corresponding integer used in the widget's constructor
    if(m_pLastList == NULL)
    {
        Error("AddEntry", "No ListBox or ComboBox in the dialog");
        return;
    }

    m_pLastList->Add(s, iId);
}
