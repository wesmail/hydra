#ifndef HRICHCALSIM_H
#define HRICHCALSIM_H

#include "hrichcal.h"

class HRichCalSim : public HRichCal {
protected:
    Int_t nTrack1;   // index of track nb. for first hit
    Int_t nTrack2;   // index of track nb. for last hit

public:
    HRichCalSim(void) : nTrack1(0), nTrack2(0) {}
    ~HRichCalSim(void) {}
    void clear();
    inline void setNTrack1(const Int_t n) {nTrack1=n;} //set trk nb of hit 1
    inline void setNTrack2(const Int_t n) {nTrack2=n;} //set trk nb of hit 2
    inline Int_t getNTrack1(void) const {return nTrack1;} //get trk nb of hit 1
    inline Int_t getNTrack2(void) const {return nTrack2;} //get trk nb of hit 2

ClassDef(HRichCalSim,1) // RICH simulated cal data object
};


#endif /* !HRICHCALSIM_H */
