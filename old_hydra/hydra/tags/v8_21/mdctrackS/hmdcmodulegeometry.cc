# include "TArrayF.h"
# include "TArrayI.h"
# include "hades.h"
# include "hdetpario.h"
# include "hmdcdetector.h"
# include "hmdcmodulegeometry.h"
# include "hpario.h"
# include "hspectrometer.h"

ClassImp(HMdcModuleGeometry)

//*-- Author : Beatriz Fuentes Arenaz
//*-- Last updated : 17/01/2002 by I. Koenig
//*-- Last updated : 16/3/2000 by Beatriz Fuentes(functions read and writeFile)
//*-- Last updated: 27/10/99
//*-- Modified : 19/05/99 
//*-- Copyright : GENP (Univ. Santiago de Compostela)

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////
//  HMdcModuleGeometry stores the geometry of one mdc module. //
//  It will change soon, since the general geometry     //
//  structure has not been completely designed.         //
//
//////////////////////////////////////////////////////////


  HMdcModuleGeometry :: HMdcModuleGeometry(const Char_t* name,const Char_t* title,
                                           const Char_t* context)
                     : HParSet(name,title,context) {
   // gets number of sectors, modules and layers from setup and allocate memory

  fNSectors = gHades->getSetup()->getDetector("Mdc")->getMaxSectors();
  fNModules = gHades->getSetup()->getDetector("Mdc")->getMaxModules();
  fNLayers  = ((HMdcDetector* )(gHades->getSetup()->getDetector("Mdc")))->getMaxComponents();

  fZPlane = new TArrayF(fNSectors*fNModules*fNLayers);
  fWireOrient = new TArrayF(fNSectors*fNModules*fNLayers);
  fCosn = new TArrayF(fNSectors*fNModules*fNLayers);
  fSinn = new TArrayF(fNSectors*fNModules*fNLayers);
  fSgpln = new TArrayF(fNSectors*fNModules*fNLayers);
  fPitch = new TArrayF(fNSectors*fNModules*fNLayers);
  fFirstWirePos = new TArrayF(fNSectors*fNModules*fNLayers);
  fCentralWire = new TArrayF(fNSectors*fNModules*fNLayers);
  fMaxNumCells = new TArrayI(fNSectors*fNModules*fNLayers);
  fCatDistance = new TArrayF(fNSectors*fNModules*fNLayers);

  strcpy(detName, "Mdc");
}



HMdcModuleGeometry :: ~HMdcModuleGeometry(void){
  // destructor

  if(fZPlane) delete fZPlane;
  if(fWireOrient) delete fWireOrient;
  if(fCosn) delete fCosn;
  if(fSinn) delete fSinn;
  if(fSgpln) delete fSgpln;
  if(fPitch) delete fPitch;
  if(fFirstWirePos) delete fFirstWirePos;
  if(fCentralWire) delete fCentralWire;
  if(fMaxNumCells) delete fMaxNumCells;
  if(fCatDistance) delete fCatDistance;

}



Bool_t HMdcModuleGeometry :: init(HParIo* inp, Int_t* set){
  // initializes the container from an input

  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}


