#include "hgeantkine.h"

//*-- Author : Romain Holzmann, GSI
//*-- Modified : 29/04/99 by Romain Holzmann
//*-- Copyright : GSI, Darmstadt

////////////////////////////////////////////////////////////////////////////
//  HGeantKine
// 
//  HGeant KINE event header
//
////////////////////////////////////////////////////////////////////////////

ClassImp(HGeantKine)

HGeantKine::HGeantKine(void) {
  trackNumber = 0;
  parentTrack = 0;
  particleID = 0;
  mediumNumber = 0;
  creationMechanism = 0;
  xVertex = yVertex = zVertex = 0.;
  xMom = yMom = zMom = 0.;
  generatorInfo = 0.;
  generatorWeight = 1.;
}

HGeantKine::HGeantKine(HGeantKine &aKine) {
  trackNumber = aKine.trackNumber;
  parentTrack = aKine.parentTrack;
  particleID = aKine.particleID;
  mediumNumber = aKine.mediumNumber;
  creationMechanism = aKine.creationMechanism;
  xVertex = aKine.xVertex;
  yVertex = aKine.yVertex;
  zVertex = aKine.zVertex;
  xMom = aKine.xMom;
  yMom = aKine.yMom;
  zMom = aKine.zMom;
  generatorInfo = aKine.generatorInfo;
  generatorWeight = aKine.generatorWeight;
}

HGeantKine::~HGeantKine(void) {
}

void HGeantKine::setParticle(Int_t aTrack, Int_t aID) {
  trackNumber = aTrack;
  particleID = aID;
}

void HGeantKine::setCreator(Int_t aPar, Int_t aMed, Int_t aMech) {
  parentTrack = aPar;
  mediumNumber = aMed;
  creationMechanism = aMech;
}

void HGeantKine::setVertex(Float_t ax, Float_t ay, Float_t az) {
  xVertex = ax;
  yVertex = ay;
  zVertex = az;
}

void HGeantKine::setMomentum(Float_t apx, Float_t apy, Float_t apz) {
  xMom = apx;
  yMom = apy;
  zMom = apz;
}

void HGeantKine::setGenerator(Float_t aInfo, Float_t aWeight) {
  generatorInfo = aInfo;
  generatorWeight = aWeight;
}

void HGeantKine::getParticle(Int_t &aTrack, Int_t &aID) {
  aTrack = trackNumber;
  aID = particleID;
}

void HGeantKine::getCreator(Int_t &aPar, Int_t &aMed, Int_t &aMech) {
  aPar = parentTrack;
  aMed = mediumNumber;
  aMech = creationMechanism;
}

void HGeantKine::getVertex(Float_t &ax, Float_t &ay, Float_t &az) {
  ax = xVertex;
  ay = yVertex;
  az = zVertex;
}

void HGeantKine::getMomentum(Float_t &apx, Float_t &apy, Float_t &apz) {
  apx = xMom;
  apy = yMom;
  apz = zMom;
}

void HGeantKine::getGenerator(Float_t &aInfo, Float_t &aWeight) {
  aInfo = generatorInfo;
  aWeight = generatorWeight;
}






