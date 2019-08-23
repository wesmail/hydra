#pragma implementation

#include "hhyplist.h"
#include <iostream>

using namespace std;

ClassImp(HHypList)

//////////////////////////////////////////////////////////////////////
//HHypList
//
// Structure for Hit Level
// One object describes one hit in detector
//
////////// **********ShowerHit defintion ****************/////////////77


HHypList::HHypList(Int_t icomb, Int_t ipart)
{
    clear();
    ncomb = icomb;
    npart = ipart;
}
	
Bool_t HHypList::clear()
{ 
   ncomb = 0;
   npart = 0;

 return kTRUE;
}

Bool_t HHypList::setList(Int_t l) {
  list = l; return kTRUE;
}

Bool_t HHypList::createTables()
{
	if( (ncomb != 0) && ( npart != 0) )
	{
	    idx.Set(ncomb*npart);
		
		fprobcomb.Set(ncomb);
		
		fprobalg.Set(ncomb);
		
		return kTRUE;
	}
	else return kFALSE;
}

Bool_t HHypList::setIdx(Int_t icomb, Int_t ipart, Int_t iIdx)
{
    idx[ icomb*npart + ipart ] = iIdx; 
	return kTRUE; 
}

Int_t  HHypList::getIdx(Int_t icomb, Int_t ipart)
{
    return idx[ icomb*npart + ipart];
}

Bool_t  HHypList::print()
{ 
	cout<<"\n\n";
	for( Int_t i=0; i<ncomb; i++ )
	{
	    for( Int_t j=0; j<npart; j++ )
		{
            cout<<"\t"<<getIdx( i, j);
		}
		cout<<"\t"<<getProbComb(i)<<"\t"<<getProbAlg(i)<<endl;
	}
	cout<<"\n";
	return kTRUE;
}

HCategory * HHypList::buildLinearCat(Text_t *classname)
{
  HLinearCategory *category=NULL;
  Int_t size = 1000;

  category = new HLinearCategory(classname, size);
  category->setDynamicObjects(kTRUE);

  return category;
} 

// -----------------------------------------------------------------------------

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
      R__b >> ncomb;
      R__b >> npart;
	  for(Int_t i=0; i<getNcomb(); i++)
	  {
          R__b >> fprobcomb[i];
          R__b >> fprobalg[i];
	  }
	  for(Int_t i=0; i<idx.GetSize(); i++)
	  {
          R__b >> idx[i];
	  }
   } 
   else 
   {
      R__b.WriteVersion(HHypList::IsA());
      TObject::Streamer(R__b);
      R__b << ncomb;
      R__b << npart;
	  for(Int_t i=0; i<getNcomb(); i++)
	  {
          R__b << fprobcomb[i];
          R__b << fprobalg[i];
	  }
	  for(Int_t i=0; i<idx.GetSize(); i++)
	  {
          R__b << idx[i];
	  }
   }
}

