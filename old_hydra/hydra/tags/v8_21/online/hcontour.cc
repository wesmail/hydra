//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hcontour.cc
// Subject:         Evaluation of a "contour condition"
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it" 
// Date:            February 2000
//
// Remarks:         The algorithm used to evaluate if the given point is
//                  inside or outside the contour was proposed by 
//                  P.Finocchiaro (INFN-LNS,Catania, "finocchiaro@lns.infn.it")
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HContour.
// The class HContour stores the contour (x,y coordinates of a given number
// of points) and evaluates if a given point (input information) stays
// inside or outside the contour.
//
//-------------------------------------------------------------------------
using namespace std;
#include "hcontour.h"
#include <iostream> 
#include <iomanip>
#include "TAxis.h"
#include "TVirtualPad.h"
#include "herror.h"
#include "TIterator.h"
#include "TList.h"
#include "TH2.h"
#include "TCanvas.h"

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
// This class stores the contour (x,y coordinates of a given number
// of points) and evaluates if a given point (input information) stays
// inside or outside the contour.
//
// The algorithm used to evaluate if the given point is inside or outside 
// the contour was proposed by 
// P.Finocchiaro (INFN-LNS,Catania, "finocchiaro@lns.infn.it")
//////////////////////////////////////////////////////////////////////// 

ClassImp(HContour)



//---------------------------------------------------------------------
//******  Bool_t HContour::addPoint(Float_t x, Float_t y)
//
// This function adds a new point to the contour.
//
//---------------------------------------------------------------------

Bool_t HContour::addPoint(Float_t x, Float_t y) {
//
// This function adds a new point to the contour.
//

  if(points == 100) return kFALSE;

  Xar[points] = x;
  Yar[points] = y;
  points++;

  return kTRUE;

}

  
//---------------------------------------------------------------------
//******  void HContour::deleteLastPoint()
//
// This function deletes the last defined point in the contour.
//
//---------------------------------------------------------------------

void HContour::deleteLastPoint() {
//
// This function deletes the last defined point in the contour.
//

  if(points > 0) points--;
  checkSelfCross();

}


//---------------------------------------------------------------------
//******  void HContour::setLink(Bool_t a)
//
// This function sets the flag which tells if the first and last points
// of the contour are linked or not.
//
//---------------------------------------------------------------------

void HContour::setLink(Bool_t a) {
//
// This function sets the flag which tells if the first and last points
// of the contour are linked or not.
//

  kLink = a;
  if(kLink) fillHelperArrays();
  checkSelfCross();

}


//---------------------------------------------------------------------
//******  void HContour::fillHelperArrays()
//
// This is a helper function which calculates the equations of all straight
// line pieces of the contour and stores them in arrays slope and offset.
//
//---------------------------------------------------------------------

void HContour::fillHelperArrays() {
//
// This is a helper function which calculates the equations of all straight
// line pieces of the contour and stores them in arrays slope and offset.
//

  Int_t i;
  if(points > 2) { // at least triangle

    slope.Set(points);
    offset.Set(points);
    Xlinked.Set(points+1);

    for(i=0; i< (points-1); i++) {

      if((Xar[i+1] - Xar[i]) != 0.) {     // not a vertical line

	slope[i] = (Yar[i+1] - Yar[i]) / (Xar[i+1] - Xar[i]);
	offset[i] = (Xar[i+1]*Yar[i] - Xar[i]*Yar[i+1]) / (Xar[i+1] - Xar[i]);

      }
      else {

	slope[i] = offset[i] = 999999.; // vertical line

      }
      Xlinked[i] = Xar[i];

    }

    if((Xar[0] - Xar[points-1]) != 0.) {

      slope[points-1] = (Yar[0] - Yar[points-1]) / (Xar[0] - Xar[points-1]);
      offset[points-1] = (Xar[0]*Yar[points-1] - Xar[points-1]*Yar[0]) / 
	(Xar[0] - Xar[points-1]);
    }
    else {

      slope[points-1] = offset[points-1] = 999999.; // vertical line

    }
    Xlinked[points-1] = Xar[points-1];
    Xlinked[points] = Xar[0];
  }

}


//---------------------------------------------------------------------
//******  Bool_t HContour::checkSelfCross()
//
// This function makes self cross check of the contour.
//
//---------------------------------------------------------------------

