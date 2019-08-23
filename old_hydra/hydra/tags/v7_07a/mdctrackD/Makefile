include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################

LIBNAME = MdcTrackD
LIBH    = hmdctrackdcontfact.h   \
          hmdcgeomobj.h          \
          hmdcsizescells.h       \
          hmdcclfnstack.h        \
          hmdcclus.h             \
          hmdcclussim.h          \
          hmdclookuptb.h         \
          hmdc34clfinder.h       \
	  hmdcefficiency.h       \
          hmdctrackfinder.h      \
          hmdctrackdset.h        \
	  hmdcclustertohit.h     \
          hmdctbarray.h          \
          hmdclistcells.h        \
          hmdclistgroupcells.h   \
          hmdcgetcontainers.h    \
          hmdctrackfitter.h      \
          hmdctrackfittera.h     \
	  hmdctrackfitterb.h     \
	  hmdctrackparam.h       \
	  hmdcwiredata.h         \
          hmdc12fit.h            \
	  hmdctrackfitpar.h      \
          hmdcclusfit.h          \
	  hmdcclusfitsim.h       \
          hmdcwirefit.h          \
	  hmdcwirefitsim.h       \
	  hmdceffmenu.h          \
          hmdctargetmon.h        \
          hmdcclusinf.h          \
          hmdcclusinfsim.h       \
	  hmdcrichcorr.h         \
          hmdchitsegfiller.h     \
	  hseqarr.h              \
	  hmdcstoreevents.h      \
          hmdcalignerd.h         \
	  hmdcalignerparam.h     \
          hmdcminimize.h         \
          hmdcdedx.h             \
          hmdcdedxmaker.h        \
	  hmdcidealtracking.h
	  
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
	@cp $(LIBLIB) $(HADDIR_MDCTRACKD_LIB)/
	@rm $(LIBLIB)
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_MDCTRACKD_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_MDCTRACKD_LIB)/active/; \
		fi; \
		cp $(LIBA) $(HADDIR_MDCTRACKD_LIB)/active/ ;\
		rm $(LIBA) ;\
		echo static library created; \
	fi
	@echo library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) hmdctrackddef.h $(LIBH) $(LIBL)

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
