using namespace std;
#include "hpidpdfmaker.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "richdef.h"
#include "hiterator.h"
#include "hcategory.h"
//#include "hpidtrackcand.h"
#include "piddef.h"
#include "hiterator.h"
#include "hcategory.h"
#include "TNtuple.h"
#include "TString.h"
#include "hgeantkine.h"
#include "hshowerhittof.h"
#include "hkicktracksim.h"
#include "hkicktrack.h"
#include "kickdef.h"
#include "hrichhitsim.h"
#include "hpidfl.h"
#include "hcategory.h"
#include "hmdcsegsim.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hrichgeometrypar.h"
#include "hruntimedb.h"
#include "hpidhitdata.h"
#include "hpidtrackdata.h"
#include "hpidtrackcandsim.h"
#include <iostream>
ClassImp(HPidPdfMaker)
  
  // ---------------------------------------------------------------------------
  HPidPdfMaker::HPidPdfMaker(const Char_t*cOutFileName, Bool_t analyseSim) //OK
    :HSUDummyRec(kFALSE){
  analyseSimData=analyseSim;
  clearMembers();
  sOutFileName = cOutFileName;
}

// ---------------------------------------------------------------------------
HPidPdfMaker::~HPidPdfMaker(){ //OK
  
}
// ---------------------------------------------------------------------------
void HPidPdfMaker::clearArray() //OK
{
  for(Int_t n=0;n<MAX_NTUPLE_SIZE;n++){
    aPatternInstance[n]=-99;
    //aAuxPatternInstance[n]=-99;
  }
}
// ---------------------------------------------------------------------------

void HPidPdfMaker::clearMembers(){ //OK
  pRichGeometryPar = NULL;
  pHitData         = NULL;
  pTrackData       = NULL;
  pGeantTrackSet   = NULL;

  pItTrackCand     = NULL;
  pTrackCand       = NULL;
  pShowerHitTof    = NULL;
  pKickTrack       = NULL;
  pRichHit         = NULL;
  pNtuple          = NULL;
  //auxpNtuple       = NULL;

  pCatPidTrkCand   = NULL;
  pCatKine         = NULL;
  pCatMdcSeg       = NULL;
  pCatMdcHit       = NULL;
  clearArray();
}

