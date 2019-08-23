//*-- Author : M. Wisniowski
//*-- Modified : 2005-9-13
////////////////////////////////////////////////////////////////////////
//
// HHypPEpEmProjector
//
// HHypPEpEmProjector projects any PP data. At the moment output contains
//
////////////////////////////////////////////////////////////////////////

 /**
    Ntuple content tescription
    sector_p  -  sector of first proton
    sector_e1 -  sector of first lepton
    sector_e2 -  sector of secound lepton
    system_p -  system of first proton (shower-tofino->0, tof->1)
    system_e1 -  system of first lepton
    system_e2 -  system of secound lepton
    P_p       -  momentum [MeV] of first proton
    Th_p      -  theta angle [rad] of first proton
    Ph_p      -  phi angle [rad] of secound proton
    q_e1      -  charge of first lepton
    P_e1      -  momentum [MeV] of first lepton
    Th_e1     -  theta angle [rad] of first lepton
    Ph_e1     -  phi angle [rad] of first lepton
    q_e2      -  charge of secound lepton
    P_e2      -  momentum [MeV] of secound lepton
    Th_e2     -  theta angle [rad] of secound lepton
    Ph_e2     -  phi angle [rad] of secound lepton
    r_e1      -  the smallest distance between track and z-axis (z_p piont) (first lepton)
    z_e1      -  z vertex of first lepton track
    r_e2      -  the smallest distance between track and z-axis (z_p piont) (secound lepton)
    z_e2      -  z vertex of secound lepton track
    z_p       -  z vertex of proton track (first proton)
    r_p       -  the smallest distance between track and z-axis (z_p piont) (first proton)
    xvert_ee   -  x component of the vertex calculated from e-e pair
    yvert_ee   -  y component of the vertex calculated from e-e pair
    zvert_ee   -  z component of the vertex calculated from e-e pair
    dist_ee    -  closet distance between e-e tracks
    m2_miss   -  squerd missing mass of two protons
    p_miss    -  missing momentum of two protons
    m2_miss_pe1e2 - four particle squerd missing mass (p p e e)
    p_miss_pe1e2  - four particle squerd missing momentum (p p e e)
    opAngle_e1e2 - e-e pair openining angle [deg]
    m2_inv_e1e2  - invariant mass of e-e pair [MeV/c2]
    tanT1tanT2   - tan()*tan()       -> for el. scattering
    cosThCM_miss - cos(Theta) of missing particle
    RKchiq_p  -  Runge Kutta chi squerd of first proton
    RKchiq_e1  -  Runge Kutta chi squerd of first lepton
    RKchiq_e2  -  Runge Kutta chi squerd of secound lepton
    ncomb      -  number of track-track combination in the event
    dsf        -  down-scale factor
    triggerBit -  tells what kind of trigger was used (trigger box setting)
    triggerDecision - secound level trigger decision
    date       -  date when file was taken
    time       -  time when file was taken
    InnerMDCchiq_p - inner MDC chi squerd of first proton
    InnerMDCchiq_e1 - inner MDC chi squerd of first lepton
    InnerMDCchiq_e2 - inner MDC chi squerd of secound lepton
    OpAngleClosestLept_e1 - opening angle between first lepton and other closest lepton
                            > 0 if closest lepton fitted
                            < 0 if closest lepton not fitted (multipl. by -1)
    OpAngleClosestHadr_e1 - opening angle between first lepton and other closest hadron
                            > 0 if closest hadron fitted
                            < 0 if closest hadron not fitted (multipl. by -1)
    OpAngleClosestLept_e2 - opening angle between secound lepton and other closest lepton
                            > 0 if closest lepton fitted
                            < 0 if closest lepton not fitted
    OpAngleClosestHadr_e2 - opening angle between secound lepton and other closest hadron
                            > 0 if closest hadron fitted
                            < 0 if closest hadron not fitted
    OpAngleClosestLept_p - opening angle between first proton and other closest lepton
                            > 0 if closest lepton fitted
                            < 0 if closest lepton not fitted (multipl. by -1)
    OpAngleClosestHadr_p - opening angle between first proton and other closest hadron
                            > 0 if closest hadron fitted
                            < 0 if closest hadron not fitted (multipl. by -1)

    **/



