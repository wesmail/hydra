
include ../Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME	= Shower
LIBH    = hshowercal.h hshowercalpar.h hshowercalibrater.h hshowerraw.h \
        hshowerunpacker.h \
        hshowercalparcell.h hshowergreader.h hshowergeanthit.h \
        hshowergeantwire.h hshowerframe.h hshowerwire.h hshowerpad.h \
        hshowerdigidetpar.h hshowerdigitizer.h hshowerhitdigitizer.h \
        hshowerpaddigitizer.h hshowerhit.h hshowerhitfinder.h hshowerhitfpar.h\
        hshowerhitheader.h hshowerdetector.h hshowerunpackerold.h \
        hshowergeometry.h hshowersectorgeom.h hshowermodulegeom.h \
        hshowerparset.h hshowerparasciifileio.h hshowerparrootfileio.h \
        hshowerasic.h hshowerparhadasciifileio.h hshowerlookupasic.h \
        hshowergrootreader.h hshowerhitfinderold.h hshowercalasic.h  \
        hgeantshower.h hshowerpid.h hshowertrack.h hshowerlookupasictab.h \
        hshowerhistcell.h hshowerhist.h hshowercalparcalculator.h \
        hshowerunpackerreal98.h hshowerunpackercalib98.h  \
        hshowerunpackerreal456.h hshowergeantreader.h hshowerunpackerreal2.h \
        hshowerunpackerreal.h hshowerunpackercalib.h hshowerunpacker98.h \
	hshowerunpackparam.h hshowerrawhist.h hshowercalparcalculatorpeak.h \
	hshowerunpackerreal2S.h hshowercalhist.h hshowerhithist.h \
	hshowercopy.h hshowerhistogram.h hshowerunpackerrealS.h \
        hshowerrawsim.h hshowercalsim.h hshowerhitsim.h hshowerhitfindersim.h


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
	@ar -r $(LIBA) *.o
	@mv $(LIBLIB) $(HADDIR)/lib/
	@mv $(LIBA) $(HADDIR)/lib/active/
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




