// File: hdihitmatchsim.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/04/06 19:39:25
//
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

