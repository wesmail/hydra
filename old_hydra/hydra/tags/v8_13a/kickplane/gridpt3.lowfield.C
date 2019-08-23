{
  //Configuration for gridpt3_core.C   Low field (10%)
#include <fstream.h>

/**************************** CONFIGURATION **********************************/

const Float_t kMinTheta=15.;
const Float_t kMaxTheta=85.;
const Float_t kMinPhi=-30.;
const Float_t kMaxPhi=30.;
const Float_t kGranularity=.5;
const Float_t kSimetry = 1.;
const Float_t kTukey2 = .1 * .1;
const Float_t kInitialPCut=20.;
const Float_t kMaxDist = 500.;
const TString mode = "meta";   //mdc3   meta
const TString bnMomentum = "pmdc"; //Get momentum from MDC2
const TString inputTreeName = "Data";
const TString inFileName = "../out.pos.sims.notargnorich.meta.lowfield.300902.root";
const TString outParFile = "pfitpar.pos.txt";

gROOT->Macro("gridpt3_core.C");
}
