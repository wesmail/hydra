#ifndef HTRACKNTUPLEMAKER_H
#define HTRACKNTUPLEMAKER_H

//-----------------------------------------------------------------------------
// HTrackCandNtupleMaker                                                      
// Reconstructor fills ntuple with data from HPidTrackCandidate category.     
// The ntuple can be used later to create parameters for pid algorithms.
// 
// enum "enumData" is used to have human-readable indices and must be         
// synchronized with varlist                                                  
// 
//--*Designed: Tassilo Christ & Tomasz Wojcik 
//--*Modified: Jacek Otwinowski 15/02/2007
//-----------------------------------------------------------------------------

#include "hsudummyrec.h"
#include "TString.h"

#define MAX_NTUPLE_SIZE 200 
#define TRACK_NTUPLE_VARS1 "f_kick_mom_exp:f_spline_mom_exp:f_rk_mom_exp:f_kick123_mom_exp:f_reft_mom_exp:n_sector:f_mom_geant:f_track_theta:f_track_phi:f_deltatheta:f_deltaphi:n_process:b_ring:b_kicktrack:b_splinetrack:b_rktrack:b_kick123track:b_reftrack:n_close_segments:f_shw_sum0:f_shw_sum1:f_shw_sum2:n_shw_cs0:n_shw_cs1:n_shw_cs2:f_shw_tof:f_tof_tof:n_tofino_mult:i_tracktype"

#define TRACK_NTUPLE_VARS2 ":f_mdc_chi2:n_rich_patmatrix:n_rich_npads:f_rich_centroid:f_rich_avcharge:n_rich_locmax:i_geant_pid:n_system:f_metamatch_quality:f_kick_pull:f_spline_quality:f_track_r_spline:f_track_z_spline:n_medium:b_corrRingSpline:b_corrRingKick:f_vertex_zcoord:n_charge_kick:n_charge_spline:f_track_z_kick:f_track_r_kick:i_geant_corrflag"

#define TRACK_NTUPLE_VARS3 ":f_kick_beta:f_kick123_beta:f_reft_beta:f_spline_beta:f_rk_beta:b_isdownscaled:n_charge_rk:n_tof_clss:n_meta_cell:f_rk_quality:b_corrRingRK:f_track_r_rk:f_track_z_rk:f_kick_mass:f_kick123_mass:f_reft_mass:f_spline_mass:f_rk_mass:f_tof_leftamp:f_tof_rightamp:f_mdc_inner_dedx:f_mdc_outer_dedx:n_shw_row:n_shw_col:f_angle_to_closest_fitted_lep:f_angle_to_closest_fitted_had:f_angle_to_closest_nonfitted_lep:f_angle_to_closest_nonfitted_had"

#define TRACK_NTUPLE_VARS4 ":n_lepton_candidates:n_rich_houtra:f_distance_to_vertex_kick:f_distance_to_vertex_spline:f_distance_to_vertex_rk:f_meta_rk_dx:f_meta_rk_dy:f_meta_rk_dz:f_meta_x_err:f_meta_y_err:f_meta_z_err:f_meta_x:f_meta_y:f_meta_z:f_meta_mdc_dx:f_meta_mdc_dy:f_meta_mdc_dz:f_meta_local_x:f_meta_local_y:n_tof_module:n_tof_cell:f_tof_eloss:f_rk_theta:f_rk_phi:f_rich_phi:f_rich_theta:b_corrMetaKick:b_corrMetaSpline:b_corrMetaRK:f_outer_mdc_chi2:n_rk_ind:f_mdc_combined_dedx:f_mdc_combined_dedxsigma"

