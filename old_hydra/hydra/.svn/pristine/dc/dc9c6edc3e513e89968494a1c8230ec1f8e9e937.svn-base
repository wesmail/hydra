#include "hpidpdfmaker.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "richdef.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hpidtrackcand.h"
#include  "piddef.h"
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

ClassImp(HPidPdfMaker)
    
// ---------------------------------------------------------------------------
HPidPdfMaker::HPidPdfMaker(const char*cOutFileName)
            :HSUDummyRec(kFALSE){
    clearMembers();
    sOutFileName = cOutFileName;
}

// ---------------------------------------------------------------------------
HPidPdfMaker::~HPidPdfMaker(){
 
    //delete pointer to ntuple
    if(NULL!=pNtuple){
//        delete pNtuple;
    }
      
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
  //Init categories and iterators
  pItTrackCand = getIterator(catPidTrackCand,kTRUE); //get main iterator 
  //and report errors
  if( NULL==pItTrackCand){
    return kFALSE;
  } 
  
  pCatKine = getCategory(catGeantKine,kTRUE);
  if( NULL==pCatKine){
    return kFALSE;
  } 
  
  pCatMdcSeg = getCategory(catMdcSeg,kTRUE);
  if( NULL==pCatMdcSeg){
    return kFALSE;
  } 
  
  pCatMdcHit = getCategory(catMdcHit,kTRUE);
  if( NULL==pCatMdcHit){
    return kFALSE;
  } 
  
  pCatMdcClusInf = getCategory(catMdcClusInf,kTRUE);
  if( NULL==pCatMdcClusInf){
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
// ---------------------------------------------------------------------------

Int_t HPidPdfMaker::execute(void) {
  Int_t nHadRing=0;
  Int_t nLepRing=0;
  Int_t nHadNoRing=0;
  Int_t nLepNoRing=0;
    //reset iterators
    pItTrackCand->Reset();
    while((pTrackCand=(HPidTrackCand *)pItTrackCand->Next())!=NULL){
    clearArray();
      //Check for ring
        aPatternInstance[b_ring]=kFALSE;
        if(pTrackCand->getRingId() >= 0){
	  if((pRichHit = (HRichHitSim*) pTrackCand->getRichHit()) != NULL){
	    aPatternInstance[b_ring]=kTRUE; 
	    aPatternInstance[i_ringtype]=getRingType(pRichHit);
	    bookRingObservables();
	  }
        }
	//else continue;
	//cout << "After rich!" << endl;

        //check for kicktrack
        aPatternInstance[b_kicktrack]=kFALSE;
        if(pTrackCand->getKickTrackId()>=0){
	  if((pKickTrack= (HKickTrackSim*)pTrackCand->getKickTrack())!=NULL){
	    aPatternInstance[b_kicktrack]=kTRUE;
	    aPatternInstance[i_tracktype]=getTrackType(pTrackCand);
	    bookTrackObservables();
	  }
        } 
	if(aPatternInstance[b_ring]==1){ //track with ring
	  if(aPatternInstance[i_tracktype]>0){ //lepton with ring
	    nLepRing++;	    
	  }
	  else{
	    nHadRing++;
	  }
	}
	else { //track without ring
	  if(aPatternInstance[i_tracktype]>0){ //lepton without ring
	    nLepNoRing++;	    
	  }
	  else{
	    nHadNoRing++;
	  }
	}
	  
	//cout << "After track!" << endl;

        //check for segments
        aPatternInstance[b_segments]=kFALSE;
        if(pTrackCand->getInnerMdcNumber()>0){
	  aPatternInstance[b_segments]= pTrackCand->getInnerMdcNumber();
        }
	//cout << "After segments!" << endl;

        //Fill shower variables
        if((pShowerHitTof = pTrackCand->getShowerHitTofTrack())!=NULL){
	  aPatternInstance[f_shw_sum0] = pShowerHitTof->getSum(0);
	  aPatternInstance[f_shw_sum1] = pShowerHitTof->getSum(1);
	  aPatternInstance[f_shw_sum2] = pShowerHitTof->getSum(2);
	  aPatternInstance[n_shw_cs0]  = pShowerHitTof->getClusterSize(0);
	  aPatternInstance[n_shw_cs1]  = pShowerHitTof->getClusterSize(1);
	  aPatternInstance[n_shw_cs1]  = pShowerHitTof->getClusterSize(2);
	  aPatternInstance[f_shw_tof]  = pShowerHitTof->getTof();
	  aPatternInstance[n_shw_mult] = pShowerHitTof->getTofinoMult();
        }
	//cout << "After shower!" << endl;
	//Fill the common observables
	bookCommonObservables();
	//cout << "After common!" << endl;

        //Fill ntuple
        pNtuple->Fill(aPatternInstance);
    }
    //cout << "HPDFMAKER: " << nHadRing << " "<< nHadNoRing<< " "<< nLepRing<< " "<< nLepNoRing<< endl;
    //return 1;
    return 0;
    
}
// ---------------------------------------------------------------------------

Bool_t HPidPdfMaker::finalize(void) {

  //write ntuple to file
  
  return writeAndCloseOutFile();


}

Bool_t HPidPdfMaker::bookCommonObservables()
{
  if(aPatternInstance[b_kicktrack])
    aPatternInstance[n_kick_sector]=(Float_t)(pKickTrack->getSector());
    aPatternInstance[n_system]=(Float_t)(pKickTrack->getSystem());

  if(aPatternInstance[b_ring] && aPatternInstance[b_kicktrack])
    {
      aPatternInstance[f_deltatheta]=pRichHit->getTheta()-
	TMath::RadToDeg() * pKickTrack->getTheta();
      aPatternInstance[f_deltaphi]=(pRichHit->getPhi() - getMdcPhi())
	* TMath::Sin(pKickTrack->getTheta());
    }
  //aPatternInstance[n_process]=0.0; //Not filled at the moment
  return kTRUE;
}
  

Float_t HPidPdfMaker::getMdcPhi()
{

  Int_t iSector = pKickTrack->getSector();
  Float_t fPhi = TMath::RadToDeg() * pKickTrack->getPhi();

  switch(iSector)
    {
    case 0:
      break;
      
    case 1:
    case 2:
    case 3:
    case 4:
      fPhi += 60.0f * iSector;
      break;
      
    default:
      fPhi -= 60.0f;
      break;
    }

  return fPhi;
}
  

Bool_t HPidPdfMaker::bookRingObservables()
{
  aPatternInstance[n_rich_patmatrix]=(float)pRichHit->getRingPatMat();
  aPatternInstance[n_rich_npads]=(float)pRichHit->getRingPadNr();
  aPatternInstance[f_rich_centroid]=(float)pRichHit->getCentroid();
  aPatternInstance[f_rich_avcharge]=(float)pRichHit->getRingAmplitude()/pRichHit->getRingPadNr();
  aPatternInstance[n_rich_locmax]=(float)pRichHit->getRingLocalMax4();
  return kTRUE;
}
  

Bool_t HPidPdfMaker::bookTrackObservables()
{

  aPatternInstance[f_mom_geant]= HPidFL::getKineMomentum(pTrackCand);;
  aPatternInstance[f_kick_theta]=TMath::RadToDeg() * pKickTrack->getTheta();
  aPatternInstance[f_kick_phi]=getMdcPhi();
  aPatternInstance[f_kick_mom_exp]=pKickTrack->getP();
  bookSegmentObservables();

  return kTRUE;
}
  

Bool_t HPidPdfMaker::bookSegmentObservables()
{

  Int_t sector = pKickTrack->getSector();
  Int_t MdcSegmentId = pKickTrack->getSegment();
  HLocation segLoc;
  segLoc.set(3,sector,0,MdcSegmentId);           // 0 - inner segment

  Int_t Mdc0HitId = ((HMdcSeg*)(pCatMdcSeg->getObject(segLoc)))->getHitInd(0);

  if(Mdc0HitId >= 0) {  //!!!!!!!!!!!!!!!!!!
    Int_t Mdc0ClusId = ((HMdcHit*)(pCatMdcHit->getObject(Mdc0HitId)))->getClusInfIndex();
    aPatternInstance[n_mdc_nw0]   = ((HMdcClusInf*)(pCatMdcClusInf->getObject(Mdc0ClusId)))->getNDrTimes();
    aPatternInstance[n_mdc_cls0] = ((HMdcClusInf*)(pCatMdcClusInf->getObject(Mdc0ClusId)))->getClusSize();
    aPatternInstance[n_mdc_cfl0] = ((HMdcClusInf*)(pCatMdcClusInf->getObject(Mdc0ClusId)))->getLevelClFinding();
  }
  
  
  Int_t Mdc1HitId = ((HMdcSeg*)(pCatMdcSeg->getObject(segLoc)))->getHitInd(1);

  if(Mdc1HitId >= 0) {  //!!!!!!!!!!!!!!!!!!
    Int_t Mdc1ClusId = ((HMdcHit*)(pCatMdcHit->getObject(Mdc1HitId)))->getClusInfIndex();
    aPatternInstance[n_mdc_nw1]   = ((HMdcClusInf*)(pCatMdcClusInf->getObject(Mdc1ClusId)))->getNDrTimes();
    aPatternInstance[n_mdc_cls1] = ((HMdcClusInf*)(pCatMdcClusInf->getObject(Mdc1ClusId)))->getClusSize();
    aPatternInstance[n_mdc_cfl1] = ((HMdcClusInf*)(pCatMdcClusInf->getObject(Mdc1ClusId)))->getLevelClFinding();
  }

  aPatternInstance[f_mdc_chi2] = ((HMdcSeg*)(pCatMdcSeg->getObject(MdcSegmentId)))->getChi2();
  return kTRUE;
}
  

Float_t HPidPdfMaker::getTrackType(HPidTrackCand* pTrackCand)
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
  
  Int_t wait;
  
  Int_t nMatchesMdcMeta, nMatchesFull, nLeptonicMatchesMdcMeta, GeantPid;
  
  Int_t nLeptonsInInnerMdc = HPidFL::countLeptonsInKickTrack(pKickTrack);

  HPidFL::getGeantMatches(pTrackCand,nMatchesMdcMeta, nMatchesFull, nLeptonicMatchesMdcMeta, GeantPid);
  if(GeantPid!=-99)
    aPatternInstance[i_geant_pid]=GeantPid;  
  /*
  if(nLeptonsInInnerMdc){
    cout << "HPIDPDFMAKER: leptons in kicktrack = " << nLeptonsInInnerMdc << endl;
    cout << "HPIDPDFMAKER: Leptonic matches KT/META = " << nLeptonicMatchesMdcMeta << endl;
    cout << "HPIDPDFMAKER: matches KT/META = " << nMatchesMdcMeta << endl;
    //cin >> wait;
  }
  */
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

