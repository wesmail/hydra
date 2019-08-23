#ifndef HPIDPDFMAKER_H
#define HPIDPDFMAKER_H

#include "hsudummyrec.h"
#include "TString.h"


#define MAX_NTUPLE_SIZE 200 
#define NTUPLE_VARS1 "f_kick_mom_exp:f_spline_mom_exp:f_rk_mom_exp:f_kick123_mom_exp:f_reft_mom_exp:n_sector:f_mom_geant:f_track_theta:f_track_phi:f_deltatheta:f_deltaphi:n_process:b_ring:b_kicktrack:b_splinetrack:b_rktrack:b_kick123track:b_reftrack:b_segments:f_shw_sum0:f_shw_sum1:f_shw_sum2:n_shw_cs0:n_shw_cs1:n_shw_cs2:f_shw_tof:f_tof_tof:n_shw_mult:i_tracktype:i_ringtype"

#define NTUPLE_VARS2 ":n_mdc_nw0:n_mdc_nw1:n_mdc_cfl0:n_mdc_cfl1:n_mdc_cls0:n_mdc_cls1:f_mdc_chi2:n_rich_patmatrix:n_rich_npads:f_rich_centroid:f_rich_avcharge:n_rich_locmax:i_geant_pid:n_system:f_metamatch_quality:f_kick_pull:f_spline_quality:f_distance_to_vertex:f_track_r_spline:f_track_z_spline:n_medium:b_corrRingSpline:b_corrRingKick:f_vertex_zcoord:n_charge_kick:n_charge_spline:f_track_z_kick:f_track_r_kick:i_geant_corrflag:f_track_qspline:f_track_chi2_out:b_is_cp_cand:f_angle_with_cp_candidate"

//#define NTUPLE_VARS3 ":f_kick_beta:f_kick123_beta:f_reft_beta:f_spline_beta:f_rk_beta:b_isdownscaled:n_tofino_mult:n_charge_rk:n_tof_clss:n_meta_cell:f_rk_quality:b_corrRingRK:f_track_r_rk:f_track_z_rk:f_kick_mass:f_kick123_mass:f_reft_mass:f_spline_mass:f_rk_mass:f_tof_leftamp:f_tof_rightamp:f_mdc_inner_dedx:f_mdc_outer_dedx:n_shw_row:n_shw_col:f_angle_to_closest_lep:f_angle_to_closest_had:b_closest_lep_isfitted"

//Since Hydra version 8.01 we use this definition
#define NTUPLE_VARS3 ":f_kick_beta:f_kick123_beta:f_reft_beta:f_spline_beta:f_rk_beta:b_isdownscaled:n_tofino_mult:n_charge_rk:n_tof_clss:n_meta_cell:f_rk_quality:b_corrRingRK:f_track_r_rk:f_track_z_rk:f_kick_mass:f_kick123_mass:f_reft_mass:f_spline_mass:f_rk_mass:f_tof_leftamp:f_tof_rightamp:f_mdc_inner_dedx:f_mdc_outer_dedx:n_shw_row:n_shw_col:f_angle_to_closest_fitted_lep:f_angle_to_closest_fitted_had:f_angle_to_closest_nonfitted_lep:f_angle_to_closest_nonfitted_had"

#define NTUPLE_VARS4 ":n_lepton_candidates:n_rich_houtra:f_distance_to_vertex_kick:f_distance_to_vertex_spline:f_distance_to_vertex_rk:f_meta_rk_dx:f_meta_rk_dy:f_meta_rk_dz:f_meta_x_err:f_meta_y_err:f_meta_z_err:f_meta_x:f_meta_y:f_meta_z:f_meta_mdc_dx:f_meta_mdc_dy:f_meta_mdc_dz:f_meta_local_x:f_meta_local_y:n_tof_module:n_tof_cell:f_tof_eloss:f_rk_theta:f_rk_phi:f_rich_phi:f_rich_theta:b_corrMetaKick:b_corrMetaSpline:b_corrMetaRK:f_outer_mdc_chi2:n_rk_ind:f_mdc_combined_dedx:f_mdc_combined_dedxsigma"

