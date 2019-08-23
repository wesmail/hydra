#ifndef HPIDPERFORMACETEST_H
#define HPIDPERFORMACETEST_H

#include "hsudummyrec.h"
#include "TString.h"
#define MAX_META_TRACKS 2
#define MAX_RICH_TRACKS 3
#define MAX_NTUPLE_SIZE 200 
#define PID_NTUPLE_VARS_1 "p_rec_id:p_geant_id:p_system:p_charge:p_rec_mom:p_geant_mom:p_beta:p_sector:p_theta:p_phi:p_tof:p_geant_flag:p_meta_quality"
#define PID_NTUPLE_VARS_2 "p_corr_flag_0:p_corr_flag_1:p_corr_flag_2:p_corr_flag_3:p_corr_flag_4"
#define PID_NTUPLE_VARS_3 "p_corr_trk_id_0:p_corr_trk_id_1:p_corr_trk_id_2:p_corr_trk_id_3:p_corr_trk_id_4"
#define PID_NTUPLE_VARS_4 "p_corr_geant_id_0:p_corr_geant_id_1:p_corr_geant_id_2:p_corr_geant_id_3:p_corr_geant_id_4"
#define PID_NTUPLE_VARS_5 "p_corr_process_id_0:p_corr_process_id_1:p_corr_process_id_2:p_corr_process_id_3:p_corr_process_id_4"
#define PID_NTUPLE_VARS_6 "p_rich_flag:p_ring_pad_nr:p_ring_centroind:p_rich_theta:p_rich_phi:p_ring_pat_mat:p_ring_hou_tra:p_ring_ampli:p_ring_local_max4"
#define PID_NTUPLE_VARS_7 "p_shw_flag:p_shw_sum_0:p_shw_sum_1:p_shw_sum_2:p_shw_cls_0:p_shw_cls_1:p_shw_cls_2:p_tof_mult"
#define PID_NTUPLE_VARS_8 "p_shw_ep:p_shw_em:p_shw_pp:p_shw_pm:p_shw_p:p_shw_fp:p_shw_fm"
#define PID_NTUPLE_VARS_9 "p_rich_ep:p_rich_em:p_rich_pp:p_rich_pm:p_rich_p:p_rich_fp:p_rich_fm"
#define PID_NTUPLE_VARS_10 "p_mvb_ep:p_mvb_em:p_mvb_pp:p_mvb_pm:p_mvb_p:p_mvb_fp:p_mvb_fm"
#define PID_NTUPLE_VARS_11 "p_relint_ep:p_relint_em:p_relint_pp:p_relint_pm:p_relint_p:p_relint_fp:p_relint_fm"
#define PID_NTUPLE_VARS_12 "p_merged_ep:p_merged_em:p_merged_pp:p_merged_pm:p_merged_p:p_merged_fp:p_merged_fm"
#define PID_NTUPLE_VARS_13 "p_bayes_ep:p_bayes_em:p_bayes_pp:p_bayes_pm:p_bayes_p:p_bayes_fp:p_bayes_fm"
#define PID_NTUPLE_VARS_14 "p_trigger_ds_flag:p_trigger_decision:p_qspline:p_rich_corr_spline:p_rich_corr_kick:p_rich_corr_rk"
#define PID_NTUPLE_VARS_15 "p_corr_parent_id_0:p_corr_parent_id_1:p_corr_parent_id_2:p_corr_parent_id_3:p_corr_parent_id_4"
#define PID_NTUPLE_VARS_16 "p_eloss_kick:p_eloss_spline:p_eloss_rk:p_kick_vertex:p_spline_vertex:p_rk_vertex"
#define PID_NTUPLE_VARS_17 "p_parent_0:p_parent_1:p_parent_2:p_parent_3:p_parent_4"
#define PID_NTUPLE_VARS_18 "p_parent_trk_id_0:p_parent_trk_id_1:p_parent_trk_id_2:p_parent_trk_id_3:p_parent_trk_id_4"
#define PID_NTUPLE_VARS_19 "p_medium_0:p_medium_1:p_medium_2:p_medium_3:p_medium_4"
#define PID_NTUPLE_VARS_20 "p_geant_mom_0:p_geant_mom_1:p_geant_mom_2:p_geant_mom_3:p_geant_mom_4"
#define PID_NTUPLE_VARS_21 "p_test_value:p_dist_vert_spline:p_dist_vert_kick:p_dist_vert_rk:p_inner_chi2:p_outer_chi2:p_track_r:p_track_z"
#define PID_NTUPLE_VARS_22 "p_rec_mom_spline:p_rec_mom_kick:p_rec_mom_rk:p_shw_r:p_shw_phi:p_shw_theta"
#define PID_NTUPLE_VARS_23 "p_iscpcandidate:p_opang_cpcandidate:p_rk_quality:p_spline_quality:p_kick_pull:p_tof_leftamp:p_tof_rightamp"
//#define PID_NTUPLE_VARS_24 "p_n_corr_tracks:p_q_io_matching:p_mdc_inner_dedx:p_mdc_outer_dedx:p_shw_row:p_shw_col:p_angle_to_closest_lep:p_angle_to_closest_had:p_closest_lep_isfitted:p_closest_had_isfitted:p_n_lepton_candidates"
//since hydra version 8.01 we use this definition
#define PID_NTUPLE_VARS_24 "p_n_corr_tracks:p_q_io_matching:p_mdc_inner_dedx:p_mdc_outer_dedx:p_shw_row:p_shw_col:p_angle_to_closest_fitted_lep:p_angle_to_closest_fitted_had:p_angle_to_closest_nonfitted_lep:p_angle_to_closest_nonfitted_had"
#define PID_NTUPLE_VARS_25 "p_n_lepton_candidates:p_geant_vert_x_0:p_geant_vert_x_1:p_geant_vert_y_0:p_geant_vert_y_1:p_geant_vert_z_0:p_geant_vert_z_1"
#define PID_NTUPLE_VARS_26 "p_is_lep_from_primary:p_grandparent_0:p_grandparent_1:p_grandparent_2:p_grandparent_3:p_grandparent_4:p_mdc_combined_dedx:p_mdc_combined_dedxsigma"

