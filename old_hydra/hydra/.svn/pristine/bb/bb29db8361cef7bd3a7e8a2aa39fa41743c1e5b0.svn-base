// @(#)$Id: hpidparcont.cc,v 1.7 2008-05-09 16:15:06 halo Exp $
//*-- Author : Marcin Jaskula 09/10/2002
// Modified  : Marcin Jaskula & Tomasz Wojcik 26/06/2002
//             Oracle interface added

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
// HPidParCont
//
// Base class for all containers which do not need any fancy for reading
// and writnig, so they may use HPidParRootFileIo and HpidParAsciiFileIo
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidparcont.h"
#include "hdetpario.h"
#include "hpario.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hdetparasciifileio.h"
#include "hdetparrootfileio.h"
#include "hparrootfileio.h"
#include "hpidparrootfileio.h"
#include "hpidparasciifileio.h"
#include "hparasciifileio.h"

#include "hparamlist.h"

#include <TError.h>
#include <TDirectory.h>
#include <unistd.h>
#include <stdio.h>

#include <time.h>

// -----------------------------------------------------------------------------

ClassImp(HPidParCont)

// -----------------------------------------------------------------------------

HPidParCont::HPidParCont(const Char_t *name, const Char_t *title,
                                const Char_t *context)
                                : HParCond(name, title, context)
{
// Constructor
// Fills description of the container with the current date
// and the author from the login of the user running the process

time_t tt = time(NULL);
Char_t   s[100];
Char_t  *p;

    // fill the current date into description
    strcpy(s, ctime(&tt));
    if((p = strchr(s, '\n')) != NULL)
        *p = '\0';

    setDescription(s);

    // set author
    if((p = getlogin()) != NULL)
        setAuthor(p);
}

// -----------------------------------------------------------------------------

HPidParCont::~HPidParCont(void)
{
// empty destructor
}

// -----------------------------------------------------------------------------

Bool_t HPidParCont::init(void)
{
// This method is called by the HRuntimeDb. The input is not selected, so we
// check the first and later the second one and call for each init(HParIo *)

HParIo     *io;
HRuntimeDb *rtdb;

    if((rtdb = gHades->getRuntimeDb()) == NULL)
    {
        Error("init()", "gHades->getRuntimeDb() == NULL");
        return kFALSE;
    }

    if((io = rtdb->getFirstInput()) != NULL)
    {
        if(init(io))
            return kTRUE;
    }

    if((io = rtdb->getSecondInput()) != NULL)
    {
        if(init(io))
            return kTRUE;
    }

    setInputVersion(-1, 2);

    Error("init", "%s not initialized from any input", GetName());

    return kFALSE;
}

// -----------------------------------------------------------------------------

Bool_t HPidParCont::init(HParIo *inp)
{
// Try to init the container using inp and PID_PAR_IO_NAME HDetParIo

    if(inp == NULL)
    {
        Error("init", "input == NULL");
        return kFALSE;
    }

    if(makePidParIo() == kFALSE)
        return kFALSE;

HDetParIo *input;

    if(((input = inp->getDetParIo(PID_PAR_IO_NAME)) != NULL)
            || ((input = inp->getDetParIo(PID_PAR_ORA_IO_NAME)) != NULL))
    {
        if(input->init(this, NULL))
        {
            detachHistograms();

            return kTRUE;
        }
    }
    else
        Error("init", "Cannot find HDetParIo: %s\n", PID_PAR_IO_NAME);

    return kFALSE;
}

// -----------------------------------------------------------------------------

Int_t HPidParCont::write(HParIo *io)
{
// Try to write the container using io and PID_PAR_IO_NAME HDetParIo

    if(io == NULL)
    {
        Error("init", "output == NULL");
        return kFALSE;
    }

    if(makePidParIo() == kFALSE)
        return kFALSE;

HDetParIo *output;

    if(((output = io->getDetParIo(PID_PAR_IO_NAME)) != NULL)
            || ((output = io->getDetParIo(PID_PAR_ORA_IO_NAME)) != NULL))
    {
        return output->write(this);
    }
    else
        Error("init", "Cannot find HDetParIo: %s\n", PID_PAR_IO_NAME);

    return -1;
}

