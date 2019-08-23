# include "hades.h"
# include "hdetpario.h"
# include "hmdcdetector.h"
# include "hmdcmodulegeometry.h"
# include "hmdcleverarmgeometry.h"
# include "hpario.h"
# include "hspectrometer.h"

ClassImp(HMdcLeverArmGeometry)

//*-- Author : Beatriz Fuentes Arenaz
//*-- Last updated : 16/3/2000 by Beatriz Fuentes(functions read and writeFile)
//*-- Modified : 8/11/99
//*-- Date: 11/98
//*-- Copyright : GENP (Univ. Santiago de Compostela)


///////////////////////////////////////////////////////////////////
//
// HMdcLeverArmGeometry
// container for the geometrical parameters of the lever arm. This 
// is a temporal container, since the general structure of HADES
// geometry has not been designed yet.  
//
///////////////////////////////////////////////////////////////////


  HMdcLeverArmGeometry :: HMdcLeverArmGeometry(void) : HParSet(){
    // default constructor

  fNSectors = gHades->getSetup()->getDetector("Mdc")->getMaxSectors();
  Int_t nModules = gHades->getSetup()->getDetector("Mdc")->getMaxModules();
  fNLeverArms = nModules/2;

  Int_t dim = fNSectors*fNLeverArms;

  fCosn = new TArrayF(dim);
  fSinn = new TArrayF(dim);
  fTraslation = new TArrayF(3*dim);

  fCosS = new TArrayF(dim);
  fSinS = new TArrayF(dim);
  fTraslationS = new TArrayF(3*dim);

  fName = "MdcLeverArmGeometry";
  fTitle = "geometry container for the mdc reconstruction in one lever arm";
  strcpy(detName, "Mdc");

}

HMdcLeverArmGeometry :: ~HMdcLeverArmGeometry(void){
  // default destructor
  
  if(fCosn) delete fCosn;
  if(fSinn) delete fSinn;
  if(fTraslation) delete fTraslation;
  if(fCosS) delete fCosS;
  if(fSinS) delete fSinS;
  if(fTraslationS) delete fTraslationS;
}


Bool_t HMdcLeverArmGeometry :: init(HParIo* inp, Int_t* set){
 // initializes the container from an input

  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}



Int_t HMdcLeverArmGeometry :: write(HParIo* output){
  // writes the container to an output

  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}



 Int_t HMdcLeverArmGeometry::writeFile(fstream *pFile) {
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="###############################################################################\n"
               "# Parameters for Santiago tracking in one lever arm of the MDC\n"
               "# Format:\n"
               "# sector lever    cosn    sinn      t1     t2       t3   MDC1-MDC2 / MDC3-MDC4\n"
               "#         arm                      (mm)   (mm)     (mm)\n"
               "#                 cosn    sinn      t1     t2       t3   MDC2-SEC / MDC4-SEC\n"
               "#                                  (mm)   (mm)     (mm)\n"     
               "###############################################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",GetName());
    pFile->write(buf,strlen(buf));

    Int_t index;
    for(Int_t s=0;s<fNSectors;s++) {
      for(Int_t l=0;l<fNLeverArms;l++) {
	index = getIndex(s,l);

        sprintf(buf,"%5i%6i%11.6f%10.5f%7.0f%10.3f%11.3f%41.6f%11.6f%6.0f%11.3f%10.3f\n",
                s,l,fCosn->At(index), fSinn->At(index), 
		fTraslation->At(index*3),fTraslation->At(index*3 + 1),
		fTraslation->At(index* 3 +2),
		fCosS->At(index), fSinS->At(index), 
		fTraslationS->At(index*3),fTraslationS->At(index*3 + 1),
		fTraslationS->At(index* 3 +2));
        pFile->write(buf,strlen(buf));
      }
    }
    sprintf(buf,"###############################################################################\n");
    pFile->write(buf,strlen(buf));
    setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}  


