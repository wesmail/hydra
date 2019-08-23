//*-- AUTHOR : Denis Bertini
//*-- Last modified : 15/12/98 by Ilse Koenig

/////////////////////////////////////////////////////////////
//HMdcParAsciIo
//
//  Class for Mdc parameter input/output from/into Asci file
//
/////////////////////////////////////////////////////////////

#include "hmdcparasciio.h"
#include "hades.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"
#include "hmdccalpar.h"
#include "hmdclookupgeom.h"
#include <iostream.h>

ClassImp(HMdcParAsciIo)

HMdcParAsciIo::HMdcParAsciIo(fstream* f) {
  pFile=f;
  fName="HMdcParIo";
  //printf(" constructor\n");
}


Bool_t HMdcParAsciIo::init(HParSet* pPar,Int_t* set) {
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"MdcRawStruct")==0) return read((HMdcRawStruct*)pPar,set);
    if (strcmp(name,"MdcGeomStruct")==0) return read((HMdcGeomStruct*)pPar,set);
    if (strcmp(name,"MdcCalPar")==0) return read((HMdcCalPar*)pPar,set);
    if (strcmp(name,"MdcLookupGeom")==0) return read((HMdcLookupGeom*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from file!"<<endl;
  return kFALSE;
}


Int_t HMdcParAsciIo::write(HParSet* pPar) {
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"MdcRawStruct")==0) return writeFile((HMdcRawStruct*)pPar);
    if (strcmp(name,"MdcGeomStruct")==0) return writeFile((HMdcGeomStruct*)pPar);
    if (strcmp(name,"MdcCalPar")==0) return writeFile((HMdcCalPar*)pPar);
    if (strcmp(name,"MdcLookupGeom")==0) return writeFile((HMdcLookupGeom*)pPar);
  }
  cerr<<name<<" could not be written to Asci file"<<endl;
  return kFALSE;
}


Bool_t HMdcParAsciIo::write(HDetector* p) {
  return kTRUE;  // not implemented!
}


