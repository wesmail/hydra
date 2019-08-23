#include "hbarray.h"
#include <iostream.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>


//-- Author : Denis Bertini <dbertini@gsi.de>
//-- Modified : 19/09/00 by  D. Bertini
//


//----------------------------------------------------------------------------
// Bit array class 
//
// Implements functions to create and manipulate array of bits. Functions
// to: dynamically create arrays, access (read and write) elements at 
// a particular position, are provided. 
//    
// 'Bits' are represented using  unsigned integer type, but minor changes
// are required to change this particular representation to other 
// basic type such as unsigned char. 
// The 'bit fields' are splitted according to the structure defined by
// the caller (link to the category setup). This splitting in sub-bitfields 
// is done using a pointer array like structure.
//
// N.B: the bit array can not be a replacement of the standard library
// <bitset> ect.. :  
// This class is mainly used to create the bit-based lookup table needed for
// random access of HDataObjects in the HBitFieldCategory container. 
// For that purpose, efficient additional mathematical functions 
// (complement, number of set bits)  have also been implemented. 
// Nevertheless, the extension of the bit array interface to a more
// complete/general one is of course possible 
//  
//
// typedef names used 
//     => including  definitions of the following 3 types
//              elem_t = a number (used as a count, i.e. never < 0,
//                                throughout).
//               bit  = an unsigned integer
//                      + If this is not unsigned char then the #define
//                        of BITS_SZ as CHAR_BIT should be changed.
//                      + If this is not internally represented by 8
//                         bits then the lookup table in the methods
//                               int  HBArray::bcount() 
//                               int  HBArray::bcount2()
//                         must be replaced.
// 
//   
// 
//----------------------------------------------------------------------------

ClassImp(HBArray)


//  macro NELEM()
//
//    The number of elements, nelem, in an array of N bits can be
//  computed using the formula:
//
//    if (0 == (N % BITS_SZ))
//       nelem = N/BITS_SZ
//    else
//       nelem = N/BITS_SZ + 1

//    This can be represented in any of these ways:
//      nelem = N/(BITS_SZ)  +  1 - (0 == (N %(BITS_SZ)))
//      nelem = N/(BITS_SZ)  +  !(0 == (N %(BITS_SZ)))
//      nelem = N/(BITS_SZ)  +  (0 != (N %(BITS_SZ)))


#define NELEM(N,ELEMPER)  (( N +  (ELEMPER) - 1) / (ELEMPER))



//* 
// Definition of BITS_SZ
// (BITS_SZ is the number of bits in a single 'bits' type 
//*

#ifdef CHAR_BIT
  // assumes typedef unsigned char bits 
   #define BITS_SZ (CHAR_BIT)

#else
   static elem_t bits_size(void);
   elem_t BITS_SZ = 0;  
   static elem_t bits_size(void) {
   elem_t i;
   bit    v = (bit)~0;
      for (i=1; (v = v >> 1) > 0; i++)
	; // empty 
      return (i);
   }
#endif  





#ifdef CHAR_BIT
UInt_t HBArray::bitPerWord=CHAR_BIT*sizeof(UInt_t); //std : 32 bit based
#endif

UInt_t** HBArray::data=NULL;
Int_t  HBArray::init_count=0;
Bool_t HBArray::caching=kFALSE;
Int_t* HBArray::indexes=NULL;
Int_t* HBArray::modules=NULL;
Int_t* HBArray::lSizes=NULL;
Int_t  HBArray::nDimensions=0;
Int_t  HBArray::nWords=0;
Int_t  HBArray::nSplit=0;
Bool_t HBArray::S_setup=kFALSE;
 
const Int_t BitSet    = 1;
const Int_t BitNotSet = 0;
const UInt_t BIT_MASK = 1;


HBArray::HBArray(void)
{
 dataCurrent=0;
 offset=0; 
 ++init_count;
}

HBArray::HBArray(UInt_t lenght)
{

 //  Dynamically allocate memory for an 
 //  array of 'nelements' defined as  bitPerWord*maxWord()

 ++init_count; 
 offset=0;
 modules = new Int_t[nSplit];
 mBits = lenght; 
 nWords = maxWords(); 
 int cap = (int) ((nWords/nSplit) +1) ; 
 // full setup ... later dynamically created !
 dataCurrent = 0; 
  data = new( UInt_t* [nSplit]);
 for (int i=0;i<nSplit;i++){
    data[i] = new UInt_t[cap];
    if (!data[i]) {
        // later exception thrown
        Error("HBArray","failed to allocate bit array"); 
        exit(1); 
    }
 }
 
}


HBArray::~HBArray(void)
{
   --init_count; 

     if ( data && init_count <0) {
       for (Int_t i=0;i<nSplit;i++) delete [] data[i];
       delete [] data;
       if (modules) delete [] modules;
     }
}