////////////////////////////////////////////////////////////////////////////////
// HPidPerformaceTest                                                         //
// Reconstructor fills ntuple with data used during pid analysis (sim & real )//
// Based on Tassilo's concept of "hpidtest"                                   //
// twojcik@if.uj.edu.pl                                                       //
////////////////////////////////////////////////////////////////////////////////

class TNtuple;
class HIterator;
class HCategory;
class HGeantKine;
class HPidTrackCand;
class HPidCandidate;
class HPidParticleSim;
class HPidParticle;
class HPidGeantTrackSet;
class HShowerHitTof;
class HTofHit;
class HKickTrack;
class HPidHitData;
class HPidTrackData;

class HPidPerformaceTest : public HSUDummyRec{
    public:
        HPidPerformaceTest(const char *pOutFileName="pid_ntuple_out.root",Bool_t bUseAllSpecies=1);
        ~HPidPerformaceTest();
        virtual Bool_t init(void);
        virtual Int_t  execute(void);
        virtual Bool_t finalize(void);
	Int_t checkGeantTrackSet(HPidGeantTrackSet* pGTS);
    private:
        void clearMembers();
        HIterator *pItTrackCand;       //iterator over HPidTrackCand objects
        HIterator *pItCand;            //iterator over HPidCand objects
        HIterator *pItPart;            //iterator over HPidParticleSimExtHit
        HPidTrackCand *pTrackCand;     //track candidate
        HPidCandidate *pCand;          //particle candidate
        HPidHitData *pHitData;         //hit data 
        HPidTrackData *pTrackData;     //track data 
        const HPidGeantTrackSet *pGeantSet;  //particle
        HPidParticle    *pPart;        //particle
        HShowerHitTof *pShwHit;        //showerhitf
        HKickTrack *pKickTrack;        //kicktrack
        HTofHit *pTof;                 //showerhitf
        HGeantKine *pKine;             //GeantKine category
        TNtuple *pNtuple;              //output ntuple
        TString sOutFileName;          //ouput file name 
        Bool_t m_bSimData;             //kTRUE if simulation data   
	Bool_t bUseAllSpecies;         //Take into account hadrons as well
        //Ntuple and variable indices
        Float_t aNtupleVars[MAX_NTUPLE_SIZE];
	
