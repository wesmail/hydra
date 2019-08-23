// $Id: hpidsingleleptoneff.cc,v 1.15 2009-02-20 15:56:55 jurkovic Exp $
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Modified by Laura Fabbietti: 05/06/05 21:47:04
// Modified by W. Przygoda / T. Wojcik (Cracow) in March 2006
// Modified by Thomas.Eberl@ph.tum.de, last modified : 2006-11-08 10:42:49
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
//    c. has reconstructed lepton PID and correlation with a RICH ring
//    d. Geant PID  == reconstructed PID
//    e. partner of the track can be real track, hadron track or secondary lepton.
//       Pairs from embedded lepton are rejected
//    f. PairCut flag != 1  (not direct cut)
//    g. common coorelation flag >= 76 (fully correlated)
// 3. If the searched track is not found in a pair, it is
//    searched in HPidParticleSim (reconstructed as single track)
//    Same criteria as for the single track accepted in the pair search.
//
//  ALL ANGLES ARE IN DEGREES:  0 < phi < 360 and 0 < theta < 90 as it is usual
//  all over the HADES analysis chain.
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
//  comDet:                             // common correlation flag (repaired for hidden leptons)
//  isCutNb:                            // cut nb of recursive cut for pairs
//                                      // ( pairs with isCutNb == 1 ==> direct cut are rejected),
//                                      // -1 for singles
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
//  opang:                              // reconstructed opening angle of the pair
//  pReconPartner                       // momentum of the partner in the pair
//  pidRecPartner                       // PID of the partner in the pair
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidsingleleptoneff.h"

#include "hades.h"
#include "hcategory.h"
#include "hevent.h"
#include "hgeantkine.h"
#include "hgeantmdc.h"
#include "hgeantshower.h"
#include "hgeanttof.h"
#include "hiterator.h"
#include "hmdcseg.h"
#include "hpairdata.h"
#include "hpairfl.h"
#include "hpairgeantdata.h"
#include "hpairsim.h"
#include "hpidfl.h"
#include "hpidparticlesim.h"
#include "hreconstructor.h"
#include "pairsdef.h"
#include "piddef.h"

#include "TFile.h"
#include "TMath.h"
#include "TNtuple.h"
#include "TString.h"
#include "TVector3.h"

#include <iostream>

using namespace std;

ClassImp(HPidSingleLeptonEff)

// -----------------------------------------------------------------------------
HPidSingleLeptonEff::HPidSingleLeptonEff(const Char_t * file)
    : HReconstructor("PidPartFiller","Filler of HPidParticle category")
{
    out = new TFile(file,"recreate");
    bookNTuple();
    maxNParticle = 100;
}

// -----------------------------------------------------------------------------
HPidSingleLeptonEff::~HPidSingleLeptonEff()
{
    if( NULL != out )
    {
       delete out;
       out = NULL;
    }
    if ( NULL != pitPartCat )
    {
       delete pitPartCat;
       pitPartCat = NULL;
    }
    if ( NULL != pitPairCat )
    {
       delete pitPairCat;
       pitPairCat = NULL;
    }
    if ( NULL != pitGeant )
    {
       delete pitGeant;
       pitGeant = NULL;
    }
    if ( NULL != pitGeant1 )
    {
       delete pitGeant1;
       pitGeant1 = NULL;
    }
}

// -----------------------------------------------------------------------------
HPidSingleLeptonEff::HPidSingleLeptonEff(Text_t name[], Text_t title[], const Char_t * file)
: HReconstructor(name,title)
{
    out = new TFile(file,"recreate");
    bookNTuple();
    maxNParticle = 100;
}

