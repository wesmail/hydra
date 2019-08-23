#include "htree.h"
#include <TROOT.h>
#include <TRealData.h>
#include <TDataMember.h>
#include <TDataType.h>
#include <TBranch.h>
#include <TFile.h>
#include <TKey.h>
#include <TClass.h>
#include <TBranchClones.h>

#include "hbranchobject.h"
#include "hbranchordclones.h"
#include "hbranchclones.h"
#include "hbitfieldcategory.h"

//*-- Author : Manuel Sanchez
//*-- Modified : 12/1/2000 by R.Holzmann
//*-- Modified : 17/12/99 by Denis Bertini
//*-- Modified : 12/05/98 by Manuel Sanchez
//*-- Modified : 19/09/00 by Denis Bertini
//*-- Copyright : GENP (Univ. Santiago de Compostela)

//////////////////////////////////////////////
// HTree
//
//  The HTree is a TTree implementing a automatic split algorithm different
// from the Root one; allowing a more complex tree structure (when used with
// HPartialEvent ...)
//
//  The Branch() functions are inherited from TTree and should be used no 
// longer. Instead, use makeBranch() and addBranch() (see below for info.)
//  
//////////////////////////////////////////////

ClassImp(HTree)

  HTree::HTree(void) : TTree() {
}

HTree::HTree(Text_t* name, Text_t* title, Int_t maxvirtualsize) : TTree (name,title,maxvirtualsize) {
}

HTree::~HTree(void) {
}





