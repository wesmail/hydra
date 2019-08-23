#include "htofgeom.h"
#include "htofrod.h"
#include "htofeventdisplay.h"
#include <iostream.h>
#include <TVirtualPad.h>
#include <TStyle.h>
#include <TGaxis.h>
#include <TText.h>

//-------------------------------------------------------
//
// Author : Dmitry Vasiliev (LNS, Catania)
//
//-------------------------------------------------------
//-------------------------------------------------------
//
// HTofGeom class
//
// helper class which provides all graphics in the canvas of
// TOF Event Display panel
//
//-------------------------------------------------------

ClassImp(HTofGeom)

Int_t totalHits;

HTofGeom::HTofGeom() : TObject() {

  pArray = new TObjArray(384);
  kSetMax = kFALSE;

}


void HTofGeom::addSector(Int_t sec) { 

  Int_t k,j;
  for(k=0; k<8; k++) {

    for(j=0;j<8;j++) {

      pArray->AddAt(new HTofRod(sec+1,k+1,j+1), 64*sec+8*k+j);
    }
  }

}


void HTofGeom::Paint(Option_t *) {

  PaintPalette();
  pArray->Paint();

}


void HTofGeom::setLoopColors() {

  Int_t i;
  Int_t entries;
  Int_t index;
  Int_t colors[8] = { 2, 3, 4, 5, 6, 8, 7, 41 };
  
  HTofHisto *pTofHisto;
  HTofRod *pRod;

  if(!kSetMax) calcEntriesMax();

  for(i=0; i<384;i++) {
    pTofHisto = (HTofHisto*) gTofDisplay->getHistoCont()->At(i);
    if(pTofHisto) {
      entries = (Int_t) pTofHisto->halfs->GetEntries();
      pRod = (HTofRod*) pArray->At(i);
      if(pRod) {
	index = entries/(entriesMax/8);
	if(index > 7) index = 7;
	if(entries == 0) pRod->SetFillColor(1);
	else pRod->SetFillColor(colors[index]);
      }
    }
  }

}


void HTofGeom::calcEntriesMax() {

  Int_t i;
  Int_t entries;
  HTofHisto *pTofHisto;


  totalHits = 0;
  entriesMax = 8;

  for(i=0; i<384; i++) {
    pTofHisto = (HTofHisto*) gTofDisplay->getHistoCont()->At(i);
    if(pTofHisto) {
      entries = (Int_t) pTofHisto->halfs->GetEntries();
      totalHits += entries;
      if(entries > entriesMax) entriesMax = entries;
    }
  }
  while(entriesMax%8 != 0) entriesMax++;

}


void HTofGeom::setEntriesMax(Int_t amax) {

  if(amax < 8) entriesMax = 8;
  else entriesMax = amax;
  while(entriesMax%8 != 0) entriesMax++;
  kSetMax = kTRUE;

}


Int_t HTofGeom::DistancetoPrimitive(Int_t px, Int_t py) {

  //Int_t i;

Int_t retval = 9999;

if(gTofDisplay->isZoom()) retval = 0;

/*

for(i=0;i<384;i++) {

  if(pArray->At(i)) {
    HTofRod *p = (HTofRod*) pArray->At(i);
    if(!p->DistancetoPrimitive(px,py)) retval = 0;
    else p->SetFillColor(5);
 }
}

//set to default color in case of mouse cursor not on the Tof area

if(retval) {
  gPad->Modified();
  gPad->Update();

  if(gTofDisplay->getListSec()->GetSelectedEntry()) {
    gTofDisplay->getListSec()->GetSelectedEntry()->Activate(kFALSE);
  }
  if(gTofDisplay->getListMod()->GetSelectedEntry()) {
    gTofDisplay->getListMod()->GetSelectedEntry()->Activate(kFALSE);
  }
  if(gTofDisplay->getListRod()->GetSelectedEntry()) {
    gTofDisplay->getListRod()->GetSelectedEntry()->Activate(kFALSE);
  }
}


//cout << "In Tof::DistancetoPrimitive()" << endl;
*/
return retval;

}


