//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HMdcCal2ParSim
//
// Container class for the calibration parameters from Cal1 to Cal2 of the MDC
// distance -> time.Contains functions for calulating distance->time for track fitter
// Digitizer
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
#include <TMath.h>

#include <iostream.h>

ClassImp(HMdcCal2ParAngleSim)
ClassImp(HMdcCal2ParModSim)
ClassImp(HMdcCal2ParSecSim)
ClassImp(HMdcCal2ParSim)



void HMdcCal2ParAngleSim::fillTime1(HMdcCal2ParAngleSim& r) {
    for(Int_t i=0;i<100;i++)
    {
	drifttime1[i]=r.drifttime1[i];
    }
}
void HMdcCal2ParAngleSim::fillTime2(HMdcCal2ParAngleSim& r) {
    for(Int_t i=0;i<100;i++)
    {
	drifttime2[i]=r.drifttime2[i];
    }
}
void HMdcCal2ParAngleSim::fillTime1Error(HMdcCal2ParAngleSim& r) {
    for(Int_t i=0;i<100;i++)
    {
	drifttime1Err[i]=r.drifttime1Err[i];
    }
}
void HMdcCal2ParAngleSim::fillTime2Error(HMdcCal2ParAngleSim& r) {
    for(Int_t i=0;i<100;i++)
    {
	drifttime2Err[i]=r.drifttime2Err[i];
    }
}


HMdcCal2ParModSim::HMdcCal2ParModSim(Int_t sec, Int_t mod, Int_t angle) {
  // constructor takes the sector, module and angle
    array = new TObjArray(angle);
    for (Int_t i=0; i<angle; ++i)
        array->AddAt(new HMdcCal2ParAngleSim(),i);
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
        array->AddAt(new HMdcCal2ParModSim(sec,i),i);
}

HMdcCal2ParSecSim::~HMdcCal2ParSecSim() {
  // destructor
  array->Delete();
  delete array;
}

HMdcCal2ParSim::HMdcCal2ParSim(const char* name,const char* title,
                         const char* context,Int_t n)
               : HParSet(name,title,context) {
  // constructor
  strcpy(detName,"Mdc");
  if (gHades) {
      fMdc = (HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  } else {
      fMdc = 0;
  }

  array = new TObjArray(n);
  for (Int_t i=0; i<n; i++) array->AddAt(new HMdcCal2ParSecSim(i),i);
  linecounter=0;
  linecounterwrite=0;
  type=0;
  }

HMdcCal2ParSim::~HMdcCal2ParSim() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HMdcCal2ParSim::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
    linecounter=0;
    linecounterwrite=0;
    type=0;

    HDetParIo* input=inp->getDetParIo("HMdcParIo");
    if (input) return (input->init(this,set));
    return kFALSE;
}

