#ifndef HSUACTIVEWIDGET_H
#define HSUACTIVEWIDGET_H
#pragma interface

// -----------------------------------------------------------------------------

#include "TROOT.h"

// -----------------------------------------------------------------------------

class HSUActiveWidget
{
public:
    HSUActiveWidget(){};
    virtual ~HSUActiveWidget(){};

    virtual void    Reset(void)   = 0;
    virtual void    SetData(void) = 0;
    virtual void    DataChanged(void) {};

    // -------------------------------------------------------------------------

    ClassDef(HSUActiveWidget, 0)    //! Base virtual class for ShowerUtil GUI
};

#endif //!HSUACTIVEWIDGET_H
