// $Id: hpaircutpar.cc,v 1.10 2006-08-12 12:47:30 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-03-28 18:28:11
//
// ---------------------------------------------------------------------------
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//                                                                           
// HPairCutPar
//                                                                           
// Container class for Pair analysis parameters
// 
// A cut in the context of this container always means that a pair object
// is set inactive via a flag. No objects are deleted from their category.
// For each cut there is a value which serves as a threshold and a flag
// which states whether the cut is active or not. Different value and flag
// combinations should be set via contexts.
// A cut can be requested as recursive. This means that all pairs containing
// a particle from a cut away pair will also be cut. Useful for an
// identified and unwanted pair type (close pair or conversion pair e.g.).
// A list with indices for particles cut by a recursive cut is kept. All 
// recursive cuts contribute to this list. Only new indices are added. At
// the end of the event loop all pairs are checked again, those having a 
// matching particle are set inactive. Requesting a cut as recursive 
// inevitably means two passes over all objects in the event. 
// The check(HPair*) function evaluates the active cuts and returns a boolean
// decision on the tested object. A bit field with a fixed order for the 
// represented cuts is initialized once when retrieving params from the 
// param source. Activated cuts are then evaluated and a second bit field
// is filled with the cut results. This result bit field is created for
// each checked object and can be retrieved from the param container.
///////////////////////////////////////////////////////////////////////////////
//
using namespace std;
// ---------------------------------------------------------------------------
#include <TObjArray.h>
// ---------------------------------------------------------------------------
#include <hades.h>
#include <hpario.h>
#include <hdetpario.h>
#include <hiterator.h>
#include <hparamlist.h>
// ---------------------------------------------------------------------------
#include "hpair.h"
#include "hpairsim.h"
#include "hpairfl.h"
#include "hpairdata.h"
// ---------------------------------------------------------------------------
#include "hpaircutpar.h"
// ---------------------------------------------------------------------------
ClassImp(HPairCutPar)
//---------------------------------------------------------------------------- 
// cut order for bit fields
      const UInt_t HPairCutPar::NOCUT = 0;
      const UInt_t HPairCutPar::DIRECTCUT = 1;
      const UInt_t HPairCutPar::OPANG = 2;
      const UInt_t HPairCutPar::DOUBLEHIT = 3;
      const UInt_t HPairCutPar::OPANGDOUBLEHIT = 10;

//---------------------------------------------------------------------------- 
HPairCutPar::HPairCutPar(const char* name,const char* title,
                                   const char* context)
                : HParCond(name,title,context) 
{
    pActiveCutNames = new TString("");
    pArrCutPairs = new TObjArray(5);
    
    clear();
}

//---------------------------------------------------------------------------- 

Bool_t HPairCutPar::init(HParIo* inp,Int_t* set) 
{
  // initializes the container from an input using the DEFAULT interface class
  HDetParIo* input=inp->getDetParIo("HCondParIo");
  if (input)   return (input->init(this,set));

  return kFALSE;
}

//---------------------------------------------------------------------------- 

Int_t HPairCutPar::write(HParIo* output) 
{
  // writes the container to an output using the DEFAULT interface class 
  HDetParIo* out=output->getDetParIo("HCondParIo");
  if (out) return out->write(this);
  return kFALSE;
}

//---------------------------------------------------------------------------- 

HPairCutPar::~HPairCutPar() 
{
     if (pArrCutPairs) delete pArrCutPairs;
     //     if (pActiveCutNames) delete pActiveCutNames;
}

//---------------------------------------------------------------------------- 

void HPairCutPar::clear() 
{
    nBitFieldFlags = 0;
    nBitFieldPassedCuts = 0;
    nBitFieldRecursiveFlags = 0;
    fPairOpeningAngle = -1.;
    nPairOpeningAngleFlag = -1;
    nPairOpeningAngleRecFlag = -1;

    nDoubleDetectorHitFlag = -1;
    nDoubleDetectorHitRecFlag = -1;
}

//---------------------------------------------------------------------------- 

