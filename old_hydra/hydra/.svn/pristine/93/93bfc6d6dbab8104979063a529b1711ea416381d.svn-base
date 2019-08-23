TEMPLATE	= lib
LANGUAGE	= C++

CONFIG	+= qt warn_on debug thread

LIBS	+= $$MYLOCALLIBS $(SYSLIBS)

INCLUDEPATH	+= $(ROOTSYS)/include

FORMS	= TGo4HadesOnline.ui \
	TGo4HadesConfig.ui \
	TGo4MdcOnline.ui \
	TGo4StartOnline.ui \
	TGo4RichOnline.ui \
	TGo4TofOnline.ui \
	TGo4TofinoOnline.ui \
	TGo4ShowerOnline.ui \
	TGo4HadesOnlinePanel.ui \
	TGo4TriggerOnline.ui \
	TGo4PhysicsOnline.ui \
	TGo4HodoOnline.ui

IMAGES	= images/ObjBrowser.png

GO4INCLUDES=$(GO4SYS) \
            $(GO4SYS)/Go4GUI \
            $(GO4SYS)/Go4GUIRegistry \
            $(GO4SYS)/Go4GUIBase \
            $(GO4SYS)/qtroot/interface \
            $(GO4SYS)/Go4CommandsAnalysis \
            $(GO4SYS)/Go4LockGuard \
            $(HADDIR_HADESGO4)/Go4HadesClient \
            
MYHYDRALIBS    = $(HADDIR)/lib/libOra.so \
		$(HADDIR)/lib/libRich.so \ 
                $(HADDIR)/lib/libRichUtil.so \ 
                $(HADDIR)/lib/libRFIOtsm.so \
		$(HADDIR)/lib/libMdc.so \
		$(HADDIR)/lib/libMdcUtil.so \
		$(HADDIR)/lib/libPid.so \ 
                $(HADDIR)/lib/libPidUtil.so \
		$(HADDIR)/lib/libHadesGo4.so \
		$(HADDIR)/lib/libMdcGarfield.so \
		$(HADDIR)/lib/libTof.so \
		$(HADDIR)/lib/libKick.so\
		$(HADDIR)/lib/libMdcTrackD.so \
	       	$(HADDIR)/lib/libMdcTrackG.so \
	        $(HADDIR)/lib/libMdcTrackS.so \
		$(HADDIR)/lib/libShowerUtil.so \
		$(HADDIR)/lib/libShower.so \
		$(HADDIR)/lib/libShowerTofino.so \
		$(HADDIR)/lib/libTofino.so \
		$(HADDIR)/lib/libTof.so \
		$(HADDIR)/lib/libStart.so \
		$(HADDIR)/lib/libTrigger.so \ 
                $(HADDIR)/lib/libTriggerUtil.so \ 
                $(HADDIR)/lib/libHodo.so \
		$(HADDIR)/lib/libRevt.so \
		$(HADDIR)/lib/libHydra.so

MYHYDRAINC     = $(HADDIR)/include

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
	 	$(GO4SYS)/Go4Log \
	 	$(GO4SYS)/Go4ConditionAnalysis 

MYLOCALINC   =    $(LOCAL) \
		$(LOCAL)/Go4UserGUI \
		$(LOCAL)/Go4HadesClient


MYQTINC      =    $(QTDIR)/include
MYQLIBS      =    -L$(QTDIR)/lib -lqt

MYQTROOTINC  =	$(GO4SYS)/qtroot/interface





 
INCLUDEPATH	+= $$MYGO4INC $$MYLOCALINC $$MYHYDRAINC $$MYQTINC $$MYQTROOTINC $$GO4INCLUDES
LIBS	+= $$MYHYDRALIBLS




QMAKE_CXXFLAGS += $(HYDRA_INCLUDES)
 
