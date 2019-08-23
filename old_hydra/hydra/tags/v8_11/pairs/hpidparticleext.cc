using namespace std;
#include "hpidparticleext.h"

// -----------------------------------------------------------------------------

ClassImp(HPidParticleExt)

    HPidParticleExt::HPidParticleExt(HPidParticle* pParticleSource)
	:itsParticle(*(pParticleSource)),
	 itsHitData(*(pParticleSource->getTrackCand()->getHitData())),
	 itsTrackData(*(pParticleSource->getTrackCand()->getTrackData()))
{
    
}