// -----------------------------------------------------------------------------
void HPidSingleLeptonEff::bookNTuple()
{

    simYieldEle  = new TNtuple("simYieldEle","simYieldEle","p:theta:phi:pt");
    simYieldPos  = new TNtuple("simYieldPos","simYieldPos","p:theta:phi:pt");
    TString sVarList("pRecon:pGeant:thetaRecon:thetaGeant:"
		     "phiRecon:phiGeant:ptGeant:isCutNb:"
		     "commonTrack:bitFieldPassedCuts:"
		     "angleHadronFitted:angleHadronNonFitted:"
		     "angleLeptonFitted:angleLeptonNonFitted:"
		     "isRing:innerChi2:rkChi2:metaMatchQality:"
		     "metaMatchRK:vertX:vertY:vertZ:"
		     "outerChi2:isHiddenLepton:beta:"
		     "pairVertx:pairVerty:pairVertz:"
		     "opang:pReconPartner:pidRecPartner"
		    );

    effEle   = new TNtuple("effEle",  "effEle", sVarList);
    effPos   = new TNtuple("effPos",  "effPos", sVarList);
    acceEle  = new TNtuple("acceEle", "acceEle", "p:theta:phi:pt");
    accePos  = new TNtuple("accePos", "accePos", "p:theta:phi:pt");
}

// -----------------------------------------------------------------------------
Bool_t
HPidSingleLeptonEff::init(void)
{

    if( NULL == (pPartCat = gHades->getCurrentEvent()->getCategory(catPidPart)))
    {
	Error("init", "No category catPidPart");
	return kFALSE;
    }
    pitPartCat = (HIterator *)pPartCat->MakeIterator();

    if( NULL == (pPairCat = gHades->getCurrentEvent()->getCategory(catPairs)) )
    {
	Error("init", "No category catPairs");
	return kFALSE;
    }
    pitPairCat  = (HIterator *) pPairCat->MakeIterator();

    if( NULL == (pGeantCat = gHades->getCurrentEvent()->getCategory(catGeantKine)) )
    {
	Error("init", "No category catGeantKine ");
	return kFALSE;
    }
    pitGeant  = (HIterator *)pGeantCat->MakeIterator();
    pitGeant1 = (HIterator *)pGeantCat->MakeIterator();

    // --------- MDC GEANT category
    if( NULL == gHades->getCurrentEvent()->getCategory(catMdcGeantRaw) )
    {
	Error("init", "No category catMdcGeantRaw ");
	return kFALSE;
    }

    // --------- TOF GEANT category
    if( NULL == gHades->getCurrentEvent()->getCategory(catTofGeantRaw) )
    {
	Error("init", "No category catTofGeantRaw ");
	return kFALSE;
    }

    // --------- SHOWER GEANT category
    if( NULL == gHades->getCurrentEvent()->getCategory(catShowerGeantRaw) )
    {
	Error("init", "No category catTofGeantRaw ");
	return kFALSE;
    }

    // ---------- EVENT HEADER category
    if ( NULL == (pEvtHeader = (HEventHeader*) gHades->getCurrentEvent()->getHeader()) )
    {
	Error("init", "Event header not found");
	return kFALSE;
    }


    return kTRUE;
}

// -----------------------------------------------------------------------------
Int_t
HPidSingleLeptonEff::execute(void)
{
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

    HGeantKine *pKine = NULL;
    TVector3    pD;

    Float_t xMom,yMom,zMom;

    Int_t aTrackLepton, aIDLepton;
    Int_t aParLepton, aMedLepton, aMechLepton;

    Int_t* partIndex = new Int_t[maxNParticle];
    for( Int_t i = 0; i < maxNParticle; ++i )
       partIndex[i] = 0;


    //-----------------------------------------------------------------
    // do not allow tracks from pairs closer than 9 deg
    removecloseTracks(partIndex);
    //-----------------------------------------------------------------

    pitGeant->Reset();
    while( NULL != (pKine = (HGeantKine*) pitGeant->Next()) )
    {

	pKine->getParticle(aTrackLepton,aIDLepton);
	pKine->getCreator (aParLepton,aMedLepton,aMechLepton);
	if(aParLepton == 0 && (aIDLepton == 2 || aIDLepton == 3))
        {
           if( aTrackLepton >= maxNParticle )
           {
              Warning("execute","Track number of lepton exceeded maxNumber ! skipping!");
              break;
           }
	    pKine->getMomentum(xMom, yMom, zMom);
	    pD.SetXYZ(xMom, yMom, zMom);
	    //-----------------------------------------------------------------
            // fill ntuple for total yield, all angles are in degrees
            // in usual HADES range 0 < phi < 360 and 0 < theta < 90
	    if( aIDLepton == 3 )
               simYieldEle->Fill(pD.Mag(),pD.Theta()*TMath::RadToDeg(),transformPhi(pD.Phi()),pD.Pt());
	    if( aIDLepton == 2 )
               simYieldPos->Fill(pD.Mag(),pD.Theta()*TMath::RadToDeg(),transformPhi(pD.Phi()),pD.Pt());
	    //-----------------------------------------------------------------
	    // check acceptance and if the track has been reconstructed.
            // fill corresponding ntuples for efficiency calculations.
	    if (partIndex[aTrackLepton] == 0) checkEff(pKine);
	    //-----------------------------------------------------------------

	}
    }

    delete [] partIndex;
    return 0;
}