void HBArray::setDimensions(Int_t nDim,Int_t *sizes){
 Int_t prod=1,nsplit=1; 
 for (Int_t i=0;i<nDim-1;i++) { nsplit*=sizes[i];}
 for (Int_t i=0;i<nDim;i++) { prod*=sizes[i];}
#if DEBUG_LEVEL>2
  Warning(" nsplit:%i prod:%i \n", nsplit,prod); 
#endif
 nWords=prod/bitPerWord; 
 nSplit=nsplit;
 lSizes = new Int_t[nDim];  
 for (Int_t i=0;i<nDim;i++) lSizes[i]=sizes[i];
 nDimensions=nDim;
 setCapacity( nWords , nSplit); 
}


void HBArray::setCapacity( Int_t nword, Int_t nsplit)
{
 if( data  &&  modules ){ 
           mBits=nWords*bitPerWord;
           clear();

 #if DEBUG_LEVEL>2 
	   Warning("setCapicity => clearing : \n");
 #endif

 }else{

 #if DEBUG_LEVEL>2       
           Warning("setCapicity => new : data:%i modules:%i \n",data,modules);
 #endif
        modules = new Int_t[nsplit]; 
        mBits=nWords*bitPerWord; 
	 // for the moment full setup !!! 
        int size = (int) ((nWords/nsplit) +1); 
        data = new( UInt_t* [nsplit]); 
        for (int i=0;i<nsplit;i++) data[i]= new UInt_t[size];
        clear();    
}
 
 if (!(data || modules) ) {
   // later exception thrown
   Error("HBArray","failed to allocate bit array"); 
   exit(1); 
 }
}

void HBArray::set(UInt_t pos)
{
  // set a bit to 1 at a position
  // in the bit-field
 
  if(pos >= mBits || pos < 0){
    Error("HBArray::set( Int_t )"," out of range at pos:%i for maxBits:%i ", pos,mBits);
   exit(1); 
  }
 UInt_t word = pos / bitPerWord; 
 UInt_t bit = pos % bitPerWord; 
 if (!dataCurrent) exit(1);
 dataCurrent[word] |= (BIT_MASK << bit); 
}


void HBArray::clear(UInt_t pos)
{
  // clear a bit at a position
 if(pos>=mBits) {
     Error("HBArray::clear"," out of range at pos:%i for maxBits:%i ", pos,mBits); 
     exit(1); 
  }

 UInt_t word = pos / bitPerWord; 
 UInt_t bit = pos % bitPerWord; 
 dataCurrent[word] &= ~(BIT_MASK << bit); 

}

void HBArray::set()
{
  // set all bitfield with the cte BitSet
for (int i=0;i<nSplit;i++) memset(data[i],BitSet,maxBytes()); 
}

void HBArray::clear()
{
 // set all bitfield with the cte Byte BitNotSet
 int size = (int) ((nWords/nSplit) +1)*4;  
#if DEBUG_LEVEL>2 
  Warning("******** dump before clear called  ************* \n");
  dump();
#endif
 for (int i=0;i<nSplit;i++){
        memset(data[i],BitNotSet,size);
                       modules[i]=-1;
#if DEBUG_LEVEL>2 
  Warning(" index:%i  %i mod:%i \n", i, data[i], modules[i]);  
#endif

 } 

#if DEBUG_LEVEL>2 
 Warning("******** dump after clear called  ************* \n");
 dump();
#endif
}

void HBArray::dump(void)
{
   
  // Dump the content of bit field on screen
  // This function should be used for debugging
  // purpose only
 const UInt_t displayWidht = 80; 
  //system("clear"); 

 for (int j=0; j<nSplit ; j++){
    dataCurrent = data[j]; 
    cout << "***************************** module ***********: "<< j << endl;
   int nbit = mBits/nSplit;  
  for(Int_t i = 0; i < nbit ; i++){
    if (!(i% displayWidht)) cout << endl; 
    if(operator[](i)) cout << "x";
    else 
      cout << "."; 
  }
  cout << endl; 
 }
}

Int_t HBArray::operator[]( UInt_t pos)
{
  if( pos>= mBits ){
    Error("HBArray "," out of range at pos:%i for maxBits:%i ", pos,mBits); 
     exit(1); 
   }

  UInt_t word = pos / bitPerWord; 
  UInt_t bit =  pos % bitPerWord; 
  
  if( dataCurrent[word] & ((BIT_MASK) << bit )){  
       return BitSet;
  }else
    return BitNotSet;              
					    
}         

Int_t HBArray::getNBitSet ( size_t  upto)
{
  // Get the Number of bit set using internally
  // a call to bcount()
  // if the DataObject does not exit a negative
  // value (-1) is returned
  

if (operator[](upto) == 0) return -1; 

size_t        rchar = (upto % 8);
size_t        lchar =  upto - rchar;
int count = (int) bcount (upto); 
for (int i = lchar+1; i <= (int) upto; i++) {
        if (operator[](i)) {
           count++;
        }
 }

return (count); 
}