// -----------------------------------------------------------------------------

void HPidParCont::detachHistograms(void)
{
// Function called after loading data from file.
// This is a dummy method to give the interface for this operation.
//
// When histograms are loaded from the file they add themself automatically
// to gFile. When the input file is closed then the histograms are removed
// as well. To eliminate this unpleasant feature pHist->SetDirectory(NULL)
// must be called.
// One should remember to delete the histograms in the clear() method
// and in the destructor as well.

}

// -----------------------------------------------------------------------------

Bool_t HPidParCont::makePidParIo(void)
{
// Add PID parameters' I/O to the runtime database

HRuntimeDb *rtdb;

    if((rtdb = gHades->getRuntimeDb()) == NULL)
    {
        ::Error("HPidParContMaker::makePidParIo",
                    "gHades->getRuntimeDb() == NULL");

        return kFALSE;
    }

HParIo     *io = NULL;
Int_t       i;
TDirectory *pTempDir = gDirectory;
Bool_t      bReturn = kTRUE;

    if(rtdb == NULL)
    {
        ::Error("HPidParContMaker::makePidParIo", "rtdb == NULL");
        return kFALSE;
    }

    for(i = 1; i <= 2; i++)
    {
        if((io = (i == 1) ? rtdb->getFirstInput() : rtdb->getSecondInput())
                == NULL)
        {
            continue;
        }

        if(io->getDetParIo(PID_PAR_IO_NAME) != NULL)
            continue;

        if(io->getDetParIo(PID_PAR_ORA_IO_NAME) != NULL)
            continue;

        if(strcmp(io->IsA()->GetName(), "HParAsciiFileIo") == 0)
        {
            io->setDetParIo(new HPidParAsciiFileIo(
                                ((HParAsciiFileIo *) io)->getFile()));
            io->setInputNumber(i);

            continue;
        }

        if(strcmp(io->IsA()->GetName(), "HParRootFileIo")==0)
        {
        HPidParRootFileIo *p = new HPidParRootFileIo(
                                ((HParRootFileIo *) io)->getParRootFile());

            io->setDetParIo(p);
            io->setInputNumber(i);

            continue;
        }

        // oracle goes here
        if(strcmp(io->IsA()->GetName(), "HParOraIo") == 0)
        {
            ::Error("HPidParContMaker::makePidParIo",
                "Oracle interface %s (input %d) has not been creted (call IK)",
                PID_PAR_ORA_IO_NAME, i);

            bReturn = kFALSE;

            continue;
        }

        ::Error("HPidParContMaker::makePidParIo",
                "Unsupported interface for input %d name: %s",
                i, io->IsA()->GetName());

        bReturn = kFALSE;
    }

    if(((io = rtdb->getOutput()) != NULL)
            && (io->getDetParIo(PID_PAR_IO_NAME) == NULL))
    {
        if(io->getDetParIo(PID_PAR_ORA_IO_NAME) != NULL)
            goto lab_AfterOutput;

        if(strcmp(io->IsA()->GetName(), "HParAsciiFileIo") ==0 )
        {
            io->setDetParIo(new HPidParAsciiFileIo(
                    ((HParAsciiFileIo *) io)->getFile()));

            goto lab_AfterOutput;
        }

        if (strcmp(io->IsA()->GetName(), "HParRootFileIo") == 0)
        {
            io->setDetParIo(
                    new HPidParRootFileIo(
                    ((HParRootFileIo *) io)->getParRootFile()));

            goto lab_AfterOutput;
        }

        // oracle goes here
        if(strcmp(io->IsA()->GetName(), "HParOraIo") == 0)
        {
            ::Error("HPidParContMaker::makePidParIo",
                "Oracle interface %s (output) has not been creted (call IK)",
                PID_PAR_ORA_IO_NAME);
        }
        else
        {
            ::Error("HPidParContMaker::makePidParIo",
                    "Unsupported interface for output name: %s",
                    io->IsA()->GetName());
        }

        bReturn = kFALSE;
    }

lab_AfterOutput:

    pTempDir->cd();

    return bReturn;
}
