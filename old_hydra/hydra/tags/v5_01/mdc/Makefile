include ../Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = Mdc
LIBH    = hmdcdetector.h hmdcparrootfileio.h hmdcparasciifileio.h \
	  hmdcrawstruct.h hmdcgeomstruct.h \
	  hmdcmboreadout.h hmdcevreadout.h \
	  hmdccalpar.h hmdccalparraw.h hmdclookupgeom.h \
	  hmdcconnectedwires.h  hmdclookupraw.h \
	  hmdcdigitpar.h hmdclayergeompar.h hmdcgeompar.h \
          hmdcgeantcell.h hmdcraw.h hmdccal1.h hmdccal2.h hmdcunpacker.h \
          hmdcgeantreader.h hmdccalibrater1.h hmdccalibrater2.h \
          hmdcdigitizer.h hgeantmdc.h hmdccal1sim.h hmdccal2sim.h \
          hmdccalibrater2sim.h hmdcmonitor.h \
	  hmdcevtdisplay.h hmdcnodes.h hmdccal1evtdisplay.h hmdcxy.h \
	  hmdcoffset.h hmdcslope.h

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
	@ar -r $(LIBA) *.o
	@mv $(LIBLIB) $(HADDIR)/lib/
	@mv $(LIBA) $(HADDIR)/lib/active/
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




