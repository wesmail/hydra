MAIN=show_mu
NOLIBNEW=ON

include $(HADDIR_BASEDIR)/Makefile.base

CXXFLAGS += -I$(ADSM_BASE)/Linux

#LORALIB	= -L$(ORACLE_HOME)/lib/ -lclntsh

#LORALIB	= -L$(ORACLE_HOME)/lib/ -lclntsh \
#	  -L$(ORACLE_HOME)/lib/ -lwtc9

#LIBS+=$(ROOTLIBS) -lProof -lTreePlayer -lMinuit 
LIBS+=$(ROOTLIBS) -lTreePlayer -lMinuit 
LIBS+=$(ROOTGLIBS) 
LIBS+=$(LORALIB)

RPATH=`echo "$(ROOTSYS)/lib:"| sed -e 's/ //g'`

all:  $(MAIN).cc
	$(CXX) $(MAIN).cc $(CXXFLAGS) -c -o $(MAIN).o
	$(LD) $(MAIN).o -Wl,-rpath,$(RPATH)\
	-L.  \
	-L$(ROOTSYS)/lib $(LIBS)  \
	-o $(MAIN)


clean: 
	@if [ -e *.o ]; then  echo removing \*.o and exec files; rm *.o; rm $(MAIN); else echo 'nothing to do, no *.o files there' ; fi  

test: 
	@echo $(LIBS)
	@echo $(RPATH)
