#ifndef HSUDIALOGFRAME_H
#define HSUDIALOGFRAME_H

#include "TGFrame.h"
#include "TGLabel.h"

// -----------------------------------------------------------------------------

class TGButton;
class HSUList;
class HSUActiveWidget;
class HSUDialogLabel;

// -----------------------------------------------------------------------------

class HSUDialogFrame : public TGHorizontalFrame
{
protected:
    HSUDialogLabel     *m_pLabel;
    TGFrame            *m_pWidget;
    HSUActiveWidget    *m_pActive;

public:
                        HSUDialogFrame(const TGWindow* p);
    virtual            ~HSUDialogFrame();

    void                Init(const Char_t *pLabel, TGFrame *pWidget,
                                    HSUActiveWidget *pActive);

    UInt_t              GetLabelWidth(void) const;
    UInt_t              GetWidgetWidth(void) const;
    UInt_t              GetWidgetHeight(void) const;

    void                SetSize(UInt_t uiLabel, UInt_t uiWidget);

    HSUActiveWidget*    GetActiveWidget(void)   { return m_pActive; }

    ClassDef(HSUDialogFrame, 0)  // One dialog box row in ShowerUtil GUI
};

// -----------------------------------------------------------------------------

class HSUDialogLabel : public TGLabel
{
public:

                        HSUDialogLabel(const TGWindow* p, const Char_t* text)
                                : TGLabel(p, text) {}

    virtual TGDimension GetDefaultSize() const
                                { return TGDimension(fWidth, fTHeight); }

    ClassDef(HSUDialogLabel, 0)  // A dialog label in ShowerUtil GUI
};

#endif //!HSUDIALOGFRAME_H
