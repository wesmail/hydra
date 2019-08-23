CXX           =
ObjSuf        = o
SrcSuf        = cxx
HedSuf        = h 
DepSuf        = d
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o 

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs) -L$(ROOTSYS)/lib -lMinuit
ROOTGLIBS    := $(shell root-config --glibs)

MAJOR         = 0
MINOR         = 1.1
LN            = ln -sf

MAKEDEP = rmkdepend

# Linux with egcs
CXX           = g++
CC            = egcs

CXXFLAGS     = -g -Wall -DR__THREAD -fPIC -D_REENTRANT 
CXXFLAGS	 += -I$(QTDIR)/include 
CXXFLAGS     += $(ROOTCFLAGS) \
		-I$(GO4SYS) \
		-I$(GO4SYS)/Go4AnalysisClient \
		-I$(GO4SYS)/Go4CommandsAnalysis \
		-I$(GO4SYS)/Go4CommandsTasksHandler \
		-I$(GO4SYS)/Go4ConditionsBase \
	 	-I$(GO4SYS)/Go4DisplayBase \
	 	-I$(GO4SYS)/Go4DynamicList \
	 	-I$(GO4SYS)/Go4Event \
	 	-I$(GO4SYS)/Go4EventServer \
	 	-I$(GO4SYS)/Go4ExampleDisplay \
	 	-I$(GO4SYS)/Go4Exceptions \
	 	-I$(GO4SYS)/Go4GUIBase \
	 	-I$(GO4SYS)/Go4GUIRegistry  \
	 	-I$(GO4SYS)/Go4ProfileTimer \
	 	-I$(GO4SYS)/Go4Queue \
	 	-I$(GO4SYS)/Go4Socket \
	 	-I$(GO4SYS)/Go4StatusBase \
	 	-I$(GO4SYS)/Go4TasksHandler \
	 	-I$(GO4SYS)/Go4TestAnalysis \
	 	-I$(GO4SYS)/Go4TestGUI \
	 	-I$(GO4SYS)/Go4ThreadManager \
	 	-I$(GO4SYS)/Go4Trace \
	 	-I$(GO4SYS)/Go4ConditionAnalysis -I.

LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared -Wl,-soname,

GLIBS         = $(ROOTGLIBS) $(SYSLIBS) -lRFIO

THREADLIBS    =   -lpthread -lThread
#THREADLIBS    =  -lRFIO -lpthread -lThread

GLIBS       += $(THREADLIBS) 

THISLIB      = $(HADDIR_HADESGO4)/Go4Library/libGo4HadesClient.so

OTHERLIBS    = 	-L$(GO4SYS)/Go4Library -lGo4Exceptions -lGo4Trace  
MANLIBS      =  -L$(GO4SYS)/Go4Library -lGo4LockGuard -lGo4ThreadManager
COMLIBS      = 	-L$(GO4SYS)/Go4Library -lGo4CommandsBase 
STATLIBS     = 	-L$(GO4SYS)/Go4Library -lGo4StatusBase
CONDLIBS     = 	-L$(GO4SYS)/Go4Library -lGo4ConditionsBase
QUEUELIBS    = 	-L$(GO4SYS)/Go4Library -lGo4Queue
TRANSLIBS    = 	-L$(GO4SYS)/Go4Library -lGo4Socket
TASKLIBS     =  -L$(GO4SYS)/Go4Library -lGo4TaskHandler -lGo4CommandsTaskHandler 
EXAMPLIBS    =  -L$(GO4SYS)/Go4Library -lGo4AnalysisClient 
GUILIBS      =  -L$(GO4SYS)/Go4Library -lGo4GUIBase
EVENTLIBS    =  -L$(GO4SYS)/Go4Event -lGo4Event -L$(GO4SYS)/Go4EventServer -lGo4EventServer
ANALIBS      =  -L$(GO4SYS)/Go4Library -lGo4Analysis -lGo4CommandsAnalysis -lGo4DynamicList 
HYDRALIBS    =  $(HADDIR_ORA_LIB)/libOra.so \
		$(HADDIR_RICH_LIB)/libRich.so \
	 	$(HADDIR_MDC_LIB)/libMdc.so \
		$(HADDIR_MDCUTIL_LIB)/libMdcUtil.so \
		$(HADDIR_HADESGO4_LIB)/libHadesGo4.so \
		$(HADDIR_MDCGARFIELD_LIB)/libMdcGarfield.so \
		$(HADDIR_TOF_LIB)/libTof.so \
		$(HADDIR_MDCTRACKD_LIB)/libMdcTrackD.so \
		$(HADDIR_MDCTRACKS_LIB)/libMdcTrackS.so \
		$(HADDIR_SHOWER_LIB)/libShower.so \
		$(HADDIR_SHOWERTOFINO_LIB)/libShowerTofino.so \
		$(HADDIR_TOFINO_LIB)/libTofino.so \
		$(HADDIR_TOF_LIB)/libTof.so \
		$(HADDIR_START_LIB)/libStart.so \
		$(HADDIR_EVTSERVER_LIB)/libRevt.so \
		$(GSIDIR)/libhrevcli.so \
		$(HADDIR_KICKPLANE_LIB)/libKick.so \
		$(HADDIR_BASE_LIB)/libHydra.so

