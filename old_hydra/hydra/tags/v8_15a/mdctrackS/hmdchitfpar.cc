# include "hades.h"
# include "hdebug.h"
# include "hdetpario.h"
# include "hmdcdetector.h"
# include "hmdchitfpar.h"
# include "hspectrometer.h"
ClassImp(HMdcHitFPar)


//*-- Author : Beatriz Fuentes Arenaz
//*-- Last updated : 17/01/2002 by I. Koenig
//*-- Last updated : 16/3/2000 by Beatriz Fuentes(functions read and writeFile)
//*-- Last updated : 13/5/99 by Beatriz Fuentes
//*-- Date: 15/3/99
//*-- Copyright : GENP (Univ. Santiago de Compostela)


//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////
//
//  HMdcHitFPar
//
// class to store parameters for the reconstruction of mdc data in one 
// module. This class provides methods to access and set these parameters.
//
//////////////////////////////////////////////////////////////////////



HMdcHitFPar :: HMdcHitFPar(const char* name,const char* title,
                           const char* context)
            : HParSet(name,title,context) {
     // gets number of sectors and modules from setup and allocate memory

  fNSectors = gHades->getSetup()->getDetector("Mdc")->getMaxSectors();
  fNModules = gHades->getSetup()->getDetector("Mdc")->getMaxModules();

  fMinNumHits       = new TArrayI(fNSectors*fNModules);
  fMaxNumCommonHits = new TArrayI(fNSectors*fNModules);
  fMaxChi           = new TArrayF(fNSectors*fNModules);
  fRoad             = new TArrayF(fNSectors*fNModules);
  fMaxSlopeX        = new TArrayF(fNSectors*fNModules);
  fMinSlopeX        = new TArrayF(fNSectors*fNModules);
  fMaxSlopeY        = new TArrayF(fNSectors*fNModules);
  fMinSlopeY        = new TArrayF(fNSectors*fNModules);
  fDistZAxis        = new TArrayF(fNSectors*fNModules);
  fZHit             = new TArrayF(fNSectors*fNModules);

  strcpy(detName, "Mdc");
 
}


HMdcHitFPar :: ~HMdcHitFPar(void){

  if(fMinNumHits)       delete fMinNumHits;
  if(fMaxNumCommonHits) delete fMaxNumCommonHits;
  if(fMaxChi)    delete fMaxChi;
  if(fRoad)      delete fRoad;
  if(fMaxSlopeX) delete fMaxSlopeX;
  if(fMinSlopeX) delete fMinSlopeX;
  if(fMaxSlopeY) delete fMaxSlopeY;
  if(fMinSlopeY) delete fMinSlopeY;
  if(fDistZAxis) delete fDistZAxis;
  if(fZHit)      delete fZHit;

}



Bool_t HMdcHitFPar :: init(HParIo* inp, Int_t* set){
  // initializes the container from an input

  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}


 Int_t HMdcHitFPar::write(HParIo* output) {
  // writes the container to an output

  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}


 Int_t HMdcHitFPar::writeFile(fstream* pFile) {
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="###############################################################################\n"
               "# Reconstruction parameters for Santiago tracking of the MDC\n"
               "# Format:\n"
               "# sect mod  minNum  maxNum   maxChi road  max    min    max    min  dist zHit\n"
               "#            Hits CommonHits             SlopeX SlopeX SlopeY SlopeY ZAxis\n"
               "###############################################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",GetName());
    pFile->write(buf,strlen(buf));
    Int_t pos;
    for(Int_t s=0;s<fNSectors;s++) {
      for(Int_t m=0;m<fNModules;m++) {
	pos = getIndex(s,m);
        sprintf(buf,"%3i%5i%7i%8i%11.3f%6.3f%5.2f%7.2f%7.2f%7.2f%6.2f%6.2f\n",
                s,m,fMinNumHits->At(pos), fMaxNumCommonHits->At(pos), 
		fMaxChi->At(pos), fRoad->At(pos), fMaxSlopeX->At(pos), 
		fMinSlopeX->At(pos), fMaxSlopeY->At(pos), fMinSlopeY->At(pos), 
		fDistZAxis->At(pos), fZHit->At(pos));
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



Bool_t HMdcHitFPar::read(fstream* pFile, Int_t* set, Int_t inputNumber) {
  if (pFile) {
    const Int_t maxbuf=1550;
    Text_t buf[maxbuf], key[20];
    Char_t c;
    Int_t sector, module, pos, index;
    Int_t minNumHits, maxNumCommonHits;
    Float_t maxChi, road, maxSlopeX, minSlopeX, maxSlopeY, minSlopeY;
    Float_t distZAxis, zHit;
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
              sscanf(buf,"%i%i%i%i%f%f%f%f%f%f%f%f",
                   &sector,&module,&minNumHits, &maxNumCommonHits, &maxChi,
                   &road, &maxSlopeX, &minSlopeX, &maxSlopeY, &minSlopeY,
                   &distZAxis, &zHit);
              pos=sector*4+module;
              if (set[pos]) {
		index = getIndex(sector,module);
		fMinNumHits->AddAt(minNumHits, index);
		fMaxNumCommonHits->AddAt(maxNumCommonHits,index);
		fMaxChi->AddAt(maxChi, index);
		fRoad->AddAt(road,index);
		fMaxSlopeX->AddAt(maxSlopeX,index);
		fMinSlopeX->AddAt(minSlopeX,index);
		fMaxSlopeY->AddAt(maxSlopeY,index);
		fMinSlopeY->AddAt(minSlopeY,index);
		fDistZAxis->AddAt(distZAxis,index);
		fZHit->AddAt(zHit,index);
              }
            }
          }
        }
      }
    }
    if (all) {
      setInputVersion(1,inputNumber);
      setChanged();
      printf("MdcSegmentFPar initialized from Ascii file\n");
      return kTRUE;
    }
  }
  return kFALSE;
} 





