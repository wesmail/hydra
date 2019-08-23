include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
##########################################
LIBNAME = GeantUtil
LIBH    = hgeantheader.h \
          hgeantkine.h \
	  hgeantmdc.h \
	  hgeantrich.h \
	  hgeanttof.h \
	  hgeantshower.h \
          hkinesim.h

LIBL    = $(LIBNAME)LinkDef.h
LIBS    = $(LIBH:.h=.cc)
LIBO    = $(LIBS:.cc=.o)
LIBDS   = $(LIBNAME)Cint.cc
LIBDH   = $(LIBDS:.cc=.h)
LIBDO   = $(LIBDS:.cc=.o)
LIBDEP  = $(LIBS:.cc=.d)
########################################


obj: cpobj 

cpobj: $(LIBO) $(LIBDO)
	@cp $(LIBO) $(LIBDO) $(TEMPBASE)

clean:
	@rm -f $(LIBDEP) $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO)  core *~
	@echo Cleaning done for $(LIBNAME) dir.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	@rootcint -f $(LIBDS) -c -p -e $(CINTINC) haddef.h hgeantdef.h calodef.h $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:	%.cc 
	@echo $(CXX) -c $<
	@$(CXX) $(CXXFLAGS)  -c $<      

%.d:	%.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
                | sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
                [ -s $@ ] || rm -f $@'
