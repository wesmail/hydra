#include "hiterator.h"
#include "hcategory.h"
#include "hpidperformacetest.h"
#include "hpidtrackcand.h"
#include "hpidparticlesim.h"
#include "hpidgeanttrackset.h"
#include "hshowerhittof.h"
#include "htofhit.h"
#include "hpidcandidate.h"
#include  "piddef.h"
#include "hiterator.h"
#include "hcategory.h"
#include "TNtuple.h"
#include "hgeantkine.h"
#include "hkicktrack.h"
#include <iostream>

using namespace std;
ClassImp(HPidPerformaceTest)
    
// ---------------------------------------------------------------------------
HPidPerformaceTest::HPidPerformaceTest(const char*cOutFileName)
            :HSUDummyRec(kFALSE){
    clearMembers();
    sOutFileName = cOutFileName;
}

// ---------------------------------------------------------------------------
HPidPerformaceTest::~HPidPerformaceTest(){
    //delete pointer to ntuple
    if(NULL!=pNtuple){
        printf("Ntuple not null!!!\n");
  //      delete pNtuple;
    }
    
}
// ---------------------------------------------------------------------------

void HPidPerformaceTest::clearMembers(){
    pItTrackCand   = NULL;
    pItCand        = NULL;
    pItPart        = NULL;
    pTrackCand     = NULL;
    pHitData       = NULL;
    pTrackData     = NULL;
    pCand          = NULL;
    pGeantSet      = NULL;
    pPart          = NULL;
    pNtuple        = NULL;
    pShwHit        = NULL;
    pTof           = NULL;
    pKine          = NULL;
    m_bSimData     = kFALSE;
    for(Int_t n=0;n<MAX_NTUPLE_SIZE;n++){
        aNtupleVars[n]=-99;
    }
}

// ---------------------------------------------------------------------------
Bool_t HPidPerformaceTest::init(void){
    //Init categories and iterators

    pItPart = getIterator(catPidPart,kTRUE);      //get main iterator 
                                                      //and reports errors
    if( NULL==pItPart){
        return kFALSE;
    } 
   
    pItTrackCand = getIterator(catPidTrackCand,kTRUE);  
                                                      
    if( NULL==pItTrackCand){
        return kFALSE;
    } 
   
    pItCand = getIterator(catPidCandidate,kTRUE);      
                                                      
    if( NULL==pItCand){
        return kFALSE;
    } 
    //if geant kine exist then we have simulation data
    HCategory *pGeantCat=NULL;
    m_bSimData=kTRUE;
    /*if((pGeantCat = getCategory(catGeantKine)) != NULL || ) {
        m_bSimData=kTRUE;
    }
    */
    //set output file
    if(openOutFile(sOutFileName.Data()) == NULL){ 
        return kFALSE;
    }
    //Create NTuple
    char cBuffer[200000];
    snprintf(cBuffer,sizeof(cBuffer),"%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s:%s",
                PID_NTUPLE_VARS_1,PID_NTUPLE_VARS_2,PID_NTUPLE_VARS_3,
                PID_NTUPLE_VARS_4,PID_NTUPLE_VARS_5,PID_NTUPLE_VARS_6,
                PID_NTUPLE_VARS_7,PID_NTUPLE_VARS_8,PID_NTUPLE_VARS_9,
                PID_NTUPLE_VARS_10,PID_NTUPLE_VARS_11,PID_NTUPLE_VARS_12,
                PID_NTUPLE_VARS_13,PID_NTUPLE_VARS_14,PID_NTUPLE_VARS_15,
                PID_NTUPLE_VARS_16,PID_NTUPLE_VARS_17,PID_NTUPLE_VARS_18,
                PID_NTUPLE_VARS_19,PID_NTUPLE_VARS_20,PID_NTUPLE_VARS_21,
                PID_NTUPLE_VARS_22,PID_NTUPLE_VARS_23,PID_NTUPLE_VARS_24);
    printf("Ntuple =%s\n",cBuffer);
    if((pNtuple=new TNtuple("N","Ntuple with pdf data",cBuffer))==NULL){
        Error("init", "Cannot create NTuple");
        return kFALSE;        
    }
    //ALL was initialized OK
    setInitOk();
    return kTRUE;
}
// ---------------------------------------------------------------------------

Int_t HPidPerformaceTest::checkGeantTrackSet(HPidGeantTrackSet* pGTS)
{
  Int_t nCorrTracks=pGTS->getNCorrelatedTrackIds();
  for(Int_t i=0;i<nCorrTracks-1;i++)
    {
      if(pGTS->getCorrelationFlag(i)<pGTS->getCorrelationFlag(i+1))
	{
	  cout << "Warning: lower: " <<pGTS->getCorrelationFlag(i) << endl;
	  cout << "Warning: upper: " <<pGTS->getCorrelationFlag(i+1) << endl;
	  cout << "####################### " << endl;
	}
    }
  return 0;
}

