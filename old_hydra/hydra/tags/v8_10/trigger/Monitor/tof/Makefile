MAIN=tof-tofIPU_corr2
NOLIBNEW=ON

include $(HADDIR_BASEDIR)/Makefile.base

CXXFLAGS += -I$(ADSM_BASE)/Linux

#LORALIB	= -L$(ORACLE_HOME)/lib/ -lclntsh

#LORALIB	= -L$(ORACLE_HOME)/lib/ -lclntsh \
#	  -L$(ORACLE_HOME)/lib/ -lwtc9

#LIBS+=$(ROOTLIBS) -lProof -lTreePlayer -lMinuit 
LIBS+=$(ROOTLIBS) -lTreePlayer -lMinuit 
LIBS+=$(ROOTGLIBS) 
LIBS+=$(LORALIB)

RPATH=`echo "$(HADDIR_QA_LIB):\
	$(HADDIR_HODO_LIB):\
	$(HADDIR_PID_LIB):\
	$(HADDIR_MDCTRACKD_LIB):\
	$(HADDIR_RICH_LIB):\
	$(HADDIR_RICHUTIL_LIB):\
	$(HADDIR_RFIO_LIB):\
	$(HADDIR_MDC_LIB):\
	$(ROOTSYS)/lib:\
	$(HADDIR_BASE_LIB):\
	$(HADDIR_KICKPLANE_LIB):\
	$(HADDIR_MDCUTIL_LIB):\
	$(HADDIR_MDCTRACKS_LIB):\
	$(HADDIR_MDCTRACKG_LIB):\
	$(HADDIR_ORA_LIB):\
	$(HADDIR_EVTSERVER_LIB):\
	$(HADDIR_START_LIB):\
	$(HADDIR_SHOWER_LIB):\
	$(HADDIR_SHOWERUTIL_LIB):\
	$(HADDIR_SHOWERTOFINO_LIB):\
	$(HADDIR_TOF_LIB):\
	$(HADDIR_TOFINO_LIB):\
	$(HADDIR_TRIGGER_LIB):\
	$(HADDIR_TRIGGERUTIL_LIB):\
	$(HADDIR_PIDUTIL_LIB):\
	$(HADDIR_PAIRS_LIB):\
	$(ADSM_BASE)/Linux \
	$(GSIDIR)"| sed -e 's/ //g'`

all:  $(MAIN).cc
	$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o
	$(LD) $(MAIN).o -Wl,-rpath,$(RPATH)\
	-L$(HADDIR_QA_LIB) -lQA \
	-L$(HADDIR_HODO_LIB) -lHodo \
	-L$(HADDIR_PID_LIB) -lPid \
	-L$(HADDIR_MDCTRACKD_LIB) -lMdcTrackD \
	-L.  \
	-L$(HADDIR_RICH_LIB) -lRich \
	-L$(HADDIR_RICHUTIL_LIB) -lRichUtil \
        -L$(HADDIR_RFIO_LIB) -lRFIOtsm \
        -L$(HADDIR_MDC_LIB) -lMdc \
	-L$(HADDIR_MDCUTIL_LIB) -lMdcUtil \
	-L$(HADDIR_MDCTRACKS_LIB) -lMdcTrackS \
	-L$(HADDIR_MDCTRACKG_LIB) -lMdcTrackG \
	-L$(ROOTSYS)/lib $(LIBS)  \
	-L$(HADDIR_KICKPLANE_LIB) -lKick \
	-L$(HADDIR_SHOWER_LIB) -lShower \
	-L$(HADDIR_EVTSERVER_LIB) -lRevt \
	-L$(HADDIR_EVTSERVER_LIB) -lRevt \
	-L$(HADDIR_SHOWERTOFINO_LIB) -lShowerTofino \
	-L$(HADDIR_SHOWERUTIL_LIB) -lShowerUtil \
	-L$(HADDIR_START_LIB) -lStart \
	-L$(HADDIR_TOF_LIB) -lTof \
	-L$(HADDIR_TOFINO_LIB) -lTofino \
	-L$(HADDIR_ORA_LIB) -lOra \
	-L$(HADDIR_WALL_LIB) -lWall \
	-L$(HADDIR_TRIGGER_LIB) -lTrigger \
	-L$(HADDIR_TRIGGERUTIL_LIB) -lTriggerUtil \
	-L$(HADDIR_PIDUTIL_LIB) -lPidUtil \
	-L$(HADDIR_PAIRS_LIB) -lPairs \
	-L$(HADDIR_BASE_LIB) -lHydra \
	-L$(ADSM_BASE)/Linux -lrawapicli -lrawserv \
	-L$(GSIDIR)  -lhrevcli\
	-o $(MAIN)


clean: 
	@if [ -e *.o ]; then  echo removing \*.o and exec files; rm *.o; rm $(MAIN); else echo 'nothing to do, no *.o files there' ; fi  

test: 
	@echo $(LIBS)
	@echo $(RPATH)
