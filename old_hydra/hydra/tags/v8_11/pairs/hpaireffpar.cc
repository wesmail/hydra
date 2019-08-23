// $Id: hpaireffpar.cc,v 1.2 2006-08-12 12:47:30 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-04-19 12:44:22
//
// ---------------------------------------------------------------------------
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPairEffPar
//                                                                           
// Container class for efficiency params
// will eventually hold histograms
// 
// 
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <fstream> 
#include <iomanip>
#include <iostream> 
// ---------------------------------------------------------------------------
#include <string.h>
// ---------------------------------------------------------------------------
#include <TDatime.h>
#include <TString.h>
// ---------------------------------------------------------------------------
#include <hpario.h>
#include <hdetpario.h>
#include <hparamlist.h>
// ---------------------------------------------------------------------------
#include "hpair.h"
// ---------------------------------------------------------------------------
#include "hpaireffpar.h"
// ---------------------------------------------------------------------------
ClassImp(HPairEffPar)
//---------------------------------------------------------------------------- 


HPairEffPar::HPairEffPar(const char* name,const char* title,
                                   const char* context)
                : HParCond(name,title,context) 
{

    clear();
}

//---------------------------------------------------------------------------- 

Bool_t HPairEffPar::init(HParIo* inp,Int_t* set) 
{
  // initializes the container from an input using the DEFAULT interface class
  HDetParIo* input=inp->getDetParIo("HCondParIo");

  if (input)   return (input->init(this,set));

  return kFALSE;
}

//---------------------------------------------------------------------------- 

Int_t HPairEffPar::write(HParIo* output) 
{
  // writes the container to an output using the DEFAULT interface class 
  HDetParIo* out=output->getDetParIo("HCondParIo");
  if (out) return out->write(this);
  return kFALSE;
}

//---------------------------------------------------------------------------- 

HPairEffPar::~HPairEffPar() 
{
    clear();
}

//---------------------------------------------------------------------------- 

void HPairEffPar::clear() 
{

}

//---------------------------------------------------------------------------- 

void HPairEffPar::printParam() 
{
    cout<<"################################################################"<<endl;
    cout<<"Name: "<<GetName()<<"\t\tTitle: "<<GetTitle()<<endl;
    cout<<"Author: "<<getAuthor()<<"\t\t\tContext: "<<getParamContext()<<endl;
    cout<<getDescription()<<endl;
    cout<<"----------------------------------------------------------------"<<endl;
    cout<<"################################################################"<<endl;
}

//---------------------------------------------------------------------------- 

void HPairEffPar::putParams(HParamList* l) 
{
  if (!l) return;

}

//---------------------------------------------------------------------------- 

Bool_t HPairEffPar::getParams(HParamList* l) 
{
  if (!l) return kFALSE;

  return kTRUE;
}

//---------------------------------------------------------------------------- 

Bool_t HPairEffPar::check(HPair* p)
{
    return kFALSE;
}

//---------------------------------------------------------------------------- 