TBranch* HTree::makeBranch(Text_t* name, void* clonesaddress, 
			   Int_t bufsize, 
			   Int_t splitlevel, int version ) {
  //This function creates a new TBranchClones for the TClonesArray in 
  //clonesaddress; but it does not add that branch to the HTree.
  

  //name:    global name of this BranchClones
  //bufsize: buffersize in bytes of each individual data member buffer
  //clonesaddress is the address of a pointer to a TClonesArray.
  //
  //This Tree option is provided in case each event consists of one
  //or more arrays of same class objects (tracks, hits,etc).
  //This function creates as many branches as there are public data members
  //in the objects pointed by the TClonesArray. Note that these data members
  //can be only basic data types, not pointers or objects.
  //
  //BranchClones have the following advantages compared to the two other
  //solutions (Branch and BranchObject).
  //  - When reading the Tree data, it is possible to read selectively
  //    a subset of one object (may be just one single data member).
  //  - This solution minimizes the number of objects created/destructed.
  //  - Data members of the same type are consecutive in the basket buffers,
  //    therefore optimizing the compression algorithm.
  //  - Array processing notation becomes possible in the query language.
  //
  //By default the branch buffers are stored in the same file as the Tree.
  //use TBranch::SetFile to specify a different file
  
 if(version == 0 ) {
   if (clonesaddress == 0) return 0;
   char *cpointer =(char*)clonesaddress;
   char **ppointer =(char**)cpointer;
   TClonesArray *list = (TClonesArray*)(*ppointer);
   if (list == 0) return 0;
   gTree = this;
   if (splitlevel) {
      TBranchClones *branch = new TBranchClones(name,clonesaddress,bufsize);
      //      fBranches.Add(branch);
      return branch;
   } else {
      HBranchObject *branch = new HBranchObject(name,list->ClassName(),
						clonesaddress,bufsize,0);
      //      fBranches.Add(branch);
      return branch;
   }
  }else if (version==1){
  
  if (clonesaddress == 0) return 0;
   char *cpointer =(char*)clonesaddress;
   char **ppointer =(char**)cpointer;
   TClonesArray *list = (TClonesArray*)(*ppointer);
   if (list == 0) return 0;
   gTree = this;
   if (splitlevel) {
      HBranchOrdClones *branch = new HBranchOrdClones(name,clonesaddress,bufsize);
      //      fBranches.Add(branch);
      return branch;
   } else {
      HBranchObject *branch = new HBranchObject(name,list->ClassName(),
						clonesaddress,bufsize,0);
      //      fBranches.Add(branch);
      return branch;
   }
  
  }else if(version==2){
   if (clonesaddress == 0) return 0;
   char *cpointer =(char*)clonesaddress;
   char **ppointer =(char**)cpointer;
   TClonesArray *list = (TClonesArray*)(*ppointer);
   if (list == 0) return 0;
   gTree = this;
   if (splitlevel) {
      HBranchClones *branch = new HBranchClones(name,clonesaddress,bufsize);
      //      fBranches.Add(branch);
      return branch;
   } else {
      HBranchObject *branch = new HBranchObject(name,list->ClassName(),
						clonesaddress,bufsize,0);
      //      fBranches.Add(branch);
      return branch;
   }
  
  }

 return 0;
}
 

  
TBranch* HTree::makeBranch(const Text_t* name, const Text_t* classname, 
			   void* addobj, 
			   Int_t bufsize, 
			   Int_t splitlevel) {
  //Build a TBranchObject for an object of class classname.
  //addobj is the address of a pointer to an object of class classname.
  //
  //This option requires access to the library where the corresponding class
  //is defined. Accessing one single data member in the object implies
  //reading the full object.
  //See the next Branch constructor for a more efficient storage
  //in case the event consists of arrays of identical objects.
  //
  //By default the branch buffers are stored in the same file as the Tree.
  //use TBranch::SetFile to specify a different file
   gTree = this;
   TBranch *branch = new HBranchObject(name,classname,addobj,bufsize,
				       splitlevel);
   //fBranches.Add(branch);
   if (!splitlevel) return branch;

   TObjArray *blist = branch->GetListOfBranches();
   const char *rdname;
   const char *dname;
   const char *dtitle;
   char branchname[64];
   TClass *cl = gROOT->GetClass(classname);
   if (!cl) return 0;
   if (!cl->GetListOfRealData()) cl->BuildRealData();
   char **apointer = (char**)(addobj);
   TObject *obj = (TObject*)(*apointer);
   Bool_t delobj = kFALSE;
   if (!obj) {
      obj = (TObject*)cl->New();
      delobj = kTRUE;
   }
//*-*- Loop on all public data members of the class and its base classes
   TBranch *branch1 = 0;
   TRealData *rd;
   TIter      next(cl->GetListOfRealData());
   while ((rd = (TRealData *) next())) {
      TDataMember *dm = rd->GetDataMember();
      if (!dm->IsPersistent()) continue; //do not process members with a ! as the first
                                         // character in the comment field
      dtitle=dm->GetTitle();
      if (dtitle[0]=='-') continue;  //do not proccess members with a - as the
                                     //first character in the comment field  
      rdname = rd->GetName();
      dname  = dm->GetName();

  //  Next line now commented, functionality to process arrays is now implemented
  //  the statement is left to show how to use Property() and kIsArray
  //     if (dm->Property() & kIsArray) continue;

      TDataType *dtype = dm->GetDataType();
      Int_t code = 0;
      if (dtype) code = dm->GetDataType()->GetType();

//*-*- Encode branch name. Use real data member name
      sprintf(branchname,"%s.%s",name,rdname);

      char leaflist[64];
      Int_t offset    = rd->GetThisOffset();
      char *pointer   = (char*)obj + offset;
      if (dm->IsaPointer()) {
         TClass *clobj = 0;
         if (!dm->IsBasic()) clobj = gROOT->GetClass(dm->GetTypeName());


	   if (clobj &&  clobj->InheritsFrom(TClonesArray::Class()) ) {    
	    // native TClonesArray
	     //  if(!strcmp("TClonesArray",clobj->GetName())|| !strcmp("HClonesArray",clobj->GetName())  )
              if(!strcmp("TClonesArray",clobj->GetName())  )
            {
	      sprintf(branchname,"%s.%s",name,&rdname[1]);
	      branch1 = new TBranchClones(branchname,pointer,bufsize);
	      blist->Add(branch1);
            }else if(!strcmp("HOrdClonesArray",clobj->GetName())){
              sprintf(branchname,"%s.%s",name,&rdname[1]);
	      branch1 = new HBranchOrdClones(branchname,pointer,bufsize);
	      blist->Add(branch1);
	    }else if(!strcmp("HClonesArray",clobj->GetName())){
	             sprintf(branchname,"%s.%s",name,&rdname[1]);
	             branch1 = new HBranchClones(branchname,pointer,bufsize);
	             blist->Add(branch1);    
	        }
            

         } else {
            if (!clobj) {
               if (code != 1) continue;
               sprintf(leaflist,"%s/%s",dname,"C");
               branch1 = new TBranch(branchname,pointer,leaflist,bufsize);
               branch1->SetTitle(dname);
               blist->Add(branch1);
            } else {
               if (!clobj->InheritsFrom(TObject::Class())) continue;
               sprintf(branchname,"%s.%s",name,&rdname[1]);
               branch1 = new HBranchObject(branchname,clobj->GetName(),pointer,bufsize,0);
              
               blist->Add(branch1);
            }
         }
      } else {
//*-*-------------Data Member is a basic data type----------
         if (dm->IsBasic()) {
            if      (code ==  1) sprintf(leaflist,"%s/%s",rdname,"B");
            else if (code == 11) sprintf(leaflist,"%s/%s",rdname,"b");
            else if (code ==  2) sprintf(leaflist,"%s/%s",rdname,"S");
            else if (code == 12) sprintf(leaflist,"%s/%s",rdname,"s");
            else if (code ==  3) sprintf(leaflist,"%s/%s",rdname,"I");
            else if (code == 13) sprintf(leaflist,"%s/%s",rdname,"i");
            else if (code ==  5) sprintf(leaflist,"%s/%s",rdname,"F");
            else if (code ==  8) sprintf(leaflist,"%s/%s",rdname,"D");
            branch1 = new TBranch(branchname,pointer,leaflist,bufsize);
            branch1->SetTitle(rdname);
            blist->Add(branch1);
         }
      }
      branch1->SetOffset(offset);
   }
   // comment this part for the moment (D.Bertini) 
   //  if (!strcmp("HBitFieldCategory",obj->GetName())){
   //                       ((HBitFieldCategory*) obj)->makeBranch(this);
   //  }                     
   if (delobj) delete obj;
  
   return branch;
}

