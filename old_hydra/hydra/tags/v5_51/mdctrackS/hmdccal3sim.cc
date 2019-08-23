# include <fstream.h>
# include "hmdccal3.h"
# include "hmdccal3sim.h"


ClassImp(HMdcCal3Sim)

//*-- Author : Beatriz Fuentes Arenaz
//*-- Modified : 30/08/2000 by Beatriz Fuentes Arenaz
//*-- Modified : 23/11/98
//*-- Date: 11/05/98
//*-- Copyright : GENP (Univ. Santiago de Compostela)

///////////////////////////////////////////////////////////////////////////////
//                                                                           
//  An HMdcCal3Sim is a calibrated hit obtained from a simulation          
//  process. An HMdcCal3Sim has the coordinate on the layer and the ID of the
//  particle that has produced the hit in the simulation.                    
//
///////////////////////////////////////////////////////////////////////////////


  HMdcCal3Sim :: HMdcCal3Sim(void) : HMdcCal3(){
    // Default constructor
    id = 0;
}

HMdcCal3Sim :: HMdcCal3Sim(HMdcCal3Sim* copy) : HMdcCal3(copy){
  id = copy->getId();
}

  HMdcCal3Sim :: HMdcCal3Sim(Int_t identifier){
    // Constructor that sets the particle ID
    id = identifier;
} 

HMdcCal3Sim::~HMdcCal3Sim(void) {
  // Default destructor
}



void HMdcCal3Sim :: print(void){
  // Prints the particle ID and the coordinates to the standard output
  cout << "id " << id << endl;
  cout << "nHits " << nHits << endl;
  cout << "pos " << pos << "  +-  " << error << endl; 
  cout << "address:" << "s: " << getSector() << "  m: " << getModule() << "  l: " << getLayer() << "  c :" << getCell() << "  h :" << getHitNumber() << endl << endl;

}

void HMdcCal3Sim :: print(ostream &file){
  // Prints the particle ID and the coordinates to file
  file << "id " << id << endl;
  file << "nHits " << nHits << endl;
  file << "pos " << pos << "  +-  " << error << endl; 
  file << "address:" << "s: " << getSector() << "  m: " << getModule() << "  l: " << getLayer() << "  c :" << getCell() << "  h :" << getHitNumber() << endl << endl;

}






