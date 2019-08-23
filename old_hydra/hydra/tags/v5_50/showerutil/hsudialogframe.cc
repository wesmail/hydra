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
#define ALONE_LABEL_PART 0.3

// -----------------------------------------------------------------------------
//
// HSUDialogFrame
//
// -----------------------------------------------------------------------------

TGLayoutHints g_FrameLayoutHintsLabel(kLHintsLeft | kLHintsTop);
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

    if(m_pWidget != NULL)
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
    if(m_pLabel == NULL)
	{
		if(m_pWidget == NULL) 
			return 0;

		return (UInt_t)(m_pWidget->GetDefaultWidth() * ALONE_LABEL_PART);
	}

	if(m_pWidget != NULL)
		return m_pLabel->GetDefaultWidth();

    return (UInt_t)(m_pLabel->GetDefaultWidth() * ALONE_LABEL_PART);
}

// -----------------------------------------------------------------------------

UInt_t HSUDialogFrame::GetWidgetWidth(void) const
{
    if(m_pLabel == NULL)
	{
		if(m_pWidget == NULL) 
			return 0;

		return (UInt_t)(m_pWidget->GetDefaultWidth() 
						* (1.0 - ALONE_LABEL_PART));
	}

	if(m_pWidget != NULL)
		return m_pWidget->GetDefaultWidth();

    return (UInt_t)(m_pLabel->GetDefaultWidth() * (1.0 - ALONE_LABEL_PART));

    return (UInt_t)(m_pLabel->GetDefaultWidth() * (1.0 - ALONE_LABEL_PART));
}

// -----------------------------------------------------------------------------

UInt_t HSUDialogFrame::GetWidgetHeight(void) const
{
    if(m_pWidget != NULL)
    {
        return TMath::Max(m_pWidget->GetDefaultHeight(),
                                            m_pWidget->GetHeight());
    }
    else
    {
        if(m_pLabel != NULL)
            return m_pLabel->GetDefaultHeight();
    }

    return 1;
}

// -----------------------------------------------------------------------------

void HSUDialogFrame::SetSize(UInt_t uiLabel, UInt_t uiWidget)
{
UInt_t uiH = GetWidgetHeight();

    if(m_pLabel != NULL)
    {
        if(m_pWidget != NULL)
        {
            m_pLabel->Resize(uiLabel + 15, uiH);
            m_pWidget->Resize(uiWidget + 5, uiH);
        }
        else
            m_pLabel->Resize(uiLabel + uiWidget + 20, uiH);
    }
    else
    {
        if(m_pWidget != NULL)
            m_pWidget->Resize(uiLabel + uiWidget + 20, uiH);
    }

    Resize(uiLabel + uiWidget + 15, uiH);
}
