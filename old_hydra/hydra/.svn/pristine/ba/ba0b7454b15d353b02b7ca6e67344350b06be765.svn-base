#ifndef HBARRAY_H
#define HBARRAY_H

#include "TObject.h"
#include "hlocateddataobject.h"
#include "hlocation.h"
#include <stddef.h>

// typedef names 
 typedef size_t                  elem_t;
 typedef UChar_t           bit;


class HBArray : public TObject {
private:
  // streamable 
  static Int_t nWords;      // Number of Words contained in the bit-field 
  static Int_t nSplit;      // Split level choosen according the category setup
  static UInt_t** data;     // Ptr array defining dynamic bit array 
  static Int_t *modules;    // Index table for each nodes 
  static Int_t nDimensions; // Number of indexes   
  static Int_t *lSizes;     // Range  of indexes 


  // not streamable 
  UInt_t mBits;                  //! 
  static UInt_t bitPerWord;      //!
  UInt_t* dataCurrent;           //!
  Int_t offset;                  //! 
  static Int_t *indexes;         //! cache mode 
  static Bool_t caching;         //! 
  static Int_t init_count;       //! 
  static Bool_t S_setup;         //! used for streamer 2 (sparse structure)

  inline UInt_t maxBytes(void) const; //!
  inline UInt_t maxWords(void) const; //!
 
public:
   HBArray(void);	
   HBArray( UInt_t mlenght);				        
   virtual ~HBArray(void);
   void set( UInt_t pos);
   void setDimensions(Int_t nDim,Int_t *sizes); 
   Int_t* getDimensions(){ return lSizes;} 
   Int_t getNDim() {return nDimensions;} 
   void clear( UInt_t pos); 
   void set(void); 
   void clear(void); 
   Int_t operator[](UInt_t pos); 
   UInt_t lenght(void){return mBits;} 
   void dump(void);
   UInt_t  bcount(const size_t nsize );
   inline UInt_t  bcount2(const size_t nsize );
   Int_t getNBitSet ( size_t  upto);
   UInt_t** getData(){ return data;}
   void  setCapacity( Int_t nword,Int_t nsplit); 
   void release(); 
   Int_t getEntries(void) { return  nWords*bitPerWord;}     
   inline Int_t getSubLocation(  HLocation *aLoc );
   inline Int_t getSubLocation( Int_t *aLoc, Int_t dim );
   inline Int_t getSubLocation( HLocatedDataObject* data );
   inline Int_t getPosition( HLocation &aLoc);
   inline void set ( HLocation &aLoc );
   inline void setMod( Int_t loc, Int_t i);
   void complement(const size_t nsize );
   void Streamer_sparse(TBuffer & R__b); 
  ClassDef(HBArray,1) // bit array class

}; //!HBARRAY_H //




   

 inline Int_t HBArray::getSubLocation(  HLocation *aLoc ) {
     Int_t linear=aLoc->operator[](0);  

           for(Int_t i=1;i<nDimensions-1;i++) 
                         linear = linear*lSizes[i]+
                                  aLoc->operator[](i);
    #if DEBUG_LEVEL>2
	   aLoc->Dump(); 
           Warning(" linear:%i nDImens:%i \n",linear, nDimensions);
    #endif
	 return linear;
  }



 inline Int_t HBArray::getSubLocation( HLocatedDataObject* data ) {
    Int_t linear=data->getLocationIndex(0);
    #if DEBUG_LEVEL>2  
    Warning(" linear:%i nDImens:%i \n",linear, nDimensions);
    #endif  
    for(Int_t i=1;i<nDimensions-1;i++) {
               linear = linear*lSizes[i]+
                        data->getLocationIndex(i);
    #if DEBUG_LEVEL>2
	 Warning(" iter:%i sizes[]:%i index:%i \n",i,lSizes[i],data->getLocationIndex(i));
    #endif

     }

    #if DEBUG_LEVEL>2
       Warning(" linear:%i \n",linear);
    #endif
 
	 return linear;
  }




 inline Int_t HBArray::getSubLocation( Int_t *aLoc, Int_t dim ) {
    #if DEBUG_LEVEL>2 
     Warning(" aLoc:%i \n",aLoc->operator[](0));
    #endif
    Int_t linear=aLoc[0];

    #if DEBUG_LEVEL>2  
    Warning(" getSubLocation linear:%i nDImens:%i \n",linear, nDimensions);
    #endif
  
           for(Int_t i=1;i<dim-1;i++) 
               linear = linear*lSizes[i]+
                         aLoc[i];
	 return linear;
  }

 inline Int_t HBArray::getPosition( HLocation &aLoc){ 

     //  Get the position of a HLocatedData objects 
     //  knowing its Location. The return value is
     //  the computed position in the linear TClonesArray.
     //  if the position cannot be recomputed a negative 
     //  value will be returned.  
     // 


     Int_t linear= getSubLocation( &aLoc );  
            dataCurrent = data[linear];
            Int_t pos = modules[linear];
     
    #if DEBUG_LEVEL>2
        Warning("linear:%i module pos : %i  \
                 dataCurrent:%i operat[]: %i \n",linear, 
	        pos, dataCurrent, operator[](aLoc[nDimensions-1]) 
               );
     #endif   
           if((pos != -1) && dataCurrent && 
              (operator[](aLoc[nDimensions-1]) == 1) ){ 
	          pos+= bcount2( ((int) aLoc[nDimensions-1]) );
 
     #if DEBUG_LEVEL>2
        Warning(" bitcount()=> cell: %i position : %i \n", 
		  pos,aLoc[nDimensions-1]); 
     #endif 

                 return ( pos );
	   } 
          else return -1; 
  }


 inline void HBArray::set ( HLocation &aLoc ){
    Int_t linear= getSubLocation( &aLoc );  
    dataCurrent = data[linear];
 
     #if DEBUG_LEVEL>2
      Warning("sub linear:%i cell loc:%i \n",linear,aLoc[nDimensions-1]);
     #endif     

    set ( aLoc[nDimensions-1] ); 
  } 
 

 inline void HBArray::setMod( Int_t loc, Int_t i){
           modules[loc] = i; 
 }
  


 inline UInt_t HBArray::maxBytes(void) const{
   return (UInt_t) (( maxWords()* sizeof(*data) )/nSplit + 1 ) ; 
 }

 inline UInt_t HBArray::maxWords(void) const{
   UInt_t res = mBits / bitPerWord; 
   if(mBits % bitPerWord) ++res;
   return res;  
 }

 inline UInt_t  HBArray::bcount2(const size_t nsize )
 {
  // Compute the number of bits set to 1 
  // for within  the first 'size' elements
  // Using a Double_t look-up on the bit integer values.

  // register type should be automatically
  // set when using optimization ("-Ox") 
  // option for compiler

  // The bit array should be allocated with at least 'nsize'
  // elements



  register     UInt_t count; 
  register     size_t        i;     
  size_t       nchar = (nsize  / 8);       // borne inf. for searching 
  size_t       mask_index = (nsize  % 8);  // mask indexing 

  UChar_t *iter = (UChar_t*) dataCurrent; 

  // defining bit-lookup table  
  static const UInt_t bitcount[256] = {0, 1, 1, 2, 1, 2, 2, 3, 1,
        2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3,
        4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2,
        3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2,
        3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3,
        4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4,
        5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1,
        2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3,
        4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3,
        4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5,
        6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3,
        4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4,
        5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5,
        6, 6, 7, 6, 7, 7, 8};


    static UInt_t  bmask[8]={      0x0 ,
				   0x1,       
				   0x3,
				   0x7, 
				   0xf, 
				   0x1f, 
				   0x3f, 
				   0x7f};       



     // lookup table will speed this up a lot ! 
      for (count = 0, i = 0; i < nchar; i++) {
        count += bitcount[*(iter)];
        iter++; 
     }  

     #if DEBUG_LEVEL>2
      Warning(" counting nchar gives: %i  + adding: %i  iter:%i  \n", count,
                bitcount[( *(iter) & bmask[mask_index]) ], *iter  ) ; 
     #endif 

     count+=bitcount[( *(iter) & bmask[mask_index]) ];  
   return (count);
} // ! bcount2() 


#endif /* !HBARRAY_H */




