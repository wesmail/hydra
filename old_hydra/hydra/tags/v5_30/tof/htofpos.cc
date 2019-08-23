////////////////////////////////////////////////////////////////////////////
//  HTofPos
// 
//  Conversion of position to the laboratory system for TOF
//
//  last modified on 11/04/00  by M.Benovic  (IP SAS)
////////////////////////////////////////////////////////////////////////////
//
//  This is a very simple tool which allows to convert the position in TOF
//  provided by TOF calibrator (sector,module,cell,x-position) to the laboratory
//  system with the origin [0,0,0] in the target
//
// To use this tool one have to first obtain the information from calibrator
// and than call one of the getTofLabX, getTofLabY or getTofLabZ function
// Example:
//
//       tofcell=tofcaltrack->getCell();
//       tofmod=tofcaltrack->getModule();
//       tofsec=tofcaltrack->getSector();
//       tofxpos=tofcaltrack->getXpos();
//       tofxlab=toflab.getTofLabX(tofsec,tofmod,tofcell,tofxpos);
//       tofylab=toflab.getTofLabY(tofsec,tofmod,tofcell,tofxpos);
//       tofzlab=toflab.getTofLabZ(tofsec,tofmod,tofcell,tofxpos);
//
//    where "tofcaltrack" is an iterator over HTofHitSim category
//    and "toflab" is an object of the type HTofPos
//
//  IMPORTANT:  Don't use this tool directly with the HGeant tof-data !!!
//  HGeant enumerates modules and cells in reverse order. To use this tool with
//  HGeant data one have to first change the enumeration:
//             module = 22 - geant_tof_module
//             cell = 7 - geant_tof_cell
//
//////////////////////////////////////////////////////////////////////////////////
#include "htofpos.h"

ClassImp(HTofPos)

HTofPos::HTofPos(void) {
  tofLabX = 0.0;                      // x position in lab. system (in mm)
  tofLabY = 0.0;                      // y position in lab. system (in mm)
  tofLabZ = 0.0;                      // z position in lab. system (in mm)
  gTofPos = this;
}

HTofPos::~HTofPos(void) {
}

///////////////////////////////////////////////////////////////////////////
// transformation routine [s,m,c,x-pos] -> [xlab,ylab,zlab]

void HTofPos::toftranslab(Int_t &s, Int_t &m, Int_t &c, Float_t &xhit) {

Float_t x0,y0,z0;
Float_t ah[8],mi[8],dist[8],thick,mi_temp,dist_temp,alpha,betha,lt;

///////////////////////////////////////////////////////////////////////////////////
//  Definition of the TOF lab. geometry (taken from technical drawing - march '96)
//  geometry is related to the sector No.0 (upper one)
//  for other sectors the rotation is applicated
//
//  ah[0-7]   :  angle between beam line and lower edge of the each module
//  mi[0-7]   :  angle between upper and lower edge of the each module
//  dist[0-7] :  distance between [0,0,0] lab. point and the centre of each module

ah[0]=82.56;ah[1]=76.18;ah[2]=69.7;ah[3]=62.75;ah[4]=58.41;ah[5]=54.0;ah[6]=49.53;ah[7]=45.;
mi[0]=6.28;mi[1]=6.38;mi[2]=6.48;mi[3]=6.95;mi[4]=4.34;mi[5]=4.41;mi[6]=4.47;mi[7]=4.53;
dist[0]=2235.;dist[1]=2200.;dist[2]=2165.;dist[3]=2130.;dist[4]=2165.;dist[5]=2135.;dist[6]=2105.;dist[7]=2075.;
////////////////////////////////////////////////////////////////////////////////////

// transformation DEG -> RAD only for calculated module
ah[m]=ah[m]*TMath::Pi()/180.;
mi[m]=mi[m]*TMath::Pi()/180.;


// determination of the scintilator thickness
  if(m<4)
    thick = 30.;        //  bigger scintilator
   else
    thick = 20.;        //  smaller scintilator

// angle between the centre of module and hit scintilator (y direction)
  mi_temp = atan((fabs(c-3.5)*thick)/(dist[m]-thick/2.));
  if(c>3) mi_temp = - mi_temp;

// distance between [0,0,0] lab. point and hitted scintilator
  dist_temp = sqrt(dist[m]*dist[m]+pow(fabs(c-3.5)*thick,2));

// angle between beam line and hitted scintilator (y direction)
  alpha = ah[m] + mi[m]/2. + mi_temp;

// position of hit in lab. system for sector No.0 (upper)
  x0 = xhit;
  y0 = sin(alpha)*dist_temp;
  z0 = cos(alpha)*dist_temp;

// angle between y-axis and hit in x-y plane
  betha = atan(x0/y0);

// distance between beam line and hit in x-y plane
  lt = sqrt(x0*x0 + y0*y0);

// rotation of the hit according to the sector number (s)
  x0 = lt * sin( betha - s*TMath::Pi()/3.);
  y0 = lt * cos( betha - s*TMath::Pi()/3.);

// exported vector of [x,y,z] position of hit in lab. frame
  position[1] = x0;
  position[2] = y0;
  position[3] = z0;

  return;
}
////////////////////////////////////////////////////////////////////////////////

// public function provided x_lab position of TOF hit
double HTofPos::getTofLabX(Int_t sec, Int_t mod, Int_t cell, Float_t xhitpos) {
  s = sec;
  m = mod;
  c = cell;
  xhit = xhitpos;

  toftranslab(s,m,c,xhit);
  tofLabX = position[1];

  return tofLabX;
}

// public function provided x_lab position of TOF hit
double HTofPos::getTofLabY(Int_t sec, Int_t mod, Int_t cell, Float_t xhitpos) {
  s = sec;
  m = mod;
  c = cell;
  xhit = xhitpos;

  toftranslab(s,m,c,xhit);
  tofLabY = position[2];

  return tofLabY;
}

// public function provided x_lab position of TOF hit
double HTofPos::getTofLabZ(Int_t sec, Int_t mod, Int_t cell, Float_t xhitpos) {
  s = sec;
  m = mod;
  c = cell;
  xhit = xhitpos;

  toftranslab(s,m,c,xhit);
  tofLabZ = position[3];

  return tofLabZ;
}


HTofPos *gTofPos;





