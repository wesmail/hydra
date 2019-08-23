ARCH         := $(shell root-config --arch)
PLATFORM      = $(ARCH)

OutPutOpt     = -o # keep whitespace after "-o"

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)

CXX           = g++
CXXFLAGS      = -O3 -Wall -g -fPIC
CPPFLAGS      = -DNDEBUG
LD            = g++
LDFLAGS       = -O3
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

#------------------------------------------------------------------------------

all: readPairsE2 readPairsS

clean:
	@rm -f *.o core

readPairsE2: readPairsE2.o pairsE.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

readPairsS: readPairsS.o pairsS.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"


#------------------------------------------------------------------------------

readPairsE2.o: readPairs.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
pairsE.o: pairs.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
readPairsS.o: readPairs.cc
	$(CXX) $(CPPFLAGS) -DSIMULATION $(CXXFLAGS) -c $< -o $@
pairsS.o: pairs.cc
	$(CXX) $(CPPFLAGS) -DSIMULATION $(CXXFLAGS) -c $< -o $@

