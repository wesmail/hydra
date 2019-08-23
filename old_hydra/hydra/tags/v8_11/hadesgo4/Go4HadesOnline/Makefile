include $(GO4SYS)/Makefile.config

MYINCLUDE     = -I $(HADDIR)/hadesgo4/Go4HadesClient -I $(HADDIR)/hadesgo4 -I $(HADDIR)/rich

CXXFLAGS     += $(ROOTCFLAGS) \
		-I$(GO4SYS) \
		-I$(QTDIR)/include \
		-I$(GO4SYS)/qtroot/interface \
		-I$(GO4SYS)/Go4GUIBase \
                -I$(GO4SYS)/Go4CommandsAnalysis \
                -I../Go4UserGUI \
		$(MYINCLUDE) 

THISLIB      = $(HADDIR)/hadesgo4/Go4Library/libGo4HadesOnline.so


HYDRALIBS    =  $(HADDIR)/lib/libHydra.so \
		$(HADDIR)/lib/libOra.so \
		$(HADDIR)/lib/libRich.so \
	        $(HADDIR)/lib/libRFIOtsm.so \
	        $(HADDIR)/lib/libMdc.so \
		$(HADDIR)/lib/libMdcUtil.so \
		$(HADDIR)/lib/libPid.so \
		$(HADDIR)/lib/libPidUtil.so \
		$(HADDIR)/lib/libHadesGo4.so \
		$(HADDIR)/lib/libMdcGarfield.so \
		$(HADDIR)/lib/libTof.so \
		$(HADDIR)/lib/libMdcTrackG.so \
		$(HADDIR)/lib/libMdcTrackD.so \
		$(HADDIR)/lib/libMdcTrackS.so \
		$(HADDIR)/lib/libShower.so \
		$(HADDIR)/lib/libShowerTofino.so \
		$(HADDIR)/lib/libTofino.so \
		$(HADDIR)/lib/libTof.so \
		$(HADDIR)/lib/libStart.so \
		$(HADDIR)/lib/libHodo.so \
		$(HADDIR)/lib/libTrigger.so \
		$(HADDIR)/lib/libTriggerUtil.so \
		$(HADDIR)/lib/libRevt.so \
		$(HADDIR)/lib/libKick.so\
		$(HADDIR)/lib/libHydra.so


GO4LIBS     += $(HYDRALIBS)  

##########################################
#Configuration area
#########################################
# Hydra include files
HYDRAINC     = 	-I$(HADDIR)/include/

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
		TGo4HodoOnlineSlots.$(HedSuf)\
		TGo4HodoOnlineStatus.$(HedSuf)\
		TGo4ShowerOnlineSlots.$(HedSuf) \
		TGo4ShowerOnlineStatus.$(HedSuf) \
		TGo4PhysicsOnlineSlots.$(HedSuf) \
		TGo4PhysicsOnlineStatus.$(HedSuf)
         
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

test:	
	@echo LIBLIB=\"$(LIBLIB)\"
	
clean:
	@rm -f $(HADDIR)/hadesgo4/Go4Library/*libGo4HadesOnline.so* 
	@rm -f $(HADDIR)/hadesgo4/Go4Library/*libGo4QtHadesOnline.so
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~ $(LIBLIBM) $(LIBLIBMM)
	@make -f Makefile.qt clean;


$(LIBLIB): $(LIBO) $(LIBDO)
	@echo "Generating ONLINELIB part of the GUI..."
	$(LD) $(SOFLAGS)$(LIBLIBM) $(LDFLAGS) $(LIBO) $(LIBDO) $(OutPutOpt) $(LIBLIBMM) $(HYDRALIBS) 
	$(LN) -sf $(LIBLIBMM) $(LIBLIBM)
	$(LN) -sf $(LIBLIBM) $(LIBLIB)
	@mv *libGo4HadesOnline.so*  $(HADDIR)/hadesgo4/Go4Library 
	@mv *libGo4QtHadesOnline.so* $(HADDIR)/hadesgo4/Go4Library
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	@$(ROOTSYS)/bin/rootcint -f $(LIBDS) -c -p -I$(GO4SYS) $(HYDRAINC) $(LIBH) $(LIBL)

#########################################
%.$(ObjSuf):	%.$(SrcSuf)
	$(CXX) $(CXXFLAGS) $(HYDRAINC) -c $<

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(HedSuf)
