// $Id: hdihitmatchsim.cc,v 1.4 2003-01-24 18:24:12 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:26:15
//
using namespace std;
#include "hdihitmatchsim.h"
#include "hhitmatchsim.h"
#include <iostream> 
#include <iomanip>

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

