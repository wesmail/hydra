// $Id: htrackinfo.h,v 1.9 2008-09-18 13:05:36 halo Exp $
// Last update by Thomas Eberl: 04/08/02 13:01:37
//
#ifndef HTRACKINFO_H
#define HTRACKINFO_H

#define RICHMAXTRACKS 3
#define MDCMAXTRACKS 5
#define TOFMAXTRACKS 2
#define SHOWERMAXTRACKS 5
#define MAXPARTICLES 5
#include "TObject.h" 

class HTrackInfo: public TObject {

private:

   Int_t parId[MAXPARTICLES];//GEANT particle 
    Int_t trackNr[MAXPARTICLES];//GEANT track number
    Int_t creaId[MAXPARTICLES];//GEANT ID of creator particle of this particle
    Int_t creaNb[MAXPARTICLES];//GEANT trk nb of creator particle
    Int_t mech[MAXPARTICLES];//GEANT mech of creation of this particle
    Int_t med[MAXPARTICLES];//GEANT med of creation of this particle
    Float_t vertx[MAXPARTICLES];//GEANT vertex of particle x
    Float_t verty[MAXPARTICLES];//GEANT vertex of particle y
    Float_t vertz[MAXPARTICLES];//GEANT vertex of particle z
    Float_t totMom[MAXPARTICLES];//GEANT calc total mom of particle
    Int_t isPi0Dalitz[MAXPARTICLES];//GEANT checked where lep comes from
    Int_t isConvLep[MAXPARTICLES];//GEANT
    Float_t richWeight[MAXPARTICLES];//ratio fired pads by trk nb to total nb pads
    Float_t mdcWeight[MAXPARTICLES];//ratio nb of drift times by trk nb to total
    Int_t iMatchedRM[MAXPARTICLES];//flag indicating a GEANT trk number matching
    Int_t iMatchedRT_S[MAXPARTICLES];
    Int_t iMatchedMT_S[MAXPARTICLES];
    Int_t iMatchedRMT_S[MAXPARTICLES];
    Int_t numPhot[MAXPARTICLES];
    Int_t isGCLepInMDC[MAXPARTICLES];  


public:
    HTrackInfo();
    ~HTrackInfo();

 HTrackInfo(const HTrackInfo& source);
virtual HTrackInfo& operator=(const HTrackInfo& source);

    void dumpToStdout();
    void reset();
    Int_t getPartNr();
    Int_t calcCorrCode(Int_t);
    Int_t getParId(Int_t index) {return parId[index];};
    Int_t getTrkNb(Int_t index) {return trackNr[index];};
    Int_t getMech(Int_t index){return mech[index];};
    Int_t getMed(Int_t index){return med[index];};
    Float_t getVertx(Int_t index){return vertx[index];};
    Float_t getVerty(Int_t index){return verty[index];};
    Float_t getVertz(Int_t index){return vertz[index];};
    Int_t getCreaTrkNb(Int_t index){return  creaNb[index];};
    Int_t getCreaId(Int_t index){return  creaId[index];};
    Float_t getTotMom(Int_t index){return totMom[index];};
    Float_t getRichWeight(Int_t index) {return richWeight[index];};
    Float_t getMdcWeight(Int_t index) {return mdcWeight[index];};
    Int_t getPi0Dalitz(Int_t index) {return isPi0Dalitz[index];};
    Int_t getConvLep(Int_t index) {return isConvLep[index];};
    Int_t getMatchedRM(Int_t index) {return iMatchedRM[index];};
    Int_t getMatchedRT_S(Int_t index) {return iMatchedRT_S[index];};
    Int_t getMatchedMT_S(Int_t index) {return iMatchedMT_S[index];};
    Int_t getMatchedRMT_S(Int_t index) {return iMatchedRMT_S[index];};
    Int_t getNumPhot(Int_t index) {return numPhot[index];};
    Int_t getGCLepInMDC(Int_t index) {return isGCLepInMDC[index];};
    void setNumPhot(Int_t index, Int_t p) { numPhot[index] = p;};
    void setGCLepInMDC(Int_t index, Int_t flag){ isGCLepInMDC[index]=flag;};
    void setParId(Int_t index, Int_t id) {parId[index] = id;};
    void setTrkNb(Int_t index, Int_t track){ trackNr[index] = track;};
    void setMech(Int_t index, Int_t m){mech[index] = m;};
    void setMed(Int_t index, Int_t m){med[index] = m;};
    void setVertx(Int_t index, Float_t x){vertx[index] = x;};
    void setVerty(Int_t index, Float_t y){verty[index] = y;};
    void setVertz(Int_t index, Float_t z){vertz[index] = z;};
    void setCreatorTrkNb(Int_t index, Int_t ctrk){ creaNb[index] = ctrk;};
    void setCreatorId(Int_t index, Int_t cid){ creaId[index] = cid;};
    void setTotMom(Int_t index, Float_t mom){totMom[index] = mom;};
    void setRichWeight(Int_t index, Float_t weight){richWeight[index] = weight;};
    void setMdcWeight(Int_t index, Float_t weight){mdcWeight[index] = weight;};
    void setPi0Dalitz(Int_t index, Int_t flag) {isPi0Dalitz[index]=flag;};
    void setConvLep(Int_t index, Int_t flag) {isConvLep[index]=flag;};
    void setMatchedRM(Int_t index) {iMatchedRM[index] = 1;};
    void setMatchedRT_S(Int_t index) {iMatchedRT_S[index] = 1;};
    void setMatchedMT_S(Int_t index) {iMatchedMT_S[index] = 1;};
    void setMatchedRMT_S(Int_t index) {iMatchedRMT_S[index] = 1;};

   
    ClassDef(HTrackInfo,1)
};

#endif /* !HTRACKINFO_H */