Int_t HPidPerformaceTest::execute(void) {
  //reset iterators
  pItPart->Reset();
  while((pPart=(HPidParticle*)pItPart->Next())!=NULL){
    for(Int_t n=0;n<MAX_NTUPLE_SIZE;n++){
      aNtupleVars[n]=-99;
    }
    pTrackCand = pPart->getPidCandidate()->getTrackCandidate();;
    if(m_bSimData==kTRUE){
      pGeantSet = ((HPidParticleSim*)pPart)->getGeantTrackSet();
    }
    //cout << "Pointer to geant track set from pid particle: " << pGeantSet << endl;
    //pGeantSet->print();
    //cout << "Total momentum of most common particle: " <<pGeantSet->getTotalMomentum() << endl;
    pHitData = pTrackCand->getHitData();
    pCand = pPart->getPidCandidate();
    pTrackData = pTrackCand->getTrackData();
    //Fill commom variables
    aNtupleVars[p_rec_id]     = pPart->getPid();
    aNtupleVars[p_system]     = pPart->getSystem();
    aNtupleVars[p_charge]     = pPart->getCharge();
    aNtupleVars[p_rec_mom]    = pPart->P();
    aNtupleVars[p_beta]       = pPart->getBetaExp();
    aNtupleVars[p_test_value] = pPart->getTestVal();
    aNtupleVars[p_sector]     = pHitData->getSector();
    aNtupleVars[p_theta]      = pPart->thetaDeg();
    aNtupleVars[p_phi]        = pPart->phiDeg();
    aNtupleVars[p_tof]        = pHitData->getTof();
    aNtupleVars[p_tof_leftamp]        = pHitData->fTOFLeftAmplitude;
    aNtupleVars[p_tof_rightamp]        = pHitData->fTOFRightAmplitude;

    //Close pair information

    //aNtupleVars[p_iscpcandidate] = pTrackData->bIsClosePairCandidate;
    //aNtupleVars[p_opang_cpcandidate] = pTrackData->fAngleWithClosePairCandidate;

    aNtupleVars[p_angle_to_closest_lep] = pTrackData->fAngleWithClosestLeptonCandidate;
    aNtupleVars[p_angle_to_closest_had] = pTrackData->fAngleWithClosestHadronCandidate;

    aNtupleVars[p_closest_lep_isfitted] = pTrackData->closestLeptonCandidateIsFitted;
    aNtupleVars[p_closest_had_isfitted] = pTrackData->closestHadronCandidateIsFitted;

    aNtupleVars[p_rec_mom_spline]= pTrackData->fMomenta[ALG_SPLINE];
    aNtupleVars[p_spline_quality]= pTrackData->fSplineChiSquare;
    
    aNtupleVars[p_rec_mom_kick]= pTrackData->fMomenta[ALG_KICK];
    aNtupleVars[p_kick_pull]= pTrackData->fPull;
    
    aNtupleVars[p_rec_mom_rk]= pTrackData->fMomenta[ALG_RUNGEKUTTA];
    aNtupleVars[p_rk_quality]= pTrackData->fRKChiSquare;
    aNtupleVars[p_q_io_matching]= pTrackData->qIOMatching[ALG_SPLINE];
    if(m_bSimData==kTRUE){
      //Geant info

      aNtupleVars[p_geant_mom]  = pGeantSet->getTotalMomentum();
      aNtupleVars[p_geant_id] = ((HPidParticleSim*)pPart)->getGeantPID();
      for(int i=0;i<(int)pGeantSet->getNCorrelatedTrackIds();i++){
	aNtupleVars[p_n_corr_tracks] = pGeantSet->getNCorrelatedTrackIds();
	aNtupleVars[p_corr_flag_0+i] = pGeantSet->getCorrelationFlag(i);
	
	aNtupleVars[p_corr_trk_id_0+i] = pGeantSet->getGeantTrackID(i);
	aNtupleVars[p_corr_geant_id_0+i] = pGeantSet->getGeantPID(i);
	aNtupleVars[p_corr_process_id_0+i] = pGeantSet->getGeantProcessID(i);
	aNtupleVars[p_corr_parent_id_0+i] = pGeantSet->getGeantParentID(i);
	aNtupleVars[p_parent_0+i] =  pGeantSet->getGeantParentID(i); 
	aNtupleVars[p_medium_0+i] =  pGeantSet->getGeantMediumID(i); 
	aNtupleVars[p_geant_mom_0+i] =  pGeantSet->getTotalMomentum(i); 
      }
      aNtupleVars[p_geant_flag] = pGeantSet->getMostCommonCorrelation();
    }
    //mdc
    aNtupleVars[p_meta_quality] = pTrackData->fMetaMatchingQuality;
    aNtupleVars[p_dist_vert_spline] = pHitData->fDistanceToVertex[ALG_SPLINE];
    aNtupleVars[p_dist_vert_kick] = pHitData->fDistanceToVertex[ALG_KICK];
    aNtupleVars[p_dist_vert_rk] = pHitData->fDistanceToVertex[ALG_RUNGEKUTTA];
    aNtupleVars[p_inner_chi2] =  pHitData->fInnerMdcChiSquare;
    aNtupleVars[p_outer_chi2] =  pHitData->fOuterMdcChiSquare;
    aNtupleVars[p_mdc_inner_dedx] =  pHitData->fInnerMdcdEdx;
    aNtupleVars[p_mdc_outer_dedx] =  pHitData->fOuterMdcdEdx;
#warning These variables where never filled with meaningful data - replace by tracking method dependent observables soon
    //aNtupleVars[p_track_r] =  pHitData->fTrackR;
    //aNtupleVars[p_track_z] =  pHitData->fTrackZ;
    
    //Rich data
    
    aNtupleVars[p_rich_corr_spline] = pHitData->hasRingCorrelation[ALG_SPLINE];
    aNtupleVars[p_rich_corr_kick] = pHitData->hasRingCorrelation[ALG_KICK];
    aNtupleVars[p_rich_corr_rk] = pHitData->hasRingCorrelation[ALG_RUNGEKUTTA];
    aNtupleVars[p_rich_flag] = pHitData->getFlagRICH();
    aNtupleVars[p_ring_pad_nr] = pHitData->nRingPadNr;
    aNtupleVars[p_ring_centroind] = pHitData->fRingCentroid;
    aNtupleVars[p_rich_theta] = pHitData->fRichTheta;
    aNtupleVars[_p_rich_phi] = pHitData->fRichPhi;
    aNtupleVars[p_ring_pat_mat] = pHitData->nRingPatMat;
    aNtupleVars[p_ring_hou_tra] = pHitData->nRingHouTra;
    aNtupleVars[p_ring_ampli] = pHitData->nRingAmplitude;
    aNtupleVars[p_ring_local_max4] = pHitData->nRingLocalMax4;
    //Pre-Shower data
    aNtupleVars[p_shw_flag] = pHitData->getFlagSHOWER();
    for(int i=0;i<3;i++){
      aNtupleVars[p_shw_sum_0+i] = pHitData->fShowerSum[i];
      aNtupleVars[p_shw_cls_0+i] = pHitData->nShowerClS[i];
    }
    aNtupleVars[p_shw_row] = pHitData->nShowerRow;
    aNtupleVars[p_shw_col] = pHitData->nShowerCol;
    //aNtupleVars[p_shw_r] = pHitData->fShowerR;
    //aNtupleVars[p_shw_phi] = pHitData->fShowerPhi;
    //aNtupleVars[p_shw_theta] = pHitData->fShowerTheta;
    //Tof + Tofino data
    aNtupleVars[p_eloss_kick] = pTrackData->fCorrectedEloss[ALG_KICK];
    aNtupleVars[p_eloss_spline] = pTrackData->fCorrectedEloss[ALG_SPLINE];
    aNtupleVars[p_eloss_rk] = pTrackData->fCorrectedEloss[ALG_RUNGEKUTTA];
    aNtupleVars[p_tof_mult]  = pHitData->iTofinoMult;
    //distance to vertex:
    aNtupleVars[p_kick_vertex] = pHitData->fDistanceToVertex[ALG_KICK];
    aNtupleVars[p_spline_vertex] = pHitData->fDistanceToVertex[ALG_SPLINE];
    aNtupleVars[p_rk_vertex] = pHitData->fDistanceToVertex[ALG_RUNGEKUTTA];
    //Fill probabability information from various algs
    //Shower
    aNtupleVars[p_shw_fp] = pCand->getValueById(algShower,-2); 
    aNtupleVars[p_shw_fm] = pCand->getValueById(algShower,-1); 
    aNtupleVars[p_shw_ep] = pCand->getValueById(algShower,2); 
    aNtupleVars[p_shw_em] = pCand->getValueById(algShower,3); 
    aNtupleVars[p_shw_pp] = pCand->getValueById(algShower,8); 
    aNtupleVars[p_shw_pm] = pCand->getValueById(algShower,9); 
    aNtupleVars[p_shw_p]  = pCand->getValueById(algShower,14); 
    //MomVsBeta
    aNtupleVars[p_mvb_fp] = pCand->getValueById(algMomVsBeta,-2); 
    aNtupleVars[p_mvb_fm] = pCand->getValueById(algMomVsBeta,-1); 
    aNtupleVars[p_mvb_ep] = pCand->getValueById(algMomVsBeta,2); 
    aNtupleVars[p_mvb_em] = pCand->getValueById(algMomVsBeta,3); 
    aNtupleVars[p_mvb_pp] = pCand->getValueById(algMomVsBeta,8); 
    aNtupleVars[p_mvb_pm] = pCand->getValueById(algMomVsBeta,9); 
    aNtupleVars[p_mvb_p]  = pCand->getValueById(algMomVsBeta,14); 
    //Rel. intensities
    aNtupleVars[p_relint_fp] = pCand->getValueById(algRelInt,-2); 
    aNtupleVars[p_relint_fm] = pCand->getValueById(algRelInt,-1); 
    aNtupleVars[p_relint_ep] = pCand->getValueById(algRelInt,2); 
    aNtupleVars[p_relint_em] = pCand->getValueById(algRelInt,3); 
    aNtupleVars[p_relint_pp] = pCand->getValueById(algRelInt,8); 
    aNtupleVars[p_relint_pm] = pCand->getValueById(algRelInt,9); 
    aNtupleVars[p_relint_p]  = pCand->getValueById(algRelInt,14); 
    //Rich
    aNtupleVars[p_rich_fp] = pCand->getValueById(algRich,-2); 
    aNtupleVars[p_rich_fm] = pCand->getValueById(algRich,-1); 
    aNtupleVars[p_rich_ep] = pCand->getValueById(algRich,2); 
    aNtupleVars[p_rich_em] = pCand->getValueById(algRich,3); 
    aNtupleVars[p_rich_pp] = pCand->getValueById(algRich,8); 
    aNtupleVars[p_rich_pm] = pCand->getValueById(algRich,9); 
    aNtupleVars[p_rich_p]  = pCand->getValueById(algRich,14); 
    //Bayes decision
    aNtupleVars[p_bayes_fp] = pCand->getBayesValue(-2,NULL,0); 
    aNtupleVars[p_bayes_fm] = pCand->getBayesValue(-1,NULL,0); 
    aNtupleVars[p_bayes_ep] = pCand->getBayesValue(2,NULL,0); 
    aNtupleVars[p_bayes_em] = pCand->getBayesValue(3,NULL,0); 
    aNtupleVars[p_bayes_pp] = pCand->getBayesValue(8,NULL,0); 
    aNtupleVars[p_bayes_pm] = pCand->getBayesValue(9,NULL,0); 
    aNtupleVars[p_bayes_p]  = pCand->getBayesValue(14,NULL,0); 
    //Merged values
    aNtupleVars[p_merged_fp] = pCand->getMergedPDFValue(-2,NULL,0); 
    aNtupleVars[p_merged_fm] = pCand->getMergedPDFValue(-1,NULL,0); 
    aNtupleVars[p_merged_ep] = pCand->getMergedPDFValue(2,NULL,0); 
    aNtupleVars[p_merged_em] = pCand->getMergedPDFValue(3,NULL,0); 
    aNtupleVars[p_merged_pp] = pCand->getMergedPDFValue(8,NULL,0); 
    aNtupleVars[p_merged_pm] = pCand->getMergedPDFValue(9,NULL,0); 
    aNtupleVars[p_merged_p]  = pCand->getMergedPDFValue(14,NULL,0); 
    
    //Trigger decision
    aNtupleVars[p_trigger_ds_flag] = 
      gHades->getCurrentEvent()->getHeader()->getDownscalingFlag();
    aNtupleVars[p_trigger_decision] = 
      gHades->getCurrentEvent()->getHeader()->getTriggerDecision();
    //quality of spline 
    aNtupleVars[p_qspline]=pTrackData->fSplineChiSquare;
    /*
      if(gHades->getCurrentEvent()->getHeader()->getDownscalingFlag()==1){
      aNtupleVars[p_trigger_lvl] = 1; //LVL1
      }
      else if (gHades->getCurrentEvent()->getHeader()
      ->getDownscalingFlag()==0 &&
      gHades->getCurrentEvent() ->getHeader()
      ->getTriggerDecision()>0){
      aNtupleVars[p_trigger_lvl] = 2; //LVL2
      }*/
    //Fill ntuple
    pNtuple->Fill(aNtupleVars);
  }
  return 0;
}
// ---------------------------------------------------------------------------

Bool_t HPidPerformaceTest::finalize(void) {
        //write ntuple to file
        return writeAndCloseOutFile();
}
