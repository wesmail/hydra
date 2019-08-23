include $(GO4SYS)/Makefile.config


#MAJOR         = 2
#MINOR         = 1.0
#VERSSUF         = $(MAJOR).$(MINOR)



THISLIB      = $(HADDIR_HADESGO4)/Go4Library/libGo4HadesClient.so

HYDRALIBS    =	$(HADDIR_ORA_LIB)/libOra.so \
		$(HADDIR_RICH_LIB)/libRich.so \
		$(HADDIR_RFIO_LIB)/libRFIOtsm.so \
		$(HADDIR_RICHUTIL_LIB)/libRichUtil.so \
	 	$(HADDIR_MDC_LIB)/libMdc.so \
		$(HADDIR_MDCUTIL_LIB)/libMdcUtil.so \
		$(HADDIR_PID_LIB)/libPid.so \
		$(HADDIR_PIDUTIL_LIB)/libPidUtil.so \
		$(HADDIR_PAIRS_LIB)/libPairs.so \
		$(HADDIR_HADESGO4_LIB)/libHadesGo4.so \
		$(HADDIR_MDCGARFIELD_LIB)/libMdcGarfield.so \
		$(HADDIR_TOF_LIB)/libTof.so \
		$(HADDIR_WALL_LIB)/libWall.so \
		$(HADDIR_KICKPLANE_LIB)/libKick.so \
		$(HADDIR_MDCTRACKG_LIB)/libMdcTrackG.so \
	       	$(HADDIR_MDCTRACKD_LIB)/libMdcTrackD.so \
		$(HADDIR_MDCTRACKS_LIB)/libMdcTrackS.so \
		$(HADDIR_SHOWER_LIB)/libShower.so \
		$(HADDIR_SHOWERUTIL_LIB)/libShowerUtil.so \
		$(HADDIR_SHOWERTOFINO_LIB)/libShowerTofino.so \
		$(HADDIR_TOFINO_LIB)/libTofino.so \
		$(HADDIR_TOF_LIB)/libTof.so \
		$(HADDIR_START_LIB)/libStart.so \
                $(HADDIR_HODO_LIB)/libHodo.so \
		$(HADDIR_TRIGGER_LIB)/libTrigger.so \
		$(HADDIR_TRIGGERUTIL_LIB)/libTriggerUtil.so \
		$(HADDIR_EVTSERVER_LIB)/libRevt.so \
		$(GSIDIR)/libhrevcli.so \
		$(HADDIR_BASE_LIB)/libHydra.so

GO4LIBS     += $(REVLIBS)
GO4LIBS     += $(HYDRALIBS)  
#GO4LIBS     += -L$(ROOTSYS)/lib -lRFIO  

##########################################
#Configuration area
#########################################
# Hydra include files
HYDRAINC     = 	-I$(HADDIR_BASE)/datasource \
		-I$(HADDIR_BASE)/datastruct \
		-I$(HADDIR_BASE)/eventhandling \
		-I$(HADDIR_BASE)/geometry  \
		-I$(HADDIR_BASE)/runtimedb \
		-I$(HADDIR_BASE)/util \
                -I$(HADDIR_BASE)/correlation \
		-I$(HADDIR_EVTSERVER)\
                -I$(HADDIR_TOF) \
		-I$(HADDIR_WALL) \
                -I$(HADDIR_KICKPLANE)\
                -I$(HADDIR_MDC) \
		-I$(HADDIR_MDCTRACKG) \
                -I$(HADDIR_MDCTRACKD) \
                -I$(HADDIR_HADESGO4) \
                -I$(HADDIR_MDCGARFIELD) \
                -I$(HADDIR_RICH) \
		-I$(HADDIR_RICHUTIL) \
                -I$(HADDIR_SHOWER) \
                -I$(HADDIR_START) \
                -I$(HADDIR_HODO) \
		-I$(HADDIR_TRIGGER) \
		-I$(HADDIR_TRIGGERUTIL) \
                -I$(HADDIR_TOFINO) \
                -I$(HADDIR_ORA) \
                -I$(HADDIR_SHOWERTOFINO)

