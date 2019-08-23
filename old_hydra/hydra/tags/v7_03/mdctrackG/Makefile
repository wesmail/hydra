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
        hbasetrack.h \
        hmetamatch.h \
        hmetamatchF.h \
        hmetamatchpar.h \
        hsplinetrack.h \
        hsplinetrackF.h \
	hkicktrackB.h \
        hkicktracksimB.h \
        hkicktrack123B.h \
        hkicktrackbaseF.h \
        hrungekutta.h \
        hrktrackB.h \
        hrktrackBF.h \
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
	@cp $(LIBLIB) $(HADDIR_MDCTRACKG_LIB)/
	@rm $(LIBLIB)
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_MDCTRACKG_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_MDCTRACKG_LIB)/active/; \
		fi; \
		cp $(LIBA) $(HADDIR_MDCTRACKG_LIB)/active/ ;\
		rm $(LIBA) ;\
		echo static library created; \
	fi
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
