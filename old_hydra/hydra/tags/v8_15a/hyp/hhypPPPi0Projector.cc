//*-- Author : M. Wisniowski
//*-- Modified : 2005-9-13
////////////////////////////////////////////////////////////////////////
//
// HHypPPPi0Projector
//
// HHypPPPi0Projector projects any PP data to the ntuple
//
////////////////////////////////////////////////////////////////////////
 
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
	beta_1     -  beta (v/c) of first proton
	beta_2     -  beta (v/c) of secound proton 
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

#include "hhypPPPi0Projector.h"
#include "hgeomvector.h"
#include "hgeomvertexfit.h"

#define c 0.299792
#define D2R 0.0174532925199432955
#define R2D 57.2957795130823229
#define P_mass 938.272309999999998
#define Pip_mass 139.57018

ClassImp(HHypPPPi0Projector)

HHypPPPi0Projector::HHypPPPi0Projector(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
    simuflag = 0;
}

HHypPPPi0Projector::~HHypPPPi0Projector()
{
}

Bool_t HHypPPPi0Projector::execute()
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
  
    Float_t beta_1=0, beta_2=0;
	Float_t r_p1=0,z_p1=0,r_p2=0,z_p2=0;
    Float_t RKchiq_p1=-10, RKchiq_p2=-10;     // RK chi2 for track1 & track2
    Short_t sector_p1=-1, sector_p2=-1, system_p1=-1, system_p2=-1; // number of sector for first and secound particle
    Float_t InnerMDCchiq_p1=-10, InnerMDCchiq_p2=-10;
    TVector3 v1(0,0,0),v2(0,0,0);
	Float_t P_1=0., P_2=0., Th_1=0., Th_2=0., Ph_1=0., Ph_2=0.;
    Int_t geant_grandparentID_p1 = -10, geant_parentID_p1 = -10, geantID_p1 = -100;
    Int_t geant_grandparentID_p2 = -10, geant_parentID_p2 = -10, geantID_p2 = -100;
    Float_t geninfo1_p1=-10, geninfo2_p1=-10;
    Float_t geninfo1_p2=-10, geninfo2_p2=-10;
    Float_t geninfo_p1=-10, geninfo_p2=-10;
 
    // Resetting the list and start looping over the combinations
    // Loop is only done over the VALID combinations
	
    mylist->CombIteratorReset();
    Int_t ncomb=mylist->getNcomb();
    Int_t icomb=-1;
    while (mylist->CombIterator()) 
    {
	   icomb++;
	   if(mylist->getProbAlg(icomb)<=0) continue; 
        TLorentzVector proton1(0,0,0,0);
        TLorentzVector proton2(0,0,0,0);
        TLorentzVector pip1(0,0,0,0);
        TLorentzVector pip2(0,0,0,0);
        TLorentzVector geant_proton1(0,0,0,0);
        TLorentzVector geant_proton2(0,0,0,0);
        TVector3 pp_vertex;
        TVector3 pp_distance;
        Float_t dist_pp=100;
						
        if (mylist->getIterStatus() == kTRUE) 
	    {
            HPidParticle  *PidPart = NULL;
	        const HPidHitData   *PidData = NULL;
	        const HPidTrackData *pTrack = NULL;
	        HPidTrackCand *PidTrackCand= NULL;
            //-------------------- simulation ----------------------------------------------
	        if (simuflag == 1 ) 
	        {
	            HPidParticleSim *my_p1 = (HPidParticleSim *) CatPartSim->
							 getObject(mylist->getIdxPidPart(icomb, 0));
	            HPidParticleSim *my_p2 = (HPidParticleSim *) CatPartSim->
							 getObject(mylist->getIdxPidPart(icomb, 1));

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

            HCategory *pidpartCat =  gHades->getCurrentEvent()->getCategory(catPidPart);

            if (pidpartCat != NULL ) 
            {
                PidPart = (HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(icomb, 0));
                if (PidPart != NULL) 
		        {
	                PidData      = PidPart->getHitData();
	                pTrack       = PidPart->getTrackData();
		            PidTrackCand = PidPart->getTrackCand();
				    InnerMDCchiq_p1 =  PidData->fInnerMdcChiSquare;
			        system_p1      = PidData->iSystem;
		            sector_p1      = PidPart->sector();
	                beta_1         = PidPart->getBetaExp();
                    r_p1           = PidPart->getR();
                    z_p1           = PidPart->getZ();
                    P_1  = pTrack->fMomenta[4];
                    Th_1 = pTrack->getRKTheta();
                    Ph_1 = pTrack->getRKPhi();
                    Th_1=Th_1*D2R;
                    Ph_1=Ph_1*D2R; 
				    P_1 = enLossCorr.getCorrMom(14,P_1,Th_1*R2D);
                    v1.SetXYZ(P_1*sin(Th_1)*cos(Ph_1),P_1*sin(Th_1)*sin(Ph_1),P_1*cos(Th_1));
                    proton1.SetVectM(v1,P_mass);
	            }
                PidPart = (HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(icomb, 1));
                if (PidPart != NULL) {
	                PidData      = PidPart->getHitData();
	                pTrack       = PidPart->getTrackData();
		            PidTrackCand = PidPart->getTrackCand();
				    InnerMDCchiq_p2 =  PidData->fInnerMdcChiSquare;
			        system_p2      = PidData->iSystem;
		            sector_p2      = PidPart->sector();
                    r_p2           = PidPart->getR();
                    z_p2           = PidPart->getZ();
                    P_2  = pTrack->fMomenta[4];
                    Th_2 = pTrack->getRKTheta();
                    Ph_2 = pTrack->getRKPhi();
                    Th_2=Th_2*D2R;
                    Ph_2=Ph_2*D2R; 
				    P_2 = enLossCorr.getCorrMom(14,P_2,Th_2*R2D);
                    v2.SetXYZ(P_2*sin(Th_2)*cos(Ph_2),P_2*sin(Th_2)*sin(Ph_2),P_2*cos(Th_2));
                    proton2.SetVectM(v2,P_mass);
	            }
	            //---------------------------------- pp Vertex calculation ---------------------------------------- 
                dist_pp =  calcVertex((HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(icomb, 0)),
                                      (HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(icomb, 1)),
                                             &pp_vertex, &pp_distance);
	            //---------------------------------- pp Vertex calculation -------- end --------------------------- 
            }

            TLorentzVector miss2 = (*beam) - (proton1 + proton2);  // beam = beam + target

            // calculating missing particle 4vector
			if(proton1.E()==0 || proton2.E()==0) {cout<<"HHypPPPi0Projector::  empty particle"<<endl; continue;}
			
            // 4vectors in CM system
            TLorentzVector miss2_cm = miss2; 
            TLorentzVector proton1_cm = proton1; 
            TLorentzVector proton2_cm = proton2; 
            miss2_cm.Boost(0.0, 0.0, -(*beam).Beta() ); 
			proton1_cm.Boost(0.0, 0.0, -(*beam).Beta() );
			proton2_cm.Boost(0.0, 0.0, -(*beam).Beta() );

            Float_t tanTh1 = TMath::Tan(proton1.Theta());
            Float_t tanTh2 = TMath::Tan(proton2.Theta());

			//TVector3 NppCM, NPi0Z;
			//NppCM = (proton1_cm.Vect()).Cross(proton2_cm.Vect());
			//NPi0Z = (miss2_cm.Vect()).Cross((*beam).Vect());
		    //float PsiCM = (TMath::RadToDeg()*TMath::ACos(NppCM*NPi0Z/(NppCM.Mag()*NPi0Z.Mag())));
		
            if(simuflag==1)
	        {
                 Float_t fpp[]={ sector_p1, sector_p2, system_p1, system_p2,
                                 proton1.P(), proton1.Theta(), proton1.Phi(),
                                 proton2.P(), proton2.Theta(), proton2.Phi(),
                                 geant_proton1.P(), geant_proton1.Theta(), geant_proton1.Phi(),
                                 geant_proton2.P(), geant_proton2.Theta(), geant_proton2.Phi(),
								 z_p1, r_p1, z_p2, r_p2,
                                 pp_vertex.X(), pp_vertex.Y(), pp_vertex.Z(), dist_pp,
                                 miss2.M2(), miss2.P(), (proton1+proton2).M2(),
								 fabs(v1.Phi() - proton2.Phi()),
                                 tanTh1*tanTh2, miss2_cm.CosTheta(),
                                 beta_1, beta_2, 
                                 RKchiq_p1, RKchiq_p2, ncomb, dsf, triggerBit,
                                 geantID_p1, geant_parentID_p1, geant_grandparentID_p1, 
                                 geantID_p2, geant_parentID_p2, geant_grandparentID_p2, 
								 geninfo_p1, geninfo1_p1, geninfo2_p1, 
								 geninfo_p2, geninfo1_p2, geninfo2_p2, 
								 InnerMDCchiq_p1, InnerMDCchiq_p2
	                           }; 
                 pp->Fill(fpp);
	        }
	        else
	        {
                 Float_t fpp[]={ sector_p1, sector_p2, system_p1, system_p2,
                                 proton1.P(), proton1.Theta(), proton1.Phi(),
                                 proton2.P(), proton2.Theta(), proton2.Phi(),
								 z_p1, r_p1, z_p2, r_p2,
                                 pp_vertex.X(), pp_vertex.Y(), pp_vertex.Z(), dist_pp,
                                 miss2.M2(), miss2.P(), (proton1+proton2).M2(), fabs(v1.Phi() - v2.Phi()),
                                 tanTh1*tanTh2, miss2_cm.CosTheta(),
                                 beta_1, beta_2, 
                                 RKchiq_p1, RKchiq_p2, ncomb, dsf, triggerBit, date, time, 
								 InnerMDCchiq_p1, InnerMDCchiq_p2
                               };

                 pp->Fill(fpp);
             }
         } else  cerr << algoName << " got no TLorentzVector " << endl;
    }
    return kTRUE;
}