void HTofGeom::ExecuteEvent(Int_t event, Int_t px, Int_t py) {

  static Int_t px_start;
  static Int_t py_start;

  static Int_t px1;
  static Int_t py1;

  Float_t  x_left, x_right;
  Float_t  y_bot, y_top;
  Float_t  temp;

  if(gTofDisplay->isZoom()) {

    switch(event) {

    case kButton1Down:

      px_start = px;
      py_start = py;
      px1 = px;
      py1 = py;

    case kButton1Motion:

      gGXW->DrawLine(px_start,py_start,px1,py_start);
      gGXW->DrawLine(px_start,py_start,px,py_start);
      gGXW->DrawLine(px1,py_start,px1,py1);
      gGXW->DrawLine(px,py_start,px,py);
      gGXW->DrawLine(px_start,py_start,px_start,py1);
      gGXW->DrawLine(px_start,py_start,px_start,py);
      gGXW->DrawLine(px_start,py1,px1,py1);
      gGXW->DrawLine(px_start,py,px,py);
      px1 = px;
      py1 = py;
      break;

    case kButton1Up:

      x_left = gPad->AbsPixeltoX(px_start);
      x_right = gPad->AbsPixeltoX(px1);
      y_top = gPad->AbsPixeltoY(py_start);
      y_bot = gPad->AbsPixeltoY(py1);

      if((x_left == x_right) || (y_bot == y_top)) break;
      if(x_left > x_right) {
	temp = x_right;
	x_right = x_left;
	x_left = temp;
      }
      if(y_bot > y_top) {
	temp = y_top;
	y_top = y_bot;
	y_bot = temp;
      }
      gPad->Range(x_left,y_bot,x_right,y_top);
      gPad->Modified();
      break;

    default:
      break;
    }
  }

/*
Int_t i;

 for(i=0;i<384;i++) {

   if(pArray->At(i)) {
    
     HTofRod *p = (HTofRod*) pArray->At(i);
     if(p->getStatus()) {
       p->ExecuteEvent(event,px,py);
       gTofDisplay->getListSec()->Select(p->getSector());
       gTofDisplay->getListMod()->Select(p->getModule());
       gTofDisplay->getListRod()->Select(p->getStrip());
     }
   }
 }
*/
}


void HTofGeom::PaintPalette() {

  Float_t xmin = 0.02;
  Float_t xmax = 0.98;
  Float_t ymin = 0.05;
  Float_t ymax = 0.08;
  Int_t i;
  char tmp[20];

  Int_t col_ar[8] = { 2, 3, 4, 5, 6, 8, 7, 41 };

  gStyle->SetPalette(8,col_ar);

  Int_t ncolors = gStyle->GetNumberOfColors();
  Float_t dx = (xmax-xmin)/ncolors;
  for (i=0; i<ncolors; i++) {
    SetFillColor(gStyle->GetColorPalette(i));
    TAttFill::Modify();
    gPad->PaintBox(xmin+i*dx,ymin,xmin+(i+1)*dx,ymax);
  }

  sprintf(tmp,"%s%i","scale  ",entriesMax);
  TText *text = new TText(0.80,0.01,tmp);
  text->SetTextFont(72);
  text->SetTextSize(0.03);
  text->SetTextColor(10);
  text->Paint();
  delete text;

  sprintf(tmp,"%s%i","hits  ",totalHits);
  text = new TText(0.1, 0.01, tmp);
  text->SetTextFont(72);
  text->SetTextSize(0.03);
  text->SetTextColor(10);
  text->Paint();
  delete text;

  //  TGaxis *axis = new TGaxis();
  //  Int_t ndiv = 1;
  //  Float_t wmin = 0.;

  //  cout << "EntriesMax = " << entriesMax << endl;

  //  Float_t wmax = entriesMax;
  //  axis->PaintAxis(xmin,ymin,xmax,ymin,wmin,wmax,ndiv,"+L");
  //  delete axis;

}












