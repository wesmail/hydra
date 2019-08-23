include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME	= Start
LIBH	= hstartdetector.h hstartraw.h hstartcal.h hstarthit.h \
	  hstarttaskset.h hstartcontfact.h \
	  hstartunpacker.h hstartunpacker99.h hstartcalibrater.h hstarthitf.h \
	  hstartcalpar.h hstartlookup.h hstartgeompar.h hstartselfcopar.h \
	  hstartparrootfileio.h hstartparasciifileio.h\
	  htboxdetector.h htboxchan.h htboxchansum.h htboxsum.h
#	  hstartmonitor.h

LIBL	= $(LIBNAME)LinkDef.h
LIBS	= $(LIBH:.h=.cc)
LIBO	= $(LIBS:.cc=.o) 
LIBDS	= $(LIBNAME)Cint.cc
LIBDH	= $(LIBDS:.cc=.h)
LIBDO	= $(LIBDS:.cc=.o)
LIBLIB	= lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)
LIBDEP	= $(LIBS:.cc=.d)
CXXFLAGS += -I$(HADDIR_TOFINO)
CINTINC  += -I$(HADDIR_TOFINO)
##########################################

shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO) 
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@ar -r $(LIBA) *.o
	@mv $(LIBLIB) $(HADDIR_START_LIB)
	@mv $(LIBA) $(HADDIR_START_LIB)/active
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) $(LIBH) $(LIBL)

include $(LIBDEP)
###########################################
%.o:	%.cc
	$(CXX) $(CXXFLAGS) -I$(HADDIR_TOFINO) -c $<

%.html : %.cc %.h $(SOLIB)
	@echo "{" > doctemp.C
	@echo "gROOT->LoadMacro(\"MakeClassDoc.C\");" >> doctemp.C
	@echo "MakeClassDoc(\"$*\");" >> doctemp.C
	@echo "}" >> doctemp.C
	@root -q -l doctemp.C
	@rm doctemp.C

%.d:	 %.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) -I$(HADDIR_TOFINO) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'






















