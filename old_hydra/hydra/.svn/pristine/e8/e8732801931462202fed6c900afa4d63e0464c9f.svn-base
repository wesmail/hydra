//*-- Author : M. Wisniowski
//*-- Modified : 2005-9-13
////////////////////////////////////////////////////////////////////////
//
// HHypPPipProjector
//
// HHypPPipProjector projects any PP data. At the moment output contains
//
////////////////////////////////////////////////////////////////////////
 

using namespace std;

#include "hhypPPipProjector.h"
#include "hgeomvector.h"
#include "hgeomvertexfit.h"

#define c 0.299792
#define D2R 0.0174532925199432955
#define R2D 57.2957795130823229
#define P_mass 938.272309999999998
#define Pip_mass 139.57018

ClassImp(HHypPPipProjector)

HHypPPipProjector::HHypPPipProjector(char *name_i, Option_t par[])
  :HHypBaseAlgorithm(name_i,par)
{
    simuflag = 0;
}

HHypPPipProjector::~HHypPPipProjector()
{
}

Bool_t HHypPPipProjector::execute()
{
#if(1)
    
	Short_t triggerBit = gHades->getCurrentEvent()->getHeader()->getTBit();
    HEventHeader *evHeader = gHades->getCurrentEvent()->getHeader();
    
    UInt_t date = evHeader->getDate();
    UInt_t time = evHeader->getTime();
	
    UInt_t dsf = evHeader->getDownscaling();
    UInt_t triggerDecision = evHeader->getTriggerDecision();

    if (!beam) 
	{
        cerr << algoName << " needs beam particle! " << endl;
        return kFALSE;
    }
  
    Float_t beta_p=0, beta_pip=0;
	Float_t r_p=0,z_p=0,r_pip=0,z_pip=0;      // vertex
    Float_t RKchiq_p=-10, RKchiq_pip=-10;     // RK chi2 for track1 & track2
    Short_t sector_p=-1, sector_pip=-1;
	Short_t system_p=-1, system_pip=-1; 
    Float_t InnerMDCchiq_p=-10, InnerMDCchiq_pip=-10;
    TVector3 v1(0,0,0), v2(0,0,0);
	Float_t P_p=0., P_pip=0., Th_p=0., Th_pip=0., Ph_p=0., Ph_pip=0.;
    Int_t geant_grandparentID_p  = -10,  geant_parentID_p  = -10,  geantID_p   = -100;
    Int_t geant_grandparentID_pip = -10, geant_parentID_pip = -10, geantID_pip = -100;
    Float_t geninfo1_p  =-10, geninfo2_p  =-10;
    Float_t geninfo1_pip=-10, geninfo2_pip=-10;
    Float_t geninfo_p   =-10, geninfo_pip =-10;
	TLorentzVector  proton(0,0,0,0), pip(0,0,0,0);
 
    // Resetting the list and start looping over the combinations
    // Loop is only done over the VALID combinations

	
    mylist->CombIteratorReset();
    Int_t ncomb=mylist->getNcomb();
    Int_t icomb=-1;
    while (mylist->CombIterator()) 
    {
	    icomb++;
	    if(mylist->getProbAlg(icomb)<=0) continue;
		   
        TLorentzVector geant_proton(0,0,0,0);
        TLorentzVector geant_pip(0,0,0,0);
        TVector3 vertex;
        TVector3 pp_distance;
        Float_t  dist_ppip=100;
						
        if (mylist->getIterStatus() == kTRUE) 
	    {
            HPidParticle  *PidPart = NULL;
	        const HPidHitData   *PidData = NULL;
	        const HPidTrackData *pTrack = NULL;
	        HPidTrackCand *PidTrackCand= NULL;
            HRKTrackB     *RkTrack=NULL;
            //-------------------- simulation ----------------------------------------------
	        if (simuflag == 1 ) 
	        {
	            HPidParticleSim *my_p = (HPidParticleSim *) CatPartSim->
							 getObject(mylist->getIdxPidPart(icomb, 0));
	            HPidParticleSim *my_pip = (HPidParticleSim *) CatPartSim->
							 getObject(mylist->getIdxPidPart(icomb, 1));

	            const HPidGeantTrackSet *p1GeantSet = my_p->getGeantTrackSet();
	            const HPidGeantTrackSet *p2GeantSet = my_pip->getGeantTrackSet();


	            geninfo_p             = p1GeantSet->getGenInfo();                
	            geninfo1_p             = p1GeantSet->getGenInfo1();                
	            geninfo2_p             = p1GeantSet->getGenInfo2();                
	            geantID_p              = p1GeantSet->getGeantPID();                 
            	geant_parentID_p       = p1GeantSet->getGeantParentID();                 
	            geant_grandparentID_p  = p1GeantSet->getGeantGrandParentID();                 
	            TVector3                  v1(p1GeantSet->getGeantMomX(), p1GeantSet->getGeantMomY(), p1GeantSet->getGeantMomZ());
	            geant_proton.SetVectM(v1,P_mass);

			    geninfo_pip             = p2GeantSet->getGenInfo();                
			    geninfo1_pip             = p2GeantSet->getGenInfo1();                
                geninfo2_pip             = p2GeantSet->getGenInfo2();                
                geantID_pip              = p2GeantSet->getGeantPID();                 
                geant_parentID_pip       = p2GeantSet->getGeantParentID();                 
                geant_grandparentID_pip  = p2GeantSet->getGeantGrandParentID();                 
                TVector3                  v2(p2GeantSet->getGeantMomX(), p2GeantSet->getGeantMomY(), p2GeantSet->getGeantMomZ());
                geant_pip.SetVectM(v2,P_mass);

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
				    InnerMDCchiq_p =  PidData->fInnerMdcChiSquare;
			        system_p      = PidData->iSystem;
		            sector_p      = PidPart->sector();
	                beta_p        = PidPart->getBetaExp();
                    r_p           = PidPart->getR();
                    z_p           = PidPart->getZ();
                    P_p  = pTrack->fMomenta[4];
                    Th_p = pTrack->getRKTheta();
                    Ph_p = pTrack->getRKPhi();
                    Th_p=Th_p*D2R;
                    Ph_p=Ph_p*D2R; 
					P_p = enLossCorr.getCorrMom(14,P_p,Th_p*R2D);
                    v1.SetXYZ(P_p*sin(Th_p)*cos(Ph_p),P_p*sin(Th_p)*sin(Ph_p),P_p*cos(Th_p));
                    proton.SetVectM(v1,P_mass);
	            }
                PidPart = (HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(icomb, 1));
                if (PidPart != NULL) {
	                PidData      = PidPart->getHitData();
	                pTrack       = PidPart->getTrackData();
		            PidTrackCand = PidPart->getTrackCand();
				    InnerMDCchiq_pip =  PidData->fInnerMdcChiSquare;
			        system_pip      = PidData->iSystem;
		            sector_pip      = PidPart->sector();
	                beta_pip        = PidPart->getBetaExp();
                    r_pip           = PidPart->getR();
                    z_pip           = PidPart->getZ();
                    P_pip  = pTrack->fMomenta[4];
                    Th_pip = pTrack->getRKTheta();
                    Ph_pip = pTrack->getRKPhi();
                    Th_pip=Th_pip*D2R;
                    Ph_pip=Ph_pip*D2R; 
					P_p = enLossCorr.getCorrMom(14,P_p,Th_p*R2D);
                    v2.SetXYZ(P_pip*sin(Th_pip)*cos(Ph_pip),P_pip*sin(Th_pip)*sin(Ph_pip),P_pip*cos(Th_pip));
                    pip.SetVectM(v2,Pip_mass);
	            }
	            //--------------------------- pp Vertex calculation ---------------------------------------- 
                dist_ppip =  calcVertex((HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(icomb, 0)),
                                      (HPidParticle *) pidpartCat->getObject(mylist->getIdxPidPart(icomb, 1)),
                                             &vertex, &pp_distance);
	            //--------------------------- pp Vertex calculation -------- end --------------------------- 
            }
            TLorentzVector miss2 = (*beam) - (proton + pip);  // beam = beam + target
			TLorentzVector R1    = miss2 + proton; 
			TLorentzVector R2    = miss2 + pip; 

    
            // calculating missing particle 4vector
			if(proton.E()==0 || pip.E()==0) {cout<<"HHypPPipProjector::  empty particle"<<endl; continue;}

			
            // 4vectors in CM system
            TLorentzVector miss2_cm = miss2; 
            TLorentzVector proton_cm = proton; 
            TLorentzVector pip_cm = pip; 
            miss2_cm.Boost(0.0, 0.0, -(*beam).Beta() ); 
			proton_cm.Boost(0.0, 0.0, -(*beam).Beta() );
			pip_cm.Boost(0.0, 0.0, -(*beam).Beta() );

            Float_t tanTh1 = TMath::Tan(proton.Theta());
            Float_t tanTh2 = TMath::Tan(pip.Theta());

			TVector3 NppCM, NPi0Z;
			NppCM = (proton_cm.Vect()).Cross(pip_cm.Vect());
			NPi0Z = (miss2_cm.Vect()).Cross((*beam).Vect());
		    float PsiCM = (TMath::RadToDeg()*TMath::ACos(NppCM*NPi0Z/(NppCM.Mag()*NPi0Z.Mag())));
		
            if(simuflag==1)
	        {
                 Float_t fpp[]={ sector_p, sector_pip, system_p, system_pip,
                                 proton.P(), proton.Theta(), proton.Phi(),
                                 pip.P(), pip.Theta(), pip.Phi(),
								 z_p, r_p, z_pip, r_pip,
                                 vertex.X(), vertex.Y(), vertex.Z(), dist_ppip,
                                 miss2.M(), miss2.P(), (proton+pip).M(), fabs(v1.Phi() - v2.Phi()),
                                 tanTh1*tanTh2, miss2_cm.CosTheta(),
                                 beta_p, beta_pip, 
                                 RKchiq_p, RKchiq_pip, ncomb, dsf, triggerBit, date, time, 
								 InnerMDCchiq_p, InnerMDCchiq_pip,
                                 geantID_p, geant_parentID_p, geant_grandparentID_p, 
                                 geantID_pip, geant_parentID_pip, geant_grandparentID_pip, 
								 geninfo_p, geninfo1_p, geninfo2_p, 
								 geninfo_pip, geninfo1_pip, geninfo2_pip 
	                           }; 
                 pp->Fill(fpp);
	        }
	        else
	        {
                 Float_t fpp[]={ sector_p, sector_pip, system_p, system_pip,
                                 proton.P(), proton.Theta(), proton.Phi(),
                                 pip.P(), pip.Theta(), pip.Phi(),
								 z_p, r_p, z_pip, r_pip,
                                 vertex.X(), vertex.Y(), vertex.Z(), dist_ppip,
                                 miss2.M(), miss2.P(), (proton+pip).M(), fabs(v1.Phi() - v2.Phi()),
                                 tanTh1*tanTh2, miss2_cm.CosTheta(), (proton_cm+pip_cm).CosTheta(),
                                 beta_p, beta_pip, 
                                 RKchiq_p, RKchiq_pip, ncomb, dsf, triggerBit, date, time, 
								 InnerMDCchiq_p, InnerMDCchiq_pip
                               };

                 pp->Fill(fpp);
             }
         } else  cerr << algoName << " got no TLorentzVector " << endl;
    }
    #endif
    return kTRUE;
}

