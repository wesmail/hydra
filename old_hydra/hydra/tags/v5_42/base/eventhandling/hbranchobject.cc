#pragma implementation

#include "hbranchobject.h"
#include <TRealData.h>
#include <TROOT.h>
#include <TList.h>
#include <TLeaf.h>
#include <TClass.h>
#include <TDataMember.h>
#include <TDataType.h>

#include "hdebug.h"
#include <iostream.h>

//*-- Author : Manuel Sanchez
//*-- Modified : 28/05/99 by Ilse Koenig
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

 fReadEntry = -1;
   Int_t nbranches = fBranches.GetEntriesFast();
   TLeaf *leaf = (TLeaf*)fLeaves.UncheckedAt(0);
   if (leaf) leaf->SetAddress(add);
   TBranch *branch;
   fAddress = (char*)add;
   char *pointer   = fAddress;
   void **ppointer = (void**)add;
   TObject *obj = (TObject*)(*ppointer);
   TClass *cl = gROOT->GetClass(fClassName.Data());
   if (!obj && cl) {
      obj = (TObject*)cl->New();
   }
   Int_t i, offset;
   if (!cl) {
      for (i=0;i<nbranches;i++)  {
         branch  = (TBranch*)fBranches[i];
         pointer = (char*)obj;
         branch->SetAddress(pointer);
      }
      return;
   }
   if (!cl->GetListOfRealData())  cl->BuildRealData();
//printf("TBranchObject::SetAddress, branch=%s\n",GetName());
   char *fullname = new char[200];
   const char *bname = GetName();
   Int_t lenName = strlen(bname);
   Int_t isDot = 0;
   if (bname[lenName-1] == '.') isDot = 1;
   const char *rdname;
   TRealData *rd;
   TIter      next(cl->GetListOfRealData());
   while ((rd = (TRealData *) next())) {
      TDataMember *dm = rd->GetDataMember();
      if (!dm->IsPersistent()) continue;
      if (nbranches!=0 && dm->GetTitle()[0]=='-') continue;
      rdname = rd->GetName();
      TDataType *dtype = dm->GetDataType();
      Int_t code = 0;
      if (dtype) code = dm->GetDataType()->GetType();
      offset  = rd->GetThisOffset();
      pointer = (char*)obj + offset;
//printf(" rdname=%s, offset=%d\n",rdname,offset);
      branch  = 0;
      if (dm->IsaPointer()) {
         TClass *clobj = 0;
         if (!dm->IsBasic()) clobj = gROOT->GetClass(dm->GetTypeName());
         if (clobj && clobj->InheritsFrom("TClonesArray")) {
	   branch = (TBranch*)fBranches.FindObject(&rdname[1]);
	   if (!branch) {
	     sprintf(fullname,"%s.%s",bname,&rdname[1]);
	     branch = (TBranch*)fBranches.FindObject(fullname);
	   }
         } else {
            if (!clobj) {
               if (code != 1) continue;
               if (isDot) sprintf(fullname,"%s%s",bname,&rdname[0]);
               else       sprintf(fullname,"%s",&rdname[0]);
               branch = (TBranch*)fBranches.FindObject(fullname);
	       if (!branch) {
		 sprintf(fullname,"%s.%s",bname,&rdname[0]);
		 branch = (TBranch*)fBranches.FindObject(fullname);
	       }
            } else {
               if (!clobj->InheritsFrom(TObject::Class())) continue;
               if (isDot) sprintf(fullname,"%s%s",bname,&rdname[1]);
               else       sprintf(fullname,"%s",&rdname[1]);
               branch = (TBranch*)fBranches.FindObject(fullname);
	       if (!branch) {
		 sprintf(fullname,"%s.%s",bname,&rdname[1]);
		 branch = (TBranch*)fBranches.FindObject(fullname);
	       }
            }
         }
      } else {
         if (dm->IsBasic()) {
            if (isDot) sprintf(fullname,"%s%s",bname,&rdname[0]);
            else       sprintf(fullname,"%s",&rdname[0]);
            branch = (TBranch*)fBranches.FindObject(fullname);
	    if (!branch) {
	      sprintf(fullname,"%s.%s",bname,&rdname[0]);
	      branch = (TBranch*)fBranches.FindObject(fullname);
	    }
         }
      }
//if(!branch)printf("cannot find branch\n");
      if(branch) branch->SetAddress(pointer);
   }
   delete [] fullname;
}


ClassImp(HBranchObject)
