using namespace std;
#include "htofrod.h"
#include "htofeventdisplay.h"
#include <iostream> 
#include <iomanip>
#include "TMath.h"
#include "TVirtualPad.h"
#include "TPoint.h"
#include "htofgeom.h"

//-------------------------------------------------------------
//
// Author : Dmitry Vasiliev (LNS, Catania)
//
//-------------------------------------------------------------
//-------------------------------------------------------------
//
// HTofRod class
//
// helper class, provides graphics for rods
// used by HTofGeom
//
//-------------------------------------------------------------

ClassImp(HTofRod)

HTofRod::HTofRod() : TObject() {}

HTofRod::HTofRod(Int_t sec, Int_t mod, Int_t rod) : TObject() {

sector = sec;
module = mod;
strip = rod;
kActive = kFALSE;

Float_t  x[4];
Float_t  y[4];
Float_t  x_temp;
Float_t  y_temp;
//Float_t  angle[6] = {0., 60., 120., 180., 240., 300.};
Float_t  angle[6] = {0., 300., 240., 180., 120., 60.};
Float_t ratio = 0.048;
Float_t width = 0.005;
Double_t deg2rad = 0.0174533;
Int_t i;
//
Float_t distance = ratio * (9 - mod) + width * (9 - rod);

//

Float_t x_centre = 0.5;
//Float_t y_centre = 0.5;
Float_t y_centre = 0.55;
//
x[0] = -distance * TMath::Tan(deg2rad*29.5);
x[1] = -x[0];
x[2] = x[1];
x[3] = x[0];
y[0] = distance;
y[1] = y[0];
y[2] = y[1] + width;
y[3] = y[2];
//

for(i=0;i<4;i++) {

  x_temp = x[i];
  y_temp = y[i];

  x[i] = x_temp * TMath::Cos(deg2rad * angle[sec-1]) - 
         y_temp * TMath::Sin(deg2rad * angle[sec-1]);
  y[i] = x_temp * TMath::Sin(deg2rad * angle[sec-1]) +
         y_temp * TMath::Cos(deg2rad * angle[sec-1]);

}

for(i=0;i<4;i++) {
  x[i] += x_centre;
  y[i] += y_centre;
}

line1 = new TLine(x[0],y[0],x[1],y[1]);
line2 = new TLine(x[1],y[1],x[2],y[2]);
line3 = new TLine(x[2],y[2],x[3],y[3]);
line4 = new TLine(x[3],y[3],x[0],y[0]);

TAttFill::SetFillColor(1);
line1->SetLineColor(10);
line2->SetLineColor(10);
line3->SetLineColor(10);
line4->SetLineColor(10);

}


void HTofRod::Paint(Option_t *) {

  TAttFill::Modify();

  TPoint *point = new TPoint[4];
  point[0].SetX(gPad->XtoAbsPixel(line1->GetX1()));
  point[0].SetY(gPad->YtoAbsPixel(line1->GetY1()));
  point[1].SetX(gPad->XtoAbsPixel(line2->GetX1()));
  point[1].SetY(gPad->YtoAbsPixel(line2->GetY1()));
  point[2].SetX(gPad->XtoAbsPixel(line3->GetX1()));
  point[2].SetY(gPad->YtoAbsPixel(line3->GetY1()));
  point[3].SetX(gPad->XtoAbsPixel(line4->GetX1()));
  point[3].SetY(gPad->YtoAbsPixel(line4->GetY1()));
  gVirtualX->DrawFillArea(4,point);

  if(GetFillColor() != 1) {  
    line1->SetLineColor(GetFillColor());
    line2->SetLineColor(GetFillColor());
    line3->SetLineColor(GetFillColor());
    line4->SetLineColor(GetFillColor());
  }
  else {
    line1->SetLineColor(10);
    line2->SetLineColor(10);
    line3->SetLineColor(10);
    line4->SetLineColor(10);
  }    

line1->PaintLine(line1->GetX1(),line1->GetY1(),line1->GetX2(),line1->GetY2());
line2->PaintLine(line2->GetX1(),line2->GetY1(),line2->GetX2(),line2->GetY2());
line3->PaintLine(line3->GetX1(),line3->GetY1(),line3->GetX2(),line3->GetY2());
line4->PaintLine(line4->GetX1(),line4->GetY1(),line4->GetX2(),line4->GetY2());

}


Int_t HTofRod::DistancetoPrimitive(Int_t px, Int_t py) {

return 9999;
Int_t l1,l2,l3,l4;

l1 = line1->DistancetoLine(px,py,line1->GetX1(),line1->GetY1(),
			   line1->GetX2(),line1->GetY2());
l2 = line2->DistancetoLine(px,py,line2->GetX1(),line2->GetY1(),
			   line2->GetX2(),line2->GetY2());
l3 = line3->DistancetoLine(px,py,line3->GetX1(),line3->GetY1(),
			   line3->GetX2(),line3->GetY2());
l4 = line4->DistancetoLine(px,py,line4->GetX1(),line4->GetY1(),
			   line4->GetX2(),line4->GetY2());

if(l1<2&&l3<2) {
  kActive = kTRUE;
  return 0;
}
else {
  kActive = kFALSE;
  return 9999;
}

}


void HTofRod::ExecuteEvent(Int_t event, Int_t px, Int_t py) {

  SetFillColor(2);
  gPad->Modified();
  gPad->Update();
  
  switch(event) {

  case kButton1Down:

    break;
    
  case kButton1Up:
    //    gTofDisplay->SendMessage(gTofDisplay,MK_MSG(kC_COMMAND,kCM_BUTTON),
    //			     M_PICK,0);


  default:
    break;
  }

}








