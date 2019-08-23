// File: htrackinfo.h
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 01/10/06 21:14:44
//
#ifndef HTRACKINFO_H
#define HTRACKINFO_H

#define RICHMAXTRACKS 3
#define MDCMAXTRACKS 5
#define TOFMAXTRACKS 2
#define SHOWERMAXTRACKS 5
#define MAXPARTICLES 5
#include <TObject.h> 
#include <TArrayI.h>   

class HTrackInfo: public TObject {

private:

    Int_t parId[MAXPARTICLES];//GEANT particle 
    Int_t trackNr[MAXPARTICLES];//GEANT track number
    Float_t richWeight[MAXPARTICLES];//ratio fired pads by trk nb to total nb pads
    Float_t mdcWeight[MAXPARTICLES];//ratio nb of drift times by trk nb to total
    Int_t iMatchedRM[MAXPARTICLES];//flag indicating a GEANT trk number matching
    Int_t iMatchedRT_S[MAXPARTICLES];
    Int_t iMatchedMT_S[MAXPARTICLES];
    Int_t iMatchedRMT_S[MAXPARTICLES];
    
public:
    HTrackInfo();
    ~HTrackInfo();
    void dumpToStdout();
    void reset();
    Int_t getPartNr();

    Int_t getParId(Int_t index) {return parId[index];};
    Int_t getTrkNb(Int_t index) {return trackNr[index];};

    Float_t getRichWeight(Int_t index) {return richWeight[index];};
    Float_t getMdcWeight(Int_t index) {return mdcWeight[index];};

    Int_t getMatchedRM(Int_t index) {return iMatchedRM[index];};
    Int_t getMatchedRT_S(Int_t index) {return iMatchedRT_S[index];};
    Int_t getMatchedMT_S(Int_t index) {return iMatchedMT_S[index];};
    Int_t getMatchedRMT_S(Int_t index) {return iMatchedRMT_S[index];};


    void setParId(Int_t ind ,Int_t id) {parId[ind] = id;};
    void setTrkNb(Int_t index, Int_t track){ trackNr[index] = track;};

    void setRichWeight(Int_t index,Float_t weight){richWeight[index] = weight;};
    void setMdcWeight(Int_t index,Float_t weight){mdcWeight[index] = weight;};

    void setMatchedRM(Int_t index) {iMatchedRM[index] = 1;};
    void setMatchedRT_S(Int_t index) {iMatchedRT_S[index] = 1;};
    void setMatchedMT_S(Int_t index) {iMatchedMT_S[index] = 1;};
    void setMatchedRMT_S(Int_t index) {iMatchedRMT_S[index] = 1;};

    
    ClassDef(HTrackInfo,1)
};

#endif /* !HTRACKINFO_H */


