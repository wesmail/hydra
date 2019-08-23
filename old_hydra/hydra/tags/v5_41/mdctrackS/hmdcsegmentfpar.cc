# include "TArrayF.h"
# include "hades.h"
# include "hdetpario.h"
# include "hmdcdetector.h"
# include "hpario.h"
# include "hspectrometer.h"
# include "hmdcsegmentfpar.h"

ClassImp(HMdcSegmentFPar)

//*-- Author : Beatriz Fuentes Arenaz
//*-- Last updated : 16/3/2000 by Beatriz Fuentes(functions read and writeFile)
//*-- Last modified: 1/2/00
//*-- Modified : 31/05/99
//*-- Date: 31/05/99
//*-- Copyright : GENP (Univ. Santiago de Compostela)

//////////////////////////////////////////////////////////
//  HMdcSegmentFPar stores the parameters needed       //
//  by the HMdcLeverArmF class, the reconstructor of    //
//  one lever arm of the MDC system                     //
//////////////////////////////////////////////////////////


  HMdcSegmentFPar :: HMdcSegmentFPar(void) : HParSet(){
 
  fNSectors       = gHades->getSetup()->getDetector("Mdc")->getMaxSectors();
  Int_t nModules = gHades->getSetup()->getDetector("Mdc")->getMaxModules();
  fNLeverArms     = nModules/2;

  Int_t dim     = fNSectors*fNLeverArms;
  fCorrX         = new TArrayF(dim);
  fCorrY         = new TArrayF(dim);
  fErrDiffX      = new TArrayF(dim);
  fErrDiffXSlope = new TArrayF(dim);
  fErrDiffY      = new TArrayF(dim);
  fErrDiffYSlope = new TArrayF(dim);
  fAlpha         = new TArrayF(dim);

  fName  = "MdcSegmentFPar";
  fTitle = "parameter container for segment finding and fitting";
  strcpy(detName, "Mdc");
}



HMdcSegmentFPar :: ~HMdcSegmentFPar(void){
  
  if(fCorrX)         delete fCorrX;
  if(fCorrY)         delete fCorrY;
  if(fErrDiffX)      delete fErrDiffX;
  if(fErrDiffXSlope) delete fErrDiffXSlope;
  if(fErrDiffY)      delete fErrDiffY;
  if(fErrDiffYSlope) delete fErrDiffYSlope;
  if(fAlpha)         delete fAlpha;
}



Bool_t HMdcSegmentFPar :: init(HParIo* inp, Int_t* set){
 // initializes the container from an input

  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}



Int_t HMdcSegmentFPar :: write(HParIo* output){
  // writes the container to an output

  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}


 Int_t HMdcSegmentFPar::writeFile(fstream *pFile) {
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="###############################################################################\n"
               "# Parameters for Santiago tracking in one lever arm of the MDC\n"
               "# Format:\n"
               "# sector lever  corrX  corrY  errDiffX  errDiffY  errDiff errDiff    alpha \n"
               "#         arm                   (mm)     (mm)      XSlope  YSlope             \n"
               "###############################################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",GetName());
    pFile->write(buf,strlen(buf));
    Int_t index;

    for(Int_t s=0;s<fNSectors;s++) {
      for(Int_t l=0;l<fNLeverArms;l++) {
	index = getIndex(s,l);
        sprintf(buf,"%4i%8i%8.2f%7.2f%8.1f%10.1f%10.1f%10.1f%8.1f\n",
                s,l,fCorrX->At(index), fCorrY->At(index), 
		fErrDiffX->At(index), fErrDiffY->At(index), 
		fErrDiffXSlope->At(index), fErrDiffYSlope->At(index),
		fAlpha->At(index));
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


Bool_t HMdcSegmentFPar::read(fstream* pFile,Int_t* set, Int_t inputNumber) {
  if (pFile) {
    const Int_t maxbuf=1550;
    Text_t buf[maxbuf], key[20];
    Char_t c;
    Int_t sector, lever, pos1,pos2, module1, index;
    Float_t errDiffX, errDiffY, errDiffXSlope, errDiffYSlope;
    Float_t corrX, corrY, alpha;
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
              sscanf(buf,"%i%i%f%f%f%f%f%f%f",&sector,&lever,&corrX,&corrY,
                     &errDiffX,&errDiffY,&errDiffXSlope,&errDiffYSlope,&alpha);
	      if(lever==0) module1 = 0;
	      else module1 = 2;
              pos1 = sector*4+module1;
              pos2 = pos1 + 1;
              if (set[pos1] && set[pos2]){   
		index = getIndex(sector,lever);
		fCorrX->AddAt(corrX,index);
		fCorrY->AddAt(corrY,index);
                fErrDiffX->AddAt(errDiffX,index);
                fErrDiffY->AddAt(errDiffY,index);
                fErrDiffXSlope->AddAt(errDiffXSlope,index);
                fErrDiffYSlope->AddAt(errDiffYSlope,index);
		fAlpha->AddAt(alpha,index);
              }
            }
          }
        }
      }
    }
    if (all) {
      setInputVersion(1,inputNumber);
      setChanged();
      printf("MdcLeverArmFPar initialized from Ascii file\n");
      return kTRUE;
    }
  }
  return kFALSE;
} 



