#ifndef HPIDPDFMAKER_H
#define HPIDPDFMAKER_H

#include "hsudummyrec.h"
#include "TString.h"


#define MAX_NTUPLE_SIZE 200 
#define NTUPLE_VARS1 "f_kick_mom_exp:n_kick_sector:f_mom_geant:f_kick_theta:f_kick_phi:f_deltatheta:f_deltaphi:n_process:b_ring:b_kicktrack:b_segments:f_shw_sum0:f_shw_sum1:f_shw_sum2:f_shw_cs0:f_shw_cs1:f_shw_cs2:f_shw_tof:n_shw_mult:i_tracktype:i_ringtype"
#define NTUPLE_VARS2 ":n_mdc_nw0:n_mdc_nw1:n_mdc_cfl0:n_mdc_cfl1:n_mdc_cls0:n_mdc_cls1:f_mdc_chi2:n_rich_patmatrix:n_rich_npads:f_rich_centroid:f_rich_avcharge:n_rich_locmax:i_geant_pid:n_system"

////////////////////////////////////////////////////////////////////////////////
// HPidPdfMaker                                                               //
// Reconstructor fills ntuple with data used  during pdf prodction            //
// for PreShower and Rich algortihms                                          //
// The parameter varlist has to be defined above!!!                           //
// enum "enumData" is used to have human-readable indices and must be         //
// synchronized with varlist                                                  //
////////////////////////////////////////////////////////////////////////////////

class TNtuple;
class HIterator;
class HCategory;
class HPidTrackCand;
class HShowerHitTof;
class HKickTrackSim;
class HRichHitSim;
class HRichGeometryPar;
class HPidPdfMaker : public HSUDummyRec{
 public:
  HPidPdfMaker(const char *pOutFileName="ntuple_out.root");
  ~HPidPdfMaker();
  virtual Bool_t init(void);
  virtual Int_t  execute(void);
  virtual Bool_t finalize(void);
 private:
  void clearMembers();  
  void clearArray();

  HRichGeometryPar* pRichGeometryPar;

  HIterator *pItTrackCand; //iterator over HPidTrackCand objects
  HPidTrackCand *pTrackCand; //track candidate
  HShowerHitTof *pShowerHitTof;
  HKickTrackSim *pKickTrack; //KickTrack of this trackCandidate
  HRichHitSim* pRichHit; //Ring of the Candidate
  TNtuple *pNtuple; //output ntuple
  TString sOutFileName; //ouput file name 
  
  //Ntuple and variable indices
  Float_t aPatternInstance[MAX_NTUPLE_SIZE];
  
  //Categories
  HCategory *pCatKine; //GeantKine category
  HCategory *pCatMdcSeg; //Mdc Seg Category
  HCategory *pCatMdcHit; //Mdc Hit Category
  HCategory *pCatMdcClusInf; //Mdc Cluster Category
  
  Float_t getTrackType(HPidTrackCand* pTrackCand);
  Float_t getRingType(HRichHitSim* pRichHit);
  Bool_t bookRingObservables();
  Bool_t bookTrackObservables();
  Bool_t bookSegmentObservables();
  Bool_t bookCommonObservables();
  Float_t getMdcPhi();

  enum enumData{
    f_kick_mom_exp,n_kick_sector,f_mom_geant,f_kick_theta,f_kick_phi,f_deltatheta,f_deltaphi,n_process,b_ring,b_kicktrack,b_segments,f_shw_sum0,f_shw_sum1,f_shw_sum2,n_shw_cs0,n_shw_cs1,n_shw_cs2,f_shw_tof,n_shw_mult,i_tracktype,i_ringtype,n_mdc_nw0,n_mdc_nw1,n_mdc_cfl0,n_mdc_cfl1,n_mdc_cls0,n_mdc_cls1,f_mdc_chi2,n_rich_patmatrix,n_rich_npads,f_rich_centroid,f_rich_avcharge,n_rich_locmax,i_geant_pid,n_system
  };
  
	ClassDef(HPidPdfMaker,0)
};
#endif /*!HPIDPDFMAKER_H*/
	  