// -----------------------------------------------------------------------------
void
HPidSingleLeptonEff::removecloseTracks(Int_t* p)
{
    // all the tracks coming fomr the white generator that
    // has a neighbour also coming from the white generator
    // that is closer than 9 deg are tagged if they
    // are inside the same sector

    HGeantKine *pKine1 = NULL;
    HGeantKine *pKine2 = NULL;

    Int_t aTrackLepton1, aIDLepton1;
    Int_t aTrackLepton2, aIDLepton2;
    Int_t aParLepton1, aMedLepton1, aMechLepton1;
    Int_t aParLepton2, aMedLepton2, aMechLepton2;

    pitGeant->Reset();
    while( NULL != (pKine1 = (HGeantKine*) pitGeant->Next()) )
    {
	pKine1->getParticle(aTrackLepton1, aIDLepton1);
	pKine1->getCreator (aParLepton1, aMedLepton1, aMechLepton1);
	if ( aParLepton1 == 0 && (aIDLepton1 == 2 || aIDLepton1 == 3) )
	{
	    if( aTrackLepton1 >= maxNParticle )
            {
		Warning("removecloseTracks", "Track number of lepton exceeded maxNumber ! skipping!");
		break;
	    }

	    pitGeant1->Reset();
	    while( NULL != (pKine2 = (HGeantKine*) pitGeant1->Next()) )
	    {
                if ( pKine1->getSector() != pKine2->getSector() )
                   continue;   // only tracks inside the same sector can influence each other

		pKine2->getParticle(aTrackLepton2, aIDLepton2);
		pKine2->getCreator (aParLepton2, aMedLepton2, aMechLepton2);
		if(aParLepton2 == 0                     &&
		   (aIDLepton2 == 2 || aIDLepton2 == 3) &&
		   aTrackLepton1 != aTrackLepton2 )
                {
		    if(calcOpeningAngleKine(pKine1, pKine2) < 9.0)
                    {
			p[aTrackLepton1] = 1;
		    }
		}
	    }
	}
    }
}


// -----------------------------------------------------------------------------
Float_t
HPidSingleLeptonEff::calcOpeningAngleKine(HGeantKine* kine1,
                                          HGeantKine* kine2)
{
    //input 2 kine objects
    //output opening angle of trajectories

    TVector3 vec1;
    TVector3 vec2;

    Float_t xMom1,yMom1,zMom1;
    Float_t xMom2,yMom2,zMom2;

    if ( NULL != kine1 )
    {
	kine1->getMomentum(xMom1, yMom1, zMom1);
	vec1.SetX(xMom1);
	vec1.SetY(yMom1);
	vec1.SetZ(zMom1);
    }
    else
    {
       Error("calcOpeningAngleKine", "First pointer is NULL");
       return -1.;
    }

    if ( NULL != kine2 )
    {
	kine2->getMomentum(xMom2, yMom2, zMom2);
	vec2.SetX(xMom2);
	vec2.SetY(yMom2);
	vec2.SetZ(zMom2);
    }
    else
    {
       Error("calcOpeningAngleKine", "Second pointer is NULL");
       return -1.;
    }

    return TMath::RadToDeg() * vec1.Angle(vec2);
}

