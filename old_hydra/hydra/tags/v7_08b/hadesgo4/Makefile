include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
INCMDC  = -I$(HADDIR_START) \
	-I$(HADDIR_TRIGGER) \
	-I$(HADDIR_RICH) \
	-I$(HADDIR_MDC) \
	-I$(HADDIR_TOF) \
	-I$(HADDIR_TOFINO)\
	-I$(HADDIR_SHOWER) \
	-I$(HADDIR_SHOWERTOFINO) \
	-I$(HADDIR_KICKPLANE) \
	-I$(HADDIR_MDCTRACKG)

LIBNAME = HadesGo4
LIBH    =hgo4onlineconfig.h hhadespar.h hprocessstat.h \
	htriggergo4mon.h htriggerhistbooker.h htriggerhistfiller.h \
	hmdcgo4mon.h hmdccalibrationgo4mon.h hmdchistbooker.h hmdchistfiller.h hmdcconnectedtdc.h \
	hstartgo4mon.h hstarthistbooker.h hstarthistfiller.h \
	hrichgo4mon.h hrichhistbooker.h hrichhistfiller.h hrichfancy.h \
	hstartcalibrationgo4mon.h hrichpattern.h \
	htofgo4mon.h htofhistbooker.h htofhistfiller.h \
	htofinogo4mon.h htofinohistbooker.h htofinohistfiller.h \
	hshowergo4mon.h hshowerhistbooker.h hshowerhistfiller.h hshowerconfig.h \
	hphysicsgo4mon.h hphysicshistbooker.h hphysicshistfiller.h \
	hmonhist.h htrendhist.h hhistarray.h hmdcconfig.h \
	hgo4parrootfileio.h hgo4parasciifileio.h hgo4contfact.h \
  htriggerhistfillerutil.h

LIBL    = $(LIBNAME)LinkDef.h
LIBS    = $(LIBH:.h=.cc)
LIBO    = $(LIBS:.cc=.o)
LIBDS   = $(LIBNAME)Cint.cc
LIBDH   = $(LIBDS:.cc=.h)
LIBDO   = $(LIBDS:.cc=.o)
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)
LIBDEP  = $(LIBS:.cc=.d)
##########################################

shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@cp $(LIBLIB) $(HADDIR_HADESGO4_LIB)/
	@rm $(LIBLIB)
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_HADESGO4_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_HADESGO4_LIB)/active/; \
		fi; \
		cp $(LIBA) $(HADDIR_HADESGO4_LIB)/active/ ;\
		rm $(LIBA) ;\
		echo static library created; \
	fi
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) $(INCMDC) $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:	%.cc
	$(CXX) $(CXXFLAGS) $(INCMDC) -c $<

%.html : %.cc %.h $(SOLIB)
	@echo "{" > doctemp.C
	@echo "gROOT->LoadMacro(\"MakeClassDoc.C\");" >> doctemp.C
	@echo "MakeClassDoc(\"$*\");" >> doctemp.C
	@echo "}" >> doctemp.C
	@root -q -l doctemp.C
	@rm doctemp.C

%.d:	%.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $(INCMDC) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'
