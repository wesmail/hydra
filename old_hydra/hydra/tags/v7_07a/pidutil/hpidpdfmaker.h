#ifndef HPIDPDFMAKER_H
#define HPIDPDFMAKER_H

#include "hsudummyrec.h"
#include "TString.h"


#define MAX_NTUPLE_SIZE 200 
#define NTUPLE_VARS1 "f_kick_mom_exp:f_spline_mom_exp:f_rk_mom_exp:f_kick123_mom_exp:f_reft_mom_exp:n_sector:f_mom_geant:f_track_theta:f_track_phi:f_deltatheta:f_deltaphi:n_process:b_ring:b_kicktrack:b_splinetrack:b_rktrack:b_kick123track:b_reftrack:b_segments:f_shw_sum0:f_shw_sum1:f_shw_sum2:n_shw_cs0:n_shw_cs1:n_shw_cs2:f_shw_tof:f_tof_tof:n_shw_mult:i_tracktype:i_ringtype"

#define NTUPLE_VARS2 ":n_mdc_nw0:n_mdc_nw1:n_mdc_cfl0:n_mdc_cfl1:n_mdc_cls0:n_mdc_cls1:f_mdc_chi2:n_rich_patmatrix:n_rich_npads:f_rich_centroid:f_rich_avcharge:n_rich_locmax:i_geant_pid:n_system:f_metamatch_quality:f_kick_pull:f_spline_quality:f_distance_to_vertex:f_track_r_spline:f_track_z_spline:n_medium:b_corrRingSpline:b_corrRingKick:f_vertex_zcoord:n_charge_kick:n_charge_spline:f_track_z_kick:f_track_r_kick:i_geant_corrflag:f_track_qspline:f_track_chi2_out:b_is_cp_cand:f_angle_with_cp_candidate"

#define NTUPLE_VARS3 ":f_kick_beta:f_kick123_beta:f_reft_beta:f_spline_beta:f_rk_beta:b_isdownscaled:n_tofino_mult:n_charge_rk:n_tof_clss:n_meta_cell:f_rk_quality:b_corrRingRK:f_track_r_rk:f_track_z_rk"

#define AUX_NTUPLE_VARS "i_geant_corrflag:i_geant_pid:n_sector:f_kick_mom_exp:f_spline_mom_exp:f_track_theta:b_corrRingKick:b_corrRingSpline:f_deltatheta:f_deltaphi:f_track_qspline:f_track_chi2_out:b_is_cp_cand:f_angle_with_cp_candidate:b_corrRingRK"

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
class HPidTrackCandSim;
class HPidTrackData;
class HPidHitData;
class HPidGeantTrackSet;
class HShowerHitTof;
class HKickTrackSim;
class HRichHitSim;
class HRichGeometryPar;


class HPidPdfMaker : public HSUDummyRec{
 public:
  HPidPdfMaker(const char *pOutFileName="ntuple_out.root",Bool_t analyseSim=kTRUE);
  ~HPidPdfMaker();
  virtual Bool_t init(void);
  virtual Int_t  execute(void);
  virtual Bool_t finalize(void);
  Bool_t hasATrack(void);
 private:
  void clearMembers();  
  void clearArray();

  Bool_t analyseSimData;

  HRichGeometryPar* pRichGeometryPar;

  HPidHitData* pHitData;
  HPidTrackData* pTrackData;
  HPidGeantTrackSet* pGeantTrackSet;

  HIterator *pItTrackCand; //iterator over HPidTrackCand objects
  HPidTrackCandSim *pTrackCand; //track candidate
  HShowerHitTof *pShowerHitTof;
  HKickTrackSim *pKickTrack; //KickTrack of this trackCandidate
  HRichHitSim* pRichHit; //Ring of the Candidate
  TNtuple *pNtuple; //output ntuple
  TNtuple *auxpNtuple; // auxiliary output ntuple
  TString sOutFileName; //ouput file name 
  
  //Ntuple and variable indices
  Float_t aPatternInstance[MAX_NTUPLE_SIZE];
  Float_t aAuxPatternInstance[MAX_NTUPLE_SIZE];
  //Categories
  HCategory* pCatPidTrkCand; // input category; - temporary only
  HCategory *pCatKine; //GeantKine category
  HCategory *pCatMdcSeg; //Mdc Seg Category
  HCategory *pCatMdcHit; //Mdc Hit Category
  HCategory *pCatMdcClusInf; //Mdc Cluster Category
  
  Float_t getTrackType(HPidTrackCandSim* pTrackCand);
  Float_t getRingType(HRichHitSim* pRichHit);
  Bool_t bookRingObservables();
  Bool_t bookTrackObservables();
  Bool_t bookSegmentObservables();
  Bool_t bookCommonObservables();
  Bool_t bookVertexInformation();
  Bool_t checkConsistency();
  void checkGeantTrackNumbers();
  Bool_t getDownscalingFlag();

  
  enum enumData{
    f_kick_mom_exp,f_spline_mom_exp,f_rk_mom_exp,f_kick123_mom_exp,f_reft_mom_exp,n_sector,f_mom_geant,f_track_theta,f_track_phi,f_deltatheta,f_deltaphi,n_process,b_ring,b_kicktrack,b_splinetrack,b_rktrack,b_kick123track,b_reftrack,b_segments,f_shw_sum0,f_shw_sum1,f_shw_sum2,n_shw_cs0,n_shw_cs1,n_shw_cs2,f_shw_tof,f_tof_tof,n_shw_mult,i_tracktype,i_ringtype,n_mdc_nw0,n_mdc_nw1,n_mdc_cfl0,n_mdc_cfl1,n_mdc_cls0,n_mdc_cls1,f_mdc_chi2,n_rich_patmatrix,n_rich_npads,f_rich_centroid,f_rich_avcharge,n_rich_locmax,i_geant_pid,n_system,f_metamatch_quality,f_kick_pull,f_spline_quality,f_distance_to_vertex,f_track_r_spline,f_track_z_spline, n_medium, b_corrRingSpline,b_corrRingKick,f_vertex_zcoord,n_charge_kick,n_charge_spline,f_track_z_kick,f_track_r_kick,i_geant_corrflag,f_track_qspline,f_track_chi2_out,b_is_cp_cand,f_angle_with_cp_candidate,f_kick_beta,f_kick123_beta,f_reft_beta,f_spline_beta,f_rk_beta,b_isdownscaled,n_tofino_mult,n_charge_rk,n_tof_clss,n_meta_cell,f_rk_quality,b_corrRingRK,f_track_r_rk,f_track_z_rk};
  enum auxenumData {
    aux_i_geant_corrflag,aux_i_geant_pid,aux_n_sector,aux_f_kick_mom_exp,aux_f_spline_mom_exp,aux_f_track_theta,aux_b_corrRingKick,aux_b_corrRingSpline,aux_f_deltatheta,aux_f_deltaphi,aux_f_track_qspline,aux_f_track_chi2_out,aux_b_is_cp_cand,aux_f_angle_with_cp_candidate
  };

  ClassDef(HPidPdfMaker, 0)
    };
#endif /*!HPIDPDFMAKER_H*/
	  