Bool_t HHypPPipProjector::init()
{
    cerr<<" HHypPPipProjector::init() "<<endl;

	enLossCorr.setDefaultPar("jan04");

    simCat = gHades->getCurrentEvent()->getCategory(catGeantKine);

    if (!simCat)
    {
	    simuflag = 0;
	}
	else
	{
        simuflag = 1;
        cout << "Projector uses SIMULATION" << endl;
        CatPartSim = NULL;          // Category
        if ((CatPartSim =
             gHades->getCurrentEvent()->getCategory(catPidPart)) == NULL) {
            Error("init", "Cannot get catPidPartSim cat");
            return kFALSE;
        }
    }

    // need to get name from channel
    TString input(channel->Get(initList));
	TFile *f=GetHFile();
	f->cd();
    if(simuflag==1)
    {
        pp =  new TNtuple(input + TString("_ppip"),
                "sector_p:sector_pip:system_p:system_pip:P_p:Th_p:Ph_p:P_pip:Th_pip:Ph_pip:z_p:r_p:z_pip:r_pip:xvert:yvert:zvert:dist_ppip:m_miss2:p_miss2:m_inv:dphi:tanT1tanT2:miss_cosThCM:inv_cosThCM:beta_p:beta_pip:RKchiq_p:RKchiq_pip:ncomb:dsf:triggerBit:date:time:InnerMDCchiq_p:InnerMDCchiq_pip:geant_ID_p:geant_parentID_p:geant_grandparentID_p:geant_ID_pip:geant_parentID_pip:geant_grandparentID_pip:geninfo_p:geninfo1_p:geninfo2_p:geninfo_pip:geninfo1_pip", 
                "sector_p:sector_pip:system_p:system_pip:P_p:Th_p:Ph_p:P_pip:Th_pip:Ph_pip:z_p:r_p:z_pip:r_pip:xvert:yvert:zvert:dist_ppip:m_miss2:p_miss2:m_inv:dphi:tanT1tanT2:miss_cosThCM:inv_cosThCM:beta_p:beta_pip:RKchiq_p:RKchiq_pip:ncomb:dsf:triggerBit:date:time:InnerMDCchiq_p:InnerMDCchiq_pip:geant_ID_p:geant_parentID_p:geant_grandparentID_p:geant_ID_pip:geant_parentID_pip:geant_grandparentID_pip:geninfo_p:geninfo1_p:geninfo2_p:geninfo_pip:geninfo1_pip"); 
	}
	else
	{
        pp =  new TNtuple(input + TString("_ppip"),
                "sector_p:sector_pip:system_p:system_pip:P_p:Th_p:Ph_p:P_pip:Th_pip:Ph_pip:z_p:r_p:z_pip:r_pip:xvert:yvert:zvert:dist_ppip:m_miss2:p_miss2:m_inv:dphi:tanT1tanT2:miss_cosThCM:inv_cosThCM:beta_p:beta_pip:RKchiq_p:RKchiq_pip:ncomb:dsf:triggerBit:date:time:InnerMDCchiq_p:InnerMDCchiq_pip", 
                "sector_p:sector_pip:system_p:system_pip:P_p:Th_p:Ph_p:P_pip:Th_pip:Ph_pip:z_p:r_p:z_pip:r_pip:xvert:yvert:zvert:dist_ppip:m_miss2:p_miss2:m_inv:dphi:tanT1tanT2:miss_cosThCM:inv_cosThCM:beta_p:beta_pip:RKchiq_p:RKchiq_pip:ncomb:dsf:triggerBit:date:time:InnerMDCchiq_p:InnerMDCchiq_pip"); 
	}
    return kTRUE;
}

Bool_t HHypPPipProjector::reinit()
{
    return kTRUE;
}

Bool_t HHypPPipProjector::finalize()
{
    pp->Write();
    return kTRUE;
}

Bool_t HHypPPipProjector::IsOpposit(Short_t sec1, Short_t sec2)
{
   if(sec1==0 && sec2==3) return 1; else
   if(sec1==1 && sec2==4) return 1; else
   if(sec1==2 && sec2==5) return 1; else
   if(sec1==3 && sec2==0) return 1; else
   if(sec1==4 && sec2==1) return 1; else
   if(sec1==5 && sec2==2) return 1; else
   return 0;
}
			  
Bool_t HHypPPipProjector::SetParamFile(TString pFile)
{
    paramFile=pFile;
}

Float_t HHypPPipProjector::calcVertex(HPidParticle *p1, HPidParticle *p2,
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

