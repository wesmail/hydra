include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = Trigger
LIBH    = hmatchuunpacker.h hmatchurich.h hmatchushower.h \
          hmatchutof.h hmatchutofheader.h hmuleptons.h hmuscalers.h hmudata.h \
          hmuEmuleptons.h muEmulation.h	htriggertaskset.h htriggerdetector.h \
          muDilepEmulation.h htriggerparmulep.h htriggerparmudilep.h \
	  htriggerparasciifileio.h htriggerparrootfileio.h htriggercontfact.h \
          htriggerparmommap.h htriggerparshowermap.h htriggerparrichmap.h \
          muEmulationSim.h muDilepEmulationSim.h hmuEmuleptonsSim.h  \
          muEmulationExp.h muDilepEmulationExp.h hmuEmuleptonsExp.h

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
	@mv $(LIBLIB) $(HADDIR_TRIGGER_LIB)/
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_TRIGGER_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_TRIGGER_LIB)/active/; \
		fi; \
		mv $(LIBA) $(HADDIR_TRIGGER_LIB)/active/ ;\
		echo static library created; \
	fi
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) triggerinfodef.h $(LIBH) $(LIBL)

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