Bool_t HContour::checkSelfCross() {
//
// This function makes self cross check of the contour.
//

  if(points<4) {
    kSelfCross = kFALSE;
    return kFALSE;
  }
  fillHelperArrays();
  Int_t index;
  Int_t i;
  Float_t x;
  Float_t y;
  TArrayF Ylinked;

  Ylinked.Set(points+1);
  for(i=0; i<points; i++) Ylinked[i] = -999999.;
  for(i=0; i<points; i++) Ylinked[i] = Yar[i];
  Ylinked[points] = Yar[0];

  if(!kLink) index = points-2;
  else index = points-1;

  //find cross point

  for(i=(index-2); i>=0; i--) {

    if(slope[index] != 999999.) {
      if(slope[i] != 999999.) {
	x = (offset[i] - offset[index]) / (slope[index] - slope[i]);
	y = slope[index]*x + offset[index];
	if(((x > Xlinked[index] && x < Xlinked[index+1]) ||
	     (x > Xlinked[index+1] && x < Xlinked[index])) &&
	    ((x > Xlinked[i] && x < Xlinked[i+1]) || 
	     (x > Xlinked[i+1] && x < Xlinked[i]))) {
	  kSelfCross = kTRUE;
	  return kTRUE;
	}
      }
      else {
	x = Xar[i];
	y = slope[index]*x + offset[index];
	if(((x > Xlinked[index] && x < Xlinked[index+1]) ||
	     (x > Xlinked[index+1] && x < Xlinked[index])) &&
	    ((y > Ylinked[i] && y < Ylinked[i+1]) || 
	     (y > Ylinked[i+1] && y < Ylinked[i]))) {
	  kSelfCross = kTRUE;
	  return kTRUE;
	}
      }
    }
    else {
      x = Xar[index];
      y = slope[i]*x + offset[i];
      if(((x > Xlinked[i] && x < Xlinked[i+1]) || 
	  (x > Xlinked[i+1] && x < Xlinked[i])) &&
	 ((y > Ylinked[index] && y < Ylinked[index+1]) ||
	  (y > Ylinked[index+1] && y < Ylinked[index]))) {
	kSelfCross = kTRUE;
	return kTRUE;
      }
    }
  }

  kSelfCross = kFALSE;
  return kFALSE;

}

    

//---------------------------------------------------------------------
//******  Bool_t HContour::fillCrossArray(Float_t x, Float_t y)
//
// This is a helper function which calculates y coordinates of cross points
// of the contour pieces with the perpendicular to the x axis drawn through
// the point (x,y).
//
//---------------------------------------------------------------------

Bool_t HContour::fillCrossArray(Float_t x, Float_t y) {
//
// This is a helper function which calculates y coordinates of cross points
// of the contour pieces with the perpendicular to the x axis drawn through
// the point (x,y).
//

  Int_t i;
  Int_t count;

  count = 0;
  for(i=0; i<points; i++) {

    if((x >= Xlinked[i] && x < Xlinked[i+1]) ||
       (x < Xlinked[i] && x >= Xlinked[i+1])) count++;

  }

  if(count%2 != 0) return kFALSE;

  cross.Set(count+1);
  count = 0;

  for(i=0; i<points; i++) {

    if((x >= Xlinked[i] && x < Xlinked[i+1]) ||
       (x < Xlinked[i] && x >= Xlinked[i+1])) 
      cross[count++] = slope[i] * x + offset[i];
  }

  cross[count] = y;

  return kTRUE;

}


//------------------------------------------------------------------
//******  void HContour::sorting()
//
// This function makes sorting of the array cross by means of "insert method".
// See Herbert Schildt "Expert C++"
//
//------------------------------------------------------------------

void HContour::sorting() {
//
// This function makes sorting of the array cross by means of "insert method".
// See Herbert Schildt "Expert C++".
//

  Float_t t;
  Int_t i;
  Int_t j;

  for(i=1; i<cross.fN; ++i) {

    t = cross[i];
    for(j=(i-1); j>=0 && t>cross[j]; j--) cross[j+1] = cross[j];
    cross[j+1] = t;

  }

}
  


//---------------------------------------------------------------------
//******  Bool_t HContour::isInside(Float_t x, Float_t y)
//
// This function evaluates if the point (x,y) is inside the contour.
//
//---------------------------------------------------------------------

Bool_t HContour::isInside(Float_t x, Float_t y) {
//
// This function evaluates if the point (x,y) is inside the contour.
//

  Int_t i;
  Int_t index = 0;
  if(!fillCrossArray(x,y)) return kFALSE;
  sorting();

  for(i=0; i<cross.fN; i++) {

    if(cross[i] == y) {
      index = i;
      break;
    }
  }

  if(index%2 == 0) return kFALSE;
  
  return kTRUE;

}



//---------------------------------------------------------------------
//******  void HContour::Paint(Option_t *)
//
// This function paints the contour in the current active pad.
//
//---------------------------------------------------------------------

