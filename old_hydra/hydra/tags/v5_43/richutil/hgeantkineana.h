// File: hgeantkineana.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/08/29 21:59:56
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
    // counters
    Int_t nCounterProcessedNbEvents;
    // switches
    Bool_t kDumpIt;
    //FILENAME
    char *pFileName;//!
    ClassDef(HGeantKineAna,1)



};

#endif //HGEANTKINEANA_H



