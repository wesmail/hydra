include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
##########################################
LIBNAME	= RuntimeDb
LIBH	= hruntimedb.h hcontfact.h hbasecontfact.h hparset.h hparcond.h \
	  hparamlist.h hrun.h hpario.h hdetpario.h \
	  hparrootfileio.h  hdetparrootfileio.h \
	  hcondparrootfileio.h hspecparrootfileio.h \
	  hparasciifileio.h hdetparasciifileio.h \
	  hcondparasciifileio.h hspecparasciifileio.h \
	  hadioascii.h hadascii.h  hdetparhadasciifileio.h hparhadasciifileio.h
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
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~
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





















