#pragma implementation
/* ************************************************************************** */
/*                                                                            */
/*   Some progress bar functions working as a reconstructor                   */
/*                                                                            */
/*                                                          MJ. 08.07.2001    */
/* ************************************************************************** */

#include <Rtypes.h>
#include <TString.h>
#include <stdio.h>
#include "hsuprogressrec.h"

ClassImp(HSUProgressRec)

/* ************************************************************************** */

HSUProgressRec::HSUProgressRec(Text_t *pName,Text_t *pTitle, int iMax) :
        HReconstructor(pName, pTitle), HSUProgress(iMax)
{
}

/* ************************************************************************** */

HSUProgressRec::~HSUProgressRec()
{
}

/* ************************************************************************** */

Bool_t HSUProgressRec::init(void)
{
//    Restart();

    return kTRUE;
}

/* ************************************************************************** */

Bool_t HSUProgressRec::reinit(void)
{
    Restart();

    return kTRUE;
}

/* ************************************************************************** */

Bool_t HSUProgressRec::finalize(void)
{
    Final();
    return kTRUE;
}

/* ************************************************************************** */

Int_t HSUProgressRec::execute(void)
{
    Next();

    return 0;
}