// variable description (order like in TBrowser)
// 
// b_corrMetaKick    - correlation Kick track with META hit is found 
// b_corrMetaRK      - correlation Runge Kutta track with META hit is found 
// b_corrMetaSpline  - correlation Spline track with META hit is found 
// b_corrRingKick    - correlation Kick track with Rich hit is found 
// b_corrRingRK      - correlation Runge Kutta track with Rich hit is found
// b_corrRingSpline  - correlation Spline track with Rich hit is found
// b_isdownscaled    - LVL1 or LVL2 triggered event (1-LVL1, 0-LVL2)
// b_kick123track    - kick123 track is found
// b_kicktrack       - kick track is found
// b_reftrack        - reference trajectory track is found
// b_ring            - Rich ring is found 
// b_rktrack         - Runge Kutta track is found
// b_splinetrack     - Spline track is found
// f_angle_to_closest_fitted_had    - angle distance to the closest fitted hadron (inner MDC)[deg]
// f_angle_to_closest_fitted_lep    - angle distance to the closest fitted lepton
// f_angle_to_closest_nonfitted_had - angle distance to the closest not fitted hadron 
// f_angle_to_closest_nonfitted_lep - angle distance to the closest not fitted lepton
// f_deltaphi 	- (Rich_phi-Mdc_phi)*sin(Mdc_theta) Rich and innerMDC [deg]
// f_deltatheta - (Rich_theta-Mdc_theta) Rich and innerMDC   
// f_distance_to_vertex_kick   - distance to event vertex for Kick track [mm]
// f_distance_to_vertex_rk     - distance to event vertex for Runge Kutta track 
// f_distance_to_vertex_spline - distance to event vertex for Spline track 
// f_kick123_beta     - particle beta (v/c) for kick123 track
// f_kick123_mass     - particle mass reconstructed for kick123 track (MeV/c^2)
// f_kick123_mom_exp  - particle momentum reconstructed for kick123 track (MeV/c)
// f_kick_beta        - particle beta (v/c) for kick track
// f_kick_mass        - particle mass reconstructed for kick track (MeV/c^2)
// f_kick_mom_exp     - particle momentum reconstructed for kick track (MeV/c)
// f_kick_mom_pull    - kick track quality parameter (x_rec-x_calc)/sigma 
// f_mdc_chi2         - chi2 of the track fit in the innerMDC segment
// f_mdc_combined_dedx 	    - de/dx in the inner and outer MDC segments
// f_mdc_combined_dedxsigma - sigma of de/dx in the inner and outer MDC segments
// f_mdc_inner_dedx 	    - de/dx in the inner MDC segment
// f_mdc_outer_dedx 	    - de/dx in the outer MDC segment
// f_meta_local_x           - Meta x coordinate in the local Meta reference frame [mm]
// f_meta_local_y           - Meta y coordinate in the local Meta reference frame 
// f_meta_mdc_dx            - x coordinate difference of the Meta hit and original outer MDC track segment 
// f_meta_mdc_dy            - y coordinate difference of the Meta hit and original outer MDC track segment 
// f_meta_mdc_dz            - z coordinate difference of the Meta hit and original outer MDC track segment 
// f_meta_rk_dx             - x coordinate difference of the Meta hit and Runge Kutta outer MDC track segment 
// f_meta_rk_dy             - y coordinate difference of the Meta hit and Runge Kutta outer MDC track segment 
// f_meta_rk_dz             - z coordinate difference of the Meta hit and Runge Kutta outer MDC track segment 
// f_meta_x      - Meta x coordinate in the lab. reference frame [mm] 
// f_meta_x_err  - Meta x error in the lab. reference frame [mm]
// f_meta_y      - Meta y coordinate in the lab. reference frame 
// f_meta_y_err  - Meta y error in the lab. reference frame
// f_meta_z      - Meta z coordinate in the lab. reference frame 
// f_meta_z_err  - Meta z error in the lab. reference frame
// f_metamatch_quality - correlation quality of the outerMDC and Meta  
// f_mom_geant         - Geant particle momentum
// f_outer_mdc_chi2    - chi2 of the track fit in the outerMDC segment
// f_reft_beta         - beta (v/c) for reference trajectory track
// f_reft_mass         - particle mass for reference trajectory track (MeV/c^2)
// f_reft_mom_exp      - particle momentum for reference trajectory track (MeV/c)
// f_rich_avcharge     - average charge accumulated in the Rich ring
// f_rich_centroid     - centroid parameter of the Rich ring
// f_rich_phi          - azimuthal angle of the track reconstructed in the Rich [deg]
// f_rich_theta        - polar angle of the track reconstructed in the Rich [deg]
// f_rk_beta           - beta (v/c) for Runge Kutta track
// f_rk_mass           - particle mass for Runge Kutta track (MeV/c^2)
// f_rk_mom_exp        - particle momentum for Runge Kutta track (MeV/c)
// f_rk_phi            - azimuthal emission angle of the Runge Kutta track [deg]
// f_rk_quality        - quality of Runge Kutta track [a.u.]
// f_rk_theta          - polar emission angle of the Runge Kutta track [deg]
// f_shw_sum0          - integrated charge on 3x3 pads area of the first chamber of Pre-Shower [pC] 
// f_shw_sum1          - integrated charge on 3x3 pads area of the second chamber of Pre-Shower  
// f_shw_sum2          - integrated charge on 3x3 pads area of the third chamber of Pre-Shower  
// f_shw_tof           - particle time of flight in Pre-Shower/Tofino [ns]
// f_spline_beta       - beta (v/c) for Spline track
// f_spline_mass       - particle mass for Spline track (MeV/c^2)
// f_spline_mom_exp    - particle momentum for Spline track (MeV/c)
// f_spline_quality    - quality of Spline track [a.u.]
// f_tof_elos          - energy loss in TOF rod
// f_tof_leftamp       - signal amplitude on the left side of TOF rod
// f_tof_rightamp      - signal amplitude on the right side of TOF rod
// f_tof_tof           - particle time of flight in TOF [ns]
// f_track_phi         - azimutal emission angle from inner MDC segment [deg]
// f_track_r_kick      - minimum distance of the Kick track to beam axis [mm]
// f_track_r_rk        - minimum distance of the Runge Kutta  track to beam axis [mm]
// f_track_r_spline    - minimum distance of the Spline track to beam axis [mm]
// f_track_theta       - polar emission angle from inner MDC segment [deg]
// f_track_z_kick      - z coordinate of the projection of intersection point of Kick track and beam plain 
// f_track_z_rk        - z coordinate of the projection of intersection point of Spline track and beam plain 
// f_track_z_spline    - z coordinate of the projection of intersection point of Runge Kutta track and beam plain 
// f_vertex_zcoord     - z vertex coordinate
// i_geantcorr_flag    - detector Geant coorelation flag (look at "enumDetBits" in pid/piddef.h)
// i_geant_pid         - particle Geant pid (Geant convension) 
// i_tracktype         - type of the track (look at  getTrackType() in pidutil/htrackntuplemaker.cc)  
// n_charge_kick       - polarity of Kick track 
// n_charge_rk         - polarity of Runge Kutta track
// n_charge_spline     - polarity of Spline track
// n_lepton_candidates - number of lepton candidates (correlation Rich&innerMDC) 
// n_medium            - Geant medium in which particle was born  
// n_meta_cell         - TOF rod number
// n_process           - Geant particle production process
// n_rich_houtra       - amplitude of Rich Hough Trnsform pattern recognition algorithm 
// n_rich_locmax       - number of local maximum (from four neighbors) in the Rich 
// n_rich_npads        - number of fired pads per Rich ring
// n_rich_patmatrix    - amplitude of Rich Paterm Matrix pattern recognition algorithm 
// n_rk_ind            - indices of Runge Kutta objects
// n_sector            - number of sectors
// n_shw_col           - Pre-Shower pad column
// n_shw_cs0           - number of fired neighboring pads (cluster size) on the first chamber of Pre-Shower
// n_shw_cs1           - number of fired neighboring pads (cluster size) on the second chamber of Pre-Shower
// n_shw_cs2           - number of fired neighboring pads (cluster size) on the third chamber of Pre-Shower
// n_shw_row           - Pre-Shower pad row
// n_system            - Pre-Shower(0) or TOF(1) system
// n_tof_cell          - number of TOF module
// n_tof_clss          - number of fired neighboring TOF rods (cluster size)  
// n_tofino_mult       - number of hits per Tofino paddle
//

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

