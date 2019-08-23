include $(GO4SYS)/Makefile.config


#MAJOR         = 2
#MINOR         = 1.0
#VERSSUF         = $(MAJOR).$(MINOR)



THISLIB      = $(HADDIR)/hadesgo4/Go4Library/libGo4HadesClient.so

HYDRALIBS    =	$(HADDIR)/lib/libOra.so \
		$(HADDIR)/lib/libRich.so \
		$(HADDIR)/lib/libRFIOtsm.so \
		$(HADDIR)/lib/libRichUtil.so \
	 	$(HADDIR)/lib/libMdc.so \
		$(HADDIR)/lib/libMdcUtil.so \
		$(HADDIR)/lib/libPid.so \
		$(HADDIR)/lib/libPidUtil.so \
		$(HADDIR)/lib/libPairs.so \
		$(HADDIR)/lib/libHadesGo4.so \
		$(HADDIR)/lib/libMdcGarfield.so \
		$(HADDIR)/lib/libTof.so \
		$(HADDIR)/lib/libWall.so \
		$(HADDIR)/lib/libKick.so \
		$(HADDIR)/lib/libMdcTrackG.so \
	       	$(HADDIR)/lib/libMdcTrackD.so \
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
		$(HADDIR)/lib/libHydra.so

GO4LIBS     += $(REVLIBS)
GO4LIBS     += $(HYDRALIBS)  
#GO4LIBS     += -L$(ROOTSYS)/lib -lRFIO  

##########################################
#Configuration area
#########################################
# Hydra include files
HYDRAINC     = 	-I$(HADDIR)/include

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
	@rm -f $(HADDIR)/hadesgo4/Go4Library/*libGo4HadesClient.so* 
	@rm -f $(HADDIR)/hadesgo4/Go4Library/*libGo4QtHadesClient.so
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~ $(LIBLIBM) $(LIBLIBMM)
	@rm -f $(TESTE) $(TESTO)

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIBM) $(LDFLAGS) $(LIBO) $(LIBDO) $(OutPutOpt) $(LIBLIBMM) $(HYDRALIBS)
	$(LN) $(LIBLIBMM) $(LIBLIBM)
	$(LN) $(LIBLIBM) $(LIBLIB)
	$(LN) $(HADDIR)/lib/libHydra.so libHydra.so
	$(LN) $(HADDIR)/lib/libHadesGo4.so libHadesGo4.so

	@mv $(HADDIR)/hadesgo4/Go4HadesClient/*libGo4HadesClient.so*   $(HADDIR)/hadesgo4/Go4Library  
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	@$(ROOTSYS)/bin/rootcint -f $(LIBDS) -c -p -I$(GO4SYS) $(HYDRAINC) $(LIBH) $(LIBL)

#########################################
%.$(ObjSuf):	%.$(SrcSuf)
	$(CXX) $(CXXFLAGS) $(HYDRAINC) -c $<


.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(HedSuf)
