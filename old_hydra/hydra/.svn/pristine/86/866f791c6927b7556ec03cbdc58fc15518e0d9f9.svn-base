include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = Mdc
LIBH    = hmdcdetector.h hmdctaskset.h hmdccontfact.h \
	  hmdcparrootfileio.h hmdcparasciifileio.h \
	  hmdcrawstruct.h hmdcgeomstruct.h \
	  hmdcmboreadout.h hmdcevreadout.h \
	  hmdccalpar.h hmdccalparraw.h hmdccal2par.h hmdccal2parsim.h \
          hmdccelleff.h hmdclookupgeom.h \
	  hmdclookupraw.h  hmdcdigitpar.h hmdclayergeompar.h hmdcgeompar.h \
          hmdcgeantcell.h hmdcraw.h hmdccal1.h hmdccal2.h hmdcunpacker.h \
          hmdcgeantreader.h hmdccalibrater1.h hmdccalibrater2.h \
          hmdcdigitizer.h hgeantmdc.h hmdccal1sim.h hmdccal2sim.h \
          hmdccalibrater2sim.h hmdcxy.h hmdcvertexfind.h \
	  hmdchit.h hmdcseg.h  hmdccellgroup.h hmdctrk.h hmdctimecut.h \
          hmdccutstat.h hmdcsetup.h hmdchitsim.h hmdcsegsim.h hmdcclosepaircutspar.h 

LIBL    = $(LIBNAME)LinkDef.h
LIBS    = $(LIBH:.h=.cc)
LIBO    = $(LIBS:.cc=.o)
LIBDS   = $(LIBNAME)Cint.cc
LIBDH   = $(LIBDS:.cc=.h)
LIBDO   = $(LIBDS:.cc=.o)
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)
LIBDEP  = $(LIBS:.cc=.d)
CXXFLAGS += -I$(HADDIR_MDC) -I$(HADDIR_MDCUTIL) -I$(HADDIR_MDCTRACKD) -I$(HADDIR_MDCTRACKS) -I$(HADDIR_START)
CINTINC  += -I$(HADDIR_MDCTRACKS) -I$(HADDIR_MDCTRACKD)
##########################################

shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@ar -r $(LIBA) *.o
	@mv $(LIBLIB) $(HADDIR_MDC_LIB)/
	@mv $(LIBA) $(HADDIR_MDC_LIB)/active/
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) hmdcdef.h $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:	%.cc
	$(CXX) $(CXXFLAGS) -c $<

%.html : %.cc %.h $(SOLIB)
	@echo "{" > doctemp.C
	@echo "gROOT->LoadMacro(\"MakeClassDoc.C\");" >> doctemp.C
	@echo "MakeClassDoc(\"$*\");" >> doctemp.C
	@echo "}" >> doctemp.C
	@root -q -l doctemp.C
	@rm doctemp.C

%.d:	%.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'




