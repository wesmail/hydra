#ifndef HPARTICLESIMFILLER_H
#define HPARTICLESIMFILLER_H

#include "hreconstructor.h"
#include "iostream.h"
#include "hlocation.h"

class HIterator;
class HCategory;
class HSpecGeomPar;

class HParticleSimFiller : public HReconstructor {

private:
  HCategory* fPartCat;      //! Pointer to Particle data category
  HCategory* fKickTrackCat; //! Pointer to KickTrack data category
  HIterator* iter;          //! Iterator over KickTrack category
  HSpecGeomPar* fSpecGeometry;          //! Geometry needed for rotation of kick track data

public:
  HParticleSimFiller(void);
  HParticleSimFiller(Text_t* name,Text_t* title);
  ~HParticleSimFiller(void) {;}

  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t  execute(void);

public:
  ClassDef(HParticleSimFiller,1) // Filler for HParticleSim container
};

#endif
