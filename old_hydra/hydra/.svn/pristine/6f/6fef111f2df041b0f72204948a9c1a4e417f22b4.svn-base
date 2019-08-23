#ifndef HPIDPARTICLEEXT_H
#define HPIDPARTICLEEXT_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidParticleExt                                                            //
//                                                                            //
// Extended container created for event mixing. Dirty solution!               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <TObject.h>

#include <hpidparticle.h>
#include <hpidtrackdata.h>
#include <hpidhitdata.h>

class HPidParticleExt: public TObject
{
 public:
    HPidParticleExt(){};
  HPidParticle itsParticle;
  HPidHitData itsHitData;
  HPidTrackData itsTrackData;

  HPidParticleExt(HPidParticle* pParticleSource);
  ~HPidParticleExt(void) {}
  
  const HPidHitData*    getHitData(void)   const { return &itsHitData;}
  const HPidTrackData*  getTrackData(void) const { return &itsTrackData;}
  const HPidParticle*   getPidParticle(void) const { return &itsParticle;}

  ClassDef(HPidParticleExt,1)
    
    } ;

#endif /* HPIDPARTICLEEXT_H */
