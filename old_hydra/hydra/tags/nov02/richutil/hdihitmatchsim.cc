// $Id: hdihitmatchsim.cc,v 1.2 2002-09-25 16:19:52 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 17:26:15
//
#include "hdihitmatchsim.h"
#include "hhitmatchsim.h"
#include <iostream.h>

ClassImp(HDiHitMatchSim)

HDiHitMatchSim::HDiHitMatchSim() {
    reset();//default init of data members
    
}

HDiHitMatchSim::~HDiHitMatchSim() {}

HDiHitMatchSim::HDiHitMatchSim(const HDiHitMatchSim& source) {

}

HDiHitMatchSim& HDiHitMatchSim::operator=(const HDiHitMatchSim& source) {
 if (this != &source) {
 }
 
 return *this;
}
void HDiHitMatchSim::dumpToStdoutSim()
{
    dumpToStdout();
}

void HDiHitMatchSim::resetSim(void) 
{
}

