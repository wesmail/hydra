include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = Alignment
LIBH    = hmdcaligner.h hmdcgeomparf.h hmdcmetahit.h hmdcmetaaligner.h \
			hmetaaligner.h hmetaalignerutil.h hmetaalignerutil2.h hmdcaligner3M.h
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
	@mv $(LIBLIB) $(HADDIR_ALIGNMENT_LIB)/
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_ORA_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_ORA_LIB)/active/; \
		fi; \
		mv $(LIBA) $(HADDIR_ORA_LIB)/active/ ;\
		echo static library created; \
	fi
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) -I$(HADDIR_MDC) -I$(HADDIR_MDCTRACKS) -I$(HADDIR_MDCTRACKD)  $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:	%.cc
	$(CXX) $(CXXFLAGS) -I$(HADDIR_MDC) -I$(HADDIR_MDCTRACKS) -c $<

%.html : %.cc %.h $(SOLIB)
	@echo "{" > doctemp.C
	@echo "gROOT->LoadMacro(\"MakeClassDoc.C\");" >> doctemp.C
	@echo "MakeClassDoc(\"$*\");" >> doctemp.C
	@echo "}" >> doctemp.C
	@root -q -l doctemp.C
	@rm doctemp.C

%.d:	%.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) -I$(HADDIR_MDC) -I$(HADDIR_MDCTRACKS) -I$(HADDIR_MDCTRACKD) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'








