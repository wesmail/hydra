//*-- AUTHOR : Ilse Koenig
//*-- Created : 10/11/2003

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
// HGeomTof
//
// Class for geometry of TOF
//
/////////////////////////////////////////////////////////////

#include "hgeomtof.h"
#include "hgeomnode.h"
#include "hgeomtofhit.h"

ClassImp(HGeomTof)

HGeomTof::HGeomTof() {
  // Constructor
  fName="tof";
  maxSectors=6;
  maxKeepinVolumes=1;
  maxModules=26;
  Int_t mod[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1};
  if (!modules) modules=new TArrayI(maxSectors*maxModules);
  for (Int_t s=0;s<maxSectors;s++) { 
    for (Int_t m=0;m<maxModules;m++) modules->AddAt(mod[m],(s*maxModules+m));
  } 
  pHit=new HGeomTofHit(this);
}

const Char_t* HGeomTof::getKeepinName(Int_t s,Int_t) {
  // Returns the name of the TOFINO keepin volume in sector s
  sprintf(keepinName,"TFN%i",s+1);  
  return keepinName;
}

const Char_t* HGeomTof::getModuleName(Int_t m) {
  // Returns the name of module m
  if (m<9) sprintf(modName,"T0%iF",m+1);
  else sprintf(modName,"T%iF",m+1);
  return modName;
}

const Char_t* HGeomTof::getEleName(Int_t m) {
  // Returns the element name in module m
  if (m<9) sprintf(eleName,"T0%i",m+1);
  else sprintf(eleName,"T%i",m+1);
  return eleName;
}

Bool_t HGeomTof::replaceTofKeepin() {
  // Replace the TOF keepin volume read from old geometry file by the corresponding sector
  if (!masterNodes) {
    Error("replaceTofKeepin","Master nodes not found!\n");       
    return kFALSE;
  }
  Char_t mod[6];
  Char_t sec[5];
  for(Int_t s=1;s<=6;s++) {
    sprintf(sec,"SEC%i",s);
    HGeomNode* mo=getMasterNode(sec);
    if (!mo) {
      Error("replaceTofKeepin","Mother volume %s not found!\n",sec);       
      return kFALSE;
    }
    for(Int_t m=1;m<=22;m++) {
      if (m<=9) sprintf(mod,"T0%iF%i",m,s);       
      else sprintf(mod,"T%iF%i",m,s);
      HGeomNode* v=getVolume(mod);
      if (v&&v->getMotherNode()==0) {
        v->setMother(mo);
        cout<<"New mother of "<<mod<<": "<<sec<<'\n';
      }
    }
  }
  return kTRUE;
}

void HGeomTof::setNodeType(HGeomNode* node) {
  // Sets the node type (Used by Oracle interface)
  const Char_t* name=node->GetName();
  if (name[1]=='F') node->setVolumeType(kHGeomKeepin);
  else if (name[3]=='F') node->setVolumeType(kHGeomModule);
  else  node->setVolumeType(kHGeomElement);
}
