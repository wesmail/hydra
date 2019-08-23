//*-- Author : Ilse Koenig
//*-- Modified : 06/09/99 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
// HMdcDisplay
// 
// class for MDC event display
///////////////////////////////////////////////////////////////////////////////

#include "hmdcevtdisplay.h"
#include "hcatevtdisplay.h"
#include "hmdccal1evtdisplay.h"
#include "hmdcnodes.h"
#include "hmdcdef.h"
#include "hmdccal1.h"
#include "hmdcdetector.h"
#include "hmdcgeompar.h"
#include "hmdclayergeompar.h"
#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hgeomshapes.h"
#include "hgeombasicshape.h"
#include "hgeomcompositevolume.h"
#include "hgeomtransform.h"
#include "hmdcconnectedwires.h"
#include "hmdcrawstruct.h"
#include "TRotMatrix.h"
#include "TString.h"

ClassImp(HMdcEvtDisplay)

HMdcEvtDisplay::HMdcEvtDisplay() {
  // default constructor
  moduleNodes=0;
  refNodes=0;
  centers=0;
  catDisplays=0;
  numCatDisp=0;
}


HMdcEvtDisplay::HMdcEvtDisplay(HDetector *det) {
  // sets the name of the display "MdcDisplay"
  // creates the array for the module nodes
  fName="MdcDisplay";
  fTitle="mdc event display";
  maxSec=det->getMaxSectors();
  maxMod=det->getMaxModules();
  maxLay=det->getMaxComponents();
  moduleNodes=new HMdcNodes(maxSec,maxMod);
  refNodes=0;
  centers=0;
  catDisplays=0;
  numCatDisp=0;
}


HMdcEvtDisplay::~HMdcEvtDisplay() {
#warning : destructor takes minutes to execute!
  // destructor deletes the array
  // The nodes are not deleted (owned by the TGeometry object in HEventDisplay)
  cleanup();
  if (moduleNodes) delete moduleNodes;
  if (catDisplays) {
    catDisplays->Delete();
    delete catDisplays;
  }
}


void HMdcEvtDisplay::cleanup() {
  if (refNodes) {
    delete refNodes;
    refNodes=0;
  }

  if (centers) {
//    centers->Delete();
    delete centers;
    centers=0;
  }

}   


Bool_t  HMdcEvtDisplay::init() {
  // creates the parameter containers if they do not exist
  // connects the category Mdc Cal1 (preliminary solution!)
  mdcGeomPar=(HMdcGeomPar*)(gHades->getSetup()->createDetGeomPar("Mdc"));
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  mdcLayGeomPar=
    (HMdcLayerGeomPar*)(rtdb->getContainer("MdcLayerGeomPar"));
  if (!mdcLayGeomPar) {
    mdcLayGeomPar=new HMdcLayerGeomPar();
    rtdb->addContainer(mdcLayGeomPar);
  }
  if (mdcGeomPar && mdcLayGeomPar) {
    shapes=mdcGeomPar->getShapes();
//    return kTRUE; 
  }
  else return kFALSE;

  HCatEvtDisplay* disp;
  Bool_t rc=kTRUE;
  for(Int_t i=0;i<numCatDisp;i++) {
    disp=(HCatEvtDisplay*)((*catDisplays)[i]);  
    if (disp) rc=disp->init();
    if (rc==kFALSE) return kFALSE;
  }
  return kTRUE;
}


Bool_t HMdcEvtDisplay::needsChange() {
  // returns kTRUE if the parameter containers have changed
  // called by HEventDisplay::reinit() to check if the geometry has to be
  // recreated
  if (mdcGeomPar->hasChanged() || mdcLayGeomPar->hasChanged()) return kTRUE;
  else return kFALSE;
}


