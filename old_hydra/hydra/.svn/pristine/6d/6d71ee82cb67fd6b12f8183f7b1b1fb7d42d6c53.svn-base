include $(HADDIR_BASEDIR)/Makefile.base
##########################################
#Configuration area
##########################################
LIBNAME	= RichUtil
LIBH    = hrichcorrelator.h \
	  hrichpca.h \
	  hrichmon.h \
	  hrichpcasim.h \
	  hrichpaircreator.h \
	  hrichpaircreatorsim.h \
	  hrichcut.h \
	  hrichcorrelatorsim.h \
	  hrichhistfac.h \
	  hdihitmatch.h \
	  hdihitmatchsim.h \
	  hrichcorrcounter.h \
	  hrichcorrcountersim.h \
	  hrichtrackletfilter.h \
	  hrichtrackletfiltersim.h \
	  hrichevtfilter.h \
	  hrichevtfiltersim.h \
	  htrackinfo.h \
	  hgeantkineana.h \
	  hrichpairana.h \
	  hrichpairanasim.h \
	  hhitmatch.h \
	  hhitmatchsim.h \
	  hhitmatchheadersim.h \
	  hhitmatchheader.h \
	  hrichdumphit.h \
	  hrichdumphitsim.h \
	  hrichdumpcal.h \
	  hrichdumpraw.h \
	  hrichlocal.h \
	  hrichsender.h \
	  hrichsendercal.h \
	  hrichsenderraw.h \
	  hrichsenderrawaccu.h \
	  hrichphotonfinder.h \
	  hrichphotonhit.h \
	  hrichphotonhitheader.h \
	  hrichdisplaycal.h \
	  hrichdisplayraw.h \
	  hrichhitfit.h \
	  hrichchernovringfitter.h \
	  hrichrecalibrater.h \
	  hrichlocalmaxcal.h \
	  hrichlocalmaxsim.h \
	  hrichcalmax.h \
	  hrichutilfunc.h \
	  hconstant.h \
	  hrichdiag.h \
          hrichcategoryviewer.h \
	  hrichlocalmaxheader.h \
	  hrichN0cal.h \
	  hlocalselection.h \
	  hlocalshift.h \
	  hphotoncluster.h \
	  hclusterselector.h \
	  hclusterselectorsim.h
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
	@ar -r $(LIBA) *.o
	@mv $(LIBLIB) $(HADDIR_RICHUTIL_LIB)/
	@mv $(LIBA) $(HADDIR_RICHUTIL_LIB)/active/
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) $(HADDIR_RICH)/richdef.h $(LIBH) $(LIBL)
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