using namespace std;

#include "hhypPEpEmProjector.h"
#include "hgeomvector.h"
#include "hgeomvertexfit.h"

#define DEBUG 0

// well known constants should not be defined
#define c 0.299792
#define D2R 0.0174532925199432955
#define R2D 57.2957795130823229
#define P_mass 938.272309999999998
#define E_mass 0.51099906000000002


ClassImp(HHypPEpEmProjector)

HHypPEpEmProjector::HHypPEpEmProjector(Char_t *name_i, Option_t par[])
:HHypBaseAlgorithm(name_i,par)
{
    simuflag = 0;
}

HHypPEpEmProjector::~HHypPEpEmProjector()
{
}

Bool_t HHypPEpEmProjector::execute()
{
    Int_t icomb=0;
    Float_t deltaTof= 100000000;
    Int_t BestComb=-1;
    mylist->CombIteratorReset();
    while (mylist->CombIterator())
    {
       Float_t deltaTof_tmp;
       mylist->getUserValue(DELTATOF_CHI2, deltaTof_tmp);
       if( deltaTof_tmp < deltaTof )
       {
         deltaTof = deltaTof_tmp;
         BestComb = icomb;
       }
       icomb++;
    }


    Short_t triggerBit = gHades->getCurrentEvent()->getHeader()->getTBit();
    HEventHeader *evHeader = gHades->getCurrentEvent()->getHeader();
    HVertex vertex = gHades->getCurrentEvent()->getHeader()->getVertex();

	UInt_t date = evHeader->getDate();
	UInt_t time = evHeader->getTime();
    UInt_t dsf = evHeader->getDownscaling();
    UInt_t triggerDecision = evHeader->getTriggerDecision();

    if (!beam)
    {
        cerr << algoName << " needs beam particle! " << endl;
        return kFALSE;
    }

	Float_t r_e1=0,z_e1=0,r_e2=0,z_e2=0,r_p=0,z_p=0;
    Float_t q_e1=0, q_e2=0;
    Float_t RKchiq_p=-10, RKchiq_e1=-10, RKchiq_e2=-10;     // RK chi2 for track1 & track2
    Short_t sector_p=-1, system_p=-1; // number of sector for first and secound particle
    Short_t sector_e1=-1, sector_e2=-1, system_e1=-1, system_e2=-1; // number of sector for e+ e-
	Float_t InnerMDCchiq_p=-10;
	Float_t InnerMDCchiq_e1=-10, InnerMDCchiq_e2=-10;
	Float_t OpAngleClosestLept_e1=-10, OpAngleClosestHadr_e1=-10;
	Float_t OpAngleClosestLept_e2=-10, OpAngleClosestHadr_e2=-10;
	Float_t OpAngleClosestLept_p=-10, OpAngleClosestHadr_p=-10;

    Int_t geant_grandparentID_p = -10, geant_parentID_p = -10, geantID_p = -100;
    Int_t geant_grandparentID_e1 = -10, geant_parentID_e1 = -10, geantID_e1 = -100;
    Int_t geant_grandparentID_e2 = -10, geant_parentID_e2 = -10, geantID_e2 = -100;
    Float_t geninfo1_p=-10, geninfo2_p=-10;
    Float_t geninfo1_e1=-10, geninfo2_e1=-10;
    Float_t geninfo1_e2=-10, geninfo2_e2=-10;
    Bool_t IsCleanE1=0;
    Bool_t IsCleanE2=0;



    // Resetting the list and start looping over the combinations
    // Loop is only done over the VALID combinations

    mylist->CombIteratorReset();
    Bool_t IsBestComb=0;
    Int_t ncomb=mylist->getNcomb();
    icomb=0;
    while (mylist->CombIterator())
    {
	  if(mylist->getProbAlg(icomb)<=0) continue;

        if( icomb == BestComb ) IsBestComb = 1;
        else IsBestComb = 0;

        TLorentzVector proton(0,0,0,0);
        TLorentzVector positron(0,0,0,0);    // in case of ppe-e- / ppe+e+ plays role of e-
        TLorentzVector electron(0,0,0,0);
        TLorentzVector positronRICH(0,0,0,0); // keep Theta and Phi taken from RICH
        TLorentzVector electronRICH(0,0,0,0);
        TLorentzVector gamma(0,0,0,0);
        TLorentzVector geant_p(0,0,0,0);
        TLorentzVector geant_e1(0,0,0,0);
        TLorentzVector geant_e2(0,0,0,0);
        TVector3 pp_vertex;
        TVector3 pp_distance;
        TVector3 ee_vertex;
        TVector3 ee_distance;
        Float_t  dist_ee=100;


        if (mylist->getIterStatus() == kTRUE)
        {
            HPidTrackCand  *PidTrackCand[4];
            const HPidHitData   *PidData = NULL;
            const HPidTrackData *pTrack = NULL;

            //-------------------- simulation ----------------------------------------------
            if (simuflag == 1 )
            {

                HPidTrackCandSim *my_p = (HPidTrackCandSim *) CatTrackCandSim->
                                                getObject(mylist->getIdxPidTrackCand(icomb, 0));
                HPidTrackCandSim *my_e1 = (HPidTrackCandSim *) CatTrackCandSim->
                                                getObject(mylist->getIdxPidTrackCand(icomb, 2));
                HPidTrackCandSim *my_e2 = (HPidTrackCandSim *) CatTrackCandSim->
                                                getObject(mylist->getIdxPidTrackCand(icomb, 3));

                const HPidGeantTrackSet *p1GeantSet = my_p->getGeantTrackSet();
                const HPidGeantTrackSet *e1GeantSet = my_e1->getGeantTrackSet();
                const HPidGeantTrackSet *e2GeantSet = my_e2->getGeantTrackSet();

                geninfo1_p             = p1GeantSet->getGenInfo1();
                geninfo2_p             = p1GeantSet->getGenInfo2();
                geantID_p              = p1GeantSet->getGeantPID();
                geant_parentID_p       = p1GeantSet->getGeantParentID();
                geant_grandparentID_p  = p1GeantSet->getGeantGrandParentID();
                TVector3                  v1(p1GeantSet->getGeantMomX(), p1GeantSet->getGeantMomY(), p1GeantSet->getGeantMomZ());
                geant_p.SetVectM(v1,P_mass);

                geninfo1_e1             = e1GeantSet->getGenInfo1();
                geninfo2_e1             = e1GeantSet->getGenInfo2();
                geantID_e1              = e1GeantSet->getGeantPID();
                geant_parentID_e1       = e1GeantSet->getGeantParentID();
                geant_grandparentID_e1  = e1GeantSet->getGeantGrandParentID();
                TVector3                  v2(e1GeantSet->getGeantMomX(), e1GeantSet->getGeantMomY(), e1GeantSet->getGeantMomZ());
                geant_e1.SetVectM(v2,P_mass);

                geninfo1_e2             = e2GeantSet->getGenInfo1();
                geninfo2_e2             = e2GeantSet->getGenInfo2();
                geantID_e2              = e2GeantSet->getGeantPID();
                geant_parentID_e2       = e2GeantSet->getGeantParentID();
                geant_grandparentID_e2  = e2GeantSet->getGeantGrandParentID();
                TVector3                  v3(e2GeantSet->getGeantMomX(), e2GeantSet->getGeantMomY(), e2GeantSet->getGeantMomZ());
                geant_e2.SetVectM(v3,P_mass);

            }  // simuflag
            //-------------------- simulation end -------------------------------------------

            HCategory *pidTrackCandCat =  gHades->getCurrentEvent()->getCategory(catPidTrackCand);

            if (pidTrackCandCat != NULL )
            {
                for(Int_t ipar=0; ipar<3; ipar++)
                {
                    PidTrackCand[ipar] = (HPidTrackCand *) pidTrackCandCat->getObject(mylist->getIdxPidTrackCand(icomb, ipar));
                    if (PidTrackCand[ipar] != NULL)
                    {
                        PidData        = PidTrackCand[ipar]->getHitData();
                        pTrack         = PidTrackCand[ipar]->getTrackData();

						Float_t closestLepton  = pTrack->getAngleWithClosestLeptonCandidate()
						                                   *(int(pTrack->closestLeptonCandidateIsFitted())-0.5)*2;
														   // Fitted tracks positive , not fitted negative value of opening angle
						Float_t closestHadron  = pTrack->getAngleWithClosestHadronCandidate()
						                                   *(int(pTrack->closestHadronCandidateIsFitted())-0.5)*2;
														   // Fitted tracks positive , not fitted negative value of opening angle
                        Int_t   system = PidData->iSystem;
                        Bool_t    isClean= PidTrackCand[ipar]->isFlagBit(HPidTrackCand::kIsUsed);
                        Float_t InnerMDCchiq = PidData->fInnerMdcChiSquare;
						Int_t   sector = PidData->getSector();
						Int_t q        = pTrack->getPolarity(4);
                        Float_t P      = pTrack->fMomenta[4];
                        Float_t Th     = D2R*pTrack->getRKTheta();
                        Float_t Ph     = D2R*pTrack->getRKPhi();
						Float_t Th_rich= D2R*PidData->getRichTheta();
						Float_t Ph_rich= D2R*PidData->getRichPhi();
                        Float_t R      = pTrack->getTrackR(4);
                        Float_t Z      = pTrack->getTrackZ(4);
                        Float_t RKchiq = pTrack->getRKChiSquare();

                        switch(ipar)   // Energy loss correction
                        {
                            case 0:  { P = enLossCorr.getCorrMom(14 ,P,Th*TMath::RadToDeg()); }
                            case 1:  { if(q==1) P = enLossCorr.getCorrMom(2 ,P,Th*TMath::RadToDeg());
                                       else     P = enLossCorr.getCorrMom(3 ,P,Th*TMath::RadToDeg());  break; }
                            case 2:  { if(q==1) P = enLossCorr.getCorrMom(2 ,P,Th*TMath::RadToDeg());
                                       else     P = enLossCorr.getCorrMom(3 ,P,Th*TMath::RadToDeg()); break;  }
                            default: cerr<<"ERROR : HHypPEpEmProjector::execute() "<<ipar<<endl;
                        }

                        TVector3 p, p_rich;
                        p.SetXYZ(P*sin(Th)*cos(Ph),P*sin(Th)*sin(Ph),P*cos(Th));
                        p_rich.SetXYZ(P*sin(Th_rich)*cos(Ph_rich),P*sin(Th_rich)*sin(Ph_rich),P*cos(Th_rich));

                        switch(ipar)
                        {
                            case 0:  {  proton.SetVectM(p,P_mass);
							            system_p=system; sector_p=sector;
                                        RKchiq_p=RKchiq; r_p=R; z_p=Z; InnerMDCchiq_p=InnerMDCchiq;
	                                    OpAngleClosestLept_p=closestLepton; OpAngleClosestHadr_p=closestHadron; break; }
                            case 1:  { positron.SetVectM(p,E_mass); positronRICH.SetVectM(p_rich,E_mass);
							            system_e1=system; sector_e1=sector;
                                        RKchiq_e1=RKchiq; q_e1=q; r_e1=R; z_e1=Z; InnerMDCchiq_e1=InnerMDCchiq;
	                                    OpAngleClosestLept_e1=closestLepton; OpAngleClosestHadr_e1=closestHadron; 
                                        IsCleanE1=isClean; break; }
                            case 2:  { electron.SetVectM(p,E_mass); electronRICH.SetVectM(p_rich,E_mass);
							            system_e2=system; sector_e2=sector;
                                        RKchiq_e2=RKchiq; q_e2=q; r_e2=R; z_e2=Z; InnerMDCchiq_e2=InnerMDCchiq;
	                                    OpAngleClosestLept_e2=closestLepton; OpAngleClosestHadr_e2=closestHadron; 
                                        IsCleanE1=isClean; break; }
                            default: cerr<<"ERROR : HHypPEpEmProjector::execute() "<<ipar<<endl;
                        }
                    }
                }
                //----------------------------- vertex calculation ------------------------------------------------
                dist_ee =  calcVertex( PidTrackCand[1], PidTrackCand[2], &ee_vertex, &ee_distance);
                //----------------------------- vertex calculation -------- end -----------------------------------
            }

            TLorentzVector miss = (*beam) - proton ;  // beam = beam + target
            TLorentzVector miss_p = (*beam) - proton ;  // beam = beam + target
            TLorentzVector miss_pee = (*beam) - proton - positron - electron ;  // beam = beam + target
            TLorentzVector inv_pee = proton + positron + electron ;  // beam = beam + target

            // 4vectors in CM system
            TLorentzVector miss_cm = miss;
            TLorentzVector proton_cm = proton;
            miss_cm.Boost(0.0, 0.0, -(*beam).Beta() );
            proton_cm.Boost(0.0, 0.0, -(*beam).Beta() );


            Float_t op_angle_e1e2 = TMath::ACos((positron.Px()*electron.Px() + positron.Py()*electron.Py() +
                        positron.Pz()*electron.Pz())/
                        sqrt(positron.Px()*positron.Px() + positron.Py()*positron.Py() +
	                               	       positron.Pz()*positron.Pz() )/
                        sqrt(electron.Px()*electron.Px() + electron.Py()*electron.Py() +
		                                   electron.Pz()*electron.Pz() ))*
			            TMath::RadToDeg();
            if(DEBUG)
            {
                cerr<<" op_angle_e1e2 : "<<op_angle_e1e2<<endl;
                cerr<<" electron.E() : "<<electron.E()<<"    electron.M() :"<<electron.M()<<endl;
                cerr<<" positron.E() : "<<positron.E()<<"    positron.M() :"<<positron.M()<<endl;
            }

            if(simuflag==1)
            {
                Float_t fpp[]={ sector_p, sector_e1, sector_e2,
                                system_p, system_e1, system_e2,
                                proton.P(), proton.Theta(), proton.Phi(),
                                q_e1, positron.P(), positron.Theta(), positron.Phi(),
                                q_e2, electron.P(), electron.Theta(), electron.Phi(),
								r_e1, z_e1, r_e2, z_e2,
								r_p, z_p,
                                ee_vertex.X(), ee_vertex.Y(), ee_vertex.Z(), dist_ee,
                                geant_p.P(), geant_p.Theta(), geant_p.Phi(),
                                geant_e1.P(), geant_e1.Theta(), geant_e1.Phi(),
                                geant_e2.P(), geant_e2.Theta(), geant_e2.Phi(),
                                miss.M2(), miss.P(),
                                (miss - electron - positron).M2(), (miss - electron - positron).P(), op_angle_e1e2,
                                (electron+positron).M2(), miss_cm.CosTheta(),
                                RKchiq_p, RKchiq_e1, RKchiq_e2,
                                ncomb, dsf, triggerBit, triggerDecision,
                                geantID_p, geant_parentID_p, geant_grandparentID_p,
                                geantID_e1, geant_parentID_e1, geant_grandparentID_e1,
                                geantID_e2, geant_parentID_e2, geant_grandparentID_e2,
                                geninfo1_p, geninfo1_e1, geninfo1_e2,
                                geninfo2_p, geninfo2_e1, geninfo2_e2,
								InnerMDCchiq_p, InnerMDCchiq_e1, InnerMDCchiq_e2,
	                            OpAngleClosestLept_e1, OpAngleClosestHadr_e1,
	                            OpAngleClosestLept_e2, OpAngleClosestHadr_e2,
	                            OpAngleClosestLept_p, OpAngleClosestHadr_p
                              };

                pp->Fill(fpp);
            }
            else
            {
                Float_t fpp[]={ vertex.getX() , vertex.getY() ,vertex.getZ() ,vertex.getChi2() ,
				                sector_p, sector_e1, sector_e2,
                                system_p, system_e1, system_e2,
                                proton.P(), proton.Theta(), proton.Phi(),
                                q_e1, positron.P(), positron.Theta(), positron.Phi(),
                                positronRICH.Theta(), positronRICH.Phi(),
                                q_e2, electron.P(), electron.Theta(), electron.Phi(),
                                electronRICH.Theta(), electronRICH.Phi(),
								r_e1, z_e1, r_e2, z_e2,
								r_p, z_p,
                                ee_vertex.X(), ee_vertex.Y(), ee_vertex.Z(), dist_ee,
                                miss.M2(), miss.P(),
                                (miss - electron - positron).M2(), (miss - electron - positron).P(), op_angle_e1e2,
                                (electron+positron).M2(), miss_cm.CosTheta(),
                                RKchiq_p, RKchiq_e1, RKchiq_e2,
                                ncomb, dsf, triggerBit, triggerDecision, date, time,
								InnerMDCchiq_p, InnerMDCchiq_e1, InnerMDCchiq_e2,
	                            OpAngleClosestLept_e1, OpAngleClosestHadr_e1,
	                            OpAngleClosestLept_e2, OpAngleClosestHadr_e2,
	                            OpAngleClosestLept_p, OpAngleClosestHadr_p, IsCleanE1, IsCleanE2, IsBestComb
                              };

                pp->Fill(fpp);
            }
        } else
        cerr << algoName << " got no TLorentzVector " << endl;
        icomb++;
    }
    return kTRUE;
}

