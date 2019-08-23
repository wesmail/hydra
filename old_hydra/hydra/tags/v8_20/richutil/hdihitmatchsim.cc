// $Id: hdihitmatchsim.cc,v 1.5 2004-10-15 08:25:14 eberl Exp $
// Last update by Thomas Eberl: 03/07/17 21:42:50
//
using namespace std;
#include "hdihitmatchsim.h"
#include "hhitmatchsim.h"
#include "hlinearcategory.h"
#include "hrichutilfunc.h"
#include "richdef.h"
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
    HLinearCategory * cat = (HLinearCategory*) HRichUtilFunc::getCategory(catMatchHit);
    HHitMatchSim *t1 = 0;
    HHitMatchSim *t2 = 0;
    if (cat)
    {
	t1 = (HHitMatchSim*)cat->getObject(this->getIndTrk1());
	if (!t1) Error("HDiHitMatchSim","HHitMatchSim object 1 not found");
	t2 = (HHitMatchSim*)cat->getObject(this->getIndTrk2());
	if (!t2) Error("HDiHitMatchSim","HHitMatchSim object 2 not found");
    }
    else Error("","no category found");
    if (t1&&t2)
    {
	t1->dumpToStdoutSim();
	t2->dumpToStdoutSim();
    }
    
}

void HDiHitMatchSim::resetSim(void) 
{
}

