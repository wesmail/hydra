// $Id: hgeantparticleinfo.h,v 1.1 2003-09-09 15:39:39 eberl Exp $
// Last update by Thomas Eberl: 03/07/07 07:12:36
//
#ifndef HGEANTPARTICLEINFO_H
#define HGEANTPARTICLEINFO_H

#define TRACKS 5

#include <TObject.h> 

class HGeantParticleInfo: public TObject {

private:

    Int_t geantTrackNr;//GEANT track number
    Int_t tracks[TRACKS];//indices of recon tracks
    Int_t corrCode[TRACKS];//corr code of GEANT particle in resp. recon track

public:
    HGeantParticleInfo();
    ~HGeantParticleInfo();

 HGeantParticleInfo(const HGeantParticleInfo& source);
virtual HGeantParticleInfo& operator=(const HGeantParticleInfo& source);

    void dumpToStdout();
    void reset();
    Int_t getTrackNr();
    Int_t getGeantTrackNr(){return geantTrackNr;};
    Int_t getReconTrk(Int_t index) {return tracks[index];};
    Int_t getCorrCode(Int_t index) {return corrCode[index];};

    void setGeantTrackNr(Int_t index) {geantTrackNr = index;};
    void setReconTrk(Int_t index, Int_t id) {tracks[index] = id;};
    void setCorrCode(Int_t index, Int_t track){corrCode[index] = track;};

    ClassDef(HGeantParticleInfo,1)
};

#endif /* !HGEANTPARTICLEINFO_H */


