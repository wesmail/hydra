# @(#)$Id: Makefile,v 1.17 2005-01-12 10:47:46 wuestenf Exp $
include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = Pid
LIBH    = hpidhitdata.h hpidtrackdata.h hpidtrackcand.h \
	  hpidtrackcandsim.h \
	  hpidgeanttrackset.h \
	  hpidtrackfiller.h \
	  hpidparcont.h \
	  hpidparasciifileio.h \
	  hpidparrootfileio.h \
	  hpidcontfact.h \
	  hpidtrackfillerpar.h \
	  hpidhistogramscont.h \
	  hpidparticlefiller.h \
	  hpidparticle.h \
	  hpidparticlesim.h \
	  hpidreconstructor.h \
	  hpidreconstructorpar.h \
	  hpidalgorithm.h \
	  hpidalgmomvsbeta.h \
	  hpidalgmomvsbetapar.h \
          hpidcandidate.h \
          hpidgaussconf.h
	  #hpidtrackeval.h \


########################################

LIBL    = $(LIBNAME)LinkDef.h
LIBS    = $(LIBH:.h=.cc)
LIBO    = $(LIBS:.cc=.o)
LIBDS   = $(LIBNAME)Cint.cc
LIBDH   = $(LIBDS:.cc=.h)
LIBDO   = $(LIBDS:.cc=.o)
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)
LIBDEP  = $(LIBS:.cc=.d)

########################################
CINTINC += -I$(HADDIR_PIDUTIL)

shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO)
#	$(LD) $(SOFLAGS) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@cp $(LIBLIB) $(HADDIR_PID_LIB)/
	@rm $(LIBLIB)
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_PID_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_PID_LIB)/active/; \
		fi; \
		cp $(LIBA) $(HADDIR_PID_LIB)/active/ ;\
		rm $(LIBA) ;\
		echo static library created; \
	fi
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:    %.cc
	$(CXX) $(CXXFLAGS) -c $<

%.html : %.cc %.h $(SOLIB)
	@echo "{" > doctemp.C
	@echo "gROOT->LoadMacro(\"MakeClassDoc.C\");" >> doctemp.C
	@echo "MakeClassDoc(\"$*\");" >> doctemp.C
	@echo "}" >> doctemp.C
	@root -q -l doctemp.C
	@rm doctemp.C

%.d:     %.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
	| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
	[ -s $@ ] || rm -f $@'