Float_t HMdcCal2ParSim::calcTime(Int_t s,Int_t m,Float_t a,Float_t d) {
    // This function calculates the drift time for a given distance
    // to sense wire.A Interpolation between the two closest angle steps
    // is performed and the drift time value returned.
    //
    sector=s;
    module=m;
    distance=d;

    
    dmin=(Int_t)(distance/0.1);

    Float_t b=TMath::Abs(a);

    time=0;

    finaltime=0;

    // only angles between 0 and 90 degree are defined
    // all other angles have to be shifted

    if(b<=90  && b>=0)
    {
        angleDeg=90-b;           // 90 deg impact ->0
    }
    else
    {
	if(b<=180 && b> 90 ) angleDeg=90-(180-b);
	if(b<=270 && b> 180) angleDeg=90-(270-b);
	if(b< 360 && b> 270) angleDeg=90-(360-b);
    }
    
    angleStep=((Int_t)((angleDeg)/5));      // this number is need to look up the matrix

    if(angleStep==18)angleStep=17;      // if angle==90, angleStep=18 ->not defined
  
    HMdcCal2ParAngleSim& rAngle =(*this)[s][m][angleStep];// pointer to the first set

    if(distance>10)
    {
	finaltime= (distance-10)*20 + rAngle.drifttime1[99];
	return finaltime;
    }

    HMdcCal2ParAngleSim rAngle2;

    if(angleStep<17)
    {
	rAngle2=(*this)[s][m][angleStep+1];               // pointer to the second set
    }
    else
    {
	rAngle2=(*this)[s][m][angleStep];
    }

    Float_t timeResolution=0;

    //###########################################################################
    Float_t y1=0;
    Float_t y2=0;
    Float_t y3=0;
    Float_t y4=0;
    Float_t t=0;
    Float_t u=0;
    Float_t anglesearch=0;
    Float_t distancesearch=0;
    //#################################### calc drift time ######################
    y1=rAngle.drifttime1 [dmin];
    y2=rAngle2.drifttime1[dmin];
    y3=rAngle2.drifttime1[dmin+1];
    y4=rAngle.drifttime1 [dmin+1];


    anglesearch=angleDeg;

    distancesearch=distance;

    t=(anglesearch - (angleStep*5) )/(Float_t)( ((angleStep+1)*5)- (angleStep*5) );
    u=(distancesearch - (dmin*0.1) )/(Float_t)((dmin+1-dmin)*0.1);

    time=( (1-t)*(1-u)*y1 ) + ( t*(1-u)*y2 )+ (t*u*y3) + ( (1-t)*u*y4 );

    //#################################### calc drift time variation #############
    y1=rAngle.drifttime1Err [dmin];
    y2=rAngle2.drifttime1Err[dmin];
    y3=rAngle2.drifttime1Err[dmin+1];
    y4=rAngle.drifttime1Err [dmin+1];


    timeResolution=( (1-t)*(1-u)*y1 ) + ( t*(1-u)*y2 )+ (t*u*y3) + ( (1-t)*u*y4 );

    finaltime=time;
    return finaltime;
}
void HMdcCal2ParSim::calcTimeDigitizer(Int_t s,Int_t m,Float_t a,
				       Float_t d,Float_t*time1,Float_t*time1Err) {
    // This function calculates the drift time for a given distance
    // to sense wire.A Interpolation between the two closest angle steps
    // is performed and the drift time value returned.
    //
    sector=s;
    module=m;
    distance=d;
 

    dmin=(Int_t)(distance/0.1);
    Float_t b=TMath::Abs(a);

    time=0;

    finaltime=0;

    // only angles between 0 and 90 degree are defined
    // all other angles have to be shifted

    if(b<=90  && b>=0)
    {
        angleDeg=90-b;           // 90 deg impact ->0
    }
    else
    {
	if(b<=180 && b> 90 ) angleDeg=90-(180-b);
	if(b<=270 && b> 180) angleDeg=90-(270-b);
	if(b< 360 && b> 270) angleDeg=90-(360-b);
    }

    angleStep=((Int_t)((angleDeg)/5));      // this number is need to look up the matrix

    if(angleStep==18)angleStep=17;      // if angle==90, angleStep=18 ->not defined

    HMdcCal2ParAngleSim& rAngle =(*this)[s][m][angleStep];// pointer to the first set

    HMdcCal2ParAngleSim rAngle2;

    if(angleStep<17)
    {
	rAngle2=(*this)[s][m][angleStep+1];               // pointer to the second set
    }
    else
    {
	rAngle2=(*this)[s][m][angleStep];
    }

    Float_t timeResolution=0;
    Float_t gauss=gRandom->Gaus(0,1); // random value between -1 and 1

    //###########################################################################
    Float_t y1=0;
    Float_t y2=0;
    Float_t y3=0;
    Float_t y4=0;
    Float_t t=0;
    Float_t u=0;
    Float_t anglesearch=0;
    Float_t distancesearch=0;
    //#################################### calc drift time ######################
    y1=rAngle.drifttime1 [dmin];
    y2=rAngle2.drifttime1[dmin];
    y3=rAngle2.drifttime1[dmin+1];
    y4=rAngle.drifttime1 [dmin+1];


    anglesearch=angleDeg;             // x

    distancesearch=distance;

    t=(anglesearch - (angleStep*5) )/(Float_t)( ((angleStep+1)*5)- (angleStep*5) );
    u=(distancesearch - (dmin*0.1) )/(Float_t)((dmin+1-dmin)*0.1);

    time=( (1-t)*(1-u)*y1 ) + ( t*(1-u)*y2 )+ (t*u*y3) + ( (1-t)*u*y4 );

    //#################################### calc drift time variation #############
    y1=rAngle.drifttime1Err [dmin];
    y2=rAngle2.drifttime1Err[dmin];
    y3=rAngle2.drifttime1Err[dmin+1];
    y4=rAngle.drifttime1Err [dmin+1];


    timeResolution=( (1-t)*(1-u)*y1 ) + ( t*(1-u)*y2 )+ (t*u*y3) + ( (1-t)*u*y4 );

    *time1=time;
    *time1Err=timeResolution*gauss;
}
void HMdcCal2ParSim::calcTime2Digitizer(Int_t s,Int_t m,Float_t a,
					Float_t d,Float_t*time2,Float_t*time2Err) {
    // This function calculates the drift time for a given distance
    // to sense wire.A Interpolation between the two closest angle steps
    // is performed and the drift time value returned.
    //
    sector=s;
    module=m;
    distance=d;
 

    dmin=(Int_t)(distance/0.1);

    Float_t b=TMath::Abs(a);

    time=0;

    finaltime=0;

    // only angles between 0 and 90 degree are defined
    // all other angles have to be shifted

    if(b<=90  && b>=0)
    {
	angleDeg=90-b;
    }
    else
    {
	if(b<=180 && b> 90 ) angleDeg=90-(180-b);
	if(b<=270 && b> 180) angleDeg=90-(270-b);
	if(b< 360 && b> 270) angleDeg=90-(360-b);
    }


    angleStep=((Int_t)((angleDeg)/5));                  // this number is need to look up the matrix

    if(angleStep==18)angleStep=17;      // if angle==90, angleStep=18 ->not defined

    HMdcCal2ParAngleSim& rAngle =(*this)[s][m][angleStep];// pointer to the first set

    HMdcCal2ParAngleSim rAngle2;

    if(angleStep<17)
    {
	rAngle2=(*this)[s][m][angleStep+1];               // pointer to the second set
    }
    else
    {
	rAngle2=(*this)[s][m][angleStep];
    }

    //########################### time resolution ################################
    // instead of a position resolution time resolution is calculated
    // this value is used to smear the calulated drift time

    Float_t timeResolution=0;
    Float_t gauss=gRandom->Gaus(0,1); // random value between -1 and 1

    //###########################################################################
    Float_t y1=0;
    Float_t y2=0;
    Float_t y3=0;
    Float_t y4=0;
    Float_t t=0;
    Float_t u=0;
    Float_t anglesearch=0;
    Float_t distancesearch=0;
    //#################################### calc drift time ######################
    y1=rAngle.drifttime2 [dmin];
    y2=rAngle2.drifttime2[dmin];
    y3=rAngle2.drifttime2[dmin+1];
    y4=rAngle.drifttime2 [dmin+1];


    anglesearch=angleDeg;

    distancesearch=distance;

    t=(anglesearch - (angleStep*5) )/(Float_t)( ((angleStep+1)*5)- (angleStep*5) );
    u=(distancesearch - (dmin*0.1) )/(Float_t)((dmin+1-dmin)*0.1);

    time=( (1-t)*(1-u)*y1 ) + ( t*(1-u)*y2 )+ (t*u*y3) + ( (1-t)*u*y4 );

    //#################################### calc drift time variation #############
    y1=rAngle.drifttime2Err [dmin];
    y2=rAngle2.drifttime2Err[dmin];
    y3=rAngle2.drifttime2Err[dmin+1];
    y4=rAngle.drifttime2Err [dmin+1];


    timeResolution=( (1-t)*(1-u)*y1 ) + ( t*(1-u)*y2 )+ (t*u*y3) + ( (1-t)*u*y4 );

    *time2=time;
    *time2Err=timeResolution*gauss;

}



