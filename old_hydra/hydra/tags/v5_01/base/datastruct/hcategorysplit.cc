#include "hcategorysplit.h"
#include <TClonesArray.h>
#include "hades.h"
#include <TBranchClones.h>
#include <TBranch.h>
#include "htree.h"
#include <TROOT.h>
#include <TClass.h>
#include <string.h>
#include "hdebug.h"
#include "hiterator.h"
#include "hlineparser.h"
#include "hdataobject.h"
#include "hcategorysplititer.h"

// Last modified 16/12/99 R.Holzmann
// Modified for clearing : 05/12/98 D.Bertini

/////////////////////////////////////////////////
//HCategorySplit
//
//  The HCategorySplit is a kind of HCategory. What makes this category
//special is the fact that the data stored in the category can be written to
//disk classified in several branches.
//
//  In split level 2 one branch is created for each "module". With "module" we
//mean each of the smallest set of data objects (cells) for the category. This
//is maybe more clear with some examples:
//
//  For the MDC, one "module" is in fact the layer.
//  For the RICH, one "module" corresponds to the Rich concept of module.
//
//  Note also that this category doesn't allow to have holes; that is, empty
//locations, if you need them use HMatrixCategorySplit instead.
/////////////////////////////////////////////////// 

HCategorySplit::HCategorySplit(void) {
  //Default constructor
  fHeader=NULL;
  strcpy(fClassName,"");
  strcpy(fBranchPattern,"");
  fTableArray=NULL;
}

HCategorySplit::HCategorySplit(Text_t *className,Int_t n,
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

HCategorySplit::~HCategorySplit(void) {
  //Destructor
  if (fTableArray) {fTableArray->Delete(); delete fTableArray;}
  if (fHeader) delete fHeader;
}

Text_t *HCategorySplit::getClassName(void) {
  //Returns the class' name for the data objects in the category
  return fClassName;
}

TClass *HCategorySplit::getClass(void) {
  //Returns a pointer to the data object's TClass.
  return gROOT->GetClass(fClassName);
}

void HCategorySplit::makeBranch(TBranch *parent) {
  //Create branches for the output
  HLineParser parser;
  TArrayI vals(fSizesArray.fN);
  Text_t name[255],buffer[255];
  TObjArray &vector=*fTableArray;
  Int_t i,j,la;
  TClonesArray **table;
  TBranchClones *br=NULL;

  parser.parse(fBranchPattern);
  

  for (i=0;i<vector.GetLast()+1;i++) {
    if ( (table=(TClonesArray **)&vector[i])!=NULL) {
      //Calculate vals
      la=i;
      for (j=fSizesArray.fN-1;j>-1;j--) {
	vals[j]=la%fSizesArray[j]+1;
	la=la/fSizesArray[j];
      }

      //Compose name
      parser.eval(&vals,buffer);
      sprintf(name,"%s.%s",fClassName,buffer);

      //Create branch
      br=new TBranchClones(name,table,2000,1);
      br->SetAddress(table);
      parent->GetListOfBranches()->Add(br);
    }			       
  }
  if (fHeader && gHades->getTree()) {
    TBranch *b=NULL;
    sprintf(buffer,"%s.%s",fClassName,"Header");
    b=gHades->getTree()->makeBranch(buffer,
				  fHeader->IsA()->GetName(),&fHeader,2000,1);
    if (b) parent->GetListOfBranches()->Add(b);
  }
}


void HCategorySplit::activateBranch(TTree *tree,Int_t splitLevel) {
  //see HCategory
  HLineParser parser;
  TArrayI vals(fSizesArray.fN);
  Text_t name[255],buffer[255];
  TObjArray &vector=*fTableArray;
  Int_t i,j,la;
  TClonesArray **table;
  TIterator *subBranches = NULL;
  TBranch *b=0;

  if (splitLevel>0) {
    parser.parse(fBranchPattern);
  

    for (i=0;i<vector.GetLast()+1;i++) {
      if ( (table=(TClonesArray **)&vector[i])!=NULL) {
	//Calculate vals
	la=i;
	for (j=fSizesArray.fN-1;j>-1;j--) {
	  vals[j]=la%fSizesArray[j]+1;
	  la=la/fSizesArray[j];
	}
	
	//Compose name
	parser.eval(&vals,buffer);
	sprintf(name,"%s.%s",fClassName,buffer);
	
	//Activate branch
	tree->SetBranchStatus(name,1);
	tree->SetBranchAddress(name,table);
	subBranches = tree->GetBranch(name)->GetListOfBranches()
	  ->MakeIterator();
	while ( (b = (TBranch*)subBranches->Next()) != NULL) {
	  tree->SetBranchStatus(b->GetName(),1);
	}
      }			       
    }
    if (fHeader && gHades->getTree()) {
      sprintf(name,"%s.%s",fClassName,"Header");
      tree->SetBranchStatus(name,1);
      tree->SetBranchAddress(name,&fHeader);
    }
  }
}

Bool_t HCategorySplit::filter(HFilter &) {
  //see HCategory
  Warning("HCategorySplit::filter","not yet implemented");
  return kFALSE;
}

Bool_t HCategorySplit::filter(HLocation &,HFilter &) {
  //see HCategory
  Warning("HCategorySplit::filter","not yet implemented");
  return kFALSE;
}

void HCategorySplit::Clear(void) {
  // explicit clear for objects 
  // in the structure. 
  // this is mandatory to allow multiple hits.
 //see HCategory
  TIter next(fTableArray);
  /*TClonesArray *aclone=NULL;
   HDataObject* obj=NULL;                
 
  for(Int_t i=0;i<fTableArray->GetEntries();i++){  
    if((aclone = (TClonesArray*) fTableArray->At(i)) != NULL){
         for (Int_t j=0;j<aclone->GetEntries();j++){
               obj = (HDataObject*) aclone-> At ( j ); 
               obj->clear(); // clear multiplicity 
	 }
     }
  }*/
  
  TObject *table;
  while ( (table=next())!=NULL) {
    table->Clear();
  }
}

Bool_t HCategorySplit::isSelfSplitable(void) {
  //see HCategory
  return kTRUE;
}

TIterator *HCategorySplit::MakeIterator(Option_t* opt,Bool_t dir) {
  //see HCategory
  HCategorySplitIter *it=NULL;
  it=new HCategorySplitIter(this);
  return it;
}

Bool_t HCategorySplit::setup(Int_t *moduleSizes,Float_t fillRate) {
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
      fTableArray->AddAt(new TClonesArray(fClassName,
					  (Int_t)(moduleSizes[i]*fillRate)),i);
    }
  }
  return kTRUE;
}

