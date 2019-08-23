NOLIBNEW=ON

include $(HADDIR_BASEDIR)/Makefile.base

CXXFLAGS += -I$(ADSM_BASE)/Linux \
		-I$(HADDIR_HYP)

LIBS+=$(ROOTLIBS) -lProof -lTreePlayer -lMinuit 
LIBS+=$(ROOTGLIBS) 

RPATH=`echo "$(HADDIR_PID_LIB):\
	$(HADDIR_RICH_LIB):\
	$(HADDIR_RICHUTIL_LIB):\
	$(HADDIR_MDC_LIB):\
	$(ROOTSYS)/lib:\
	$(HADDIR_BASE_LIB):\
	$(HADDIR_KICKPLANE_LIB):\
	$(HADDIR_MDCUTIL_LIB):\
	$(HADDIR_MDCTRACKS_LIB):\
	$(HADDIR_MDCTRACKD_LIB):\
	$(HADDIR_MDCTRACKG_LIB):\
	$(HADDIR_EVTSERVER_LIB):\
	$(HADDIR_START_LIB):\
	$(HADDIR_SHOWER_LIB):\
	$(HADDIR_SHOWERUTIL_LIB):\
	$(HADDIR_SHOWERTOFINO_LIB):\
	$(HADDIR_TOF_LIB):\
	$(HADDIR_TOFINO_LIB):\
	$(HADDIR_TRIGGER_LIB):\
	$(HADDIR_TRIGGERUTIL_LIB):\
	$(HADDIR_PID_LIB):\
	$(HADDIR_PIDUTIL_LIB):\
	$(HADDIR_PHYANA_LIB):\
	$(HADDIR_HYP_LIB):\
	$(ADSM_BASE)/Linux"| sed -e 's/ //g'`

MAIN=pidLepExp

all:  $(MAIN).cc
	$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o
	$(LD) $(MAIN).o -Wl,-rpath,$(RPATH)\
	-L.  \
	-L$(HADDIR_PID_LIB) -lPid \
	-L$(HADDIR_RICH_LIB) -lRich \
	-L$(HADDIR_RICHUTIL_LIB) -lRichUtil \
	-L$(HADDIR_MDCTRACKD_LIB) -lMdcTrackD \
	-L$(HADDIR_MDC_LIB) -lMdc \
	-L$(HADDIR_MDCUTIL_LIB) -lMdcUtil \
	-L$(HADDIR_MDCTRACKS_LIB) -lMdcTrackS \
	-L$(HADDIR_MDCTRACKG_LIB) -lMdcTrackG \
	-L$(ROOTSYS)/lib $(LIBS)  \
	-L$(HADDIR_KICKPLANE_LIB) -lKick \
	-L$(HADDIR_SHOWER_LIB) -lShower \
	-L$(HADDIR_SHOWERUTIL_LIB) -lShowerUtil \
	-L$(HADDIR_SHOWERTOFINO_LIB) -lShowerTofino \
	-L$(HADDIR_START_LIB) -lStart \
	-L$(HADDIR_TOF_LIB) -lTof \
	-L$(HADDIR_TOFINO_LIB) -lTofino \
	-L$(HADDIR_QA_LIB) -lQA \
	-L$(HADDIR_TRIGGER_LIB) -lTrigger \
	-L$(HADDIR_TRIGGERUTIL_LIB) -lTriggerUtil \
	-L$(HADDIR_PIDUTIL_LIB) -lPidUtil \
	-L$(HADDIR_PHYANA_LIB) -lPhyAna \
	-L$(HADDIR_HYP_LIB) -lHyp \
	-L$(HADDIR_BASE_LIB) -lHydra \
	-o pidLepExp

pidLepExp_h: pidLepExp_h.cc
	$(CXX) pidLepExp_h.cc $(CXXFLAGS) -c -o pidLepExp_h.o
	$(LD) pidLepExp_h.o -Wl,-rpath,$(RPATH)\
	 -L.  \
	-L$(HADDIR_PID_LIB) -lPid \
	-L$(HADDIR_MDCTRACKD_LIB) -lMdcTrackD \
	-L$(HADDIR_MDC_LIB) -lMdc \
	-L$(HADDIR_MDCUTIL_LIB) -lMdcUtil \
	-L$(HADDIR_MDCTRACKS_LIB) -lMdcTrackS \
	-L$(ROOTSYS)/lib $(LIBS)  \
	-L$(HADDIR_KICKPLANE_LIB) -lKick \
	-L$(HADDIR_RICH_LIB) -lRich \
	-L$(HADDIR_RICHUTIL_LIB) -lRichUtil \
	-L$(HADDIR_SHOWER_LIB) -lShower \
	-L$(HADDIR_SHOWERUTIL_LIB) -lShowerUtil \
	-L$(HADDIR_SHOWERTOFINO_LIB) -lShowerTofino \
	-L$(HADDIR_START_LIB) -lStart \
	-L$(HADDIR_TOF_LIB) -lTof \
	-L$(HADDIR_TOFINO_LIB) -lTofino \
	-L$(HADDIR_QA_LIB) -lQA \
	-L$(HADDIR_BASE_LIB) -lHydra \
	-L$(HADDIR_PID_LIB) -lPid \
	-L$(HADDIR_PIDUTIL_LIB) -lPidUtil \
	-L$(HADDIR_PHYANA_LIB) -lPhyAna \
	-L$(HADDIR_HYP_LIB) -lHyp \
	-L$(HADDIR_TRIGGER_LIB) -lTrigger \
	-L$(HADDIR_TRIGGERUTIL_LIB) -lTriggerUtil \
	-o pidLepExp_h

clean: 
	@if [ -e *.o ]; then  echo removing \*.o files; rm *.o ; else echo 'nothing to do, no *.o files there' ; fi  

test: 
	@echo $(LIBS)
	@echo $(RPATH)

