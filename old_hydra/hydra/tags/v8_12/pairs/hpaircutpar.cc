// $Id: hpaircutpar.cc,v 1.11 2006-11-24 16:14:03 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-24 11:31:25
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
#include <TVector3.h>
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
      const UInt_t HPairCutPar::DIST = 4;
      const UInt_t HPairCutPar::VERTEX = 5;
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
}

//---------------------------------------------------------------------------- 

void HPairCutPar::clear() 
{
    nBitFieldFlags = 0;
    nBitFieldPassedCuts = 0;
    nBitFieldRecursiveFlags = 0;
    nPairOpeningAngleFlag = -1;
    nPairOpeningAngleRecFlag = -1;
    fPairOpeningAngle = -1000.;
    nDoubleDetectorHitFlag = -1;
    nDoubleDetectorHitRecFlag = -1;
    nPairDistFlag = -1;
    fPairDist = -1000.;
    nPairVertexFlag = -1;
    fZRichExit = -1000.;
    fZRichBulb = -1000.;
    fZTargetShift = -1000.;
    fRFlanch = -1000.;
    fXBeamCenter = -1000.;
    fYBeamCenter = -1000.;
    fBeamRadius = -1000.;
    kIsInit = kFALSE;
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
    cout<<"PairDistFlag:\t"<<nPairDistFlag<<endl;
    cout<<"PairDist:\t"<<fPairDist<<endl;
    cout<<"PairVertexFlag:\t"<<nPairVertexFlag<<endl;
    cout<<"fZRichExit:\t"<<fZRichExit<<endl;
    cout<<"fZRichBulb:\t"<<fZRichBulb<<endl;
    cout<<"fZTargetShift:\t"<<fZTargetShift<<endl;
    cout<<"fRFlanch:\t"<<fRFlanch<<endl;
    cout<<"fXBeamCenter:\t"<<fXBeamCenter<<endl;
    cout<<"fYBeamCenter:\t"<<fYBeamCenter<<endl;
    cout<<"fBeamRadius:\t"<<fBeamRadius<<endl;
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
  l->add("PairDist",fPairDist);
  l->add("PairDistFlag",nPairDistFlag);
  l->add("ZRichExit",fZRichExit);
  l->add("ZRichBulb",fZRichBulb);
  l->add("ZTargetShift",fZTargetShift);
  l->add("RFlanch",fRFlanch);
  l->add("XBeamCenter",fXBeamCenter);
  l->add("YBeamCenter",fYBeamCenter);
  l->add("BeamRadius",fBeamRadius);
  l->add("PairVertexFlag",nPairVertexFlag);
}

//---------------------------------------------------------------------------- 

Bool_t HPairCutPar::getParams(HParamList* l) 
{
  if (!l) return kFALSE;

  // ----------------- OPANG
  if (!l->fill("PairOpeningAngleFlag",&nPairOpeningAngleFlag)) return kFALSE;
  if (!l->fill("PairOpeningAngleRecFlag",&nPairOpeningAngleRecFlag)) return kFALSE;
  if (nPairOpeningAngleFlag || nPairOpeningAngleRecFlag) 
  {
	if (!l->fill("PairOpeningAngle",&fPairOpeningAngle)) return kFALSE;
  }
  // ----------------- DOUBLEHIT
  if (!l->fill("DoubleDetectorHitFlag",&nDoubleDetectorHitFlag)) return kFALSE;
  if (!l->fill("DoubleDetectorHitRecFlag",&nDoubleDetectorHitRecFlag)) return kFALSE;
  // ----------------- DIST
  if (!l->fill("PairDistFlag",&nPairDistFlag)) return kFALSE;
  if (nPairDistFlag) 
  {
      if (!l->fill("PairDist",&fPairDist)) return kFALSE;
  }
  // ----------------- VERTEX FLANCH
  if (!l->fill("PairVertexFlag",&nPairVertexFlag)) return kFALSE;
  if (nPairVertexFlag) 
  {
      if (!l->fill("ZRichExit",&fZRichExit)) return kFALSE;
      if (!l->fill("ZRichBulb",&fZRichBulb)) return kFALSE;
      if (!l->fill("ZTargetShift",&fZTargetShift)) return kFALSE;
      if (!l->fill("RFlanch",&fRFlanch)) return kFALSE;
      if (!l->fill("XBeamCenter",&fXBeamCenter)) return kFALSE;
      if (!l->fill("YBeamCenter",&fYBeamCenter)) return kFALSE;
      if (!l->fill("BeamRadius",&fBeamRadius)) return kFALSE;
  }


  finalizeInit();

  return kTRUE;
}



