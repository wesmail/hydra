
MYHYDRALIBS    =	$(HADDIR_ORA_LIB)/libOra.so \
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

MYHYDRAINC     = 	$(HADDIR_BASE)/datasource \
		$(HADDIR_BASE)/datastruct \
		$(HADDIR_BASE)/eventhandling \
		$(HADDIR_BASE)/geometry  \
		$(HADDIR_BASE)/runtimedb \
		$(HADDIR_BASE)/util \
                $(HADDIR_BASE)/correlation \
                $(HADDIR_REVT) \
		$(HADDIR_EVTSERVER)\
                $(HADDIR_TOF) \
                $(HADDIR_KICKPLANE)\
                $(HADDIR_MDC) \
                $(HADDIR_HADESGO4) \
                $(HADDIR_MDCGARFIELD) \
                $(HADDIR_RICH) \
                $(HADDIR_SHOWER) \
                $(HADDIR_START) \
                $(HADDIR_TRIGGER) \
                $(HADDIR_TOFINO) \
                $(HADDIR_ORA) \
                $(HADDIR_SHOWERTOFINO)

MYGO4INC    =   $(GO4SYS) \
		$(GO4SYS)/Go4AnalysisClient \
		$(GO4SYS)/Go4CommandsAnalysis \
		$(GO4SYS)/Go4CommandsTasksHandler \
		$(GO4SYS)/Go4ConditionsBase \
	 	$(GO4SYS)/Go4DisplayBase \
	 	$(GO4SYS)/Go4DynamicList \
	 	$(GO4SYS)/Go4Event \
	 	$(GO4SYS)/Go4EventServer \
	 	$(GO4SYS)/Go4ExampleDisplay \
	 	$(GO4SYS)/Go4Exceptions \
	 	$(GO4SYS)/Go4GUIBase \
	 	$(GO4SYS)/Go4GUIRegistry  \
	 	$(GO4SYS)/Go4ProfileTimer \
	 	$(GO4SYS)/Go4Queue \
	 	$(GO4SYS)/Go4Socket \
	 	$(GO4SYS)/Go4StatusBase \
	 	$(GO4SYS)/Go4TasksHandler \
	 	$(GO4SYS)/Go4TestAnalysis \
	 	$(GO4SYS)/Go4TestGUI \
	 	$(GO4SYS)/Go4ThreadManager \
	 	$(GO4SYS)/Go4Trace \
	 	$(GO4SYS)/Go4ConditionAnalysis 

MYGO4LIBS    =    -L$(GO4SYS)/Go4Library \
		-L$(GO4SYS)/Go4Library -lqtroot \
		-L$(GO4SYS)/Go4Library -lGo4GUIRegistry \
		-L$(GO4SYS)/Go4Library -lGo4StatusBase \
		-L$(GO4SYS)/Go4Library -lGo4GUIBase \
		-L$(GO4SYS)/Go4Library -lGo4ThreadManager \
		-L$(GO4SYS)/Go4Library -lGo4LockGuard \
		-L$(GO4SYS)/Go4Library -lGo4CommandsBase \
		-L$(GO4SYS)/Go4Library -lGo4CommandsTaskHandler \
		-L$(GO4SYS)/Go4Library -lGo4Exceptions \
		-L$(GO4SYS)/Go4Library -lGo4Trace \
		-L$(GO4SYS)/Go4Library -lpthread \
		-L$(GO4SYS)/Go4Library -lThread \
		-L$(GO4SYS)/Go4Library -lGo4Version \
		-L$(GO4SYS)/Go4Library -lGo4QtBaseGUI

MYLOCALINC   =    $(LOCAL) \
		$(LOCAL)/Go4UserGUI \
		$(LOCAL)/Go4HadesClient

MYLOCALIBS   =    $(LOCAL)/Go4Library/libGo4HadesClient.so \
                $(LOCAL)/Go4Library/libGo4UserGUI.so

MYROOTLIBS   =    -L$(ROOTSYS)/lib -lCore \
		-L$(ROOTSYS)/lib -lCint \
		-L$(ROOTSYS)/lib -lHist \
		-L$(ROOTSYS)/lib -lGraf \
		-L$(ROOTSYS)/lib -lGraf3d \
		-L$(ROOTSYS)/lib -lGpad \
		-L$(ROOTSYS)/lib -lTree \
		-L$(ROOTSYS)/lib -lRint \
		-L$(ROOTSYS)/lib -lPostscript \
		-L$(ROOTSYS)/lib -lMatrix \
		-L$(ROOTSYS)/lib -lPhysics \
		-L$(ROOTSYS)/lib -lnsl \
		-L$(ROOTSYS)/lib -lm \
		-L$(ROOTSYS)/lib -ldl \
		-L$(ROOTSYS)/lib -rdynamic

MYQTINC      =    $(QTDIR)/include
MYQLIBS      =    -L$(QTDIR)/lib -lqt

MYQTROOTINC  =	$(GO4SYS)/qtroot/interface

FORMS	= TGo4HadesOnline.ui TGo4HadesConfig.ui TGo4MdcOnline.ui TGo4StartOnline.ui TGo4RichOnline.ui TGo4TofOnline.ui TGo4TofinoOnline.ui TGo4ShowerOnline.ui TGo4HadesOnlinePanel.ui TGo4TriggerOnline.ui
IMAGES	= images/ObjBrowser.png 
TEMPLATE	=lib
CONFIG	+= qt warn_on debug thread
INCLUDEPATH	+= $(ROOTSYS)/include $${MYGO4INC} $${MYLOCALINC} $${MYHYDRAINC} $${MYQTINC} $${MYQTROOTINC}
LIBS	+=  $${MYHYDRALIBLS} $${MYLOCALLIBS} $${MYROOTLIBS} $$(MYGO4LIBS) $(SYSLIBS) $${MYQTLIBS}
DBFILE	= Go4QtHadesOnline.db
LANGUAGE	= C++
