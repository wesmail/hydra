// Modified in Aug 2007 by W. Przygoda (Cracow)
// 
// This task is used to generate ntuples of embedded white tracks 
// with white distributions in p, theta, phi.
// There are ntuples with such particles in the acceptance and 
// additionally identified and accepted particles.
// These ntuples are later divided by each other (external code)
// to get (p,theta,phi) distributions of reconstruction efficiency
 

#define WHITE_TRACKS_NUMBER 6


#include <iostream>
#include "hpidsingletrackeff.h"
#include "hades.h"
#include "hevent.h"
#include "piddef.h"
#include "hrichhit.h"
#include "hmdcseg.h"
#include "hgeantkine.h"
#include "TH1.h"
#include "TH3.h"
#include "TMath.h"
#include "hgeomvector2.h"
#include "hgeomvector.h"
#include "hpidfl.h"
#include "hruntimedb.h"
#include "hpairgeantdata.h"
#include "hpidparticlesim.h"

ClassImp(HPidSingleTrackEff)
// -----------------------------------------------------------------------------
HPidSingleTrackEff::HPidSingleTrackEff(const Char_t * file)
                    : HReconstructor("PidPartFiller",
                                     "Filler of HPidParticle category")
{
    out = new TFile(file,"recreate");
    bookNTuple();

  
    
  
    //cout<<" kindOFPair  "<<kindOFPair.Data()<<endl;
}
// =============================================================================

// -----------------------------------------------------------------------------
HPidSingleTrackEff::~HPidSingleTrackEff(){

    /*delete simYield;
    delete eff;
    delete acc;*/

    delete out;

}
// =============================================================================

// -----------------------------------------------------------------------------
HPidSingleTrackEff::HPidSingleTrackEff(const Text_t name[],const  Text_t title[],const Char_t * file)
                    : HReconstructor(name,title)
{    
    out = new TFile(file,"recreate");
    bookNTuple();
   
}
// =============================================================================

// -----------------------------------------------------------------------------
void HPidSingleTrackEff::bookNTuple(){

    simYield = new TNtuple("simYield","simYield","p:theta:phi:pt");
    TString sVarList("pRecon:pGeant:thetaRecon:thetaGeant:"
		     "phiRecon:phiGeant:ptGeant:commonTrack:"
		     "angleHadron:hadronIsFitted:angleLepton:leptonIsFitted:"
		     "isRing:innerChi2:rkChi2:"
		     "metaMatchQality:metaMatchRK:"
		     "vertX:vertY:vertZ");

    eff = new TNtuple("eff","eff",sVarList);
    acc = new TNtuple("acc","acc","p:theta:phi:pt");

}
// =============================================================================

// -----------------------------------------------------------------------------
Bool_t HPidSingleTrackEff::init(void){

	cout << "INIT of HPidSingleTrackEff" << endl; 

  if((pInputCat = gHades->getCurrentEvent()->getCategory(catPidPart))
                == NULL)
    {
        Error("init", "No category catPidPart");
        return kFALSE;
    }

  pidPartInput = (HIterator *)pInputCat->MakeIterator();

    pGeantCat = gHades->getCurrentEvent()->getCategory(catGeantKine);
    if(!pGeantCat){
        Error("init", "No category catGeantKine ");
        return kFALSE;
    }
    pitGeant = (HIterator *)pGeantCat->MakeIterator();
    pitGeant1 = (HIterator *)pGeantCat->MakeIterator();
    
// --------- MDC GEANT category
    
    pMdcGeantCat = gHades->getCurrentEvent()->getCategory(catMdcGeantRaw);
    if(!pMdcGeantCat){
        Error("init", "No category catMdcGeantRaw ");
        return kFALSE;
    }
    pitMdcGeant = (HIterator *)pMdcGeantCat->MakeIterator();

// --------- TOF GEANT category

    pTofGeantCat = gHades->getCurrentEvent()->getCategory(catTofGeantRaw);
    if(!pTofGeantCat){
        Error("init", "No category catTofGeantRaw ");
        return kFALSE;
    }
    pitTofGeant = (HIterator *)pTofGeantCat->MakeIterator();

// --------- SHOWER GEANT category

    pShowerGeantCat = gHades->getCurrentEvent()->getCategory(catShowerGeantRaw);
    if(!pShowerGeantCat){
        Error("init", "No category catTofGeantRaw ");
        return kFALSE;
    }
    pitShowerGeant = (HIterator *)pShowerGeantCat->MakeIterator();

    //HRuntimeDb* rtdb = gHades->getRuntimeDb();
    //if (rtdb)   pC  = (HPairCutPar*)rtdb->findContainer("PairCutParameters");  

// ---------- EVENT HEADER category

    pEvtHeader = (HEventHeader*) gHades->getCurrentEvent()->getHeader();
    if (!pEvtHeader)
    {
        Error("HPairData::init(HPair*)","current event header not found");
        return kFALSE;
    }

    
  return kTRUE;
}
// =============================================================================

