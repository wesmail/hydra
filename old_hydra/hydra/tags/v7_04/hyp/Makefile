include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = Hyp
LIBH    = hhypbasealgorithm.h hhypreconstructor.h hhypPPPipPimXAlg.h \
	  hhypchannel.h hhypcomb.h hhypkine.h hhyplist.h hhyplistfiller.h hhyplistreader.h \
	  hhypPPEpEmXAlg.h hhypPPPipPimProjector.h hhypPPPipPimPi0Alg.h

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
	@mv $(LIBLIB) $(HADDIR_HYP_LIB)/
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_HYP_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_HYP_LIB)/active/; \
		fi; \
		mv $(LIBA) $(HADDIR_HYP_LIB)/active/ ;\
		echo static library created; \
	fi
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) hypinfodef.h $(LIBH) $(LIBL)

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




