///////////////////////////////////////////////////////////////////////////
//  HGeantKine
//
//  GEANT KINE event data
//
//  last modified on 29/1/99 by R.Holzmann  (GSI)
///////////////////////////////////////////////////////////////////////////
#ifndef HGEANTKINE_H
#define HGEANTKINE_H

#include "hlocateddataobject.h"

class HGeantKine : public HLocatedDataObject
{
private:
  Int_t trackNumber;        // GEANT track number
  Int_t parentTrack;        // track number of parent particle
  Int_t particleID;         // GEANT particle ID number
  Int_t mediumNumber;       // GEANT medium of creation number
  Int_t creationMechanism;  // GEANT creation mechanism number
  Float_t xVertex;          // x of track vertex  (in cm)
  Float_t yVertex;          // y
  Float_t zVertex;          // z
  Float_t xMom;             // x component of particle momentum (in MeV/c)
  Float_t yMom;             // y
  Float_t zMom;             // z
  Float_t generatorInfo;    // event generator info
  Float_t generatorWeight;  // associated weight 
public:
  HGeantKine(void);
  HGeantKine(HGeantKine &aKine);
  ~HGeantKine(void);
  void setParticle(Int_t aTrack, Int_t aID);
  void setCreator(Int_t aPar, Int_t aMed, Int_t aMech);
  void setVertex(Float_t ax, Float_t ay, Float_t az);
  void setMomentum(Float_t apx, Float_t apy, Float_t apz);
  void setGenerator(Float_t aInfo, Float_t aWeight);
  void getParticle(Int_t &aTrack, Int_t &aID);
  void getCreator(Int_t &aPar, Int_t &aMed, Int_t &aMech);
  void getVertex(Float_t &ax, Float_t &ay, Float_t &az);
  void getMomentum(Float_t &apx, Float_t &apy, Float_t &apz);
  void getGenerator(Float_t &aInfo, Float_t &aWeight);
  inline Int_t getNLocationIndex(void) {return 1;}
  inline Int_t getLocationIndex(Int_t) {return 0;} 

  ClassDef(HGeantKine,1) // Geant KINE event data class
};

#endif  /*! HGEANTKINE_H */









