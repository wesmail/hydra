//*-- AUTHOR : Jochen Markert
//*-- Modified : 24/05/2000

///////////////////////////////////////////////////////////////
//HMdcTrackFitPar
//
//
//////////////////////////////////////////////////////////////

#include "hmdctrackfitpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hmdcparasciifileio.h"
#include "hpario.h"
#include "hmdcdetector.h"
#include "hspectrometer.h"


HMdcTrackFitPar::HMdcTrackFitPar(Int_t s,Int_t m) : fSecs(s) {

    for (Int_t i=0;i<s; i++) fSecs.AddAt(new HMdcTrackFitParSec(m),i);
    setNameTitle();
    if (gHades) {
	fMdc = (HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
    } else {
	fMdc = 0;
    }
}

void HMdcTrackFitPar::clear(void) {
   for (Int_t s=0;s<getSize();s++) {
    HMdcTrackFitParSec &sector=this->operator[](s);
    
    for(Int_t mod=0;mod<sector.getSize();mod++) {
      sector[mod].fill(0.,
		       0,
		       0., 0., 0., 0.,0.,0.,
		       0., 0., 0., 0.,0.,0.,
		       0.,0.
		      );
    }
   }
  isContainer=kTRUE;
}






void HMdcTrackFitPar::setNameTitle() {
  fName="MdcTrackFitPar";
  fTitle="parameters for DUBNA fitter";
  strcpy(detName,"Mdc");
}

Bool_t HMdcTrackFitPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  lineCounter=0;
  lineCounterWrite=0;
  return kFALSE;
}

Int_t HMdcTrackFitPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcTrackFitPar::readline(const char* buf, Int_t* set) {
  // decodes one line read from ascii file I/O 
  Int_t sec,mod,n;

  Double_t stp1,stp2,stp3,stp4,stp5,stp6;
  Double_t wt1,wt2,wt3,wt4,wt5,wt6;
  Double_t stN,tukey;
  Float_t  dVMDC;
  Int_t    maxIt;

  stp1=stp2=stp3=stp4=stp5=stp6=0;
  wt1=wt2=wt3=wt4=wt5=wt6=0;
  stN=tukey=0;
  dVMDC=0;
  maxIt=0;

  if(lineCounter%4==0)lineCounter=0;

  if(lineCounter==0) sscanf(buf,"%i%i%f%i",&sec,&mod,&dVMDC,&maxIt);
  if(lineCounter==1) sscanf(buf,"%i%i%lf%lf%lf%lf%lf%lf",&sec,&mod,&stp1,&stp2,&stp3,&stp4,&stp5,&stp6);
  if(lineCounter==2) sscanf(buf,"%i%i%lf%lf%lf%lf%lf%lf",&sec,&mod,&wt1,&wt2,&wt3,&wt4,&wt5,&wt6);
  if(lineCounter==3) sscanf(buf,"%i%i%lf%lf",&sec,&mod,&stN,&tukey);

  n = sec*4 + mod; //FIXME
  if (set[n]) {
    if (sec>-1 && sec<6 && mod>-1 && mod<4) {
	//(*this)[sec][mod].fill( );
	if(lineCounter==0)
	{
	    (*this)[sec][mod].setDriftVelocityMDC(dVMDC);
	    (*this)[sec][mod].setMaxIteration(maxIt);
	}
	if(lineCounter==1)
	{
	    (*this)[sec][mod].setStep1(stp1);
	    (*this)[sec][mod].setStep2(stp2);
	    (*this)[sec][mod].setStep3(stp3);
	    (*this)[sec][mod].setStep4(stp4);
	    (*this)[sec][mod].setStep5(stp5);
	    (*this)[sec][mod].setStep6(stp6);
	}
	if(lineCounter==2)
	{
	    (*this)[sec][mod].setWeight1(wt1);
	    (*this)[sec][mod].setWeight2(wt2);
	    (*this)[sec][mod].setWeight3(wt3);
	    (*this)[sec][mod].setWeight4(wt4);
	    (*this)[sec][mod].setWeight5(wt5);
	    (*this)[sec][mod].setWeight6(wt6);
	}
	if(lineCounter==3)
	{
	    (*this)[sec][mod].setStepNorm(stN);
	    (*this)[sec][mod].setTukeyConstant(tukey);
	}
	lineCounter++;
    } else {
	Error("readLine","Addres: sector_%i, module_%i not known",sec,mod);
    }
    set[n] = 999;
  }
}

void HMdcTrackFitPar::putAsciiHeader(TString& b) {
  b =
    "#######################################################################\n"
    "# Drift time cuts for noise reduction of the MDC\n"
    "# Format:\n"
    "# sector mod Vdrift Mdc MaxIteration\n"
    "# sector mod step1-6\n"
    "# sector mod weight1-6\n"
    "# sector mod setNorm Tukey Constant\n"
    "#######################################################################\n";
}

Bool_t HMdcTrackFitPar::writeline(char*buf, Int_t s, Int_t m) {
  Bool_t r = kTRUE;

  if (fMdc)
    if (fMdc->getModule(s,m) != 0) {
      if  (s>-1 && s<getSize()) {
	HMdcTrackFitParSec &sector = (*this)[s];
	if (m>-1 && m<sector.getSize()) {

            if(lineCounterWrite%4==0)lineCounterWrite=0;

	    if(lineCounterWrite==0)
	    {
		sprintf(buf,"%i %i %5.1f %i\n",
			s,m,
			(*this)[s][m].getDriftVelocityMDC(),
                        (*this)[s][m].getMaxIteration());
		      }
	    if(lineCounterWrite==1)
	    {
		sprintf(buf,"%i %i %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f\n",
			s,m,
			(*this)[s][m].getStep1(),
			(*this)[s][m].getStep2(),
			(*this)[s][m].getStep3(),
			(*this)[s][m].getStep4(),
			(*this)[s][m].getStep5(),
			(*this)[s][m].getStep6());
	    }
	    if(lineCounterWrite==2)
	    {
		sprintf(buf,"%i %i %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f\n",
			s,m,
			(*this)[s][m].getWeight1(),
			(*this)[s][m].getWeight2(),
			(*this)[s][m].getWeight3(),
			(*this)[s][m].getWeight4(),
			(*this)[s][m].getWeight5(),
			(*this)[s][m].getWeight6());
	    }
	    if(lineCounterWrite==3)
	    {
		sprintf(buf,"%i %i %5.2f %5.2f\n",
			s,m,
			(*this)[s][m].getStepNorm(),
                        (*this)[s][m].getTukeyConstant());
	    }

    lineCounterWrite++;
	} else r = kFALSE;
      } else r = kFALSE;
    } else strcpy(buf,"");
  
  return r;
}

ClassImp(HMdcTrackFitPar) 
ClassImp(HMdcTrackFitParMod)
ClassImp(HMdcTrackFitParSec)
