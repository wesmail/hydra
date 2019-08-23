include ../../Makefile.base
##########################################
#Configuration area
##########################################
LIBNAME	= Geometry
LIBH	= hgeomtransform.h hgeomvector.h hgeomrotation.h \
	  hgeomvolume.h hgeomcompositevolume.h hspecgeompar.h hdetgeompar.h \
	  hgeomshapes.h hgeombasicshape.h hgeombrik.h  hgeomtrap.h \
	  hgeomtrd1.h hgeompgon.h hgeompcon.h hgeomtube.h hgeomtubs.h \
	  hgeomcone.h hgeomcons.h hgeomsphe.h hgeomeltu.h \
	  heventdisplay.h  hdetevtdisplay.h hcatevtdisplay.h

LIBL	= $(LIBNAME)LinkDef.h
LIBS	= $(LIBH:.h=.cc)
LIBO	= $(LIBS:.cc=.o)
LIBDS	= $(LIBNAME)Cint.cc
LIBDH	= $(LIBDS:.cc=.h)
LIBDO	= $(LIBDS:.cc=.o)
LIBDEP	= $(LIBS:.cc=.d)
########################################


obj: $(LIBO) $(LIBDO) cpobj 

cpobj:
	@cp $(LIBO) $(LIBDO) $(TEMPBASE)

clean:
	@rm -f $(LIBDEP) $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO)  core *~
	@echo Cleaning done for $(LIBNAME) directory.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:	%.cc 
	@echo $(CXX) -c $<
	@$(CXX) $(CXXFLAGS)  -c $<	

%.d:	 %.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'





