Int_t HMdcModuleGeometry :: write(HParIo* output){
  // writes the container to an output
  
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}


 Int_t HMdcModuleGeometry::writeFile(fstream *pFile) {
  if (pFile) {
    Text_t buf[155];
    Text_t com[]="##################################################################################\n"
               "# Geometrical parameters for Santiago tracking of the MDC\n"
               "# Format:\n"
               "# sector module layer \n"
               "# zplane: z coordinate of plane   (mm) \n"
               "# angle: orientation of wires in layer (deg)\n"
               "# cosn    \n" 
               "# sinn   \n"
               "# sgpln: sigma of layer (mm)\n"
               "# pitch   (mm)\n"
               "# catdistance: distance between cathodes (mm)\n"
               "# firstWirePos: position of first wire (mm)\n"
               "# centralWire: number of central wire (mm)\n"
                "# maxNumCells: maximum number of cells in layer\n"
               "####################################################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",GetName());
    pFile->write(buf,strlen(buf));
    Int_t index;

    for(Int_t s=0;s<fNSectors;s++) {
      for(Int_t m=0;m<fNModules;m++) {
        for(Int_t l=0;l<fNLayers; l++){
	  index = getIndex(s,m,l);
	  sprintf(buf,"%2i%3i%3i%7.2f%5.0f%10.5f%8.5f%7.4f%5.1f%5.1f%9.1f%6.1f%5i\n",
		  s, m, l, fZPlane->At(index), fWireOrient->At(index),
		  fCosn->At(index), fSinn->At(index), fSgpln->At(index), 
		  fPitch->At(index), fCatDistance->At(index), 
		  fFirstWirePos->At(index), fCentralWire->At(index),
		  fMaxNumCells->At(index));
	  pFile->write(buf,strlen(buf));
        }
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


Bool_t HMdcModuleGeometry::read(fstream* pFile,Int_t* set, Int_t inputNumber) {
  if (pFile) {
    const Int_t maxbuf=1550;
    Text_t buf[maxbuf], key[20];
    Char_t c;
    Int_t sector, module, layer, pos, index;
    Float_t zplane,angle, cosn, sinn, sgpln, pitch, catDistance, firstWirePos;
    Float_t centralWire;
    Int_t maxNumCells;
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
              sscanf(buf,"%i%i%i%f%f%f%f%f%f%f%f%f%i",
                   &sector,&module,&layer,&zplane,&angle,&cosn,&sinn,&sgpln, 
                   &pitch,&catDistance,&firstWirePos,&centralWire,
                   &maxNumCells);
              pos=sector*4+module;
              if (set[pos]) {
		index = getIndex(sector, module, layer);
		fZPlane->AddAt(zplane,index);
		fWireOrient->AddAt(angle,index);
		fCosn->AddAt(cosn,index);
		fSinn->AddAt(sinn,index);
		fSgpln->AddAt(sgpln,index);
		fPitch->AddAt(pitch,index);
		fCatDistance->AddAt(catDistance,index);
		fFirstWirePos->AddAt(firstWirePos,index);
		fCentralWire->AddAt(centralWire,index);
		fMaxNumCells->AddAt(maxNumCells,index);
                }
            }
          }
        }
      }
    }
    if (all) {
      setInputVersion(1,inputNumber);
      setChanged();
      printf("MdcGeometry initialized from Ascii file\n");
      return kTRUE;
    }
  }
  return kFALSE;
} 

Bool_t HMdcModuleGeometry :: setZPlane(Int_t sector, Int_t module, Int_t layer, Float_t z){

  if ((sector<0) || (sector>=fNSectors)) return kFALSE;
  if ((module<0) || (module>=fNModules)) return kFALSE;
  if ((layer<0)  || (layer>=fNLayers  )) return kFALSE; 

  fZPlane->AddAt(z, getIndex(sector,module,layer));
return kTRUE;

}

Bool_t HMdcModuleGeometry :: setWireOrient(Int_t sector, Int_t module, Int_t layer, Float_t z){

  if ((sector<0) || (sector>=fNSectors)) return kFALSE;
  if ((module<0) || (module>=fNModules)) return kFALSE;
  if ((layer<0)  || (layer>=fNLayers  )) return kFALSE; 

  fWireOrient->AddAt(z, getIndex(sector,module,layer));
return kTRUE;

}


Bool_t HMdcModuleGeometry :: setCosn(Int_t sector, Int_t module, Int_t layer, Float_t cos){

  if ((sector<0) || (sector>=fNSectors)) return kFALSE;
  if ((module<0) || (module>=fNModules)) return kFALSE;
  if ((layer<0)  || (layer>=fNLayers  )) return kFALSE; 

  fCosn->AddAt(cos, getIndex(sector,module,layer));
return kTRUE;

}

Bool_t HMdcModuleGeometry :: setSinn(Int_t sector, Int_t module, Int_t layer, Float_t sin){

  if ((sector<0) || (sector>=fNSectors)) return kFALSE;
  if ((module<0) || (module>=fNModules)) return kFALSE;
  if ((layer<0)  || (layer>=fNLayers  )) return kFALSE; 

  fSinn->AddAt(sin, getIndex(sector,module,layer));
return kTRUE;

}