TBranch *HTree::makeBranch(Text_t* name, void* address, 
			  Text_t* leaflist, Int_t bufsize) {
  //This Branch constructor is provided to support non-objects in
  //a Tree. The variables described in leaflist may be simple variables
  //or structures.
  //See the two following constructors for writing objects in a Tree.
  //
  //By default the branch buffers are stored in the same file as the Tree.
  //use TBranch::SetFile to specify a different file  
   gTree = this;
   TBranch *branch = new TBranch(name,address,leaflist,bufsize);
   //fBranches.Add(branch);
   return branch;
}

TBranch* HTree::addBranch(Text_t* name, void* clonesaddress, 
			  Int_t bufsize, Int_t splitlevel) {
 //It does the same as the correspondig makeBranch() method but adds the
  //generated branch to the HTree's top level
  TBranch *r=NULL;
  r=makeBranch(name,clonesaddress,bufsize,splitlevel);
  fBranches.Add(r);
  return r;
}

TBranch* HTree::addBranch(const Text_t* name,const Text_t* classname, 
			  void* addobj,Int_t bufsize, 
			  Int_t splitlevel) {
  //It does the same as the correspondig makeBranch() method but adds the
  //generated branch to the HTree's top level
  TBranch *r=NULL;
  r=makeBranch(name,classname,addobj,bufsize,splitlevel);
  fBranches.Add(r);
  return r;
}

TBranch* HTree::addBranch(Text_t* name, void* address, Text_t* leaflist, 
			  Int_t bufsize) {
 //It does the same as the correspondig makeBranch() method but adds the
  //generated branch to the HTree's top level
  TBranch *r=NULL;
  r=makeBranch(name,address,leaflist,bufsize);
  fBranches.Add(r);
  return r;
}

void HTree::printContainerSizes(void) {
// This function returns a formatted list of data containers with the
// file size of each container.  To be used with Hydra < v6.00 (old
// branching scheme), probably can be adapted for new Root...

   printf("\n");
   printf("      File: %s  \n",this->GetCurrentFile()->GetName());
   printf("            (%i entries) \n",(int) this->GetEntries());
   printf("\n");
   printf("         Container          File Size         Size/evt\n");
   printf("      ---------------  ------------------   ------------\n");

   Int_t nBytes = 0, nBytesBranch2 = 0;
   TObjArray *array2 = 0, *array3 = 0, *array4 = 0;
   TBranch *branch1 = 0, *branch2 = 0, *branch3 = 0, *branch4 = 0;

//   array1 = this->GetListOfBranches();

   for(Int_t i=0;i<fBranches.GetEntries();i++) {

      branch1 = (TBranch*) fBranches.At(i);
      array2 = branch1->GetListOfBranches();

      for(Int_t j=0;j<array2->GetEntries();j++) {

         branch2 = (TBranch*) array2->At(j);
         array3 = branch2->GetListOfBranches();

         Int_t nBranch2 = array3->GetEntries();

         nBytesBranch2 = 0;

         for(Int_t k=0;k<nBranch2;k++) {

            branch3 = (TBranch*) array3->At(k);
            nBytesBranch2 += (int) branch3->GetZipBytes();

            array4 = branch3->GetListOfBranches();
            Int_t nBranch3 = array4->GetEntries();

            for(Int_t l=0;l<nBranch3;l++) {

               branch4 = (TBranch*) array4->At(l);
               nBytesBranch2+= (int) branch4->GetZipBytes();
            }

         }
         if(nBytesBranch2) printf(" %20s %11i (%4.1f%%)   %6.1f bytes\n",
               branch2->GetName(),nBytesBranch2,
               nBytesBranch2*100./this->GetZipBytes(), nBytesBranch2/branch1->GetEntries() );

         nBytes += nBytesBranch2;
      }

   }
   printf("                       ------------------   ------------\n");
   printf("                      %11i (%4.1f%%)   %6.3f kB/event\n",
         nBytes,nBytes*100./this->GetZipBytes(), nBytes/branch1->GetEntries()/1024. );
   printf("\n");

}