//-----------------------------------------------------------------------------
class HTrackNtupleMaker : public HSUDummyRec{
 public:
  HTrackNtupleMaker(TString pOutFileName="ntuple_out.root",TString event="real", TString storetracks="alltrackcandidates");
  ~HTrackNtupleMaker() {};

  // all Hydra reconstuructor classes must have this
  virtual Bool_t init(void);
  virtual Int_t  execute(void);
  virtual Bool_t finalize(void);
  virtual Bool_t reinit(void) { return kTRUE; }

  Bool_t hasATrack(void); // returns kTRUE if at least one tracking method was successfull

 private:
  void clearMembers();    // set all datamembers back to default values
  void clearArray();      // clear the data in the pattern list for the next instance of HPidTrackCand
 
  Bool_t bAnalyseSimData; // abalysis for simulation events
  Bool_t bLeptonTrackCandidates; // store only candidates for lepton tracks (is correlation with RICH) 

  HPidTrackCandSim  *pTrackCand;     // track candidate to be investigated
  HPidHitData 	    *pHitData;       // datector hits 
  HPidTrackData	    *pTrackData;     // reconstructed tracks
  HPidGeantTrackSet *pGeantTrackSet; // geant information

  TNtuple *pNtuple;  		     //output ntuple
  TString sOutFileName;              //ouput file name 
  
