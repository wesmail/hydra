using namespace std;
# include "hmdccaltable.h"
# include "hmdccal3.h"
# include <iostream> 
# include <iomanip>


ClassImp(HMdcCal3)

//*-- Author : Beatriz Fuentes Arenaz
//*-- Modified : 30/08/2000 by Beatriz Fuentes (new name)
//*-- Modified : 24/11/98 by Beatriz Fuentes
//*-- Date : 11/05/98
//*-- Copyright : GENP (Univ. Santiago de Compostela)

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
//   An HMdcCal3 is a calibrated hit in a mdc plane.                  
//   It contains:                                             
//     - the coordinate in the plane in the local          
//       reference system (layer)                             
//     - error of the coordinate                             
//     - nHits: number of hits the cal3 object belongs to     
//     - fAddress: address of the cal3                         
//                                                            
//   The class contains the following not-inline functions:   
//   - a default constructor                                  
//   - a default destructor                                   
//   - print(void): prints the hit information to the         
//                  standard output                           
//   - print(ostream &file): prints the hit information       
//                           to file                          
////////////////////////////////////////////////////////////////


HMdcCal3 ::HMdcCal3(void){
  // Default constructor
  nHits = 0;
  error = 0.;
  pos=20000.;
  fAddress = 0x00000000; 
  link = NULL;
}

HMdcCal3 ::HMdcCal3(HMdcCal3 *copy){
  // Default constructor
  nHits = copy->getFlag();
  error = copy->getError();
  pos=copy->getPos();
  fAddress = copy->getAddress(); 
  link = copy->getLink();
}


HMdcCal3::~HMdcCal3(void) {
  // Default destructor
  //delete link;
}


void HMdcCal3::setAddress(Char_t sector, Char_t module, Char_t layer, Short_t cell, Char_t hitNumber){
  setSector(sector);
  setModule(module);
  setLayer(layer);
  setCell(cell);
  setHitNumber(hitNumber);
}


void HMdcCal3::print(void){
  // Prints the particle coordinates to the standard output
  cout << "nHits " << nHits << endl;
  cout << "pos " << pos << "  +-  " << error<< endl; 
  cout << "address:" << "s: " << getSector() << "  m: " << getModule() << "  l: " << getLayer() << "  c :" << getCell() << "  h :" << getHitNumber() << endl << endl;
}

void HMdcCal3::print(ostream &file){
  // Prints the particle coordinates to file
  file << "nHits " << nHits << endl;
  file << "pos " << pos  << "  +-  " << error << endl; 
  file << "address:" << "s: " << getSector() << "  m: " << getModule() << "  l: " << getLayer() << "  c :" << getCell() << "  h :" << getHitNumber() << endl << endl;
}