/*
#define AUX_NTUPLE_VARS "i_geant_corrflag:i_geant_pid:n_sector:f_kick_mom_exp:f_spline_mom_exp:f_track_theta:b_corrRingKick:b_corrRingSpline:f_deltatheta:f_deltaphi:f_track_qspline:f_track_chi2_out:b_is_cp_cand:f_angle_with_cp_candidate:b_corrRingRK:n_lepton_candidates"
*/
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
  HPidPdfMaker(const Char_t *pOutFileName="ntuple_out.root",Bool_t analyseSim=kTRUE);
  ~HPidPdfMaker();

  //all algorithms must have this
  virtual Bool_t init(void);
  virtual Int_t  execute(void);
  virtual Bool_t finalize(void);

  Bool_t hasATrack(void); //Returns kTRUE if at least one tracking method was successfull
 private:

  void clearMembers();    //Set all datamembers back to default values
  void clearArray();      //Clear the data in the pattern list for the next instance of HPidTrackCand

  Bool_t analyseSimData;

  //Parameter containers required for this reconstructor
  HRichGeometryPar* pRichGeometryPar;

  HPidHitData* pHitData;
  HPidTrackData* pTrackData;
  HPidGeantTrackSet* pGeantTrackSet;

  HIterator *pItTrackCand;                //iterator over HPidTrackCand objects
  HPidTrackCandSim *pTrackCand;           //track candidate to be investigated
  HShowerHitTof *pShowerHitTof;           //pointer to META hit
  HKickTrackSim *pKickTrack;              //KickTrack of this trackCandidate
  HRichHitSim* pRichHit;                  //Ring of the Candidate
  TNtuple *pNtuple;                       //output ntuple
  //TNtuple *auxpNtuple;                    // auxiliary output ntuple with fewer variables
  TString sOutFileName;                   //ouput file name 
  
  //Ntuple and variable indices
  Float_t aPatternInstance[MAX_NTUPLE_SIZE];
  //Float_t aAuxPatternInstance[MAX_NTUPLE_SIZE];

  //Categories
  HCategory* pCatPidTrkCand; // input category;
  HCategory *pCatKine; //GeantKine category
  HCategory *pCatMdcSeg; //Mdc Seg Category
  HCategory *pCatMdcHit; //Mdc Hit Category
  HCategory *pCatKickTrack; //Kicktrack Category

  //SubRoutines called in execute function to store data from hits for later
  //correlation purposes

  //Check how many leptons contributed to this track before the field
  Float_t getTrackType(HPidTrackCandSim* pTrackCand);

  //Check how many leptons contributed to this ring
  Float_t getRingType(HRichHitSim* pRichHit);

  
  Bool_t bookRingObservables(); //Collect ring information into ntuple
  Bool_t bookTrackObservables(); //Collect tracking information into ntuple
  Bool_t bookSegmentObservables(); //Collect segment information into ntuple
  Bool_t bookCommonObservables(); //Collect common ....
  Bool_t bookVertexInformation(); //Collect vertex

  
  //Make sure that Geant Kine objects of the current event are all there and are sorted by
  //track number
  void checkGeantTrackNumbers();

  //Was the current Event a downscaled one? Important because only downscaled and simulated events
  //can be compared
  Bool_t getDownscalingFlag();

  
  enum enumData{
 f_kick_mom_exp,f_spline_mom_exp,f_rk_mom_exp,f_kick123_mom_exp,f_reft_mom_exp,n_sector,f_mom_geant,
 f_track_theta,f_track_phi,f_deltatheta,f_deltaphi,n_process,b_ring,b_kicktrack,b_splinetrack,b_rktrack,
 b_kick123track,b_reftrack,b_segments,f_shw_sum0,f_shw_sum1,f_shw_sum2,n_shw_cs0,n_shw_cs1,n_shw_cs2,
 f_shw_tof,f_tof_tof,n_shw_mult,i_tracktype,i_ringtype,n_mdc_nw0,n_mdc_nw1,n_mdc_cfl0,n_mdc_cfl1,
 n_mdc_cls0,n_mdc_cls1,f_mdc_chi2,n_rich_patmatrix,n_rich_npads,f_rich_centroid,f_rich_avcharge,
 n_rich_locmax,i_geant_pid,n_system,f_metamatch_quality,f_kick_pull,f_spline_quality,f_distance_to_vertex,
 f_track_r_spline,f_track_z_spline,n_medium,b_corrRingSpline,b_corrRingKick,f_vertex_zcoord,n_charge_kick,
 n_charge_spline,f_track_z_kick,f_track_r_kick,i_geant_corrflag,f_track_qspline,f_track_chi2_out,
 b_is_cp_cand,f_angle_with_cp_candidate,f_kick_beta,f_kick123_beta,f_reft_beta,f_spline_beta,
 f_rk_beta,b_isdownscaled,n_tofino_mult,n_charge_rk,n_tof_clss,n_meta_cell,f_rk_quality,b_corrRingRK,
 f_track_r_rk,f_track_z_rk,f_kick_mass,f_kick123_mass,f_reft_mass,f_spline_mass,f_rk_mass,f_tof_leftamp,
 f_tof_rightamp,f_mdc_inner_dedx,f_mdc_outer_dedx,n_shw_row,n_shw_col,f_angle_to_closest_fitted_lep,
 f_angle_to_closest_fitted_had,f_angle_to_closest_nonfitted_lep,f_angle_to_closest_nonfitted_had,
 n_lepton_candidates,n_rich_houtra,f_distance_to_vertex_kick,f_distance_to_vertex_spline,
 f_distance_to_vertex_rk,f_meta_rk_dx,f_meta_rk_dy,f_meta_rk_dz,f_meta_x_err,f_meta_y_err,
 f_meta_z_err,f_meta_x,f_meta_y,f_meta_z,f_meta_mdc_dx,f_meta_mdc_dy,f_meta_mdc_dz,f_meta_local_x,
 f_meta_local_y,n_tof_module,n_tof_cell,f_tof_eloss,f_rk_theta,f_rk_phi,f_rich_phi,f_rich_theta,
 b_corrMetaKick,b_corrMetaSpline,b_corrMetaRK,f_outer_mdc_chi2,n_rk_ind,f_mdc_combined_dedx,f_mdc_combined_dedxsigma};
  
  
  ClassDef(HPidPdfMaker, 0)
    };
#endif /*!HPIDPDFMAKER_H*/
	  
