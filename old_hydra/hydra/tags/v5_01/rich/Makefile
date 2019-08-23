include ../Makefile.base

##########################################
#Configuration area
##########################################
LIBNAME	= Rich
LIBH    = hrichraw.h hrichunpackerraw.h hrichunpackercal.h  \
          hrichgeantreader.h hrichcal.h hrichdisplayraw.h hrichdisplaycal.h\
          hrichwiresignal.h hrichframe.h hrichwire.h \
          hrichcalibrater.h hrichhitheader.h hrichhit.h \
          hrichdetector.h hrichanalysis.h hrichanalysispar.h hrichpad.h \
          hrichpadsignal.h hrichlabel.h hrichpadgraph.h hrichpadclean.h \
          hrichpadlabel.h hrichringfind.h hgeantrich.h \
          hrichcalpar.h hrichcalparcell.h hrichgeometrypar.h  \
          hrichparhadasciifileio.h hrichparrootfileio.h hrichparset.h \
          hrichsectorpar.h \
          hrichparasciifileio.h hrichdigitisationpar.h hrichdigitizer.h \
          hrichpedestalraw.h hrichpedestalcal.h hrichcalsim.h hrichtrack.h  

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
	@mv $(LIBLIB) $(HADDIR)/lib/
	@mv $(LIBA) $(HADDIR)/lib/active/
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

