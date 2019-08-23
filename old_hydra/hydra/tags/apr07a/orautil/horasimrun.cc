//*-- AUTHOR : Ilse Koenig
//*-- Created : 26/09/2002 by Ilse Koenig

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
// HOraSimRun
//
// Class for a HGEANT simulations run
//
//////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "horasimrun.h"
#include "horasimrunconfig.h"
#include "horasimutil.h"
#include <iostream>
#include <iomanip>
#include <fstream>

ClassImp(HOraSimRun)

HOraSimRun::HOraSimRun() {
  runId=-1;
  fileNum=-1;
  fileName="";
  fileDirectory="";
  fileSize=-1;
  created="";
  nEvents=-1;
  config=new HOraSimRunConfig;
  eventFile="";  
}

HOraSimRun::~HOraSimRun() {
  if (config) {
    delete config;
    config=0;
  }
}

void HOraSimRun::print() {
  config->print();
  cout<<"Run Id:           "<<runId<<'\n';
  cout<<"File Number:      "<<fileNum<<'\n';
  cout<<"File Name:        "<<fileName<<'\n';
  cout<<"FileDirectory:    "<<fileDirectory<<'\n';
  cout<<"FileSize:         ";
  if (fileSize!=-1) cout<<fileSize;
  cout<<"\n";
  cout<<"Created At:       "<<created<<'\n';
  cout<<"Number of events: ";
  if (nEvents!=-1) cout<<nEvents;
  cout<<'\n';
  cout<<"Event File:       "<<eventFile<<'\n';
  cout<<"********************************************************************************\n";
}

void HOraSimRun::setNEvents(const char* s) {
  TString e(s);
  nEvents=(Int_t)convertToInt(e);
}

void HOraSimRun::setFileSize(const char* s) {
  TString b(s);
  fileSize=convertToInt(b);
}

Int_t HOraSimRun::convertToInt(TString& s) {
  Int_t i=-1;
  if (!s.Contains("?")) {
    s.ToUpper();
    Float_t n;
    sscanf(s.Data(),"%f",&n);
    if (s.Contains("K")) n=n*1.e+3;
    else {
      if (s.Contains("M")) n=n*1.e+6;
      else {
        if (s.Contains("G")) n=n*1.e+9;
      }
    }
    i=(Int_t)n;
  }
  return i;
}    
