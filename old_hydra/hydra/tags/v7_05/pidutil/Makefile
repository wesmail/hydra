include $(HADDIR_BASEDIR)/Makefile.base
##########################################
#Configuration area
##########################################
LIBNAME	= PidUtil
LIBH    = hpidfl.h hpidmdcfunc.h hpidpdfmaker.h 

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

CINTINC += -I$(HADDIR_TOOLS)
########################################
shlib: $(LIBLIB) 

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@cp $(LIBLIB) $(HADDIR_PIDUTIL_LIB)/
	@rm $(LIBLIB)
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_PIDUTIL_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_PIDUTIL_LIB)/active/; \
		fi; \
		cp $(LIBA) $(HADDIR_PIDUTIL_LIB)/active/ ;\
		rm $(LIBA) ;\
		echo static library created; \
	fi
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) $(HADDIR_PID)/piddef.h $(LIBH) $(LIBL)
include $(LIBDEP)

loud:
	echo $(CINTINC)

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

