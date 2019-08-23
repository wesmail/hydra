////////////////////////////////////////////////////////// hhyplist.cc
//                                                        
//     HHypList - container class for HADES EVENT HYPOTHESIS
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

#include "hhyplist.h"
#include "hhypcomb.h"
#include <iostream>

using namespace std;

ClassImp(HHypList)


//--------------------------- constructors -------------------------------

HHypList::HHypList( HHypList *OldList, Int_t IdxHypKine )
{
    p_hypcomb = NULL;
    setIdxHypComb( OldList->getIdxHypComb() );
    HCategory *combCat=gHades->getCurrentEvent()->getCategory(catHypComb);
    //    cout<<"\n\n idx_hypcomb = "<<idx_hypcomb<<"\n\n"; 
    p_hypcomb = (HHypComb*)combCat->getObject(idx_hypcomb);
  
    if( p_hypcomb == NULL ) 
    {
        cout<<"\n HHypList::HHypList ERROR (pointer to HHypComb not set 1) \n";
    }
    else
    {
        prepareHypList();
    }
  
    if( IdxHypKine == -1 )  setIdxHypKine( OldList->getIdxHypKine() );
    else setIdxHypKine(IdxHypKine);
  
    for(Int_t Icomb=0; Icomb<(OldList->getNcomb()); Icomb++) 
    {
        setProbAlg( Icomb, OldList->getProbAlg(Icomb) );
    }   // copy 'fprobalg' from 'OldList' to new HHypList object
}

HHypList::HHypList( HHypComb *HypComb, Int_t IdxHypKine )
{   
    p_hypcomb = HypComb;
    setIdxHypKine( IdxHypKine );     // don't use kine container
	
    if( p_hypcomb == NULL ) 
    {
        cout<<"\n HHypList::HHypList ERROR (pointer to HHypComb not set 2) \n";
    }
    else
    {
        prepareHypList();
        for(Int_t Icomb=0; Icomb<getNcomb(); Icomb++) 
        {
            setProbAlg( Icomb, getProbComb(Icomb) );
        }
    }
}

HHypList::HHypList( Int_t Idx_HypComb, Int_t IdxHypKine )
{
    p_hypcomb = NULL;
    setIdxHypKine( IdxHypKine );     // don't use kine container
	
    HCategory *combCat=gHades->getCurrentEvent()->getCategory(catHypComb);
    setIdxHypComb( Idx_HypComb );  

    p_hypcomb = (HHypComb*)combCat->getObject(idx_hypcomb);
	
    if( p_hypcomb == NULL ) 
    {
        cout<<"\n HHypList::HHypList ERROR (pointer to HHypComb not set 3) \n";
    }
    else
    {
        prepareHypList();
        for(Int_t Icomb=0; Icomb<getNcomb(); Icomb++) 
        {
      	    setProbAlg( Icomb, getProbComb(Icomb) );
 	}
    }
}
//-------------------------------------------------------------------------



//------------------------------- functions -------------------------------

Bool_t HHypList::setListId( Int_t Id_list )
{
    id_list = Id_list;
    return kTRUE;
}

Int_t  HHypList::getListId()
{
    return id_list;
}

Bool_t HHypList::prepareHypList()
{
    Int_t Ncomb = getNcomb();
	
    if( Ncomb != 0 )
    {
 	fprobalg.Set(Ncomb);
	return kTRUE;
    }
    else return kFALSE;
}

Int_t  HHypList::getIdxPidPart(Int_t Icomb, Int_t Ipart)
{
    return p_hypcomb->getIdxPidPart(Icomb, Ipart);
}

Int_t  HHypList::getNcomb()           
{
    return p_hypcomb->getNcomb();
}

Int_t  HHypList::getNpart()            
{
    return p_hypcomb->getNpart();
}

Float_t HHypList::getProbComb(Int_t Icomb)            
{
    return p_hypcomb->getProbComb(Icomb);
}

TLorentzVector HHypList::getTLorentzVector(char * name, Int_t num)
{
    if ((comb_iter >= p_hypcomb->getNcomb()) || (comb_iter<0))
    {
        iterstatus=kFALSE;
	return TLorentzVector();
    }

    Int_t part_counter=-1;
    Int_t part_num=0;
    Int_t part_id = HPhysicsConstants::pid(name);

    //    cout << "looking for " << name <<  endl;

    while ((part_num != num) && (part_counter < (getNpart() - 1) ))
    {
        part_counter++;
	//        cout << getPid(comb_iter, part_counter) << endl;
        if (part_id == getPid(comb_iter, part_counter)) part_num++;
    }
 
    if (part_num == num) 
    {
      //       cout << "particle " << name << " is on position " <<  part_counter << endl;
       return getTLorentzVector(comb_iter, part_counter);
    }
    iterstatus=kFALSE;
    return TLorentzVector();
}

