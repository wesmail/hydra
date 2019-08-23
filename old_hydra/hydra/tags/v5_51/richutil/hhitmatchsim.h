// File: hhitmatchsim.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/26 10:53:31
//
#ifndef HHITMATCHSIM_H
#define HHITMATCHSIM_H

#include "hhitmatch.h"
#include "htrackinfo.h"

class HHitMatchSim: public HHitMatch {

protected :
    
HTrackInfo trackMatched;//info from GEANT
Int_t isFakeFlag;
Int_t isLeptonFlag;
    //Int_t isHadronFlag;
public:
   
    HTrackInfo * getTrackInfoObj() {return & trackMatched;};

public:

 HHitMatchSim();
 virtual ~HHitMatchSim();

 HHitMatchSim(const HHitMatchSim& source);

 virtual HHitMatchSim& operator=(const HHitMatchSim& source);
 void resetSim();
 void dumpToStdoutSim();
 Int_t isFake(){return isFakeFlag;};
 void setFakeFlag(Int_t i){isFakeFlag=i;};
 Int_t isLepton(){return isLeptonFlag;};
 void setLeptonFlag(Int_t i){isLeptonFlag=i;};
//  Int_t isHadron(){return isHadronFlag;};
//  void setHadronFlag(Int_t i){isHadronFlag=i;};
 ClassDef(HHitMatchSim,1)
};


#endif // HHITMATCHSIM_H