Bool_t HMdcLeverArmGeometry::read(fstream *pFile,Int_t* set,Int_t inputNumber) {
  if (pFile) {
    const Int_t maxbuf=155;
    Text_t buf[maxbuf], key[25];
    Char_t c;
    Int_t sector, lever, pos1,pos2, module1, index;
    Float_t cosn, sinn, tras0, tras1, tras2;
    Float_t coss, sins, trass0, trass1, trass2;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        if (strstr(buf,GetName())!=0) {
          while (!all && !pFile->eof()) {
            c=pFile->get();
            if (c=='#' || c=='/') all=kTRUE;
            else {
              pFile->putback(c);
              pFile->getline(buf,maxbuf);
              sscanf(buf,"%i%i%f%f%f%f%f%f%f%f%f%f",&sector,&lever,
                     &cosn,&sinn,&tras0,&tras1,&tras2,
		     &coss,&sins,&trass0,&trass1,&trass2);
	      if(lever==0) module1 = 0;
	      else module1 = 2;
              pos1 = sector*4+module1;         
	      pos2 = pos1 + 1;       
              if (set[pos1] && set[pos2]){   
		index = getIndex(sector,lever);
                fCosn->AddAt(cosn,index);
                fSinn->AddAt(sinn,index);
                fTraslation->AddAt(tras0,index*3);
                fTraslation->AddAt(tras1,index*3 +1);
                fTraslation->AddAt(tras2,index*3 +2);
		fCosS->AddAt(coss,index);
                fSinS->AddAt(sins,index);
                fTraslationS->AddAt(trass0,index*3);
                fTraslationS->AddAt(trass1,index*3 +1);
                fTraslationS->AddAt(trass2,index*3 +2);
		
              }
            }
          }
        }
      }
    }
    if (all) {
      setInputVersion(1,inputNumber);
      setChanged();
      printf("MdcLeverArmGeometry initialized from Ascii file\n");
      return kTRUE;
    }
  }
  return kFALSE;
} 



inline Float_t HMdcLeverArmGeometry :: getCosn(Int_t sect, Int_t lever){
  return fCosn->At(getIndex(sect,lever));
}

inline Float_t HMdcLeverArmGeometry :: getSinn(Int_t sect, Int_t lever){
  return fSinn->At(getIndex(sect,lever));
}

inline Float_t HMdcLeverArmGeometry :: getTrasElement(Int_t sect, Int_t lever, Int_t index){
  return fTraslation->At(getIndex(sect,lever)*3+index);
}

inline Float_t HMdcLeverArmGeometry :: getCosS(Int_t sect, Int_t lever){
  return fCosS->At(getIndex(sect,lever));
}

inline Float_t HMdcLeverArmGeometry :: getSinS(Int_t sect, Int_t lever){
  return fSinS->At(getIndex(sect,lever));
}

inline Float_t HMdcLeverArmGeometry :: getTrasElementS(Int_t sect, Int_t lever, Int_t index){
  return fTraslationS->At(getIndex(sect,lever)*3+index);
}

Bool_t HMdcLeverArmGeometry :: setCosn(Int_t sect, Int_t lever, Float_t cos){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  Int_t pos = sect*fNLeverArms + lever ;
  fCosn->AddAt(cos, pos);
return kTRUE;
}


Bool_t HMdcLeverArmGeometry :: setSinn(Int_t sect, Int_t lever, Float_t sin){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  Int_t pos = sect*fNLeverArms + lever ;
  fSinn->AddAt(sin, pos);
return kTRUE;
}


Bool_t HMdcLeverArmGeometry :: setTrasElement(Int_t sect, Int_t lever, Int_t index, Float_t element){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  fTraslation->AddAt(element, getIndex(sect,lever)*3 + index);
return kTRUE;
}

Bool_t HMdcLeverArmGeometry :: setCosS(Int_t sect, Int_t lever, Float_t cos){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  Int_t pos = sect*fNLeverArms + lever ;
  fCosS->AddAt(cos, pos);
return kTRUE;
}


Bool_t HMdcLeverArmGeometry :: setSinS(Int_t sect, Int_t lever, Float_t sin){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  Int_t pos = sect*fNLeverArms + lever ;
  fSinS->AddAt(sin, pos);
return kTRUE;
}


Bool_t HMdcLeverArmGeometry :: setTrasElementS(Int_t sect, Int_t lever, Int_t index, Float_t element){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  fTraslationS->AddAt(element, getIndex(sect,lever)*3 + index);
return kTRUE;
}

void HMdcLeverArmGeometry::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMdcLeverArmGeometry.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
      R__b >> fNSectors;
      R__b >> fNLeverArms;
      fCosn->Streamer(R__b);
      fSinn->Streamer(R__b);
      fTraslation->Streamer(R__b);  
      fCosS->Streamer(R__b);
      fSinS->Streamer(R__b);
      fTraslationS->Streamer(R__b);
   } else {
      R__b.WriteVersion(HMdcLeverArmGeometry::IsA());
      HParSet::Streamer(R__b);
      R__b << fNSectors;
      R__b << fNLeverArms;
      fCosn->Streamer(R__b);
      fSinn->Streamer(R__b);
      fTraslation->Streamer(R__b);
      fCosS->Streamer(R__b);
      fSinS->Streamer(R__b);
      fTraslationS->Streamer(R__b);
      }
}
   
