//*-- Author : M. Palka
//*-- Modified : 2008-21-08
//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypKpKmProjector
//
// HHypKpKmProjector projects any P K+ K- data to the ntuple
//
////////////////////////////////////////////////////////////////////////

#warning "This code is not written in a way to be compatible with other hyp features"
#warning "Remark: This code is a 1:1 copy of HHypPKpKmProjector!!!!"
#warning "(which is a 'copy' of Marcins code)"
#warning "Problems: (which also refer to Marcins code)"
#warning "Momentum is not taken from Hyp list -> new momenta from kine fit or energy loss will not be used"
#warning "Energy loss correction is done in the projector, THSI SHOULD NOT BE DONE IN THAT WAY!!!"
#warning "as there is a seperate TASK for that"

	/**
	Ntuple content tescription
	sector_p1  -  sector of first proton
	sector_p2  -  sector of secound proton
	system_p1  -  system of first proton (shower-tofino->0, tof->1)
	system_p2  -  system of secound proton
	P_p1       -  momentum [MeV] of first proton
	Th_p1      -  theta angle [rad] of first proton
	Ph_p1      -  phi angle [rad] of secound proton
	P_p2       -  momentum [MeV] of secound proton
	Th_p2      -  theta angle [rad] of secound proton
	Ph_p2      -  phi angle [rad] of secound proton
	z_p1       -  z vertex of proton track (first proton)
	r_p1       -  the smallest distance between track and z-axis (z_p1 piont) (first proton)
	z_p2       -  z vertex of proton track (secound proton)
	r_p2       -  the smallest distance between track and z-axis (z_p1 piont) (secound proton)
	xvert_pp   -  x component of the vertex calculated from two protons
	yvert_ppa  -  y component of the vertex calculated from two protons
	zvert_pp   -  z component of the vertex calculated from two protons
	dist_pp    -  closet distance between proton-proton tracks
	m2_miss2   -  squerd missing mass of two protons
	p_miss2    -  missing momentum of two protons
	m2_inv     -  squerd invariant mass of two protons (check of missidentification p-pip )
	dphi_12    -  | Ph_p1 - Ph_p2 | -> for el. scattering
	tanT1tanT2 -  tan()*tan()       -> for el. scattering
	miss_cosThCM - cos(Theta) of missing particle
	RKchiq_p1  -  Runge Kutta chi squerd of first proton
	RKchiq_p2  -  Runge Kutta chi squerd of secound proton
	ncomb      -  number of track-track combination in the event
	dsf        -  down-scale factor
	triggerBit -  tells what kind of trigger was used (trigger box setting)
	date       -  date when file was taken
	time       -  time when file was taken
	InnerMDCchiq_p1 - inner MDC chi squerd of first proton
	InnerMDCchiq_p2 - inner MDC chi squerd of secound proton
	**/


using namespace std;

#include "hhypKpKmProjector.h"
#include "hgeomvector.h"
#include "hgeomvertexfit.h"

// Masses/Constants should not be fixed in code in this way!
#define c 0.299792
#define D2R 0.0174532925199432955
#define R2D 57.2957795130823229
#define P_mass 938.272309999999998
#define k_mass 493.677
#define Pip_mass 139.57018

Double_t Tofino_dEdx_Corr(Float_t AnglePhi, Float_t Sector, Float_t TofELoss, TH1F* Tofino_dEdx_offsets)
{
    // variables
    Int_t Phibin;
    Double_t Tofino_dEdx_pion, Tofino_dEdx_proton; // measured dEdx values at choosen bin
    Double_t Eloss_val_pion_calc   = 0.892;  // nominal dEdx values at choosen bin
    Double_t Eloss_val_proton_calc = 2.2764; // nominal dEdx values at choosen bin
    Double_t Tofino_dEx_pion_ratio, Tofino_dEx_proton_ratio;

    if(Sector != 5)
    {
        Phibin = (Int_t)((AnglePhi-(Sector+1.0)*60.0)/15.0);
    }
    else
    {
        Phibin = (Int_t)(AnglePhi/15.0);
    }

    Tofino_dEdx_pion        = Tofino_dEdx_offsets->GetBinContent(Sector*4+Phibin+1);
    //Tofino_dEdx_proton      = Tofino_dEdx_offsets->GetBinContent(Sector*4+Phibin+1+24);
    //cout << "Tofino_dEdx_pion = " << Tofino_dEdx_pion << ", Tofino_dEdx_proton = " << Tofino_dEdx_proton << endl;
    Tofino_dEx_pion_ratio   = Eloss_val_pion_calc/Tofino_dEdx_pion;
    //Tofino_dEx_proton_ratio = Eloss_val_proton_calc/Tofino_dEdx_proton;
    Float_t TofELoss_corr = TofELoss*Tofino_dEx_pion_ratio;

    return TofELoss_corr;
}

    TFile *file_dEdx_Tofino   = TFile::Open("/d/hades12/data/schmah_may07/Tofino_calib/Tofino_dEdx_shifts.root");
    TH1F* Tofino_dEdx_offsets = (TH1F*)file_dEdx_Tofino->Get("Tofino_dEdx_offsets");