Bool_t  HMdcEvtDisplay::reinit() {
#warning : reinit() still not working correctly if invoked a second time
  // creates the displays for the different categories
  // calls makeClean() and recreates the arrays
  catDisplays=new TObjArray(1);
  numCatDisp = 0;
  HEvent* event=gHades->getCurrentEvent();
  HCategory* cat=0;
  if ((cat=event->getCategory(catMdcCal1))) {
    HMdcCal1EvtDisplay* disp=new HMdcCal1EvtDisplay(cat,moduleNodes);
    addCatEvtDisplay(disp);
  }
  //  if ((cat=event->getCategory(catMdcCal2))) {
  //    HMdcCal2EvtDisplay* disp=new HMdcCal2EvtDisplay(cat,moduleNodes);
  //    addCategory(disp);
  //  }

  cleanup();
  if (moduleNodes) moduleNodes->removeCompNodes();
  TArrayI mods((maxSec*maxMod));
  maxLay=0;
  Int_t maxWire=0;
  for(Int_t i=0;i<mdcLayGeomPar->getSize();i++) {
    HMdcLayerGeomParSec& sec=(*mdcLayGeomPar)[i];
    for(Int_t j=0;j<sec.getSize();j++) {
      HMdcLayerGeomParMod& mod=sec[j];
      Int_t k=0;
      for(;k<mod.getSize();k++) {  
        Int_t n=mod[k].getNumWires();
        if (n>0) mods.AddAt(1,(i*maxMod+j));
        if (n>=maxWire) maxWire=n;
      }
      if (k>=maxLay) maxLay=k;
    }
  }
  moduleNodes->createCompNodes(mods,maxLay,maxWire);
  maxRefMod=mdcGeomPar->getNumRefModules();
  refNodes=new HMdcNodes(1,maxRefMod);
  moduleNodes->createCompNodes(mods,maxLay,maxWire);
  centers=new TObjArray(maxRefMod*(maxLay+1));
  HCatEvtDisplay* disp;
  Bool_t rc=kTRUE;
  for(Int_t i=0;i<numCatDisp;i++) {
    disp=(HCatEvtDisplay*)((*catDisplays)[i]);  
    if (disp) rc=disp->reinit();
    if (rc==kFALSE) return kFALSE;
  }
  connectedWires=
    (HMdcConnectedWires*)(gHades->getRuntimeDb()->getContainer("MdcConnectedWires"));
  return kTRUE;
}


void HMdcEvtDisplay::addCatEvtDisplay(HCatEvtDisplay* disp) {
  if (catDisplays->IndexOf(disp)>=0) return;
  catDisplays->AddAtAndExpand(disp,numCatDisp);
  numCatDisp++;
}


HCatEvtDisplay* HMdcEvtDisplay::getCatEvtDisplay(Text_t* name) {
  HCatEvtDisplay* d=0;
  d=(HCatEvtDisplay*)(catDisplays->FindObject(name));
  return d; 
}


