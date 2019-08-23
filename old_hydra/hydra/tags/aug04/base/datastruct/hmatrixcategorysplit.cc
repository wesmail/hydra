#include "hmatrixcategorysplit.h"
#include "hclonestable.h"
#include "hades.h"
#include <TBranchClones.h>
#include <TBranch.h>
#include <TObject.h>
#include "htree.h"
#include <TROOT.h>
#include <TClass.h>
#include <string.h>
#include "hdebug.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hlineparser.h"

//*-- Author : Manuel Sanchez
//*-- Modified : 16/12/1999 by R. Holzmann
//*-- Modified : 9/11/1998 by Manuel Sanchez
//*-- Modified : 20/07/2001 by Dan Magestro (HDataObject --> TObject)

///////////////////////////////////////
//HMatrixCategorySplit
//
// This class extends HCategorySplit by allowing to have empty locations
//(holes).
///////////////////////////////////////

HMatrixCategorySplit::HMatrixCategorySplit(void) {
  //Constructor
  fHeader=NULL;
  strcpy(fClassName,"");
  strcpy(fBranchPattern,"");
  fTableArray=NULL;
}

HMatrixCategorySplit::HMatrixCategorySplit(Text_t *className,Int_t n,
					   Int_t *siz,Text_t *branchPattern,
					   TObject *header) {
  // Sets the category up for objects of the class className. The category
  //will hold a matrix of n-dimensional matrix for the modules; the maximun 
  //range for each index in the matrix is set in siz. In the output tree the
  //branches are created with names according to the pattern branchPattern:
  //For example if n=2 siz={2,2} and branchPattern="S%0%.M%1%" the branches
  //will be:
  //   S1.M1
  //   S1.M2
  //   S2.M1
  //   S2.M2
  //
  //  fHeader is a pointer to a category header.
  Int_t prod=1,i;
  fSizesArray.Set(n,siz);
  for (i=0;i<n;i++) prod*=fSizesArray[i];
  fTableArray=new TObjArray(prod);
  strcpy(fClassName,className);
  strcpy(fBranchPattern,branchPattern);
  fHeader=header;
}

HMatrixCategorySplit::~HMatrixCategorySplit(void) {
  //It's obviuos. Isn't it?
}

Bool_t HMatrixCategorySplit::setup(Int_t *moduleSizes,Float_t fillRate) {
  //Sets the category up, i.e. creates the internal structures to hold the data
  //objects.
  //
  //Parameters:
  //  moduleSizes-> array with the size (max number of cells) for each "module"
  //                a value of -1 means an empty module for which no output
  //                branch is created.
  //  fillRate-> expected multiplicity.
  Int_t prod=1,i;

  for (i=0;i<fSizesArray.fN;i++) prod*=fSizesArray.fArray[i];
  fTableArray->Delete();

  for (i=0;i<prod;i++) {
    if (moduleSizes[i]>-1) {
      fTableArray->AddAt(new HClonesTable(fClassName,
					  moduleSizes[i],
					  (Int_t)(moduleSizes[i]*fillRate)),i);
    }
  }
  return kTRUE;
}

Bool_t HMatrixCategorySplit::setup(Int_t nActive,Int_t *activeModules,
				   Int_t size,Float_t fillRate) {
  //Sets the category up, i.e. creates the internal structures to hold the data
  //objects.
  //
  //Parameters:
  //  nActive-> number of active "modules"
  //  activeModules-> array of nActive integers corresponding to the numbers of
  //                  the active modules.
  //  size-> size (max number of cells) for each module.
  //  fillRate-> expected multiplicity.  
  Int_t i;
  fTableArray->Delete();
  for (i=0;i<nActive;i++) {
    fTableArray->AddAt(new HClonesTable(fClassName,size,
					(Int_t)(size*fillRate)),
		       activeModules[i]);
  }
  return kTRUE;
}

TObject *&HMatrixCategorySplit::getNewSlot(HLocation &aLoc,Int_t *pIndex) {
  //see HCategory
  Int_t la,i,j;
  HClonesTable *table;
  la=aLoc[0];
  for (i=1;i<aLoc.getNIndex()-1;i++) la=la*fSizesArray[i]+aLoc[i];
  if ( (table=(HClonesTable *)fTableArray->At(la))!=NULL) {
    for (j=0;j<table->getCapacity();j++) {
      if (table->At(j)==NULL) {
        if(pIndex) *pIndex = la*fSizesArray[i]+j;
	return table->operator[](j);
      }
    }
  }

  return gNullObject;
}

TObject *&HMatrixCategorySplit::getSlot(HLocation &aLoc,Int_t *pIndex) {
  //see HCategory

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMatrixCategorySplit::getSlot");
#endif
  Int_t la,i;
  HClonesTable *table;
  la=aLoc[0];
  for (i=1;i<aLoc.getNIndex()-1;i++) la=la*fSizesArray[i]+aLoc[i];
  if ( (table=(HClonesTable *)fTableArray->At(la))!=NULL) {
    if(pIndex) *pIndex = la*fSizesArray[i]+aLoc.getOffset();
    return table->operator[](aLoc.getOffset());
  }
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMatrixCategorySplit::getSlot");
#endif
  return gNullObject;
}

TObject *HMatrixCategorySplit::getObject(HLocation &aLoc) {
  //See HCategory
  Int_t la,i;
  HClonesTable *table;
  TObject *r=NULL;

  //aLoc.Dump();
  la=aLoc[0];
  for (i=1;i<aLoc.getNIndex()-1;i++) la=la*fSizesArray[i]
				       +aLoc.getUncheckedIndex(i);
  if ( (table=(HClonesTable *)fTableArray->UncheckedAt(la))!=NULL) {
    //printf("module %i exists ",la);
    r=(TObject *)table->At(aLoc.getOffset());
  } //else printf("module %i doesn't exist ",la);
  
  //if (r) printf("and data exists");
  //else printf("and data doesn't exist");
  return r;
}

ClassImp(HMatrixCategorySplit)





