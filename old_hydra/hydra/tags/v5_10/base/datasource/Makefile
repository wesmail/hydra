include ../../Makefile.base
UNAME=$(shell uname)
##########################################
# Rpc def objects
##########################################
RPC_OBJ       =   server_clnt.o server_xdr.o

##########################################
#Configuration area
##########################################
LIBNAME	= DataSource
LIBH	= hdatasource.h hgeantkine.h hgeantreader.h hgeantsource.h \
          hldbase.h hldevt.h hldfilesource.h hldfilevt.h hldsource.h \
          hldsubevt.h hldunpack.h hrootsource.h hkinegeantreader.h \
          hsimulgeantreader.h hdirectsource.h \
          hldremotesource.h hldremotevt.h hkinesim.h

LIBL	= $(LIBNAME)LinkDef.h
LIBS	= $(LIBH:.h=.cc)
LIBO	= $(LIBS:.cc=.o)
LIBDS	= $(LIBNAME)Cint.cc
LIBDH	= $(LIBDS:.cc=.h)
LIBDO	= $(LIBDS:.cc=.o)
LIBDEP	= $(LIBS:.cc=.d)
########################################


obj: $(LIBO) $(LIBDO) $(RPC_OBJ) cpobj 

cpobj:
	@cp $(LIBO) $(LIBDO) $(RPC_OBJ) $(TEMPBASE)

clean:
	@rm -f $(LIBDEP) $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(RPC_OBJ)  core *~
	@echo Cleaning done for $(LIBNAME) dir.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS)  -c $(CINTINC) $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:	%.cc 
	@echo $(CXX) $(CXXFLAGS) -c $<
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

%.d:	 %.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'





















