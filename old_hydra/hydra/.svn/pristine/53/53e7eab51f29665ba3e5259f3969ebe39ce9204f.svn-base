//*-- AUTHOR : AN
//*-- Modified : 05/07/2000 by AN

////////////////////////////////////////////////////////////////
// HMdcFitPar
// Container class for storing of fitting parameters,
// contains read/write functions for ASCII/ROOT files 
////////////////////////////////////////////////////////////////

#include "hmdctrackdpar.h"
#include "hpario.h"
#include "hdetpario.h"


ClassImp(HMdcTrackDPar)

HMdcTrackDPar::HMdcTrackDPar() {
  fName="MdcTrackDPar";
  fTitle="clusters finding and fitting  parameters for tracking";
  strcpy(detName,"Mdc");
  array = new TObjArray(6);
//  for (Int_t i = 0; i < 6; i++) {
//    (*array)[i] = new HMdcTrackDParSec;
//  } 
}

HMdcTrackDPar::~HMdcTrackDPar() {
  if( array ) {
    array->Delete();
    delete array;
  }
}

Bool_t HMdcTrackDPar::init(HParAsciiFileIo* inp,
    Text_t* fname="/u/nekhaev/anal/Hydra/v5_01/macros/Mdc/fit.txt", 
    Text_t* status="in" ) {
  Char_t c, buf[100];
  if(!strcmp(status,"in")) {
    ifstream file(fname);
    if(!file) {
      cout<<"Cannot open file "<< fname << endl;
      return kFALSE;
    }
    c=file.get();
    if (c!='#') {
    }
  }
  else {
    cout << "Write function for ASCII not emplemented yet" << endl;
  }
  return kTRUE;  
}

Bool_t HMdcTrackDPar::init(HParAsciiFileIo* inp,Text_t* fname="/u/nekhaev/anal/Hydra/v5_01/macros/Mdc/fit.txt", Text_t* status="in" ) {
  Char_t c, buf[100];
  if(!strcmp(status,"in")) {
    ifstream file(fname);
    if(!file)
      {
	cout<<"Cannot open file "<< fname << endl;
	return kFALSE;
      }
    c=file.get();
    if (c!='#') {
      file.putback(c);
      file.getline(buf,100);
      sscanf(buf,"%f",&driftVelocity);
      file.getline(buf,100);
      sscanf(buf,"%i",&maxNMdcTimes);
      file.getline(buf,100);
      sscanf(buf,"%i",&maxIteration);
      file.getline(buf,100);
      sscanf(buf,"%lf%lf%lf%lf%lf%lf",&step[0],&step[1],&step[2],&step[3],&step[4],&step[5]);
      file.getline(buf,100);
      sscanf(buf,"%lf%lf%lf%lf%lf%lf",&weight[0],&weight[1],&weight[2],&weight[3],&weight[4],&weight[5]);
      file.getline(buf,100);
      sscanf(buf,"%lf",&stepNorm);
      file.getline(buf,100);
      sscanf(buf,"%lf%lf%lf",&xVertex,&yVertex,&zVertex);
      file.getline(buf,100);
      sscanf(buf,"%lf",&particleVelocity);
      file.getline(buf,100);
      sscanf(buf,"%lf",&tukeyConstant);
    }
  }
  else {
    cout << "Write function for ASCII not emplemented yet" << endl;
  }
  return kTRUE;
}

Bool_t HMdcTrackDPar::init(HParRootFileIo* inp,Text_t* fname="/u/nekhaev/anal/Hydra/v5_01/macros/Mdc/fit.root", Text_t* status="in" ) {
  cout << "Not emplemented yet !" << endl;
  return kFALSE;
}

