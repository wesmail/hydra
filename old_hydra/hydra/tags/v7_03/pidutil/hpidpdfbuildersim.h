// hpidPDFBuilderSim.h, by Tassilo Christ 13.05.03
// Reconstructor class intended to construct Ntuples that extract statistical
// information about PidTrackCandidates found by the PIDTrackFiller
// --- To be used only upon sim data!



#ifndef  HPIDPDFBUILDERSIM_H
#define  HPIDPDFBUILDERSIM_H
#include "hreconstructor.h"
#include "haddef.h"
#include "piddef.h"

//Theese patterns are used to store (in a defined order) the obswervables
//describing the PidTrackCandidates and their associated variables.
//The benefit of this is that the TTrees holding the information can be 
//filled in a very simple way!
class TH1F;
class TTree;
class HCategory;
class HIterator;
class HPidTrackCand;
class HRichHit;
class HRichHitSim;
class HKickTrack;
class HRichAnalysisPar;
class HRichGeometryPar;
class HGeantKine;
class HMdcSeg;
class HMdcHit;

//Attention! Keep the order
#define p_dPhi           0 
#define p_dTheta         1 
#define p_pmq            2 
#define p_padnr          3 
#define p_centroid       4 
#define p_locmax         5 
#define p_avcharge       6 
#define p_theta          7 
#define p_phi            8 
#define p_sector         9 
#define p_neighbour      10
#define p_openingAngle   11
#define p_dalitz         12
#define p_gamma          13
#define p_ringtype       14
#define p_corrtype       15
#define p_mdcchisquare   16
#define p_iscorrgeantlep 17
#define p_nsegments      18
#define p_kicktype       19
#define p_nwiresmdc0     20
#define p_nwiresmdc1     21
#define p_clussizemdc0   22
#define p_clussizemdc1   23  
#define p_clusfinderlvl0 24
#define p_clusfinderlvl1 25
#define p_momentum       26

enum NEIGHBOUR {NO_RING_NEIGHBOUR=-1,FAKE_RING_NEIGHBOUR=0,
		SINGLE_RING_NEIGHBOUR=1,DOUBLE_RING_NEIGHBOUR=2};
enum CORRTYPE {R_KT=1, R_S=2, R_KT_S=3, KT=4, KT_S=5};

class HPidPDFBuilderSim : public HReconstructor 
{
 public:
  

  // default settings: 
  // make only PID histograms (no CP ID) 
  // don't use rings correlated to segments - just use KTs  
  HPidPDFBuilderSim(Option_t optionlist[]="SIM", Char_t* OutputFile = "Lepton_PID_PDFs.root");
  ~HPidPDFBuilderSim();

  
  Bool_t init();
  Bool_t finalize();
  Int_t execute();
  Bool_t clearPattern();
  
  Bool_t bookRingObservables();
  Bool_t bookKickObservables();
  
  Bool_t checkForRing();
  Bool_t checkForKickTrack();
  Int_t checkForSegments();

  Bool_t findLeptonicTrackCandidate(Int_t ringcounter); 

  Bool_t getGeantMatches(HPidTrackCand* giveCandidate, Int_t& nMatchesRichMdc,
			 Int_t& nMatchesMdcMeta, Int_t& nMatchesFull,
			 Int_t& nLeptonicMatchesFull, Int_t& nLeptonicMatchesMdcMeta);

  Bool_t getGeantMatches(HPidTrackCand* giveCandidate, Int_t& nMatchesMdcMeta, Int_t& nMatchesFull,
			 Int_t& nLeptonicMatchesMdcMeta);
			 


  Float_t getClosestCandidateAngularDistance();
  HRichHitSim* getClosestNeighbourRing();
  NEIGHBOUR findNeighbourType();

  Float_t getCorrelationType(Bool_t hasARing, Bool_t hasAKickTrack, Bool_t hasNSegments);

  Float_t getKickTrackType(Char_t* mode="withring");
  Float_t getRingType(HRichHitSim* theSimRing);

  Float_t getMdcPhi();

 private:

  Int_t evcount;
  //Pattern Vector to hold the current information for convenient storage in the tree
  Float_t PatternInstance[27];

  //Tree to store the patterns - written to output file
  TTree* tPatterns;
  TTree* tDoublePatterns;
  TTree* tHadronPatterns;
  TTree* tFakePatterns;

  TH1F* MomDistHadRing; 
  TH1F* MomDistLepRing; 
  TH1F* MomDistHadNoRing; 
  TH1F* MomDistLepNoRing; 
  TH1F* MomDistHadTotal; 
  TH1F* MomDistLepTotal; 
  TH1F* MomDistHadRatio; 
  TH1F* MomDistLepRatio; 

  //String holding the list of variables
  TString *LeafList;
  TString *LeafListDoubles;
  TString *LeafListFakes;

  //Destination file
  TString* OutFileName;

  //Categories and Iterators
  HCategory* pRichHitSimCategory;
  HCategory* pMdcSegCategory;
  HCategory* pMdcHitCategory;
  HCategory* pMdcClusInfCategory;
  HCategory* pGeantKineCategory;
  HCategory* pInputCategory;
  

  HIterator* pIteratorRichHitSim;
  HIterator* pIteratorMdcSeg;
  HIterator* pIteratorMdcHit;
  HIterator* pIteratorMdcClusInf;
  HIterator* pIteratorGeantKine;
  HIterator* pIteratorInput;
  HIterator* pAuxIterator;

  //Pointers to the candidate, the ring and the KickTrack
  HPidTrackCand* theCandidate;

  HRichHit*    theRing;
  HRichHitSim* theSimRing;
  HMdcSeg*  theSegment;

  HKickTrack*  theKickTrack;
  
  HGeantKine*  theKine;

  //Ananlysis Parameters of the RICH needed to determine search window width for neighbour search
  HRichAnalysisPar *pAnalysisPar;
  HRichGeometryPar *pGeometryPar;

  Bool_t USE_SIMINFO;
  Bool_t USE_SEGMENTS;  

  //Statistical variables
  //Fake Candidates with Ring and KT;
  //Int_t N_Cand_FakeRing_KT;

  //Good Candidates with Ring and KT;
  //Int_t N_Cand_GoodRing_KT;

  //Candidates with Ring and Segment;
  //Int_t N_Cand_Ring_Seg;
  
  //Candidates with no Ring but KT (Hadrons!);
  Int_t N_Cand_Hadron_KT[nMomentumBins];
  
  //Candidates with single-leptonic KT but NO ring
  Int_t N_Cand_RichIneffSingle[nMomentumBins];

  //Candidates with double-leptonic KT but NO ring
  Int_t N_Cand_RichIneffDouble[nMomentumBins];

  //Good candidates which are kicked out because a different KT 
  //matches better with the ring
  //Int_t N_Cand_SuppLeptons;

  //Good rings seen by the rich, but missed by the MDC
  Int_t N_Cand_MdcIneff;

  //Int_t N_TotCand;


 public:
  ClassDef(HPidPDFBuilderSim,0) 
};

#endif // HPIDPDFBUILDERSIM_H
