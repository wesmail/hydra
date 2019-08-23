#pragma implementation

#include "hbranchobject.h"
#include <TRealData.h>
#include <TROOT.h>
#include <TList.h>
#include <TLeaf.h>
#include <TClass.h>
#include <TDataMember.h>

#include "hdebug.h"
#include <iostream.h>

//*-- Author : Manuel Sanchez
//*-- Modified : 19/05/98 by Manuel Sanchez
//*-- Copyright : GENP (Univ. Santiago de Compostela)

//////////////////////////////////////////////////////
//HBranchObject
//
// This is a "Root tree" branch for the case of an object; it differs from
//TBranchObject in the SetAddress() method which now is adapted to the split
//algorithm implied by the use of HTree and the HEvent classes.
//
//////////////////////////////////////////////////////
HBranchObject::HBranchObject(void) : TBranchObject() {
}

HBranchObject::HBranchObject(const Text_t *name,const Text_t *classname, 
			     void* addobj,
			     Int_t basketsize,Int_t splitlevel=0,
			     Int_t compress=-1) : TBranchObject(name,classname,
								addobj,
								basketsize,
								splitlevel,
								compress) {
}

HBranchObject::~HBranchObject(void) {
}

void HBranchObject::SetAddress(void *add) {
  //Sets the address of this branch.
   Int_t nbranches = fBranches.GetEntriesFast();
#if DEBUG_LEVEL>2
   gDebuger->message("HBranchObject::SetAddress(%s)",GetName());
   gDebuger->incIdentLevel();
   gDebuger->message("nbranches=%i",nbranches);
#endif
//   if (!nbranches) {
      TLeaf *leaf = (TLeaf*)fLeaves.UncheckedAt(0);
      if (leaf) leaf->SetAddress(add);
//      return;
//   }
   fAddress = (char*)add;
   char *pointer   = fAddress;
   void **ppointer = (void**)add;
   TObject *obj = (TObject*)(*ppointer);
   TClass *cl = gROOT->GetClass(fClassName.Data());
   if (!obj && cl) {
      obj = (TObject*)cl->New();
   }
   TList *rdata  = 0;
   TIterator *rdIter=0;
   TRealData *rd = 0;
   if (cl) {
      if (!cl->GetListOfRealData())  cl->BuildRealData();
      rdata = cl->GetListOfRealData();
      rd    = (TRealData *)rdata->First();
   }
   Int_t i, offset;

   /*for (i=0;i<nbranches;i++)  {
      TBranch *branch  = (TBranch*)fBranches[i];
      if (!cl) {
         offset = 0;
      } else {
         while(1) {
            TDataMember *member = rd->GetDataMember();
            if (member->IsPersistent() && (member->IsBasic() || member->IsaPointer())) break;
            rd = (TRealData *)rdata->After(rd);
         }
         offset = rd->GetThisOffset();
         rd = (TRealData *)rdata->After(rd);
      }
      pointer = (char*)obj +offset;
      branch->SetAddress(pointer);
   }*/
   rdIter=rdata->MakeIterator();

   for (i=0;i<nbranches;i++) {
     TBranch *branch= (TBranch *)fBranches[i];
     if (!cl) {
       offset=0;
       pointer=(char*)obj+offset;
       branch->SetAddress(pointer);
     } else {
       rdIter->Reset();
       while (((rd=(TRealData *)rdIter->Next())!=NULL) && 
	      (strstr(branch->GetName(),rd->GetName())==NULL)) ;
       if (rd!=NULL) {
	 TDataMember *member=rd->GetDataMember();
	 if (member->IsPersistent() && 
	     (member->IsBasic() || member->IsaPointer()) &&
	     (member->GetTitle()[0]!='-')) {
	   offset=rd->GetThisOffset();
	   pointer=(char*)obj+offset; //Ojo  aqui
	   branch->SetAddress(pointer);
	 }
       }
     }
   }
   delete rdIter;
#if DEBUG_LEVEL>2
   gDebuger->decIdentLevel();
   gDebuger->message("setAddress.return");
#endif
}


ClassImp(HBranchObject)












