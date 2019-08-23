//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HMdcCal2Par
//
// Container class for the calibration parameters from Cal1 to Cal2 of the MDC
// time->distance
////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

#include "hmdccal2par.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <TF1.h>
#include <iostream.h>

ClassImp(HMdcCal2ParAngle)
ClassImp(HMdcCal2ParMod)
ClassImp(HMdcCal2ParSec)
ClassImp(HMdcCal2Par)

void HMdcCal2ParAngle::fill(HMdcCal2ParAngle& r) {
  par0=r.getPar0();
  par1=r.getPar1();
  par2=r.getPar2();
  par3=r.getPar3();
  par4=r.getPar4();
  par5=r.getPar5();
  par6=r.getPar6();
  par7=r.getPar7();
  par8=r.getPar8();
}
HMdcCal2ParMod::HMdcCal2ParMod(Int_t sec, Int_t mod, Int_t angle) {
  // constructor takes the sector, module and angle
    array = new TObjArray(angle);
    for (Int_t i=0; i<angle; ++i)
        (*array)[i] = new HMdcCal2ParAngle();
}

HMdcCal2ParMod::~HMdcCal2ParMod() {
  // destructor
  array->Delete();
  delete array;
}

HMdcCal2ParSec::HMdcCal2ParSec(Int_t sec, Int_t mod) {
  // constructor takes the sector, module number
  array = new TObjArray(mod);
  for (Int_t i=0; i<mod; i++)
        (*array)[i] = new HMdcCal2ParMod(sec,i);
}

HMdcCal2ParSec::~HMdcCal2ParSec() {
  // destructor
  array->Delete();
  delete array;
}

HMdcCal2Par::HMdcCal2Par(Int_t n) : HParSet() {
  // constructor set the name of the container to "MdcCal2Par"
  fName="MdcCal2Par";
  fTitle="cal2 calibration parameters for Mdc";
  strcpy(detName,"Mdc");

  ftimeToDistance  = new TF1("calcdistance","pol6",0,800);

  array = new TObjArray(n);
  for (Int_t i=0; i<n; i++) (*array)[i] = new HMdcCal2ParSec(i);
}

HMdcCal2Par::~HMdcCal2Par() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HMdcCal2Par::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
    HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));


  return kFALSE;
}
Float_t HMdcCal2Par::calcDistance(Int_t s,Int_t m,Float_t a,Float_t t) {
    //
    //
    sector=s;
    module=m;
    Float_t b=TMath::Abs(a);
    time=t;
  
    // only angles between 0 and 90 degree are defined
    // all other angles have to be shifted

    if(b<=90  && b>=0  ) angleDeg=b;
    if(b<=180 && b> 90 ) angleDeg=b-90;
    if(b<=270 && b> 180) angleDeg=b-180;
    if(b< 360 && b> 270) angleDeg=b-270;

    angleStep=(Int_t)(angleDeg/5);
    distance=0;
    HMdcCal2ParAngle& rAngle=(*this)[sector][module][angleStep];
    ftimeToDistance->SetParameters((rAngle.getPar0()),
				   (rAngle.getPar1()),
				   (rAngle.getPar2()),
				   (rAngle.getPar3()),
				   (rAngle.getPar4()),
				   (rAngle.getPar5()),
				   (rAngle.getPar6()));


    if(time<=(rAngle.getPar7()) && time >=0)  // non linear part
    {
	distance=ftimeToDistance->Eval(time);
    }
    else
    {
	if(time>(rAngle.getPar7()) ) // constant part
	{
	    distance=(rAngle.getPar8());
	}
	if(time<0)
	{
         distance=-1;    // put all values for negative times to -1
	}

    }

    if(time<0 || distance<0 || distance>10){
       //   cout<<s<<" "<<m<<" "<<time<<" "<< distance<< endl;
    }

    if(time>=0 && distance>0 && distance<10){
	return distance;
    }
    else
    {
        //cout<<"From HMdcCalibrater2: values of time or distance out of range"<<endl;
	return    -1;
    }
}
Int_t HMdcCal2Par::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcCal2Par::readline(const char* buf, Int_t* set) {
  // decodes one line read from ascii file I/O and calls HMdcCal2ParAngle::fill(...)
    Int_t   sec,mod,angle;
    Float_t par0,par1,par2,par3,par4,par5,par6,par7,par8;
    sscanf(buf,"%i%i%i%*s",&sec, &mod, &angle);
    sscanf(&buf[7],"%e%e%e%e%e%e%e%e%e",
	   &par0, &par1, &par2,
	   &par3, &par4, &par5,
	   &par6,&par7,&par8);
    Int_t n=sec*4+mod;
    if (!set[n]) return;
    HMdcCal2ParAngle& rAngle=(*this)[sec][mod][angle];
    rAngle.fill(par0,par1,par2,par3,par4,par5,par6,par7,par8);
    set[n]=999;
}

void HMdcCal2Par::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
	"# Cal2 Calibration parameters of the MDC\n"
	"# time->distance\n"
	"# Format:\n"
	"# sector  module  angle  par0 par1 par2 par3 par4 par5 pa6 par7 par8\n";
}

Bool_t HMdcCal2Par::writeline(char *buf, Int_t sec, Int_t mod, Int_t angle) {
  // writes one line to the buffer used by ascii file I/O
  HMdcCal2ParAngle& rAngle=(*this)[sec][mod][angle];

  sprintf(buf,"%1i %1i %2i ",sec, mod, angle);
  
  sprintf(buf,"%13e %13e %13e %13e %13e %13e %13e %13e %13e\n",
          rAngle.getPar0(),rAngle.getPar1(),rAngle.getPar2(),
          rAngle.getPar3(),rAngle.getPar4(),rAngle.getPar5(),
          rAngle.getPar6(),rAngle.getPar7(),rAngle.getPar8());
  return kTRUE;
}

void HMdcCal2Par::clear() {
  // clears the container
  for(Int_t s=0;s<getSize();s++) {
    HMdcCal2ParSec& sec=(*this)[s];
    for(Int_t m=0;m<sec.getSize();m++) {
      HMdcCal2ParMod& mod=sec[m];
      for(Int_t l=0;l<18;l++) {
          HMdcCal2ParAngle& angle=mod[l];
          angle.clear();
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}

