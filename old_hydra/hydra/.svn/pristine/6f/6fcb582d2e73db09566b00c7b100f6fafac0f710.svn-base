//*-- Author : Ilse Koenig
//*-- Last modified : 06/12/2001 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////
//
// HEventDisplay
//
// This class uses the ROOT class TGeometry to display detectors in a 3D 
// view. 
//
///////////////////////////////////////////////////////////////////////////
using namespace std;
#include "heventdisplay.h"
#include "hdetevtdisplay.h"
#include "hades.h" 
#include "hevent.h"
#include "htaskset.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hparset.h"
#include "hspecgeompar.h"
#include "hdetgeompar.h"
#include "hgeomvolume.h"
#include "hgeomshapes.h"
#include "hgeombasicshape.h"
#include "TArrayD.h"
#include "TString.h"
#include <iostream>  
#include <iomanip>
#include <stdlib.h>

ClassImp(HEventDisplay)

HEventDisplay::HEventDisplay(Int_t nDisp)
          : HReconstructor("Event Display","Hades Event Display") {
  // Constructor takes the number of displays which should be created.
  // The default value is the number of detectors defined in the actual setup 
  if (gHades) {
    hgeom=0;
    if (nDisp>0) detDisplays=new TOrdCollection(nDisp);
    else {
      HSpectrometer* spec = gHades->getSetup();
      TList* detList = spec->getListOfDetectors();
      Int_t nDet=detList->GetSize();
      detDisplays=new TOrdCollection(nDet);
    } 
    masterNodes=0;
    targetNodes=0;
    iter=0;
  } else {
    TObject::Error("HEventDisplay","No Analysis defined !");
    exit(1);
  }
} 


HEventDisplay::~HEventDisplay() {
  // deletes the displays and the arrays of nodes but not the nodes inside
  if (hgeom) delete hgeom;
  if (detDisplays) {
    detDisplays->Delete();
    delete detDisplays;
  }
  if (masterNodes) delete masterNodes;
  if (targetNodes) delete targetNodes;
  if (iter) delete iter;
}


void HEventDisplay::addDetDisplay(HDetEvtDisplay* disp) {
  detDisplays->Add(disp);
}


Bool_t HEventDisplay::init() {
  if (detDisplays==0) {
    Error("HEventDisplay::init()","No detector displays defined");
    return kFALSE;
  }
  iter=detDisplays->MakeIterator();
  HDetEvtDisplay *disp=0;
  Bool_t rc=kTRUE;
  while (rc && (disp=(HDetEvtDisplay*)(iter->Next()))) {
    rc=disp->init();
  }
  fActive=kTRUE;
  specGeomPar=(HSpecGeomPar*)(gHades->getRuntimeDb()->getContainer("SpecGeomPar"));
  if (rc && specGeomPar) return kTRUE;
  return kFALSE;
}


Bool_t HEventDisplay::reinit() { 
  Bool_t hasChanged=kFALSE;
  HDetEvtDisplay *disp=0;
  if (specGeomPar->hasChanged()) hasChanged=kTRUE;
  else {
    iter->Reset();
    while (hasChanged==kFALSE && (disp=(HDetEvtDisplay*)(iter->Next()))) {
      hasChanged=disp->needsChange();
    }
  }
  // temporay solution: to avoid the deletion of the TGeometry object hgeom
  // the geometry is created only once and cannot be changed during an analysis
  if (hasChanged && hgeom==0) {
  //  if (hasChanged) {
    Bool_t rc=kTRUE;
    iter->Reset();
    while (rc==kTRUE && (disp=(HDetEvtDisplay*)(iter->Next()))) {
      rc=disp->reinit();
    }
    if (rc==kFALSE) return kFALSE;
    if (hgeom) {
      delete hgeom;
      hgeom=0;
    }
    if (masterNodes) {    
      delete masterNodes;
      masterNodes=0;
    }
    if (targetNodes) {
      delete targetNodes;
      targetNodes=0;
    }
    hgeom=new TGeometry("HadesGeom","HadesGeom");
    return createGeom();
  }
  return kTRUE;
}