UInt_t  HBArray::bcount(const size_t nsize )
{
 // Compute the number of 1 bits in the first 'size' elements
 // using single lookup to the bit integer value.

 // register type should be automatically 
 // set when using optimization ("-Oxx")
 // option for the gnu compiler

 // The bit array should be allocated with at least 'nsize'
 // elements




  register     unsigned int count; 
  register     size_t        i;     
  size_t        nchar = ( nsize  / (8));
  unsigned char *iter = (unsigned char*) dataCurrent; 

 
  static const UInt_t bitcount[256] = {0, 1, 1, 2, 1, 2, 2, 3, 1, \
        2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, \
        4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, \
        3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, \
        3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, \
        4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, \
        5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, \
        2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, \
        4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, \
        4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, \
        6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, \
        4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, \
        5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, \
        6, 6, 7, 6, 7, 7, 8};

   if (bitcount[(sizeof bitcount / sizeof bitcount[0]) - 1] == 8 ) {
     // lookup table will speed this up a lot  

     for (count = 0, i = 0; i < nchar; i++) {
        count += bitcount[*(iter)];
        iter++; 
     }      
   } else {
     // worst case ! 
     for (count = 0, i = 0; i < nsize; i++) {
        if (operator[](i)) {
           count++;
        }
     }
   }
   return (count);
} // ! count() 


void HBArray::complement(const size_t nsize ){

// the bit array is composed of *exactly* `size' elements.
// All the bits in the array have been flipped 

  size_t nelem = NELEM(nsize,(BITS_SZ));
  register size_t i;
  for (i=0; i < nelem; i++) {
      dataCurrent[i] = ~(dataCurrent[i]);
   }
}



void HBArray::release(){
  
  //  printf( " init_count value  %i \n",init_count); 
  //  printf(" releasing static storage \n");

        for(Int_t i=0;i<nSplit;i++) {
            if( data[i] ) delete [] data[i];
     }  
     if (data) delete [] data;
     if (modules) delete [] modules;
}


void HBArray::Streamer(TBuffer & R__b) {

  if (R__b.IsReading()) {

    TObject::Streamer(R__b);
    R__b >> nWords;
    R__b >> nSplit; 

    setCapacity(nWords,nSplit);
    if (nWords>0) {
      int cap = (int) ((nWords/nSplit) + 1);
      for(int i=0;i<nSplit;i++) 
      R__b.ReadFastArray( (UInt_t*) data[i], cap );
      R__b.ReadFastArray( &modules[0],nSplit);
      
      if(!lSizes) {
      R__b >> nDimensions;   
      lSizes = new Int_t[nDimensions]; 
      R__b.ReadFastArray(  &lSizes[0], nDimensions);
         } 
      
    }
    
  } else {
          
    TObject::Streamer(R__b);
    
    R__b << nWords;
    R__b << nSplit; 
    if (nWords>0) {
     int cap = (int) ((nWords/nSplit) + 1);
      for(int i=0;i<nSplit;i++) 
      R__b.WriteFastArray( (UInt_t*) data[i] ,cap);
      R__b.WriteFastArray(  &modules[0], nSplit);
 
    R__b << nDimensions;   
    R__b.WriteFastArray(  &lSizes[0], nDimensions);  
   
    }    
  }
}



void HBArray::Streamer_sparse(TBuffer & R__b) {
// alternative streamer function
// when using sparse data      
  char nch0=0, nch1=1, nch;
  Int_t module=-1;   

  if (R__b.IsReading()) {
    TObject::Streamer(R__b);

    if(!S_setup){
      printf(" HBarray:: reading setup \n");  
     R__b >> nWords;
     R__b >> nSplit; 
     if(!lSizes) {
       R__b >> nDimensions;   
       lSizes = new Int_t[nDimensions]; 
       R__b.ReadFastArray(  &lSizes[0], nDimensions);
     } 
     //     printf(" after setup nW:%i nS:%i \n",nWords,nSplit);
     //     printf(" allocate bit array \n"); 
     S_setup=kTRUE; 
    }
 
    //    printf(" nW:%i nS:%i \n",nWords,nSplit);
 
     setCapacity(nWords,nSplit);
    if (nWords>0) {
      int cap = (int) ((nWords/nSplit) + 1);
      for(int i=0;i<nSplit;i++){
         R__b >> nch; 
	 // module[i]=-1; 
       if( nch == 1 ){
	 // printf(" found module at pos:%i dev:%i ptr data \n ",i,module,data[i]);
         R__b >> module; 
         modules[i] = module;  
	 R__b.ReadFastArray( (UInt_t*) data[i], cap );
       }   
      }
    }
  } else {          

    TObject::Streamer(R__b);

    if( ! S_setup ) {    
    printf(" HBarray:: writing setup \n");   
    R__b << nWords;
    R__b << nSplit;
    R__b << nDimensions;   
    R__b.WriteFastArray(  &lSizes[0], nDimensions);
    S_setup = kTRUE;   
    } 
 
    if (nWords>0) {
     int cap = (int) ((nWords/nSplit) + 1);
     for(int i=0;i<nSplit;i++){
	// test sparse filling of TBuffer  
	if( modules[i]==-1) {
        
	  R__b << nch0; continue; 
       }else{
	 //	  printf(" module found for split:%i \n",i);
          R__b << nch1; 
          R__b << modules[i]; 
          R__b.WriteFastArray( (UInt_t*) data[i] ,cap);
       }
     }
    }    
  }
}


