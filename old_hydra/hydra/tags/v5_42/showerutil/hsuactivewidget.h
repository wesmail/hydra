#ifndef HSUACTIVEWIDGET_H
#define HSUACTIVEWIDGET_H
#pragma interface

// -----------------------------------------------------------------------------

#include <TROOT.h>

// -----------------------------------------------------------------------------

class HSUActiveWidget
{
public:
    virtual void    Reset(void)   = 0;
    virtual void    SetData(void) = 0;
    virtual void    DataChanged(void) {};

    // -------------------------------------------------------------------------

    ClassDef(HSUActiveWidget, 0)
};

#endif //!HSUACTIVEWIDGET_H