void HPairCutPar::printParam() 
{
    cout<<"################################################################"<<endl;
    cout<<"Name: "<<GetName()<<"\t\tTitle: "<<GetTitle()<<endl;
    cout<<"Author: "<<getAuthor()<<"\t\t\tContext: "<<getParamContext()<<endl;
    cout<<getDescription()<<endl;
    cout<<"----------------------------------------------------------------"<<endl;
    cout<<"PairOpeningAngleFlag:\t"<<nPairOpeningAngleFlag<<endl;
    cout<<"PairOpeningAngleRecFlag:\t"<<nPairOpeningAngleRecFlag<<endl;
    cout<<"PairOpeningAngle:\t"<<fPairOpeningAngle<<endl;
    cout<<"DoubleDetectorHitFlag:\t"<<nDoubleDetectorHitFlag<<endl;
    cout<<"DoubleDetectorHitRecFlag:\t"<<nDoubleDetectorHitRecFlag<<endl;
    cout<<"################################################################"<<endl;
}

//---------------------------------------------------------------------------- 

void HPairCutPar::putParams(HParamList* l) 
{
  if (!l) return;
  l->add("PairOpeningAngle",fPairOpeningAngle);
  l->add("PairOpeningAngleFlag",nPairOpeningAngleFlag);
  l->add("PairOpeningAngleRecFlag",nPairOpeningAngleRecFlag);
  l->add("DoubleDetectorHitFlag",nDoubleDetectorHitFlag);
  l->add("DoubleDetectorHitRecFlag",nDoubleDetectorHitRecFlag);
}

//---------------------------------------------------------------------------- 

Bool_t HPairCutPar::getParams(HParamList* l) 
{
  if (!l) return kFALSE;

  // ----------------- OPANG
  if (!l->fill("PairOpeningAngle",&fPairOpeningAngle)) return kFALSE;
  if (!l->fill("PairOpeningAngleFlag",&nPairOpeningAngleFlag)) return kFALSE;
  if (!l->fill("PairOpeningAngleRecFlag",&nPairOpeningAngleRecFlag)) return kFALSE;
  if (!l->fill("DoubleDetectorHitFlag",&nDoubleDetectorHitFlag)) return kFALSE;
  if (!l->fill("DoubleDetectorHitRecFlag",&nDoubleDetectorHitRecFlag)) return kFALSE;

  // fill bit field for flags
  if (nPairOpeningAngleFlag) HPairFL::setBit((UInt_t)OPANG,nBitFieldFlags);
  else HPairFL::clearBit((UInt_t)OPANG,nBitFieldFlags);
  if (nPairOpeningAngleFlag&&nPairOpeningAngleRecFlag) 
      HPairFL::setBit((UInt_t)OPANG,nBitFieldRecursiveFlags);
  else HPairFL::clearBit((UInt_t)OPANG,nBitFieldRecursiveFlags);

  if (nDoubleDetectorHitFlag) HPairFL::setBit((UInt_t)DOUBLEHIT,nBitFieldFlags);
  else HPairFL::clearBit((UInt_t)DOUBLEHIT,nBitFieldFlags);
  if (nDoubleDetectorHitFlag&&nDoubleDetectorHitRecFlag) 
      HPairFL::setBit((UInt_t)DOUBLEHIT,nBitFieldRecursiveFlags);
  else HPairFL::clearBit((UInt_t)DOUBLEHIT,nBitFieldRecursiveFlags);

  // set active cuts string
  if (nPairOpeningAngleFlag) pActiveCutNames->Append("isGoodOpang");
  if (nDoubleDetectorHitFlag) pActiveCutNames->Append(":isNotDoubleHit");
  // ----------------- XXXX
  
  //  HPairFL::printBitField(nBitFieldFlags);
  return kTRUE;
}

//---------------------------------------------------------------------------- 