  //Ntuple and variable indices
  Float_t aPatternInstance[MAX_NTUPLE_SIZE];

  //Categories
  HCategory *pCatPidTrkCand;	// input category;
  HIterator *pItTrackCand;   	// iterator over HPidTrackCand objects
  HCategory *pCatKine; 		// GeantKine category

  // Check how many leptons contributed to this track before the field
  Float_t getTrackType(HPidTrackCandSim *pTrackCand);

  Bool_t bookRingObservables();    //Collect ring information into ntuple
  Bool_t bookTrackObservables();   //Collect tracking information into ntuple
  Bool_t bookSegmentObservables(); //Collect segment information into ntuple
  Bool_t bookCommonObservables();  //Collect common ....
  Bool_t bookVertexInformation();  //Collect vertex
  
  // Make sure that Geant Kine objects of the current event 
  // are all there and are sorted by track number
  void checkGeantTrackNumbers();
  // Was the current Event a downscaled one? 
  // Important because only downscaled and simulated events
  // can be compared
  Bool_t getDownscalingFlag();

  enum enumData{
    f_kick_mom_exp,f_spline_mom_exp,f_rk_mom_exp,f_kick123_mom_exp,f_reft_mom_exp,n_sector,f_mom_geant,
    f_track_theta,f_track_phi,f_deltatheta,f_deltaphi,n_process,b_ring,b_kicktrack,b_splinetrack,b_rktrack,
    b_kick123track,b_reftrack,n_close_segments,f_shw_sum0,f_shw_sum1,f_shw_sum2,n_shw_cs0,n_shw_cs1,n_shw_cs2,
    f_shw_tof,f_tof_tof,n_tofino_mult,i_tracktype,f_mdc_chi2,n_rich_patmatrix,n_rich_npads,f_rich_centroid,f_rich_avcharge,
    n_rich_locmax,i_geant_pid,n_system,f_metamatch_quality,f_kick_pull,f_spline_quality,
    f_track_r_spline,f_track_z_spline,n_medium,b_corrRingSpline,b_corrRingKick,f_vertex_zcoord,n_charge_kick,
    n_charge_spline,f_track_z_kick,f_track_r_kick,i_geant_corrflag,
    f_kick_beta,f_kick123_beta,f_reft_beta,f_spline_beta,
    f_rk_beta,b_isdownscaled,n_charge_rk,n_tof_clss,n_meta_cell,f_rk_quality,b_corrRingRK,
    f_track_r_rk,f_track_z_rk,f_kick_mass,f_kick123_mass,f_reft_mass,f_spline_mass,f_rk_mass,f_tof_leftamp,
    f_tof_rightamp,f_mdc_inner_dedx,f_mdc_outer_dedx,n_shw_row,n_shw_col,f_angle_to_closest_fitted_lep,
    f_angle_to_closest_fitted_had,f_angle_to_closest_nonfitted_lep,f_angle_to_closest_nonfitted_had,
    n_lepton_candidates,n_rich_houtra,f_distance_to_vertex_kick,f_distance_to_vertex_spline,
    f_distance_to_vertex_rk,f_meta_rk_dx,f_meta_rk_dy,f_meta_rk_dz,f_meta_x_err,f_meta_y_err,
    f_meta_z_err,f_meta_x,f_meta_y,f_meta_z,f_meta_mdc_dx,f_meta_mdc_dy,f_meta_mdc_dz,f_meta_local_x,
    f_meta_local_y,n_tof_module,n_tof_cell,f_tof_eloss,f_rk_theta,f_rk_phi,f_rich_phi,f_rich_theta,
    b_corrMetaKick,b_corrMetaSpline,b_corrMetaRK,f_outer_mdc_chi2,n_rk_ind,f_mdc_combined_dedx,f_mdc_combined_dedxsigma};

  

  
  
  ClassDef(HTrackNtupleMaker, 0)
    };
#endif /*!HTRACKNTUPLEMAKER_H*/
	  