// -----------------------------------------------------------------------------
Int_t HPidSingleTrackEff::execute(void){

    HGeantKine *pKine;
   
    Int_t aParent, aMedium, aMechanism;
    Int_t aTrack, aID;
   
    Int_t * partIndex = new Int_t[WHITE_TRACKS_NUMBER];
    for(Int_t i = 0;i<WHITE_TRACKS_NUMBER;i++) partIndex[i] = 0;
    Int_t aCount = 0;

    removecloseTracks(partIndex);

    pitGeant->Reset();
    while((pKine = (HGeantKine*) pitGeant->Next()) != NULL){

	 pKine->getParticle(aTrack, aID);
	 pKine->getCreator(aParent, aMedium, aMechanism);
	 if(aParent==0) {
	     Float_t xMom,yMom,zMom;
	     TVector3 pD;
	     pKine->getMomentum(xMom,yMom,zMom);
	     pD.SetXYZ(xMom,yMom,zMom);
	     simYield->Fill(pD.Mag(),pD.Theta(),pD.Phi(),pD.Pt()); 
	     if (partIndex[aCount]==0) checkEff(pKine);
	     ++aCount;
	 }
    }
    
    delete [] partIndex;
    return 0;
}
// =============================================================================

// -----------------------------------------------------------------------------
void  HPidSingleTrackEff::removecloseTracks(Int_t *p){

    // all the tracks coming fomr the white generator that has a neighbour
    // also coming from the white generator that is closer than 9 deg are tagged
    HGeantKine *pKine=NULL;
    HGeantKine *pKine1=NULL;
    pitGeant->Reset();
    
    Int_t aTrack,aID,aTrack1,aID1;
    Int_t  aParent,aMedium,aMechanism,aParent1,aMedium1,aMechanism1;
	Int_t aCount = -1;

    while((pKine = (HGeantKine*) pitGeant->Next()) != NULL){

	pKine->getParticle(aTrack,aID);
	pKine->getCreator(aParent,aMedium,aMechanism);

	 if(aParent==0){
		++aCount;
	    //pitGeant1->Reset();
	    pitGeant1 = pitGeant;
	    while((pKine1 = (HGeantKine*) pitGeant1->Next()) != NULL){
		pKine1->getParticle(aTrack1,aID1);
		pKine1->getCreator(aParent1,aMedium1,aMechanism1);
		   if(aParent1==0 && aTrack != aTrack1) {
		      if (calcOpeningAngleKine(pKine,pKine1)<9.5){
			     p[aCount] = 1;
		      }
		   }
	    }
	 }
    }
    
}
// =============================================================================

// -----------------------------------------------------------------------------
Float_t HPidSingleTrackEff::calcOpeningAngleKine(HGeantKine *kine1,HGeantKine *kine2)
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
    //cout<<cosfOpeningAngle<<endl;
    Float_t fOpeningAngle=0.;
    if (TMath::Abs(cosfOpeningAngle) <= 1) 
	fOpeningAngle=TMath::ACos(cosfOpeningAngle) * rad2deg;
    
    return fOpeningAngle;
}
// =============================================================================

