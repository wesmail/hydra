include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = Mdc
LIBH    = hgeantmdc.h \
          hmdccalpar.h \
          hmdccalparraw.h \
          hmdccal1.h \
          hmdccal1sim.h \
          hmdccal2.h \
          hmdccal2par.h \
          hmdccal2sim.h \
          hmdccal2parsim.h \
          hmdccalibrater1.h \
          hmdccalibrater2.h \
          hmdccalibrater2sim.h \
          hmdccelleff.h \
          hmdccellgroup.h \
          hmdcclosepaircutspar.h \
          hmdccontfact.h \
          hmdccutstat.h \
          hmdcdataword.h \
          hmdcdetector.h \
	  hmdcdedx.h \
          hmdcdigitpar.h \
          hmdcevreadout.h \
          hmdcgeompar.h \
          hmdcgeomstruct.h \
          hmdcgeantcell.h \
          hmdcgeantreader.h \
          hmdcdigitizer.h \
          hmdchit.h \
          hmdchitsim.h \
          hmdclayergeompar.h \
          hmdclookupgeom.h \
          hmdclookupraw.h \
          hmdcmboreadout.h \
          hmdcparrootfileio.h \
          hmdcparasciifileio.h \
          hmdcraw.h \
          hmdcrawstruct.h \
          hmdcraweventheader.h \
          hmdcseg.h \
          hmdcsegsim.h \
          hmdcsetup.h \
          hmdctaskset.h \
          hmdctdcthreshold.h \
	  hmdctimecut.h \
          hmdctrk.h \
          hmdcvertexfind.h \
	  hmdcwirestat.h \
          hmdcunpacker.h \
	  hmdctrkcand.h \
	  hmdcxy.h \
	  hmdctdcchannel.h  	

LIBL    = $(LIBNAME)LinkDef.h
LIBS    = $(LIBH:.h=.cc)
LIBO    = $(LIBS:.cc=.o)
LIBDS   = $(LIBNAME)Cint.cc
LIBDH   = $(LIBDS:.cc=.h)
LIBDO   = $(LIBDS:.cc=.o)
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)
LIBDEP  = $(LIBS:.cc=.d)
# CXXFLAGS += -I$(HADDIR_MDC) -I$(HADDIR_MDCUTIL) -I$(HADDIR_MDCTRACKD) -I$(HADDIR_MDCTRACKS) -I$(HADDIR_START)
CXXFLAGS += -I$(HADDIR_MDCUTIL) -I$(HADDIR_MDCTRACKD) -I$(HADDIR_MDCTRACKS) -I$(HADDIR_START)
CINTINC  += -I$(HADDIR_MDCUTIL) -I$(HADDIR_MDCTRACKS) -I$(HADDIR_MDCTRACKD)
##########################################

shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@mv $(LIBLIB) $(HADDIR_MDC_LIB)/
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_MDC_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_MDC_LIB)/active/; \
		fi; \
		mv $(LIBA) $(HADDIR_MDC_LIB)/active/ ;\
		echo static library created; \
	fi
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


test:
	@echo CXXFLAGS: $(CXXFLAGS)