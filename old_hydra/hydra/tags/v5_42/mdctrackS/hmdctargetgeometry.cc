# include "hades.h"
# include "hdetpario.h"
# include "hmdcdetector.h"
# include "hspectrometer.h"
# include "hmdctargetgeometry.h"

ClassImp(HMdcTargetGeometry)

//*-- Author : Beatriz Fuentes Arenaz
//*-- Last updated : 16/3/2000 by Beatriz Fuentes(functions read and writeFile)
//*-- Modified : 31/05/99
//*-- Copyright : GENP (Univ. Santiago de Compostela)

//////////////////////////////////////////////////////////
//  HMdcTargetGeometry stores the geometry of the target.  //
//  It will change soon, since the general geometry     //
//  structure has not been completely designed.         //

//////////////////////////////////////////////////////////


  HMdcTargetGeometry :: HMdcTargetGeometry(void) : HParSet(){
  
  fNSectors = gHades->getSetup()->getDetector("Mdc")->getMaxSectors();
  fNModules = gHades->getSetup()->getDetector("Mdc")->getMaxModules();

  fXDim = fYDim = 0;
  fZPos = new TArrayF(fNSectors*fNModules);

  fName = "MdcTargetGeometry";
  fTitle = "target geometry for mdc reconstruction";
  strcpy(detName, "Mdc");
}



HMdcTargetGeometry :: ~HMdcTargetGeometry(void){
  if (fZPos) delete fZPos;
}



Bool_t HMdcTargetGeometry :: init(HParIo* inp, Int_t* set){
  // initializes the container from an input

  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}



Int_t HMdcTargetGeometry :: write(HParIo* output){
  // writes the container to an output

  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}


 Int_t HMdcTargetGeometry::writeFile(fstream *pFile) {
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="##############################################################################\n"
               "# Target geometrical parameters for Santiago tracking of the MDC\n"
               "# Format:\n"
               "# xdim   ydim             sector module zpos\n"
               "# (mm)   (mm)                           (mm)\n"
               "################################################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",GetName());
    pFile->write(buf,strlen(buf));
    sprintf(buf,"%7.2f%7.2f\n", fXDim, fYDim);
    pFile->write(buf,strlen(buf));

    Int_t index;
    for(Int_t s=0;s<fNSectors;s++) {
      for(Int_t m=0;m<fNModules;m++) {
	index = getIndex(s,m);
        sprintf(buf,"\t\t\t%4i%8i%15.8f\n",s,m,fZPos->At(index));
        pFile->write(buf,strlen(buf));
      }
    }
    sprintf(buf,"###########################################################\n");
    pFile->write(buf,strlen(buf));
    setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}  


Bool_t HMdcTargetGeometry::read(fstream* pFile,Int_t* set, Int_t inputNumber) {
  if (pFile) {
    const Int_t maxbuf=1550;
    Text_t buf[maxbuf], key[20];
    Char_t c;
    Int_t sector, module, pos, index;
    Float_t zpos;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        if (strstr(buf,GetName())!=0) {
          pFile->getline(buf,maxbuf);
          sscanf(buf,"%f%f",&fXDim,&fYDim);

          while (!all && !pFile->eof()) {
            c=pFile->get();
            if (c=='#' || c=='/') all=kTRUE;
            else {
              pFile->putback(c);
              pFile->getline(buf,maxbuf);
              sscanf(buf,"%i%i%f",&sector,&module,&zpos);
              pos=sector*4+module;
              if (set[pos]){
		index = getIndex(sector,module);
		fZPos->AddAt(zpos,index);
	      }
            }
          }
        }
      }
    }
    if (all) {
      setInputVersion(1,inputNumber);
      setChanged();
      printf("TargetGeometry initialized from Ascii file\n");
      return kTRUE;
    }
  }
  return kFALSE;
} 






Bool_t HMdcTargetGeometry :: setZPos(Int_t nSect, Int_t nMod, Double_t z){

  if ((nSect<0) || (nSect>=fNSectors)) return kFALSE;
  if ((nMod<0)  || (nMod>=fNModules )) return kFALSE;

  Int_t pos = nSect*fNModules + nMod ;
  fZPos->AddAt(z, pos);
return kTRUE;
}


inline Double_t HMdcTargetGeometry :: getZPos(Int_t sect, Int_t mod){
  return fZPos->At(getIndex(sect,mod));
}				     

