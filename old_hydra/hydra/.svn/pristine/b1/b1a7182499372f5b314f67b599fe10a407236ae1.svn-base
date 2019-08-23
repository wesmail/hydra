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
#include "hrichhitsim.h"
#include "hpidfl.h"
#include "hcategory.h"
#include "hmdcsegsim.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hmdcclusinf.h"
#include "hrichgeometrypar.h"
#include "hruntimedb.h"
#include "hpidhitdata.h"
#include "hpidtrackdata.h"
#include "hpidtrackcandsim.h"
ClassImp(HPidPdfMaker)
  
  // ---------------------------------------------------------------------------
  HPidPdfMaker::HPidPdfMaker(const char*cOutFileName)
    :HSUDummyRec(kFALSE){
  clearMembers();
  sOutFileName = cOutFileName;
}

// ---------------------------------------------------------------------------
HPidPdfMaker::~HPidPdfMaker(){
  
}
// ---------------------------------------------------------------------------
void HPidPdfMaker::clearArray()
{
  for(Int_t n=0;n<MAX_NTUPLE_SIZE;n++){
    aPatternInstance[n]=-99;
  }
}
// ---------------------------------------------------------------------------

void HPidPdfMaker::clearMembers(){
  pRichGeometryPar = NULL;
  pItTrackCand = NULL;
  pNtuple      = NULL;
  pTrackCand   = NULL;
  pCatKine     = NULL;
  clearArray();
}

// ---------------------------------------------------------------------------
Bool_t HPidPdfMaker::init(void){
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  pRichGeometryPar = (HRichGeometryPar*)rtdb->getContainer("RichGeometryParameters");
  HEvent *pEvent= gHades->getCurrentEvent();
  //Init categories and iterators
  //pItTrackCand = getIterator(catPidTrackCand,kTRUE); //get main iterator 
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
  
  pCatKine = getCategory(catGeantKine,kTRUE);
  if( NULL==pCatKine){
    cout <<"No kine" << endl;
    return kFALSE;
  } 
  
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
  
  pCatMdcClusInf = getCategory(catMdcClusInf,kTRUE);
  if( NULL==pCatMdcClusInf){
    cout <<"No mdcclusinf" << endl;
    return kFALSE;
  } 
  
  //set output file
  if(openOutFile(sOutFileName.Data()) == NULL){ 
    return kFALSE;
  }
  //Create NTuple
  TString ntuple_str(NTUPLE_VARS1);
  ntuple_str.Append(NTUPLE_VARS2);
  printf("Ntuple %s\n",ntuple_str.Data());
  if((pNtuple=new TNtuple("N","Ntuple with pdf data",ntuple_str))==NULL){
    Error("init", "Cannot create NTuple");
    return kFALSE;        
  }

  //ALL was initialized OK
  setInitOk();
  
  return kTRUE;
}
Bool_t HPidPdfMaker::checkConsistency()
{
  if(aPatternInstance[b_rktrack]>0 || aPatternInstance[b_reftrack]>0||aPatternInstance[b_kick123track]>0 || aPatternInstance[b_kicktrack]>0)
    return kFALSE;
  return kTRUE;
}
// ---------------------------------------------------------------------------