Int_t  HMdcParAsciIo::writeFile(HMdcGeomStruct* pPar){
  Text_t buf[155];
  //printf("pfile ptr %i \n", pFile);
  if (pFile) {
    sprintf(buf,"#############################################\n");
    pFile->write(buf,strlen(buf));
    sprintf(buf,"%10s\n",pPar->GetName());
    pFile->write(buf,strlen(buf));
    sprintf(buf,"#############################################\n");
    pFile->write(buf,strlen(buf));

    HMdcGeomStruct& pp = *pPar;
    for(Int_t s=0;s<6;s++) {
      HMdcGeomSecStru& sec= pp[s];
      for(Int_t m=0;m<4;m++) {
        HMdcGeomModStru& mod=sec[m];
        for(Int_t l=0;l<6;l++) {
          Int_t ncell= mod[l];
          sprintf(buf,"%10i%10i%10i%10i\n", s, m, l, ncell);
          //printf(" buffer ===> %s \n",buf);
          pFile->write(buf,strlen(buf));
        }
      }
    }
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}


Int_t  HMdcParAsciIo::writeFile(HMdcCalPar* pPar){
  if (pFile) {
    Text_t buf[155];
    Float_t slo, off;
    sprintf(buf,"#############################################\n");
    pFile->write(buf,strlen(buf));
    sprintf(buf,"%10s\n",pPar->GetName());
    pFile->write(buf,strlen(buf));
    sprintf(buf,"#############################################\n");
    pFile->write(buf,strlen(buf));

    HMdcCalPar& pp = *pPar;
    for(Int_t s=0;s<6;s++) {
      HMdcCalParSec& sec=pp[s];
      for(Int_t m=0;m<4;m++) {
        HMdcCalParMod& mod=sec[m];
        for(Int_t l=0;l<6;l++) {
          HMdcCalParLayer& lay=mod[l];
          Int_t nc=lay.getSize();
          for(Int_t n=0;n<nc;n++) {
            HMdcCalParCell& cell=lay[n];
            slo=cell.getSlope();
            off=cell.getOffset();
            sprintf(buf,"%3i%3i%3i%5i%8.4f%8.4f\n", s, m, l, n, slo, off);
            pFile->write(buf,strlen(buf));
          }
        }
      }
    }
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}

Bool_t HMdcParAsciIo::read(HMdcGeomStruct* pPar,Int_t*) {
if (pFile){
    const Int_t MAXBUF=155;
    Text_t buf[MAXBUF],key[20];
    Text_t name[MAXBUF];
    Int_t sector, module, layer, nwires;
    Char_t c;
    Bool_t all=kFALSE;
      
      while (!all && !pFile->eof()) {
        pFile->getline(buf, MAXBUF);
   	 if( (buf[0]=='[')) {
           sscanf(buf,"%s",key);
            if(strstr(buf,pPar->GetName())!=0){
              while (!all && !pFile->eof()) {             
                 c=pFile->get();
                 if (c=='#' || c=='/') all=kTRUE;   
              else {
              pFile->putback(c);
              pFile->getline(buf,MAXBUF);
              sscanf(buf,"%i%3i%3i%3i",
                           &sector,&module,&layer,&nwires);
	      // printf("scanning => %10i%10i%10i%10i \n",sector,module
	      // ,layer,nwires);       
              HMdcGeomStruct &pp = *pPar;
              pp[sector-1][module-1].fill(layer-1,nwires);
              } 
	    }
	 }
       }
      }
      if (all) {
    pPar->setInputVersion(1,inputNumber);
    pPar->setChanged();
    //    pPar->setStatic();
    printf("MdcGeomStruct initialized from Asci file\n");
    return kTRUE;
      }
 }
  return kFALSE;
}



Bool_t HMdcParAsciIo::read(HMdcRawStruct* pPar,Int_t* set) {
  if (pFile) {
    const Int_t maxbuf=155;
    Text_t buf[maxbuf], mboName[10], key[20];
    Char_t c;
    Int_t sector, module, readout, num;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        if (strstr(buf,pPar->GetName())!=0) {
          while (!all && !pFile->eof()) {
            c=pFile->get();
            if (c=='#' || c=='/') all=kTRUE;
            else {
              pFile->putback(c);
              pFile->getline(buf,maxbuf);
              sscanf(buf,"%i%3i%3i  %s%4i",
                           &sector,&module,&readout,mboName,&num);
              HMdcRawMothStru& r=(*pPar)[sector-1][module-1][readout-1];
              r.SetName(mboName);
              r.setNTdcs(num);
            }
          }
        }
      }
    }
    if (all) {
    pPar->setInputVersion(1,inputNumber);
    pPar->setChanged();
    pPar->setStatic();
    return kTRUE;
    }
  }
  return kFALSE;
} 

Bool_t HMdcParAsciIo::read(HMdcCalPar*,Int_t*) {
  return kFALSE;  // not implemented
}


Bool_t HMdcParAsciIo::read(HMdcLookupGeom* pPar,Int_t* set) {
  if (pFile) {
    const Int_t maxbuf=155;
    Text_t buf[maxbuf], mboName[10], key[20];
    Char_t c;
    Int_t sector, module, readout, tdc, chan, layer, wire;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        if (strstr(buf,pPar->GetName())!=0) {
          while (!all && !pFile->eof()) {
            c=pFile->get();
            if (c=='#' || c=='/') all=kTRUE;
            else {
              pFile->putback(c);
              pFile->getline(buf,maxbuf);
              if (c=='M')
                sscanf(buf,"%s%3i%3i%3i",mboName,&sector,&module,&readout);
              else {
                sscanf(buf,"%2i%3i%4i%5i",
                             &tdc,&chan,&layer,&wire);
                HMdcLookupChan& r=
                    (*pPar)[sector-1][module-1][readout-1][((tdc-1)*8+(chan-1))];
                r.fill(layer-1,wire-1);
              }
            }
          }
        }
      }
    }
    if (all) {
    pPar->setInputVersion(1,inputNumber);
    pPar->setChanged();
    return kTRUE;
    }
  }
  return kFALSE;
} 

Int_t HMdcParAsciIo::writeFile(HMdcRawStruct*) {
  return kFALSE;  // not implemented
}  

Int_t HMdcParAsciIo::writeFile(HMdcLookupGeom*) {
  return kFALSE;  // not implemented
}