Bool_t HHypPPPi0Projector::init()
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

        CatPartSim = NULL;          // Category

        if ((CatPartSim =
             gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
            Error("init", "Cannot get catPidPartSim cat");
            return kFALSE;
        }
    }
    /*
        CatPartSim = NULL;          // Category
        if ((CatPartSim =
             gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
            Error("init", "Cannot get catPidPartSim cat");
            simuflag = 0;
            return kFALSE;
        } else simuflag = 1;
    */

    // need to get name from channel
    TString input(channel->Get(initList));
	TFile *f=GetHFile();
	f->cd();
    if(simuflag==1)
    {
        pp =  new TNtuple(input + TString("_pp"),
                "sector_p1:sector_p2:system_p1:system_p2:P_p1:Th_p1:Ph_p1:P_p2:Th_p2:Ph_p2:gP_p1:gTh_p1:gPh_p1:gP_p2:gTh_p2:gPh_p2:z_p1:r_p1:z_p2:r_p2:xvert_pp:yvert_pp:zvert_pp:dist_pp:m2_miss2:p_miss2:m2_inv:dphi_12:tanT1tanT2:miss_cosThCM:beta_1:beta_2:RKchiq_p1:RKchiq_p2:ncomb:dsf:triggerBit:geant_ID_p1:geant_parentID_p1:geant_grandparentID_p1:geant_ID_p2:geant_parentID_p2:geant_grandparentID_p2:geninfo_p1:geninfo1_p1:geninfo2_p1:geninfo_p2:geninfo1_p2:geninfo2_p2:InnerMDCchiq_p1:InnerMDCchiq_p2", 
                "sector_p1:sector_p2:system_p1:system_p2:P_p1:Th_p1:Ph_p1:P_p2:Th_p2:Ph_p2:gP_p1:gTh_p1:gPh_p1:gP_p2:gTh_p2:gPh_p2:z_p1:r_p1:z_p2:r_p2:xvert_pp:yvert_pp:zvert_pp:dist_pp:m2_miss2:p_miss2:m2_inv:dphi_12:tanT1tanT2:miss_cosThCM:beta_1:beta_2:RKchiq_p1:RKchiq_p2:ncomb:dsf:triggerBit:geant_ID_p1:geant_parentID_p1:geant_grandparentID_p1:geant_ID_p2:geant_parentID_p2:geant_grandparentID_p2:geninfo_p1:geninfo1_p1:geninfo2_p1:geninfo_p2:geninfo1_p2:geninfo2_p2:InnerMDCchiq_p1:InnerMDCchiq_p2"); 
	}
	else
	{
        pp =  new TNtuple(input + TString("_pp"),
                "sector_p1:sector_p2:system_p1:system_p2:P_p1:Th_p1:Ph_p1:P_p2:Th_p2:Ph_p2:z_p1:r_p1:z_p2:r_p2:xvert_pp:yvert_pp:zvert_pp:dist_pp:m2_miss2:p_miss2:m2_inv:dphi_12:tanT1tanT2:miss_cosThCM:beta_1:beta_2:RKchiq_p1:RKchiq_p2:ncomb:dsf:triggerBit:date:time:InnerMDCchiq_p1:InnerMDCchiq_p2", 
                "sector_p1:sector_p2:system_p1:system_p2:P_p1:Th_p1:Ph_p1:P_p2:Th_p2:Ph_p2:z_p1:r_p1:z_p2:r_p2:xvert_pp:yvert_pp:zvert_pp:dist_pp:m2_miss2:p_miss2:m2_inv:dphi_12:tanT1tanT2:miss_cosThCM:beta_1:beta_2:RKchiq_p1:RKchiq_p2:ncomb:dsf:triggerBit:date:time:InnerMDCchiq_p1:InnerMDCchiq_p2"); 
	}
    return kTRUE;
}

