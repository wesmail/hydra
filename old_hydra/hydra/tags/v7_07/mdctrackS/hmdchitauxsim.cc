# include "hmdccal3.h"
# include "hmdchitauxsim.h"

ClassImp(HMdcHitAuxSim)

//*-- Author : Beatriz Fuentes Arenaz
//*-- Modified : 24/09/2000
//*-- Copyright : GENP (Univ. Santiago de Compostela)


////////////////////////////////////////////////////////////////////////
//                                                                    
// HMdcHitAuxSim                                                      
// An HMdcHitAuxSim inherits from HMdcHitAux, and has also an id    
// to identify the simulated track. 
// A category of HMdcHitAuxSim is build and used to compare the
// reconstructed data with the geant data
// 
////////////////////////////////////////////////////////////////////////


  HMdcHitAuxSim :: HMdcHitAuxSim(void) : HMdcHitAux(){ 
  // default constructor, sets id=0
  id = 0;
}


  HMdcHitAuxSim :: ~HMdcHitAuxSim(void){ 
    // default destructor
}


  void HMdcHitAuxSim :: print(ostream &file){

    // prints the hit information to file
    file << "hit " << id << endl;
    file << "number of impacts " << myNumberOfHits << endl;

    file << "calibrated data: " << endl;
    for (Int_t i=0; i<6; i++){
      if(getCal(i)!=NULL){
        getCal(i)->print(file);
      }
    }


}