// -----------------------------------------------------------------------------
void HPidSingleTrackEff::checkEff(HGeantKine *p1){
    Float_t mom1;
    mom1 = 0;
    Int_t trackNb, id;
    TVector3 pD1;
    Float_t xMom1,yMom1,zMom1;
    p1->getMomentum(xMom1,yMom1,zMom1);
   
    p1->getParticle(trackNb,id);
    pD1.SetXYZ(xMom1,yMom1,zMom1);
    mom1 = TMath::Sqrt( xMom1*xMom1 + yMom1*yMom1 + zMom1*zMom1);
    
     if(isGeantTrackInAcceptance(p1)) {
	    acc->Fill(mom1,pD1.Theta(),pD1.Phi(),pD1.Pt());
     }

     // checking
     Float_t thetaExp, phiExp, momExp;
     thetaExp = phiExp = momExp = 0.;
     
	   const HPidHitData *pHitData = 0;
	   const HPidTrackData *pTrackData = 0;

       HPidParticleSim *part = 0;

       pidPartInput->Reset();
       while(( part = ( HPidParticleSim *)pidPartInput->Next()) != 0)
	 {
                pHitData = part->getHitData();
		pTrackData = part->getTrackData();
		Float_t isRing = static_cast<Float_t>( pHitData->getRingCorrelation(4) );

		Float_t hadrFitted = pTrackData->getAngleWithClosestCandidate(-1, 1);
		Float_t hadrNonFitted = pTrackData->getAngleWithClosestCandidate(-1, -1);
		Float_t leptFitted = pTrackData->getAngleWithClosestCandidate(1, 1);
		Float_t leptNonFitted = pTrackData->getAngleWithClosestCandidate(1, -1);
		
		// fix for hydra before 8.00a version
		//Float_t hadrFitted = pTrackData->getAngleWithClosestHadronCandidate();
		//Float_t leptFitted = pTrackData->getAngleWithClosestLeptonCandidate();
		//Float_t leptNonFitted = static_cast<Float_t>( pTrackData->getClosestLeptonCandidateIsFitted() );
		//Float_t hadrNonFitted = static_cast<Float_t>( pTrackData->getClosestHadronCandidateIsFitted() );
		Float_t innerChi = pHitData->getInnerMdcChiSquare();
		Float_t rkChi = pTrackData->getRKChiSquare();
		Float_t metaMatchQuality = pTrackData->getMetaMatchingQuality();
		Float_t mmRK = TMath::Sqrt( pTrackData->getRkMetadx()*pTrackData->getRkMetadx() +
			       pTrackData->getRkMetady()*pTrackData->getRkMetady() +
			       pTrackData->getRkMetadz()*pTrackData->getRkMetadz() );
		HVertex& vertex = pEvtHeader->getVertex();
		Float_t xVert = vertex.getX();
		Float_t yVert = vertex.getY();
		Float_t zVert = vertex.getZ();
		 
	   if(trackNb==part->getGeantTrackSet()->getGeantTrackID()&&
	      part->getGeantTrackSet()->getGeantParentTrackID()==-1&&
	      part->getGeantTrackSet()->getMostCommonCorrelation()>=74){
	       Float_t aVar[] = {
		   part->P(),
		   mom1,
		   part->Theta(),
		   pD1.Theta(),
		   part->Phi(),
		   pD1.Phi(),
		   pD1.Pt(),
		   part->getGeantTrackSet()->getMostCommonCorrelation(),
		   hadrFitted,
                   hadrNonFitted,
		   leptFitted,
		   leptNonFitted,
		   isRing,
		   innerChi,
		   rkChi,
		   metaMatchQuality,
		   mmRK,
		   xVert,yVert,zVert
	       };
	     eff->Fill(aVar);
	     break;  
	   }
	 }
     
}
// =============================================================================

// -----------------------------------------------------------------------------
Bool_t HPidSingleTrackEff::isGeantTrackInAcceptance(HGeantKine *pG){
     pG->getParticle(lTrack,lId);

     nStatMDC1 = nStatMDC2 = nStatMDC3 = nStatMDC4 = 0;
     nStatTof = nStatShower = 0;
     HGeantMdc *pMdc;
     pitMdcGeant->Reset();
     while((pMdc = (HGeantMdc*) pitMdcGeant->Next()) != NULL){

             if (pMdc->getTrack() == lTrack)
             {
                switch (((Int_t)pMdc->getModule()))
                {
                    case 0: nStatMDC1 = 1;
                            break;
                    case 1: nStatMDC2 = 1;
                            break;
                    case 2: nStatMDC3 = 1;
                            break;
                    case 3: nStatMDC4 = 1;
                            break;
                    default: cerr << "WRONG MDC module number!" << endl;
                }
             }
     }

     HGeantTof *pTof;
     pitTofGeant->Reset();
     while((pTof = (HGeantTof*) pitTofGeant->Next()) != NULL){

            if (pTof->getTrack() == lTrack)
            {
               nStatTof = 1;
            }
     }

     HGeantShower *pShower;
     pitShowerGeant->Reset();
     while((pShower = (HGeantShower*) pitShowerGeant->Next()) != NULL){

            if (pShower->getTrack() == lTrack)
            {
               nStatShower = 1;
            }

     }

     if (nStatMDC1 && nStatMDC2 && nStatMDC3 && nStatMDC4 && (nStatTof || nStatShower))
       return kTRUE;

 return kFALSE;
}
// =============================================================================

// -----------------------------------------------------------------------------
Bool_t  HPidSingleTrackEff::finalize(){
    cout<<" finalize "<<endl;
    
    out->cd();

    simYield->Write();
    acc->Write();
    eff->Write();

    out->Close();
    
    return kTRUE;   
}
// =============================================================================