// -----------------------------------------------------------------------------
void
HPidSingleLeptonEff::checkEff(HGeantKine* pKine)
{
    // checks
    // 1. if GEANT track is in acceptance ==> fills the acceptance ntuple.
    // 2. loops over HPairSim category and counts number of reconstructed pairs
    //    which found the fully correlated track in a pair containing hadron or leptons.
    //    checks the direct cut flag for the pair.
    // 3. loops again over HPairSim and tries to find the searched track in a pair
    // 4. if track has not been found in a pair we do loop over HPidParticle category
    //    and try to find the corresponding single track.
    // 5. lepton ntuple are filled in case 3 and 4.

          HPairSim*        pair       = NULL;
          HPidParticleSim* part       = NULL;
          HPidParticleSim* part2      = NULL;
    const HPidHitData*     pHitData   = NULL;
    const HPidTrackData*   pTrackData = NULL;

    TVector3 pD;
    TVector3 pairvertex;
    TVector3 pairdistance;

    Float_t xMom       = 0.;
    Float_t yMom       = 0.;
    Float_t zMom       = 0.;
    Float_t thetaExp   = 0.;
    Float_t phiExp     = 0.;
    Float_t momExp     = 0.;
    Float_t mmRK       = -1.;

    Int_t aTrackLepton, aIDLepton;
    Int_t comDet          = 0;
    Int_t whichParticle   = -1;
    Int_t indexPart       = -1;

    Bool_t isHiddenLepton = kFALSE;
    Bool_t flagPair       = 0;
    Bool_t isGoodPair     = 0;

    pKine->getMomentum(xMom, yMom, zMom);
    pKine->getParticle(aTrackLepton, aIDLepton);
    pD.SetXYZ(xMom, yMom, zMom);

    //-----------------------------------------------------------------
    // 1. if GEANT track is in acceptance and fills the acceptance ntuple if true.
    // All angles are in degrees
    // in usual HADES range 0 < phi < 360 and 0 < theta < 90
    if( isGeantTrackInAcceptance(pKine) )
    {
        if(aIDLepton == 2) accePos->Fill(pD.Mag(),pD.Theta() * TMath::RadToDeg(),transformPhi(pD.Phi()),pD.Pt());
        if(aIDLepton == 3) acceEle->Fill(pD.Mag(),pD.Theta() * TMath::RadToDeg(),transformPhi(pD.Phi()),pD.Pt());
    } else return; // nothing to do for this track
    //-----------------------------------------------------------------


    //-----------------------------------------------------------------
    // 3. loops again over HPairSim and tries to find the
    //    searched track in a pair

    pitPairCat->Reset();
    while ( NULL != ( pair = (HPairSim*) pitPairCat->Next()) )
    {
	comDet = checkTrackId(pair, aTrackLepton, whichParticle, isHiddenLepton);

	if( comDet >= 76 ) // track == trackNb, lepton PID, primary, PID == rec PID
	{   // not rejected
	    flagPair = 1;
	    if (pair->getIsCutFlag() != 1 && isGoodPair == 0 )
	    {   // not direct cut, recursive cuting sets this variable to values > 1
                // fully correlated track and first pair match found

                calculateTrackProperties(pair, thetaExp, phiExp, momExp, whichParticle);

                //-----------------------------------------------------------------
                // ----------- track & particle data retrieved here

                if      (whichParticle == 1) indexPart = pair->getIndexParticle1();
                else if (whichParticle == 2) indexPart = pair->getIndexParticle2();
                else    {
                   Error("checkEff()","Wrong number for particle selection!");
                   continue;
                }

                part = (HPidParticleSim*) pPartCat->getObject(indexPart);
                pHitData      = part->getHitData();
                pTrackData    = part->getTrackData();

                mmRK = TMath::Sqrt(pTrackData->getRkMetadx() * pTrackData->getRkMetadx() +
                                   pTrackData->getRkMetady() * pTrackData->getRkMetady() +
                                   pTrackData->getRkMetadz() * pTrackData->getRkMetadz() );

                HPairFL::calcVertex((HPidParticleSim*) pPartCat->getObject(pair->getIndexParticle1()),
                                    (HPidParticleSim*) pPartCat->getObject(pair->getIndexParticle2()),
                                    &pairvertex,&pairdistance);

                if      (whichParticle == 1) part2 = (HPidParticleSim*) pPartCat->getObject(pair->getIndexParticle2());
                else if (whichParticle == 2) part2 = (HPidParticleSim*) pPartCat->getObject(pair->getIndexParticle1());
                //-----------------------------------------------------------------

                //-----------------------------------------------------------------
                // Fill ntuple for track found in a pair
                Float_t aVar[] = {
                   momExp,
                   pD.Mag(),
                   thetaExp,
                   pD.Theta() * TMath::RadToDeg(),
                   phiExp,
                   transformPhi(pD.Phi()),
                   pD.Pt(),
                   pair->getIsCutFlag(),                                   // isCutNb
                   comDet,                                                 // common detector
                   pair->getBitFieldPassedCuts(),                          // for direct cut = info about passed cuts
                   pTrackData->getAngleWithClosestCandidate(-1,  1),       // hadron fitted,
                   pTrackData->getAngleWithClosestCandidate(-1, -1),       // hadron non fitted,
                   pTrackData->getAngleWithClosestCandidate( 1,  1),       // lepton fitted,
                   pTrackData->getAngleWithClosestCandidate( 1, -1),       // lepton non fitted,
                   static_cast<Float_t>(pHitData->getRingCorrelation(4)),  // has RK ring
                   pHitData->getInnerMdcChiSquare(),                       // innerChi2,
                   pTrackData->getRKChiSquare(),                           // rkChi2,
                   pTrackData->getMetaMatchingQuality(),                   // metaMatchQuality,
                   mmRK,                                                   // RK-META distance
                   pEvtHeader->getVertex().getX(),                         // vertex X
                   pEvtHeader->getVertex().getY(),                         // vertex Y
                   pEvtHeader->getVertex().getZ(),                         // vertex Z
                   pHitData->getOuterMdcChiSquare(),                       // outerChi,
                   isHiddenLepton,                                         // hidden lepton flag
                   pTrackData->fTofRecBeta[4],                             // beta,
                   pairvertex.x(),                                         // pairVertx,
                   pairvertex.y(),                                         // pairVerty,
                   pairvertex.z(),                                         // pairVertz,
                   pair->getOpeningAngleDeg(),                             // opening angle,
                   part2->P(),                                             // momentum of partner
                   part2->getPid()                                         // PID of partner
                };

                if(aIDLepton == 3) effEle->Fill(aVar);
                if(aIDLepton == 2) effPos->Fill(aVar);
                //-----------------------------------------------------------------

                isGoodPair = 1;  // we found the track, do not loop further
	    } // end of iscutflag == 0
	} // end of comDet > 0
    }
    //-----------------------------------------------------------------


    //-----------------------------------------------------------------
    // 4. if track has not been found in a pair we do loop over
    //    HPidParticle category and try to find the corresponding single track.

    if ( 0 == flagPair )
    {
	pitPartCat->Reset();
	while( NULL != (part = ( HPidParticleSim *)pitPartCat->Next()) )
	{
	    comDet = checkSingleTrack(part, aTrackLepton, isHiddenLepton);
            if(comDet >= 76) // track == trackNb, lepton PID, primary, PID == rec PID, fully correlated
	    {
		//-----------------------------------------------------------------
		// ----------- track & particle data retrieved here

		pHitData      = part->getHitData();
		pTrackData    = part->getTrackData();

		mmRK = TMath::Sqrt(pTrackData->getRkMetadx() * pTrackData->getRkMetadx() +
				   pTrackData->getRkMetady() * pTrackData->getRkMetady() +
				   pTrackData->getRkMetadz() * pTrackData->getRkMetadz() );
		//-----------------------------------------------------------------


		//-----------------------------------------------------------------
		// Fill ntuple for track not found in a pair
		Float_t aVar[] = {
		    part->P(),
		    pD.Mag(),
		    part->thetaDeg(),
		    pD.Theta() * TMath::RadToDeg(),
		    part->phiDeg(),
		    transformPhi(pD.Phi()),
		    pD.Pt(),
		    -1,         // isCutNb
		    comDet,
		    -1,         // bitFieldPassedCuts
                    pTrackData->getAngleWithClosestCandidate(-1,  1),       // hadron fitted,
                    pTrackData->getAngleWithClosestCandidate(-1, -1),       // hadron non fitted,
                    pTrackData->getAngleWithClosestCandidate( 1,  1),       // lepton fitted,
                    pTrackData->getAngleWithClosestCandidate( 1, -1),       // lepton non fitted,
                    static_cast<Float_t>(pHitData->getRingCorrelation(4)),  // has RK ring
                    pHitData->getInnerMdcChiSquare(),                       // innerChi2,
                    pTrackData->getRKChiSquare(),                           // rkChi2,
                    pTrackData->getMetaMatchingQuality(),                   // metaMatchQuality,
                    mmRK,                                                   // RK-META distance
                    pEvtHeader->getVertex().getX(),                         // vertex X
                    pEvtHeader->getVertex().getY(),                         // vertex Y
                    pEvtHeader->getVertex().getZ(),                         // vertex Z
                    pHitData->getOuterMdcChiSquare(),                       // outerChi,
                    isHiddenLepton,                                         // hidden lepton flag
                    pTrackData->fTofRecBeta[4],                             // beta,
		    -1000,                                                  // pairVertx
		    -1000,                                                  // pairVerty
		    -1000,                                                  // pairVertz
		    -1,                                                     // opangle,
		    -1,                                                     // pReconPartner
                    -1                                                      // PID_recPartner
		};

		if(aIDLepton == 3) effEle ->Fill(aVar);
		if(aIDLepton == 2) effPos->Fill(aVar);
		//-----------------------------------------------------------------

		break;

	    } // comDet >=76
	} // end of HpidPartcle loop
    } // case there has been no pair found
}

