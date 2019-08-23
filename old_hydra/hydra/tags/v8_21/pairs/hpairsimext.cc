// $Id: hpairsimext.cc,v 1.2 2006-11-24 16:14:03 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-13 16:49:49
//
//
// ---------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPairSimExt                                                                 
// The only possible interface is HPidParticleSim
//                                                                           
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <iostream>
// ---------------------------------------------------------------------------
#include <hades.h>
// ---------------------------------------------------------------------------
#include "hpairsimext.h"
// ---------------------------------------------------------------------------
ClassImp(HPairSimExt)
// ---------------------------------------------------------------------------
HPairSimExt::HPairSimExt() 
{
    reset();
}

// ---------------------------------------------------------------------------
HPairSimExt::HPairSimExt(HPidParticleSim *p1, HPidParticleSim *p2  ) 
{
    reset();

    itsParticle1  = * (p1);
    itsParticle2  = * (p2);

    calcPairProperties(p1,p2);
}

// ---------------------------------------------------------------------------
HPairSimExt::~HPairSimExt() 
{

}

// ---------------------------------------------------------------------------
HPairSimExt::HPairSimExt(const HPairSimExt& source) 
{

}

// ---------------------------------------------------------------------------
void HPairSimExt::print()
{
    HPair::print();
}


// ---------------------------------------------------------------------------
void HPairSimExt::reset(void) 
{

}

// ---------------------------------------------------------------------------
Char_t HPairSimExt::getCharge(void) const
{

    return (itsParticle1.getCharge() + itsParticle2.getCharge());
}
// ---------------------------------------------------------------------------
void HPairSimExt::Clear(Option_t *opt)
{
    itsParticle1.Clear();
    itsParticle2.Clear();
    
}


