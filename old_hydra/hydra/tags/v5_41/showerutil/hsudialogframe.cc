#pragma implementation

// -----------------------------------------------------------------------------

#include "hsudialogframe.h"
#include <TMath.h>
#include <TGWidget.h>

// -----------------------------------------------------------------------------

ClassImp(HSUDialogFrame);
ClassImp(HSUDialogLabel);

// -----------------------------------------------------------------------------

#define SAFE_DELETE(A) { if(A != NULL) { delete A; A = NULL; }}

// -----------------------------------------------------------------------------
//
// HSUDialogFrame
//
// -----------------------------------------------------------------------------

TGLayoutHints g_FrameLayoutHintsLabel(kLHintsLeft | kLHintsTop);
//TGLayoutHints g_FrameLayoutHintsWidget(kLHintsRight);
TGLayoutHints g_FrameLayoutHintsWidget(kLHintsLeft);

// -----------------------------------------------------------------------------

HSUDialogFrame::HSUDialogFrame(const TGWindow* p)
                : TGHorizontalFrame(p, 10, 10, 0)
{
    m_pLabel  = NULL;
    m_pWidget = NULL;
    m_pActive = NULL;
}

// -----------------------------------------------------------------------------

void HSUDialogFrame::Init(const Char_t *pLabel, TGFrame *pWidget,
                            HSUActiveWidget *pActive)
{
    m_pWidget = pWidget;
    m_pActive = pActive;

    if(pLabel != NULL)
    {
        m_pLabel = new HSUDialogLabel(this, pLabel);
        m_pLabel->SetTextJustify(kTextLeft | kTextCenterY);
        AddFrame(m_pLabel, &g_FrameLayoutHintsLabel);
    }

    AddFrame(m_pWidget, &g_FrameLayoutHintsWidget);
}

// -----------------------------------------------------------------------------

HSUDialogFrame::~HSUDialogFrame()
{
    SAFE_DELETE(m_pLabel);
    SAFE_DELETE(m_pWidget);
}

// -----------------------------------------------------------------------------

UInt_t HSUDialogFrame::GetLabelWidth(void) const
{
    return (m_pLabel == NULL) ? 0 : m_pLabel->GetDefaultWidth();
}

// -----------------------------------------------------------------------------

UInt_t HSUDialogFrame::GetWidgetWidth(void) const
{
    return m_pWidget->GetDefaultWidth();
}

// -----------------------------------------------------------------------------

UInt_t HSUDialogFrame::GetWidgetHeight(void) const
{
    return TMath::Max(m_pWidget->GetDefaultHeight(), m_pWidget->GetHeight());
}

// -----------------------------------------------------------------------------

void HSUDialogFrame::SetSize(UInt_t uiLabel, UInt_t uiWidget)
{
UInt_t uiH = m_pWidget->GetDefaultHeight();

    if(m_pLabel != NULL)
    {
        m_pLabel->Resize(uiLabel + 15, uiH);
        m_pWidget->Resize(uiWidget + 5, uiH);
    }
    else
        m_pWidget->Resize(uiLabel + uiWidget + 20, uiH);

    Resize(uiLabel + uiWidget + 15, uiH);
}
