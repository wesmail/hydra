//*-- AUTHOR : A.Nekhaev
//*-- Modified : 17/10/2000 by A.Nekhaev

////////////////////////////////////////////////////////////////
// HMdcFitPar
// Container class for storing of fitting parameters,
// contains read/write functions for ASCII/ROOT files 
////////////////////////////////////////////////////////////////

#include "hmdctrackfitpar.h"
#include "hpario.h"
#include "hdetpario.h"


ClassImp(HMdcTrackFitPar)

HMdcTrackFitPar::HMdcTrackFitPar() {
  fName="MdcTrackFitPar";
  fTitle="fitting  parameters for tracking";
  strcpy(detName,"Mdc");
  HParAsciiFileIo* inp=0;
}

HMdcTrackFitPar::~HMdcTrackFitPar() {
}

Bool_t HMdcTrackFitPar::init(HParAsciiFileIo* inp,Text_t* fname="fit.txt", Text_t* status="in" ) {
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
      sscanf(buf,"%f",&driftVelocityMDC1);
      file.getline(buf,100);	    
      sscanf(buf,"%f",&driftVelocityMDC2);
      file.getline(buf,100);	    
      sscanf(buf,"%f",&driftVelocityMDC3);
      file.getline(buf,100);	    
      sscanf(buf,"%f",&driftVelocityMDC4);
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

Bool_t HMdcTrackFitPar::init(HParRootFileIo* inp,Text_t* fname="fit.root", Text_t* status="in" ) {
  cout << "Not emplemented yet !" << endl;
  return kFALSE;
}

