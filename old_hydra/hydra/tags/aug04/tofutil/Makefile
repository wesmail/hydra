include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME	= TofUtil
LIBH	= htofmonitor.h htofconfigpanel.h \
	  htofsector.h htofeventdisplay.h htofhistaction.h \
	  htofrod.h htofgeom.h

LIBL	= $(LIBNAME)LinkDef.h
LIBS	= $(LIBH:.h=.cc)
LIBO	= $(LIBS:.cc=.o)
LIBDS	= $(LIBNAME)Cint.cc
LIBDH	= $(LIBDS:.cc=.h)
LIBDO	= $(LIBDS:.cc=.o)
LIBLIB	= lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)
LIBDEP	= $(LIBS:.cc=.d)
##########################################

shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO) 
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@ar -r $(LIBA) *.o
	@mv $(LIBA) $(HADDIR_TOF_LIB)/active
	@mv $(LIBLIB) $(HADDIR_TOF_LIB)
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) tofdef.h $(LIBH) $(LIBL)

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



