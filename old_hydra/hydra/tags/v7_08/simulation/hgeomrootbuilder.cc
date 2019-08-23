//*-- AUTHOR : Ilse Koenig
//*-- Created : 10/11/03 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
// HGeomRootBuilder
//
// Class to create the geometry in ROOT
//
///////////////////////////////////////////////////////////////////////////////

#include "hgeomrootbuilder.h"
#include "hgeommedium.h"
#include "hgeomnode.h"
#include "hgeomtransform.h"
#include "TGeoManager.h"
#include "TGeoMedium.h"
#include "TGeoMaterial.h"
#include "TGeoVolume.h"
#include "TGeoMatrix.h"
#include "TGeoPgon.h"
#include "TGeoPcon.h"

ClassImp(HGeomRootBuilder)

HGeomRootBuilder::HGeomRootBuilder() {
  // Default constructor
  geoManager=0;
}

HGeomRootBuilder::HGeomRootBuilder(const char* name,const char* title)
               : HGeomBuilder(name,title) {
  // Constructor
  geoManager=0;
}

Bool_t HGeomRootBuilder::createNode(HGeomNode* volu) {
  // Creates the volume
  if (!geoManager||!volu) return kFALSE;
  //volu->print();
  TString nodeName(volu->GetName(),4);
  HGeomNode* mother=volu->getMotherNode();
  if (!mother&&!volu->isTopNode()) {
    Error("createNode","Mother volume of %s not found\n",volu->GetName());
    return kFALSE;
  }
  TGeoVolume* rv=0;
  HGeomNode* cv=volu->getCopyNode();
  //if (cv) cout<<"Copy of "<<cv->GetName()<<endl;
  if (cv) rv=cv->getRootVolume();
  if (!rv) {
    HGeomMedium* medium=volu->getMedium();
    Int_t nMed=medium->getMediumIndex();
    if (nMed<=0) nMed=createMedium(medium);
    if (nMed<=0) return kFALSE;
    TArrayD* par=volu->getParameters();
    // for (Int_t k=0;k<par->GetSize();k++) cout<<par->At(k)<<" ";
    // cout<<endl;
    rv=geoManager->Volume(nodeName.Data(),volu->getShape().Data(),
          nMed,par->GetArray(),par->GetSize());
    volu->setCreated();
    if (volu->isModule()&&cv) {
      cv->setCreated();
      cv->setRootVolume(rv);
      cv->getPosition();
    }
  }
  if (!rv) return kFALSE;
  volu->setRootVolume(rv);
  if (volu->isTopNode()) geoManager->SetTopVolume(rv);
  else {
    HGeomTransform* trans=volu->getPosition();
    const HGeomRotation& rot=trans->getRotMatrix();
    const HGeomVector& pos=trans->getTransVector();
    TGeoMatrix* tr=0;
    if (((HGeomRotation&)rot).isUnitMatrix())
      tr=new TGeoTranslation(pos.getX(),pos.getY(),pos.getZ());
    else {
      nRot++;
      char b[10];
      sprintf(b,"R%i",nRot);
      TGeoRotation* r=new TGeoRotation(b);
      Double_t a[9];
      for (Int_t i=0;i<9;i++) a[i]=rot(i);
      r->SetMatrix(a);
      tr=new TGeoCombiTrans(pos.getX(),pos.getY(),pos.getZ(),r);
    }
    TGeoVolume* mo=mother->getRootVolume();
    if (!mo) return kFALSE;
    mo->AddNode(rv,volu->getCopyNo(),tr);
  }
  return kTRUE;
}

Int_t HGeomRootBuilder::createMedium(HGeomMedium* med) {
  // Creates the medium
  if (!geoManager&&!med) return 0;
  Int_t nComp=med->getNComponents();
  Int_t weightFac=med->getWeightFac();
  TGeoMaterial* material=0;
  Double_t p[3];
  if (nComp==1) {
    med->getComponent(0,p);
    material=new TGeoMaterial(med->GetName(),p[0],p[1],med->getDensity(),
                         med->getRadiationLength());
          // Interaction length not defined!!!!!!
  } else {
    material=new TGeoMixture(med->GetName(),nComp,med->getDensity());
    Double_t sumWeights=0.;
    if (weightFac<0) {
      for(Int_t i=0;i<nComp;i++) {
        med->getComponent(i,p);
        sumWeights+=p[0]*p[2];
      }
    }
    for(Int_t i=0;i<nComp;i++) {
      med->getComponent(i,p);
      if (weightFac>0)
        ((TGeoMixture*)material)->DefineElement(i,p[0],p[1],p[2]);
      else
        ((TGeoMixture*)material)->DefineElement(i,p[0],p[1],p[0]*p[2]/sumWeights);
    }
  }
  nMed++;
  med->setMediumIndex(nMed);
  Double_t mp[10];
  med->getMediumPar(mp);
  TGeoMedium* medium=new TGeoMedium(med->GetName(),nMed,material,mp);
  if (medium) return nMed;
  else return 0;
}

void HGeomRootBuilder::finalize() {
  // Closes the geometry input in ROOT and draws the cave
  if (geoManager) {
    geoManager->CloseGeometry();
    geoManager->SetVisLevel(2);
    geoManager->GetTopVolume()->Draw();
  }    
}

void HGeomRootBuilder::checkOverlaps(Double_t ovlp) {
  // Checks the geometry for overlaps and extrusions wit a default precision of 1 micron
  if (geoManager) {
    geoManager->CheckOverlaps(ovlp,"SAME");
    geoManager->PrintOverlaps();
  }
}
