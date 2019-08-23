//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HMdcCal2ParSim
//
// Container class for the calibration parameters from Cal1 to Cal2 of the MDC
// distance -> time
////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

#include "hmdccal2parsim.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <TF1.h>
#include <iostream.h>

ClassImp(HMdcCal2ParAngleSim)
ClassImp(HMdcCal2ParModSim)
ClassImp(HMdcCal2ParSecSim)
ClassImp(HMdcCal2ParSim)

void HMdcCal2ParAngleSim::fill(HMdcCal2ParAngleSim& r) {
  par0=r.getPar0();
  par1=r.getPar1();
  par2=r.getPar2();
  par3=r.getPar3();
  par4=r.getPar4();
  par5=r.getPar5();
  par6=r.getPar6();
  par7=r.getPar7();
  par8=r.getPar8();
  par9=r.getPar9();
  par10=r.getPar10();

}
HMdcCal2ParModSim::HMdcCal2ParModSim(Int_t sec, Int_t mod, Int_t angle) {
  // constructor takes the sector, module and angle
    array = new TObjArray(angle);
    for (Int_t i=0; i<angle; ++i)
        (*array)[i] = new HMdcCal2ParAngleSim();
}

HMdcCal2ParModSim::~HMdcCal2ParModSim() {
  // destructor
  array->Delete();
  delete array;
}

HMdcCal2ParSecSim::HMdcCal2ParSecSim(Int_t sec, Int_t mod) {
  // constructor takes the sector, module number
  array = new TObjArray(mod);
  for (Int_t i=0; i<mod; i++)
        (*array)[i] = new HMdcCal2ParModSim(sec,i);
}

HMdcCal2ParSecSim::~HMdcCal2ParSecSim() {
  // destructor
  array->Delete();
  delete array;
}

HMdcCal2ParSim::HMdcCal2ParSim(Int_t n) : HParSet() {
  // constructor set the name of the container to "MdcCal2ParSim"
  fName="MdcCal2ParSim";
  fTitle="cal2 calibration parameters for Mdc [distance->time]";
  strcpy(detName,"Mdc");

  fdistanceToTime   = new TF1("calctime","pol5",0,800);
  fdistanceToTimeLin= new TF1("calctimeLin","pol2",0,800);

  array = new TObjArray(n);
  for (Int_t i=0; i<n; i++) (*array)[i] = new HMdcCal2ParSecSim(i);
}

HMdcCal2ParSim::~HMdcCal2ParSim() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HMdcCal2ParSim::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Float_t HMdcCal2ParSim::calcTime(Int_t s,Int_t m,Float_t a,Float_t d) {
    //
    //
    sector=s;
    module=m;
    distance=d;
    Float_t b=TMath::Abs(a);

    // only angles between 0 and 90 degree are defined
    // all other angles have to be shifted
    /*
    if(b<0   && b>=-90 ) angleDeg=b+90;
    if(b<-90 && b>=-180) angleDeg=b+180;
    if(b<=90 && b>= 0  ) angleDeg=b;
    */
    if(b<=90  && b>=0  ) angleDeg=b;
    if(b<=180 && b> 90 ) angleDeg=b-90;
    if(b<=270 && b> 180) angleDeg=b-180;
    if(b< 360 && b> 270) angleDeg=b-270;


    angleStep=(Int_t)(angleDeg/5);
    //cout<<a<<" "<<"Sim angle "<<angleDeg<<" step "<<angleStep<<endl;
    time=0;
    HMdcCal2ParAngleSim& rAngle=(*this)[sector][module][abs(angleStep)];

    fdistanceToTime->SetParameters((rAngle.getPar0()),
				   (rAngle.getPar1()),
				   (rAngle.getPar2()),
				   (rAngle.getPar3()),
				   (rAngle.getPar4()),
				   (rAngle.getPar5()));

    fdistanceToTimeLin->SetParameters((rAngle.getPar7()),
				     (rAngle.getPar8()));

    if(distance <= (rAngle.getPar6()) && distance>=0)  // nonlinear part
    {
	time=fdistanceToTime->Eval(distance);
    }
    else
    {
	if(distance <= (rAngle.getPar9()) ) // linear part
	{
	    time=fdistanceToTimeLin->Eval(distance);
	}
	else if(distance > (rAngle.getPar9()) ) // constant part
	{
	    time= (rAngle.getPar10());
	}
    }


    if(time<0 || distance<0 || distance>10){
	 // cout<<"from cal2parsim  s "<<s<<" m "<<m<<" time "<<time<<" dist "<< distance<< endl;
    }
    if(time>=0 && distance<10 && distance >= 0){
	return time;
    }
    else
    {
	//cout<<"From HMdcCalibrater2 : values of time or distance out of range"<<endl;
	return    -1;
    
    }
}

Int_t HMdcCal2ParSim::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcCal2ParSim::readline(const char* buf, Int_t* set) {
  // decodes one line read from ascii file I/O and calls HMdcCal2ParAngleSim::fill(...)
  Int_t   sec,mod,angle;
  Float_t par0,par1,par2,par3,par4,par5,par6,par7,par8,par9,par10;
  sscanf(buf,"%i%i%i%*s",&sec, &mod, &angle);

  sscanf(&buf[7],"%e%e%e%e%e%e%e%e%e%e%e",
         &par0, &par1, &par2,
	 &par3, &par4, &par5,
	 &par6, &par7, &par8,
	 &par9, &par10 );
  Int_t n=sec*4+mod;
  if (!set[n]) return;
  HMdcCal2ParAngleSim& rAngle=(*this)[sec][mod][angle];
  rAngle.fill(par0,par1,par2,par3,par4,par5,par6,par7,par8,par9,par10);
  set[n]=999;
}

void HMdcCal2ParSim::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
	"# Cal2 Calibration parameters of the MDC\n"
	"# distance->time\n"
	"# Format:\n"
	"# sector  module  angle  par0 par1 par2 par3 par4 par5 pa6 par7 par8 par9 par10\n";
}

Bool_t HMdcCal2ParSim::writeline(char *buf, Int_t sec, Int_t mod, Int_t angle) {
  // writes one line to the buffer used by ascii file I/O
  HMdcCal2ParAngleSim& rAngle=(*this)[sec][mod][angle];
  sprintf(buf,"%1i %1i %2i ",sec, mod, angle);
   
  sprintf(buf,"%13e %13e %13e %13e %13e %13e %13e %13e %13e %13e %13e\n",
          rAngle.getPar0(),rAngle.getPar1(),rAngle.getPar2(),
          rAngle.getPar3(),rAngle.getPar4(),rAngle.getPar5(),
	  rAngle.getPar6(),rAngle.getPar7(),rAngle.getPar8(),
	  rAngle.getPar9(),rAngle.getPar10());
  return kTRUE;
}

void HMdcCal2ParSim::clear() {
  // clears the container
  for(Int_t s=0;s<getSize();s++) {
    HMdcCal2ParSecSim& sec=(*this)[s];
    for(Int_t m=0;m<sec.getSize();m++) {
      HMdcCal2ParModSim& mod=sec[m];
      for(Int_t l=0;l<18;l++) {
          HMdcCal2ParAngleSim& angle=mod[l];
          angle.clear();
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}

