#include "hrichfancy.h"
#include "iostream.h"
#include "TObjString.h"
#include "hades.h"
#include "hrichgeometrypar.h"
#include "hrichpad.h"
#include "TNode.h"
#include "TTRAP.h"
#include "TTUBS.h"
#include "hruntimedb.h"
#include "TRotMatrix.h"
ClassImp(HRichFancy)


HRichFancy::HRichFancy(const char* name   ="",
		       const char* title  ="",
		       Int_t active=0,Int_t resetable=0): TNamed(name,title) 
{
  fIsActive=1;
  fIsResetable=resetable;
  refreshrate=500;
  isKnownToGUI=kFALSE;
}

void HRichFancy::reset(Int_t level,Int_t count)
{
    switch (level)
    {
    case 0:   cout <<"reseting" << endl;
    break;
    case 1:   cout <<"reseting" << endl;
    break;
    case 2:   cout <<"reseting" << endl;
    break;
    default : Error("HRichFancy::reset","UNKNOWN ARGUMENT: Level= %i !",level);
    }
}
void HRichFancy::create()
{
    if(fIsActive==1)
    {
      cout << "creating" << endl;
    }
}

void HRichFancy::draw()
{
    if(fIsActive==1)
    {
      //cout << "Drawing the fancy display" << MainBrik <<endl;
      
    }
}

