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

LIBS    +=	$(HADDIR)/lib/libOra.so \
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
		$(HADDIR)/lib/libShower.so \
		$(HADDIR)/lib/libShowerUtil.so \
		$(HADDIR)/lib/libShowerTofino.so \
		$(HADDIR)/lib/libTofino.so \
		$(HADDIR)/lib/libTof.so \
		$(HADDIR)/lib/libStart.so \
		$(HADDIR)/lib/libHodo.so \
		$(HADDIR)/lib/libTrigger.so \
                $(HADDIR)/lib/libTriggerUtil.so \
		$(HADDIR)/lib/libRevt.so \
		$(HADDIR)/lib/libPairs.so \
		$(HADDIR)/lib/libWall.so \
		$(HADDIR)/lib/libHydra.so

LIBS	+= $(LOCAL)/Go4Library/libGo4QtHadesOnline.so $(LOCAL)/Go4Library/libGo4HadesOnline.so 
