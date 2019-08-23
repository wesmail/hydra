DESTDIR =../Go4Library
# use the following line to add your own libraries
VERSION=2.10.0
FORMS	= TGo4MainUserPanel.ui TGo4UserWidget1.ui
IMAGES	= images/go4logo2.png 
TEMPLATE	=lib
CONFIG	+= qt warn_on debug thread
INCLUDEPATH	+= $(ROOTSYS)/include $(GO4SYS)/Go4StatusBase $(GO4SYS)/Go4GUIBase $(GO4SYS)/Go4GUIRegistry $(GO4SYS)/Go4Analysis $(GO4SYS)/Go4GUI $(GO4SYS) $(LOCAL)/Go4HadesOnline $(LOCAL) $(GO4SYS)/qtroot/interface
DBFILE	= Go4UserGui.db
LANGUAGE	= C++

LIBS    +=	$(HADDIR_ORA_LIB)/libOra.so \
		$(HADDIR_RICH_LIB)/libRich.so \
                $(HADDIR_RICH_LIB)/libRichUtil.so \
                $(HADDIR_RFIO_LIB)/libRFIOtsm.so \
		$(HADDIR_MDC_LIB)/libMdc.so \
		$(HADDIR_MDCUTIL_LIB)/libMdcUtil.so \
		$(HADDIR_PID_LIB)/libPid.so \
                $(HADDIR_PIDUTIL_LIB)/libPidUtil.so \
		$(HADDIR_HADESGO4_LIB)/libHadesGo4.so \
		$(HADDIR_MDCGARFIELD_LIB)/libMdcGarfield.so \
		$(HADDIR_TOF_LIB)/libTof.so \
		$(HADDIR_KICKPLANE_LIB)/libKick.so\
		$(HADDIR_MDCTRACKD_LIB)/libMdcTrackD.so \
	       	$(HADDIR_MDCTRACKG_LIB)/libMdcTrackG.so \
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
		$(HADDIR_PAIRS_LIB)/libPairs.so \
		$(HADDIR_WALL_LIB)/libWall.so \
		$(GSIDIR)/libhrevcli.so \
		$(HADDIR_BASE_LIB)/libHydra.so

LIBS	+= $(LOCAL)/Go4Library/libGo4QtHadesOnline.so $(LOCAL)/Go4Library/libGo4HadesOnline.so 
