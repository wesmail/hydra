#include "htofsector.h"
#include <TMath.h>
#include <TPoint.h>
#include <TVirtualPad.h>

//-----------------------------------------------------------
//
// Author : Dmitry Vasiliev (LNS, Catania)
//
//-----------------------------------------------------------
//-----------------------------------------------------------
//
// HTofSector class
//
// helper class
// provides graphics in the canvas of HADES TOF Configuration panel
//
//----------------------------------------------------------------

ClassImp(HTofSector)

HTofSector::HTofSector() : TObject() {}

HTofSector::HTofSector(Int_t sec) : TObject() {

number = sec;

Float_t  x[4];
Float_t  y[4];
Float_t  x_temp;
Float_t  y_temp;
//Float_t  angle[6] = {0., 60., 120., 180., 240., 300.};
Float_t  angle[6] = {0., 300., 240., 180., 120., 60.};
Float_t L = 0.3;
Float_t l = 0.05;
Double_t deg2rad = 0.0174533;
Int_t i;

//

Float_t x_centre = 0.5;
Float_t y_centre = 0.5;

//
x[0] = -l * TMath::Tan(deg2rad*29.5);
x[1] = -x[0];
x[2] = L * TMath::Tan(deg2rad*29.5);
x[3] = -x[2];
y[0] = l;
y[1] = y[0];
y[2] = L;
y[3] = y[2];
//
for(i=0;i<4;i++) {

  x_temp = x[i];
  y_temp = y[i];

  x[i] = x_temp * TMath::Cos(deg2rad * angle[number-1]) - 
         y_temp * TMath::Sin(deg2rad * angle[number-1]);
  y[i] = x_temp * TMath::Sin(deg2rad * angle[number-1]) +
         y_temp * TMath::Cos(deg2rad * angle[number-1]);

}

for(i=0;i<4;i++) {
  x[i] += x_centre;
  y[i] += y_centre;
}

line1 = new TLine(x[0],y[0],x[1],y[1]);
line2 = new TLine(x[1],y[1],x[2],y[2]);
line3 = new TLine(x[2],y[2],x[3],y[3]);
line4 = new TLine(x[3],y[3],x[0],y[0]);

TAttFill::SetFillColor(3);
line1->SetLineColor(1);
line2->SetLineColor(1);
line3->SetLineColor(1);
line4->SetLineColor(1);

}


void HTofSector::Paint(Option_t *) {

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
gGXW->DrawFillArea(4,point);

line1->PaintLine(line1->GetX1(),line1->GetY1(),line1->GetX2(),line1->GetY2());
line2->PaintLine(line2->GetX1(),line2->GetY1(),line2->GetX2(),line2->GetY2());
line3->PaintLine(line3->GetX1(),line3->GetY1(),line3->GetX2(),line3->GetY2());
line4->PaintLine(line4->GetX1(),line4->GetY1(),line4->GetX2(),line4->GetY2());

} 