CXXFLAGS      += $(HYDRAINC)

LIBNAME =  Go4HadesClient
LIBH    =  TGo4Hades.$(HedSuf) \
	TGo4HadesConfigParameter.$(HedSuf) \
	TGo4MdcActiveParameter.$(HedSuf) \
	TGo4PhysicsActiveParameter.$(HedSuf) \
	TGo4ResetParameter.$(HedSuf) \
	TGo4TriggerParameter.$(HedSuf) \

LIBL     = $(LIBNAME)LinkDef.$(HedSuf)
LIBS     = $(LIBH:.$(HedSuf)=.$(SrcSuf))
LIBO     = $(LIBS:.$(SrcSuf)=.$(ObjSuf))
LIBDS    = $(LIBNAME)Dict.$(SrcSuf)
LIBDH    = $(LIBDS:.$(SrcSuf)=.$(HedSuf))
LIBDO    = $(LIBDS:.$(SrcSuf)=.$(ObjSuf))
LIBLIB   = lib$(LIBNAME).$(DllSuf)
LIBLIBM  = lib$(LIBNAME).$(DllSuf).$(MAJOR)
LIBLIBMM = lib$(LIBNAME).$(DllSuf).$(MAJOR).$(MINOR)
LIBDEP   = $(LIBS:.$(SrcSuf)=.$(DepSuf))
##########################################

TESTO      = MainGo4HadesClient.$(ObjSuf)
TESTS      = MainGo4HadesClient.$(SrcSuf)
TESTE      = MainGo4HadesClient$(ExeSuf)

$(TESTE):     $(TESTO)
	$(LD) $(LDFLAGS) $(TESTO) $(GLIBS) $(ROOTLIBS) $(ANALIBS) $(GO4LIBS) $(THREADLIBS) $(THISLIB) $(OutPutOpt) $(TESTE)
	@echo "$(TESTE) done"      

PROGRAMS = $(LIBLIB)

lib: libroot 

libroot:	$(PROGRAMS) 

install:

#all:  lib $(TESTE) 
all: clean lib $(TESTE) 

depend:
	touch Make-depend
	$(MAKEDEP) -fMake-depend -Y -- $(CXXFLAGS) -- $(GO4FITGUIS) $(TESTS) > /dev/null 2>&1

###--------------------------------------------------------------------------

include Make-depend


test:	
	@echo LIBLIB=\"$(LIBLIB)\"

clean:
	@rm -f $(HADDIR_HADESGO4)/Go4Library/*libGo4HadesClient.so* 
	@rm -f $(HADDIR_HADESGO4)/Go4Library/*libGo4QtHadesClient.so
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~ $(LIBLIBM) $(LIBLIBMM)
	@rm -f $(TESTE) $(TESTO)

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIBM) $(LDFLAGS) $(LIBO) $(LIBDO) $(OutPutOpt) $(LIBLIBMM) $(HYDRALIBS)
	$(LN) $(LIBLIBMM) $(LIBLIBM)
	$(LN) $(LIBLIBM) $(LIBLIB)
	$(LN) $(HADDIR_BASE_LIB)/libHydra.so libHydra.so
	$(LN) $(HADDIR_HADESGO4_LIB)/libHadesGo4.so libHadesGo4.so

	@mv $(HADDIR_HADESGO4)/Go4HadesClient/*libGo4HadesClient.so*   $(HADDIR_HADESGO4)/Go4Library  
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	@$(ROOTSYS)/bin/rootcint -f $(LIBDS) -c -p -I$(GO4SYS) $(HYDRAINC) $(LIBH) $(LIBL)

#########################################
%.$(ObjSuf):	%.$(SrcSuf)
	$(CXX) $(CXXFLAGS) $(HYDRAINC) -c $<


.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(HedSuf)
