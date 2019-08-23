// $Id: hpaircontfact.cc,v 1.5 2008-05-09 16:14:32 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-04-19 12:47:06
//
// ---------------------------------------------------------------------------
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
//  HPairContFact
//
//  Factory for the parameter containers in libPairs
//
/////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <iomanip>
#include <iostream> 
// ---------------------------------------------------------------------------
#include <hruntimedb.h>
// ---------------------------------------------------------------------------
#include "hpaircontfact.h"
#include "hpaircutpar.h"
#include "hpaireffpar.h"
// ---------------------------------------------------------------------------
ClassImp(HPairContFact)
// ---------------------------------------------------------------------------
static HPairContFact gHPairContFact;  // instantiated when libPairs is loaded
// ---------------------------------------------------------------------------

HPairContFact::HPairContFact() 
{
  // Constructor (called when the library is loaded)
  fName="PairContFact";
  fTitle="Factory for parameter containers in libPairs";
  setAllContainers();
  HRuntimeDb::instance()->addContFactory(this);
}


//---------------------------------------------------------------------------- 

void HPairContFact::setAllContainers() 
{
  // Creates the Container objects with all accepted contexts and adds them to
  // the list of containers for the Pair library.

  HContainer* ca=new HContainer("PairCutParameters",
                                "Pair Analysis Parameters",
                                "PairDefault");
//   ca->addContext("PairPi0Dalitz");
//   ca->addContext("PairEta");

  containers->Add(ca);



  HContainer* ca2=new HContainer("PairEffParameters",
                                "Pair Efficiency Parameters",
                                "PairDefault");
  containers->Add(ca2);
}

//---------------------------------------------------------------------------- 

HParSet* HPairContFact::createContainer(HContainer* c) 
{
  // Calls the constructor of the corresponding parameter container.
  // For an actual context, which is not an empty string and not the default context
  // of this container, the name is concatenated with the context.
    const Char_t* name=c->GetName();
    if (strcmp(name,"PairCutParameters")==0)
	return new HPairCutPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    
    if (strcmp(name,"PairEffParameters")==0)
	return new HPairEffPar(c->getConcatName().Data(),c->GetTitle(),c->getContext());
    
  return 0;
}