TLorentzVector HHypList::getTLorentzVector(Int_t Icomb, Int_t Ipart)
{
    TLorentzVector Vect;
  
    if( getIdxHypKine() == -1 )
    {
        HPidParticle *PidPart = NULL;
      
        HCategory *pidpartCat=gHades->getCurrentEvent()->getCategory( catPidPart );
        PidPart = (HPidParticle*)pidpartCat->getObject( getIdxPidPart(Icomb, Ipart) );
      
        if (PidPart == NULL) 
	{
	    status=kFALSE; 
	    return Vect;
	}
      
        Double_t Mass = HPhysicsConstants::mass( getPid(Icomb, Ipart) );
      
        TVector3 Vect3;
        Vect = (TLorentzVector)(*PidPart);
        Vect3 = Vect.Vect();
        Vect.SetVectM( Vect3, Mass );
    }
    else
    {
        HHypKine *HypKine = NULL;
      
        HCategory *hypkineCat=gHades->getCurrentEvent()->getCategory( catHypKine );
        HypKine = (HHypKine*)hypkineCat->getObject( getIdxHypKine() );
        Vect = HypKine->getTLorentzVector(Icomb, Ipart);
    }
    return Vect;
}

Bool_t  HHypList::setProbAlg(Int_t Icomb, Float_t Prob)
{ 
    fprobalg[Icomb] = Prob;
    return kTRUE;
}

Float_t HHypList::getProbAlg(Int_t Icomb)               
{
    return fprobalg[Icomb];
}

Int_t   HHypList::getPid(Int_t Icomb, Int_t Ipart)
{
    return p_hypcomb->getPid(Icomb, Ipart);
}

Bool_t HHypList::setIdxHypComb(Int_t Idx)
{
    idx_hypcomb = Idx; 
    return kTRUE;
}

Int_t  HHypList::getIdxHypComb()
{
    return idx_hypcomb;
}
		
Bool_t HHypList::setIdxHypKine(Int_t Idx)
{
    idx_hypkine = Idx; 
    return kTRUE;
}

Int_t  HHypList::getIdxHypKine()
{
    return idx_hypkine;
}

Bool_t HHypList::CombIteratorReset(void)
{
    comb_iter=-1;
    iterstatus=kTRUE;
    return kTRUE;
}

Bool_t HHypList::getIterStatus(void)
{
    return iterstatus;
}

Bool_t HHypList::CombIterator(void)
{
    comb_iter++;
 
    if (comb_iter >= p_hypcomb->getNcomb() ) return kFALSE;

    while ((comb_iter < p_hypcomb->getNcomb() ) && (getProbAlg(comb_iter ) <= 0))
    {
      //cout << comb_iter << " iteration has " << getProbAlg( comb_iter) << endl;
        comb_iter++;
    }
    //    cout << comb_iter << " iteration " << endl;


    if ((comb_iter < p_hypcomb->getNcomb()) && (getProbAlg( comb_iter ) > 0)) return kTRUE;
    else return kFALSE;
}

		
Bool_t  HHypList::print()
{ 
    cout<<"\n";  // Idx table
    for( Int_t i=0; i<getNcomb(); i++ )
    {
        for( Int_t j=0; j<(getNpart()); j++ )
 	{
            cout<<"\t"<<getIdxPidPart( i, j);
	}
	cout<<"\t"<<getProbComb(i)<<"\t"<<getProbAlg(i)<<endl;
    }
    cout<<"\n";   // Pid table
    for( Int_t i=0; i<getNcomb(); i++ )
    {
        for( Int_t j=0; j<(getNpart()); j++ )
 	{
            cout<<"\t"<<getPid( i, j);
	}
        cout<<"\n";
    }	
    cout<<"\n";
    return kTRUE;
}

HCategory *HHypList::buildLinearCat(Text_t *classname)
{
    HLinearCategory *category=NULL;
    Int_t size = 1000;

    category = new HLinearCategory(classname, size);
    category->setDynamicObjects(kTRUE);

    return category;
} 

//-------------------------------------------------------------------------

void HHypList::Streamer(TBuffer &R__b)
{
    // Stream an object of class HHypList.

    if (R__b.IsReading()) 
    {
        Version_t R__v = R__b.ReadVersion(); 
        if (R__v == 1) 
        {
            Version_t R__v;
            R__v = R__b.ReadVersion();	// simulate HDataObject v 1 readout
        }
        TObject::Streamer(R__b);
        R__b >> idx_hypcomb;
        R__b >> idx_hypkine;
        R__b >> id_list;
        for(Int_t i=0; i<getNcomb(); i++)
        {
            R__b >> fprobalg[i];
        }
    } 
    else 
    {
        R__b.WriteVersion(HHypList::IsA());
        TObject::Streamer(R__b);
        R__b << idx_hypcomb;
        R__b << idx_hypkine;
        R__b << id_list;
        for(Int_t i=0; i<getNcomb(); i++)
        {
            R__b << fprobalg[i];
        }
    }
}

//-------------------------------------------------------------------------
