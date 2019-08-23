{
  //Configuration for matchpar3_core.C. Full field
#include <fstream.h>

/**************************** CONFIGURATION **********************************/

const Float_t kMinTheta=15.;
const Float_t kMaxTheta=85.;
const Float_t kMinPhi=-30.;
const Float_t kMaxPhi=30.;
const Float_t kGranularity=0.5;
const Float_t kSimetry = 1.;
const Float_t kTukey2 = .01 * .01;
const Float_t kMaxPError = 20;
const Float_t kInitialPCut=100.;
const Float_t kMaxDist = 500.;
const TString mode = "meta";   //mdc3   meta
const TString inputTreeName = "Res";
const TString inFileName = "res.root";
const TString outParFile = "matchpar.el.txt";

gROOT->Macro("matchpar3_core.C");
}
