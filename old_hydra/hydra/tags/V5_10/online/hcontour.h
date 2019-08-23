#if !defined HCONTOUR_H
#define HCONTOUR_H

//-------------------------------------------------------------------------
//
// File name:       hcontour.h
// Subject:         Evaluation of a "contour condition"
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it" 
// Date:            February 2000
//
// Remarks:         The algorithm used to evaluate if a given point is
//                  inside or outside the contour was proposed by 
//                  P.Finocchiaro (INFN-LNS,Catania, "finocchiaro@lns.infn.it")
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of a class HContour.
// The class HContour stores the contour (x,y coordinates of a given number
// of points) and evaluates if a given point (input information) stays
// inside or outside the contour
//
//-------------------------------------------------------------------------

#include <TGraph.h>
#include <TNamed.h>
#include <TArrayF.h>

class HContour : public TNamed {

 protected:
  
  Float_t       Xar[100];
  Float_t       Yar[100];
  Int_t         points;
  Bool_t        kLink;
  Bool_t        kSelfCross;
  TGraph       *graph;//!
  TArrayF       slope;
  TArrayF       offset;
  TArrayF       Xlinked;
  TArrayF       cross;

  //----------------------
  Bool_t        kDefine;
  //----------------------

 protected:

  Bool_t addPoint(Float_t x, Float_t y);
  void fillHelperArrays();
  Bool_t fillCrossArray(Float_t x, Float_t y);
  void sorting();
  
 public:
  
  HContour() : TNamed() { 
    points = 0; graph = 0; kLink = kFALSE; define();
  }
  virtual ~HContour() { 
    if(graph) { delete graph; graph = 0; }
  }
  void Paint(Option_t *t);
  virtual Int_t DistancetoPrimitive(Int_t px, Int_t py) {return 0;}
  virtual void ExecuteEvent(Int_t event, Int_t px, Int_t py);
  void setLink(Bool_t a = kTRUE);
  void deleteLastPoint();
  Bool_t isLinked() { return kLink; }
  Bool_t isInside(Float_t x, Float_t y);
  Bool_t checkSelfCross();
  void copy(TObject *hc);
  Bool_t checkLimits(Float_t xmin, Float_t xmax, Float_t ymin, Float_t ymax);

  //----------------------
  void define(Bool_t a = kTRUE) { kDefine = a; }
  void setAll();
  void show(const char* msg);
  void undo();

  //----------------------

 public:

  ClassDef(HContour,1)

};

#endif












