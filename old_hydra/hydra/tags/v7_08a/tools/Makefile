include $(HADDIR_BASEDIR)/Makefile.base
ifdef DISTCC #distributed compiler
	CXX=$(DISTCC)
endif
##########################################
#Configuration area
#########################################
LIBNAME = Tools
LIBH    = ptools.h \
	  mptools.h \
	  hptools.h \
          hreport.h

LIBL    = $(LIBNAME)LinkDef.h
LIBS    = $(LIBH:.h=.cc)
LIBO    = $(LIBS:.cc=.o)
LIBDS   = $(LIBNAME)Cint.cc
LIBDH   = $(LIBDS:.cc=.h)
LIBDO   = $(LIBDS:.cc=.o)
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBDEP  = $(LIBS:.cc=.d)
CXXFLAGS += -I$(HADDIR_MDC) -I$(HADDIR_MDCUTIL) -I$(HADDIR_MDCTRACKD) -I$(HADDIR_MDCTRACKS) -I$(HADDIR_START)
CINTINC  += -I$(HADDIR_MDCTRACKS) -I$(HADDIR_MDCTRACKD)

ifdef RFIO_SUPPORT
CXXFLAGS += -D RFIO_SUPPORT
else
CXXFLAGS += -U RFIO_SUPPORT
endif

ifdef ORACLE_HOME
CXXFLAGS += -D ORACLE_SUPPORT
else
CXXFLAGS += -U ORACLE_SUPPORT
endif

##########################################

shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@cp $(LIBLIB) $(HADDIR_TOOLS_LIB)/
	@rm $(LIBLIB)
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_TOOLS_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_TOOLS_LIB)/active/; \
		fi; \
		cp $(LIBA) $(HADDIR_TOOLS_LIB)/active/ ;\
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




