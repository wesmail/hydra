#include<hshowergeompar.h>
#include<hdetpario.h>
#include "hpario.h"


ClassImp(HShowerGeomPar)
HShowerGeomPar::HShowerGeomPar(const char* name, const char* title, const char* context)
					:HDetGeomPar(name,title,context,"Shower") {	
}

Bool_t HShowerGeomPar::init(HParIo* inp,Int_t* set){
	HDetParIo* input=inp->getDetParIo("HShowerParIo");
	if (input) return (input->init(this,set));
	return kFALSE;

}
Int_t HShowerGeomPar::write(HParIo* output) {
  HDetParIo* out=output->getDetParIo("HShowerParIo");
  if (out) return out->write(this);
  return -1;
}

inline Int_t HShowerGeomPar::getSecNum(const TString& name){
		// Returns the sector index retrieved from the module name SHxMx
		return (Int_t)(name[4]-'0')-1;} 
inline Int_t HShowerGeomPar::getModNumInMod(const TString& name){
		// Returns the module index retrieved from the module name SHxMx
		return (Int_t)(name[2]-'0')-1;} 
inline Int_t HShowerGeomPar::getModNumInComp(const TString& name){
		// Returns the module index retrieved from the  layer of wires name SxSW 
		return (Int_t)name[1]-'0' -1;} 