Bool_t HHypPEpEmProjector::init()
{
    enLossCorr.setDefaultPar("jan04");

    simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);

    if (!simCat)
    {
        simuflag = 0;
    }
    else
    {
        simuflag = 1;
        //cout << "Projector uses SIMULATION" << endl;

        CatTrackCandSim = NULL;          // Category

        if ((CatTrackCandSim =
            gHades->getCurrentEvent()->getCategory(catPidTrackCand)) == NULL) {
            Error("init", "Cannot get catPidTrackCandSim cat");
            return kFALSE;
        }
    }

    // need to get name from channel
    TString input(channel->Get(initList));
    TFile *f=GetHFile();
    f->cd();
    if(simuflag==1)
    {
        pp =  new TNtuple(input + TString("_pp"),

            "sector_p:sector_e1:sector_e2:system_p:system_e1:system_e2:P_p:Th_p:Ph_p:q_e1:P_e1:Th_e1:Ph_e1:q_e2:P_e2:Th_e2:Ph_e2:r_e1:z_e1:r_e2:z_e2:r_p:z_p:xvert_ee:yvert_ee:zvert_ee:dist_ee:gP_p:gTh_p:gPh_p:gP_e1:gTh_e1:gPh_e1:gP_e2:gTh_e2:gPh_e2:m2_miss:p_miss:m2_miss_pe1e2:p_miss_pe1e2:opAngle_e1e2:m2_inv_e1e2:cosThCM_miss:RKchiq_p:RKchiq_e1:RKchiq_e2:ncomb:dsf:triggerBit:triggerDecision:geantID_p:geant_parentID_p:geant_grandparentID_p:geantID_e1:geant_parentID_e1:geant_grandparentID_e1:geantID_e2:geant_parentID_e2:geant_grandparentID_e2:geninfo1_p:geninfo1_e1:geninfo1_e2:geninfo2_p:geninfo2_e1:geninfo2_e2:InnerMDCchiq_p:InnerMDCchiq_e1:InnerMDCchiq_e2:OpAngleClosestLept_e1:OpAngleClosestHadr_e1:OpAngleClosestLept_e2:OpAngleClosestHadr_e2:OpAngleClosestLept_p:OpAngleClosestHadr_p",
            "sector_p:sector_e1:sector_e2:system_p:system_e1:system_e2:P_p:Th_p:Ph_p:q_e1:P_e1:Th_e1:Ph_e1:q_e2:P_e2:Th_e2:Ph_e2:r_e1:z_e1:r_e2:z_e2:r_p:z_p:xvert_ee:yvert_ee:zvert_ee:dist_ee:gP_p:gTh_p:gPh_p:gP_e1:gTh_e1:gPh_e1:gP_e2:gTh_e2:gPh_e2:m2_miss:p_miss:m2_miss_pe1e2:p_miss_pe1e2:opAngle_e1e2:m2_inv_e1e2:cosThCM_miss:RKchiq_p:RKchiq_e1:RKchiq_e2:ncomb:dsf:triggerBit:triggerDecision:geantID_p:geant_parentID_p:geant_grandparentID_p:geantID_e1:geant_parentID_e1:geant_grandparentID_e1:geantID_e2:geant_parentID_e2:geant_grandparentID_e2:geninfo1_p:geninfo1_e1:geninfo1_e2:geninfo2_p:geninfo2_e1:geninfo2_e2:InnerMDCchiq_p:InnerMDCchiq_e1:InnerMDCchiq_e2:OpAngleClosestLept_e1:OpAngleClosestHadr_e1:OpAngleClosestLept_e2:OpAngleClosestHadr_e2:OpAngleClosestLept_p:OpAngleClosestHadr_p");
    }
    else
    {
        pp =  new TNtuple(input + TString("_pp"),
 "eVert_x:eVert_y:eVert_Z:eVert_chi2:sector_p:sector_e1:sector_e2:system_p:system_e1:system_e2:P_p:Th_p:Ph_p:q_e1:P_e1:Th_e1:Ph_e1:Th_rich_e1:Ph_rich_e1:q_e2:P_e2:Th_e2:Ph_e2:Th_rich_e2:Ph_rich_e2:r_e1:z_e1:r_e2:z_e2:r_p:z_p:xvert_ee:yvert_ee:zvert_ee:dist_ee:m2_missp:p_missp:m2_miss_pee:p_miss_pee:m2_pee:p_pee:opAngle_ee:m2_inv_ee:cosThCM_miss:RKchiq_p:RKchiq_e1:RKchiq_e2:ncomb:dsf:triggerBit:triggerDecision:date:time:InnerMDCchiq_p:InnerMDCchiq_e1:InnerMDCchiq_e2:OpAngleClosestLept_e1:OpAngleClosestHadr_e1:OpAngleClosestLept_e2:OpAngleClosestHadr_e2:OpAngleClosestLept_p:OpAngleClosestHadr_p:IsCleanE1:IsCleanE2:IsBestComb",
            "eVert_x:eVert_y:eVert_Z:eVert_chi2:sector_p:sector_e1:sector_e2:system_p:system_e1:system_e2:P_p:Th_p:Ph_p:q_e1:P_e1:Th_e1:Ph_e1:Th_rich_e1:Ph_rich_e1:q_e2:P_e2:Th_e2:Ph_e2:Th_rich_e2:Ph_rich_e2:r_e1:z_e1:r_e2:z_e2:r_p:z_p:xvert_ee:yvert_ee:zvert_ee:dist_ee:m2_missp:p_missp:m2_miss_pee:p_miss_pee:m2_pee:p_pee:opAngle_ee:m2_inv_ee:cosThCM_miss:RKchiq_p:RKchiq_e1:RKchiq_e2:ncomb:dsf:triggerBit:triggerDecision:date:time:InnerMDCchiq_p:InnerMDCchiq_e1:InnerMDCchiq_e2:OpAngleClosestLept_e1:OpAngleClosestHadr_e1:OpAngleClosestLept_e2:OpAngleClosestHadr_e2:OpAngleClosestLept_p:OpAngleClosestHadr_p:IsCleanE1:IsCleanE2:IsBestComb");
    }
    //f->Close();
    return kTRUE;

}

