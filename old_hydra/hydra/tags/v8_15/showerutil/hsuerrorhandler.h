// @(#)$Id: hsuerrorhandler.h,v 1.1 2002-12-01 02:12:14 jaskula Exp $
//*-- Author : Marcin Jaskula 28/11/2002

#ifndef HSUErrorHandler_H
#define HSUErrorHandler_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HSUErrorHandler                                                            //
//                                                                            //
// Color error handler                                                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "TError.h"

// -----------------------------------------------------------------------------

class HSUErrorHandler
{
public:

    enum
    {
        kIdxInfo = 0,
        kIdxWarning,
        kIdxError,
        kIdxBreak,
        kIdxSysError,
        kIdxFatal,

        kIdxMax,

        kDefaultColor = 99
    };

    // -------------------------------------------------------------------------

    static Int_t iColors[kIdxMax];

    // -------------------------------------------------------------------------

    static void setColor(Int_t iIndex, Int_t iColor = kDefaultColor);

    // -------------------------------------------------------------------------

    static void setHandler(Bool_t bHSUErrorHandler = kTRUE);

    // -------------------------------------------------------------------------

    static void printError(int level, Bool_t abort, const char *location,
                              const char *msg);

    // -------------------------------------------------------------------------

private:

    static void setTermColor(Int_t iBackground, Int_t iForegorund);
    static void setTermColor(Int_t iC) { setTermColor((iC / 10) % 10, iC % 10);}
    static void resetTermColor(void)   { setTermColor(9, 9);                   }

};

// -----------------------------------------------------------------------------

#endif //HSUErrorHandler_H
