# @(#)$Id: Makefile,v 1.14 2004-04-30 16:25:13 christ Exp $
include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = Pid
LIBH    = hpidtrackcand.h hpidtrackfiller.h hpidtrackfillersim.h \
		hpidgeanttrackset.h hpidcommontrack.h hpidcontfact.h hpidparcont.h \
		hpidparrootfileio.h hpidparasciifileio.h \
		hpidhistogramscont.h \
		hpidtrackfillerpar.h hpidreconstructorpar.h \
		hpidalgmomvsbetapar.h hpidgaussconf.h \
		hpidcandidate.h hpidalgorithm.h hpidreconstructor.h \
		hpidalgmomvsbeta.h \
		hpidalgmomvseloss.h hpidalgmomvselosspar.h hpidelossmpvshiftpar.h\
		hpidalgringproperties.h hpidalgshower.h\
		hpidalgringpropertiespar.h hpidalgshowerpar.h\
		hpidparticle.h hpidparticlesim.h hpidparticlesimext.h\
		hpidparticlehit.h hpidparticlesimexthit.h \
		hpidparticlefiller.h hpidhitdata.h hpidmdcfunc.h \
		hpiddilepton.h hpiddileptonfiller.h \
		hpidcandidate_4_8.h hpidcandidate_3_8.h hpidcandidate_1_8.h \
		hpidcandidate_5_8.h hpidcandidate_7_8.h hpidcandidate_2_8.h \
		hpidsimtrigger.h hpidmomimp.h hpidmomimpbeta.h \
		hpidcandidate_2_9.h hpidcandidate_3_9.h hpidcandidate_4_9.h \
		hpidcandidate_5_9.h hpidcandidate_6_9.h hpidcandidate_3_5.h \
                hpidalgleptoncand.h hpidcandidate_3_2.h hpidcandidate_5_5.h \
		hpidcandidate_7_5.h 

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
shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO)
#	$(LD) $(SOFLAGS) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@ar -r $(LIBA) *.o
	@mv $(LIBLIB) $(HADDIR_PID_LIB)/
	@mv $(LIBA) $(HADDIR_PID_LIB)/active/
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




