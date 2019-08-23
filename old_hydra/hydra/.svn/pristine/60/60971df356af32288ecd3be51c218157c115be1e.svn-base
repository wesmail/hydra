include $(HADDIR_BASEDIR)/Makefile.base 
#ifdef MYDEVHADDIR
# include $(MYDEVHADDIR)/Makefile.base.local
#else 
# include $(HADDIR)/Makefile.base
#endif

##########################################
#Configuration area
#########################################
ifdef MYDEVHADDIR
INCMDC       = -I$(HADDIR_MDC) -I$(HADDIR_MDCTRACKD) -I$(HADDIR_KICKPLANE)
HADDIRLIB    = $(MYDEVHADDIR)/lib
HADDIRMDCTD  = $(HADDIR_MDCTRACKD)
else 
INCMDC       = -I$(HADDIR)/mdc -I$(HADDIR)/mdctrackD -I$(HADDIR)/kickplane
HADDIRLIB    = $(HADDIR)/lib
HADDIRMDCTD  = $(HADDIR)/mdctrackD
endif

LIBNAME = MdcTrackG
LIBH    = hmdctrackgfield.h \
    	hmdctrackgcorrections.h \
        hmdctrackgcorrpar.h \
        hmdctrackgfieldpar.h \
        hmdctrackgspline.h \
        hmdctrackgcontfact.h\
        hmdctrackgsplinecorr.h\
        hbasetrack.h hsplinetrack.h hsplinetrackF.h \

#         hmdcsimeventinf.h \
#	  hmdc34gtest.h \
#	  hmdctimesc.h hmdcrichal.h hmdcrichalignment.h \
	  
	  
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
	@mv $(LIBLIB) $(HADDIRLIB)/.
	@mv $(LIBA) $(HADDIRLIB)/active/.
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) $(INCMDC) -I$(HADDIRMDCTD) hmdctrackgdef.h $(LIBH) $(LIBL)

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
