////////////////////////////////////////////////////////// hhypkine.cc
//
//     HHypKine - container class for HADES EVENT HYPOTHESIS
//
//     09.02.2005                          Marcin Wisniowski
//////////////////////////////////////////////////////////////////////

#pragma implementation
#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include "hratreeext.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"

#include "hypinfodef.h"

#include "hhypkine.h"
#include "hhypcomb.h"
#include <iostream>

using namespace std;

ClassImp(HHypKine)

//-------------------------- constructors ------------------------------

HHypKine::HHypKine(Int_t Ncomb, Int_t Npart)
{
    clear();
    ncomb = Ncomb;
    npart = Npart;
    prepareHypKine();
}

HHypKine::HHypKine( Int_t Idx_HypComb )
{
    HHypComb *p_hypcomb = NULL;

	HCategory *combCat=gHades->getCurrentEvent()->getCategory(catHypComb);
	p_hypcomb = (HHypComb*)combCat->getObject(Idx_HypComb);

	if( p_hypcomb == NULL )
	{
	    cout<<"\n HHypKine::HHypKine ERROR (pointer to HHypComb not set ) \n";
	}
	else
	{
	    setNcomb( p_hypcomb->getNcomb() );
	    setNpart( p_hypcomb->getNpart() );
		prepareHypKine();

	    for(Int_t icomb=0; icomb<(p_hypcomb->getNcomb()); icomb++)
		{
		    setProbComb( icomb, p_hypcomb->getProbComb(icomb) );
	        for(Int_t ipart=0; ipart<(p_hypcomb->getNpart()); ipart++)
			{
                setPid(icomb, ipart, p_hypcomb->getPid(icomb, ipart) );
			} // copy (TArrayI)pid from HHypComb
		}     // copy (TArrayF)fprobcomb from HHypComb
	}
}												 

//---------------------------------------------------------------------




//---------------------------- functions ------------------------------
Bool_t HHypKine::clear()
{ 
    ncomb = 0;
    npart = 0;
    return kTRUE;
}

Bool_t HHypKine::setCombId(Int_t Cid)
{
    cid = Cid;
    return kTRUE;
}

Int_t  HHypKine::getCombId()
{
    return cid;
}

Bool_t HHypKine::setNcomb(Int_t Ncomb)
{
    ncomb = Ncomb;
	return kTRUE;
}

Int_t  HHypKine::getNcomb()            
{
    return ncomb;
}
		
Bool_t HHypKine::setNpart(Int_t Npart)
{
    npart = Npart; 
	return kTRUE;
}

Int_t  HHypKine::getNpart()  
{
    return npart;
}
		 
Bool_t  HHypKine::setProbComb(Int_t Icomb, Float_t Prob)
{
    fprobcomb[Icomb] = Prob;
	return kTRUE;
}

Float_t HHypKine::getProbComb(Int_t Icomb)
{
    return fprobcomb[Icomb];
}

Bool_t HHypKine::prepareHypKine()
{
	if( (ncomb != 0) && ( npart != 0) )
	{
	    pid.Set(ncomb*npart);
	    px.Set(ncomb*npart);
	    py.Set(ncomb*npart);
	    pz.Set(ncomb*npart);
		fprobcomb.Set(ncomb);
		return kTRUE;
	}
	else return kFALSE;
}

Bool_t HHypKine::setTLorentzVector(Int_t Icomb, Int_t Ipart, TLorentzVector Vect)
{
    px[Icomb*npart+Ipart] = Vect.Px();
    py[Icomb*npart+Ipart] = Vect.Py();
    pz[Icomb*npart+Ipart] = Vect.Pz();
	return kTRUE;
}
	   
TLorentzVector HHypKine::getTLorentzVector(Int_t Icomb, Int_t Ipart)
{
    Double_t Mass = HPhysicsConstants::mass( getPid(Icomb, Ipart) );
    TLorentzVector Vect;
	TVector3 Vect3;
	Vect3.SetXYZ(px[Icomb*npart+Ipart], py[Icomb*npart+Ipart],
	                pz[Icomb*npart+Ipart]);
	Vect.SetVectM( Vect3, Mass);

	return Vect;		
}

Bool_t HHypKine::setPid(Int_t Icomb, Int_t Ipart, Int_t Pid)
{
    pid[ Icomb*npart + Ipart ] = Pid; 
	return kTRUE; 
}

Int_t  HHypKine::getPid(Int_t Icomb, Int_t Ipart)
{
    return pid[ Icomb*npart + Ipart];
}

Bool_t  HHypKine::print()
{ 
	cout<<"\n";
	for( Int_t i=0; i<ncomb; i++ )
	{
	    for( Int_t j=0; j<npart; j++ )
		{
            //cout<<"\t"<<getIdxPidPart( i, j);
		}
		cout<<"\t"<<getProbComb(i)<<endl;
	}
	cout<<"\n";
	return kTRUE;
}

HCategory * HHypKine::buildLinearCat(Text_t *classname)
{
  HLinearCategory *category=NULL;
  Int_t size = 1000;

  category = new HLinearCategory(classname, size);
  category->setDynamicObjects(kTRUE);

  return category;
} 

// -----------------------------------------------------------------------------

void HHypKine::Streamer(TBuffer &R__b)
{
   // Stream an object of class HHypKine.

   if (R__b.IsReading()) 
   {
      Version_t R__v = R__b.ReadVersion(); 
      if (R__v == 1) 
      {
      Version_t R__v;
            R__v = R__b.ReadVersion();	// simulate HDataObject v 1 readout
      }
      TObject::Streamer(R__b);
      R__b >> ncomb;
      R__b >> npart;
      R__b >> cid;
	  for(Int_t i=0; i<getNcomb(); i++)
	  {
          R__b >> fprobcomb[i];
	  }
	  for(Int_t i=0; i<pid.GetSize(); i++)
	  {
          R__b >> pid[i];
          R__b >> px[i];
          R__b >> py[i];
          R__b >> pz[i];
	  }
   } 
   else 
   {
      R__b.WriteVersion(HHypKine::IsA());
      TObject::Streamer(R__b);
      R__b << ncomb;
      R__b << npart;
      R__b << cid;
	  for(Int_t i=0; i<getNcomb(); i++)
	  {
          R__b << fprobcomb[i];
	  }
	  for(Int_t i=0; i<pid.GetSize(); i++)
	  {
          R__b << pid[i];
          R__b << px[i];
          R__b << py[i];
          R__b << pz[i];
	  }
   }
}
//---------------------------------------------------------------------
