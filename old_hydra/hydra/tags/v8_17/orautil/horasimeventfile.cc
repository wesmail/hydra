//*-- AUTHOR : Ilse Koenig
//*-- Created : 16/09/2002 by Ilse Koenig

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
// HOraSimEventFile
//
// Class for the event files for HGEANT
//
//////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "horasimeventfile.h"
#include <iostream>
#include <iomanip>

ClassImp(HOraSimEventFile)

HOraSimEventFile::HOraSimEventFile() {
  fileName="";
  fileDirectory="";
  fileSize=-1;
  created="";
  nEvents=-1;
} 

void HOraSimEventFile::print() {
  cout<<"File Name:        "<<fileName<<'\n';
  cout<<"FileDirectory:    "<<fileDirectory<<'\n';
  cout<<"FileSize:         ";
  if (fileSize!=-1) cout<<fileSize;
  cout<<"\n";
  cout<<"Created At:       "<<created<<'\n';
  cout<<"Number of events: ";
  if (nEvents!=-1) cout<<nEvents;
  cout<<'\n';
  cout<<"********************************************************************************\n";
}

void HOraSimEventFile::read(fstream& fin) {
  TString b, e;
  fin>>fileName>>fileDirectory>>created>>b>>e;
  fileSize=convertToInt(b);
  nEvents=convertToInt(e);
}

void HOraSimEventFile::setNEvents(const Char_t* s) {
  TString e(s);
  nEvents=(Int_t)convertToInt(e);
}

void HOraSimEventFile::setFileSize(const Char_t* s) {
  TString b(s);
  fileSize=convertToInt(b);
}

Int_t HOraSimEventFile::convertToInt(TString& s) {
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
