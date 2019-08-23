#ifndef HPARTICLEFILLER_H
#define HPARTICLEFILLER_H

#include "hreconstructor.h"
#include "iostream.h"
#include "hlocation.h"

class HIterator;
class HCategory;
class HSpecGeomPar;

class HParticleFiller : public HReconstructor {

private:
  HCategory* fPartCat;          //! Pointer to Particle data category
  HCategory* fKickTrackCat;     //! Pointer to KickTrack data category
  HCategory* fRichHitCat;       //! Pointer to KickTrack data category
  HIterator* iterTracks;        //! Iterator over KickTrack category
  HIterator* iterRings;         //! Iterator over KickTrack category
  HSpecGeomPar* fSpecGeometry;  //! Geometry needed for rotation of kick track data
  Bool_t kSkip;                 //! Option to skip events without certain requirements

public:
  HParticleFiller(void);
  HParticleFiller(Text_t* name,Text_t* title,Bool_t skip=kFALSE);
  ~HParticleFiller(void) {;}

  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t  execute(void);


public:
  ClassDef(HParticleFiller,1) // Particle filler for HParticle container
};

#endif
