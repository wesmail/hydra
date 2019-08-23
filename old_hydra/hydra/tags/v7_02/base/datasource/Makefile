include $(HADDIR_BASEDIR)/Makefile.base

UNAME=$(shell uname)
##########################################
# Rpc def objects
##########################################
RPC_OBJ       =   server_clnt.o server_xdr.o

##########################################
#Configuration area
##########################################
LIBNAME	= DataSource

LIBH	= hdatasource.h hgeantreader.h hgeantsource.h \
	  hldbase.h hldevt.h hldfilevt.h hldsource.h \
	  hldfilesource.h hldsubevt.h hldfileoutput.h hldunpack.h \
	  hrootsource.h hkinegeantreader.h hsimulgeantreader.h \
	  hdirectsource.h hldremotesource.h hldremotevt.h

ifdef RFIO_SUPPORT
LIBH     += hrfiorootsource.h hldfilesource.h hldfilevt.h
ADSM_INC = $(ADSM_BASE)/inc
RFIO_INC = -I$(ADSM_INC) -I$(ROOTSYS)/rfio/inc 
CXXFLAGS += $(RFIO_INC)
CINTINC  += $(RFIO_INC) 
CINTFLAGS+=-DRFIO_SUPPORT
endif


LIBL	= $(LIBNAME)LinkDef.h
LIBS	= $(LIBH:.h=.cc)
LIBO	= $(LIBS:.cc=.o)
LIBDS	= $(LIBNAME)Cint.cc
LIBDH	= $(LIBDS:.cc=.h)
LIBDO	= $(LIBDS:.cc=.o)
LIBDEP	= $(LIBS:.cc=.d)
########################################


obj: cpobj 

cpobj:  $(LIBO) $(LIBDO) $(RPC_OBJ)
	@cp $(LIBO) $(LIBDO) $(RPC_OBJ) $(TEMPBASE)

clean:
	@rm -f $(LIBDEP) $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(RPC_OBJ)  core *~
	@echo Cleaning done for $(LIBNAME) dir.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	@rootcint  -f $(LIBDS)  -c -p -e $(CINTINC) $(CINTFLAGS) $(LIBH) $(LIBL) 

include $(LIBDEP)
#########################################
%.o:	%.cc 
	@echo $(CXX) -c $<
	@$(CXX) $(CXXFLAGS)  -c $<	

ifeq ($(UNAME),Linux)
server_clnt.o:   linux_server_clnt.cc
	$(CXX) -c linux_server_clnt.cc -o $@
server_xdr.o:   linux_server_xdr.cc
	$(CXX) -c linux_server_xdr.cc -o $@
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


test:
	@echo CXX $(CXXFLAGS)


