Bool_t  HMdcEvtDisplay::createGeom(TNode* masterNode,
                                const HGeomTransform* masterCenter,
                                const HGeomTransform* masterTransform,
                                Int_t nSector) {
  // creates the geometry
  Int_t nModules=mdcGeomPar->getNumModules();
  Int_t nMod=0;
  TObjArray* modules=0;
  if (nSector<0) {
    modules=new TObjArray(nMod);
    nMod=nModules;
    for (Int_t i=0;i<nMod;i++) modules->AddAt(mdcGeomPar->getModule(i),i);
  } else {
    modules=new TObjArray(maxMod);
    mdcGeomPar->getSector(modules,nSector);
    nMod=maxMod;
  }

  // creates modules
  for (Int_t i=0;i<nMod;i++){
    HModGeomPar* modgeompar=(HModGeomPar*)modules->At(i);
    if (modgeompar==0) continue;
    Text_t* name=(char*)modgeompar->GetName();
    Int_t nSec=mdcGeomPar->getSecNum(name);
    HGeomCompositeVolume* refMod=modgeompar->getRefVolume();
    if (refMod) {
      Text_t* refName=(char*)refMod->GetName();
      HGeomBasicShape* s=shapes->selectShape(refMod);
      HGeomTransform tMod(modgeompar->getLabTransform());
      tMod.transTo(*masterTransform);
      s->calcVoluPosition(refMod,tMod,*masterCenter);
      Int_t nm=mdcGeomPar->getModNumInMod(refName);
      TNode* mRef=refNodes->getNode(0,nm);
      TShape* rVolu=0;
      HGeomTransform mCenter(*(s->getCenterPosition()));
      if (mRef) {
        rVolu=mRef->GetShape();
      } else {
        rVolu=s->createRootVolume(refMod);
        if (!rVolu) {
          Error("HMdcEvtDisplay::createGeom(...)","volume not created");
          return kFALSE;
        }
        addCenter(&mCenter,nm);
      }
      HGeomTransform* tr=s->getVoluPosition();
      HGeomRotation r(tr->getRotMatrix());
      TRotMatrix* rot=r.createTRotMatrix(name,name);
      const HGeomVector& v=tr->getTransVector();
      TNode* mNode=new TNode(name,name,rVolu,v(0),v(1),v(2),rot);
      moduleNodes->addNode(mNode,nSec,i);
      if (!mRef) refNodes->addNode(mNode,0,nm);
      mNode->cd();

      // create layers
      for(Int_t j=0;j<maxLay;j++) {
        HGeomVolume* layer=refMod->getComponent(j);
        Text_t* name=(char*)layer->GetName();
        TNode* lRef=refNodes->getNode(0,nm,j);
        TNode* lNode=0;
        HGeomTransform lCenter;
        if (lRef) {
          lCenter.setTransform(*(getCenter(nm,j)));
          lNode=copyNode(name,lRef);
        }
        else {
          s=shapes->selectShape(layer);
          rVolu=s->createRootVolume(layer);
          s->calcVoluPosition(layer,mCenter);
          lCenter.setTransform(*(s->getCenterPosition()));
          addCenter(&lCenter,nm,j);
          tr=s->getVoluPosition();
          r=tr->getRotMatrix();
          TRotMatrix* rot=r.createTRotMatrix(name,name);
          const HGeomVector& v=tr->getTransVector();
          lNode=new TNode(name,name,rVolu,v(0),v(1),v(2),rot);
          refNodes->addNode(lNode,0,nm,j);
        }
        moduleNodes->addNode(lNode,nSec,i,j);
        lNode->cd();
        
        // create wires
        HMdcLayerGeomParLay& layPar=(*mdcLayGeomPar)[nSec][i][j];
        Int_t nWir=layPar.getNumWires();
        if (lRef)  copyWires(nm,nWir,nSec,i,j);
        else {
          Float_t pitch=layPar.getPitch()*10.;    // needed in mm
          Float_t width=layPar.getCatDist()*10.;  // needed in mm
          Float_t angle=layPar.getWireOrient();
          Float_t centWire=layPar.getCentralWireNr() -1; // started with 1!
          Double_t sinangle=TMath::Sin(angle*TMath::Pi()/180.);
          Double_t cosangle=TMath::Cos(angle*TMath::Pi()/180.);
          Double_t p3[3], p5[3];
          p3[1]=0.-pitch/2.;  // needed in mm
          p3[2]=0.-width/2.;
          p5[1]=pitch/2.;
          p5[2]=width/2.;
          Text_t wSubName[5], wName[8];
          sprintf(wSubName,"D%iL%iW",(i+1),(j+1));
          HGeomVolume wire;
          wire.SetName(wSubName);
          wire.setShape("BOX ");
          wire.createPoints(8);
          HGeomTransform& wt=wire.getTransform();
          HGeomRotation wr(angle,0.,0.);
          wt.setRotMatrix(wr);
          HGeomVector pos;
          wt.setTransVector(pos);

	  // mid plane of layer
          HGeomVector v0=*(layer->getPoint(0)) + *(layer->getPoint(4));
          v0*=0.5;
          HGeomVector v1=*(layer->getPoint(1)) + *(layer->getPoint(5));
          v1*=0.5;
          HGeomVector v2=*(layer->getPoint(2)) + *(layer->getPoint(6));
          v2*=0.5;
          HGeomVector v3=*(layer->getPoint(3)) + *(layer->getPoint(7));
          v3*=0.5;
          // rotate to wire coordinate system
          HGeomVector lp0=wt.transTo(v0);
          HGeomVector lp1=wt.transTo(v1);
          HGeomVector lp2=wt.transTo(v2);
          HGeomVector lp3=wt.transTo(v3);
          // left side
          Double_t dx=lp1(0)-lp0(0);
          Double_t dy=lp1(1)-lp0(1);
          Double_t dxL=pitch*dx/dy;
          Double_t xcL=lp0(0)-lp0(1)*dx/dy;  //central wire
          // right side
          dx=lp2(0)-lp3(0);
          dy=lp2(1)-lp3(1);
          Double_t dxR=pitch*dx/dy;
          Double_t xcR=lp3(0)-lp3(1)*dx/dy;  //central wire
          // bottom
          dx=lp3(0)-lp0(0);
          dy=lp3(1)-lp0(1);
          Double_t dxB=pitch*dx/dy;
          Double_t xcB=lp0(0)-lp0(1)*dx/dy;  //central wire
          // top
          dx=lp2(0)-lp1(0);
          dy=lp2(1)-lp1(1);
          Double_t dxT=pitch*dx/dy;
          Double_t xcT=lp1(0)-lp1(1)*dx/dy;  //central wire
          // wire numbers where the wire changes from sides to bottom/top
          Int_t kmin=0, kmax=nWir;
          if (angle>0) {
            kmin=(Int_t)(lp3(1)/pitch+centWire)+1;
            kmax=(Int_t)(lp1(1)/pitch+centWire);
          }
          if (angle<0) {
            kmin=(Int_t)(lp0(1)/pitch+centWire)+1;
            kmax=(Int_t)(lp2(1)/pitch+centWire);
          }

          s=shapes->selectShape(&wire);
          //done once to create the TRotMatrix
          s->calcVoluPosition(&wire,lCenter);
          tr=s->getVoluPosition();
          r=tr->getRotMatrix();
          rot=r.createTRotMatrix(wSubName,wSubName);

          for(Int_t k=0;k<nWir;k++) {
            sprintf(wName,"%s%i",wSubName,k);
            wire.SetName(wName);
            Float_t dn=k-centWire;
            if (k>=kmin && k<=kmax) {
              p3[0]=xcR+dn*dxR;  // right
              p5[0]=xcL+dn*dxL;  // left
            } else {
              if (k<kmin) {
                if (angle>0) {
                  p3[0]=xcB+dn*dxB;
                  p5[0]=xcL+dn*dxL;
                } else {
                  p3[0]=xcR+dn*dxR;
                  p5[0]=xcB+dn*dxB;
                }
              } else {
                if (angle>0) {
                  p3[0]=xcR+dn*dxR;
                  p5[0]=xcT+dn*dxT;
                } else {
                  p3[0]=xcT+dn*dxT;
                  p5[0]=xcL+dn*dxL;
                }
              }
            }
            wire.setPoint(3,p3[0],p3[1],p3[2]);
            wire.setPoint(5,p5[0],p5[1],p5[2]);
            rVolu=s->createRootVolume(&wire);
            pos.setX(-dn*pitch*sinangle);
            pos.setY(dn*pitch*cosangle);
            wt.setTransVector(pos);
            s->calcVoluPosition(&wire,lCenter);
            const HGeomVector& v=s->getVoluPosition()->getTransVector();
            TNode* wNode=new TNode(wName,wName,rVolu,v(0),v(1),v(2),rot);
            refNodes->addNode(wNode,0,nm,j,k);
            moduleNodes->addNode(wNode,nSec,i,j,k);
	      // consistency check
              // if (kmin<0 || kmax>nWir) 
              //   printf("%s  nWires: %3i   kmin: %3i   kmax: %3i  ***\n",
	      //          name,nWir,kmin,kmax);
              // Double_t a=dn*pitch;
              // if (angle>=0 && (a<=lp0(1) || a>=lp2(1)))
              //   printf("%s  wire: %3i  y:%10.3f\n",name,k,a);
              // if (angle<0 && (a<=lp3(1) || a>=lp1(1)))
              //   printf("%s  wire: %3i  y:%10.3f\n",name,k,a);
          }
        }
        mNode->cd();
      }
      masterNode->cd();
    }
  }
  return kTRUE;
}    


