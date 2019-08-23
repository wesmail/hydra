# $Id: Makefile,v 1.5 2005-08-25 08:21:19 wuestenf Exp $
# Author: Thomas.Eberl@ph.tum.de, last modified : 2005-04-27 19:44:24
include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
##########################################
LIBNAME	= Pairs
LIBH    = hpair.h \
	  hpairsim.h \
	  hpaireffpar.h \
          hpairfiller.h \
	  hpairfilter.h \
	  hpairfl.h \
	  hpairqa.h \
	  hpairdata.h \
	  hpairgeantdata.h \
	  hpairhisto.h \
	  hpaircontfact.h \
	  hpaircutpar.h
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
	@cp $(LIBLIB) $(HADDIR_PAIRS_LIB)/
	@rm $(LIBLIB)
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_PAIRS_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_PAIRS_LIB)/active/; \
		fi; \
		mv $(LIBA) $(HADDIR_PAIRS_LIB)/active/ ;\
		echo static library created; \
	fi
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) pairsdef.h $(LIBH) $(LIBL)

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

