// $Id: hpidsingleleptoneff.cc,v 1.13 2008-05-09 16:15:22 halo Exp $
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 05/06/05 21:47:04
// Modified in March 2006 by W. Przygoda / T. Wojcik (Cracow)
// Thomas.Eberl@ph.tum.de, last modified : 2006-11-08 10:42:49
//
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//
// HPidSingleLeptonEff
//
// This task is used to generate ntuples of embedded leptons
// with white distributions in 1/p, theta, phi.
// There are ntuples with such leptons in the acceptance and
// additionally identified and accepted leptons.
// These ntuples are later divided by each other (external code)
// to get (p,theta,phi) distributions of reconstruction efficiency
//
// Strategy:
//
// 1. Start from HGeantKine. Select primary leptons. Do not select
//    tracks which have a partner closer than 9.5 deg in the same sector.
//    ( "single tracks")
// 2. Try to find the GEANT track in created Pairs. Definition for the
//    single track to be accepted is:
//    a. most common track == GEANT track
//    b. is primary track
//    c. has reconstructed lepton PID
//    d. Geant PID  == reconstructed PID
//    e. partner of the track can be real track, hadron track or secondary lepton.
//       Pairs from embedded lepton are rejected
//    f. PairCut flag == 0  (not cut)
//    g. common coorelation flag >= 76 (fully correlated)
// 3. If the searched track is not found in a pair, it is
//    searched in HPidParticleSim (reconstructed as single track)
//    Same criteria as for the single track accepted in the pair search.
//
//
//
//  ntuple description:
//
//  members of the ACCEPTANCE and YIELD ntuples
//  p:theta:phi:pt calculated from HGeantKine
//
//  EFFICIENCY ntuple:
//
//  pRecon:thetaRecon:phiRecon:         // reconstructed track properties
//  pGeant:thetaGeant:phiGeant:ptGeant: // GEANT track properties
// 
//  commonTrack:                        // common correlation flag (repaired for hidden leptons)
//  isDirect:                           // 1 if a candidate pair with pair1->getIsCutFlag() != 0 has been found
//  isCut:                              // 0 for pairs, -1 for singles
//  cutNb:
//  nHadron:                            // number of pairs with hadron partner for the searched track
//  nLepton:                            // number of pairs with lepton partner for the searched track
//  isCP:                               // getAngleToClosestFittedLepton1 < 9 || getAngleToClosestNonFittedLepton1 < 9
//  angleHadronFitted:                  // environment of the reconstructed track
//  angleHadronNonFitted:               // ..
//  angleLeptonFitted:                  // ..
//  angleLeptonNonFitted:               // ..
//  isRing:                             // ring correlated with RK
//  innerChi2:outerChi2:                // chi2 of inner and outer MDCs
//  rkChi2:                             // RK chi2
//  metaMatchQality:                    //
//  metaMatchRK:                        // matching RK with META
//  vertX:vertY:vertZ:                  // event vertex
// 
//  isHiddenLepton:                     // 1 if a lepton track has been hidden by secondary in SHOWER
//  beta:                               // reconstructed beta of the track (start time corrected)
//  pairVertx:pairVerty:pairVertz:      // vertex of the pair
//  opangle:                            // reconstructed opening angle of the pair
//  pReconPartner                       // momentum of the partner in the pair
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidsingleleptoneff.h"

#include "hades.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hreconstructor.h"
#include "hgeomvector2.h"
#include "hgeomvector.h"

#include "hgeantkine.h"
#include "hgeanttof.h"
#include "hgeantshower.h"
#include "hgeantmdc.h"

#include "hrichhit.h"
#include "hmdcseg.h"

#include "hpidfl.h"
#include "hpidparticlesim.h"
#include "piddef.h"

#include "hpairsim.h"
#include "hpairdata.h"
#include "hpairgeantdata.h"
#include "hpaircutpar.h"
#include "hpairfl.h"

#include "TVector3.h"
#include "TH1.h"
#include "TH3.h"
#include "TMath.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TString.h"

#include <iostream>

using namespace std;

ClassImp(HPidSingleLeptonEff)

    HPidSingleLeptonEff::HPidSingleLeptonEff(Char_t * file)
    : HReconstructor("PidPartFiller","Filler of HPidParticle category")
{
    out = new TFile(file,"recreate");
    bookNTuple();
    maxNParticle = 100;
}

