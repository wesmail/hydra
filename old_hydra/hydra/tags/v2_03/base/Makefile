include ../Makefile.base
UNAME=$(shell uname)
##########################################
# Rpc def objects
##########################################
RPC_OBJ       =   server_clnt.o \
                              server_xdr.o


##########################################
#Configuration area
##########################################
LIBNAME	= Hydra
LIBH	= hbranchobject.h hcategory.h hdataobjid.h hdataobject.h \
	  hdatasource.h hdebug.h hevent.h heventheader.h hfilter.h \
	  hgeantreader.h hgeantsource.h hiterator.h \
	  hlinearcatiter.h hlinearcategory.h hlocateddataobject.h\
	  hlocation.h hmatrixcatiter.h hmatrixcategory.h hnode.h \
	  hpartialevent.h hrecevent.h hrecobject.h \
	  hreconstructor.h hrootsource.h hindextable.h\
	  htrack.h htree.h hades.h hldbase.h \
	  hldsource.h hldfilesource.h \
	  hldevt.h hldsubevt.h hldunpack.h hclonestable.h \
	  hmatrixcategorysplit.h hlineparser.h \
	  hruntimedb.h hparset.h hratree.h hcategorysplit.h \
	  hcategorysplititer.h htask.h htaskset.h heventfile.h \
	  hdetector.h hspectrometer.h hldfilevt.h \
          hpario.h hdetpario.h hparfileio.h hdetparfileio.h \
          hparascifileio.h hldremotevt.h hldremotesource.h \
          hadioascii.h hratreeext.h hgeantheader.h hgeantkine.h


LIBL	= $(LIBNAME)LinkDef.h
LIBS	= $(LIBH:.h=.cc)
LIBO	= $(LIBS:.cc=.o)
LIBDS	= $(LIBNAME)Cint.cc
LIBDH	= $(LIBDS:.cc=.h)
LIBDO	= $(LIBDS:.cc=.o)
LIBLIB	= lib$(LIBNAME).$(SOEXT)
LIBDEP	= $(LIBS:.cc=.d)
########################################


shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) $(RPC_OBJ) core *~

$(LIBLIB): $(LIBO) $(LIBDO) $(RPC_OBJ)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO)  $(RPC_OBJ)  -o $(LIBLIB)

	@mv $(LIBLIB) ../lib
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(LIBH) $(LIBL)


include $(LIBDEP)
#########################################

%.o:	%.cc 
	@echo $(CXX) -c $<
	@$(CXX) $(CXXFLAGS)  -c $<	

ifeq ($(UNAME),Linux)
server_clnt.o:   linux_server_clnt.cc
	g++ -c linux_server_clnt.cc -o $@
server_xdr.o:   linux_server_xdr.cc
	g++ -c linux_server_xdr.cc -o $@
else
server_clnt.o:   osf_server_clnt.c
	gcc -c osf_server_clnt.c -o $@
server_xdr.o:   osf_server_xdr.c
	gcc -c osf_server_xdr.c -o $@
endif


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





