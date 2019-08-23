// @(#)$Id: funFiller.cc,v 1.1 2002-11-21 16:11:39 jaskula Exp $
// Source for condition in HPidParticleFiller (see pidFun.C)
// -----------------------------------------------------------------------------

#include "hpidparticlefiller.h"
#include "hpidcandidate.h"
#include "hpidtrackcand.h"

// -----------------------------------------------------------------------------

Long_t funFiller(const HPidCandidate *pCand, Int_t iPos)
{
// checks if the CL < 0.5 and PDF from Bayas > 0.5

    return ((HPidParticleFiller::checkParticlePar(pCand, iPos, 1, 0.5, ">"))
        && (HPidParticleFiller::checkParticlePar(pCand, iPos, 100, 0.5, "<")));
}
