#include "hclonesarray.h"
#include "hlocateddataobject.h"
#include <TROOT.h>
#include <TClass.h>
#include "assert.h"

ClassImp( HClonesArray)

//-- Author : Denis Bertini <dbertini@gsi.de>
//-- Modified : 19/09/00 by  D. Bertini
//
//*************************************************************************//
// This Class overloads the TClonesArray::Compress() algorithm in order
// to provide slot position for the 'sub nodes' used in the HBitFieldCategory.
// The call of the method HClonesArray::Compress() once per event
// is then mandatory for the correct usage of this particular category.
// This is automatically assured by overloading TBranchClones::Fill() 
// function (see HBranchClones).
//
//*************************************************************************//


HClonesArray::HClonesArray() : TClonesArray(){
barray=0;
}

HClonesArray::HClonesArray(const Char_t *classname, 
                           Int_t          s, 
                           Bool_t b) : TClonesArray(classname,s,b) {


  //  SetBit(kForgetBits);
} 


void HClonesArray::Compress(){ 

   // Remove empty slots from array.
   // Bookeeping of slot positions for
   // the 'sub nodes of the HBitCategory 
   // is provided.  

   Int_t j = 0, je = 0;
   // problem with inheritance 
   TObject ** keep = &( (*fKeep)[0]); 
   TObject **tmp = new TObject* [fSize];
   
   //   Int_t count =0;
   Int_t mod = 0; 
   Int_t temp = -1;  
 
   for (Int_t i = 0; i < fSize; i++) {
      if (fCont[i]) {
	// !!slow process!! 
	//     	mod = barray->getSubLocation( 
	//	 (((HLocatedDataObject*) fCont[i])->getLocation())
        //         );

       mod = barray->getSubLocation(((HLocatedDataObject*) fCont[i])); 

      if ( temp != mod ){
           barray->setMod(mod, j);
	   //   printf(" SET: module  %i  at:%i \n",mod,j);
           temp = mod;
         }

         fCont[j] = fCont[i];
         *(keep + j) = *(keep + i);
         j++;
      } else {
         tmp[je] = *(keep + i);
         je++;
      }
   }

   fLast = j - 1;

   Int_t jf = 0;
   for ( ; j < fSize; j++) {
      fCont[j] = 0;
       *(keep + j) = tmp[jf]; 
      jf++;
   }
   
   delete [] tmp;

   assert(je == jf);
}


 void HClonesArray::Streamer(TBuffer &b)
{
 
   // Modified streamer with no storage of empty slot in file
   // since the HClonesArray is always a compact structure 


   Int_t   nobjects;
   TString s;

   if (b.IsReading()) {
      Version_t v = b.ReadVersion();
      if (v > 1) {
         fName.Streamer(b);
      }
      s.Streamer(b);
      TClass *cl = gROOT->GetClass(s.Data());
      b >> nobjects;
      if (nobjects < 0)
         nobjects = -nobjects;  // still there for backward compatibility
      b >> fLowerBound;
      if (fClass == 0 && fKeep == 0) {
         fClass = cl;
         fKeep  = new TObjArray(fSize);
     //   printf("clones streamer, nobjects=%d, fSize=%d \n",nobjects,fSize);
         Expand(nobjects);
      }
      if (cl != fClass) {
         Error("Streamer", "expecting objects of type %s, finding objects"
            " of type %s", fClass->GetName(), cl->GetName());
         return;
      }

      // make sure there are enough slots in the fKeep array
      if (fKeep->GetSize() < nobjects)
         Expand(nobjects);
      TObject ** keep = &( (*fKeep)[0]); 
 
      for (Int_t i = 0; i < nobjects; i++) {
            if (!(*(keep+i)))
               *(keep +i ) = (TObject*)fClass->New();

            fCont[i] = *( keep + i );
            (*(keep+i))->Streamer(b);
            fLast = i;
      }
      Changed();
   } else {
      b.WriteVersion(TClonesArray::IsA());
      fName.Streamer(b);
      s = fClass->GetName();
      s.Streamer(b);
      nobjects = GetEntriesFast();
      b << nobjects;
      b << fLowerBound;
      for (Int_t i = 0; i < nobjects; i++) {
           printf(" i write objects nb:%i \n",i);
           if(fCont[i]) fCont[i]->Streamer(b);
      }
   }
}

/*
 void HClonesArray::Streamer(TBuffer &b)
{
   // Second version with  expand of a sparse structure 
   // This gave bad results 
   // Modified streamer with no storage of empty slot in file
   // since the HClonesArray is always a compact structure 

  // Warning("Streamer()"," using the modified IO");
   Int_t   nobjects;
   Char_t nch0,nch1,nch;
   TString s;
   Int_t j=0;


   if (b.IsReading()) {
      Version_t v = b.ReadVersion();
      if (v > 1) {
         fName.Streamer(b);
      }
      s.Streamer(b);
      TClass *cl = gROOT->GetClass(s.Data());
      b >> nobjects;
      if (nobjects < 0)
         nobjects = -nobjects;  // still there for backward compatibility
      b >> fLowerBound;
      if (fClass == 0 && fKeep == 0) {
         fClass = cl;
         fKeep  = new TObjArray(fSize);
    //   printf("clones streamer, nobjects=%d, fSize=%d \n",nobjects,fSize);
         Expand(nobjects);
      }
      if (cl != fClass) {
         Error("Streamer", "expecting objects of type %s, finding objects"
            " of type %s", fClass->GetName(), cl->GetName());
         return;
      }

      // make sure there are enough slots in the fKeep array
      if (fKeep->GetSize() < nobjects)  Expand(nobjects);
      
      TObject ** keep = &( (*fKeep)[0]); 
      // i reconstruct sparse structure ...  
      for (Int_t i = 0; i < nobjects; i++) {
	b >> nch;
        if (nch) {   
              
            if (!(*(keep+i)))
               *(keep +i ) = (TObject*)fClass->New();
            fCont[i] = *( keep + i );
            (*(keep+i))->Streamer(b);
            fLast = i;
	    // printf("filling slot at pos :%i %i\n", i,fCont[i]);            
	}else{
         fCont[i] = NULL; 
	 // printf("filling 0 slot at pos :%i %i\n", i,fCont[i]);            
	} 
        
      }


      Changed();
   } else {
      b.WriteVersion(TClonesArray::IsA());
      fName.Streamer(b);
      s = fClass->GetName();
      s.Streamer(b);
      nobjects = GetEntriesFast();
      nch0=0;
      nch1=1;   
      //  printf(" nobject=> %i \n ",nobjects);
      b << nobjects;
      b << fLowerBound;
      for (Int_t i = 0; i < fSize; i++) {
        if (!fCont[i]) {
	  //            nch = 0;
            b << nch0;
         } else {
	   //            nch = 1;
             b << nch1;
            fCont[i]->Streamer(b);
	 }
      }
   }
}


*/