HPidSingleLeptonEff::~HPidSingleLeptonEff(){
    if(out) delete out;
}

HPidSingleLeptonEff::HPidSingleLeptonEff(Text_t name[], Text_t title[],Char_t * file)
: HReconstructor(name,title)
{
    out = new TFile(file,"recreate");
    bookNTuple();
    maxNParticle = 100;
}

void HPidSingleLeptonEff::bookNTuple(){

    simYieldEle  = new TNtuple("simYieldEle" ,"simYieldEle" ,"p:theta:phi:pt");
    simYieldPosi = new TNtuple("simYieldPosi","simYieldPosi","p:theta:phi:pt");
    TString sVarList("pRecon:pGeant:thetaRecon:thetaGeant:"
		     "phiRecon:phiGeant:ptGeant:isCut:isCP:"
		     "commonTrack:isDirect:cutNb:nHadron:nLepton:"
		     "angleHadronFitted:angleHadronNonFitted:"
		     "angleLeptonFitted:angleLeptonNonFitted:"
		     "isRing:innerChi2:rkChi2:metaMatchQality:"
		     "metaMatchRK:vertX:vertY:vertZ:"
		     "outerChi2:isHiddenLepton:beta:"
		     "pairVertx:pairVerty:pairVertz:"
		     "opangle:pReconPartner"
		    );

    effEle   = new TNtuple("effEle"  ,"effEle"  ,sVarList);
    effPosi  = new TNtuple("effPosi" ,"effPosi" ,sVarList);
    acceEle  = new TNtuple("acceEle" ,"acceEle" ,"p:theta:phi:pt");
    accePosi = new TNtuple("accePosi","accePosi","p:theta:phi:pt");
}