Int_t HPidPdfMaker::execute(void) {
  Float_t momx,momy,momz;
  checkGeantTrackNumbers();
  //reset iterators
    pItTrackCand->Reset();
    while((pTrackCand=(HPidTrackCandSim *)pItTrackCand->Next())!=NULL)
      {
	clearArray();


	
	pHitData = pTrackCand->getHitData();
	pTrackData = pTrackCand->getTrackData();
	pGeantTrackSet = pTrackCand->getGeantTrackSet();

	//cout << "__"<<pGeantTrackSet->getNCorrelatedTrackIds()<<endl;
	//pGeantTrackSet->getMomentumComponents(momx,momy,momz,pGeantTrackSet->getNCorrelatedTrackIds()-1);
	//cout << "__"<<momy<<endl;

	pRichHit=HPidFL::getRichHitSim(pHitData->iIndRICH);

	//Check for ring
        aPatternInstance[b_ring]=kFALSE;
        if(pHitData->iIndRICH >= 0){
	  aPatternInstance[b_ring]=kTRUE; 
	  aPatternInstance[i_ringtype]=getRingType(pRichHit);
	  if(pHitData->hasRingCorrelation[ALG_SPLINE]) 
	    {
	      aPatternInstance[b_corrRingSpline]=kTRUE;
	    }
	  else
	    {
	      aPatternInstance[b_corrRingSpline]=kFALSE;
	    }
	  if(pHitData->hasRingCorrelation[ALG_KICK]) 
	    {
	      aPatternInstance[b_corrRingKick]=kTRUE;
	    }
	  else
	    {
	      aPatternInstance[b_corrRingKick]=kFALSE;
	    }

	  bookRingObservables();
	}
	else
	  {
	    aPatternInstance[b_corrRingSpline]=kFALSE;
	    aPatternInstance[b_corrRingKick]=kFALSE;
	  }
	
	//check for available track types
	aPatternInstance[b_kicktrack]= (pTrackData->nKickTrackInd>=0)? kTRUE : kFALSE;
	
	aPatternInstance[b_splinetrack]= (pTrackData->nSplineTrackInd>=0)? kTRUE : kFALSE;
	
	aPatternInstance[b_rktrack]= (pTrackData->nRKTrackInd>=0)? kTRUE : kFALSE;
	
	aPatternInstance[b_reftrack]= (pTrackData->nRefTrackInd>=0)? kTRUE : kFALSE;
	
	aPatternInstance[b_kick123track]= (pTrackData->nKickTrack123Ind>=0)? kTRUE : kFALSE;
	/*
	if(!checkConsistency())
	  {cout << "Error! We have track ids where none are supposed to be after filling the pattern instance!!!" << endl;}
	*/
	
	//if(hasATrack()){
	aPatternInstance[i_tracktype]=getTrackType(pTrackCand);
	bookTrackObservables();
	bookVertexInformation();
	  //}
	
	//check for segments
	aPatternInstance[b_segments]=kFALSE;
	if(pTrackData->unInnerMdcNumber>0){
	  aPatternInstance[b_segments]= pTrackData->unInnerMdcNumber;
	}
	
	
	//Fill shower/tof variables
	if(pHitData->iIndShower>=0){
	  aPatternInstance[f_shw_sum0] = pHitData->fShowerSum[0];
	  aPatternInstance[f_shw_sum1] = pHitData->fShowerSum[1];
	  aPatternInstance[f_shw_sum2] = pHitData->fShowerSum[2];
	  aPatternInstance[n_shw_cs0]  = pHitData->nShowerClS[0];
	  aPatternInstance[n_shw_cs1]  = pHitData->nShowerClS[1];
	  aPatternInstance[n_shw_cs2]  = pHitData->nShowerClS[2];
	  aPatternInstance[f_shw_tof]  = pHitData->fShowerTimeOfFlight;
	  aPatternInstance[n_shw_mult] = pHitData->iTofinoMult;
	}
	if(pHitData->iIndTOF>=0)
	  {
	    aPatternInstance[f_tof_tof]  = pHitData->fTOFTimeOfFlight;
	  }
	
	bookCommonObservables();
	
	
	//Fill ntuple
	pNtuple->Fill(aPatternInstance);
      }
    return 0;
    
}

Bool_t HPidPdfMaker::hasATrack()
{
  return (aPatternInstance[b_kicktrack]>0 || aPatternInstance[b_splinetrack]>0 || aPatternInstance[b_rktrack]>0 ||aPatternInstance[b_reftrack]>0 || aPatternInstance[b_kick123track]>0);
}

// ---------------------------------------------------------------------------

Bool_t HPidPdfMaker::finalize(void) {

  //write ntuple to file

  return writeAndCloseOutFile();

}