Int_t HMdcCal2ParSim::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcCal2ParSim::readline(const char* buf, Int_t* set) {
  // decodes one line read from ascii file I/O and calls HMdcCal2ParAngleSim::fillTime1(...)
  Int_t   sec,mod,angle,type;
  Float_t par1,par2,par3,par4,par5,par6,par7,par8,par9,par10;

  if(linecounter%10==0) linecounter=0;

  sscanf(buf,"%i%i%i%i%*s",&sec, &mod, &angle, &type);
  sscanf(&buf[9],"%f%f%f%f%f%f%f%f%f%f",
         &par1, &par2,
	 &par3, &par4, &par5,
	 &par6, &par7, &par8,
	 &par9, &par10 );

  Int_t n=sec*4+mod;

  if (!set[n]) return;
  HMdcCal2ParAngleSim& rAngle=(*this)[sec][mod][angle];
  switch (type){
  case  0: rAngle.fillTime1(linecounter,par1,par2,par3,par4,par5,par6,par7,par8,par9,par10);
  break;
  case  1:rAngle.fillTime1Error(linecounter,par1,par2,par3,par4,par5,par6,par7,par8,par9,par10);
  break;
  case  2:rAngle.fillTime2(linecounter,par1,par2,par3,par4,par5,par6,par7,par8,par9,par10);
  break;
  case  3:rAngle.fillTime2Error(linecounter,par1,par2,par3,par4,par5,par6,par7,par8,par9,par10);
  break;
  default : Error("HMdcCal2Parsim::readline()","argument \"type\" out of range");
  break;
  }
  linecounter++;

  set[n]=999;
}