Bool_t HPairCutPar::check(HPair* p)
{

    if (p==0)
    {
	::Error("check","input pair is null");
	return kFALSE;
    }

    // reset bit field with passed flags for this pair
    nBitFieldPassedCuts=0;


    // fill bit field with 'passed' flags

    // check opening angle - if active
    if (nPairOpeningAngleFlag)
    {
	if (checkOpeningAngle(p)) 
	{   // pair has passed
	    HPairFL::setBit(OPANG,nBitFieldPassedCuts);
	}
	else 
	{
	    HPairFL::clearBit(OPANG,nBitFieldPassedCuts);
	
	    if (nPairOpeningAngleRecFlag) // cut was requested as recursive
	    {
	    // store bad pair
		if (pArrCutPairs) pArrCutPairs->Add(p);
		else ::Error("check","bad pair array is NULL");
	    }
	}
    }

    // check double hit - if active
     if (nDoubleDetectorHitFlag)
    {
	if (!checkCommonDetectorHit(p)) 
	{   // pair has passed
	    HPairFL::setBit(DOUBLEHIT,nBitFieldPassedCuts);
	}
	else 
	{
	    HPairFL::clearBit(DOUBLEHIT,nBitFieldPassedCuts);
	
	    if (nDoubleDetectorHitRecFlag) // cut was requested as recursive
	    {
	    // store bad pair
		if (pArrCutPairs) pArrCutPairs->Add(p);
		else ::Error("check","bad pair array is NULL");
	    }
	}
    }
    
    // store the bit field indicating passed cuts in the pair
    p->setBitFieldPassedCuts(nBitFieldPassedCuts);

    // check which active cuts were actually passed
    Int_t tmp=nBitFieldPassedCuts&nBitFieldFlags;

    Bool_t kRetValue = kFALSE;
    // if all active cuts were passed, pair has passed
    if (nBitFieldFlags==tmp) 
    {   // set flag indicating pair was not cut
	p->setIsCutFlag(NOCUT);
	kRetValue = kTRUE;
    }
    else 
    {   // pair is flagged as cut
	p->setIsCutFlag(DIRECTCUT); 
	kRetValue = kFALSE;
    }
    return kRetValue;
}

//---------------------------------------------------------------------------- 

Bool_t HPairCutPar::checkOpeningAngle(HPair* p)
{
    if (p->getOpeningAngleDeg() > fPairOpeningAngle) return kTRUE;
    return kFALSE;
}

//---------------------------------------------------------------------------- 

Bool_t  HPairCutPar::checkCommonDetectorHit(HPair* p)
{
// if the pair has common hit return kTRUE
HPairData pd(p);

    if(pd.hasDoubleDetectorHits() > 0) return kTRUE;
    return kFALSE;
}

//---------------------------------------------------------------------------- 

Int_t HPairCutPar::getNbActiveCuts()
{
    return HPairFL::countPositiveBits(nBitFieldFlags);
}

//---------------------------------------------------------------------------- 

