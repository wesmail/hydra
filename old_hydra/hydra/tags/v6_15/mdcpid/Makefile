include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = MdcPid
LIBH    = hmdcdedxhists.h \
          hmdccpselector.h \
          hmdccpcutsfinder.h \
	  hcategorymaker.h \
	  hmdcextractor.h \
	  hmdcextractors.h \
	  hmdcwireinfo.h

LIBL    = $(LIBNAME)LinkDef.h
LIBS    = $(LIBH:.h=.cc)
LIBO    = $(LIBS:.cc=.o)
LIBDS   = $(LIBNAME)Cint.cc
LIBDH   = $(LIBDS:.cc=.h)
LIBDO   = $(LIBDS:.cc=.o)
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)
LIBDEP  = $(LIBS:.cc=.d)
CXXFLAGS += -I$(HADDIR_MDC) -I$(HADDIR_MDCTRACKD) -I$(HADDIR_MDCTRACKS) -I$(HADDIR_MDCUTIL) -I$(HADDIR_START) -I$(HADDIR_KICKPLANE) -I$(HADDIR_PID) -I$(HADDIR_PIDUTIL)
CINTINC  += -I$(HADDIR_MDCTRACKS) -I$(HADDIR_MDCTRACKD) -I$(HADDIR_MDCUTIL) -I$(HADDIR_START) -I$(HADDIR_KICKPLANE) -I$(HADDIR_PID) -I$(HADDIR_PIDUTIL)
##########################################

shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@mv $(LIBLIB) $(HADDIR_MDCPID_LIB)
	@if [ "$(ACTIVELIB)" = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_MDCPID_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_MDCPID_LIB)/active/; \
		fi; \
		mv $(LIBA) $(HADDIR_MDCPID_LIB)/active/ ;\
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