Int_t HTree::MakeCode(const char* filename) {

//----
// MakeCode adapted for Htree with split level 2
// <D.Bertini@gsi.de>

// Connect output file
   char *tfile = new char[1000];
   if (filename) strcpy(tfile,filename);
   else          sprintf(tfile,"%s.C",GetName());
   FILE *fp = fopen(tfile,"w");
  if (!fp) {
      Error("MakeCode","Cannot open output file:%s\n",tfile);
      return -1;
   }
//connect the htree file
   char *treefile = new char[1000];
   if (fDirectory && fDirectory->GetFile())
                strcpy(treefile,fDirectory->GetFile()->GetName());
   else         strcpy(treefile,"Memory Directory");

   TDatime td;
   fprintf(fp,"{\n");
   fprintf(fp,"///////////////////////////////////////////////////////////\n");
   fprintf(fp,"// This file has been automatically generated \n");
   fprintf(fp,"// (%s by ROOT version%s)\n",td.AsString(),gROOT->GetVersion());
   fprintf(fp,"// from HTree %s/%s\n",this->GetName(),this->GetTitle());
   fprintf(fp,"// found on file: %s\n",treefile);
   fprintf(fp,"///////////////////////////////////////////////////////////\n");
   fprintf(fp,"\n");
   fprintf(fp,"\n");

// Reset and file connect
   fprintf(fp,"//Reset ROOT and connect tree file\n");
   fprintf(fp,"   gROOT->Reset();\n");
   fprintf(fp,"   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(\"%s\");\n",treefile);
   fprintf(fp,"   if (!f) {\n");
   fprintf(fp,"      f = new TFile(\"%s\");\n",treefile);
   if (gDirectory != gFile) {
      fprintf(fp,"      f->cd(\"%s\");\n",gDirectory->GetPath());
   }
   fprintf(fp,"   }\n");
   fprintf(fp,"   HTree *%s = (HTree*)gDirectory->Get(\"%s\");\n\n",
                  GetName(),GetName());
   fprintf(fp,"   if(!%s) {cout << \"No tree in file!\" << endl; exit();}\n",
                  GetName());

   fprintf(fp,"\n   HRecEvent* fEvent = (HRecEvent*)f->Get(\"Event\");\n");
   fprintf(fp,"   if(fEvent) HEventHeader* fHead = fEvent->getHeader();\n");
   fprintf(fp,"   T->SetBranchAddress(\"Event\",&fEvent);\n\n");

//**** Scanning of Htree 
//**** for the moment 
   TFile* f = fDirectory->GetFile(); 
   TList* total = f->GetListOfKeys(); 
   Int_t entries = total->GetSize();

//**** Only work in split mode 2 for the moment
   Char_t sl=*(strchr(GetTitle(),'.')+1);
   switch (sl) {
      case '0' : Error("MakeCode", "split 0 not yet implemented");
                 return -1; 
                 break;
      case '1' : Error("MakeCode", "split 0 not yet implemented");
                 return -1; 
                 break;
   } 


// Scan the File structure linked to the Htree and parse 
// the relevant subdirectories
   for(Int_t i=0;i<entries;i++){
     TKey* akey = (TKey*) total->At(i);
     Text_t ptr[50];
     Text_t ptrObj[50];
     Text_t branch[100];
     char* partial[9]={"Mdc","Tof","Tofino","Start","Rich","Shower","Simul","Tracks","Trigger"};
     for(Int_t k=0; k<9; k++){
       if(strstr(akey->GetName(),partial[k]) != 0){
	 f->cd(akey->GetName());
	 fprintf(fp,"   f->cd(\"%s\"); \n",akey->GetName());
	 TList* subkey = gDirectory->GetListOfKeys();
	 Int_t s2 = subkey->GetSize();
	 for(Int_t j=0;j<s2;j++){ 
	   TKey* akey2 = (TKey*) subkey->At(j);
	   if(strstr(akey2->GetClassName(),"LinearCategory") != 0){
	     sprintf(ptrObj,"ptl%s",akey2->GetName());     
	     fprintf(fp,"   %s* %s = new %s();\n",akey2->GetName(),
		     ptrObj,akey2->GetName());  
	     sprintf(ptr,"catl%s",akey2->GetName()); 
	     fprintf(fp,"   HLinearCategory* %s = (HLinearCategory*) gDirectory->Get(\"%s\");\n",ptr,akey2->GetName());
	     sprintf(branch,"brl%s",akey2->GetName());
	     fprintf(fp,"   TBranch* %s = %s->GetBranch(\"%s\");\n",
		     branch,this->GetName(),akey2->GetName()); 
	     fprintf(fp,"   if(%s) %s->SetAddress(&%s);\n\n",branch, branch, ptr);  
	   }

	   if(strstr(akey2->GetClassName(),"MatrixCategory") != 0){
	     sprintf(ptrObj,"ptm%s",akey2->GetName());     
	     fprintf(fp,"   %s* %s = new %s();\n",akey2->GetName(),
		     ptrObj,akey2->GetName());  
	     sprintf(ptr,"catm%s",akey2->GetName()); 
	     fprintf(fp,"   HMatrixCategory* %s = (HMatrixCategory*) gDirectory->Get(\"%s\");\n",ptr,akey2->GetName());
	     sprintf(branch,"brm%s",akey2->GetName());
	     fprintf(fp,"   TBranch* %s = %s->GetBranch(\"%s\");\n",
		     branch,this->GetName(),akey2->GetName()); 
	     fprintf(fp,"   if(%s) %s->SetAddress(&%s);\n\n",branch, branch, ptr);
	   }

	  if(strstr(akey2->GetClassName(),"MatrixCategorySplit") != 0){
	    sprintf(ptrObj,"pts%s",akey2->GetName());     
	    fprintf(fp,"   %s* %s = new %s();\n",akey2->GetName(),
		    ptrObj,akey2->GetName());  
	    sprintf(ptr,"cats%s",akey2->GetName()); 
	    fprintf(fp,"   HMatrixCategorySplit* %s = (HMatrixCategorySplit*) gDirectory->Get(\"%s\");\n",ptr,akey2->GetName());
	    sprintf(branch,"brs%s",akey2->GetName());
	    fprintf(fp,"   TBranch* %s = %s->GetBranch(\"%s\");\n",
		    branch,this->GetName(),akey2->GetName()); 
	    fprintf(fp,"   if(%s) %s->SetAddress(&%s);\n\n",branch, branch, ptr);
	  }
	 }
       }
     }
   }

   fprintf(fp,"   f->cd();\n");

//Generate instructions to make the loop on entries
   fprintf(fp,"\n//     This is the loop skeleton\n");
   fprintf(fp,"//       To read only selected branches, Insert statements like:\n");
   fprintf(fp,"// %s->SetBranchStatus(\"*\",0);  // disable all branches\n",GetName());
   fprintf(fp,"// %s->SetBranchStatus(\"branchname\",1);  // activate branchname\n",GetName());
   fprintf(fp,"\n   Int_t nentries = %s->GetEntries();\n",GetName());
   fprintf(fp,"\n   Int_t nbytes = 0;\n");
   fprintf(fp,"//   for (Int_t i=0; i<nentries;i++) {\n");
   fprintf(fp,"//   For Accessing  the Data you can:  \n");
   fprintf(fp,"//    1)Use of a Total Io on theHTree \n");
   fprintf(fp,"//      nbytes += %s->GetEntry(i);\n",GetName());
   fprintf(fp,"//    2)Use of a specific branch Io on the Htree \n");
   fprintf(fp,"//      nbytes += branchname->GetEntry(i);\n");
   fprintf(fp,"\n");
   fprintf(fp,"//   To loop over Data-Objects:\n");
   fprintf(fp,"//      TIter iter = pCat->MakeIterator();\n");
   fprintf(fp,"//      iter->Reset();\n");
   fprintf(fp,"//      while ( (pObj=(HDataObjName*)iter->Next()) != 0 ){ \n");
   fprintf(fp,"//         pObj->getValue();\n");
 
   fprintf(fp,"//      }\n");
   fprintf(fp,"//   pCat->Clear();\n");
   fprintf(fp,"//   }\n");
   fprintf(fp,"}\n");

   fclose(fp);
   printf("Macro %s generated from HTree: %s\n",filename,GetName());

   delete [] treefile; 
   delete [] tfile;
   return 0;
}

