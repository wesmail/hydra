include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
##########################################
LIBNAME	= EventHandling
LIBH	= hades.h hdetector.h hevent.h heventheader.h \
          hpartialevent.h hrecevent.h hreconstructor.h hspectrometer.h \
          htask.h htaskset.h htree.h
LIBL	= $(LIBNAME)LinkDef.h
LIBS	= $(LIBH:.h=.cc)
LIBO	= $(LIBS:.cc=.o)
LIBDS	= $(LIBNAME)Cint.cc
LIBDH	= $(LIBDS:.cc=.h)
LIBDO	= $(LIBDS:.cc=.o)
LIBDEP	= $(LIBS:.cc=.d)
########################################


obj: cpobj 

cpobj: $(LIBO) $(LIBDO)
	@cp $(LIBO) $(LIBDO) $(TEMPBASE)

clean:
	@rm -rf $(LIBDEP) $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO)  core *~
	@echo Cleaning done for $(LIBNAME) dir.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	@rootcint -f $(LIBDS) -c $(CINTINC) $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:	%.cc 
	@echo $(CXX) -c $<
	@$(CXX) $(CXXFLAGS)  -c $<	

%.d:	 %.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'





