Bool_t HCategorySplit::setup(Int_t nActive,Int_t *activeModules,
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
    fTableArray->AddAt(new TClonesArray(fClassName,
					(Int_t)(size*fillRate)),
		       activeModules[i]);
  }
  return kTRUE;
}

TObject *&HCategorySplit::getNewSlot(HLocation &aLoc,Int_t *pIndex) {
  //see HCategory
  Int_t la,i;
  TClonesArray *table;
  la=aLoc[0];
  for (i=1;i<aLoc.getNIndex()-1;i++) la=la*fSizesArray[i]+aLoc[i];
  if ( (table=(TClonesArray *)fTableArray->At(la))!=NULL) {
     if(pIndex) *pIndex = la*fSizesArray[i]+table->GetLast()+1;
     return table->operator[](table->GetLast()+1);
  }

  return gNullObject;
}

TObject *&HCategorySplit::getSlot(HLocation &aLoc,Int_t *pIndex) {
  //see HCategory
  Int_t la,i;
  TClonesArray *table;
  la=aLoc[0];
  for (i=1;i<aLoc.getNIndex()-1;i++) la=la*fSizesArray[i]+aLoc[i];
  if ( (table=(TClonesArray *)fTableArray->At(la))!=NULL) {
     if(pIndex) *pIndex = la*fSizesArray[i]+aLoc.getOffset();
     return table->operator[](aLoc.getOffset());
  }
  //gDebuger->leaveFunc("HMatrixCategorySplit::getSlot");
  return gNullObject;
}

HDataObject *HCategorySplit::getObject(HLocation &aLoc) {
  //see HCategory
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMatrixCategorySplit::getObject");
#endif
  Int_t la,i;
  TClonesArray *table;
  HDataObject *r=NULL;

  la=aLoc[0];
  for (i=1;i<aLoc.getNIndex()-1;i++) la=la*fSizesArray[i]
				       +aLoc.getUncheckedIndex(i);
  if ( (table=(TClonesArray *)fTableArray->UncheckedAt(la))!=NULL) {
    r=(HDataObject *)table->At(aLoc.getOffset());
  } else Error("getObject","module %i doesn't exist ",la);
  
  return r;
}



ClassImp(HCategorySplit)




