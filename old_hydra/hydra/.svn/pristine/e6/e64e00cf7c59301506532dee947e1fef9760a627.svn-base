include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME	= Shower
LIBH    = hshowercal.h hshowercalpar.h hshowercalibrater.h hshowerraw.h \
        hshowerunpacker.h hshowergeantreader.h\
        hshowercalparcell.h  \
        hshowergeantwire.h hshowerframe.h hshowerwire.h hshowerpad.h \
        hshowerdigidetpar.h hshowerdigitizer.h hshowerhitdigitizer.h \
        hshowerpaddigitizer.h hshowerhit.h hshowerhitfinder.h hshowerhitfpar.h\
        hshowerhitheader.h hshowerdetector.h \
        hshowergeometry.h hshowersectorgeom.h hshowermodulegeom.h \
        hshowerparset.h hshowercontfact.h hshowerparasciifileio.h \
        hshowerparrootfileio.h hshowerparhadasciifileio.h \
        hshowerpid.h hshowertrack.h \
        hshowertaskset.h \
        hshowerhistcell.h hshowerhist.h hshowercalparcalculator.h \
		hshowerunpackparam.h hshowerrawhist.h \
		hshowercalhist.h hshowerhithist.h \
		hshowercopy.h hgeomvector2.h\
		hshowerhittrack.h hshowerpidtrack.h hshowerhittrackmatcher.h \
		hshowerpidtrackmatcher.h hshowercriterium.h hshowerparticleidentifier.h\
		hshowersubidpar.h 

########################################

LIBL	= $(LIBNAME)LinkDef.h
LIBS	= $(LIBH:.h=.cc)
LIBO	= $(LIBS:.cc=.o)
LIBDS	= $(LIBNAME)Cint.cc
LIBDH	= $(LIBDS:.cc=.h)
LIBDO	= $(LIBDS:.cc=.o)
LIBLIB	= lib$(LIBNAME).$(SOEXT)
LIBA	= lib$(LIBNAME).$(AEXT)
LIBDEP	= $(LIBS:.cc=.d)

########################################
shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO) 
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@cp $(LIBLIB) $(HADDIR_SHOWER_LIB)/
	@rm $(LIBLIB)
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_SHOWER_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_SHOWER_LIB)/active/; \
		fi; \
		cp $(LIBA) $(HADDIR_SHOWER_LIB)/active/ ;\
		rm $(LIBA) ;\
		echo static library created; \
	fi
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) showerdef.h $(LIBH) $(LIBL)

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




