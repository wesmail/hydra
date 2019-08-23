// File: hdihitmatchsim.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/04/06 19:03:05
//
#ifndef HDIHITMATCHSIM_H
#define HDIHITMATCHSIM_H
#include "hdihitmatch.h"

class HHitMatchSim;
class HDiHitMatchSim: public HDiHitMatch {

protected:

public:

    HDiHitMatchSim();
    HDiHitMatchSim(HHitMatchSim*, HHitMatchSim*);
    virtual ~HDiHitMatchSim();

    HDiHitMatchSim(const HDiHitMatchSim& source);
    virtual HDiHitMatchSim& operator=(const HDiHitMatchSim& source);

    void resetSim();
    void dumpToStdoutSim();

    
 ClassDef(HDiHitMatchSim,1)
};


#endif // HDIHITMATCHSIM_H
