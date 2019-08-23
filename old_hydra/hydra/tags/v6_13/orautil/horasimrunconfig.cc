//*-- AUTHOR : Ilse Koenig
//*-- Created : 26/09/2002 by Ilse Koenig

//////////////////////////////////////////////////////////////////////////////
//
// HOraSimRunConfig
//
// Class for the configuration of HGEANT simulations runs
//
//////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "horasimrunconfig.h"
#include <iostream>
#include <iomanip>
#include <fstream>

ClassImp(HOraSimRunConfig)

HOraSimRunConfig::HOraSimRunConfig() {
  generation=-1;
  geantVersion="";
  hydraVersion="";
  rootVersion="";
  cernLib="";
  compiler="";
  geantFlags="";
  parameters="";
  author="";
  description="";
}

void HOraSimRunConfig::print() {
  cout<<"*******  Configuration of Simulation Runs **************************************\n";
  cout<<"GEANT Version:    "<<geantVersion<<'\n';
  cout<<"HYDRA Version:    "<<hydraVersion<<'\n';
  cout<<"ROOT Version:     "<<rootVersion<<'\n';
  cout<<"CERN Library:     "<<cernLib<<'\n';
  cout<<"Compiler:         "<<compiler<<'\n';
  TString s(geantFlags);
  cout<<"GEANT Flags:      "<<s.ReplaceAll("\n","\n                  ")<<'\n';
  s=parameters;
  cout<<"Parameters:       "<<s.ReplaceAll("\n","\n                  ")<<'\n';
  cout<<"Author:           "<<author<<'\n';
  s=description;
  cout<<"Description:      "<<s.ReplaceAll("\n","\n                  ")<<'\n';
  cout<<"********************************************************************************\n";
}

Bool_t HOraSimRunConfig::compare(HOraSimRunConfig& c) {
  if (getGeneration()!=generation
      || strcmp(c.getGeantVersion(),geantVersion)!=0
      || strcmp(c.getHydraVersion(),hydraVersion)!=0
      || strcmp(c.getRootVersion(),rootVersion)!=0
      || strcmp(c.getCernLib(),cernLib)!=0
      || strcmp(c.getCompiler(),compiler)!=0
      || strcmp(c.getGeantFlags(),geantFlags)!=0
      || strcmp(c.getParameters(),parameters)!=0
      || strcmp(c.getAuthor(),author)!=0
      || strcmp(c.getDescription(),description)!=0) return kFALSE;
  return kTRUE;
}