Bool_t HMdcModuleGeometry :: setSgpln(Int_t sector, Int_t module, Int_t layer, Float_t sigma){

  if ((sector<0) || (sector>=fNSectors)) return kFALSE;
  if ((module<0) || (module>=fNModules)) return kFALSE;
  if ((layer<0)  || (layer>=fNLayers  )) return kFALSE; 

  fSgpln->AddAt(sigma, getIndex(sector,module,layer));
return kTRUE;

}


Bool_t HMdcModuleGeometry :: setPitch(Int_t sector, Int_t module, Int_t layer, Float_t dist){

  if ((sector<0) || (sector>=fNSectors)) return kFALSE;
  if ((module<0) || (module>=fNModules)) return kFALSE;
  if ((layer<0)  || (layer>=fNLayers  )) return kFALSE; 

  fPitch->AddAt(dist, getIndex(sector,module,layer));
return kTRUE;
}


Bool_t HMdcModuleGeometry :: setFirstWirePos(Int_t sector, Int_t module, Int_t layer, Float_t pos){

  if ((sector<0) || (sector>=fNSectors)) return kFALSE;
  if ((module<0) || (module>=fNModules)) return kFALSE;
  if ((layer<0)  || (layer>=fNLayers  )) return kFALSE; 

  fFirstWirePos->AddAt(pos, getIndex(sector,module,layer));
return kTRUE;

}

Bool_t HMdcModuleGeometry :: setCentralWire(Int_t sector, Int_t module, Int_t layer, Float_t num){

  if ((sector<0) || (sector>=fNSectors)) return kFALSE;
  if ((module<0) || (module>=fNModules)) return kFALSE;
  if ((layer<0)  || (layer>=fNLayers  )) return kFALSE; 

  fCentralWire->AddAt(num, getIndex(sector,module,layer));
return kTRUE;

}

Bool_t HMdcModuleGeometry :: setMaxNumCells(Int_t sector, Int_t module, Int_t layer, Int_t num){


  if ((sector<0) || (sector>=fNSectors)) return kFALSE;
  if ((module<0) || (module>=fNModules)) return kFALSE;
  if ((layer<0)  || (layer>=fNLayers  )) return kFALSE; 

  fMaxNumCells->AddAt(num, getIndex(sector,module,layer));
return kTRUE;

}

Bool_t HMdcModuleGeometry :: setCatDistance(Int_t sector, Int_t module, Int_t layer, Float_t distance){

  if ((sector<0) || (sector>=fNSectors)) return kFALSE;
  if ((module<0) || (module>=fNModules)) return kFALSE;
  if ((layer<0)  || (layer>=fNLayers  )) return kFALSE; 

  fCatDistance->AddAt(distance, getIndex(sector,module,layer));
return kTRUE;

}

void HMdcModuleGeometry::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMdcModuleGeometry.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HParSet::Streamer(R__b);
      R__b >> fNSectors;
      R__b >> fNModules;
      R__b >> fNLayers;
      fZPlane->Streamer(R__b);
      fWireOrient->Streamer(R__b);
      fCosn->Streamer(R__b);
      fSinn->Streamer(R__b);
      fSgpln->Streamer(R__b);
      fPitch->Streamer(R__b);
      fCatDistance->Streamer(R__b);
      fFirstWirePos->Streamer(R__b);
      fCentralWire->Streamer(R__b);
      fMaxNumCells->Streamer(R__b);
   } else {
      R__b.WriteVersion(HMdcModuleGeometry::IsA());
      HParSet::Streamer(R__b);
      R__b << fNSectors;
      R__b << fNModules;
      R__b << fNLayers;
      fZPlane->Streamer(R__b);
      fWireOrient->Streamer(R__b);
      fCosn->Streamer(R__b);
      fSinn->Streamer(R__b);
      fSgpln->Streamer(R__b);
      fPitch->Streamer(R__b);
      fCatDistance->Streamer(R__b);
      fFirstWirePos->Streamer(R__b);
      fCentralWire->Streamer(R__b);
      fMaxNumCells->Streamer(R__b);
   }
}
