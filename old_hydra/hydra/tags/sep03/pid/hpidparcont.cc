// @(#)$Id: hpidparcont.cc,v 1.4 2003-04-03 13:29:41 jaskula Exp $
//*-- Author : Marcin Jaskula 09/10/2002

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

HPidParCont::HPidParCont(const char *name, const char *title,
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

    if((input = inp->getDetParIo(PID_PAR_IO_NAME)) != NULL)
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

    if((output = io->getDetParIo(PID_PAR_IO_NAME)) != NULL)
        return output->write(this);
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

        if(strcmp(io->IsA()->GetName(), "HParAsciiFileIo") == 0)
        {
            io->setDetParIo(new HPidParAsciiFileIo(
                                ((HParAsciiFileIo *) io)->getFile()));
            io->setInputNumber(i);

            ::Warning("HPidParContMaker::makePidParIo",
                    "Input %d set to HPidParAsciiFileIo", i);

            continue;
        }

        if(strcmp(io->IsA()->GetName(), "HParRootFileIo")==0)
        {
        HPidParRootFileIo *p = new HPidParRootFileIo(
                                ((HParRootFileIo *) io)->getParRootFile());

            io->setDetParIo(p);
            io->setInputNumber(i);

            //::Warning("HPidParContMaker::makePidParIo",
            //      "Input %d set to HPidParRootFileIo", i);

            continue;
        }

        // oracle should go here
        ::Error("HPidParContMaker::makePidParIo",
                "No interface for input %d name: %s", i, io->IsA()->GetName());
    }


    if(((io = rtdb->getOutput()) != NULL)
            && (io->getDetParIo(PID_PAR_IO_NAME) == NULL))
    {
        ::Warning("HPidParContMaker::makePidParIo", "Output name: %s",
                io->IsA()->GetName());

        if(strcmp(io->IsA()->GetName(), "HParAsciiFileIo") ==0 )
        {
            io->setDetParIo(new HPidParAsciiFileIo(
                    ((HParAsciiFileIo *) io)->getFile()));

            //::Warning("HPidParContMaker::makePidParIo",
            //      "Ouput set to HPidParRootFileIo");

            goto lab_AfterOutput;
        }

        if (strcmp(io->IsA()->GetName(), "HParRootFileIo") == 0)
        {
            io->setDetParIo(
                    new HPidParRootFileIo(
                    ((HParRootFileIo *) io)->getParRootFile()));

            goto lab_AfterOutput;
        }

        // oracle should go here
        ::Error("HPidParContMaker::makePidParIo",
                "No interface for output name: %s", io->IsA()->GetName());
    }

lab_AfterOutput:

    pTempDir->cd();

    return kTRUE;
}