inline Float_t HMdcSegmentFPar :: getCorrX(Int_t sect, Int_t lever){
  return fCorrX->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getCorrY(Int_t sect, Int_t lever){
  return fCorrY->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getErrDiffX(Int_t sect, Int_t lever){
  return fErrDiffX->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getErrDiffXSlope(Int_t sect, Int_t lever){
  return fErrDiffXSlope->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getErrDiffY(Int_t sect, Int_t lever){
  return fErrDiffY->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getErrDiffYSlope(Int_t sect, Int_t lever){
  return fErrDiffYSlope->At(getIndex(sect,lever));
}

inline Float_t HMdcSegmentFPar :: getAlpha(Int_t sect, Int_t lever){
  return fAlpha->At(getIndex(sect,lever));
}




Bool_t HMdcSegmentFPar :: setCorrX(Int_t sect, Int_t lever, Float_t corr){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  Int_t pos = sect*fNLeverArms + lever ;
  fCorrX->AddAt(corr, pos);
  return kTRUE;
}


Bool_t HMdcSegmentFPar :: setCorrY(Int_t sect, Int_t lever, Float_t corr){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  Int_t pos = sect*fNLeverArms + lever ;
  fCorrY->AddAt(corr, pos);
  return kTRUE;
}


Bool_t HMdcSegmentFPar :: setErrDiffX(Int_t sect, Int_t lever, Float_t error){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  Int_t pos = sect*fNLeverArms + lever ;
  fErrDiffX->AddAt(error, pos);
  return kTRUE;
}


Bool_t HMdcSegmentFPar :: setErrDiffXSlope(Int_t sect, Int_t lever, Float_t error){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  Int_t pos = sect*fNLeverArms + lever ;
  fErrDiffXSlope->AddAt(error, pos);
  return kTRUE;
}


Bool_t HMdcSegmentFPar :: setErrDiffY(Int_t sect, Int_t lever, Float_t error){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  Int_t pos = sect*fNLeverArms + lever ;
  fErrDiffY->AddAt(error, pos);
  return kTRUE;
}


Bool_t HMdcSegmentFPar :: setErrDiffYSlope(Int_t sect, Int_t lever, Float_t error){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  Int_t pos = sect*fNLeverArms + lever ;
  fErrDiffYSlope->AddAt(error, pos);
  return kTRUE;
}

Bool_t HMdcSegmentFPar :: setAlpha(Int_t sect, Int_t lever, Float_t value){

  if ((sect<0) || (sect>=fNSectors)) return kFALSE;
  if ((lever<0)  || (lever>=fNLeverArms )) return kFALSE;

  Int_t pos = sect*fNLeverArms + lever ;
  fAlpha->AddAt(value, pos);
  return kTRUE;
}


void HMdcSegmentFPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMdcSegmentFPar.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
      R__b >> fNSectors;
      R__b >> fNLeverArms;
      fCorrX->Streamer(R__b);
      fCorrY->Streamer(R__b);
      fErrDiffX->Streamer(R__b);
      fErrDiffY->Streamer(R__b);
      fErrDiffXSlope->Streamer(R__b);
      fErrDiffYSlope->Streamer(R__b);
      fAlpha->Streamer(R__b);
   } else {
      R__b.WriteVersion(HMdcSegmentFPar::IsA());
      HParSet::Streamer(R__b);
      R__b << fNSectors;
      R__b << fNLeverArms;
      fCorrX->Streamer(R__b);
      fCorrY->Streamer(R__b);
      fErrDiffX->Streamer(R__b);
      fErrDiffY->Streamer(R__b);
      fErrDiffXSlope->Streamer(R__b);
      fErrDiffYSlope->Streamer(R__b);
      fAlpha->Streamer(R__b);
   }
}