        enum enumData{
	  p_rec_id,p_geant_id,p_system,p_charge,p_rec_mom,p_geant_mom,p_beta,p_sector,p_theta,p_phi,p_tof,p_geant_flag,p_meta_quality,
	  p_corr_flag_0,p_corr_flag_1,p_corr_flag_2,p_corr_flag_3,p_corr_flag_4,
	  p_corr_trk_id_0,p_corr_trk_id_1,p_corr_trk_id_2,p_corr_trk_id_3,p_corr_trk_id_4,
	  p_corr_geant_id_0,p_corr_geant_id_1,p_corr_geant_id_2,p_corr_geant_id_3,p_corr_geant_id_4,
	  p_corr_process_id_0,p_corr_process_id_1,p_corr_process_id_2,p_corr_process_id_3,p_corr_process_id_4,
	  p_rich_flag,p_ring_pad_nr,p_ring_centroind,p_rich_theta,_p_rich_phi,p_ring_pat_mat,p_ring_hou_tra,p_ring_ampli,p_ring_local_max4,
	  p_shw_flag,p_shw_sum_0,p_shw_sum_1,p_shw_sum_2,p_shw_cls_0,p_shw_cls_1,p_shw_cls_2,p_tof_mult,
	  p_shw_ep,p_shw_em,p_shw_pp,p_shw_pm,p_shw_p,p_shw_fp,p_shw_fm,
	  p_rich_ep,p_rich_em,p_rich_pp,p_rich_pm,p_rich_p,p_rich_fp,p_rich_fm,
	  p_mvb_ep,p_mvb_em,p_mvb_pp,p_mvb_pm,p_mvb_p,p_mvb_fp,p_mvb_fm,
	  p_relint_ep,p_relint_em,p_relint_pp,p_relint_pm,p_relint_p,p_relint_fp,p_relint_fm,
	  p_merged_ep,p_merged_em,p_merged_pp,p_merged_pm,p_merged_p,p_merged_fp,p_merged_fm,
	  p_bayes_ep,p_bayes_em,p_bayes_pp,p_bayes_pm,p_bayes_p,p_bayes_fp,p_bayes_fm,
	  p_trigger_ds_flag,p_trigger_decision,p_qspline,p_rich_corr_spline,p_rich_corr_kick,p_rich_corr_rk,
	  p_corr_parent_id_0,p_corr_parent_id_1,p_corr_parent_id_2,p_corr_parent_id_3,p_corr_parent_id_4,
	  p_eloss_kick,p_eloss_spline,p_eloss_rk,p_kick_vertex,p_spline_vertex,p_rk_vertex,
	  p_parent_0,p_parent_1,p_parent_2,p_parent_3,p_parent_4,
	  p_parent_trk_id_0,p_parent_trk_id_1,p_parent_trk_id_2,p_parent_trk_id_3,p_parent_trk_id_4,
	  p_medium_0,p_medium_1,p_medium_2,p_medium_3,p_medium_4,
	  p_geant_mom_0,p_geant_mom_1,p_geant_mom_2,p_geant_mom_3,p_geant_mom_4,
	  p_test_value,p_dist_vert_spline,p_dist_vert_kick,p_dist_vert_rk,p_inner_chi2,p_outer_chi2,p_track_r,p_track_z,
	  p_rec_mom_spline,p_rec_mom_kick,p_rec_mom_rk,p_shw_r,p_shw_phi,p_shw_theta,
	  p_iscpcandidate,p_opang_cpcandidate,p_rk_quality,p_spline_quality,p_kick_pull,p_tof_leftamp,p_tof_rightamp,
	  p_n_corr_tracks,p_q_io_matching,p_mdc_inner_dedx,p_mdc_outer_dedx,p_shw_row,p_shw_col,
	  p_angle_to_closest_fitted_lep,p_angle_to_closest_fitted_had,
	  p_angle_to_closest_nonfitted_lep, p_angle_to_closest_nonfitted_had,
	  p_n_lepton_candidates,p_geant_vert_x_0,p_geant_vert_x_1,p_geant_vert_y_0,p_geant_vert_y_1,p_geant_vert_z_0,
	  p_geant_vert_z_1,p_is_lep_from_primary,p_grandparent_0,p_grandparent_1,p_grandparent_2,p_grandparent_3,
	  p_grandparent_4,p_mdc_combined_dedx,p_mdc_combined_dedxsigma
        };

ClassDef(HPidPerformaceTest,0)
};
#endif /*!HPIDPERFORMACETEST_H*/

