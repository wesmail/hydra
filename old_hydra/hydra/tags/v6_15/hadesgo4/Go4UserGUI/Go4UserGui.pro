DESTDIR =../Go4Library
# use the following line to add your own libraries

FORMS	= TGo4MainUserPanel.ui TGo4UserWidget1.ui 
IMAGES	= images/go4logo2.png 
TEMPLATE	=lib
CONFIG	+= qt warn_on debug thread
INCLUDEPATH	+= $(ROOTSYS)/include $(GO4SYS)/Go4StatusBase $(GO4SYS)/Go4GUIBase $(GO4SYS)/Go4GUIRegistry $(GO4SYS)/Go4Analysis $(GO4SYS)/Go4GUI $(GO4SYS) $(LOCAL)/Go4HadesOnline $(LOCAL) $(GO4SYS)/qtroot/interface
LIBS	+= $(LOCAL)/Go4Library/libGo4HadesOnline.so $(LOCAL)/Go4Library/libGo4QtHadesOnline.so
DBFILE	= Go4UserGui.db
LANGUAGE	= C++