Bool_t HMdcHitFPar :: setMinNumHits(Int_t nSect, Int_t nMod, Int_t num){

  if ((nSect<0) || (nSect>=fNSectors)) return kFALSE;
  if ((nMod<0)  || (nMod>=fNModules )) return kFALSE;

  Int_t pos = nSect*fNModules + nMod ;
  fMinNumHits->AddAt(num, pos);
return kTRUE;
}


Bool_t HMdcHitFPar :: setMaxNumCommonHits(Int_t nSect, Int_t nMod, Int_t num){

  if ((nSect<0) || (nSect>=fNSectors)) return kFALSE;
  if ((nMod<0)  || (nMod>=fNModules )) return kFALSE;

  Int_t pos = nSect*fNModules + nMod ;
  fMaxNumCommonHits->AddAt(num, pos);
return kTRUE;
}


Bool_t HMdcHitFPar :: setMaxChi(Int_t nSect, Int_t nMod, Float_t chi){

  if ((nSect<0) || (nSect>=fNSectors)) return kFALSE;
  if ((nMod<0)  || (nMod>=fNModules )) return kFALSE;

  Int_t pos = nSect*fNModules + nMod ;
  fMaxChi->AddAt(chi, pos);
return kTRUE;
}


Bool_t HMdcHitFPar :: setRoad(Int_t nSect, Int_t nMod, Float_t dist){

  if ((nSect<0) || (nSect>=fNSectors)) return kFALSE;
  if ((nMod<0)  || (nMod>=fNModules )) return kFALSE;

  Int_t pos = nSect*fNModules + nMod ;
  fRoad->AddAt(dist, pos);
return kTRUE;
}


Bool_t HMdcHitFPar :: setMaxSlopeX(Int_t nSect, Int_t nMod, Float_t slope){

  if ((nSect<0) || (nSect>=fNSectors)) return kFALSE;
  if ((nMod<0)  || (nMod>=fNModules )) return kFALSE;

  Int_t pos = nSect*fNModules + nMod ;
  fMaxSlopeX->AddAt(slope, pos);
return kTRUE;
}


Bool_t HMdcHitFPar :: setMinSlopeX(Int_t nSect, Int_t nMod, Float_t slope){

  if ((nSect<0) || (nSect>=fNSectors)) return kFALSE;
  if ((nMod<0)  || (nMod>=fNModules )) return kFALSE;

  Int_t pos = nSect*fNModules + nMod ;
  fMinSlopeX->AddAt(slope, pos);
return kTRUE;
}


Bool_t HMdcHitFPar :: setMaxSlopeY(Int_t nSect, Int_t nMod, Float_t slope){

  if ((nSect<0) || (nSect>=fNSectors)) return kFALSE;
  if ((nMod<0)  || (nMod>=fNModules )) return kFALSE;

  Int_t pos = nSect*fNModules + nMod ;
  fMaxSlopeY->AddAt(slope, pos);
return kTRUE;
}


Bool_t HMdcHitFPar :: setMinSlopeY(Int_t nSect, Int_t nMod, Float_t slope){

  if ((nSect<0) || (nSect>=fNSectors)) return kFALSE;
  if ((nMod<0)  || (nMod>=fNModules )) return kFALSE;

  Int_t pos = nSect*fNModules + nMod ;
  fMinSlopeY->AddAt(slope, pos);
return kTRUE;
}



Bool_t HMdcHitFPar :: setDistZAxis(Int_t nSect, Int_t nMod, Float_t distance){

  if ((nSect<0) || (nSect>=fNSectors)) return kFALSE;
  if ((nMod<0)  || (nMod>=fNModules )) return kFALSE;

  Int_t pos = nSect*fNModules + nMod ;
  fDistZAxis->AddAt(distance, pos);
return kTRUE;
}


Bool_t HMdcHitFPar :: setZHit(Int_t nSect, Int_t nMod, Float_t z){

  if ((nSect<0) || (nSect>=fNSectors)) return kFALSE;
  if ((nMod<0)  || (nMod>=fNModules )) return kFALSE;

  Int_t pos = nSect*fNModules + nMod ;
  fZHit->AddAt(z, pos);
return kTRUE;
}

void HMdcHitFPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMdcHitFPar.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
      R__b >> fNSectors;
      R__b >> fNModules;
      fMinNumHits->Streamer(R__b);
      fMaxNumCommonHits->Streamer(R__b);
      fMaxChi->Streamer(R__b);
      fRoad->Streamer(R__b);
      fMaxSlopeX->Streamer(R__b);
      fMinSlopeX->Streamer(R__b);
      fMaxSlopeY->Streamer(R__b);
      fMinSlopeY->Streamer(R__b);
      fDistZAxis->Streamer(R__b);
      fZHit->Streamer(R__b);
   } else {
      R__b.WriteVersion(HMdcHitFPar::IsA());
      HParSet::Streamer(R__b);
      R__b << fNSectors;
      R__b << fNModules;
      fMinNumHits->Streamer(R__b);
      fMaxNumCommonHits->Streamer(R__b);
      fMaxChi->Streamer(R__b);
      fRoad->Streamer(R__b);
      fMaxSlopeX->Streamer(R__b);
      fMinSlopeX->Streamer(R__b);
      fMaxSlopeY->Streamer(R__b);
      fMinSlopeY->Streamer(R__b);
      fDistZAxis->Streamer(R__b);
      fZHit->Streamer(R__b);

   }
}

