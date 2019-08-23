//*-- AUTHOR : Ilse Koenig
//*-- Created : 16/09/2002 by Ilse Koenig

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
// HOraSimEventFileConfig
//
// Class for the configuration of event files for HGEANT
//
//////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "horasimeventfileconfig.h"
#include <iostream>
#include <iomanip>
#include <fstream>

ClassImp(HOraSimEventFileConfig)

HOraSimEventFileConfig::HOraSimEventFileConfig() {
  packageName="";
  packageVersion="";
  compiler="";
  eventSystem="";
  energy=-1;
  configInfo="";
  fileFormat="";
  author="";
  description="";
}

Bool_t HOraSimEventFileConfig::readConfigInfo(const Char_t* fileName) {
  fstream* fin=new fstream;
  fin->open(fileName,ios::in);
  if (fin && (fin->rdbuf()->is_open()==1)) {
    const Int_t maxbuf=1024;
    Text_t buf[maxbuf];
    Bool_t first=kTRUE;
    while (!fin->eof()) {
      fin->getline(buf,maxbuf);
      if (buf[0]!='\0') {
        if (!first) configInfo.Append("\n");
        else first=kFALSE;
        configInfo.Append(buf);
      }
    }
    fin->close();
    delete fin;
    return kTRUE;
  }
  Error("readConfigInfo(const Char_t*)","No File open");
  delete fin;
  return kFALSE;
}

void HOraSimEventFileConfig::decodeSystem(const Char_t* buf) {
  TString s(buf);
  eventSystem=s(0,s.First('@'));
  eventSystem=eventSystem.Strip(s.kBoth);
  Int_t i=s.Last(' ')+1;
  Int_t l=s.Length()-i;
  TString ss=s(i,l);
  ss.ToLower();
  i=ss.First("agev");
  ss=ss(0,i);
  ss=ss.Strip(s.kTrailing);
  sscanf(ss.Data(),"%f",&energy);
}

void HOraSimEventFileConfig::print() {
  cout<<"*******  Configuration of Event Generator Files ******************************\n";
  cout<<"PackageName:      "<<packageName<<'\n';
  cout<<"PackageVersion:   "<<packageVersion<<'\n';
  cout<<"Compiler:         "<<compiler<<'\n';
  cout<<"EventSystem:      "<<eventSystem<<'\n';
  cout<<"Energy:           "<<energy<<"AGeV\n";
  TString s(configInfo);
  cout<<"ConfigInfo:       "<<s.ReplaceAll("\n","\n                  ")<<'\n';
  cout<<"FileFormat:       "<<fileFormat<<'\n';
  cout<<"Author:           "<<author<<'\n';
  s=description;
  cout<<"Description:      "<<s.ReplaceAll("\n","\n                  ")<<'\n';
  cout<<"********************************************************************************\n";
}