// -----------------------------------------------------------------------------
Int_t
HPidSingleLeptonEff::checkSingleTrack(HPidParticleSim* part,
                                      Int_t            lepTrackNb,
                                      Bool_t&          isHiddenLepton)
{
    // Checks if the track euqal to the GEANT track, is primary, has lepton PID
    // and PID == GEANT PID. Return the most common detector correlation if successfull
    // or 0 if not. If the most common correlation misses the META ( < 76) and the
    // track was going to system 0 (SHOWER), we try to find if the lepton track
    // was hidden by a secondary. If so, the in commonCorrelation the META bit is set
    // and the isHiddenLepton flag becomes true.

    Int_t comDet     = 0;
    Int_t first      = -1;
    HGeantKine* kine = NULL;

    kine = (HGeantKine*) pGeantCat->getObject(lepTrackNb - 1);
    if(part->getGeantTrackSet()->getGeantTrackID()          == lepTrackNb &&   // same track number
       part->getGeantTrackSet()->getGeantParentTrackID()    == 0          &&   // primary
       part->getHitData()->getRingCorrelation(4)            == kTRUE      &&   // RK-RICH correlation
       (part->getPid() == 2 || part->getPid() == 3)                       &&   // lepton PID
       kine->getID() == part->getPid()                                         // PID == GEANT PID
      )
    {
        comDet = part->getGeantTrackSet()->getMostCommonCorrelation();

	if( comDet < 76 && part->getSystem() == 0 )
        {
	    first = findFirstHitInShower(lepTrackNb);
	    if( first == lepTrackNb )
            {
                comDet = comDet | kSeenInMETA; // set META bit
                isHiddenLepton = kTRUE;
	    } else
            {
                isHiddenLepton = kFALSE;
	    }
	}
	return comDet;
    } else
    {
	return 0;
    }

}