void HMdcEvtDisplay::copyWires(Int_t nRefMod,Int_t nWir,Int_t nSec,Int_t nMod,
                            Int_t nLay) {
  // copies the wire nodes
  for(Int_t k=0;k<nWir;k++) {
    TNode* ref=refNodes->getNode(0,nRefMod,nLay,k);
    if (ref) {
      TNode* node=copyNode(ref->GetName(),ref); 
      moduleNodes->addNode(node,nSec,nMod,nLay,k);
    }
  }
}


TNode* HMdcEvtDisplay::copyNode(const Text_t* name,TNode* ref) {
  // copies a node
  TShape* rVolu=ref->GetShape();
  Double_t dx=ref->GetX();
  Double_t dy=ref->GetY();
  Double_t dz=ref->GetZ();         
  TRotMatrix* lrot=ref->GetMatrix();
  TNode* node=new TNode(name,name,rVolu,dx,dy,dz,lrot);
  return node;
}


void HMdcEvtDisplay::clear() {
  // loops over all displays for the categories and calls their clear()
  HCatEvtDisplay* disp;
  for(Int_t i=0;i<numCatDisp;i++) {
    disp=(HCatEvtDisplay*)((*catDisplays)[i]);  
    if (disp) disp->clear();
  }
}


void HMdcEvtDisplay::draw(Int_t s,Int_t m,Int_t l) {
  // draws a module or a layer
  // (s: index of sector, m: index of module, l: index of layer) 
  TNode* node=0;
  if (l>=0) node=moduleNodes->getNode(s,m,l);
  else node=moduleNodes->getNode(s,m);
  if (node) node->Draw();
}