Bool_t HHypPPPi0Projector::reinit()
{
    return kTRUE;
}

Bool_t HHypPPPi0Projector::finalize()
{
    pp->Write();
    return kTRUE;
}

Bool_t HHypPPPi0Projector::IsOpposit(Short_t sec1, Short_t sec2)
{
   if(sec1==0 && sec2==3) return 1; else
   if(sec1==1 && sec2==4) return 1; else
   if(sec1==2 && sec2==5) return 1; else
   if(sec1==3 && sec2==0) return 1; else
   if(sec1==4 && sec2==1) return 1; else
   if(sec1==5 && sec2==2) return 1; else
   return 0;
}
			  
Bool_t HHypPPPi0Projector::SetParamFile(TString pFile)
{
    paramFile=pFile;
	return 0;
}

Float_t HHypPPPi0Projector::calcVertex(HPidParticle *p1, HPidParticle *p2,
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
        hoff[0].setXYZ(p1->getR()*TMath::Cos(p1->phiDeg()*TMath::DegToRad()
                                        + TMath::PiOver2()),
                                p1->getR()*TMath::Sin(p1->phiDeg()*TMath::DegToRad()
                                         + TMath::PiOver2()),
                                p1->getZ());
        hoff[1].setXYZ(p2->getR()*TMath::Cos(p2->phiDeg()*TMath::DegToRad()
                                        + TMath::PiOver2()),
                                p2->getR()*TMath::Sin(p2->phiDeg()*TMath::DegToRad()
                                         + TMath::PiOver2()),
                                p2->getZ());

        dir[0] = p1->Vect();
        dir[1] = p2->Vect();
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

