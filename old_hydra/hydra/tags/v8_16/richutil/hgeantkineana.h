// $Id: hgeantkineana.h,v 1.9 2003-04-29 13:49:19 eberl Exp $
// Last update by Thomas Eberl: 03/02/06 12:45:11
//
#ifndef HGEANTKINEANA_H
#define HGEANTKINEANA_H
#include <TH2.h>
#include <TH3.h>
#include <TFile.h>
#include <TEventList.h>
#include <TObjArray.h>
#include "hreconstructor.h"
class HGeantKine;
class HParSet;
class HCategory;
class HIterator;
class HGeantKine;
class HRichHitSim;
class HGeantKineAna : public HReconstructor {
public:

	HGeantKineAna();
        HGeantKineAna(Text_t *name,Text_t *title);
        HGeantKineAna(Text_t *name,Text_t *title,char* filename,char *evtgen);
	~HGeantKineAna();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        Bool_t reconPionFromGammaConversionLeptons(TObjArray*);
        Int_t findAcceptedDalitzLeptons(TObjArray&);
        void  checkAcceptedDalitzLeptons(TObjArray&);
        Int_t getTracksForRings(TObjArray&,TObjArray&);
        Int_t getPairsForTracks(TObjArray&,TObjArray&);
        Int_t fillDalitzPairs(TObjArray&,TObjArray&);
        Int_t matchGEANTwithFoundPair(TObjArray&,TObjArray&);
        Int_t matchGEANTwithFoundTracks(TObjArray&,TObjArray&);
        Int_t matchFoundPairwithGEANT(TObjArray&,TObjArray&);
        Int_t matchFoundTrackswithGEANT(TObjArray&,TObjArray&);
        void dumpKine(HGeantKine*);
        void histoKine(HGeantKine*);
        void histoLepton(HGeantKine*,TH2D*);
        void histoLeptonPair(TObjArray&,TH2D*);
        void histoGammas(HGeantKine*,HGeantKine*);
        void findPionDalitzLeptons();
        TObjArray* getPionGammaConversionLeptons();
        void findPionGammaConversionLeptons();
        void showLeptonsFromPi0Decays(char*);
        void showGammasFromPi0Decays();
        HRichHitSim* checkRings(HGeantKine*);
        Int_t checkTrackPairs(TObjArray&,TObjArray&);
        Int_t checkForPair(TObjArray&,char*);
        Int_t checkRingCorrelation(HRichHitSim*,TObjArray&);
        void calcParticleAnglesT(HGeantKine*,Float_t&,Float_t&);
        void calcParticleAnglesV(HGeantKine*,Float_t&,Float_t&);
        Float_t calcOpeningAngleT(Float_t,Float_t,Float_t,Float_t);
        Float_t calcOpeningAngleV(HGeantKine*,HGeantKine*);
        void iniCounters();
        void iniSwitches();
        void iniControlHistos();
        HCategory* getGeantKineCat(){return fGeantKineCat;}
        void setGeantKineCat(HCategory* cat){fGeantKineCat = cat;}
        HCategory* getGeantMirrCat(){return fGeantRichMirrorCat;}
        void setGeantMirrCat(HCategory* cat){fGeantRichMirrorCat = cat;}
        HCategory* getGeantMDCCat(){return fGeantMdcCat;}
        void setGeantMDCCat(HCategory* cat){fGeantMdcCat = cat;}
        HCategory* getHitMatchCat(){return pHitMatchCat;}
        void setHitMatchCat(HCategory* cat){pHitMatchCat = cat;}
        HCategory* getDiHitMatchCat(){return pDiHitMatchCat;}
        void setDiHitMatchCat(HCategory* cat){pDiHitMatchCat = cat;}
        HGeantKine* findParent(HGeantKine*);
        void setGeoPar(HParSet* p){pGeoPar = p;};
        HCategory* getRichHitCat(){return fRichPID;};
        void saveHistos();
protected:
    
    HCategory *fGeantKineCat; //!
    HIterator *iter_kine; //!
    HCategory *fGeantRichMirrorCat; //!
    HIterator *iter_mirror; //!
    HCategory *fGeantMdcCat; //!
    HIterator *iter_mdcgeant; //!
    HCategory *fRichPID; //!
    HIterator *fRichIter; //!
    HCategory *pHitMatchCat; //!
    HIterator *pIterMatchHit; //!
    HCategory *pDiHitMatchCat; //!
    HIterator *pIterDiHitMatch; //!
    HParSet *pGeoPar; //!
    
    TFile *pFileOut;//!
    // histos
    TObjArray *pHistArray;//!
    TH2D *pHistElePosMomConv;//!
    TH1F *pHistOpenAngle;//!
    TH2D *pHistConvEleMom;//!
    TH2D *pHistConvPosMom;//!
    TH2D *pHistConvGammaMom;//!
    TH1F *pHistConvPairSource;//!
    TH3D *pHistElePosMomThetaConv;//!
    TH3D *pHistConvPairVertex;//!
    TH1F *g_conv_opangle;//!
    TH2F *deltaP_opangle;//!
    TH1D *med_conv;//!
    TH1D *mech_conv;//!
    TH1D *par_conv;//!
    TH2D *par_med_conv;//!
    TH2D *par_mech_conv;//!
    TH2D *mech_mom_conv;//!
    TH1D *med_dalitz;//!
    TH1D *mech_dalitz;//!
    TH1D *par_dalitz;//!
    TH2D *par_med_dalitz;//!
    TH2D *par_mech_dalitz;//!
    TH2D *mech_mom_dalitz;//!
    //
    TH1D *hidkine ;//!
    TH1D *hmedkine ;//!
    TH1D *hmechkine ;//!
    TH1D *hmomkine ;//!
    TH2D *hmomthetakine;//!
    TH2D *hmompairkine;//!
    TH2D *hthetapairkine;//!
    TH1D *hparentkine ;//!
    TH3D *hvertexkine ;//!
    TH1D *hcntdistx; //!
    TH1D *hcntdisty; //!
    TH2D *hcntdistxy; //!
    TH1D *hweight;//!
    TH2D *hweightdist;//!
    TH1D *hpi0invmass;//!
    TH1D *hdalitzopang;//!
    TH1D *hdalitzinvmass;//!
    //
    TH2D *hgammathetatheta; //!
    TH2D *hgammamommom; //!
    TH2D *hgammamom1theta2; //!
    TH2D *hgammamomtheta; //!
    // Eff
    TH2D *hmomthetaallsingle; //!
    TH2D *hmomthetareconsingle; //!
    TH2D *hmomthetaallpair; //!
    TH2D *hmomthetareconpair; //!
    // Conversion
    TH2D *hpolpol; //!
    TH2D *hpoldiffinvmass; //!
    TH2D *hmomdiffpoldiff; //!
    // Event list
    TEventList *el; //!
    // counters
    Int_t nEvtsProcessed;//!
    Int_t nbRings; //!
    Int_t nbLeptons; //!
    Int_t nbDalitzPairs; //!
    Int_t nbDalitzSingle; //!
    Int_t nbTracks; //!
    Int_t nbTrackPairs; //!
    Int_t nbgMatchedPairs; //! confirmed reconstr pairs
    Int_t nbgMatchedTracks; //!
    Int_t nbgMatchedGPairs; //! GEANT pairs reconstructed
    Int_t nbgMatchedGTracks; //!
    
    // switches
    Bool_t kDumpIt;
    //FILENAME
    char *pFileName;//!
    //EVT GEN (needed for cuts)
    char *pEvtGen;//!
    ClassDef(HGeantKineAna,1)



};

#endif //HGEANTKINEANA_H