Bool_t HPairCutPar::checkRecursive(HIterator* pIter)
{
    // This function is executed at the end of the execute function of the 
    // filter. The array pArrCutPairs is filled with bad pairs
    // The array nParts is filled here with those
    // particle indices from bad pairs for this event

    if (pArrCutPairs==0)
    {
	::Error("checkRecursive","bad pair array is 0");
	return kFALSE;
    }

    if (pIter==0)
    {
	::Error("checkRecursive","pair cat iterator is 0");
	return kFALSE;
    }

    Int_t nMaxNbGuiltyParticles = 2*(pArrCutPairs->GetLast()+1);
    Int_t *nOpang = new Int_t[nMaxNbGuiltyParticles];
    Int_t *nDouble = new Int_t[nMaxNbGuiltyParticles];

    for (Int_t j=0;j<nMaxNbGuiltyParticles;j++) nOpang[j]=-2;
    for (Int_t j=0;j<nMaxNbGuiltyParticles;j++) nDouble[j]=-2;

    for (Int_t i=0; i<pArrCutPairs->GetLast()+1; i++)
    {
	HPair* p = (HPair*)(*pArrCutPairs)[i];

    
	if(nPairOpeningAngleFlag)
	{
	  
      Int_t nBitField =  p->getBitFieldPassedCuts();	
	  if(HPairFL::getBit(OPANG,nBitField) == 0)
	  {
	    HPairFL::isNewIndex(p->getIndexParticle1(),nOpang,nMaxNbGuiltyParticles);
	    HPairFL::isNewIndex(p->getIndexParticle2(),nOpang,nMaxNbGuiltyParticles);
	  }
	}

	if(nDoubleDetectorHitFlag)
	{
      Int_t nBitField =  p->getBitFieldPassedCuts();	
	  if(HPairFL::getBit(DOUBLEHIT,nBitField) == 0)
	  {
	    HPairFL::isNewIndex(p->getIndexParticle1(),nDouble,nMaxNbGuiltyParticles);
	    HPairFL::isNewIndex(p->getIndexParticle2(),nDouble,nMaxNbGuiltyParticles);
	  }
	}

    }

    // loop over uncut pairs in event and check for a particle from the list 
    pIter->Reset();
    if (gHades->isReal())
    {
	HPair* pPair = 0;
	while( (pPair = (HPair*) pIter->Next()) ) 
	{
	    if (pPair->getIsCutFlag()) continue; // already cut
	    else 
	    { 
		  if(nPairOpeningAngleRecFlag)
		  {
		      cout<<"opang recursion"<<endl;
		    if (compareParticleIndicesWithList(pPair,nOpang,nMaxNbGuiltyParticles)) pPair->setIsCutFlag(OPANG);
		  }
			
		  if(nDoubleDetectorHitRecFlag)
		  {
		      cout<<"double hit recursion"<<endl;
		    if (compareParticleIndicesWithList(pPair,nDouble,nMaxNbGuiltyParticles)) pPair->setIsCutFlag(DOUBLEHIT);
		  }
		  if(nPairOpeningAngleRecFlag && nDoubleDetectorHitRecFlag)
		  {
            if(compareParticleIndicesWithList(pPair,nOpang,nMaxNbGuiltyParticles) && compareParticleIndicesWithList(pPair,nDouble,nMaxNbGuiltyParticles))
			pPair->setIsCutFlag(OPANGDOUBLEHIT);
		  }

	    }
	}
    }
    else
    {
	HPairSim* pPair = 0;
	while( (pPair = (HPairSim*) pIter->Next()) )
	{

	    if (pPair->getIsCutFlag()) continue; // already cut
	    else 
		  if(nPairOpeningAngleRecFlag)
		  {
		    if (compareParticleIndicesWithList(pPair,nOpang,nMaxNbGuiltyParticles)) pPair->setIsCutFlag(OPANG);
		  }
			
		  if(nDoubleDetectorHitRecFlag)
		  {
		    if (compareParticleIndicesWithList(pPair,nDouble,nMaxNbGuiltyParticles)) pPair->setIsCutFlag(DOUBLEHIT);
		  }
		  if(nPairOpeningAngleRecFlag && nDoubleDetectorHitRecFlag)
		  {
            if(compareParticleIndicesWithList(pPair,nOpang,nMaxNbGuiltyParticles) && compareParticleIndicesWithList(pPair,nDouble,nMaxNbGuiltyParticles))
			pPair->setIsCutFlag(OPANGDOUBLEHIT);
		  }

	}
    }

    // delete array of bad pairs, do not delete objects, as they belong to
    // category !
    delete pArrCutPairs;

    // create new array for next event
    pArrCutPairs = new TObjArray(5);
    
    // delete array for checking indices of singles
    delete [] nOpang;
    delete [] nDouble;


    return kTRUE;

}

//---------------------------------------------------------------------------- 

