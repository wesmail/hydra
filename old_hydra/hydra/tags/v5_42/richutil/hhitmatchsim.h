// File: hhitmatchsim.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/09 08:39:49
//
#ifndef HHITMATCHSIM_H
#define HHITMATCHSIM_H

#include "hhitmatch.h"
#include "htrackinfo.h"

class HHitMatchSim: public HHitMatch {

protected :
    
HTrackInfo trackMatched;//info from GEANT
public:
   
    HTrackInfo * getTrackInfoObj() {return & trackMatched;};

public:

 HHitMatchSim();
 virtual ~HHitMatchSim();

 HHitMatchSim(const HHitMatchSim& source);

 virtual HHitMatchSim& operator=(const HHitMatchSim& source);
 virtual void reset();
 virtual void dumpToStdout(); 
 ClassDef(HHitMatchSim,1)
};


#endif // HHITMATCHSIM_H
