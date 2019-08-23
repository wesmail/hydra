include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
##########################################
LIBNAME	= DataStruct
LIBH	= hcategory.h hcategorysplit.h hcategorysplititer.h hclonestable.h \
hdataobject.h hdataobjid.h hfilter.h hindextable.h  hiterator.h \
hlinearcategory.h hlinearcatiter.h hlineparser.h hlocateddataobject.h \
hlocation.h  hmatrixcategory.h hmatrixcategorysplit.h hmatrixcatiter.h \
hnode.h hratree.h hratreeext.h  hrecobject.h htrack.h hlinkeddataobject.h \
 hcompositecategory.h hordclonesarray.h \
hordcategory.h hordcatiter.h hclonesarray.h hbarray.h hbitfieldcategory.h 

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
	@rm -rf $(LIBDEP) $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO)   core *~
	@echo Cleaning done for $(LIBNAME) dir.

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





















