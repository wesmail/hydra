// $Id: hhitmatchheadersim.cc,v 1.5 2004-10-15 08:25:15 eberl Exp $
// Last update by Thomas Eberl: 04/08/02 14:29:01
//
using namespace std;
#include "hhitmatchheadersim.h"
#include <iostream> 
#include <iomanip>
ClassImp(HHitMatchHeaderSim)

HHitMatchHeaderSim::HHitMatchHeaderSim() {
    reset();
}

HHitMatchHeaderSim::~HHitMatchHeaderSim() {}

HHitMatchHeaderSim::HHitMatchHeaderSim(const HHitMatchHeaderSim& source) {

}

HHitMatchHeaderSim& HHitMatchHeaderSim::operator=(const HHitMatchHeaderSim& source) {
 if (this != &source) {
 }
 
 return *this;
}
void HHitMatchHeaderSim::dumpToStdoutSim()
{
    HHitMatchHeader::dumpToStdout();
    cout<<"-------------- HHitMatchHeaderSim --------------"<<endl;
    if(nNb_Part) cout<<"Nb of particles: "<<nNb_Part<<endl;
    if(fAvrgRichWeight) cout<<"Avrg Rich Weight: "
			    <<fAvrgRichWeight<<endl;
    if(fAvrgMdcWeight) cout<<"Avrg Mdc Weight: "
			   <<fAvrgMdcWeight<<endl;
    if(nNb_Part) 
    {
	cout<<"Particle ID/# ... ";
	for(Int_t n=0;n<MAXPARIDS;n++) 
	{
	    if(nParIdArr[n]>0) cout<<n<<"/"<<nParIdArr[n]<<"  ";
	}
	cout<<endl;
    }
    if(nNbFakes)  cout<<"Nb of fakes: "<<nNbFakes<<endl;
    if(nGNb_RM)   cout<<"Nb of RM: "<<nGNb_RM<<endl;
    if(nGNb_RT_S) cout<<"Nb of R (T||S): "<<nGNb_RT_S<<endl;
    if(nGNb_MT_S) cout<<"Nb of M (T||S): "<<nGNb_MT_S<<endl;
    if(nGNb_RMT_S) cout<<"Nb of RM(T||S): "<<nGNb_RMT_S<<endl;
    if(imppar>-1) cout<<"Impact Parameter: "<<imppar<<endl;
    cout<<"------------------------------------------------"<<endl;
    
}

void HHitMatchHeaderSim::resetSim(void) 
{
    nNb_Part=0;
    fAvrgRichWeight=0.;
    fAvrgMdcWeight=0.;
    for(Int_t k=0;k<MAXPARIDS;k++) nParIdArr[k]=0;
    nGNb_RM=nGNb_RT_S=nGNb_MT_S=nGNb_RMT_S=0;
    nNbFakes=0;
    imppar=-1;
}






