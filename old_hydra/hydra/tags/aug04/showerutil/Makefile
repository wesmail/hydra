include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME	= ShowerUtil
LIBH    = hsuprogress.h hsuprogressrec.h hsuhldinfo.h \
			hsuexemacro.h hsudummyrec.h hsuerrorhandler.h \
			hsupaddivider.h hsuchain.h # \
			hsuframe.h hsuactivewidget.h hsueditboxes.h hsulistboxes.h \
			hsudialog.h hsudialogframe.h hsubuttons.h
# ^^^ uncomment those lines to get GUI interface
# do the same in ShowerUtilLinkDef.h

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
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~ .main

$(LIBLIB): $(LIBO) $(LIBDO) 
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) -o $(LIBLIB)
	@ar -r $(LIBA) *.o
	@mv $(LIBLIB) $(HADDIR_SHOWERUTIL_LIB)
	@mv $(LIBA) $(HADDIR_SHOWERUTIL_LIB)/active/
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) $(LIBH) $(LIBL)

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