Bool_t HPidPdfMaker::bookCommonObservables()
{
  aPatternInstance[n_sector]=(Float_t)(pHitData->nSector);
  if(pHitData->nSector>5 || pHitData->nSector<0)
    cout <<"NOPE! This sector does not exist!" << pHitData->nSector << endl;
  aPatternInstance[n_system]=(Float_t)(pHitData->iSystem);

  if(aPatternInstance[b_ring] && hasATrack())
    {
      aPatternInstance[f_deltatheta]=pHitData->fRichTheta-pHitData->fMdcTheta;
      aPatternInstance[f_deltaphi]=(pHitData->fRichPhi - pHitData->fMdcPhi)
	* TMath::Sin(pHitData->fMdcTheta);
    }

  return kTRUE;
}

Bool_t HPidPdfMaker::bookVertexInformation()
{
  if(!(pHitData && pTrackData))
    {
      ::Error("HPidPdfMaker::bookVertexInfo","No hit/track data found!");
      return kFALSE;
    }

  aPatternInstance[f_distance_to_vertex]=pHitData->fDistanceToVertex;
  aPatternInstance[f_track_r_spline]=pTrackData->fTrackR[ALG_SPLINE];
  aPatternInstance[f_track_z_spline]=pTrackData->fTrackZ[ALG_SPLINE];
  aPatternInstance[f_vertex_zcoord]=gHades->getCurrentEvent()->getHeader()->getVertexZ();
  return kTRUE;
}
  
  

Bool_t HPidPdfMaker::bookRingObservables()
{
  aPatternInstance[n_rich_patmatrix]=(float)pHitData->nRingPatMat;
  aPatternInstance[n_rich_npads]=(float)pHitData->nRingPadNr;
  aPatternInstance[f_rich_centroid]=(float)pHitData->fRingCentroid;
  aPatternInstance[f_rich_avcharge]=(float)pHitData->nRingAmplitude/(float)pHitData->nRingPadNr;
  aPatternInstance[n_rich_locmax]=(float)pHitData->nRingLocalMax4;
  return kTRUE;
}
  

Bool_t HPidPdfMaker::bookTrackObservables()
{
  if(aPatternInstance[i_tracktype]>=0)
    {
      aPatternInstance[f_mom_geant]=pGeantTrackSet->getTotalMomentum(0);//If the most common particle was NOT a fake - get its momentum
    }
  aPatternInstance[n_process]=pGeantTrackSet->getGeantProcessID(0);
  aPatternInstance[n_medium]=pGeantTrackSet->getGeantMediumID(0);
  aPatternInstance[f_metamatch_quality]=pTrackData->fMetaMatchingQuality;
  aPatternInstance[f_track_theta]=pHitData->fMdcTheta;
  aPatternInstance[f_track_phi]=pHitData->fMdcPhi;
  aPatternInstance[f_kick_mom_exp]=pTrackData->fMomenta[ALG_KICK];
  aPatternInstance[f_spline_mom_exp]=pTrackData->fMomenta[ALG_SPLINE];
  aPatternInstance[f_kick123_mom_exp]=pTrackData->fMomenta[ALG_KICK123];
  aPatternInstance[f_reft_mom_exp]=pTrackData->fMomenta[ALG_REFT];
  aPatternInstance[f_rk_mom_exp]=pTrackData->fMomenta[ALG_RUNGEKUTTA];
  aPatternInstance[f_kick_pull]=pTrackData->fPull;
  aPatternInstance[f_spline_quality]=pTrackData->fSplineChiSquare;
  bookSegmentObservables();

  return kTRUE;
}
  

Bool_t HPidPdfMaker::bookSegmentObservables()
{

  if(pHitData->iIndInnerSeg >= 0) {  //!!!!!!!!!!!!!!!!!!
    aPatternInstance[n_mdc_nw0]  = pHitData->iMdcNWires0;  
    aPatternInstance[n_mdc_cls0] = pHitData->iMdcClsSize0; 
    aPatternInstance[n_mdc_cfl0] = pHitData->iMdcLevelCls0;
  }
  
  
  if(pHitData->iIndOuterSeg >= 0) {  //!!!!!!!!!!!!!!!!!!
    aPatternInstance[n_mdc_nw1]  = pHitData->iMdcNWires1;  
    aPatternInstance[n_mdc_cls1] = pHitData->iMdcClsSize1; 
    aPatternInstance[n_mdc_cfl1] = pHitData->iMdcLevelCls1;
  }

  aPatternInstance[f_mdc_chi2] = pHitData->fInnerMdcChiSquare;
  return kTRUE;
}