//---------------------------------------------------------------------------- 

void HPairCutPar::finalizeInit(void)
{

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
  

  
  if (nPairDistFlag) HPairFL::setBit((UInt_t)DIST,nBitFieldFlags);
  else HPairFL::clearBit((UInt_t)DIST,nBitFieldFlags);



  if (nPairVertexFlag) HPairFL::setBit((UInt_t)VERTEX,nBitFieldFlags);
  else HPairFL::clearBit((UInt_t)VERTEX,nBitFieldFlags);


  // set active cuts string
  if (nPairOpeningAngleFlag) pActiveCutNames->Append(":isGoodOpang");
  if (nDoubleDetectorHitFlag) pActiveCutNames->Append(":isNotDoubleHit");
  if (nPairDistFlag) pActiveCutNames->Append(":isGoodDist");
  if (nPairVertexFlag) pActiveCutNames->Append(":isFreePairVertex");
  // ----------------- XXXX

  kIsInit = kTRUE;

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
    
    if (nPairDistFlag)
    {
	if (checkPairDist(p)) 
	{   // pair has passed
	    HPairFL::setBit(DIST,nBitFieldPassedCuts);
	}
	else 
	{
	    HPairFL::clearBit(DIST,nBitFieldPassedCuts);
	    
	}
    }
    
    if (nPairVertexFlag)
    {
	if (checkPairVertex(p)) 
	{   // pair has passed
	    HPairFL::setBit(VERTEX,nBitFieldPassedCuts);
	}
	else 
	{
	    HPairFL::clearBit(VERTEX,nBitFieldPassedCuts);
	
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

Bool_t HPairCutPar::checkPairDist(HPair* p)
{
    HPairData pd(p);
    if (pd.pDist() < fPairDist) return kTRUE;
    return kFALSE;
}

//---------------------------------------------------------------------------- 

Bool_t HPairCutPar::checkPairVertex(HPair* p)
{
    HPairData pd(p);
    Float_t z_f = fZRichBulb - (fZRichExit - fZTargetShift);
    Float_t r_f = fRFlanch;
    TVector3 beam_center(fXBeamCenter, fYBeamCenter, 0);
    Float_t z_v = pd.getVertZ();
    Float_t r_v = pd.getVert().Perp();
    Float_t phi_v = pd.getVert().Phi();
    Float_t delta_phi, d;
    beam_center -= pd.getVert();
    
    // cut if far from the beam center
    if (beam_center.Perp() > fBeamRadius) return kFALSE;
    
    // first leg through flanch
    delta_phi = pd.getPhiDeg1() * TMath::DegToRad() - phi_v;
    d = TMath::Sqrt(TMath::Power(r_f, 2.0) - TMath::Power(r_v, 2.0)
		    * TMath::Power(TMath::Sin(delta_phi), 2.0))
	- TMath::Cos(delta_phi);
    if ((z_f - z_v) > 0 &&
	TMath::Tan(pd.getThetaDeg1() * TMath::DegToRad()) > (d / (z_f - z_v)))
	return kFALSE;
    
    // second leg through flanch
    delta_phi = pd.getPhiDeg2() * TMath::DegToRad() - phi_v;
    d = TMath::Sqrt(TMath::Power(r_f, 2.0) - TMath::Power(r_v, 2.0)
		    * TMath::Power(TMath::Sin(delta_phi), 2.0))
	- TMath::Cos(delta_phi);
    if ((z_f - z_v) > 0 &&
	TMath::Tan(pd.getThetaDeg2() * TMath::DegToRad()) > (d / (z_f - z_v)))
	return kFALSE;
    
    return kTRUE;
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
		HPairFL::isNewIndex(p->getIndexParticle1(),nOpang,
				    nMaxNbGuiltyParticles);
		HPairFL::isNewIndex(p->getIndexParticle2(),nOpang,
				    nMaxNbGuiltyParticles);
	    }
	}
	
	if(nDoubleDetectorHitFlag)
	{
	    Int_t nBitField =  p->getBitFieldPassedCuts();	
	    if(HPairFL::getBit(DOUBLEHIT,nBitField) == 0)
	    {
		HPairFL::isNewIndex(p->getIndexParticle1(),nDouble,
				    nMaxNbGuiltyParticles);
		HPairFL::isNewIndex(p->getIndexParticle2(),nDouble,
				    nMaxNbGuiltyParticles);
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
		    if (compareParticleIndicesWithList(pPair,nOpang,
						       nMaxNbGuiltyParticles)) 
			pPair->setIsCutFlag(OPANG);
		}
			
		if(nDoubleDetectorHitRecFlag)
		{
		    if (compareParticleIndicesWithList(pPair,nDouble,
						       nMaxNbGuiltyParticles)) 
			pPair->setIsCutFlag(DOUBLEHIT);
		}

		if(nPairOpeningAngleRecFlag && nDoubleDetectorHitRecFlag)
		{
		    if(compareParticleIndicesWithList(pPair,nOpang,
						      nMaxNbGuiltyParticles) && 
		       compareParticleIndicesWithList(pPair,nDouble,
						      nMaxNbGuiltyParticles))
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
		    if (compareParticleIndicesWithList(pPair,nOpang,
						       nMaxNbGuiltyParticles)) 
			pPair->setIsCutFlag(OPANG);
		}
			
	    if(nDoubleDetectorHitRecFlag)
	    {
		if (compareParticleIndicesWithList(pPair,nDouble,
						   nMaxNbGuiltyParticles)) 
		    pPair->setIsCutFlag(DOUBLEHIT);
	    }
	    if(nPairOpeningAngleRecFlag && nDoubleDetectorHitRecFlag)
	    {
		if(compareParticleIndicesWithList(pPair,nOpang,
						  nMaxNbGuiltyParticles) && 
		   compareParticleIndicesWithList(pPair,nDouble,
						  nMaxNbGuiltyParticles))
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
		HPairFL::isNewIndex(p->getIndexParticle1(),nOpang,
				    nMaxNbGuiltyParticles);
		HPairFL::isNewIndex(p->getIndexParticle2(),nOpang,
				    nMaxNbGuiltyParticles);
	    }
	}

	if(nDoubleDetectorHitFlag)
	{
	    Int_t nBitField =  p->getBitFieldPassedCuts();	
	    if(HPairFL::getBit(DOUBLEHIT,nBitField) == 0)
	    {
		HPairFL::isNewIndex(p->getIndexParticle1(),nDouble,
				    nMaxNbGuiltyParticles);
		HPairFL::isNewIndex(p->getIndexParticle2(),nDouble,
				    nMaxNbGuiltyParticles);
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
		    if (compareParticleIndicesWithList(pPair,nOpang,
						       nMaxNbGuiltyParticles)) 
			pPair->setIsCutFlag(OPANG);
		}
			
		if(nDoubleDetectorHitRecFlag)
		{
		    if (compareParticleIndicesWithList(pPair,nDouble,
						       nMaxNbGuiltyParticles)) 
			pPair->setIsCutFlag(DOUBLEHIT);
		}

		if(nPairOpeningAngleRecFlag && nDoubleDetectorHitRecFlag)
		{
		    if(compareParticleIndicesWithList(pPair,nOpang,
						      nMaxNbGuiltyParticles) && 
		       compareParticleIndicesWithList(pPair,nDouble,
						      nMaxNbGuiltyParticles))
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
		    if (compareParticleIndicesWithList(pPair,nOpang,
						       nMaxNbGuiltyParticles)) 
			pPair->setIsCutFlag(OPANG);
		}
			
	    if(nDoubleDetectorHitRecFlag)
	    {
		if (compareParticleIndicesWithList(pPair,nDouble,
						   nMaxNbGuiltyParticles)) 
		    pPair->setIsCutFlag(DOUBLEHIT);
	    }
	    if(nPairOpeningAngleRecFlag && nDoubleDetectorHitRecFlag)
	    {
		if(compareParticleIndicesWithList(pPair,nOpang,
						  nMaxNbGuiltyParticles) && 
		   compareParticleIndicesWithList(pPair,nDouble,
						  nMaxNbGuiltyParticles))
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

