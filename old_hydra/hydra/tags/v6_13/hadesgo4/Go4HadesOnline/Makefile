CXX           =
ObjSuf        = o
SrcSuf        = cxx
HedSuf        = h 
DepSuf        = d
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o 

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

#MAJOR         = 0
#MINOR         = 1.1
MAJOR         = 2
MINOR         = 1.0
LN            = ln -sf

MYINCLUDE     = -I $(HADDIR_HADESGO4)/Go4HadesClient -I $(HADDIR_HADESGO4) -I $(HADDIR_RICH)
MAKEDEP = rmkdepend

# Linux with egcs
#CXX         = insure
#CC			=insure
#CXX           = codewizard -Zvm
CXX           = g++
CC            = egcs

CXXFLAGS     = -g -O -Wall -DR__THREAD -fPIC -D_REENTRANT 
#-Wuninitialized 
#-Werror

CXXFLAGS     += $(ROOTCFLAGS) \
		-I$(GO4SYS) \
		-I$(QTDIR)/include \
		-I$(GO4SYS)/qtroot/interface \
		-I$(GO4SYS)/Go4GUIBase \
		$(MYINCLUDE) 

#LD            = codewizard -Zvm
#LD            = insure
LD            = g++
#LDFLAGS       = -O
SOFLAGS       = -shared -Wl,-soname,

LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

THREADLIBS    =  -lpthread -lThread

LIBS        += $(THREADLIBS) 
GLIBS       += $(THREADLIBS) 
THISLIB      = -L./ -lGo4HadesOnline


OTHERLIBS    = -L$(GO4SYS)/Go4Library -lGo4Exceptions 
MANLIBS      = -L$(GO4SYS)/Go4Library -lGo4LockGuard 
COMLIBS      = -L$(GO4SYS)/Go4Library -lGo4CommandsBase -lGo4CommandsTaskHandler 
STATLIBS     = -L$(GO4SYS)/Go4Library -lGo4StatusBase -lGo4ThreadLists
TASKLIBS     = -L$(GO4SYS)/Go4Library -lGo4TaskHandler
GUILIBS      = -L$(GO4SYS)/Go4Library -lGo4GUIBase  
GUIREGISTRY  = -L$(GO4SYS)/Go4Library -lGo4GUIRegistry  -lGo4ConditionsBase
DISLIBS	     = -L$(GO4SYS)/Go4Library -lGo4Display -lGo4Analysis -lGo4DynamicList
VERSIONLIBS  = -L$(GO4SYS)/Go4Library -lGo4Version
QTROOTLIB    = -L$(GO4SYS)/Go4Library  -lqtroot
QTLIBS       = -L$(QTDIR)/lib -lqt-mt
HYDRALIBS    =	$(HADDIR_ORA_LIB)/libOra.so \
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
		$(HADDIR_TRIGGER_LIB)/libTrigger.so \
		$(HADDIR_EVTSERVER_LIB)/libRevt.so \
		$(GSIDIR)/libhrevcli.so \
		$(HADDIR_KICKPLANE_LIB)/libKick.so\
		$(HADDIR_BASE_LIB)/libHydra.so


OTHERLIBS     += $(MANLIBS)\
		 $(TRANSLIBS) \
		 $(QUEUELIBS) \
		 $(COMLIBS) \
		 $(STATLIBS) \
		 $(TASKLIBS) \
		 $(EXAMPLIBS) \
		 $(GUILIBS) \
		 $(DISLIBS) \
		 $(QTROOTLIB)
OTHERLIBS     += $(VERSIONLIBS)
OTHERLIBS     += $(HYDRALIBS)  

GO4INC    =   	-I$(GO4SYS)/ \
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
                -I$(HADDIR_TRIGGER) \
                -I$(HADDIR_TOFINO) \
                -I$(HADDIR_ORA) \
                -I$(HADDIR_SHOWERTOFINO)

CXXFLAGS      += $(HYDRAINC)

LIBNAME =       Go4HadesOnline
LIBH    = 	TGo4HadesOnlineStatus.$(HedSuf) \
		TGo4HadesOnlineSlots.$(HedSuf) \
		TGo4HadesOnlinePanelSlots.$(HedSuf) \
		TGo4HadesOnlinePanelStatus.$(HedSuf)\
		TGo4HadesConfigSlots.$(HedSuf) \
		TGo4HadesConfigStatus.$(HedSuf) \
		TGo4StartOnlineSlots.$(HedSuf) \
		TGo4StartOnlineStatus.$(HedSuf)\
		TGo4TriggerOnlineSlots.$(HedSuf) \
		TGo4TriggerOnlineStatus.$(HedSuf)\
		TGo4RichOnlineSlots.$(HedSuf) \
		TGo4RichOnlineStatus.$(HedSuf)\
		TGo4MdcOnlineSlots.$(HedSuf) \
		TGo4MdcOnlineStatus.$(HedSuf)\
		TGo4TofOnlineSlots.$(HedSuf) \
		TGo4TofOnlineStatus.$(HedSuf)\
		TGo4TofinoOnlineSlots.$(HedSuf) \
		TGo4TofinoOnlineStatus.$(HedSuf)\
		TGo4ShowerOnlineSlots.$(HedSuf) \
		TGo4ShowerOnlineStatus.$(HedSuf)
         
LIBL     = T$(LIBNAME)LinkDef.$(HedSuf)
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

PROGRAMS = $(LIBLIB)

lib: libroot install 

libroot:	$(PROGRAMS) 

qtgui:        
	@echo "Generating Qt part of the GUI..."
	qmake Go4QtHadesOnline.pro -o Makefile.qt ;
	make -f Makefile.qt ;
	@echo "QT finished"

install:
#		@cp -fd *.so* $(GO4SYS)/Go4Library/.

example: qtgui

all:  qtgui lib install qtgui

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
	@rm -f $(HADDIR_HADESGO4)/Go4Library/*libGo4HadesOnline.so* 
	@rm -f $(HADDIR_HADESGO4)/Go4Library/*libGo4QtHadesOnline.so
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~ $(LIBLIBM) $(LIBLIBMM)
	@make -f Makefile.qt clean;


$(LIBLIB): $(LIBO) $(LIBDO)
	@echo "Generating ONLINELIB part of the GUI..."
	$(LD) $(SOFLAGS)$(LIBLIBM) $(LDFLAGS) $(LIBO) $(LIBDO) $(OutPutOpt) $(LIBLIBMM) $(HYDRALIBS) 
	$(LN) -sf $(LIBLIBMM) $(LIBLIBM)
	$(LN) -sf $(LIBLIBM) $(LIBLIB)
	@mv *libGo4HadesOnline.so*  $(HADDIR_HADESGO4)/Go4Library 
	@mv *libGo4QtHadesOnline.so* $(HADDIR_HADESGO4)/Go4Library
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
