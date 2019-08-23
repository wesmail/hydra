include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = Kick
LIBH    = hsurface.h hkicktaskset.h hkickplane.h hkicktrackf.h hkickdetdigi.h hmdcsimhit.h hkickgrid.h hkicktrackfpar.h hkickimpact.h hkickcandidate.h hkickcandidateevaluator.h hmdcsimseg.h htofhitsim2.h hkickparasciifileio.h hkicktrack.h hkicktask.h hkickmassfit.h hkickmdctrk.h hkickmdctrkf.h hkickmomfitd.h hkickmatch.h hkickparrootfileio.h hkickcontfact.h

CXXFLAGS += -I$(HADDIR_MDC) \
            -I$(HADDIR_TOF) \
            -I$(HADDIR_SHOWER) \
            -I$(HADDIR_KICKPLANE) \
	    -I$(HADDIR_SHOWERTOFINO) \
	    -I$(HADDIR_MDCTRACKD) 
CINTINC	+= -I$(HADDIR_TOF) \
           -I$(HADDIR_MDC) \
           -I$(HADDIR_SHOWER) \
           -I$(HADDIR_SHOWERTOFINO) \
           -I$(HADDIR_MDCTRACKD) 

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
	@mv $(LIBLIB) $(HADDIR_KICKPLANE_LIB)/
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) kickdef.h $(LIBH) $(LIBL)

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

%.d:	%.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'




