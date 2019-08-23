//*-- AUTHOR : Ilse Koenig
//*-- modified : 31/06/99 by Ilse Koenig

/////////////////////////////////////////////////////////////
//
// HSpecParAsciiFileIo
//
// Class for parameter input/output from/into Ascii file for
// the spectrometer (not related to a special detector)
//
/////////////////////////////////////////////////////////////

#include "hspecparasciifileio.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hgeomvolume.h"
#include "hgeomshapes.h"
#include "hspecgeompar.h"

ClassImp(HSpecParAsciiFileIo)

HSpecParAsciiFileIo::HSpecParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor calls the base class constructor
  fName="HSpecParIo";
}


Bool_t HSpecParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls the appropriate read function of the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"SpecGeomPar")==0) return read((HSpecGeomPar*)pPar,set);
  }
  cerr<<"initialization of "<<name<<" not possible from file!"<<endl;
  return kFALSE;
}

Int_t HSpecParAsciiFileIo::write(HParSet* pPar) {
  // calls the appropriate write function of the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    if (strcmp(name,"SpecGeomPar")==0) return writeFile((HSpecGeomPar*)pPar);
  }
  cerr<<name<<" could not be written to Ascii file"<<endl;
  return kFALSE;
}


Bool_t HSpecParAsciiFileIo::read(HSpecGeomPar* pPar,Int_t* set) {
  if (!pFile) return kFALSE;
  TString key[3]={"GeomParCave","GeomParSectors","GeomParTargets"};
  Bool_t allFound=kTRUE;
  const Int_t maxbuf=1550;
  Text_t buf[maxbuf];
  HGeomVolume volu;
  HGeomTransform transform;
  HGeomShapes* shapes=pPar->getShapes();
  for(Int_t i=0;i<3;i++) {
    pFile->clear();
    pFile->seekg(0,ios::beg);
    Int_t k=0;
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if( (buf[0]=='[')) {
        if (strstr(buf,(const char*)key[i])!=0) {
          while (!all && !pFile->eof()) {
            pFile->getline(buf,maxbuf);
            if (buf[0]=='#') all=kTRUE;   
            else {
              if (buf[0]=='/') continue;
              if (readVolume(&volu,shapes,buf)==kFALSE) return kFALSE;
              if (set[i]) {
                switch(i) {
	          case 0: { pPar->addCave(&volu); break; }
	          case 1: { pPar->addSector(&volu); break; }
	          case 2: { pPar->addTarget(&volu); break; }
                }
                k++; 
              }
	    }
          }
        }
      }
    }
    if (k) set[i]=0;
    else allFound=kFALSE;
  }
  if (allFound) {
    pPar->setInputVersion(1,inputNumber);
    pPar->setChanged();
    printf("%s initialized from Ascii file\n",pPar->GetName());
  }
  return allFound;  
}


Int_t HSpecParAsciiFileIo::writeFile(HSpecGeomPar* pPar) {
  if (pFile) {
    Text_t buf[1000];
    Text_t com1[]=
        "#########################################################\n"
        "# Parameters describing the geometry of the cave,\n"
        "# the sector(s) and the target(s)\n"
        "#\n"
        "#   [GeomParCave] parameters of the cave:\n"
        "#      name of volume   shape\n";
    Text_t com2[]=
        "#   [GeomParSectors] parameters of the sectors:\n"
        "#      name of volume   shape   name of mother\n";
    Text_t com3[]=
        "#   [GeomParTargets] parameters of the targets:\n"
        "#      name of volume   shape   name of mother\n";
    Text_t com4[]=
        "#      x, y and z components of points \n"
        "#      rotation matrix\n"
        "#      translation vector\n"
        "#\n";
    Text_t com5[]=
        "#########################################################\n";
    sprintf(buf,"%s%s%s",
         com1,com4,com5);
    pFile->write(buf,strlen(buf));
    sprintf(buf,"[GeomParCave]\n");
    pFile->write(buf,strlen(buf));
    HGeomShapes* shapes=pPar->getShapes();
    HGeomVolume* volu=pPar->getCave();
    if (volu) writeVolume(volu,shapes);
    sprintf(buf,"%s%s%s%s[GeomParSectors]\n",com5,com2,com4,com5);
    pFile->write(buf,strlen(buf));
    for(Int_t i=0;i<pPar->getNumSectors();i++) {
      volu=pPar->getSector(i);
      if (volu) writeVolume(volu,shapes);
    }
    sprintf(buf,"%s%s%s%s[GeomParTargets]\n",com5,com3,com4,com5);
    pFile->write(buf,strlen(buf));
    for(Int_t i=0;i<pPar->getNumTargets();i++) {
      volu=pPar->getTarget(i);
      if (volu) writeVolume(volu,shapes);
    }
pFile->write(com5,strlen(com5));
    //    sprintf(buf,"########################################################\n");
    //    pFile->write(buf,strlen(buf));
    pPar->setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}