VERSIONLIBS    =    -L$(GO4SYS)/Go4Library -lGo4Version

OTHERLIBS     += $(MANLIBS) \
		 $(TRANSLIBS) \
		 $(QUEUELIBS) \
		 $(COMLIBS) \
		 $(CONDLIBS) \
		 $(STATLIBS) \
		 $(TASKLIBS) \
		 $(EXAMPLIBS) \
		 $(EVENTLIBS) \
		 $(ANALIBS) \
		 $(GUILIBS)
OTHERLIBS     += $(VERSIONLIBS) 
OTHERLIBS     += $(REVLIBS)
OTHERLIBS     += $(HYDRALIBS)  

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
                -I$(HADDIR_REVT) \
		-I$(HADDIR_EVTSERVER)\
                -I$(HADDIR_TOF) \
                -I$(HADDIR_KICKPLANE)\
                -I$(HADDIR_MDC) \
                -I$(HADDIR_HADESGO4) \
                -I$(HADDIR_MDCGARFIELD) \
                -I$(HADDIR_RICH) \
                -I$(HADDIR_SHOWER) \
                -I$(HADDIR_START) \
                -I$(HADDIR_TOFINO) \
                -I$(HADDIR_ORA) \
                -I$(HADDIR_SHOWERTOFINO)

CXXFLAGS      += $(HYDRAINC)

LIBNAME =       Go4HadesClient
LIBH    = TGo4Hades.$(HedSuf)\

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
	$(LD) $(LDFLAGS) $(TESTO) $(GLIBS) $(ROOTLIBS) $(ANALIBS) $(OTHERLIBS) $(THISLIB) $(OutPutOpt) $(TESTE)
	@echo "$(TESTE) done"      


PROGRAMS = $(LIBLIB)

lib: libroot 

libroot:	$(PROGRAMS) 

install:

all:  clean lib $(TESTE) 

depend:
	touch Make-depend
	$(MAKEDEP) -fMake-depend -Y -- $(CXXFLAGS) -- $(GO4FITGUIS) $(TESTS) > /dev/null 2>&1

###--------------------------------------------------------------------------

include Make-depend

#.$(SrcSuf).$(ObjSuf):
#	$(CXX) $(CXXFLAGS) -c $<

test:	
	@echo LIBLIB=\"$(LIBLIB)\"

clean:
	@rm -f $(HADDIR_HADESGO4)/Go4Library/*libGo4HadesClient.so* 
	@rm -f $(HADDIR_HADESGO4)/Go4Library/*libGo4QtHadesClient.so
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~ $(LIBLIBM) $(LIBLIBMM)
	@rm -f $(TESTE) $(TESTO)

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIBM) $(LDFLAGS) $(LIBO) $(LIBDO) $(OutPutOpt) $(LIBLIBMM)
	$(LN) $(LIBLIBMM) $(LIBLIBM)
	$(LN) $(LIBLIBM) $(LIBLIB)
	$(LN) $(HADDIR_BASE_LIB)/libHydra.so libHydra.so
	$(LN) $(HADDIR_HADESGO4_LIB)/libHadesGo4.so libHadesGo4.so

	@mv $(HADDIR_HADESGO4)/Go4HadesClient/*libGo4HadesClient.so*   $(HADDIR_HADESGO4)/Go4Library  
	#@mv $(HADDIR_HADESGO4)/Go4HadesClient/*libGo4QtHadesClient.so* $(HADDIR_HADESGO4)/Go4Library
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	@$(ROOTSYS)/bin/rootcint -f $(LIBDS) -c -p -I$(GO4SYS) $(HYDRAINC) $(LIBH) $(LIBL)

#include $(LIBDEP)
#########################################
%.$(ObjSuf):	%.$(SrcSuf)
	$(CXX) $(CXXFLAGS) $(HYDRAINC) -c $<

#%.$(DepSuf):	%.$(SrcSuf)
#	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $(HYDRAINC) $< \
#		| sed '\''s/\($*\)\.$(ObjSuf)[ :]*/\1.$(ObjSuf) $@ : /g'\'' > $@; \
#		[ -s $@ ] || rm -f $@'

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(HedSuf)
#.$(DepSuf)
