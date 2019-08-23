// $Id: hdihitmatchsim.h,v 1.5 2002-10-02 09:26:19 eberl Exp $
// Last update by Thomas Eberl: 02/10/02 11:23:39
//
#ifndef HDIHITMATCHSIM_H
#define HDIHITMATCHSIM_H
#include "hdihitmatch.h"

class HHitMatchSim;
class HDiHitMatchSim: public HDiHitMatch {

protected:

public:

    HDiHitMatchSim();
    HDiHitMatchSim(HHitMatchSim*, HHitMatchSim*){};
    virtual ~HDiHitMatchSim();

    HDiHitMatchSim(const HDiHitMatchSim& source);
    virtual HDiHitMatchSim& operator=(const HDiHitMatchSim& source);

    void resetSim();
    void dumpToStdoutSim();

    
 ClassDef(HDiHitMatchSim,1)
};


#endif // HDIHITMATCHSIM_H
