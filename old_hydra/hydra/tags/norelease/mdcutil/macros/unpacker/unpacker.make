NOLIBNEW=ON

include $(HADDIR_BASEDIR)/Makefile.base

CXXFLAGS += -I$(ADSM_BASE)/Linux

LORALIB	:= -L$(ORACLE_HOME)/lib/ -lclntsh 


LIBS+=$(ROOTLIBS) -lProof -lTreePlayer -lMinuit -lThread
LIBS+=$(ROOTGLIBS) 
LIBS+=$(LORALIB)

RPATH:=`echo "$(HADDIR_MDC_LIB):\
	$(HADDIR_QA_LIB):\
	$(ROOTSYS)/lib:\
	$(HADDIR_BASE_LIB):\
	$(HADDIR_KICKPLANE_LIB):\
	$(HADDIR_MDCUTIL_LIB):\
	$(HADDIR_MDCTRACKS_LIB):\
	$(HADDIR_MDCTRACKD_LIB):\
	$(HADDIR_MDCTRACKG_LIB):\
	$(HADDIR_ORA_LIB):\
	$(HADDIR_RFIO_LIB):\
	$(HADDIR_RICH_LIB):\
	$(HADDIR_EVTSERVER_LIB):\
	$(HADDIR_START_LIB):\
	$(HADDIR_SHOWER_LIB):\
	$(HADDIR_SHOWERTOFINO_LIB):\
	$(HADDIR_TOF_LIB):\
	$(HADDIR_TOFINO_LIB):\
 	$(HADDIR_TOOLS_LIB):\
        $(ADSM_BASE)/Linux:\
	$(GSIDIR)"| sed -e 's/ //g'`

MYROOTLIBS :=-L$(ROOTSYS)/lib $(LIBS)

OTHERLIBS  := -L./

HYDRALIBS  := $(HADDIR_MDC_LIB)/libMdc.so \
	     $(HADDIR_MDCTRACKD_LIB)/libMdcTrackD.so \
	     $(HADDIR_MDCTRACKG_LIB)/libMdcTrackG.so \
	     $(HADDIR_MDCUTIL_LIB)/libMdcUtil.so \
	     $(HADDIR_MDCTRACKS_LIB)/libMdcTrackS.so \
	     $(HADDIR_KICKPLANE_LIB)/libKick.so \
	     $(HADDIR_RICH_LIB)/libRFIOtsm.so \
	     $(HADDIR_RICH_LIB)/libRich.so \
	     $(HADDIR_RICH_LIB)/libRich.so \
	     $(HADDIR_RICHUTIL_LIB)/libRichUtil.so \
	     $(HADDIR_SHOWER_LIB)/libShower.so \
	     $(HADDIR_EVTSERVER_LIB)/libRevt.so \
	     $(HADDIR_SHOWERTOFINO_LIB)/libShowerTofino.so \
	     $(HADDIR_START_LIB)/libStart.so \
	     $(HADDIR_TOF_LIB)/libTof.so \
	     $(HADDIR_WALL_LIB)/libWall.so \
	     $(HADDIR_TOFINO_LIB)/libTofino.so \
	     $(HADDIR_TOOLS_LIB)/libTools.so \
	     $(HADDIR_QA_LIB)/libQA.so \
	     $(HADDIR_ORA_LIB)/libOra.so \
	     $(HADDIR_BASE_LIB)/libHydra.so 

ADSMLIBS   := $(ADSM_BASE)/Linux/librawapicli.a \
	     $(ADSM_BASE)/Linux/librawserv.a 

REVLIBS	   := $(GSIDIR)/libhrevcli.so

LINKERLIBS := $(OTHERLIBS) $(MYROOTLIBS) $(HYDRALIBS) $(ADSMLIBS) $(REVLIBS)

MAIN:=unpacker
ALL := $(MAIN)                     $(MAIN)Stubborn           $(MAIN)CleanBadEvents \
      $(MAIN)NastyDebug $(MAIN)NastyDebugStubborn $(MAIN)NastyDebugCleanBadEvents \
      $(MAIN)Quiet           $(MAIN)QuietStubborn      $(MAIN)QuietCleanBadEvents

.PHONY: all verbose backup clean test
all: backup verbose $(MAIN) nasty quiet stubborn nasty nastystubborn quiet quietstubborn blackliststubborn blacklist

$(MAIN): $(MAIN).cc
	@echo '--------------------------------------------------------------------------------'
	@echo "compiling $@ "  
	@$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o
	@echo ... done
	@echo "linking $@ " 
	@$(LD) $(MAIN).o -Wl,-rpath,$(RPATH) $(LINKERLIBS) -o $(MAIN)
	@echo ... done

stubborn: $(MAIN).cc
	@echo '--------------------------------------------------------------------------------'
	@echo "compiling $@ "  
	@$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o -D STUBBORN=on
	@echo ... done
	@echo "linking $@ " 
	@$(LD) $(MAIN).o -Wl,-rpath,$(RPATH) $(LINKERLIBS) -o $(MAIN)Stubborn
	@echo ... done

cleanbad: $(MAIN).cc
	@echo '--------------------------------------------------------------------------------'
	@echo "compiling $@ "  
	@$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o -D CLEANBAD=on
	@echo ... done
	@echo "linking $@ " 
	@$(LD) $(MAIN).o -Wl,-rpath,$(RPATH) $(LINKERLIBS) -o $(MAIN)CleanBadEvents
	@echo ... done