void HContour::Paint(Option_t *) {
//
// This function paints the contour in the current active pad.
//

  if(!gPad) return;

  if(points == 0) return;
  TObject *select = 0;
  TH2F *hist2d = 0;
  TList *list = gPad->GetListOfPrimitives();

  if(list) {
    TIterator *it = list->MakeIterator();

    while((select = it->Next()) != NULL) {
      if(!strcmp(select->ClassName(),"TH2F")) {
	hist2d = (TH2F*) select;
	Float_t xmin = hist2d->GetXaxis()->GetXmin();
	Float_t xmax = hist2d->GetXaxis()->GetXmax();
	Float_t ymin = hist2d->GetYaxis()->GetXmin();
	Float_t ymax = hist2d->GetYaxis()->GetXmax();
	if(checkLimits(xmin,xmax,ymin,ymax)) break;
	else {
	  HError::message("Histogram axis ranges do not match contour limits.");
	  //	  return;
	}
      }
    }
  }

  if(!hist2d) {
    HError::message("HContour::Paint(): No histogram found in the active pad.");
    return;
  }

  if(graph && graph->IsOnHeap()) {
    delete graph;
    graph = 0;
  }

  Int_t i;

  if(!kLink) graph = new TGraph(points);
  else graph = new TGraph(points+1);

  graph->SetName("Graph");
  graph->SetTitle("Graph");
  graph->SetLineColor(1);
  if(kSelfCross) {
    graph->SetLineColor(2);
  }
  else {
    graph->SetLineColor(4);
    //    ((TCanvas*)gPad)->FeedbackMode(kTRUE);
  }

  for(i=0; i<points; i++) graph->SetPoint(i, Xar[i], Yar[i]);
  if(kLink) graph->SetPoint(points,Xar[0],Yar[0]);

  graph->Paint("L");

}


//---------------------------------------------------------------------
//******  void HContour::ExecuteEvent(Int_t event, Int_t px, Int_t py)
//
// This function executes mouse event in the current active pad.
//
//---------------------------------------------------------------------

void HContour::ExecuteEvent(Int_t event, Int_t px, Int_t py) {
//
// This function executes mouse event in the current active pad.
//

  if(!gPad) return;

  if(kDefine) {

    switch(event) {  

    case kButton1Down:
      if(!kSelfCross) {
	addPoint(gPad->AbsPixeltoX(px),gPad->AbsPixeltoY(py));
	setLink(kFALSE);
      }
      break;

    case kButton1Double:
      if(!kSelfCross) {
	deleteLastPoint();
	setLink();
      }
      break;

    }
  }
  else {

    /*    if(kLink && !kSelfCross) {

      switch(event) {  

      case kMouseMotion:

	if(isInside(gPad->AbsPixeltoX(px),gPad->AbsPixeltoY(py))) 
	  show("inside");
	else show("outside");

	break;

      }
      }*/
  }
  gPad->Modified();

}

  
void HContour::show(const Char_t* msg) {
//
// This function prints the message msg.
// Was used at the prototyping stage.
//

  fprintf(stderr, "%s", "          ");
  fprintf(stderr, "\r%s",msg);

}


void HContour::undo() {
//
// This function makes one step back during the contour definition.
//

  if(!gPad) return;
  define();
  if(isLinked()) setLink(kFALSE);
  else deleteLastPoint();
  gPad->Modified();
  gPad->Update();

}


//---------------------------------------------------------------------
//******  void HContour::setAll()
//
// This function connects the last point of the contour with the first one.
//
//---------------------------------------------------------------------

void HContour::setAll() {
//
// This function connects the last point of the contour with the first one.
//

  if(!gPad) return;
  if(kDefine && !checkSelfCross()) {
    setLink();
    gPad->Modified();
    gPad->Update();
  }

}

  
//---------------------------------------------------------------------
//******  void HContour::copy(TObject *hc)
//
// This function copies contour to hc.
//
//---------------------------------------------------------------------

void HContour::copy(TObject *hc) {
//
// This function copies contour to hc.
//

  if(!hc) return;
  TNamed::Copy(*hc);
  Int_t i;
  for(i=0; i<100; i++) {
    (*((HContour*)hc)).Xar[i] = Xar[i];
    (*((HContour*)hc)).Yar[i] = Yar[i];
  }
  ((HContour*)hc)->points = points;
  ((HContour*)hc)->kLink  = kLink;
  ((HContour*)hc)->kSelfCross = kSelfCross;
  ((HContour*)hc)->kDefine = kDefine;
  slope.Copy(((HContour*)hc)->slope);
  offset.Copy(((HContour*)hc)->offset);
  Xlinked.Copy(((HContour*)hc)->Xlinked);
  cross.Copy(((HContour*)hc)->cross);

}


//---------------------------------------------------------------------
//******  Bool_t HContour::checkLimits(Float_t xmin, Float_t xmax,
//                                     Float_t ymin, Float_t ymax)
//
// This function checks if the contour is inside the area given by 
// xmin, ymin, xmax, ymax. This function is called from Paint(Option_t *)
//
//---------------------------------------------------------------------

Bool_t HContour::checkLimits(Float_t xmin, Float_t xmax, 
			     Float_t ymin, Float_t ymax) {
//
// This function checks if the contour is inside the area given by 
// xmin, ymin, xmax, ymax. This function is called from Paint(Option_t *)
//


  for(Int_t i=0; i<points; i++) {

    if(Xar[i] < xmin || Xar[i] > xmax || Yar[i] < ymin || Yar[i] > ymax) 
      return kFALSE;
  }

  return kTRUE;

}