// -----------------------------------------------------------------------------
Int_t
HPidSingleLeptonEff::checkTrackId(HPairSim* p,
                                  Int_t     lepTrackNb,
                                  Int_t&    whichPart,
                                  Bool_t&   isHiddenLepton)
{
    // check if the searched track is contained in the pair
    // allow pairs with geant hadrons or geant secondary leptons
    // pairs with embedded leptons are rejected. Checks the
    // reconstructed PID for leptons and check reconstructed PID == GEANT PID
    // Int_t whichPart will be -1 if no track was accepted and 1
    // or 2 if particle 1 or 2 of the pair was accepted.

    HPidParticleSim* part   = NULL;
    HPairGeantData   pg(p);
    Int_t            comDet = 0;
    whichPart               = -1;
    isHiddenLepton          = kFALSE;

    part   = (HPidParticleSim*) pPartCat->getObject(p->getIndexParticle1());
    comDet = checkSingleTrack(part, lepTrackNb, isHiddenLepton);
    if ( comDet > 0 )
    { // track == lepTrackNb, lepton PID, primary, PID == rec PID
	if ( (pg.getParentTrackNumber2() == 0 && 
              (pg.getGPid2() != 2 && pg.getGPid2() != 3)) || // generator hadron or
             (pg.getParentTrackNumber2() != 0 ))             // secondary lepton (decay in GEANT) or real tracks
	{
	    whichPart = 1;
	    return comDet;
	}
	else return 0;                             // reject pairs from embedded leptons
    }

    part = (HPidParticleSim*) pPartCat->getObject(p->getIndexParticle2());
    comDet = checkSingleTrack(part,lepTrackNb,isHiddenLepton);
    if ( comDet > 0 )
    { // track == lepTrackNb, lepton PID, primary, PID == rec PID
	if ( (pg.getParentTrackNumber1() == 0 && 
              (pg.getGPid1() != 2 && pg.getGPid1() != 3)) || // generator hadron or
             (pg.getParentTrackNumber1() != 0 ))             // secondary lepton (decay in GEANT) or real tracks
	{
	    whichPart = 2;
	    return comDet;
	}
	else return 0;                             // reject pairs from embedded leptons
    }
    return 0;
}

