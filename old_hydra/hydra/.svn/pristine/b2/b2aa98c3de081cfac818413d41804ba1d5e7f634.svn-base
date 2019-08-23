include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
##########################################
LIBNAME	= Rich
LIBH    = hdeco.h \
	  hgeantrich.h \
	  hrichanalysis.h \
	  hrichanalysispar.h \
	  hrichanalysissim.h \
	  hrichcal.h \
	  hrichcleaner.h \
	  hrichthresholdpar.h \
	  hrichcalpar.h \
	  hrichcalparcell.h \
	  hrichcalsim.h \
	  hrichcorrelatorpar.h \
	  hrichcontfact.h \
	  hrichdetector.h \
	  hrichdigitisationpar.h \
	  hrichdigitizer.h \
	  hrichframe.h \
	  hrichgeantreader.h \
	  hrichgeometrypar.h \
	  hrichhit.h \
	  hrichhitcandidate.h \
	  hrichhitfilter.h \
	  hhitmatchfilter.h \
	  hrichhitheader.h \
	  hrichhitselection.h \
	  hrichhitsim.h \
	  hrichlabel.h \
	  hrichmappingpar.h \
	  hrichobjfilter.h \
	  hrichpad.h \
	  hrichpadclean.h \
	  hrichpadfilter.h \
	  hrichpadgraph.h \
	  hrichpadlabel.h \
	  hrichpadsignal.h \
	  hrichparasciifileio.h \
	  hrichparhadasciifileio.h \
	  hrichparrootfileio.h \
	  hrichparset.h \
	  hrichparticleidentifier.h \
	  hrichpedestalraw.h \
	  hrichpedrawnew.h \
	  hrichpid.h \
	  hrichraw.h \
	  hrichringfind.h \
	  hrichringfindsim.h \
	  hrichsectorpar.h \
	  hrichtaskset.h \
	  hrichtrack.h \
	  hrichunpackercal99.h \
	  hrichunpackerraw99.h \
	  hrichwire.h \
	  hrichwiresignal.h \
	  hrichpedestalupdate.h \
	  hrichcalibrater.h 
#	  hrichunpackercal.h \
#	  hrichunpackerraw.h \
#	  hrichpedestalcal.h \
#	  hrichpedcalnew.h \
########################################

LIBL	= $(LIBNAME)LinkDef.h
LIBS	= $(LIBH:.h=.cc)
LIBO	= $(LIBS:.cc=.o)
LIBDS	= $(LIBNAME)Cint.cc
LIBDH	= $(LIBDS:.cc=.h)
LIBDO	= $(LIBDS:.cc=.o)
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)
LIBDEP	= $(LIBS:.cc=.d)

########################################
shlib: $(LIBLIB) 

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@ar -r $(LIBA) *.o
	@mv $(LIBLIB) $(HADDIR_RICH_LIB)/
	@mv $(LIBA) $(HADDIR_RICH_LIB)/active/
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) richdef.h $(LIBH) $(LIBL)

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

%.d:	 %.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

