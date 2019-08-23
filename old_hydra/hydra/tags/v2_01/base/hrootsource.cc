//*-- Author : Manuel Sanchez
//*-- Modified : 5/05/98 by Manuel Sanchez
//*-- Copyright : GENP (Univ. Santiago de Compostela)

//////////////////////////////////////////////////////
// HRootSource
//
//  The HRootSource class reads all or part of the data stored in a Root file
// using a TTree.
//
//////////////////////////////////////////////////////

#include "hrootsource.h"
#include "hades.h"
#include <iostream.h>

ClassImp(HRootSource)

  HRootSource::HRootSource(void) {
    fInputTree=NULL;
    fInputFile=NULL;
    fCursor=0;
}

HRootSource::~HRootSource(void) {
  Clear();
}

Bool_t HRootSource::init(void) {
#warning "HRootSource::init not yet implemented..."
  return kTRUE;
}

EDsState HRootSource::getNextEvent(void) {
  if (fInputTree) {
    if (fInputTree->GetEvent(fCursor)>0) {
      fCursor++;
      return kDsOk;
    } else return kDsError;
  }
  return kDsOk;
}

Bool_t HRootSource::getEvent(Int_t eventN) {
  if (fInputTree) {
    if (fInputTree->GetEvent(eventN)>0) return kTRUE;
    else return kFALSE;
  }
  return kFALSE;
}

void HRootSource::Clear(void) {
  if (fInputTree) delete fInputTree;
  if (fInputFile) delete fInputFile;
}

Bool_t HRootSource::setInput(Text_t *fileName,Text_t *treeName) {
  Bool_t r=kTRUE;

  Clear();
  fInputFile=new TFile(fileName);
  if ( (fInputTree=(TTree *)fInputFile->Get(treeName))!=NULL) {
    Char_t sl=*(strchr(fInputTree->GetTitle(),'.')+1);
    switch (sl) {
    case '0' : fSplitLevel=0; break;
    case '1' : fSplitLevel=1; break;
    case '2' : fSplitLevel=2; break;
    default : fSplitLevel=0;
    }
    //fInputTree->SetBranchStatus("*",kFALSE);
    gHades->activateTree(fInputTree);
    r=kTRUE;
  } else {
    Clear();
    r=kFALSE;
  }
  return r;
}

inline Int_t HRootSource::getSplitLevel(void) {
  return fSplitLevel;
}

void HRootSource::deactivateBranch(Text_t *branchName) {
  if (fInputTree) {
    fInputTree->SetBranchStatus(branchName,kFALSE);
  }
}

TTree *HRootSource::getTree(void) {
  return fInputTree;
}



