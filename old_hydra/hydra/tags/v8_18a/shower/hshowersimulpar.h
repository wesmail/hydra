//*-- Author : Jacek Otwinowski	 27/09/2006

#ifndef HSHOWERSIMULPAR_H
#define HSHOWERSIMULPAR_H

////////////////////////////////////////////////////////////////////////////////
//
// HShowerSimulPar
//
// Base class for shower simulation parameters
//
////////////////////////////////////////////////////////////////////////////////

#include <TArrayF.h>
#include "hparcond.h"

// -----------------------------------------------------------------------------

#define SHOWERSIMUL_PAR_NAME     "ShowerSimulPar"
#define SHOWERSIMUL_PAR_TITLE    "Parameters for HShowerSimultPar"
#define SHOWERSIMUL_PAR_CONTEXT  "ShowerStandardSimul"

#define SHOWER_PAR_ORA_IO_NAME "ShowerParIo"
#define SHOWER_PAR_IO_NAME     "HCondParIo"

// -----------------------------------------------------------------------------

class HShowerSimulPar : public HParCond
{
public:
                    HShowerSimulPar(const Char_t *name=SHOWERSIMUL_PAR_NAME, 
									const Char_t *title=SHOWERSIMUL_PAR_TITLE,
                                	const Char_t *context=SHOWERSIMUL_PAR_CONTEXT);
    virtual        ~HShowerSimulPar(void);

    // -------------------------------------------------------------------------
	
    Bool_t  init(HParIo *inp, Int_t *);
    Int_t   write(HParIo *io);

    // -------------------------------------------------------------------------

    void    printParam(void);

    // -------------------------------------------------------------------------

    void    putParams(HParamList *);
    Bool_t  getParams(HParamList *);

    // -------------------------------------------------------------------------

	void setGain(Int_t number, Float_t value)
	{
		m_fGain.AddAt(value,number);
	}
	
	Float_t getGain(Int_t number)
	{
		return m_fGain.At(number);
	}

	void setThreshold(Int_t number, Float_t value)
	{
		m_fThreshold.AddAt(value,number);
	}
	
	Float_t getThreshold(Int_t number)
	{
		return m_fThreshold.At(number);
	}

    ClassDef(HShowerSimulPar, 1)  

private:
	
    // gain of the 18 chambers
	TArrayF m_fGain;

	// charege threshold for efficiency determination for 18 chambers
	TArrayF m_fThreshold;
};

// -----------------------------------------------------------------------------

#endif  /* !HSHOWERSIMULPAR_H */




