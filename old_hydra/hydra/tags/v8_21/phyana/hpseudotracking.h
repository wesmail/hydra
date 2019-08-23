// File: hpseudotracking.h
//
// Author: Jaro Bielcik <bielcik@gsi.de>
// Last update by Laura Fabbietti 06/02/01 19:30
// modified by T. Eberl <Thomas.Eberl@ph.tum.de> on
// Wed Jul 11 16:29:27 CEST 2001
// this class should look in sim data for tracks 
// which gave a hit in all detectors and fill 
// the hparticlesim container

#ifndef HPSEUDOTRACKING_H
#define HPSEUDOTRACKING_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "TArrayI.h"
#include "TArrayF.h"
#include "phyanadef.h"

class HIterator;
class HMdcLayerGeomPar;
class HTofDigitPar;
class HMdcCal1Sim;
class HTofHitSim;
class HRichHitSim;
class HShowerHitTrack;
class HGeantKine;

class HPseudoTracking : public HReconstructor {
private:


  //    HCategory* fRichCat;  

    HCategory* fTofCat; 
    HCategory* fMdcCat;
    HCategory* fShowerCat;
    HCategory* fTofinoCat;
    HCategory* fHGeantKine;
    HCategory* fParticle;
    HCategory* fRichCat;
    HCategory* fGeantKineCat;

    HIterator*  iter_kine;
    HIterator* iter_rich;
    HIterator* iter_tof;     
    HIterator* iter_mdc;     
    HIterator* iter_shower;     
    HIterator* iter_tofino;
    HIterator* iter;
    HIterator* iter_part;

    HCategory* getPartCat() { return fParticle; }
  
    
    TArrayI rich_trackn;
    TArrayI rich_tracknF;
    TArrayF rich_Theta;
    TArrayF rich_Phi;
    TArrayI * mdc_trackn;
    TArrayI * tof_trackn;
    TArrayI * shower_trackn;
 
    Int_t count_leptons;
    Int_t i_RJ;
    Int_t evtCount;
    void pseudoTracking();
    void fillRichTrackList();
    void scanRingTrack(HRichHitSim *richHit,Int_t track);
    void fillMdcTrackList();
    void fillTofTrackList();
    void fillShowerTrackList();
    void closeLepRej();
    void closeLepRejNew();
    Bool_t  checkThetaPhi(Float_t theta,Float_t phi);
    //void assignEvtNum(Int_t evt);

public:
    HPseudoTracking(void);
    HPseudoTracking(const Text_t* name,const Text_t* title,Int_t closeRej);
    ~HPseudoTracking(void);
    Bool_t init(void);
    Bool_t finalize(void);
    Int_t execute(void);
    void fillPart(TArrayI * pTrackArray);
    // Int_t getId(Int_t track);
    HGeantKine* getObj(Int_t nTrack);
    Float_t calcEnergy(Float_t px, Float_t py,Float_t pz,Int_t id);
      
public:
      ClassDef(HPseudoTracking,0) 
};

#endif





















