// File: hgeantkineana.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/02/19 00:24:13
//
#ifndef HGEANTKINEANA_H
#define HGEANTKINEANA_H
#include <TH2.h>
#include <TH3.h>
#include <TFile.h>
#include "hreconstructor.h"
class HGeantKine;
class HCategory;
class HIterator;
class HGeantKine;
class HGeantKineAna : public HReconstructor {
public:

	HGeantKineAna();
        HGeantKineAna(Text_t *name,Text_t *title);
        HGeantKineAna(Text_t *name,Text_t *title,char* filename);
	~HGeantKineAna();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        void dumpKine(HGeantKine*);
        void findPionDalitzLeptons();
        void findPionGammaConversionLeptons();
        void calcParticleAnglesT(HGeantKine*,Float_t&,Float_t&);
        void calcParticleAnglesV(HGeantKine*,Float_t&,Float_t&);
        Float_t calcOpeningAngleT(Float_t,Float_t,Float_t,Float_t);
        Float_t calcOpeningAngleV(HGeantKine*,HGeantKine*);
        void iniCounters();
        void iniSwitches();
        void iniControlHistos();
        HCategory* getGeantKineCat(){return fGeantKineCat;}
        void setGeantKineCat(HCategory* cat){fGeantKineCat = cat;}
        HGeantKine* findParent(HGeantKine*);
        void saveHistos();
protected:
    HCategory *fGeantKineCat; //!
    HIterator *iter_kine; //!
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
    // counters
    Int_t nCounterProcessedNbEvents;
    // switches
    Bool_t kDumpIt;
    //FILENAME
    char *pFileName;//!
    ClassDef(HGeantKineAna,1)



};

#endif //HGEANTKINEANA_H