Bool_t HPairCutPar::checkRecursive(TObjArray* pArrAllPairs)
{
    // This function is executed at the end of the execute function of the 
    // filter. The array pArrCutPairs is filled with bad pairs
    // The array nParts is filled here with those
    // particle indices from bad pairs for this event

    if (pArrCutPairs==0)
    {
	::Error("checkRecursive","bad pair array is 0");
	return kFALSE;
    }

    if (pArrAllPairs==0)
    {
	::Error("checkRecursive","all pair arr is 0");
	return kFALSE;
    }

    Int_t nMaxNbGuiltyParticles = 2*(pArrCutPairs->GetLast()+1);
    Int_t *nOpang = new Int_t[nMaxNbGuiltyParticles];
    Int_t *nDouble = new Int_t[nMaxNbGuiltyParticles];

    for (Int_t j=0;j<nMaxNbGuiltyParticles;j++) nOpang[j]=-2;
    for (Int_t j=0;j<nMaxNbGuiltyParticles;j++) nDouble[j]=-2;

    for (Int_t i=0; i<pArrCutPairs->GetLast()+1; i++)
    {
	HPair* p = (HPair*)(*pArrCutPairs)[i];

    
	if(nPairOpeningAngleFlag)
	{
	  
      Int_t nBitField =  p->getBitFieldPassedCuts();	
	  if(HPairFL::getBit(OPANG,nBitField) == 0)
	  {
	    HPairFL::isNewIndex(p->getIndexParticle1(),nOpang,nMaxNbGuiltyParticles);
	    HPairFL::isNewIndex(p->getIndexParticle2(),nOpang,nMaxNbGuiltyParticles);
	  }
	}

	if(nDoubleDetectorHitFlag)
	{
      Int_t nBitField =  p->getBitFieldPassedCuts();	
	  if(HPairFL::getBit(DOUBLEHIT,nBitField) == 0)
	  {
	    HPairFL::isNewIndex(p->getIndexParticle1(),nDouble,nMaxNbGuiltyParticles);
	    HPairFL::isNewIndex(p->getIndexParticle2(),nDouble,nMaxNbGuiltyParticles);
	  }
	}

    }

    // loop over uncut pairs in event and check for a particle from the list 

    if (gHades->isReal())
    {
	for (Int_t nPairCnt=0; nPairCnt<pArrAllPairs->GetLast()+1;nPairCnt++)
	{
	    HPair* pPair = ((HPair*)(*pArrAllPairs)[nPairCnt]);
	    if (pPair->getIsCutFlag()) continue; // already cut
	    else 
	    { 
		  if(nPairOpeningAngleRecFlag)
		  {
		      cout<<"opang recursion"<<endl;
		    if (compareParticleIndicesWithList(pPair,nOpang,nMaxNbGuiltyParticles)) pPair->setIsCutFlag(OPANG);
		  }
			
		  if(nDoubleDetectorHitRecFlag)
		  {
		      cout<<"double hit recursion"<<endl;
		    if (compareParticleIndicesWithList(pPair,nDouble,nMaxNbGuiltyParticles)) pPair->setIsCutFlag(DOUBLEHIT);
		  }
		  if(nPairOpeningAngleRecFlag && nDoubleDetectorHitRecFlag)
		  {
            if(compareParticleIndicesWithList(pPair,nOpang,nMaxNbGuiltyParticles) && compareParticleIndicesWithList(pPair,nDouble,nMaxNbGuiltyParticles))
			pPair->setIsCutFlag(OPANGDOUBLEHIT);
		  }

	    }
	}
    }
    else
    {
	for (Int_t nPairCnt=0; nPairCnt<pArrAllPairs->GetLast()+1;nPairCnt++)
	{
	    HPairSim* pPair = ((HPairSim*)(*pArrAllPairs)[nPairCnt]);

	    if (pPair->getIsCutFlag()) continue; // already cut
	    else 
		  if(nPairOpeningAngleRecFlag)
		  {
		    if (compareParticleIndicesWithList(pPair,nOpang,nMaxNbGuiltyParticles)) pPair->setIsCutFlag(OPANG);
		  }
			
		  if(nDoubleDetectorHitRecFlag)
		  {
		    if (compareParticleIndicesWithList(pPair,nDouble,nMaxNbGuiltyParticles)) pPair->setIsCutFlag(DOUBLEHIT);
		  }
		  if(nPairOpeningAngleRecFlag && nDoubleDetectorHitRecFlag)
		  {
            if(compareParticleIndicesWithList(pPair,nOpang,nMaxNbGuiltyParticles) && compareParticleIndicesWithList(pPair,nDouble,nMaxNbGuiltyParticles))
			pPair->setIsCutFlag(OPANGDOUBLEHIT);
		  }

	}
    }

    // delete array of bad pairs, do not delete objects, as they belong to
    // category !
    delete pArrCutPairs;

    // create new array for next event
    pArrCutPairs = new TObjArray(5);
    
    // delete array for checking indices of singles
    delete [] nOpang;
    delete [] nDouble;


    return kTRUE;

}

//---------------------------------------------------------------------------- 

Bool_t HPairCutPar::compareParticleIndicesWithList(HPair* p,Int_t* list,Int_t size)
{
    Int_t idx1 = p->getIndexParticle1();
    Int_t idx2 = p->getIndexParticle2();

    for(Int_t j=0;j<size;j++)
	if (list[j] == idx1 || list[j] == idx2) return kTRUE;

    return kFALSE;

}

