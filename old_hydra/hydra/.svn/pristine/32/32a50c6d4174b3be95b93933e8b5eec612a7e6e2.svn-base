include ../../Makefile.base
##########################################
#Configuration area
##########################################
LIBNAME	= Geometry
LIBH	= hdetgeompar.h hgeombasicshape.h hgeombrik.h hgeomcompositevolume.h \
hgeomrotation.h hgeomshapes.h hgeomshapes.h hgeomtransform.h hgeomtrap.h \
hgeomtrd1.h hgeomvector.h hgeomvolume.h

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
	@cp $(LIBO) $(LIBDO) $(TEMP)/base

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





