nasty: $(MAIN).cc
	@echo '--------------------------------------------------------------------------------'
	@echo "compiling $@ "  
	@$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o -D NASTY=on
	@echo ... done
	@echo "linking $@ " 
	@$(LD) $(MAIN).o -Wl,-rpath,$(RPATH) $(LINKERLIBS) -o $(MAIN)NastyDebug
	@echo ... done

nastystubborn: $(MAIN).cc
	@echo '--------------------------------------------------------------------------------'
	@echo "compiling $@ "  
	@$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o -D STUBBORN=on -D NASTY=on
	@echo ... done
	@echo "linking $@ " 
	@$(LD) $(MAIN).o -Wl,-rpath,$(RPATH) $(LINKERLIBS) -o $(MAIN)NastyDebugStubborn
	@echo ... done

nastycleanbad: $(MAIN).cc
	@echo '--------------------------------------------------------------------------------'
	@echo "compiling $@ "  
	@$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o -D CLEANBAD=on -D NASTY=on
	@echo ... done
	@echo "linking $@ " 
	@$(LD) $(MAIN).o -Wl,-rpath,$(RPATH) $(LINKERLIBS) -o $(MAIN)NastyDebugCleanBadEvents
	@echo ... done

quiet: $(MAIN).cc
	@echo '--------------------------------------------------------------------------------'
	@echo "compiling $@ "  
	@$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o -D QUIET=on
	@echo ... done
	@echo "linking $@ " 
	@$(LD) $(MAIN).o -Wl,-rpath,$(RPATH) $(LINKERLIBS) -o $(MAIN)Quiet
	@echo ... done

quietstubborn: $(MAIN).cc
	@echo '--------------------------------------------------------------------------------'
	@echo "compiling $@ "  
	@$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o -D STUBBORN=on -D QUIET=on
	@echo ... done
	@echo "linking $@ " 
	@$(LD) $(MAIN).o -Wl,-rpath,$(RPATH) $(LINKERLIBS) -o $(MAIN)QuietStubborn
	@echo ... done

quietcleanbad: $(MAIN).cc
	@echo '--------------------------------------------------------------------------------'
	@echo "compiling $@ "  
	@$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o -D CLEANBAD=on -D QUIET=on
	@echo ... done
	@echo "linking $@ " 
	@$(LD) $(MAIN).o -Wl,-rpath,$(RPATH) $(LINKERLIBS) -o $(MAIN)QuietCleanBadEvents
	@echo ... done

blacklist: $(MAIN).cc
	@echo '--------------------------------------------------------------------------------'
	@echo "compiling $@ "  
	@$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o -D BLACKLIST=on
	@echo ... done
	@echo "linking $@ " 
	@$(LD) $(MAIN).o -Wl,-rpath,$(RPATH) $(LINKERLIBS) -o $(MAIN)BlackList
	@echo ... done

blackliststubborn: 
	@echo '--------------------------------------------------------------------------------'
	@echo "compiling $@ "  
	@$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o -D BLACKLIST=on -D STUBBORN=on
	@echo ... done
	@echo "linking $@ " 
	@$(LD) $(MAIN).o -Wl,-rpath,$(RPATH) $(LINKERLIBS) -o $(MAIN)BlackListStubborn
	@echo ... done

verbose: 
	@echo '--------------------------------------------------------------------------------'
	@echo compiling:; for item in `echo $(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o -D\"options\"`; do echo $${item}; done;
	@echo '--------------------------------------------------------------------------------'
	@MYRPATH=$(RPATH); echo linking  :; for item in `echo $(LD) $(MAIN).o -Wl,-rpath,$${MYRPATH} $(LINKERLIBS) -o $(MAIN)\"mode\"`; do echo $${item}; done;


all2:  $(MAIN).cc
	$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o
	$(LD) -O $(MAIN).o $(LINKERLIBS) -o $(MAIN)

clean: 
	@ls *.o >/dev/null; if [ $$? -eq 0 ]; then  echo removing \*.o files; rm *.o ; else echo 'nothing to do, no *.o files there' ; fi  

test: 
	@echo --------------------------------------------------------------------------------	
	@echo libs:---------------------------------------------------------------------------	
	@echo $(LIBS)
	@echo --------------------------------------------------------------------------------	
	@echo rpath:--------------------------------------------------------------------------	
	@echo $(RPATH)	
	@echo --------------------------------------------------------------------------------	
	@echo hydralibs:----------------------------------------------------------------------	
	@echo $(HYDRALIBS)X
	@echo --------------------------------------------------------------------------------	
	@echo linkerlibs:----------------------------------------------------------------------	
	@echo $(LINKERLIBS)
	@echo --------------------------------------------------------------------------------	
	@echo CXXFLAGS:------------------------------------------------------------------------	
	@echo $(CXXFLAGS)
	@echo --------------------------------------------------------------------------------	

backup:
	@if [ ! -d $(MAIN)_backup ] ; then mkdir $(MAIN)_backup; fi
	@for item in ${ALL}; do if [ -f $${item} ]; then cp $${item} $(MAIN)_backup/$${item}_`date +%d-%m-%y_%H\.%M\.%S`; fi; done     

