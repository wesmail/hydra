#pragma implementation
#include "hshowercalparcell.h"
#include "TClass.h"
#include "hdataobject.h"

ClassImp(HShowerCalParCell)

//*-- Author : Leszek Kidon
//*-- Created: 1998

//*-- Modified: Marcin Jaskula
//*-- Sat Sep  1 18:23:44 CEST 2001

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
// HShowerCalParCell
//
// HShowerCalParCell contains slope and offset parameters
// for calibration of one pad
//
//////////////////////////////////////////////////////////////////////

HShowerCalParCell::HShowerCalParCell() {
    reset();
}

// -----------------------------------------------------------------------------

HShowerCalParCell::HShowerCalParCell(Float_t fSlope, Float_t fOffset) {
    reset();
    setParams(fSlope, fOffset, 0.0, 1.0);
}

// -----------------------------------------------------------------------------

HShowerCalParCell::HShowerCalParCell(Float_t fSlope, Float_t fOffset,
                                    Float_t fThreshold, Float_t fGain)
{
    reset();
    setParams(fSlope, fOffset, fThreshold, fGain);
}

// -----------------------------------------------------------------------------

Int_t HShowerCalParCell::reset()
{
   setParams(1.0, 0.0, 0.0, 1.0);
   return 1;
}

// -----------------------------------------------------------------------------

void HShowerCalParCell::setParams(Float_t fSlope, Float_t fOffset) {
//set slope and offset
    setSlope(fSlope);
    setOffset(fOffset);
}

// -----------------------------------------------------------------------------

void HShowerCalParCell::setParams(Float_t fSlope, Float_t fOffset,
                        Float_t fThreshold, Float_t fGain)
{
    setSlope(fSlope);
    setOffset(fOffset);
    setThreshold(fThreshold);
    setGain(fGain);
}

// -----------------------------------------------------------------------------

void HShowerCalParCell::Streamer(TBuffer &R__b)
{
   // Stream an object of class HShowerCalParCell.

    if (R__b.IsReading())
    {
    Version_t R__v = R__b.ReadVersion();

        if(R__v <= 2)
        {
        Int_t     iAsicNr;

            if(R__v == 1)
            {
            Version_t R__v;

                R__v = R__b.ReadVersion(); // simulate HDataObject v 1 readout
            }
            else
            {
                Warning("Streamer", "Class Version 2 CORRUPTED !!!\n"
                            "\t\t\t\tCrash possible\n");
            }

            TObject::Streamer(R__b);

            R__b >> m_fOffset;
            R__b >> m_fSlope;
            R__b >> iAsicNr;

            m_fThreshold = m_fOffset;
            m_fGain = 1.0f;
        }
        else
        {
            TObject::Streamer(R__b);

            R__b >> m_fOffset;
            R__b >> m_fSlope;
            R__b >> m_fThreshold;
            R__b >> m_fGain;
        }
    }
    else
    {
        R__b.WriteVersion(HShowerCalParCell::IsA());

        TObject::Streamer(R__b);

        R__b << m_fOffset;
        R__b << m_fSlope;
        R__b << m_fThreshold;
        R__b << m_fGain;
    }
}

// -----------------------------------------------------------------------------

void HShowerCalParCell::print(void)
{
    printf("S: %f  O: %f  T: %f  G: %f\n", getSlope(), getOffset(),
                    getThreshold(), getGain());
}
