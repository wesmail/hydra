// @(#)$Id: PidLinkDef.h,v 1.35 2009-02-13 19:22:14 halo Exp $
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link off all enum;

#pragma link C++ class HPidTaskSet;
#pragma link C++ class HPidHitData;           
#pragma link C++ class HPidTrackData-;
#pragma link C++ class HPidTrackCand-;          
#pragma link C++ class HPidTrackCandSim;      
#pragma link C++ class HPidTrackFiller;       
#pragma link C++ class HPidSkipTrigger;       
#pragma link C++ class HPidParticle-;
#pragma link C++ class HPidParticleSim;       
#pragma link C++ class HPidParticleFiller;    
#pragma link C++ class HPidEvtInfoFiller;    
//#pragma link C++ class HPidTrackEval;
//#pragma link C++ class HPidDynamic;
//#pragma link C++ class HPidDynFiller;
//#pragma link C++ class HPidDynUpdate;

#pragma link C++ class HPidGaussConf;
#pragma link C++ class HPidCandidate-;
#pragma link C++ class HPidReconstructor;       
#pragma link C++ class HPidReconstructorPar;
#pragma link C++ class HPidTrackFillerPar;      
#pragma link C++ class HPidContFact;            
#pragma link C++ class HPidGeantTrackSet;         
#pragma link C++ class HPidParRootFileIo;       
#pragma link C++ class HPidParAsciiFileIo;     
#pragma link C++ class HPidParCont;             
#pragma link C++ class HPidReconstructorPar;    
#pragma link C++ class HPidHistogramsCont-;         
#pragma link C++ class HPidTofRec;              
#pragma link C++ class HPidMdcStart;
#pragma link C++ class HPidAlgRingPropertiesPar;
#pragma link C++ class HPidAlgMdcEloss;
#pragma link C++ class HPidAlgMdcElossPar;
#pragma link C++ class HPidAlgShower;           
#pragma link C++ class HPidAlgShowerPar;        
#pragma link C++ class HPidAlgRingProperties; 
#pragma link C++ class HPidAlgMomVsBetaPar; 
#pragma link C++ class HPidAlgMomVsBeta;  
#pragma link C++ class HPidAlgorithm;     
#pragma link C++ class HPidAlgStandCuts;    
#pragma link C++ class HPidAlgStandCutsPar;  
#pragma link C++ class HPidAlgHardCuts;
#pragma link C++ class HPidAlgHardCutsPar-;
//#pragma link C++ class HPidCommonTrack;
#pragma link C++ class HPidDilepton;
#pragma link C++ class HPidCPParam;
#pragma link C++ class HPidEvtInfo-;

#pragma link C++ class HPidTrackSorter;
#pragma link C++ class HPidTrackCleaner;

#pragma link C++ global catPidTrackCand;
#pragma link C++ global catPidCandidate;
#pragma link C++ global catPidPart;
#pragma link C++ global catPidDilepton;
#pragma link C++ global catPidPoorCandidate;
#pragma link C++ global catPidDynamic;
#pragma link C++ global catPidEvtInfo;


#pragma link C++ enum EnumPidAlgorithm_t;

#endif
