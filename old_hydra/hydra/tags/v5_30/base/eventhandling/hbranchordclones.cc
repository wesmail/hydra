#pragma implementation

#include "hbranchordclones.h"
#include "hcompositecategory.h"
#include "hordcategory.h"
#include <TRealData.h>
#include <TROOT.h>
#include <TList.h>
#include <TLeaf.h>
#include <TClass.h>
#include <TDataMember.h>
#include <TDataType.h>
#include "hordclonesarray.h"
#include "hdebug.h"
#include <iostream.h>

//////////////////////////////////////////////////////
//HBranchOrdClones
//////////////////////////////////////////////////////



HBranchOrdClones::HBranchOrdClones(void) : TBranchClones() {
}

HBranchOrdClones::HBranchOrdClones(const Text_t *classname, 
			     void* clonesaddress,
			     Int_t basketsize,
			     Int_t compress=-1) : TBranchClones(classname,
								clonesaddress,
								basketsize,
								compress) {
}

HBranchOrdClones::~HBranchOrdClones(void) {
}

Int_t HBranchOrdClones::Fill(){
#if DEBUG_LEVEL>0
Warning("Fill"," dedicated fill called \n"); 
#endif
((HOrdClonesArray*) fList)->createPattern(); 
 return TBranchClones::Fill(); 
}



ClassImp(HBranchOrdClones)
