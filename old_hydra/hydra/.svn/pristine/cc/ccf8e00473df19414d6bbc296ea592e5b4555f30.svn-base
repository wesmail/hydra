// @(#)$Id: hsupaddivider.h,v 1.1 2003-07-11 10:41:01 jaskula Exp $
//*-- Author : Marcin Jaskula 08/04/2003

#ifndef HSUPadDivider_H
#define HSUPadDivider_H

////////////////////////////////////////////////////////////////////////////////
//
// HSUPadDivider
//
////////////////////////////////////////////////////////////////////////////////

#include <TCanvas.h>
#include <TObjArray.h>

// -----------------------------------------------------------------------------

class HSUPadDivider
{
public:
                    HSUPadDivider(TPad *pPad, UInt_t iRows, UInt_t iCols,
                        Float_t fLeftMarg = -1.0f, Float_t fBottomMarg = -1.0f);


    TVirtualPad*    getPad(Int_t i) const
                        { return (TVirtualPad *) arr.At(i - 1); }

private:

    TObjArray       arr;
};

// -----------------------------------------------------------------------------

#endif //HSUPadDivider_H