void HRichFancy::init()
{

  Int_t nSec=6;
  Char_t name1[20];
  Char_t title1[20];
  Char_t name2[20];
  Char_t title2[20];
  
  HRichGeometryPar* fRichGeomPar=(HRichGeometryPar*)(gHades->getRuntimeDb()->getContainer("RichGeometryParameters"));
  HRichPadTab* padstable = fRichGeomPar->getPadsPar();
  HRichPad* currentPad;
  Int_t nPads = padstable->getPadsNr();

  for(Int_t tt=0;tt<60000;tt++){
    padnr2logcoord[tt]=-1;
  }

  MainBrik = new TBRIK("MAINBRIK","MAINBRIK","void",0.,0.,0.);
  MainBrik->SetVisibility(0);

  MainNode = new TNode("MAINNODE","MAINNODE",MainBrik);
  MainNode->cd();
  MainNode->SetVisibility(-4);
   

  trapezoids = new TTRAP* [padstable->getPadsNr()*nSec];
  Float_t dz=0.5;
  for(int i=0;i<padstable->getPadsNr();i++)
    {
      currentPad = padstable->getPad(i);
      if(currentPad->getPadActive()){
 	HRichPadCorner* c1 = currentPad->getCorner(0);
 	HRichPadCorner* c2 = currentPad->getCorner(1);
 	HRichPadCorner* c3 = currentPad->getCorner(2);
 	HRichPadCorner* c4 = currentPad->getCorner(3);
	
 	Float_t tmp[8];
	
 	tmp[0] = h1(c1->getX(),c2->getX(),c3->getX(),c4->getX());
 	tmp[1] = bl1(c2->getY(),c3->getY());
 	tmp[2] = tl1(c1->getY(),c4->getY());
 	tmp[3] = alpha1(c1->getX(),c2->getX(),c1->getY(),c2->getY(),c3->getY(),c4->getY());
 	tmp[4] = h2(c1->getX(),c2->getX(),c3->getX(),c4->getX());
 	tmp[5] = bl2(c2->getY(),c3->getY());
 	tmp[6] = tl2(c1->getY(),c4->getY());
 	tmp[7] = alpha2(c1->getX(),c2->getX(),c1->getY(),c2->getY(),c3->getY(),c4->getY());
	
 	for (Int_t k=0; k<nSec; k++){
 	  sprintf (title1,"TRAP_%d_%d",i,k);
 	  sprintf (name1, "TRAP_%d_%d",i,k);
 	  trapezoids[(k*padstable->getPadsNr())+i] = new TTRAP(name1,title1,"void",dz,0,0,tmp[0],tmp[1],tmp[2],tmp[3],tmp[4],tmp[5],tmp[6],tmp[7]);
 	  trapezoids[(k*padstable->getPadsNr())+i] -> SetLineColor(k+2);
 	}
      }
      
    }

  Int_t noHelperVol = 2*nSec;
  //geoNodes = new TNode* [nSec*padstable->getPadsNr()+noHelperVol+1];
  geoNodeArray = new TObjArray(nSec*padstable->getPadsNr()+noHelperVol+1);
  //node1->SetVisibility(1);
  
  TRotMatrix ** rotmat;
  rotmat = new TRotMatrix* [7];
  rotmat[0] = new TRotMatrix("rot0","rot0",90.,90.,90.,180.,0.,0.);
  rotmat[1] = new TRotMatrix("rot1","rot1",90.,150.,90.,240.,0.,0.);
  rotmat[2] = new TRotMatrix("rot2","rot2",90.,210.,90.,300.,0.,0.);
  rotmat[3] = new TRotMatrix("rot3","rot3",90.,270.,90.,0.,0.,0.);
  rotmat[4] = new TRotMatrix("rot4","rot4",90.,330.,90.,60.,0.,0.);
  rotmat[5] = new TRotMatrix("rot5","rot5",90.,30.,90.,120.,0.,0.);
  rotmat[6] = new TRotMatrix("rot6","rot6",110.,0.,90.,90.,20.,0.); // for 3d-view
  
  Char_t cSecNodeName[20];
  Char_t cRotMatName[5];
  
  for (Int_t k=0; k<nSec; k++){
    sprintf (cSecNodeName,"ROTNODE_%d",nSec*nPads+k+1);
    sprintf (cRotMatName,"rot%d",k);
    //geoNodes[nSec*nPads+k+1] = new TNode(cSecNodeName,cSecNodeName,"MAINBRIK",0.,0.,0.,cRotMatName);//  ????????????
    geoNodeArray->AddAt(new TNode(cSecNodeName,cSecNodeName,"MAINBRIK",0.,0.,0.,cRotMatName),nSec*nPads+k+1);
  }
  
  
  //TTUBS* tubs = new TTUBS("tubs","tubs","void",6.,65.,0.,-28.485,28.485);
  TTUBS* tubs = new TTUBS("tubs","tubs","void",6.,65.,0.,-28.,28.);
  tubs->SetLineColor(0);

  for (Int_t k=0; k<nSec; k++){
    //geoNodes[nSec*nPads+k+1] -> cd();
    ((TNode*)((*geoNodeArray)[nSec*nPads+k+1]) )->cd();
    sprintf (cSecNodeName,"TUBENODE_%d",nSec*nPads+nSec+k+1);
    //geoNodes[nSec*nPads+nSec+k+1] = new TNode(cSecNodeName,cSecNodeName,"tubs",0.,0.,0.,"rot6");
    geoNodeArray->AddAt(new TNode(cSecNodeName,cSecNodeName,"tubs",0.,0.,0.,"rot6"),nSec*nPads+nSec+k+1);
  }
  
  for ( Int_t k=0; k < nSec; k++ ) {
    //geoNodes[nSec*nPads+k+nSec+1] -> cd();
    ((TNode*)((*geoNodeArray)[nSec*nPads+k+nSec+1]))->cd();
    for (Int_t i=0; i < nPads; i++) {
      currentPad = padstable->getPad(i);
      if(currentPad->getPadActive()){
	HRichPadCorner* c1 = currentPad->getCorner(0);
	HRichPadCorner* c2 = currentPad->getCorner(1);
	HRichPadCorner* c3 = currentPad->getCorner(2);
	HRichPadCorner* c4 = currentPad->getCorner(3);
	sprintf (title2,"TRAPNODE_%d_%d_s%dc%dr%d",i+1,k+1,k,currentPad->getPadX(),currentPad->getPadY());
	sprintf (name2, "TRAPNODE_%d_%d_s%dc%dr%d",i+1,k+1,k,currentPad->getPadX(),currentPad->getPadY());
	sprintf (name1, "TRAP_%d_%d",i,k);
	Float_t tmp1[2];
	tmp1[0] = yd(c1->getY(),c2->getY(),c3->getY(),c4->getY());
	tmp1[1] = xd(c1->getX(),c2->getX(),c3->getX(),c4->getX());

	//geoNodes[(k*nPads)+(i+1)] = new TNode(name2,title2,name1,tmp1[0],tmp1[1],0.);
	geoNodeArray->AddAt(new TNode(name2,title2,name1,tmp1[0],tmp1[1],0.),(k*nPads)+(i+1));

	padnr2logcoord[k*10000+currentPad->getPadX()*100+currentPad->getPadY()]=k*nPads+i+1;
	//geoNodes[(k*nPads)+(i+1)] -> SetVisibility(0);
	((TNode*)((*geoNodeArray)[(k*nPads)+(i+1)]))->SetVisibility(0);
	//geoNodes[(k*nPads)+(i+1)] -> SetLineColor( ( gRandom->Rndm(1) )*50+i );
	//geoNodes[(k*nPads)+(i+1)] -> SetLineColor(50+i);
	//geoNodes[(k*nPads)+(i+1)] -> SetLineColor(2);
      }
    }
  }
  
  
  
}

HRichFancy::~HRichFancy(){
  cout << "deleting" << endl;
  delete MainBrik;
  delete MainNode;
}