// -----------------------------------------------------------------------------
Bool_t HPidSingleLeptonEff::init(void){

    if((pPartCat = gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL)
    {
	Error("init", "No category catPidPart");
	return kFALSE;
    }
    pitPartCat = (HIterator *)pPartCat->MakeIterator();

    if((pPairCat = gHades->getCurrentEvent()->getCategory(catPairs)) == NULL){
	Error("init", "No category catPairs");
	return kFALSE;
    }
    pitPairCat  = (HIterator *) pPairCat->MakeIterator();


    pGeantCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
    if(!pGeantCat){
	Error("init", "No category catGeantKine ");
	return kFALSE;
    }
    pitGeant  = (HIterator *)pGeantCat->MakeIterator();
    pitGeant1 = (HIterator *)pGeantCat->MakeIterator();

    // --------- MDC GEANT category

    if(!gHades->getCurrentEvent()->getCategory(catMdcGeantRaw)){
	Error("init", "No category catMdcGeantRaw ");
	return kFALSE;
    }
    // --------- TOF GEANT category

    if(!gHades->getCurrentEvent()->getCategory(catTofGeantRaw)){
	Error("init", "No category catTofGeantRaw ");
	return kFALSE;
    }
    // --------- SHOWER GEANT category

    if(!gHades->getCurrentEvent()->getCategory(catShowerGeantRaw)){
	Error("init", "No category catTofGeantRaw ");
	return kFALSE;
    }

    // ---------- EVENT HEADER category

    pEvtHeader = (HEventHeader*) gHades->getCurrentEvent()->getHeader();
    if (!pEvtHeader)
    {
	Error("HPairData::init(HPair*)","current event header not found");
	return kFALSE;
    }


    HRuntimeDb* rtdb = gHades->getRuntimeDb();
    if (rtdb)   pC  = (HPairCutPar*) rtdb -> getContainer("PairCutParameters");
    if (!pC){
	Error("init()","PairCutParameters initialization via RTDB returned a NULL pointer");
	return kFALSE;
    }

    return kTRUE;
}
Int_t HPidSingleLeptonEff::execute(void){

    // All the lepton pairs contained in the GeantKine container are formed
    // The yield is stored in a ntuple (simYieldN) as a function of the pair mass
    // rapidity and transverse momentum. Moreover the lepton pairs in the acceptance
    // are stored in a ntuple (accept).
    // The HPidDilepton class contains all the pairs that have been reconstructed.
    // For each of the GeantKine pair in the acceptance we checked if this pair
    // has been properly
    // recontructed and is contained in the HPidDilepton Container.
    // For the first correctly recontructed pair a second ntuple (eff) will
    // be filled with weight 1, for each additional reconstructed pair
    // that dont correpond to any Geant pair, a third ntuple (fake) will be filled
    // with weight 1.
    // All these informations will be stored in Ntuples  to allow
    // later on the user to create of efficiency matrixes with different bining.
    HGeantKine *pKine;
    Int_t aParLepton, aMedLepton, aMechLepton;
    Int_t aTrackLepton, aIDLepton;

    Int_t * partIndex = new Int_t[maxNParticle];
    for(Int_t i = 0;i < maxNParticle; i ++) partIndex[i] = 0;

    Int_t countLep = 0;

    //-----------------------------------------------------------------
    // do not allow tracks from pairs closer than 9 deg
    removecloseTracks(partIndex);
    //-----------------------------------------------------------------

    pitGeant->Reset();
    while((pKine = (HGeantKine*) pitGeant->Next()) != NULL){

	pKine->getParticle(aTrackLepton,aIDLepton);
	pKine->getCreator (aParLepton,aMedLepton,aMechLepton);

	if(aParLepton == 0 && (aIDLepton == 2 || aIDLepton == 3)){
	    Float_t xMom,yMom,zMom;
	    TVector3 pD;
	    pKine->getMomentum(xMom,yMom,zMom);
	    pD.SetXYZ(xMom,yMom,zMom);
	    //-----------------------------------------------------------------
            // fill ntuple for total yield
	    if(aIDLepton == 3) simYieldEle ->Fill(pD.Mag(),pD.Theta(),pD.Phi(),pD.Pt());
	    if(aIDLepton == 2) simYieldPosi->Fill(pD.Mag(),pD.Theta(),pD.Phi(),pD.Pt());
	    //-----------------------------------------------------------------
	    // check acceptance and if the track has been reconstructed.
            // fill corresponding ntuples for efficiency calculations.
	    if (partIndex[countLep] == 0) checkEff(pKine);
	    //-----------------------------------------------------------------
	    countLep ++;

	    if(countLep == maxNParticle) {
		Warning("execute()","number of leptons exceeded maxNumber ! skipping!");
		break;
	    }
	}
    }

    delete [] partIndex;
    return 0;
}

void  HPidSingleLeptonEff::removecloseTracks(Int_t *p){

    // all the tracks coming fomr the white generator that
    // has a neighbour also coming from the white generator
    // that is closer than 9 deg are tagged if they
    // are inside the same sector
    HGeantKine *pKine  = NULL;
    HGeantKine *pKine1 = NULL;

    Int_t aTrackLepton,aIDLepton,aTrackLepton1,aIDLepton1;
    Int_t aParLepton,aMedLepton,aMechLepton,aParLepton1,aMedLepton1,aMechLepton1;
    Int_t countLep = -1;

    pitGeant->Reset();
    while((pKine = (HGeantKine*) pitGeant->Next()) != NULL)
    {
	pKine->getParticle(aTrackLepton,aIDLepton);
	pKine->getCreator (aParLepton,aMedLepton,aMechLepton);
        Int_t s = pKine->getSector();
	if(aParLepton == 0 && (aIDLepton == 2 || aIDLepton == 3))
	{
	    countLep ++;

	    if(countLep == maxNParticle) {
		Warning("removecloseTracks()","number of leptons exceeded maxNumber ! skipping!");
		break;
	    }

	    pitGeant1->Reset();
	    while((pKine1 = (HGeantKine*) pitGeant1->Next()) != NULL)
	    {
		Int_t s1 = pKine1->getSector();
                if(s != s1) continue;   // only tracks inside the same sector can influence each other
		pKine1->getParticle(aTrackLepton1,aIDLepton1);
		pKine1->getCreator (aParLepton1,aMedLepton1,aMechLepton1);
		if(aParLepton1 == 0                     &&
		   (aIDLepton1 == 2 || aIDLepton1 == 3) &&
		   aTrackLepton != aTrackLepton1){
		    if(calcOpeningAngleKine(pKine,pKine1) < 9.5){
			p[countLep] = 1;
		    }
		}
	    }
	}
    }
}


Float_t HPidSingleLeptonEff::calcOpeningAngleKine(HGeantKine *kine1,HGeantKine *kine2)
{
    //input 2 kine objects
    //output opening angle of trajectories

    Double_t rad2deg = TMath::RadToDeg();

    HGeomVector vec1;
    if (kine1){
	Float_t xMom1,yMom1,zMom1;
	kine1->getMomentum(xMom1,yMom1,zMom1);
	vec1.setX(xMom1);
	vec1.setY(yMom1);
	vec1.setZ(zMom1);

	vec1/=vec1.length();//norm
    }
    HGeomVector vec2;
    if (kine2){
	Float_t xMom2,yMom2,zMom2;
	kine2->getMomentum(xMom2,yMom2,zMom2);
	vec2.setX(xMom2);
	vec2.setY(yMom2);
	vec2.setZ(zMom2);

	vec2/=vec2.length();//norm
    }

    Float_t cosfOpeningAngle = vec1.scalarProduct(vec2);
    Float_t fOpeningAngle = 0.;
    if (TMath::Abs(cosfOpeningAngle) <= 1){
	fOpeningAngle = TMath::ACos(cosfOpeningAngle) * rad2deg;
    }
    return fOpeningAngle;
}

void HPidSingleLeptonEff::checkEff(HGeantKine *p1)
{
    // checks
    // 1. if GEANT track is in acceptance and fills the acceptance ntuple if true.
    // 2. loops over HPairSim category and counts number of reconstructed pairs
    //    which found the fully correlated track in a pair containing hadron or leptons.
    //    checks the direct cut flag for the pair.
    // 3. loops again over HPairSim and tries to find the searched track in a pair
    // 4. if track has not been found in a pair we do loop over HPidParticle category
    //    and try to find the corresponding single track.
    // 5. lepton ntuple are filled in case 3 and 4.

    Float_t mom1 = 0;
    TVector3 pD1;
    Int_t   trackNb, id;
    Float_t xMom1,yMom1,zMom1;
    p1->getMomentum(xMom1,yMom1,zMom1);

    p1->getParticle(trackNb,id);
    pD1.SetXYZ(xMom1,yMom1,zMom1);
    mom1 = TMath::Sqrt( xMom1 * xMom1 + yMom1 * yMom1 + zMom1 * zMom1);

    //-----------------------------------------------------------------
    // 1. if GEANT track is in acceptance and fills the acceptance ntuple if true.
    if(isGeantTrackInAcceptance(p1)) {
	if(id == 2) accePosi->Fill(mom1,pD1.Theta(),pD1.Phi(),pD1.Pt());
	if(id == 3) acceEle ->Fill(mom1,pD1.Theta(),pD1.Phi(),pD1.Pt());
    } else return; // nothing to do for this track
    //-----------------------------------------------------------------

    Int_t comDet          = 0;
    Int_t whichParticle   = -1;
    Bool_t isHiddenLepton = kFALSE;

    //Check for direct cuts
    Int_t  nLepCount  = 0;
    Int_t  nHadCount  = 0;
    Int_t  bDirectCut = 0;

    //-----------------------------------------------------------------
    // 2. loops over HPairSim category and counts number of reconstructed pairs
    //    which found the fully correlated track in a pair containing hadron or leptons.
    //    checks the direct cut flag for the pair.
    HPairSim * pair1 = NULL;
    pitPairCat->Reset();
    while(( pair1 = (HPairSim*) pitPairCat->Next()) != NULL)
    {
	comDet = checkTrackId(pair1,trackNb,whichParticle,isHiddenLepton);
	if(comDet >= 76){
	    if(pair1->getPid1() < 0 || pair1->getPid2() < 0) { ++ nHadCount; }
	    if(pair1->getPid1() > 0 && pair1->getPid2() > 0) { ++ nLepCount; }
	    if(pair1->getPid1() > 0 || pair1->getPid2() > 0 && pair1->getIsCutFlag() != 0) bDirectCut = 1;
	}
    }
    //-----------------------------------------------------------------


    //-----------------------------------------------------------------
    // 3. loops again over HPairSim and tries to find the
    //    searched track in a pair

    HPairSim* pair     = NULL;
    Int_t   cpFlag     = 0;
    Int_t   flagPair   = 0;
    Float_t thetaExp   = 0.;
    Float_t phiExp     = 0.;
    Float_t momExp     = 0.;
    Int_t   isGoodPair = 0;

    pitPairCat->Reset();
    while(( pair = (HPairSim*) pitPairCat->Next()) != NULL)
    {
	comDet = checkTrackId(pair,trackNb,whichParticle,isHiddenLepton);
	if(comDet > 0) // track == trackNb, lepton PID, primary, PID == rec PID
	{   // not rejected
	    calculateTrackProperties(pair,thetaExp, phiExp, momExp,cpFlag,whichParticle);
	    flagPair = 1;
	    if (pair->getIsCutFlag() == 0)
	    {   // not cut

		if(comDet >= 76 && isGoodPair == 0 )
		{   // fully correlated track and first match

		    const HPidHitData*   pHitData   = 0;
		    const HPidTrackData* pTrackData = 0;

		    //-----------------------------------------------------------------
		    // ----------- track & particle data retrieved here

		    Float_t isRing           =  0.;
		    Float_t hadrFitted       = -1.;
		    Float_t hadrNonFitted    = -1.;
		    Float_t leptFitted       = -1.;
		    Float_t leptNonFitted    = -1.;
		    Float_t innerChi         = -1.;
		    Float_t rkChi            = -1.;
		    Float_t metaMatchQuality = -1.;
		    Float_t mmRK             = -1.;
		    Float_t xVert            =  0.;
		    Float_t yVert            =  0.;
		    Float_t zVert            =  0.;

		    Float_t outerChi         = -1.;
		    Float_t beta             = -1.;
                    Float_t pairVertx        = -1000.;
                    Float_t pairVerty        = -1000.;
                    Float_t pairVertz        = -1000.;
		    Float_t opangle          = -1.;
                    Float_t pReconPartner    = -1.;

		    HPidParticleSim * part = 0;
		    Int_t indexPart = -1;
		    if      (whichParticle == 1) indexPart = pair->getIndexParticle1();
		    else if (whichParticle == 2) indexPart = pair->getIndexParticle2();
		    else    {
			Error("checkEff()","Wrong number for particle selection!");
			continue;
		    }
		    part = (HPidParticleSim*) pPartCat->getObject(indexPart);

		    pHitData      = part->getHitData();
		    pTrackData    = part->getTrackData();
		    isRing        = static_cast<Float_t>( pHitData->getRingCorrelation(4) );  // has RK ring
		    hadrFitted    = pTrackData->getAngleWithClosestCandidate(-1,  1);         // look to the neighbourhood of the track
		    hadrNonFitted = pTrackData->getAngleWithClosestCandidate(-1, -1);
		    leptFitted    = pTrackData->getAngleWithClosestCandidate( 1,  1);
		    leptNonFitted = pTrackData->getAngleWithClosestCandidate( 1, -1);

		    innerChi         = pHitData->getInnerMdcChiSquare();
		    rkChi            = pTrackData->getRKChiSquare();
		    metaMatchQuality = pTrackData->getMetaMatchingQuality();
		    mmRK = TMath::Sqrt(pTrackData->getRkMetadx() * pTrackData->getRkMetadx() +
				       pTrackData->getRkMetady() * pTrackData->getRkMetady() +
				       pTrackData->getRkMetadz() * pTrackData->getRkMetadz() );
		    HVertex& vertex  = pEvtHeader->getVertex();
		    xVert = vertex.getX();
		    yVert = vertex.getY();
		    zVert = vertex.getZ();

                    TVector3 pairvertex;
		    TVector3 pairdistance;
		    HPairFL::calcVertex((HPidParticleSim*) pPartCat->getObject(pair->getIndexParticle1()),
					(HPidParticleSim*) pPartCat->getObject(pair->getIndexParticle2()),
					&pairvertex,&pairdistance);

		    outerChi         = pHitData->getOuterMdcChiSquare();
                    pairVertx        = pairvertex.x() ;
                    pairVerty        = pairvertex.y();
                    pairVertz        = pairvertex.z();
                    opangle          = pair->getOpeningAngleDeg();
		    beta             = pTrackData->fTofRecBeta[4] ;
	
                    HPidParticleSim* part2 ;
		    if      (whichParticle == 1) part2 = (HPidParticleSim*) pPartCat->getObject(pair->getIndexParticle2());
		    else if (whichParticle == 2) part2 = (HPidParticleSim*) pPartCat->getObject(pair->getIndexParticle1());
	            pReconPartner    = part->P();
		    //-----------------------------------------------------------------

		    //-----------------------------------------------------------------
                    // Fill ntuple for track found in a pair
		    Float_t aVar[] = {
			momExp,
			mom1,
			thetaExp,
			pD1.Theta(),
			phiExp,
			pD1.Phi(),
			pD1.Pt(),
			0,          // isCut
			cpFlag,
			comDet,
			bDirectCut,
			0,          // cutNb
			nHadCount,
			nLepCount,
			hadrFitted,
			hadrNonFitted,
			leptFitted,
			leptNonFitted,
			isRing,
			innerChi,
			rkChi,
			metaMatchQuality,
			mmRK,
			xVert,
			yVert,
			zVert,
			outerChi,
			isHiddenLepton,
		        beta,
			pairVertx,
			pairVerty,
			pairVertz,
			opangle,
			pReconPartner
		    };


		    if(id == 3) effEle ->Fill(aVar);
		    if(id == 2) effPosi->Fill(aVar);
		    //-----------------------------------------------------------------

		    isGoodPair = 1;  // we found the track, do not loop further
		} // end of comDet >= 76 && isGoodPair == 0
	    } // end of iscutflag == 0
	} // end of comDet > 0
    }
    //-----------------------------------------------------------------


    //-----------------------------------------------------------------
    // 4. if track has not been found in a pair we do loop over
    //    HPidParticle category and try to find the corresponding single track.

    if(flagPair == 0)
    {
	HPidParticleSim* part = 0;
	pitPartCat->Reset();
	while(( part = ( HPidParticleSim *)pitPartCat->Next()) != NULL)
	{
	    comDet = checkSingleTrack(part,trackNb,isHiddenLepton);
            if(comDet >= 76) // track == trackNb, lepton PID, primary, PID == rec PID, fully correlated
	    {
		const HPidHitData*   pHitData   = 0;
		const HPidTrackData* pTrackData = 0;

		//-----------------------------------------------------------------
		// ----------- track & particle data retrieved here
		Float_t isRing           =  0.;
		Float_t hadrFitted       = -1.;
		Float_t hadrNonFitted    = -1.;
		Float_t leptFitted       = -1.;
		Float_t leptNonFitted    = -1.;
		Float_t innerChi         = -1.;
        	Float_t rkChi            = -1.;
		Float_t metaMatchQuality = -1.;
		Float_t mmRK             = -1.;
		Float_t xVert            =  0.;
		Float_t yVert            =  0.;
		Float_t zVert            =  0.;
        	Float_t outerChi         = -1.;
		Float_t beta             = -1.;

		pHitData      = part->getHitData();
		pTrackData    = part->getTrackData();
		isRing        = static_cast<Float_t>( pHitData->getRingCorrelation(4) );    // has RK ring
		hadrFitted    = pTrackData->getAngleWithClosestCandidate(-1,  1);           // look to the neighbourhood of the track
		hadrNonFitted = pTrackData->getAngleWithClosestCandidate(-1, -1);
		leptFitted    = pTrackData->getAngleWithClosestCandidate( 1,  1);
		leptNonFitted = pTrackData->getAngleWithClosestCandidate( 1, -1);

		innerChi         = pHitData->getInnerMdcChiSquare();
		rkChi            = pTrackData->getRKChiSquare();
		metaMatchQuality = pTrackData->getMetaMatchingQuality();
		mmRK = TMath::Sqrt(pTrackData->getRkMetadx() * pTrackData->getRkMetadx() +
				   pTrackData->getRkMetady() * pTrackData->getRkMetady() +
				   pTrackData->getRkMetadz() * pTrackData->getRkMetadz() );
		HVertex& vertex  = pEvtHeader->getVertex();
		xVert = vertex.getX();
		yVert = vertex.getY();
		zVert = vertex.getZ();

		outerChi         = pHitData->getOuterMdcChiSquare();
		beta             = pTrackData->fTofRecBeta[4] ;
		//-----------------------------------------------------------------


		//-----------------------------------------------------------------
		// Fill ntuple for track not found in a pair
		Float_t aVar[] = {
		    part->P(),
		    mom1,
		    part->Theta(),
		    pD1.Theta(),
		    part->Phi(),
		    pD1.Phi(),
		    pD1.Pt(),
		    -1,         // isCut
		    -1,         // isCP
		    comDet,
		    -1,         // isDirect
		    0,          // cutNb
		    -1,         // nhadron
		    -1,         // nlepton
		    hadrFitted,
		    hadrNonFitted,
		    leptFitted,
		    leptNonFitted,
		    isRing,
		    innerChi,
		    rkChi,
		    metaMatchQuality,
		    mmRK,
		    xVert,
		    yVert,
		    zVert,
		    outerChi,
		    isHiddenLepton,
		    beta,
		    -1000,      // pairVertx
		    -1000,      // pairVerty
		    -1000,      // pairVertz
		    -1,         // opangle,
		    -1          // pReconPartner
		};

		if(id == 3) effEle ->Fill(aVar);
		if(id == 2) effPosi->Fill(aVar);
		//-----------------------------------------------------------------

		break;

	    } // comDet >=76
	} // end of HpidPartcle loop
    } // case there has been no pair found
}

Int_t HPidSingleLeptonEff::checkSingleTrack(HPidParticleSim* part,Int_t lepTrackNb,Bool_t& isHiddenLepton)
{
    // checks if the track euqal to the GEANT track, is primary , has lepton PID
    // and PID == GEANT PID. return the most common detector correlation if successfull
    // or 0 if not. If the most common correlation misses the META ( < 76) and the
    // track was going to system 0 (SHOWER), we try to find if the lepton track
    // was hidden by a secondary. If so, the in commonCorrelation the META bit is set
    // and the isHiddenLepton flag becomes true.


    HGeantKine* kine = (HGeantKine*) pGeantCat->getObject(lepTrackNb - 1);

    if(part->getGeantTrackSet()->getGeantTrackID()          == lepTrackNb &&   // same track number
       part->getGeantTrackSet()->getGeantParentTrackID()    == 0          &&   // primary
       (part->getPid() == 2 || part->getPid() == 3)                       &&   // lepton PID
       kine->getID() == part->getPid()                                         // PID == GEANT PID
      )
    {
        Int_t comDet = part->getGeantTrackSet()->getMostCommonCorrelation();

	if(comDet < 76 && part->getSystem() == 0){
	    Int_t first = findFirstHitInShower(lepTrackNb);
	    if(first == lepTrackNb) {
                comDet = comDet | kSeenInMETA; // set META bit
                isHiddenLepton = kTRUE;
	    } else {
                isHiddenLepton = kFALSE;
	    }
	}
	return comDet;
    } else {
	return 0;
    }

}
Int_t  HPidSingleLeptonEff::checkTrackId(HPairSim *p,Int_t lepTrackNb,Int_t& whichPart,Bool_t& isHiddenLepton)
{
    // check if the searched track is contained in the pair
    // allow pairs with geant hadrons or geant secondary leptons
    // pairs with embedded leptons are rejected. Checks the
    // reconstructed PID for leptons and check reconstructed PID == GEANT PID
    // Int_t whichPart will be -1 if no track was accepted and 1
    // or 2 if particle 1 or 2 of the pair was accepted.

    HPairGeantData pg(p);
    whichPart      = -1;
    isHiddenLepton = kFALSE;

    HPidParticleSim* part = (HPidParticleSim*) pPartCat->getObject(p->getIndexParticle1());
    Int_t comDet = checkSingleTrack(part,lepTrackNb,isHiddenLepton);
    if(comDet > 0){ // track == lepTrackNb, lepton PID, primary, PID == rec PID
	if((pg.getParentTrackNumber2() == 0 && (pg.getGPid2() != 2 && pg.getGPid2() != 3)) || //generator hadron
	   (pg.getParentTrackNumber2() != 0 ))     // secondary lepton (decay in GEANT) or real tracks
	{
	    whichPart = 1;
	    return comDet;
	}
	else return 0;                             // reject pairs from embedded leptons
    }

    part = (HPidParticleSim*) pPartCat->getObject(p->getIndexParticle2());
    comDet = checkSingleTrack(part,lepTrackNb,isHiddenLepton);
    if(comDet > 0){ // track == lepTrackNb, lepton PID, primary, PID == rec PID
	if((pg.getParentTrackNumber1() == 0 && (pg.getGPid1() != 2 && pg.getGPid1() != 3)) || //generator hadron
	   (pg.getParentTrackNumber1() != 0 ))     // secondary lepton (decay in GEANT) or real tracks
	{
	    whichPart = 2;
	    return comDet;
	}
	else return 0;                             // reject pairs from embedded leptons
    }
    return 0;
}

void HPidSingleLeptonEff::calculateTrackProperties(HPairSim *pair,
						   Float_t& th,Float_t& ph,Float_t& p,Int_t& cpF,
						   Int_t whichParticle)
{
    // calculates the pair theta,phi

    cpF = 0;
    HPairGeantData pg(pair);
    HPairData pd(pair);

    if(whichParticle == 1 && pg.getParentTrackNumber1() == 0){
	th = pd.getThetaDeg1();
	ph = pd.getPhiDeg1();
	p  = pd.getMom1();

	if (pd.getAngleToClosestFittedLepton1()    < 9 ||
	    pd.getAngleToClosestNonFittedLepton1() < 9)
	{
	    cpF = 1;
	}
    }
    else if(whichParticle == 2 && pg.getParentTrackNumber2() == 0){
	th = pd.getThetaDeg2();
	ph = pd.getPhiDeg2();
	p  = pd.getMom2();

	if (pd.getAngleToClosestFittedLepton2()    < 9 ||
	    pd.getAngleToClosestNonFittedLepton2() < 9)
	{
	    cpF = 1;
	}
    } else {
       Error("calculateTrackProperties()","Called with not identified Particle number!");
    }

}
Bool_t HPidSingleLeptonEff::isGeantTrackInAcceptance(HGeantKine *pG)
{
    // checks if thw Gent track is inside the Hades acceptance.
    // The acceptance is defined by a hit in the 4 MDCs and TOF
    // or SHOWER. Since Drift chambers can be taken out of the
    // HGeant geometry, for the outer drift chambers a hit in
    // MDC3 or MDC4 is required.

    Int_t nStatMDC1   =  pG->getNMdcHits(0);
    Int_t nStatMDC2   =  pG->getNMdcHits(1);
    Int_t nStatMDC3   =  pG->getNMdcHits(2);
    Int_t nStatMDC4   =  pG->getNMdcHits(3);
    Int_t nStatTof    =  pG->getNTofHits();
    Int_t nStatShower =  pG->getNShowerHits();

    if(nStatMDC1 && nStatMDC2 && ( nStatMDC3 || nStatMDC4 ) &&  (nStatTof || nStatShower))
    {
	return kTRUE;
    }
    return kFALSE;
}

Int_t HPidSingleLeptonEff::findFirstHitInShower(Int_t numTrack)
{
    //-------------------------------------------------
    // find the first track ID entering the SHOWER
    // Used to suppress the secondaries created in the
    // SHOWER itself. Returns the input track number
    // if no previous GEANT hit in shower is found.

    if(numTrack <= 0) return numTrack; // nothing to do for real data

    HGeantKine* kine = (HGeantKine*) pGeantCat->getObject(numTrack - 1);
    if(kine->getParentTrack() == 0) { return numTrack; } // nothing to do

    Int_t s         = kine->getSector();
    Int_t first     = 0;
    Int_t tempTrack = numTrack;

    while( (kine = kine->getParent(tempTrack,(HLinearCategory*)pGeantCat)) != 0) {
	if( (first = kine->getFirstShowerHit()) != -1) {
	    HGeantShower* gshr = (HGeantShower*) pGeantCat->getObject(first);

	    if(gshr->getSector() == s) { tempTrack = kine->getTrack(); }
	    else                       { break;}
	} else { break; }
    }
    return tempTrack;
}

Bool_t  HPidSingleLeptonEff::finalize()
{

    out->cd();
    simYieldEle ->Write();
    simYieldPosi->Write();
    effEle      ->Write();
    effPosi     ->Write();

    acceEle ->Write();
    accePosi->Write();
    out->Close();

    return kTRUE;
}
