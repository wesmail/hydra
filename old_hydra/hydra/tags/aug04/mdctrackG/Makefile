include $(HADDIR_BASEDIR)/Makefile.base 

##########################################
#Configuration area
#########################################


LIBNAME = MdcTrackG
LIBH    = hmdctrackgfield.h \
    	hmdctrackgcorrections.h \
        hmdctrackgcorrpar.h \
        hmdctrackgfieldpar.h \
        hmdctrackgspline.h \
        hmdctrackgcontfact.h \
        hmdctrackgsplinecorr.h \
        hbasetrack.h \
        hmetamatch.h \
        hmetamatchF.h \
        hsplinetrack.h \
        hsplinetrackF.h \
	hkicktrackB.h \
        hkicktracksimB.h \
        hkicktrack123B.h \
        hkicktrackbaseF.h \
	hsplinetaskset.h \
	hsplinetofclF.h \
        htofrec.h
#	htrackmatchf.h
	  	  
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
	@mv $(LIBLIB) $(HADDIR_MDCTRACKG_LIB)/.
	@mv $(LIBA) $(HADDIR_MDCTRACKG_LIB)/active/.
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) hmdctrackgdef.h $(LIBH) $(LIBL)

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
