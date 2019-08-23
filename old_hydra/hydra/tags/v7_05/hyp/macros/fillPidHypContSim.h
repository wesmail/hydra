#ifndef HANALYSISDST_H
#define HANALYSISDST_H
      
#ifdef __CINT__
#define EXIT_SUCCESS  0
#define EXIT_FAILURE  1
#endif

#ifndef __CINT__
/* #include <stdio.h> */
/* #include <iostream.h> */
/* #include <fstream.h> */
/* #include <iomanip.h> */
/* #include <stdlib.h> */
//#include <stdio>
#include <iostream>
#include <fstream>
#include <iomanip>
/* #include <stdlib.h> */

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
#include <TLorentzVector.h>
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

#include "hrichdetector.h"
#include "hmdcdetector.h"
#include "htofdetector.h"
#include "htofinodetector.h"
#include "hshowerdetector.h"
#include "hstartdetector.h"
#include "htboxdetector.h"
#include "htriggerdetector.h"
#include "htriggertaskset.h"

#include "hrichtaskset.h"
#include "hrichIPUtaskset.h"
#include "hrichchernovringfitter.h"

#include "hkicktaskset.h"
#include "hmdctaskset.h"
#include "hshowertaskset.h"
#include "hshowertofinotaskset.h"
#include "hstarttaskset.h"
#include "htoftaskset.h"
#include "htofinotaskset.h"

//#include "hqamaker.h"

#include "hrichunpackerraw99.h"
#include "hrichunpackercal99.h"
#include "hmdcunpacker.h"
#include "htofunpacker.h"
#include "htofinounpacker.h"
#include "hshowerunpacker.h"
#include "hstartunpacker.h"
#include "hmatchuunpacker.h"

#include "hspectrometer.h"

#include "showerdef.h"
#include "hstartdef.h"
#include "tofinodef.h"
#include "tofdef.h"
#include "richdef.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "hrootsource.h"
#include "hldfilesource.h"
#include "hmdcraw.h"
#include "hmdccal1.h"
#include "hmdccal1sim.h"
#include "hmdccal2.h"
#include "hmdccal2sim.h"
#include "hmdcclus.h"
#include "hmdchit.h"
#include "hmdchitsim.h"
#include "hmdcseg.h"
#include "hmdcsegsim.h"
#include "hmdcsetup.h"
#include "hmdcvertexfind.h"
#include "hsplinetrackF.h"
#include "hkicktrackbaseF.h"
#include "hmetamatchF.h"
#include "hparoraio.h"
#include "hparrootfileio.h"
#include "hruntimedb.h"
#include "htofraw.h"
#include "htofhit.h"
#include "htofhitsim.h"
#include "htofinocal.h"
#include "htofinocalsim.h"
#include "hkicktrack.h"

#include "hrichIPUparthresholds.h"
#include "hrichIPUparlocmax.h"
#include "hqamaker.h"

#include "hsudummyrec.h"
#include "hpidfl.h"
#include "hphysicsconstants.h"

#include "hhypkine.h"
#include "hhypbasealgorithm.h"
#include "hhypPPEpEmXAlg.h"
#include "hhypPPPipPimPi0Alg.h"
#include "hhypPPPipPimProjector.h"
#include "hhypPPPipPimXAlg.h"
#include "hhypchannel.h"
#include "hhypcomb.h"

#include "hhyplist.h"
#include "hhyplistfiller.h"
#include "hhyplistreader.h"
#include "hhypreconstructor.h"

#include <hpidalgorithm.h>
#include <hpidreconstructor.h>
#include "hpidparticlefiller.h"
#include "hpidtrackfiller.h"
#include "hpidperformacetest.h"
#include "hpidperformacetestexp.h"
#include "hpidalgmomvsbeta.h"
#include "htofrec.h"

#endif
#endif
