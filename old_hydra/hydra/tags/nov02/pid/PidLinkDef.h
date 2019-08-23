// @(#)$Id: PidLinkDef.h,v 1.4 2002-12-03 14:51:03 jaskula Exp $
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link off all enum;

#pragma link C++ class HPidTrackCand;
#pragma link C++ class HPidGeantTrackSet;
#pragma link C++ class HPidTrackFiller;
#pragma link C++ class HPidTrackFillerSim;
#pragma link C++ class HPidAlgorithm;
#pragma link C++ class HPidReconstructor;
#pragma link C++ class HPidCandidate;
#pragma link C++ class HPidAlgMomVsBeta;

#pragma link C++ class HPidCandidate_3_8;
#pragma link C++ class HPidCandidate_4_8;
#pragma link C++ class HPidCandidate_1_8;
#pragma link C++ class HPidCandidate_5_8;

#pragma link C++ class HPidContFact;
#pragma link C++ class HPidParRootFileIo;
#pragma link C++ class HPidParAsciiFileIo;
#pragma link C++ class HPidParCont;
#pragma link C++ class HPidHistogramsCont-;

#pragma link C++ class HPidTrackFillerPar-;
#pragma link C++ class HPidReconstructorPar;
#pragma link C++ class HPidAlgMomVsBetaPar;

#pragma link C++ class HPidGaussConf;

#pragma link C++ class HPidPhysicsConstants;

#pragma link C++ class HPidParticle-;
#pragma link C++ class HPidParticleSim;
#pragma link C++ class HPidParticleSimExt;
#pragma link C++ class HPidParticleFiller;
#pragma link C++ class HPidParticleFillerSim;

#pragma link C++ class HPidDilepton;
#pragma link C++ class HPidDileptonFiller;

#pragma link C++ global catPidTrackCand;
#pragma link C++ global catPidCandidate;
#pragma link C++ global catPidPart;

#pragma link C++ enum EnumPidAlgorithm_t;

#endif