// -----------------------------------------------------------------------------
void
HPidSingleLeptonEff::calculateTrackProperties(HPairSim* pair,
                                              Float_t&  th,
                                              Float_t&  ph,
                                              Float_t&  p,
                                              Int_t     whichParticle)
{
    // calculates the pair theta, phi (in deg!!!)
    // Close pair candidates the angles to the closest
    // neighbours are stored separately

    HPairGeantData pg(pair);
    HPairData pd(pair);

    if ( 1 == whichParticle && 0 == pg.getParentTrackNumber1() )
    {
        th = pd.getThetaDeg1();
        ph = pd.getPhiDeg1();
        p  = pd.getMom1();
    }
    else if ( 2 == whichParticle && 0 == pg.getParentTrackNumber2() )
    {
        th = pd.getThetaDeg2();
        ph = pd.getPhiDeg2();
	p  = pd.getMom2();

    } else
    {
       Error("calculateTrackProperties()","Called with not identified Particle number!");
    }

}

// -----------------------------------------------------------------------------
Bool_t
HPidSingleLeptonEff::isGeantTrackInAcceptance(HGeantKine *pG)
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

// -----------------------------------------------------------------------------
Int_t
HPidSingleLeptonEff::findFirstHitInShower(Int_t numTrack)
{
    // find the first track ID entering the SHOWER
    // Used to suppress the secondaries created in the
    // SHOWER itself. Returns the input track number
    // if no previous GEANT hit in shower is found.

    Int_t       first     = -1;
    Int_t       sector    = -1;
    Int_t       tempTrack = numTrack;
    HGeantKine* kine      = NULL;

    if( numTrack <= 0 )
       return numTrack; // nothing to do for real data

    kine = (HGeantKine*) pGeantCat->getObject(numTrack - 1);
    if( kine->getParentTrack() == 0 )
    { // nothing to do
       return numTrack;
    }

    sector = kine->getSector();

    while( 0 != (kine = kine->getParent(tempTrack, (HLinearCategory*)pGeantCat)) )
    {
	if( -1 != (first = kine->getFirstShowerHit()) )
        {
	    HGeantShower* gshr = (HGeantShower*) pGeantCat->getObject(first);

	    if( gshr->getSector() == sector )
            {
               tempTrack = kine->getTrack();
            }
	    else
            {
               break;
            }
	} else
        {
           break;
        }
    }
    return tempTrack;
}

// -----------------------------------------------------------------------------
Bool_t
HPidSingleLeptonEff::finalize()
{
    out->cd();
    simYieldEle->Write();
    simYieldPos->Write();
    effEle     ->Write();
    effPos     ->Write();

    acceEle->Write();
    accePos->Write();
    out->Close();

    return kTRUE;
}

// -----------------------------------------------------------------------------
Float_t
HPidSingleLeptonEff::transformPhi(Float_t Phi)
{

   Float_t dPhi;

   if( (dPhi = TMath::RadToDeg() * Phi) < 0.0 )
      dPhi += 360.0;

   return dPhi;
}
