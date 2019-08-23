
include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
INCONLINE  = -I$(HADDIR_TOF) -I$(HADDIR_TOFUTIL)
LIBNAME	= Online
LIBH	= honlinecontrol.h hparameter.h hcontour.h hcontourcont.h \
	  hconditionout.h \
	  hlocalcondition.h hexpstring.h hlocalcont.h hdetset.h \
	  hdetsetcont.h hindextransform.h herror.h \
	  hhistogram.h hhistocont.h hglobalcondition.h \
	  hglobalcont.h honlprocessevent.h hconsistency.h hmemorycheck.h \
	  hcalclocation.h hconsistencywarn.h honlobjbrowser.h hbrowserdb.h \
	  hprojection.h hhistovisual.h hdisplaydb.h honlmainwindow.h \
	  hconfigrun.h hconfigure.h hslice.h hpreferences.h hcontourmanager.h \
	  hconversion.h

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
	@mv $(LIBA) $(HADDIR_ONLINE_LIB)/active/
	@mv $(LIBLIB) $(HADDIR_ONLINE_LIB)/
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) $(INCONLINE) tofdef.h $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:	%.cc
	$(CXX) $(CXXFLAGS) $(INCONLINE) -c $<

%.html : %.cc %.h $(SOLIB)
	@echo "{" > doctemp.C
	@echo "gROOT->LoadMacro(\"MakeClassDoc.C\");" >> doctemp.C
	@echo "MakeClassDoc(\"$*\");" >> doctemp.C
	@echo "}" >> doctemp.C
	@root -q -l doctemp.C
	@rm doctemp.C

%.d:	 %.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $(INCONLINE) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'



