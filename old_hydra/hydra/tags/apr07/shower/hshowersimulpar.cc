//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
// HShowerSimulPar
//
// Base class for all containers which do not need any fancy for reading
// and writnig, so they may use HShowerParRootFileIo and HShowerParAsciiFileIo
//
////////////////////////////////////////////////////////////////////////////////

#include "hshowersimulpar.h"
#include "hdetpario.h"
#include "hpario.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hdetparasciifileio.h"
#include "hdetparrootfileio.h"
#include "hparrootfileio.h"
#include "hshowerparrootfileio.h"
#include "hshowerparasciifileio.h"
#include "hparasciifileio.h"

#include "hparamlist.h"

#include <TError.h>
#include <TDirectory.h>
#include <unistd.h>
#include <stdio.h>

#include <time.h>

// -----------------------------------------------------------------------------

ClassImp(HShowerSimulPar)

// -----------------------------------------------------------------------------

HShowerSimulPar::HShowerSimulPar(const char *name, const char *title,
                                const char *context)
                                : HParCond(name, title, context)
{
// Constructor
// Fills description of the container with the current date
// and the author from the login of the user running the process

time_t tt = time(NULL);
char   s[100];
char  *p;

    // fill the current date into description
    strcpy(s, ctime(&tt));
    if((p = strchr(s, '\n')) != NULL)
        *p = '\0';

    setDescription(s);

    // set author
    if((p = getlogin()) != NULL)
        setAuthor(p);

	// set arrays size (18 chambers)
	m_fGain.Set(18);
	m_fThreshold.Set(18);

	// reset arrays
	m_fGain.Reset(0);
	m_fThreshold.Reset(0);
}

// -----------------------------------------------------------------------------

HShowerSimulPar::~HShowerSimulPar(void)
{
// empty destructor
}

// -----------------------------------------------------------------------------

void HShowerSimulPar::printParam(void) 
{
	// prints HShowerSimulPar
	//
	for(int iChamber=0; iChamber<18; iChamber++)
	{
		printf("\t Chamber %d Gain %4.2f, Threshold %4.2f  \n",
					iChamber,m_fGain[iChamber],m_fThreshold[iChamber]);
	}
}

// -----------------------------------------------------------------------------

Bool_t HShowerSimulPar::init(HParIo *inp, Int_t* set)
{
// Try to init the container using inp and SHOWER_PAR_IO_NAME HDetParIo

    if(inp == NULL)
    {
        Error("init", "input == NULL");
        return kFALSE;
    }

HDetParIo *input;

    if(((input = inp->getDetParIo(SHOWER_PAR_IO_NAME)) != NULL)
            || ((input = inp->getDetParIo(SHOWER_PAR_ORA_IO_NAME)) != NULL))
    {
        if(input->init(this, set))
        {
            return kTRUE;
        }
    }
    else
        Error("init", "Cannot find HDetParIo: %s\n", SHOWER_PAR_IO_NAME);

    return kFALSE;
}

// -----------------------------------------------------------------------------

Int_t HShowerSimulPar::write(HParIo *io)
{
// Try to write the container using io and SHOWER_PAR_IO_NAME HDetParIo

    if(io == NULL)
    {
        Error("init", "output == NULL");
        return kFALSE;
    }

HDetParIo *output;

    if(((output = io->getDetParIo(SHOWER_PAR_IO_NAME)) != NULL)
            || ((output = io->getDetParIo(SHOWER_PAR_ORA_IO_NAME)) != NULL))
    {
        return output->write(this);
    }
    else
        Error("init", "Cannot find HDetParIo: %s\n", SHOWER_PAR_IO_NAME);

    return -1;
}

// -----------------------------------------------------------------------------

void HShowerSimulPar::putParams(HParamList* l)
{
	if(l!=NULL)
	{
		l->add("m_fGain",m_fGain,3);
		l->add("m_fThreshold",m_fThreshold,3);
	}
}

// -----------------------------------------------------------------------------

Bool_t HShowerSimulPar::getParams(HParamList* l)
{
	if(!l) return kFALSE;

	if( !(l->fill("m_fGain",&m_fGain)) ) return kFALSE;
	if( !(l->fill("m_fThreshold",&m_fThreshold)) ) return kFALSE;

return kTRUE;	
}

// -----------------------------------------------------------------------------