ClassImp(HHypKpKmProjector)

HHypKpKmProjector::HHypKpKmProjector(Char_t *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
    simuflag = 0;
}

HHypKpKmProjector::~HHypKpKmProjector()
{
}

Bool_t HHypKpKmProjector::execute()
{
	Short_t triggerBit = gHades->getCurrentEvent()->getHeader()->getTBit();
    HEventHeader *evHeader = gHades->getCurrentEvent()->getHeader();

    UInt_t date = evHeader->getDate();
    UInt_t time = evHeader->getTime();
	//cout<<" date : "<<date<<"         time : "<<time<<endl;

    UInt_t dsf = evHeader->getDownscaling();
    //UInt_t triggerDecision = evHeader->getTriggerDecision();

    if (!beam)
	{
        cerr << algoName << " needs beam particle! " << endl;
        return kFALSE;
    }
    Float_t r_p1=0,z_p1=0,r_p2=0,z_p2=0, kaon_M2=0;
    Float_t RKchiq_p1=-10, RKchiq_p2=-10;     // RK chi2 for track1 & track2
    Short_t sector_p1=-1, sector_p2=-1, system_p1=-1, system_p2=-1; // number of sector for first and secound particle
    Float_t InnerMDCchiq_p1=-10, InnerMDCchiq_p2=-10;
    TVector3 v1(0,0,0),v2(0,0,0);
    Float_t r_k1=0,z_k1=0,r_k2=0,z_k2=0;
    Float_t RKchiq_k1=-10, RKchiq_k2=-10;     // RK chi2 for track1 & track2
    Short_t sector_k1=-1, sector_k2=-1, system_k1=-1, system_k2=-1; // number of sector for first and secound particle
    Float_t InnerMDCchiq_k1=-10, InnerMDCchiq_k2=-10;
    TVector3 v_k1(0,0,0),v_k2(0,0,0);
  
    Float_t P_1=0., P_k1=0., P_k2=0., Th_1=0., Th_2=0., Ph_1=0., Ph_2=0.;
    Float_t k_1=0., k_2=0., Th_k1=0., Th_k2=0., Ph_k1=0., Ph_k2=0.;
    Int_t geant_grandparentID_p1 = -10, geant_parentID_p1 = -10, geantID_p1 = -100;
    Int_t geant_grandparentID_p2 = -10, geant_parentID_p2 = -10, geantID_p2 = -100;
    Float_t geninfo1_p1=-10, geninfo2_p1=-10;
    Float_t geninfo1_p2=-10, geninfo2_p2=-10;
    Float_t geninfo_p1=-10, geninfo_p2=-10;
    Float_t dEdx_p1=0;
    Float_t dEdx_p2=0;
    Float_t dEdx_k1=0;
    Float_t dEdx_k2=0;
    Float_t dEdx_MDC_p1=0;
    Float_t dEdx_MDC_p2=0;
    Float_t dEdx_MDC_k1=0;
    Float_t dEdx_MDC_k2=0;

    Float_t MetaMatchChiq_p1=0;
    Float_t qIOMatch_p1     =0;
    Float_t RKChiq_p1       =0;
    Float_t SegInnerChiq_p1 =0;
    Float_t SegOuterChiq_p1 =0;
    Float_t MetaAccepted_p1 =0;
    Float_t RKmass_p1       =0;
    Float_t RKPathLength_p1 =0;
    Float_t Tof_p1          =0;
    Float_t dEdx_TOF_p1     =0;
    Int_t RK_Ring_Corr_p1   =0;
    Float_t dEdx_Tofino_p1     =0;

    Float_t MetaMatchChiq_k1=0;
    Float_t qIOMatch_k1     =0;
    Float_t RKChiq_k1       =0;
    Float_t SegInnerChiq_k1 =0;
    Float_t SegOuterChiq_k1 =0;
    Float_t MetaAccepted_k1 =0;
    Float_t RKmass_k1       =0;
    Float_t RKPathLength_k1 =0;
    Float_t Tof_k1          =0;
    Float_t dEdx_TOF_k1     =0;
    Int_t RK_Ring_Corr_k1   =0;
    Float_t dEdx_Tofino_k1     =0;

    Float_t MetaMatchChiq_k2=0;
    Float_t qIOMatch_k2     =0;
    Float_t RKChiq_k2       =0;
    Float_t SegInnerChiq_k2 =0;
    Float_t SegOuterChiq_k2 =0;
    Float_t MetaAccepted_k2 =0;
    Float_t RKmass_k2       =0;
    Float_t RKPathLength_k2 =0;
    Float_t Tof_k2          =0;
    Float_t dEdx_TOF_k2     =0;
    Int_t RK_Ring_Corr_k2   =0;
    Float_t dEdx_Tofino_k2     =0;

    Float_t proton1_Phi = 0;
    Float_t kaon1_Phi = 0;
    Float_t kaon2_Phi = 0;
    Float_t proton1_Theta = 0;
    Float_t kaon1_Theta = 0;
    Float_t kaon2_Theta = 0;
    
    Float_t proton1_P = 0;
    Float_t kaon1_P = 0;
    Float_t kaon2_P = 0;
    Float_t kk_vertex_X = 0;
    Float_t kk_vertex_Y = 0;
    Float_t kk_vertex_Z = 0;
    Float_t miss2_M2=0;
    Float_t miss2_P=0;
    Float_t miss2_cm_CosTheta = 0;

    Float_t deltaTof= 100000000;
     Int_t BestComb  = 0;
    Int_t IsBestComb  = 0;
    Int_t icomb=-1;
    mylist->CombIteratorReset();
    while (mylist->CombIterator())
    {
       Float_t deltaTof_tmp;
       icomb++;
       mylist->getUserValue(DELTATOF_CHI2, deltaTof_tmp);
       if( deltaTof_tmp < deltaTof )
       {
         deltaTof = deltaTof_tmp;
         BestComb = icomb;
       }
    }


    // Resetting the list and start looping over the combinations
    // Loop is only done over the VALID combinations

    mylist->CombIteratorReset();
    Int_t ncomb=mylist->getNcomb();
    icomb=-1;
    while (mylist->CombIterator())
    {
	   icomb++;
       if( icomb == BestComb ) IsBestComb = 1;
       else IsBestComb = 0;
	   if(mylist->getProbAlg(icomb)<=0) continue;
        TLorentzVector proton1(0,0,0,0);
        TLorentzVector proton2(0,0,0,0);
        TLorentzVector pip1(0,0,0,0);
        TLorentzVector pip2(0,0,0,0);
        TLorentzVector kaon1(0,0,0,0);
        TLorentzVector kaon2(0,0,0,0);

        TLorentzVector geant_proton1(0,0,0,0);
        TLorentzVector geant_proton2(0,0,0,0);
        TVector3 pp_vertex;
        TVector3 pp_distance;
        TVector3 kk_vertex;
        TVector3 kk_distance;

        Float_t dist_kk=100;

        if (mylist->getIterStatus() == kTRUE)
	    {
	        const HPidHitData   *PidData = NULL;
	        const HPidTrackData *pTrack = NULL;
	        HPidTrackCand *PidTrackCand= NULL;
            //-------------------- simulation ----------------------------------------------
	        if (simuflag == 1 )
	        {
		  HPidTrackCandSim *my_p1 = (HPidTrackCandSim *) CatTrackCandSim->
		    getObject(mylist->getIdxPidTrackCand(icomb, 0));
		  HPidTrackCandSim *my_p2 = (HPidTrackCandSim *) CatTrackCandSim->
		    getObject(mylist->getIdxPidTrackCand(icomb, 1));
		  
		  const HPidGeantTrackSet *p1GeantSet = my_p1->getGeantTrackSet();
		  const HPidGeantTrackSet *p2GeantSet = my_p2->getGeantTrackSet();
		  
		  
		  geninfo_p1             = p1GeantSet->getGenInfo();
		  geninfo1_p1             = p1GeantSet->getGenInfo1();
		  geninfo2_p1             = p1GeantSet->getGenInfo2();
		  geantID_p1              = p1GeantSet->getGeantPID();
		  geant_parentID_p1       = p1GeantSet->getGeantParentID();
		  geant_grandparentID_p1  = p1GeantSet->getGeantGrandParentID();
		  TVector3                  v1(p1GeantSet->getGeantMomX(), p1GeantSet->getGeantMomY(), p1GeantSet->getGeantMomZ());
		  geant_proton1.SetVectM(v1,P_mass);
		  
		  geninfo_p2             = p2GeantSet->getGenInfo();
		  geninfo1_p2             = p2GeantSet->getGenInfo1();
		  geninfo2_p2             = p2GeantSet->getGenInfo2();
		  geantID_p2              = p2GeantSet->getGeantPID();
		  geant_parentID_p2       = p2GeantSet->getGeantParentID();
		  geant_grandparentID_p2  = p2GeantSet->getGeantGrandParentID();
		  TVector3                  v2(p2GeantSet->getGeantMomX(), p2GeantSet->getGeantMomY(), p2GeantSet->getGeantMomZ());
		  geant_proton2.SetVectM(v2,P_mass);

            }  // simuflag

            //-------------------- simulation end -------------------------------------------

            HCategory *pidpartCat =  gHades->getCurrentEvent()->getCategory(catPidTrackCand);

            if (pidpartCat != NULL )
            {
	      PidTrackCand = (HPidTrackCand *) pidpartCat->getObject(mylist->getIdxPidTrackCand(icomb, 0));
	      if (PidTrackCand != NULL)
		{
		  PidData      = PidTrackCand->getHitData();
		  pTrack       = PidTrackCand->getTrackData();
		  InnerMDCchiq_p1 =  PidData->fInnerMdcChiSquare;

		  MetaMatchChiq_p1     = pTrack->fMetaMatchingQuality;
		  qIOMatch_p1          = pTrack->qIOMatching[4];
		  RKChiq_p1            = pTrack->fRKChiSquare;
		  SegInnerChiq_p1      = PidData->fInnerMdcChiSquare;
		  SegOuterChiq_p1      = PidData->fOuterMdcChiSquare;
		  MetaAccepted_p1      = pTrack->bIsAccepted[2];
		  RKmass_p1            = pTrack->fTofRecMassSquared[4];
		  RKPathLength_p1      = pTrack->getPathLength(4);
		  Tof_p1               = PidData->getTof();
		  
		  dEdx_TOF_p1 = pTrack->fCorrectedEloss[4];
		  if(PidData ->hasRingCorrelation[4])
		    {
		      RK_Ring_Corr_p1 = 1;
		    }
		  else
		    {
		      RK_Ring_Corr_p1 = 0;
		    }
		  
		  
		  system_p1      = PidData->iSystem;
		  sector_p1      = PidData->getSector();
		  r_p1           = pTrack->getTrackR(4);
		  z_p1           = pTrack->getTrackZ(4);
		  P_1  = pTrack->fMomenta[4];
		  Th_1 = pTrack->getRKTheta();
		  Ph_1 = pTrack->getRKPhi();
		  Th_1=Th_1*D2R;
		  Ph_1=Ph_1*D2R;
		  proton1_Phi = proton1.Phi();
		  proton1_Theta = proton1.Theta();
		  if(system_p1 == 0)
		    {
		      dEdx_Tofino_p1 = Tofino_dEdx_Corr(Ph_1,sector_p1,dEdx_TOF_p1,Tofino_dEdx_offsets);
		    }
	      
		  dEdx_MDC_p1 = PidData -> getCombinedMdcdEdx();
		  P_1 = enLossCorr.getCorrMom(14,P_1,Th_1*R2D);
		  v1.SetXYZ(P_1*sin(Th_1)*cos(Ph_1),P_1*sin(Th_1)*sin(Ph_1),P_1*cos(Th_1));
		  proton1.SetVectM(v1,P_mass);
		  proton1_P = proton1.P();
		  
		}
	      PidTrackCand = (HPidTrackCand *) pidpartCat->getObject(mylist->getIdxPidTrackCand(icomb, 1));
	      if (PidTrackCand != NULL) {
		//cout << "in : " << endl;
		PidData      = PidTrackCand->getHitData();
		pTrack       = PidTrackCand->getTrackData();
		InnerMDCchiq_k1 =  PidData->fInnerMdcChiSquare;

		MetaMatchChiq_k1     = pTrack->fMetaMatchingQuality;
		qIOMatch_k1          = pTrack->qIOMatching[4];
		RKChiq_k1            = pTrack->fRKChiSquare;
		SegInnerChiq_k1      = PidData->fInnerMdcChiSquare;
		SegOuterChiq_k1      = PidData->fOuterMdcChiSquare;
		MetaAccepted_k1      = pTrack->bIsAccepted[2];
		RKmass_k1            = pTrack->fTofRecMassSquared[4];
		RKPathLength_k1      = pTrack->getPathLength(4);
		Tof_k1               = PidData->getTof();
		dEdx_TOF_k1 = pTrack->fCorrectedEloss[4];
		if(PidData ->hasRingCorrelation[4])
		    {
		      RK_Ring_Corr_k1 = 1;
		    }
		  else
		    {
		      RK_Ring_Corr_k1 = 0;
		    }

		system_k1      = PidData->iSystem;
		sector_k1      = PidData->getSector();
		r_k1           = pTrack->getTrackR(4);
		z_k1           = pTrack->getTrackZ(4);
		k_1  = pTrack->fMomenta[4];
		Th_k1 = pTrack->getRKTheta();
		Ph_k1 = pTrack->getRKPhi();
		Th_k1=Th_k1*D2R;
		Ph_k1=Ph_k1*D2R;
		kaon1_Phi = kaon1.Phi();
		kaon1_Theta = kaon1.Theta();
		if(system_k1 == 0)
		  {
		    dEdx_Tofino_k1 = Tofino_dEdx_Corr(Ph_k1,sector_k1,dEdx_TOF_k1,Tofino_dEdx_offsets);
		  }
		dEdx_k1 = PidData -> getInnerMdcdEdx();
		dEdx_MDC_k1 = PidData -> getCombinedMdcdEdx();
		k_1 = enLossCorr.getCorrMom(11,k_1,Th_k1*R2D);
		v_k1.SetXYZ(k_1*sin(Th_k1)*cos(Ph_k1),P_k1*sin(Th_k1)*sin(Ph_k1),P_k1*cos(Th_k1));
		kaon1.SetVectM(v_k1,k_mass);
		kaon1_P= kaon1.P();
		//cout <<"InnerMDCchiq_k1 : " <<InnerMDCchiq_k1 << " k_1 :" <<k_1 <<endl;
	      }
	      PidTrackCand = (HPidTrackCand *) pidpartCat->getObject(mylist->getIdxPidTrackCand(icomb, 2));
	      if (PidTrackCand != NULL) {
		PidData      = PidTrackCand->getHitData();
		pTrack       = PidTrackCand->getTrackData();
		InnerMDCchiq_k2 =  PidData->fInnerMdcChiSquare;

		MetaMatchChiq_k2     = pTrack->fMetaMatchingQuality;
		qIOMatch_k2          = pTrack->qIOMatching[4];
		RKChiq_k2            = pTrack->fRKChiSquare;
		SegInnerChiq_k2      = PidData->fInnerMdcChiSquare;
		SegOuterChiq_k2      = PidData->fOuterMdcChiSquare;
		MetaAccepted_k2      = pTrack->bIsAccepted[2];
		RKmass_k2            = pTrack->fTofRecMassSquared[4];
		RKPathLength_k2      = pTrack->getPathLength(4);
		Tof_k2               = PidData->getTof();
		dEdx_TOF_k2 = pTrack->fCorrectedEloss[4];
		if(PidData ->hasRingCorrelation[4])
		    {
		      RK_Ring_Corr_k2 = 1;
		    }
		  else
		    {
		      RK_Ring_Corr_k2 = 0;
		    }


		system_k2      = PidData->iSystem;
		sector_k2      = PidData->getSector();
		r_k2           = pTrack->getTrackR(4);
		z_k2           = pTrack->getTrackZ(4);
		k_2  = pTrack->fMomenta[4];
		Th_k2 = pTrack->getRKTheta();
		Ph_k2 = pTrack->getRKPhi();
		Th_k2=Th_k2*D2R;
		Ph_k2=Ph_k2*D2R;
		if(system_k2 == 0)
		  {
		    dEdx_Tofino_k2 = Tofino_dEdx_Corr(Ph_k2,sector_k2,dEdx_TOF_k2,Tofino_dEdx_offsets);
		  }
		kaon2_Phi = kaon2.Phi();
		kaon2_Theta = kaon2.Theta();
		dEdx_k2 = PidData -> getInnerMdcdEdx();
		dEdx_MDC_k2 = PidData -> getCombinedMdcdEdx();
		k_2 = enLossCorr.getCorrMom(12,k_2,Th_k2*R2D);
		v_k2.SetXYZ(k_2*sin(Th_k2)*cos(Ph_k2),P_k2*sin(Th_k2)*sin(Ph_k2),P_k2*cos(Th_k2));
		kaon2.SetVectM(v_k2,k_mass);
		kaon2_P=kaon2.P();
	      }
	      
	      //---------------------------------- pp Vertex calculation ----------------------------------------
	      dist_kk =  calcVertex((HPidTrackCand *) pidpartCat->getObject(mylist->getIdxPidTrackCand(icomb, 1)),
				    (HPidTrackCand *) pidpartCat->getObject(mylist->getIdxPidTrackCand(icomb, 2)),
				    &kk_vertex, &kk_distance);
	      //---------------------------------- pp Vertex calculation -------- end ---------------------------
            }
            TLorentzVector miss2 = (*beam) - (proton1 + kaon1 + kaon2);  // beam = beam + target
	    
            TLorentzVector invkk = (kaon1 + kaon2);  // beam = beam + target
            // calculating missing particle 4vector
	    //	    if(proton1.E()==0 || proton2.E()==0) {cout<<"HHypKpKmProjector::  empty particle"<<endl; continue;}

            // 4vectors in CM system
            TLorentzVector miss2_cm = miss2;
            TLorentzVector proton1_cm = proton1;
            TLorentzVector kaon1_cm = kaon1;
            TLorentzVector kaon2_cm = kaon2;
            miss2_cm.Boost(0.0, 0.0, -(*beam).Beta() );
			kaon1_cm.Boost(0.0, 0.0, -(*beam).Beta() );
			kaon2_cm.Boost(0.0, 0.0, -(*beam).Beta() );
			miss2_M2 = miss2.M2();
			kk_vertex_X = kk_vertex.X();
			kk_vertex_Y = kk_vertex.Y();
			kk_vertex_Z = kk_vertex.Z();

			//          Float_t tanTh1 = TMath::Tan(proton1.Theta());
			//            Float_t tanTh2 = TMath::Tan(proton2.Theta());

	    //TVector3 NppCM, NPi0Z;
	    //NppCM = (proton1_cm.Vect()).Cross(proton2_cm.Vect());
	    //NPi0Z = (miss2_cm.Vect()).Cross((*beam).Vect());
	    //float PsiCM = (TMath::RadToDeg()*TMath::ACos(NppCM*NPi0Z/(NppCM.Mag()*NPi0Z.Mag())));
            if(simuflag==1)
	        {
//                  Float_t fpp[]={ sector_p1, sector_p2, system_p1, system_p2,
//                                  proton1.P(), proton1.Theta(), proton1.Phi(),
//                                  proton2.P(), proton2.Theta(), proton2.Phi(),
//                                  geant_proton1.P(), geant_proton1.Theta(), geant_proton1.Phi(),
//                                  geant_proton2.P(), geant_proton2.Theta(), geant_proton2.Phi(),
// 								 z_p1, r_p1, z_p2, r_p2,
//                                  pp_vertex.X(), pp_vertex.Y(), pp_vertex.Z(), dist_pp,
//                                  miss2.M2(), miss2.P(), (proton1+proton2).M2(),
// 								 fabs(v1.Phi() - proton2.Phi()),
//                                  tanTh1*tanTh2, miss2_cm.CosTheta(),
//                                  RKchiq_p1, RKchiq_p2, ncomb, dsf, triggerBit,
//                                  geantID_p1, geant_parentID_p1, geant_grandparentID_p1,
//                                  geantID_p2, geant_parentID_p2, geant_grandparentID_p2,
// 								 geninfo_p1, geninfo1_p1, geninfo2_p1,
// 								 geninfo_p2, geninfo1_p2, geninfo2_p2,
// 								 InnerMDCchiq_p1, InnerMDCchiq_p2, IsBestComb
//		};
//		  pkpkm->Fill(fpkpkm);
	        }
	    else
	      {
	        kaon_M2 = (kaon1+kaon2).M2();
		Float_t fpkpkm[]=
		  { MetaMatchChiq_p1,qIOMatch_p1,RKChiq_p1,SegInnerChiq_p1,SegOuterChiq_p1,MetaAccepted_p1,
		    RKmass_p1,RKPathLength_p1,Tof_p1,MetaMatchChiq_k1,qIOMatch_k1,RKChiq_k1,SegInnerChiq_k1,
		    SegOuterChiq_k1,MetaAccepted_k1,RKmass_k1,RKPathLength_k1,Tof_k1,MetaMatchChiq_k2,qIOMatch_k2,
		    RKChiq_k2,SegInnerChiq_k2,SegOuterChiq_k2,MetaAccepted_k2,RKmass_k2,RKPathLength_k2,Tof_k2,
		    sector_p1, sector_p2, system_p1, system_k1, system_k2,
		    proton1.P(), proton1.Theta(), proton1.Phi(),
		    kaon1.P(), kaon1.Theta(), kaon1.Phi(),
		    kaon2.P(), kaon2.Theta(), kaon2.Phi(),
		    P_1, k_1, k_2 ,
		    z_p1, r_p1, z_k1, r_k1, z_k2, r_k2,
		    Th_1, Th_k1, Th_k2, Ph_1, Ph_k1, Ph_k2,  
		    dEdx_p1, dEdx_k1, dEdx_k2,
		    dEdx_MDC_p1, dEdx_MDC_k1, dEdx_MDC_k2,
		    kk_vertex.X(), kk_vertex.Y(), kk_vertex.Z(), dist_kk,
		    miss2.M2(), miss2.P(), kaon_M2,
		    miss2_cm.CosTheta(),
		    RKchiq_p1, RKchiq_k1,RKchiq_k2,  ncomb, dsf, triggerBit, date, time,
		    InnerMDCchiq_p1, InnerMDCchiq_k1,InnerMDCchiq_k2, IsBestComb,
		    RK_Ring_Corr_p1,RK_Ring_Corr_k1,RK_Ring_Corr_k2,dEdx_TOF_p1,dEdx_TOF_k1,
		    dEdx_TOF_k2,dEdx_Tofino_p1,dEdx_Tofino_k1,dEdx_Tofino_k2
		  };
		
		pkpkm->Fill(fpkpkm);
	      }
         } else  cerr << algoName << " got no TLorentzVector " << endl;
    }
    return kTRUE;
}