// ---------------------------------------------------------------------------
Bool_t HPidPdfMaker::init(void){ //OK
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  if(!rtdb)
    {
      Error("init","No Runtime Database found!");
      return kFALSE;
    }
  pRichGeometryPar = (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
  HEvent *pEvent= gHades->getCurrentEvent();

  //Init categories and iterators
  pCatPidTrkCand = pEvent->getCategory(catPidTrackCand);
  if(NULL==pCatPidTrkCand){
    cout <<"No trk cand!" << endl;
    return kFALSE;
  }
  
  if((pItTrackCand = (HIterator *)pCatPidTrkCand->MakeIterator()) == NULL)
    {
        Error("init", "Cannot make catPidTrackCand category iterator");
        return kFALSE;
    }


  //and report errors
  if( NULL==pItTrackCand){
    cout <<"No iterator" << endl;
    return kFALSE;
  } 
  if(analyseSimData)
    {
      pCatKine = getCategory(catGeantKine,kTRUE);
      if( NULL==pCatKine){
	cout <<"No kine" << endl;
	return kFALSE;
      }
    } 
  /*
  pCatMdcSeg = getCategory(catMdcSeg,kTRUE);
  if( NULL==pCatMdcSeg){
    cout <<"No mdcseg" << endl;
    return kFALSE;
  } 
  
  pCatMdcHit = getCategory(catMdcHit,kTRUE);
  if( NULL==pCatMdcHit){
    cout <<"No mdchit" << endl;
    return kFALSE;
  } 
  */
  //set output file
  if(openOutFile(sOutFileName.Data()) == NULL){ 
    return kFALSE;
  }

  //Create big NTuple
  TString ntuple_str(NTUPLE_VARS1);
  ntuple_str.Append(NTUPLE_VARS2);
  ntuple_str.Append(NTUPLE_VARS3);
  ntuple_str.Append(NTUPLE_VARS4);
  printf("Ntuple %s\n",ntuple_str.Data());
  if((pNtuple=new TNtuple("N","Ntuple with pdf data",ntuple_str))==NULL){
    Error("init", "Cannot create NTuple");
    return kFALSE;        
  }

  //Create small NTuple
  /*
  TString aux_ntuple_str(AUX_NTUPLE_VARS);
  printf("Auxiliary Ntuple %s\n",aux_ntuple_str.Data());
  if((auxpNtuple=new TNtuple("AUXN","Auxiliary Ntuple with pdf data for normalization",aux_ntuple_str))==NULL){
    Error("init", "Cannot create NTuple");
    return kFALSE;        
  }
  */

  //ALL was initialized OK
  setInitOk();
  cout << "Init of pdfmaker OK! "<< endl;
  return kTRUE;
}


Int_t HPidPdfMaker::execute(void) {

  if(analyseSimData)
    checkGeantTrackNumbers();
  Bool_t isDSEvent=getDownscalingFlag();
  //reset iterators

  pItTrackCand->Reset();
  Int_t leptonCandidateCounter=0;
  
  //Loop over all candidates in this event and count the lepton candidates
  //In this way we collect "per-event-information"

  Int_t nrks=0;
  Int_t nkicks=0;
  Int_t npidtrackcands=0;
  Int_t nrks_full=0;

  while((pTrackCand=(HPidTrackCandSim *)pItTrackCand->Next())!=NULL)
    {
      pHitData = pTrackCand->getHitData();
      for(Int_t i=0;i<MAX_MOM_ALGS;i++)
	{
	  if(pHitData->getRingCorrelation(i))
	    {
	      leptonCandidateCounter++;
	      break;
	    }
	}
    }
  //cout << "leptoncandidates: " << leptonCandidateCounter << endl;

  //Reset the iterator and fill the "per-instance-information"
  pItTrackCand->Reset();
  while((pTrackCand=(HPidTrackCandSim *)pItTrackCand->Next())!=NULL)
    {
      npidtrackcands++;
      clearArray();

      aPatternInstance[b_isdownscaled]=isDSEvent;
      aPatternInstance[n_lepton_candidates]=leptonCandidateCounter;
      //cout << "Index: " << n_lepton_candidates << endl;
      //cout << "Setting target value in ntuple: " << aPatternInstance[n_lepton_candidates] << endl;
      pHitData = pTrackCand->getHitData();
      pTrackData = pTrackCand->getTrackData();
      if(analyseSimData)
	{
	  pGeantTrackSet = pTrackCand->getGeantTrackSet();
	  
	  if(pHitData->getFlagRICH())
	    {
	      pRichHit=HPidFL::getRichHitSim(pHitData->getIndRICH());
	    }
	}
      
      
      //Check for ring
      aPatternInstance[b_ring]=kFALSE;
      aPatternInstance[b_corrRingSpline]=kFALSE;
      //aAuxPatternInstance[aux_b_corrRingSpline]=kFALSE;
      aPatternInstance[b_corrRingKick]=kFALSE;
      //aAuxPatternInstance[aux_b_corrRingKick]=kFALSE;
      aPatternInstance[b_corrRingRK]=kFALSE;
      
      if(pHitData->getFlagRICH()){
	
	aPatternInstance[b_ring]=kTRUE; 
	if(analyseSimData)
	  {
	    aPatternInstance[i_ringtype]=getRingType(pRichHit);
	  }
	
	if(pHitData->getRingCorrelation(ALG_SPLINE)) 
	  {
	    aPatternInstance[b_corrRingSpline]=kTRUE;
	    //aAuxPatternInstance[aux_b_corrRingSpline]=kTRUE;
	  }
	else
	  {
	    aPatternInstance[b_corrRingSpline]=kFALSE;
	    //aAuxPatternInstance[aux_b_corrRingSpline]=kFALSE;
	  }
	
	if(pHitData->getRingCorrelation(ALG_KICK)) 
	  {
	    aPatternInstance[b_corrRingKick]=kTRUE;
	    //aAuxPatternInstance[aux_b_corrRingKick]=kTRUE;
	  }
	else
	  {
	    aPatternInstance[b_corrRingKick]=kFALSE;
	    //aAuxPatternInstance[aux_b_corrRingKick]=kFALSE;
	  }
	
	if(pHitData->getRingCorrelation(ALG_RUNGEKUTTA))
	  {
	    aPatternInstance[b_corrRingRK]=kTRUE;
	    ////aAuxPatternInstance[aux_b_corrRingRK]=kTRUE;
	  }
	else
	  {
	    aPatternInstance[b_corrRingRK]=kFALSE;
	    //aAuxPatternInstance[aux_b_corrRingRK]=kFALSE;
	  }
	
	bookRingObservables();
      }

      
      //check for available track types
      aPatternInstance[b_kicktrack]= (pTrackData->getKickTrackInd()>=0 && pTrackData->getIsAccepted(ALG_KICK))? kTRUE : kFALSE;
      
      aPatternInstance[b_splinetrack]= (pTrackData->getSplineTrackInd()>=0 && pTrackData->getIsAccepted(ALG_SPLINE))? kTRUE : kFALSE;
      
      aPatternInstance[b_rktrack]= (pTrackData->getRKTrackInd()>=0  && pTrackData->getIsAccepted(ALG_RUNGEKUTTA))? kTRUE : kFALSE;
      aPatternInstance[n_rk_ind]=pTrackData->getRKTrackInd();

      aPatternInstance[b_reftrack]= (pTrackData->getRefTrackInd()>=0)? kTRUE : kFALSE;
      
      aPatternInstance[b_kick123track]= (pTrackData->getKickTrack123Ind()>=0)? kTRUE : kFALSE;

      if(pTrackData->getKickTrackInd()>=0) nkicks++;
      if(aPatternInstance[b_rktrack]==1) 
	{
	  nrks++;
	  if(pHitData->getMetaTrackCorrelation(ALG_RUNGEKUTTA))
	    {
	      nrks_full++;
	    }
	}
      
      if(analyseSimData){
	aPatternInstance[i_tracktype]=getTrackType(pTrackCand);
      }
      bookTrackObservables();
      bookVertexInformation();
      
      //check for segments
      aPatternInstance[b_segments]=kFALSE;
      if(pTrackData->getNCloseTracklets()>0){
	aPatternInstance[b_segments]= pTrackData->getNCloseTracklets();
      }
      
      aPatternInstance[b_corrMetaRK]=kFALSE;
      aPatternInstance[b_corrMetaSpline]=kFALSE;
      aPatternInstance[b_corrMetaKick]=kFALSE;
      
      if(pHitData->iIndShower>=0 || pHitData->iIndTOF>=0)
	{
	  if(pHitData->getMetaTrackCorrelation(ALG_KICK))
	    {
	      aPatternInstance[b_corrMetaKick]=kTRUE;
	    }
	  if(pHitData->getMetaTrackCorrelation(ALG_SPLINE))
	    {
	      aPatternInstance[b_corrMetaSpline]=kTRUE;
	    }
	  if(pHitData->getMetaTrackCorrelation(ALG_RUNGEKUTTA))
	    {
	      aPatternInstance[b_corrMetaRK]=kTRUE;
	    }


	}
      
      //Fill shower/tof variables
      if(pHitData->iIndShower>=0){
	aPatternInstance[f_shw_sum0] = pHitData->getShowerSum(0);
	aPatternInstance[f_shw_sum1] = pHitData->getShowerSum(1);
	aPatternInstance[f_shw_sum2] = pHitData->getShowerSum(2);
	aPatternInstance[n_shw_cs0]  = pHitData->getShowerClS(0);
	aPatternInstance[n_shw_cs1]  = pHitData->getShowerClS(1);
	aPatternInstance[n_shw_cs2]  = pHitData->getShowerClS(2);
	aPatternInstance[n_shw_row]  = pHitData->getShowerRow();
	aPatternInstance[n_shw_col]  = pHitData->getShowerCol();
	aPatternInstance[f_shw_tof]  = pHitData->getShowerTimeOfFlight();
	aPatternInstance[n_shw_mult] = pHitData->getTofinoMult();
	aPatternInstance[f_meta_local_x] = pHitData->getMetaLocalX();
	aPatternInstance[f_meta_local_y] = pHitData->getMetaLocalY();
      }
      if(pHitData->iIndTOF>=0)
	{
	  aPatternInstance[f_tof_tof]  = pHitData->getTOFTimeOfFlight();
	  aPatternInstance[f_tof_leftamp]  = pHitData->getTOFLeftAmplitude();
	  aPatternInstance[f_tof_rightamp]  = pHitData->getTOFRightAmplitude();
	  aPatternInstance[f_tof_eloss]  = pHitData->getTofEloss();
	  aPatternInstance[n_tof_clss]  = pHitData->getTofClsSize();
	  aPatternInstance[n_meta_cell]  = pHitData->getMetaCell();
	  aPatternInstance[n_tof_module] = pHitData->getTofModule();
	  aPatternInstance[n_tof_cell] = pHitData->getTofCell();
	  aPatternInstance[f_meta_local_x] = pHitData->getMetaLocalX();
	}
      aPatternInstance[n_tofino_mult] = pHitData->getTofinoMult();	
      bookCommonObservables();
      
      
      //Fill ntuple
      pNtuple->Fill(aPatternInstance);
      //auxpNtuple->Fill(aAuxPatternInstance);
    }
  return 0;
  
}

//Returns kTRUE if at least one tracking method was successful
Bool_t HPidPdfMaker::hasATrack() //OK
{
  return (aPatternInstance[b_kicktrack]>0 || aPatternInstance[b_splinetrack]>0 || aPatternInstance[b_rktrack]>0 ||aPatternInstance[b_reftrack]>0 || aPatternInstance[b_kick123track]>0);
}

// ---------------------------------------------------------------------------

Bool_t HPidPdfMaker::finalize(void) { //OK
   
  //write ntuple to file

  return writeAndCloseOutFile();

}

Bool_t HPidPdfMaker::getDownscalingFlag() //OK
{
  if(analyseSimData) //All simulation data are first level data - so they represent a downscaled sample
    {
      return kTRUE;
    }
  if(gHades->getCurrentEvent()->getHeader()->getDownscalingFlag())
    {
      return kTRUE;
    }
  return kFALSE;
}


Bool_t HPidPdfMaker::bookCommonObservables() //OK
{  

  /*Here we determine the distances of the close by segments (fitted or not , with or without ring)*/

  aPatternInstance[f_angle_to_closest_fitted_lep]=pTrackData->getAngleWithClosestCandidate(1,1);
  aPatternInstance[f_angle_to_closest_fitted_had]=pTrackData->getAngleWithClosestCandidate(-1,1);

  aPatternInstance[f_angle_to_closest_nonfitted_lep]=pTrackData->getAngleWithClosestCandidate(1,-1);
  aPatternInstance[f_angle_to_closest_nonfitted_had]=pTrackData->getAngleWithClosestCandidate(-1,-1);


  /*OBSOLETE
  //aPatternInstance[b_closest_lep_isfitted]=pTrackData->closestLeptonCandidateIsFitted();
  //aPatternInstance[b_closest_had_isfitted]=pTrackData->closestHadronCandidateIsFitted();

  //aPatternInstance[f_angle_with_cp_candidate]=pTrackData->fAngleWithClosePairCandidate;
  */

  aPatternInstance[n_charge_rk]=pTrackData->getPolarity(ALG_RUNGEKUTTA);
  aPatternInstance[n_charge_kick]=pTrackData->getPolarity(ALG_KICK);
  aPatternInstance[n_charge_spline]=pTrackData->getPolarity(ALG_SPLINE);
  aPatternInstance[n_sector]=(Float_t)(pHitData->getSector());
  //aAuxPatternInstance[aux_n_sector]=(Float_t)(pHitData->getSector());
  if(pHitData->nSector>5 || pHitData->nSector<0)
    cout <<"NOPE! This sector does not exist!" << pHitData->getSector() << endl;
  aPatternInstance[n_system]=(Float_t)(pHitData->getSystem());

  if(aPatternInstance[b_ring] && hasATrack())
    {
      aPatternInstance[f_deltatheta]=pHitData->getDeltaThetaMDCRICH();
      //aAuxPatternInstance[aux_f_deltatheta]=pHitData->getDeltaThetaMDCRICH();

      aPatternInstance[f_deltaphi]=pHitData->getDeltaPhiMDCRICH();
      //aAuxPatternInstance[aux_f_deltaphi]=pHitData->getDeltaPhiMDCRICH();
    }

  return kTRUE;
}

Bool_t HPidPdfMaker::bookVertexInformation() //OK
{
  if(!(pHitData && pTrackData))
    {
      ::Error("HPidPdfMaker::bookVertexInfo","No hit/track data found!");
      return kFALSE;
    }
  

  if(pTrackData->getIsAccepted(ALG_SPLINE)){
    aPatternInstance[f_track_r_spline]=pTrackData->getTrackR(ALG_SPLINE);
    aPatternInstance[f_track_z_spline]=pTrackData->getTrackZ(ALG_SPLINE);
    aPatternInstance[f_distance_to_vertex_spline]=pHitData->getDistanceToVertex(ALG_SPLINE);
  }

  if(pTrackData->getIsAccepted(ALG_KICK)){
    aPatternInstance[f_track_r_kick]=pTrackData->getTrackR(ALG_KICK);
    aPatternInstance[f_track_z_kick]=pTrackData->getTrackZ(ALG_KICK);
    aPatternInstance[f_distance_to_vertex_kick]=pHitData->getDistanceToVertex(ALG_KICK);
  }

  if(pTrackData->getIsAccepted(ALG_RUNGEKUTTA)){
    aPatternInstance[f_track_r_rk]=pTrackData->getTrackR(ALG_RUNGEKUTTA);
    aPatternInstance[f_track_z_rk]=pTrackData->getTrackZ(ALG_RUNGEKUTTA);
    aPatternInstance[f_distance_to_vertex_rk]=pHitData->getDistanceToVertex(ALG_RUNGEKUTTA);
  }
  aPatternInstance[f_vertex_zcoord]=gHades->getCurrentEvent()->getHeader()->getVertexZ();
  return kTRUE;
}
  
  

Bool_t HPidPdfMaker::bookRingObservables() //OK
{
  aPatternInstance[n_rich_patmatrix]=(float)pHitData->getRingPatMat();
  aPatternInstance[n_rich_npads]=(float)pHitData->getRingPadNr();
  aPatternInstance[f_rich_centroid]=(float)pHitData->getRingCentroid();
  aPatternInstance[f_rich_avcharge]=(float)pHitData->getRingAmplitude()/(float)pHitData->getRingPadNr();
  aPatternInstance[n_rich_locmax]=(float)pHitData->getRingLocalMax4();
  aPatternInstance[n_rich_houtra]=(float)pHitData->getRingHouTra();
  aPatternInstance[f_rich_theta]=(float)pHitData->getRichTheta();
  aPatternInstance[f_rich_phi]=(float)pHitData->getRichPhi();
  return kTRUE;
}
  

Bool_t HPidPdfMaker::bookTrackObservables() //OK
{
  if(aPatternInstance[i_geant_pid]>0 && analyseSimData)
    {
      aPatternInstance[f_mom_geant]=pGeantTrackSet->getTotalMomentum(0);
      //If the most common particle was NOT a fake - get its momentum
    }
  if(analyseSimData){
    aPatternInstance[n_process]=pGeantTrackSet->getGeantProcessID(0);
    aPatternInstance[n_medium]=pGeantTrackSet->getGeantMediumID(0);
  }
  aPatternInstance[f_metamatch_quality]=pTrackData->getMetaMatchingQuality();

  aPatternInstance[f_track_theta]=pHitData->getMdcTheta();
  aPatternInstance[f_rk_theta]=pTrackData->getRKTheta();

  //aAuxPatternInstance[aux_f_track_theta]=pHitData->getMdcTheta();

  aPatternInstance[f_track_phi]=pHitData->getMdcPhi();
  aPatternInstance[f_rk_phi]=pTrackData->getRKPhi();

  aPatternInstance[f_kick_mom_exp]=pTrackData->getMomenta(ALG_KICK);
  aPatternInstance[f_kick_beta]=pTrackData->getBeta(ALG_KICK);
  //aAuxPatternInstance[aux_f_kick_mom_exp]=pTrackData->getMomenta(ALG_KICK);

  aPatternInstance[f_spline_mom_exp]=pTrackData->getMomenta(ALG_SPLINE);
  aPatternInstance[f_spline_beta]=pTrackData->getBeta(ALG_SPLINE);
  //aAuxPatternInstance[aux_f_spline_mom_exp]=pTrackData->getMomenta(ALG_SPLINE);

  aPatternInstance[f_kick123_mom_exp]=pTrackData->getMomenta(ALG_KICK123);
  aPatternInstance[f_kick123_beta]=pTrackData->getBeta(ALG_KICK123);

  aPatternInstance[f_reft_mom_exp]=pTrackData->getMomenta(ALG_REFT);
  aPatternInstance[f_reft_beta]=pTrackData->getBeta(ALG_REFT);

  aPatternInstance[f_rk_mom_exp]=pTrackData->getMomenta(ALG_RUNGEKUTTA);
  aPatternInstance[f_rk_beta]=pTrackData->getBeta(ALG_RUNGEKUTTA);

  aPatternInstance[f_kick_pull]=pTrackData->getPull();
  aPatternInstance[f_spline_quality]=pTrackData->getSplineChiSquare();
  aPatternInstance[f_rk_quality]=pTrackData->getRKChiSquare();

  aPatternInstance[f_meta_rk_dx]=pTrackData->getRkMetadx();
  aPatternInstance[f_meta_rk_dy]=pTrackData->getRkMetady();
  aPatternInstance[f_meta_rk_dz]=pTrackData->getRkMetadz();
  aPatternInstance[f_meta_x]=pTrackData->getxMeta();
  aPatternInstance[f_meta_y]=pTrackData->getyMeta();
  aPatternInstance[f_meta_z]=pTrackData->getzMeta();
  aPatternInstance[f_meta_x_err]=pTrackData->geterrXMeta();
  aPatternInstance[f_meta_y_err]=pTrackData->geterrYMeta();
  aPatternInstance[f_meta_z_err]=pTrackData->geterrZMeta();
  aPatternInstance[f_meta_mdc_dx]=pTrackData->getMdcMetadx();
  aPatternInstance[f_meta_mdc_dy]=pTrackData->getMdcMetady();

  aPatternInstance[f_kick_mass]=pTrackData->getMass2(ALG_KICK);
  aPatternInstance[f_spline_mass]=pTrackData->getMass2(ALG_SPLINE);
  aPatternInstance[f_kick123_mass]=pTrackData->getMass2(ALG_KICK123);
  aPatternInstance[f_reft_mass]=pTrackData->getMass2(ALG_REFT);
  aPatternInstance[f_rk_mass]=pTrackData->getMass2(ALG_RUNGEKUTTA);
  bookSegmentObservables();

  return kTRUE;
}
  

Bool_t HPidPdfMaker::bookSegmentObservables() //OK
{

  if(pHitData->iIndInnerSeg >= 0) {  //!!!!!!!!!!!!!!!!!!
    aPatternInstance[n_mdc_nw0]  = pHitData->getMdcNWires(0);  
    aPatternInstance[n_mdc_cls0] = pHitData->getMdcClsSize(0); 
    aPatternInstance[n_mdc_cfl0] = pHitData->getMdcLevelCls(0);
    aPatternInstance[n_mdc_nw1]  = pHitData->getMdcNWires(1);  
    aPatternInstance[n_mdc_cls1] = pHitData->getMdcClsSize(1); 
    aPatternInstance[n_mdc_cfl1] = pHitData->getMdcLevelCls(1);
  
    aPatternInstance[f_mdc_inner_dedx] = pHitData->getInnerMdcdEdx();
  }
  
  
  if(pHitData->iIndOuterSeg >= 0) {  //!!!!!!!!!!!!!!!!!!
    aPatternInstance[f_mdc_outer_dedx] = pHitData->getOuterMdcdEdx();
  }

  aPatternInstance[f_mdc_chi2] = pHitData->getInnerMdcChiSquare();
  aPatternInstance[f_outer_mdc_chi2] = pHitData->getOuterMdcChiSquare();

  aPatternInstance[f_mdc_combined_dedx]=pHitData->getCombinedMdcdEdx();
  aPatternInstance[f_mdc_combined_dedxsigma]=pHitData->getCombinedMdcdEdxSigma();
  return kTRUE;
}

//Check presence and sorting of objects in Geant Kine category
void HPidPdfMaker::checkGeantTrackNumbers()  //OK
{
  HGeantKine *pKine;
  Int_t iTrackNumber,iPID, wait;
  Int_t i=0;
  Int_t nKineObjects=0;
  Int_t iMax = pCatKine->getEntries();
  //cout << "Kine has: " << iMax << "entries" << endl;
  Int_t lastTrackID=0;
  while((pKine = (HGeantKine *) pCatKine->getObject(i))!=NULL)
    {
      nKineObjects++;
      pKine->getParticle(iTrackNumber,iPID);
      if(lastTrackID+1!=iTrackNumber)
	{
	  cout << "Warning! Tracknumbers are not consecutive!" << endl;
	  cout << "Object number: " << i << "has address: " << pKine;
	  cout <<" PID is: " <<iPID << endl;
	  cout <<" TrackID is: " <<iTrackNumber << endl;
	  cin >> wait;
	}
      lastTrackID++;
      i++;
    }
  if(nKineObjects!=iMax) cout << "Mismatch between number of announced and found objects!" << endl;
}

Float_t HPidPdfMaker::getTrackType(HPidTrackCandSim* pTrackCand) //OK
{
  // This function determines what kind of track we are dealing with
  // We always must ask whether a TRACK is from a single lepton, from two leptons, 
  // from a hadron or a fake. 
  
  // Principal decisions taken upon each track:
  // a) does the track contain at least ONE matching geant ID in all hits in MDC-segment and META ?
  // -----> If this is wrong, set the tracktype to -1 and continue with ringtype determination
  // -----> If this is true ask
  //         b) is the matching geant ID leptonic or hadronic?
  //         -----> If it is hadronic, set the tracktype to 0 and continue with ringtype determination
  //         -----> If it is leptonic, ask
  //                c) does the track contain at least 2 leptonic geant IDs in the inner segment ?
  //                -----> If this is true, set the tracktype to 2
  //                -----> If this is wrong, set the tracktype to 1



  
  Int_t nMatchesMdcMeta, nMatchesFull, nLeptonicMatchesMdcMeta, GeantPid;
  //cout << "Counting leptons now" << endl;
  Int_t nLeptonsInInnerMdc = HPidFL::countLeptonsInInnerMdc(pGeantTrackSet);

  HPidFL::getGeantMatches(pTrackCand,nMatchesMdcMeta, nMatchesFull, nLeptonicMatchesMdcMeta, GeantPid);
  if(GeantPid!=-99)
    {
      aPatternInstance[i_geant_pid]=GeantPid;  
      //aAuxPatternInstance[aux_i_geant_pid]=GeantPid;  
    }
  
  if(!nLeptonsInInnerMdc &&(GeantPid==2 ||GeantPid==3))
    {
      cout << "Inconsistent result! There was an electron without leptonic matches in mdc and meta!" << endl; 
      HPidFL::countLeptonsInInnerMdc(pGeantTrackSet, kTRUE);
    }
  aPatternInstance[i_geant_corrflag]=pTrackCand->getGeantTrackSet()->getMostCommonCorrelation();
  //aAuxPatternInstance[aux_i_geant_corrflag]=pTrackCand->getGeantTrackSet()->getMostCommonCorrelation();
  if(nLeptonicMatchesMdcMeta)             //This track has a matched lepton in MDC and META!
    {
      return (Float_t)nLeptonsInInnerMdc;   //The tracktype is determined by the number of leptons in the segment;
    }
  if(nMatchesMdcMeta) //No common leptons in MDC and META, but common track IDs 
    {                      //-> this was NOT a fake but some hadron!
      return 0.;
    }
  return -1.; // No common leptons, no common track IDs at all - this was a fake
   
}
  
//Count the leptons contributing to this ring
Float_t HPidPdfMaker::getRingType(HRichHitSim* pRichHit) //OK
{
  Int_t nMirrTracks1, nMirrTracks2;

  //Check whether this ring is of leptonic origin
  if (HPidFL::isDoubleLepOnMirror(pRichHit, nMirrTracks1, nMirrTracks2))
    {
      return 2.0;  //Light from two different leptons was responsible for firing the pads on the ring
    }
  else //It was not a Double_t - was it a single ?
    {
      if (HPidFL::isLepOnMirror(pRichHit, nMirrTracks1))
	{
	  return 1.0; //Light from only one lepton made this ring
	}
      else {
	return 0.0; //There where no pads fired by a photon!
      } //It was obviously a fake
    }
}


// -----------------------------------------------------------------------------

