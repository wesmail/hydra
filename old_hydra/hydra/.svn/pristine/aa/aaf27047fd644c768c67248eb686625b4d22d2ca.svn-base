#LIBS	+= -L/u/gabriel/hydra_v608/lib -lTrigger -lShower

LIBS	+= -L$(ROOTSYS)/lib -lCore -lCint -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lnsl -lm -ldl -rdynamic -L$(GO4SYS)/Go4Library -lqtroot -L$(GO4SYS)/Go4Library -lGo4GUIRegistry -lGo4StatusBase -lGo4GUIBase -lGo4ThreadManager -lGo4LockGuard -lGo4CommandsBase -lGo4CommandsTaskHandler -lGo4Exceptions -lGo4Trace -lpthread -lThread -lGo4Version $(SYSLIBS) -lGo4QtBaseGUI -L$(LOCAL)/Go4HadesClient
FORMS	= TGo4HadesOnline.ui TGo4HadesConfig.ui TGo4MdcOnline.ui TGo4StartOnline.ui TGo4RichOnline.ui TGo4TofOnline.ui TGo4TofinoOnline.ui TGo4ShowerOnline.ui TGo4HadesOnlinePanel.ui 
IMAGES	= images/ObjBrowser.png 
TEMPLATE	=lib
CONFIG	+= qt warn_on debug thread
INCLUDEPATH	+= $(ROOTSYS)/include $(GO4SYS)/Go4StatusBase $(GO4SYS)/Go4GUIBase $(GO4SYS)/Go4GUIRegistry $(GO4SYS)/ $(LOCAL)/ $(LOCAL)/Go4UserGUI $(LOCAL)/Go4HadesClient $(HADDIR_HADESGO4)
LIBS	+= -L$(ROOTSYS)/lib -lCore -lCint -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lnsl -lm -ldl -rdynamic -L$(GO4SYS)/Go4Library -lqtroot -L$(GO4SYS)/Go4Library -lGo4GUIRegistry -lGo4StatusBase -lGo4GUIBase -lGo4ThreadManager -lGo4LockGuard -lGo4CommandsBase -lGo4CommandsTaskHandler -lGo4Exceptions -lGo4Trace -lpthread -lThread -lGo4Version $(SYSLIBS) -lGo4QtBaseGUI -L$(LOCAL)/Go4HadesClient
DBFILE	= Go4QtHadesOnline.db
LANGUAGE	= C++