Bool_t HHypPEpEmProjector::reinit()
{
    return kTRUE;
}

Bool_t HHypPEpEmProjector::finalize()
{
    pp->Write();
    return kTRUE;
}

Bool_t HHypPEpEmProjector::IsOpposit(Short_t sec1, Short_t sec2)
{
    if(sec1==0 && sec2==3) return 1; else
    if(sec1==1 && sec2==4) return 1; else
    if(sec1==2 && sec2==5) return 1; else
    if(sec1==3 && sec2==0) return 1; else
    if(sec1==4 && sec2==1) return 1; else
    if(sec1==5 && sec2==2) return 1; else
    return 0;
}

Bool_t HHypPEpEmProjector::SetParamFile(TString pFile)
{
    paramFile=pFile;
    return kTRUE;
}

Float_t HHypPEpEmProjector::calcVertex(HPidTrackCand *p1, HPidTrackCand *p2,
        TVector3 *vertex, TVector3 *distance)
{
// calcVertex should return
// 1. the vertex of two tracks (no weights included, so it returns the
//    center of closest approach vector)
// 2. a vector with direction and magnitude of the distance
//    (using stefanos algebra to calculate the magnitude, root cross product
//    to give the direction)

        HGeomVector hoff[2];
        HGeomVector hdir[2];
        HGeomVector hvertex;
        HGeomVertexFit hfitter;
        TVector3 dir[2];
        Float_t dist;
        Float_t det1, det2;

        // extract coordinates from p1, p2, fill them into HGeomVector
        // to use Manuels fitter
        hoff[0].setXYZ( p1->getTrackData()->getTrackR(4)*TMath::Cos(p1->getTrackData()->getRKPhi()*D2R
                                        + TMath::PiOver2()),
                                p1->getTrackData()->getTrackR(4)*TMath::Sin(p1->getTrackData()->getRKPhi()*D2R
                                         + TMath::PiOver2()),
                                p1->getTrackData()->getTrackZ(4));
        hoff[1].setXYZ( p2->getTrackData()->getTrackR(4)*TMath::Cos(p1->getTrackData()->getRKPhi()*D2R
                                        + TMath::PiOver2()),
                                p1->getTrackData()->getTrackR(4)*TMath::Sin(p1->getTrackData()->getRKPhi()*D2R
                                         + TMath::PiOver2()),
                                p2->getTrackData()->getTrackZ(4));
    
        dir[0].SetMagThetaPhi(p1->getTrackData()->getMomenta(4),p1->getTrackData()->getRKTheta()*D2R,
                              p1->getTrackData()->getRKPhi()*D2R);// = p1->Vect();
        dir[1].SetMagThetaPhi(p2->getTrackData()->getMomenta(4),p2->getTrackData()->getRKTheta()*D2R,
                              p2->getTrackData()->getRKPhi()*D2R);// = p2->Vect();

                hdir[0].setXYZ(dir[0].X(),dir[0].Y(),dir[0].Z());
        hdir[1].setXYZ(dir[1].X(),dir[1].Y(),dir[1].Z());

        hfitter.reset();
        for (Int_t i = 0; i < 2; i++) {
                hfitter.addLine(hoff[i],hdir[i]);
        }
        hfitter.getVertex(hvertex);
        vertex->SetXYZ(hvertex.getX(),hvertex.getY(),hvertex.getZ());
        // Function to calculate the distance between two lines in the space
        // c.f. Stefano
        det1 = (
                (hoff[0].getX()-hoff[1].getX()) *
                (hdir[0].getY()*hdir[1].getZ()-hdir[0].getZ()*hdir[1].getY()) -
                (hoff[0].getY()-hoff[1].getY()) *
                (hdir[0].getX()*hdir[1].getZ()-hdir[0].getZ()*hdir[1].getX()) +
                (hoff[0].getZ()-hoff[1].getZ()) *
                (hdir[0].getX()*hdir[1].getY()-hdir[0].getY()*hdir[1].getX())
        );

        det2 = TMath::Sqrt(
                (hdir[0].getX()*hdir[1].getY() - hdir[0].getY()*hdir[1].getX()) *
                (hdir[0].getX()*hdir[1].getY() - hdir[0].getY()*hdir[1].getX()) +
                (hdir[0].getX()*hdir[1].getZ() - hdir[0].getZ()*hdir[1].getX()) *
                (hdir[0].getX()*hdir[1].getZ() - hdir[0].getZ()*hdir[1].getX()) +
                (hdir[0].getY()*hdir[1].getZ() - hdir[0].getZ()*hdir[1].getY()) *
                (hdir[0].getY()*hdir[1].getZ() - hdir[0].getZ()*hdir[1].getY())
                );

        // Create a distance vector and scale it with dist
        *distance = dir[0].Cross(dir[1]);

        if (det2==0) {
                dist = -1.;
        } else {
                dist = TMath::Abs(det1/det2);
                distance->SetMag(dist);
        }
        return dist;
}