void HMdcEvtDisplay::drawConnectedWires(Int_t s,Int_t m,Text_t* mbo) {
  // draws all wires read out in a module or all wires connected to an MBO
  // given by its logical position
  HMdcRawStruct* p=
    (HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct");
  if (p) {
    Int_t n=(*p)[s][m].getMboIndex(mbo);
    if (n>=0) drawConnectedWires(s,m,(*p)[s][m].getMboIndex(mbo));
    else printf("no MBO at this logical position\n");
  }
}


void HMdcEvtDisplay::drawConnectedWires(Int_t s,Int_t m,Int_t mo) {
  // draws all wires read out in a module or all wires connected to an MBO
  // given by index
  // (s: index of sector, m: index of module, l: index of mbo)
  if (connectedWires) { 
    moduleNodes->clear();
    HMdcLookupGMod& cw=(*connectedWires)[s][m];
    TNode* modNode=moduleNodes->getNode(s,m);
    if (modNode) {
      TNode* node=0;
      Int_t l, c;
      if (mo>=0) {
        HMdcLookupMoth& cMoth=cw[mo];
        for(Int_t j=0;j<cMoth.getSize();j++) {
          l=cMoth[j].getNLayer();
          c=cMoth[j].getNCell();
          node=moduleNodes->getNode(s,m,l,c);
          if (node) node->SetVisibility(1);
        }
      } else {
        for(Int_t i=0;i<cw.getSize();i++) { 
          HMdcLookupMoth& cMoth=cw[i];
          for(Int_t j=0;j<cMoth.getSize();j++) {
            l=cMoth[j].getNLayer();
            c=cMoth[j].getNCell();
            node=moduleNodes->getNode(s,m,l,c);
            if (node) node->SetVisibility(1);
          }
        }
      }
      modNode->Draw();
    }
  } else {
    printf("Parameter container MdcConnectedWires not available\n");
  }
}


Int_t HMdcEvtDisplay::execute() {
  // loops over all displays for the categories and calls their execute()
  HCatEvtDisplay* disp;
  for(Int_t i=0;i<numCatDisp;i++) {
    disp=(HCatEvtDisplay*)((*catDisplays)[i]);  
    if (disp) disp->execute();
  }
  return 1;
}


TNode* HMdcEvtDisplay::getNode(Int_t s,Int_t m,Int_t l,Int_t w) {
  // returns a node
  return moduleNodes->getNode(s,m,l,w);
}


void HMdcEvtDisplay::addCenter(HGeomTransform* t,Int_t m,Int_t l) {
  // adds a center in the array 'centers'
  if (l<0) centers->AddAt(t,m);
  else centers->AddAt(t,maxRefMod+m*maxLay+l);
}


HGeomTransform* HMdcEvtDisplay::getCenter(Int_t m,Int_t l) {
  if (l<0) return (HGeomTransform*)(centers->At(m));
  else return (HGeomTransform*)(centers->At(maxRefMod+m*maxLay+l));
}
