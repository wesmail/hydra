// File: hhitmatchsim.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/08 16:36:49
//
//**********************************************************
// this class is a container class meant to hold one
// "correlation" between different detectors 
//**********************************************************
#include "hhitmatchsim.h"
#include <iostream.h>
ClassImp(HHitMatchSim)

HHitMatchSim::HHitMatchSim() {
    reset();
}

HHitMatchSim::~HHitMatchSim() {}

HHitMatchSim::HHitMatchSim(const HHitMatchSim& source) {

}

HHitMatchSim& HHitMatchSim::operator=(const HHitMatchSim& source) {
 if (this != &source) {
 }
 
 return *this;
}
void HHitMatchSim::dumpToStdout()
{
    HHitMatch::dumpToStdout();
}

void HHitMatchSim::reset(void) 
{
    HHitMatch::reset();
 
}






