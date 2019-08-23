#ifndef HHypListCat_H
#define HHypListCat_H

#include <TROOT.h>
#include <TArrayF.h>
#include <TArrayI.h>
#include <TObject.h>
#include <hcategory.h>
#include <hlinearcategory.h>

class HHypList : public TObject{
public:
        HHypList(Int_t comb, Int_t npart);
        HHypList(){ clear(); }
       ~HHypList() {}

        Bool_t clear();
	Bool_t setList(Int_t);
		
        Bool_t setNcomb(Int_t Ncomb) { ncomb = Ncomb; return kTRUE; }
        Int_t  getNcomb()            { return ncomb;  }
		
        Bool_t setNpart(Int_t Npart) { npart = Npart; return kTRUE; }
        Int_t  getNpart()            { return npart;  }

        Bool_t  setProbComb(Int_t Ncomb, Float_t prob) { fprobcomb[Ncomb] = prob; return kTRUE; }
        Float_t getProbComb(Int_t Ncomb)               { return fprobcomb[Ncomb]; }

        Bool_t  setProbAlg(Int_t Ncomb, Float_t prob) { fprobalg[Ncomb] = prob; return kTRUE; }
        Float_t getProbAlg(Int_t Ncomb)               { return fprobalg[Ncomb]; }

		Bool_t createTables();

		Bool_t setIdx(Int_t icomb, Int_t ipart, Int_t iIdx);
		Int_t  getIdx(Int_t icomb, Int_t ipart);

		Bool_t print();
		//void Streamer(TBuffer &R__b);

		static HCategory *buildLinearCat(Text_t *calssname) ;

private:
	   TArrayF fprobcomb;    // probTrack1*probTrack2*.... 
	   TArrayF fprobalg;     // AlgX(icomb) -> prob
	   TArrayI idx;     // size = npart*ncomb 
	   Int_t ncomb;     // 0,1,2, .... ,k
	   Int_t npart;     // 0,1,2, .... ,j

	   Int_t list;      // channel list ID (see hypchannel)

       ClassDef(HHypList,1) //ROOT extension
};

#endif