void HPidPdfMaker::checkGeantTrackNumbers()  
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

Float_t HPidPdfMaker::getTrackType(HPidTrackCandSim* pTrackCand)
{
  // This function determines what kind of track we are dealing with
  // The result tells us where to book the ring associated to the track
  // For this we make a basic convention: 
  // We use exclusively track observables to determine where the ring goes.
  // We are NOT asking whether the ring has two, one or no leptonic GeantID 
  // in it. This may sound wrong, but it is correct! Please see 
  // <www.e12.physik.tu-muenchen.de:~/tachrist/tracktypes.pdf> for an
  // explanation and also the section below!

  // ------------------------------------------------------------------------
  // When we are building RICH ring-property-PDFs we are left with only four types of 
  // tracks: Those which we call double-leptonic, single-leptonic and fakes and hadronic. 
  
  // We always must ask whether a TRACK is from a single, double, hadron or fake. 
  // According to the answer to this question we book the RING which in this
  // sense is just a property of the track to the corresponding PDF.
  
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

  // Determination of ringtype:
  // a) set the ringtype to the number of geant leptons in the ring
  

  // This allows for the following combinations:
  //
  // tracktype->     -1,     0,     1,     2
  // ringtype
  //    |
  //    V
  //
  //    0             F      H      S      D 
  //
  //    1             F      H      S      D         F: Fake, H: Hadron, D: Double, S: Single
  //
  //    2             F      H      S      D
  
  // This scheme does NOT take into account situations where one has one lepton in
  // a kicktrack, one in the matching ring, but they come from two physically DIFFERENT
  // Geant particles! This is probably not really neccessary


  //-------------------------------------------------------------------------------------
  
  Int_t nMatchesMdcMeta, nMatchesFull, nLeptonicMatchesMdcMeta, GeantPid;
  //cout << "Counting leptons now" << endl;
  Int_t nLeptonsInInnerMdc = HPidFL::countLeptonsInInnerMdc(pGeantTrackSet);

  HPidFL::getGeantMatches(pTrackCand,nMatchesMdcMeta, nMatchesFull, nLeptonicMatchesMdcMeta, GeantPid);
  if(GeantPid!=-99)
    aPatternInstance[i_geant_pid]=GeantPid;  
  
  if(!nLeptonsInInnerMdc &&(GeantPid==2 ||GeantPid==3))
    {
      cout << "Inconsistent result! There was an electron without leptonic matches in mdc and meta!" << endl; 
      HPidFL::countLeptonsInInnerMdc(pGeantTrackSet, kTRUE);
    }
  


  if(nLeptonicMatchesMdcMeta) //This track has a matched lepton in MDC and META!
    return (Float_t)nLeptonsInInnerMdc;   //The tracktype is determined by the number of leptons in the segment;
  
  if(nMatchesMdcMeta) //No common leptons, but common track IDs - this was NOT a fake but some hadron!
    return 0.;

  return -1.; // No common leptons, no common track IDs at all - this was a fake
   
}
  

Float_t HPidPdfMaker::getRingType(HRichHitSim* pRichHit)
{
  Int_t nMirrTracks1, nMirrTracks2;
  //Check whether this ring is of leptonic origin
  if (HPidFL::isDoubleLepOnMirror(pRichHit, nMirrTracks1, nMirrTracks2))
    {
      return 2.0; 
    }
  else //It was not a double - was it a single ?
    {
      if (HPidFL::isLepOnMirror(pRichHit, nMirrTracks1))
	{
	  return 1.0; 
	}
      else {
	return 0.0;
      } //It was obviously a fake
    }
}


// -----------------------------------------------------------------------------