Bool_t HHypKpKmProjector::init()
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
      pkpkm =  new TNtuple(input + TString("_pkpkm"),
			   "sector_p1:sector_p2:system_p1:system_p2:P_p1:Th_p1:Ph_p1:P_p2:Th_p2:Ph_p2:gP_p1:gTh_p1:gPh_p1:gP_p2:gTh_p2:gPh_p2:z_p1:r_p1:z_p2:r_p2:xvert_pp:yvert_pp:zvert_pp:dist_pp:m2_miss2:p_miss2:m2_inv:dphi_12:tanT1tanT2:miss_cosThCM:RKchiq_p1:RKchiq_p2:ncomb:dsf:triggerBit:geant_ID_p1:geant_parentID_p1:geant_grandparentID_p1:geant_ID_p2:geant_parentID_p2:geant_grandparentID_p2:geninfo_p1:geninfo1_p1:geninfo2_p1:geninfo_p2:geninfo1_p2:geninfo2_p2:InnerMDCchiq_p1:InnerMDCchiq_p2:IsBestComb",
			   "sector_p1:sector_p2:system_p1:system_p2:P_p1:Th_p1:Ph_p1:P_p2:Th_p2:Ph_p2:gP_p1:gTh_p1:gPh_p1:gP_p2:gTh_p2:gPh_p2:z_p1:r_p1:z_p2:r_p2:xvert_pp:yvert_pp:zvert_pp:dist_pp:m2_miss2:p_miss2:m2_inv:dphi_12:tanT1tanT2:miss_cosThCM:RKchiq_p1:RKchiq_p2:ncomb:dsf:triggerBit:geant_ID_p1:geant_parentID_p1:geant_grandparentID_p1:geant_ID_p2:geant_parentID_p2:geant_grandparentID_p2:geninfo_p1:geninfo1_p1:geninfo2_p1:geninfo_p2:geninfo1_p2:geninfo2_p2:InnerMDCchiq_p1:InnerMDCchiq_p2:IsBestComb");
    }
  else
    {
      pkpkm =  new TNtuple(input + TString("_pkpkm"),
         		   "MetaMatchChiq_p1:qIOMatch_p1:RKChiq_p1:SegInnerChiq_p1:SegOuterChiq_p1:MetaAccepted_p1:RKmass_p1:RKPathLength_p1:Tof_p1:MetaMatchChiq_k1:qIOMatch_k1:RKChiq_k1:SegInnerChiq_k1:SegOuterChiq_k1:MetaAccepted_k1:RKmass_k1:RKPathLength_k1:Tof_k1:MetaMatchChiq_k2:qIOMatch_k2:RKChiq_k2:SegInnerChiq_k2:SegOuterChiq_k2:MetaAccepted_k2:RKmass_k2:RKPathLength_k2:Tof_k2:sector_p1:sector_p2:system_p1:system_k1:system_k2:proton1_P:proton1_Theta:proton1_Phi:kaon1_P:kaon1_Theta:kaon1_Phi:kaon2_P:kaon2_Theta:kaon2_Phi:P_1:k_1:k_2:z_p1:r_p1:z_k1:r_k1:z_k2:r_k2:Th_1:Th_k1:Th_k2:Ph_1:Ph_k1:Ph_k2:dEdx_p1:dEdx_k1:dEdx_k2:dEdx_MDC_p1:dEdx_MDC_k1:dEdx_MDC_k2:kk_vertex_X:kk_vertex_Y:kk_vertex_Z:dist_kk:miss2_M2:miss2_P:kaon_M2:miss2_cm_CosTheta:RKchiq_p1:RKchiq_k1:RKchiq_k2: ncomb:dsf:triggerBit:date:time:InnerMDCchiq_p1:InnerMDCchiq_k1:InnerMDCchiq_k2:IsBestComb:RK_Ring_Corr_p1:RK_Ring_Corr_k1:RK_Ring_Corr_k2:dEdx_TOF_p1:dEdx_TOF_k1:dEdx_TOF_k2:dEdx_Tofino_p1:dEdx_Tofino_k1:dEdx_Tofino_k2",
			   "MetaMatchChiq_p1:qIOMatch_p1:RKChiq_p1:SegInnerChiq_p1:SegOuterChiq_p1:MetaAccepted_p1:RKmass_p1:RKPathLength_p1:Tof_p1:MetaMatchChiq_k1:qIOMatch_k1:RKChiq_k1:SegInnerChiq_k1:SegOuterChiq_k1:MetaAccepted_k1:RKmass_k1:RKPathLength_k1:Tof_k1:MetaMatchChiq_k2:qIOMatch_k2:RKChiq_k2:SegInnerChiq_k2:SegOuterChiq_k2:MetaAccepted_k2:RKmass_k2:RKPathLength_k2:Tof_k2:sector_p1:sector_p2:system_p1:system_k1:system_k2:proton1_P:proton1_Theta:proton1_Phi:kaon1_P:kaon1_Theta:kaon1_Phi:kaon2_P:kaon2_Theta:kaon2_Phi:P_1:k_1:k_2:z_p1:r_p1:z_k1:r_k1:z_k2:r_k2:Th_1:Th_k1:Th_k2:Ph_1:Ph_k1:Ph_k2:dEdx_p1:dEdx_k1:dEdx_k2:dEdx_MDC_p1:dEdx_MDC_k1:dEdx_MDC_k2:kk_vertex_X:kk_vertex_Y:kk_vertex_Z:dist_kk:miss2_M2:miss2_P:kaon_M2:miss2_cm_CosTheta:RKchiq_p1:RKchiq_k1:RKchiq_k2: ncomb:dsf:triggerBit:date:time:InnerMDCchiq_p1:InnerMDCchiq_k1:InnerMDCchiq_k2:IsBestComb:RK_Ring_Corr_p1:RK_Ring_Corr_k1:RK_Ring_Corr_k2:dEdx_TOF_p1:dEdx_TOF_k1:dEdx_TOF_k2:dEdx_Tofino_p1:dEdx_Tofino_k1:dEdx_Tofino_k2");
    }
  return kTRUE;
}

Bool_t HHypKpKmProjector::reinit()
{
    return kTRUE;
}

Bool_t HHypKpKmProjector::finalize()
{
    pkpkm->Write();
    return kTRUE;
}

Bool_t HHypKpKmProjector::IsOpposit(Short_t sec1, Short_t sec2)
{
   if(sec1==0 && sec2==3) return 1; else
   if(sec1==1 && sec2==4) return 1; else
   if(sec1==2 && sec2==5) return 1; else
   if(sec1==3 && sec2==0) return 1; else
   if(sec1==4 && sec2==1) return 1; else
   if(sec1==5 && sec2==2) return 1; else
   return 0;
}

Bool_t HHypKpKmProjector::SetParamFile(TString pFile)
{
    paramFile=pFile;
	return 0;
}

Float_t HHypKpKmProjector::calcVertex(HPidTrackCand *p1, HPidTrackCand *p2,
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

