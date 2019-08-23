//*-- AUTHOR: D.Magestro
//*-- Created: 28/05/01
//*-- Modified: 07/11/01
#ifndef HPARTICLEFILLER_H
#define HPARTICLEFILLER_H
using namespace std;
#include "hreconstructor.h"
#include <iostream> 
#include <iomanip>
#include "hlocation.h"

class HIterator;
class HCategory;
class HSpecGeomPar;
class HCutTrack;
class HCutLeptonId;
class HCutHadronId;
class HKickTrack;
class HRichHit;
class HParticle;

class HParticleFiller : public HReconstructor {

protected:
  HCategory* fPartCat;          //! Pointer to Particle data category
  HCategory* fKickTrackCat;     //! Pointer to KickTrack data category
  HCategory* fRichHitCat;       //! Pointer to KickTrack data category
  HCutTrack* trackCuts;         //  Cuts applied to tracks
  HCutLeptonId* leptonCuts;     //  Cuts applied to leptons
  HCutHadronId* hadronIdCuts;     //  Cuts applied to hadrons
  HIterator* iterTracks;        //! Iterator over KickTrack category
  HIterator* iterRings;         //! Iterator over KickTrack category
  HSpecGeomPar* fSpecGeometry;  //! Geometry needed for rotation of kick track data
  Bool_t kSkip;                 //! Option to skip events without certain requirements

public:
  HParticleFiller(void);
  HParticleFiller(Text_t* name,Text_t* title,Bool_t skip=kFALSE);
  ~HParticleFiller(void) {;}

  void fillMomenta(HKickTrack *track, HParticle *part);
  void fillVertex(HKickTrack *track, HParticle *part);
  void makeLepton(HRichHit *ring, HParticle *part);
  void setCutTrack(HCutTrack* cutTrack) { trackCuts = cutTrack; }
  HCutTrack* getCutTrack() { return trackCuts; }

  void setCutLeptonId(HCutLeptonId* cutLepton) { leptonCuts = cutLepton; }
  HCutLeptonId* getCutLeptonPid() { return leptonCuts; }

  void setCutHadronId(HCutHadronId* cutHadron) { hadronIdCuts = cutHadron; }
  HCutHadronId* getCutHadronPid() { return hadronIdCuts; }

  Bool_t init(void);
  Bool_t finalize(void) {return kTRUE;}
  Int_t  execute(void);


public:
  ClassDef(HParticleFiller,1) // Particle filler for HParticle container
};

#endif
