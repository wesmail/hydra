
include ../Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME	= Shower
LIBH	= hshowercal.h hshowercalpar.h hshowercalibrater.h hshowerraw.h \
	hbasereadparam.h hshowerreadparam.h hshowerunpacker.h hobjtable.h \
	hshowercalparcell.h hshowergreader.h hshowergeanthit.h \
	hshowergeantwire.h hshowerframe.h hshowerwire.h hshowerpad.h \
	hshowerdigidetpar.h hshowerdigitizer.h hshowerhitdigitizer.h \
	hshowerpaddigitizer.h hshowerhit.h hshowerhitfinder.h hshowerhitfpar.h\
	hshowerhitheader.h hshowerdetector.h hshowerunpackerold.h


LIBL	= $(LIBNAME)LinkDef.h
LIBS	= $(LIBH:.h=.cc)
LIBO	= $(LIBS:.cc=.o)
LIBDS	= $(LIBNAME)Cint.cc
LIBDH	= $(LIBDS:.cc=.h)
LIBDO	= $(LIBDS:.cc=.o)
LIBSLIB	= lib$(LIBNAME).$(SOEXT)
LIBLIB	= lib$(LIBNAME).$(LIBEXT)
LIBDEP	= $(LIBS:.cc=.d)

########################################
shlib: $(LIBSLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBSLIB): $(LIBO) $(LIBDO) 
	$(LD) $(SOFLAGS)$(LIBSLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBSLIB)
	@mv $(LIBSLIB) ../lib
	@echo Library $(LIBSLIB) done.

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




