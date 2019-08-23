include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
INCMDC  = -I$(HADDIR_MDC) -I$(HADDIR_KICKPLANE)

LIBNAME = MdcTrackD
LIBH    = hmdctrackdcontfact.h hmdcgeomobj.h hmdcsizescells.h hmdcclfnstack.h \
          hmdcclus.h hmdcclussim.h hmdclookuptb.h hmdc34clfinder.h \
	  hmdcefficiency.h hmdctrackfinder.h hmdctrackdset.h \
	  hmdcclustertohit.h hmdctbarray.h hmdclistcells.h hmdcgetcontainers.h \
          hmdctrackfitter.h hmdctrackfitterh.h hmdctrackfittera.h hmdc12fit.h \
	  hmdctrackfitpar.h hmdcclusfit.h hmdcwirefit.h \
	  hmdceffmenu.h hmdctargetmon.h hmdcclusinf.h hmdcclusinfsim.h \
	  hmdcrichcorr.h
	  
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
	@mv $(LIBLIB) $(HADDIR_MDCTRACKD_LIB)/.
	@mv $(LIBA) $(HADDIR_MDCTRACKD_LIB)/active/.
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) $(INCMDC) hmdctrackddef.h $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:	%.cc
	$(CXX) $(CXXFLAGS) $(INCMDC) -c $<

%.html : %.cc %.h $(SOLIB)
	@echo "{" > doctemp.C
	@echo "gROOT->LoadMacro(\"MakeClassDoc.C\");" >> doctemp.C
	@echo "MakeClassDoc(\"$*\");" >> doctemp.C
	@echo "}" >> doctemp.C
	@root -q -l doctemp.C
	@rm doctemp.C

%.d:	%.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $(INCMDC) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'
