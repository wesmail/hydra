#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

#include <TCanvas.h>
#include <TCutG.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TMath.h>
#include <TPad.h>
#include <TPaveText.h>
#include <TROOT.h>
//#include <TRFIOFile.h>
#include <TStopwatch.h>
#include <TString.h>
#include <TStyle.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TUnixSystem.h>

#include "hades.h"
#include "haddef.h"
#include "heventheader.h"
#include "hiterator.h"
#include "hmatrixcategory.h"
#include "hrecevent.h"
#include "htree.h"
#include "hrootsource.h"

#include "hmdctrackdset.h"
#include "hsplinetaskset.h"
#include "hparasciifileio.h"
#include "hmdcsetup.h"
#include "hrichdetector.h"
#include "hmdcdetector.h"
#include "htofdetector.h"
#include "htofinodetector.h"
#include "htofinodetector.h"
#include "hshowerdetector.h"
#include "hstartdetector.h"
#include "htboxdetector.h"
#include "htriggerdetector.h"

#include "hrichcorrelatorpar.h"
#include "hrichdigitisationpar.h"
#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"
#include "hrichchernovringfitter.h"

#include "hrichtaskset.h"
#include "hrichIPUtaskset.h"
#include "hrichevtfilter.h"
#include "hkicktaskset.h"
#include "hmdctaskset.h"
#include "hshowertaskset.h"
#include "hstarttaskset.h"
#include "htoftaskset.h"
#include "htofinotaskset.h"

#include "hqamaker.h"

#include "hparoraio.h"

#include "hrichunpackerraw99.h"
#include "hrichunpackercal99.h"
#include "hmdcunpacker.h"
#include "htofunpacker.h"
#include "htofinounpacker.h"
#include "hshowerunpacker.h"
#include "hstartunpacker.h"
#include "hmatchuunpacker.h"
#include "htriggerdetector.h"
#include "htriggertaskset.h"
#include "muEmulationSim.h"
#include "muEmulationExp.h"
#include "hrichIPUremakeSim.h"

#include "hspectrometer.h"

#include "showerdef.h"
#include "hstartdef.h"
#include "tofinodef.h"
#include "tofdef.h"
#include "richdef.h"
#include "hmdcdef.h"

#include "hpidtrackfiller.h"
#include "hpidtrackcandsim.h"
#include "hpidpdfmaker.h"

#include "hldfilesource.h"
#include "hmdcraw.h"
#include "hmdccal1.h"
#include "hmdccal1.h"
#include "hmdccal1sim.h"
#include "hmdccal2.h"
#include "hmdccal2sim.h"
#include "hmdcclus.h"
#include "hmdchit.h"
#include "hmdchitsim.h"
#include "hmdcseg.h"
#include "hmdcsegsim.h"
#include "hmdcvertexfind.h"
#include "hparrootfileio.h"
#include "hruntimedb.h"
#include "htofraw.h"
#include "htofhit.h"
#include "htofhitsim.h"
#include "htofinocal.h"
#include "htofinocalsim.h"
#include "hkicktrack.h"

#include "hsplinetrackF.h"
#include "hmetamatchF.h"
#include "hrktrackBF.h"
#include "hkicktrackbaseF.h"
#include "hmdcdigitizer.h"