Bool_t HEventDisplay::createGeom() {
  Int_t nSec=specGeomPar->getNumSectors();
  masterNodes=new TObjArray(nSec+1);
  HGeomVolume* vol=specGeomPar->getCave();
  const HGeomTransform& caveTransform=vol->getTransform();
  HGeomShapes* shapes=specGeomPar->getShapes();
  HGeomBasicShape* s=shapes->selectShape(vol);
  TShape* ts=s->createRootVolume(vol);
  const HGeomTransform tr;
  s->calcVoluPosition(vol,tr);
  HGeomTransform centerCave(*(s->getCenterPosition()));
  TNode* caveNode = new TNode("CAVE","CAVE",ts);
  caveNode->SetVisibility(0);
  masterNodes->AddAt(caveNode,0);
  caveNode->cd();
  ts=0;
  Int_t n=specGeomPar->getNumTargets();
  if (n>0) targetNodes=new TObjArray(n); 
  for(Int_t i=0;i<n;i++) {
    vol=specGeomPar->getTarget(i);
    if (vol) {
      Text_t* name=(char*)vol->GetName();
      s=shapes->selectShape(vol);
      if (s==0) return kFALSE;
      if (ts==0) ts=s->createRootVolume(vol);
      s->calcVoluPosition(vol,centerCave);
      HGeomTransform* mTR=s->getVoluPosition();
      HGeomRotation r(mTR->getRotMatrix());
      TRotMatrix *mRot=r.createTRotMatrix(name,name);
      const HGeomVector& v=mTR->getTransVector();
      targetNodes->AddAt(new TNode(name,name,ts,v(0),v(1),v(2),mRot),i);
    }
  }

  HDetEvtDisplay* disp;
  if (nSec<=0) {
    iter->Reset();
    while ((disp=(HDetEvtDisplay*)(iter->Next()))) {
      if (disp->createGeom(caveNode,&centerCave,&caveTransform)==kFALSE)
          return kFALSE; 
    }
  } else {
    ts=0;
    Text_t* name=0;
    for(Int_t i=0;i<nSec;i++) {
      vol=specGeomPar->getSector(i);
      if (vol) {
        name=(char*)vol->GetName();
        s=shapes->selectShape(vol);
        if (s==0) return kFALSE;
        if (ts==0) ts=s->createRootVolume(vol);
        s->calcVoluPosition(vol,centerCave);
        HGeomTransform center(*(s->getCenterPosition()));
        HGeomTransform* pos=s->getVoluPosition();
        HGeomRotation r(pos->getRotMatrix());
        TRotMatrix *mRot=r.createTRotMatrix(name,name);
        const HGeomVector& v=pos->getTransVector();
        TNode* node=new TNode(name,name,ts,v(0),v(1),v(2),mRot);
        node->SetVisibility(0);
        masterNodes->AddAt(node,i+1);
        node->cd();
        iter->Reset();
        while ((disp=(HDetEvtDisplay*)(iter->Next()))) {
          if (disp->createGeom(node,&center,&(vol->getTransform()),i)==kFALSE)
              return kFALSE;
        }
        caveNode->cd();
      }
    }
  }
  iter->Reset();
  while ((disp=(HDetEvtDisplay*)(iter->Next()))) {
     disp->cleanup(); 
  }
  return kTRUE;
}


void HEventDisplay::draw(Int_t nSec) {
  if (nSec<0) hgeom->Draw();
  else {
    TNode* sec=(TNode*)masterNodes->At(nSec+1);
    if (sec) sec->Draw();
  }
}

void HEventDisplay::clear() {
  iter->Reset();
  HDetEvtDisplay *disp=0;
  while ((disp=(HDetEvtDisplay*)(iter->Next()))) {
    disp->clear();
  }
}


Int_t HEventDisplay::execute() {
  //loops over all detector displays and calls execute()
  // draws the complete geometry
  iter->Reset();
  HDetEvtDisplay *disp=0;
  Int_t rc=0;
  while (rc>=0 && (disp=(HDetEvtDisplay*)(iter->Next()))) {
    rc=disp->execute();
  }
  if (rc>=0) hgeom->Draw(); 
  return 0;
}
