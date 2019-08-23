// hpidPDFBuilderExp.h, by Tassilo Christ 23.07.03
// Reconstructor class intended to construct Ntuples that extract statistical
// information about PidTrackCandidates found by the PIDTrackFiller
// Ntuples from sim and exp data are to be compared with respect to statistical correlations
// --- To be upon exp AND sim data!



#ifndef  HPIDPDFBUILDEREXP_H
#define  HPIDPDFBUILDEREXP_H
#include "hreconstructor.h"
#include "haddef.h"
#include "piddef.h"

class TTree;
class HCategory;
class HIterator;
class HPidTrackCand;
class HRichHit;
class HKickTrack;
class HRichAnalysisPar;
class HRichGeometryPar;
class HMdcSeg;
class HMdcHit;


//These are the opservables which are to be correlated
//Attention! Keep the order
#define n_dPhi           0 
#define n_dTheta         1 
#define n_pmq            2 
#define n_padnr          3 
#define n_centroid       4 
#define n_locmax         5 
#define n_avcharge       6 
#define n_theta          7 
#define n_phi            8 
#define n_sector         9 
#define n_openingAngle   10
#define n_mdcchisquare   11
#define n_nsegments      12
#define n_nwiresmdc0     13
#define n_nwiresmdc1     14
#define n_clussizemdc0   15
#define n_clussizemdc1   16  
			  
class HPidPDFBuilderExp : public HReconstructor 
{
 public:
  

  // default settings: 
  HPidPDFBuilderExp(Char_t* Extension = "",Char_t* OutputFile = "CandidateProperties");
  ~HPidPDFBuilderExp();

  
  Bool_t init();
  Bool_t finalize();
  Int_t execute();
  Bool_t clearPattern();
  
  Bool_t bookRingObservables();
  Bool_t bookKickObservables();
  
  Bool_t checkForRing();
  Bool_t checkForKickTrack();
  Int_t checkForSegments();


  Float_t getClosestCandidateAngularDistance();

  //Float_t getCorrelationType(Bool_t hasARing, Bool_t hasAKickTrack, Bool_t hasNSegments);

  Float_t getMdcPhi();

 private:

  //We use a tree to store the patterns because an Ntuple doesn't have enough data members
  TTree* CandidateProperties;

  Int_t evcount;

  //Pattern Vector to hold the current information for convenient storage in the tree
  Float_t PatternInstance[17];

  //String holding the list of variables
  TString *LeafList;

  //Destination file
  TString* OutFileName;

  //Categories and Iterators
  HCategory* pRichHitCategory;
  HCategory* pMdcSegCategory;
  HCategory* pMdcHitCategory;
  HCategory* pMdcClusInfCategory;
  HCategory* pInputCategory;
  

  HIterator* pIteratorRichHit;
  HIterator* pIteratorMdcSeg;
  HIterator* pIteratorMdcHit;
  HIterator* pIteratorMdcClusInf;
  HIterator* pIteratorInput;
  HIterator* pAuxIterator;

  //Pointers to the candidate, the ring and the KickTrack
  HPidTrackCand* theCandidate;

  HRichHit*    theRing;
  HMdcSeg*  theSegment;
  HKickTrack*  theKickTrack;

  //Ananlysis Parameters of the RICH needed to determine search window width for neighbour search
  HRichAnalysisPar *pAnalysisPar;
  HRichGeometryPar *pGeometryPar;

  //Total number of candidates
  Int_t N_Cand_Total;


 public:
  ClassDef(HPidPDFBuilderExp,0) 
};

#endif // HPIDPDFBUILDEREXP_H
