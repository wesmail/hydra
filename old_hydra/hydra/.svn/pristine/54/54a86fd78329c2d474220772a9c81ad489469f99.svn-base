include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = Dst
LIBH    = hdstutilities.h  \
	  hdstsimulation.h \
          hdstrealdata.h   \
	  hdstproduction.h

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
	@cp $(LIBLIB) $(HADDIR_MDC_LIB)/
	@rm $(LIBLIB)
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_MDC_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_MDC_LIB)/active/; \
		fi; \
		cp $(LIBA) $(HADDIR_MDC_LIB)/active/ ;\
		rm $(LIBA) ;\
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
