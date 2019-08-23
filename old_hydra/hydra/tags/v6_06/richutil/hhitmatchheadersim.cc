// File: hhitmatchheadersim.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/10/25 16:44:00
//

#include "hhitmatchheadersim.h"
#include <iostream.h>
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
    cout<<"------------------------------------------------"<<endl;
    
}

void HHitMatchHeaderSim::resetSim(void) {
nNb_Part=0;
fAvrgRichWeight=0.;
fAvrgMdcWeight=0.;
for(Int_t k=0;k<MAXPARIDS;k++) nParIdArr[k]=0;
nGNb_RM=nGNb_RT_S=nGNb_MT_S=nGNb_RMT_S=0;
nNbFakes=0;


}






