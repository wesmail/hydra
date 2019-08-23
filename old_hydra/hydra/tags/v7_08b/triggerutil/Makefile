include $(HADDIR_BASEDIR)/Makefile.base
##########################################
#Configuration area
##########################################
LIBNAME	= TriggerUtil
LIBH    = hrichanalysisIPU.h \
	  hrichanalysisIPUSim.h \
	  hrichringfindIPU.h \
	  hrichringfindIPUSim.h \
          hrichIPUtaskset.h \
	  hrichhitIPU.h \
	  hrichhitIPUSim.h \
          hrichIPUparlocmax.h \
          hrichIPUparpattern.h \
          hrichIPUparthresholds.h \
          hrichIPUremake.h \
          hrichIPUremakeSim.h \
          hrichIPUmap.h \
          hlocmaxfilter.h \
          hlocmaxfilterSim.h \
	  hshowercalibraterIPU.h \
	  hshowerhitfinderIPU.h \
	  hshowercalIPU.h \
	  hshowerhitIPU.h \
          hshowercheckIPU.h
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

CXXFLAGS += -I$(HADDIR_RICH) -I$(HADDIR_TRIGGER)
CINTINC  += -I$(HADDIR_RICH) -I$(HADDIR_TRIGGER)
########################################
shlib: $(LIBLIB) 

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@cp $(LIBLIB) $(HADDIR_TRIGGERUTIL_LIB)/
	@rm $(LIBLIB)
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_TRIGGERUTILC_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_TRIGGERUTIL_LIB)/active/; \
		fi; \
		mv $(LIBA) $(HADDIR_TRIGGERUTIL_LIB)/active/ ;\
		echo static library created; \
	fi
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	@rootcint -f $(LIBDS) -c $(CINTINC) $(HADDIR_RICH)/richdef.h $(LIBH) $(LIBL)
include $(LIBDEP)
#########################################
%.o:	%.cc 
	@echo $(CXX) -c $<
	@$(CXX) $(CXXFLAGS) -c $<

%.html : %.cc %.h $(SOLIB)
	@echo "{" > doctemp.C
	@echo "gROOT->LoadMacro(\"MakeClassDoc.C\");" >> doctemp.C
	@echo "MakeClassDoc(\"$*\");" >> doctemp.C
	@echo "}" >> doctemp.C
	@root -q -l doctemp.C
	@rm doctemp.C

%.d:    %.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'



