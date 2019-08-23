#pragma implementation

#include "hbranchclones.h"
#include "hcompositecategory.h"
#include "hbitfieldcategory.h"
#include <TRealData.h>
#include <TROOT.h>
#include <TList.h>
#include <TLeaf.h>
#include <TClass.h>
#include <TDataMember.h>
#include <TDataType.h>
#include "hclonesarray.h"
#include "hdebug.h"
#include <iostream.h>

//////////////////////////////////////////////////////
//HBranchClones
//////////////////////////////////////////////////////



HBranchClones::HBranchClones(void) : TBranchClones() {
}

HBranchClones::HBranchClones(const Text_t *classname, 
			     void* clonesaddress,
			     Int_t basketsize,
			     Int_t compress=-1) : TBranchClones(classname,
								clonesaddress,
								basketsize,
								compress) {
}

HBranchClones::~HBranchClones(void) {
}

Int_t HBranchClones::Fill(){
#if DEBUG_LEVEL>0
Warning("Fill"," dedicated fill called \n"); 
#endif

 ((HClonesArray*) fList)->Compress(); 
 return TBranchClones::Fill(); 
}



ClassImp(HBranchClones)