void HMdcCal2ParSim::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
	"# Cal2 Calibration parameters of the MDC\n"
	"# distance->time\n"
	"# Format: type: 0=time1, 1=time2, 2=variation of time1, 3=variation of time2\n"
	"# sector  module  angle type par0 par1 par2 par3 par4 par5 pa6 par7 par8 par9 par10\n";
}

Bool_t HMdcCal2ParSim::writeline(char *buf, Int_t sec, Int_t mod, Int_t angle) {
  // writes one line to the buffer used by ascii file I/O
 Bool_t r = kTRUE;
 //Int_t type;
 //Int_t typecount;

 if (fMdc)
     if (fMdc->getModule(sec,mod) != 0) {
	 if  (sec>-1 && sec<getSize()) {
	     HMdcCal2ParSecSim &sector = (*this)[sec];
	     if (mod>-1 && mod<sector.getSize()) {

		 HMdcCal2ParAngleSim& rAngle=(*this)[sec][mod][angle];
		 Char_t dummy[20];

		 if(linecounterwrite%10==0&&linecounterwrite>1)
		 {
		     linecounterwrite=0;
		     type++;
		 }
                 if(type>3)
		 {
		    type=0;
		 }
		 sprintf(dummy,"%1i %1i %2i %1i",sec, mod, angle, type);
		 switch (type){
		 case 0:
		     sprintf(buf,"%s %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
			     dummy,
			     rAngle.getDriftTime1(linecounterwrite,0),rAngle.getDriftTime1(linecounterwrite,1),
			     rAngle.getDriftTime1(linecounterwrite,2),rAngle.getDriftTime1(linecounterwrite,3),
			     rAngle.getDriftTime1(linecounterwrite,4),rAngle.getDriftTime1(linecounterwrite,5),
			     rAngle.getDriftTime1(linecounterwrite,6),rAngle.getDriftTime1(linecounterwrite,7),
			     rAngle.getDriftTime1(linecounterwrite,8),rAngle.getDriftTime1(linecounterwrite,9) );
		     break;
		 case 1:
		     sprintf(buf,"%s %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
			     dummy,
			     rAngle.getDriftTime1Error(linecounterwrite,0),rAngle.getDriftTime1Error(linecounterwrite,1),
			     rAngle.getDriftTime1Error(linecounterwrite,2),rAngle.getDriftTime1Error(linecounterwrite,3),
			     rAngle.getDriftTime1Error(linecounterwrite,4),rAngle.getDriftTime1Error(linecounterwrite,5),
			     rAngle.getDriftTime1Error(linecounterwrite,6),rAngle.getDriftTime1Error(linecounterwrite,7),
			     rAngle.getDriftTime1Error(linecounterwrite,8),rAngle.getDriftTime1Error(linecounterwrite,9) );
		     break;
		 case 2:
		     sprintf(buf,"%s %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
			     dummy,
			     rAngle.getDriftTime2(linecounterwrite,0),rAngle.getDriftTime2(linecounterwrite,1),
			     rAngle.getDriftTime2(linecounterwrite,2),rAngle.getDriftTime2(linecounterwrite,3),
			     rAngle.getDriftTime2(linecounterwrite,4),rAngle.getDriftTime2(linecounterwrite,5),
			     rAngle.getDriftTime2(linecounterwrite,6),rAngle.getDriftTime2(linecounterwrite,7),
			     rAngle.getDriftTime2(linecounterwrite,8),rAngle.getDriftTime2(linecounterwrite,9) );
		     break;
		 case 3:
		     sprintf(buf,"%s %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
			     dummy,
			     rAngle.getDriftTime2Error(linecounterwrite,0),rAngle.getDriftTime2Error(linecounterwrite,1),
			     rAngle.getDriftTime2Error(linecounterwrite,2),rAngle.getDriftTime2Error(linecounterwrite,3),
			     rAngle.getDriftTime2Error(linecounterwrite,4),rAngle.getDriftTime2Error(linecounterwrite,5),
			     rAngle.getDriftTime2Error(linecounterwrite,6),rAngle.getDriftTime2Error(linecounterwrite,7),
			     rAngle.getDriftTime2Error(linecounterwrite,8),rAngle.getDriftTime2Error(linecounterwrite,9) );
                 break;
		 default:Error("HMdcCal2ParSim::writeline()","argument \"type\" out of range ");
                 break;
		 }

		 linecounterwrite++;
	     } else r = kFALSE;
	 }  else r = kFALSE;
     }else strcpy(buf,"");

 return r;
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
void HMdcCal2ParSim::printParam() {
  // prints the container
    for(Int_t s=0;s<getSize();s++) {
	HMdcCal2ParSecSim& sec=(*this)[s];
	for(Int_t m=0;m<sec.getSize();m++) {
	    HMdcCal2ParModSim& mod=sec[m];
	    for(Int_t l=0;l<18;l++) {
		HMdcCal2ParAngleSim& rAngle=mod[l];
		for(Int_t mytype=0;mytype<4;mytype++){
		    for(Int_t myline=0;myline<10;myline++){


			printf("%1i %1i %2i %1i",s, m, l, mytype);
			switch (mytype){
			case 0:
			    printf("%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
				   rAngle.getDriftTime1(myline,0),rAngle.getDriftTime1(myline,1),
				   rAngle.getDriftTime1(myline,2),rAngle.getDriftTime1(myline,3),
				   rAngle.getDriftTime1(myline,4),rAngle.getDriftTime1(myline,5),
				   rAngle.getDriftTime1(myline,6),rAngle.getDriftTime1(myline,7),
				   rAngle.getDriftTime1(myline,8),rAngle.getDriftTime1(myline,9) );
			    break;
			case 1:
			    printf("%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
				   rAngle.getDriftTime1Error(myline,0),rAngle.getDriftTime1Error(myline,1),
				   rAngle.getDriftTime1Error(myline,2),rAngle.getDriftTime1Error(myline,3),
				   rAngle.getDriftTime1Error(myline,4),rAngle.getDriftTime1Error(myline,5),
				   rAngle.getDriftTime1Error(myline,6),rAngle.getDriftTime1Error(myline,7),
				   rAngle.getDriftTime1Error(myline,8),rAngle.getDriftTime1Error(myline,9) );
			    break;
			case 2:
			    printf("%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
				   rAngle.getDriftTime2(myline,0),rAngle.getDriftTime2(myline,1),
				   rAngle.getDriftTime2(myline,2),rAngle.getDriftTime2(myline,3),
				   rAngle.getDriftTime2(myline,4),rAngle.getDriftTime2(myline,5),
				   rAngle.getDriftTime2(myline,6),rAngle.getDriftTime2(myline,7),
				   rAngle.getDriftTime2(myline,8),rAngle.getDriftTime2(myline,9) );
			    break;
			case 3:
			    printf("%7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f\n",
				   rAngle.getDriftTime2Error(myline,0),rAngle.getDriftTime2Error(myline,1),
				   rAngle.getDriftTime2Error(myline,2),rAngle.getDriftTime2Error(myline,3),
				   rAngle.getDriftTime2Error(myline,4),rAngle.getDriftTime2Error(myline,5),
				   rAngle.getDriftTime2Error(myline,6),rAngle.getDriftTime2Error(myline,7),
				   rAngle.getDriftTime2Error(myline,8),rAngle.getDriftTime2Error(myline,9) );
			    break;
			default:Error("HMdcCal2ParSim::printParam()","argument \"type\" out of range ");
			break;
			}
		    }
		}
	    }
	}
    }
}

