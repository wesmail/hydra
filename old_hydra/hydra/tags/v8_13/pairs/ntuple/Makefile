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

all: readPairsE readPairsEnov02 readPairsEhyd800 readPairsS readPairsSnov02 readPairsShyd800 

show: showPairsE showPairsEnov02 showPairsEhyd800  showPairsS showPairsSnov02 showPairsShyd800

clean:
	@rm -f *.o core

showPairsS: showPairsS.o pairsS.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

showPairsSnov02: showPairsSnov02.o pairsSnov02.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

showPairsShyd800: showPairsShyd800.o pairsShyd800.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

showPairsE: showPairsE.o pairsE.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

showPairsEnov02: showPairsEnov02.o pairsEnov02.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

showPairsEhyd800: showPairsEhyd800.o pairsEhyd800.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

readPairsE: readPairsE.o pairsE.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

readPairsEnov02: readPairsEnov02.o pairsEnov02.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

readPairsEhyd800: readPairsEhyd800.o pairsEhyd800.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

readPairsS: readPairsS.o pairsS.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

readPairsSnov02: readPairsSnov02.o pairsSnov02.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"

readPairsShyd800: readPairsShyd800.o pairsShyd800.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -o $@
	@echo "$@ done"


#------------------------------------------------------------------------------

showPairsE.o: showPairs.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
showPairsEnov02.o: showPairs.cc
	$(CXX) $(CPPFLAGS) -DNOV02 -DFORMATBEFORE_AUG06 $(CXXFLAGS) -c $< -o $@
showPairsEhyd800.o: showPairs.cc
	$(CXX) $(CPPFLAGS) -DFORMAT_HYDRA800 $(CXXFLAGS) -c $< -o $@
showPairsS.o: showPairs.cc
	$(CXX) $(CPPFLAGS) -DSIMULATION $(CXXFLAGS) -c $< -o $@
showPairsSnov02.o: showPairs.cc
	$(CXX) $(CPPFLAGS) -DSIMULATION -DNOV02 -DFORMATBEFORE_AUG06 $(CXXFLAGS) -c $< -o $@
showPairsShyd800.o: showPairs.cc
	$(CXX) $(CPPFLAGS) -DSIMULATION -DFORMAT_HYDRA800 $(CXXFLAGS) -c $< -o $@
####### HISTOGRAM FILLING
readPairsE.o: readPairs.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
pairsE.o: pairs.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
readPairsEnov02.o: readPairs.cc
	$(CXX) $(CPPFLAGS) -DNOV02 -DFORMATBEFORE_AUG06 $(CXXFLAGS) -c $< -o $@
pairsEnov02.o: pairs.cc
	$(CXX) $(CPPFLAGS) -DNOV02 -DFORMATBEFORE_AUG06 $(CXXFLAGS) -c $< -o $@
readPairsEhyd800.o: readPairs.cc
	$(CXX) $(CPPFLAGS) -DFORMAT_HYDRA800 $(CXXFLAGS) -c $< -o $@
pairsEhyd800.o: pairs.cc
	$(CXX) $(CPPFLAGS) -DFORMAT_HYDRA800 $(CXXFLAGS) -c $< -o $@
####### SIMULATION
readPairsS.o: readPairs.cc
	$(CXX) $(CPPFLAGS) -DSIMULATION $(CXXFLAGS) -c $< -o $@
pairsS.o: pairs.cc
	$(CXX) $(CPPFLAGS) -DSIMULATION $(CXXFLAGS) -c $< -o $@
readPairsSnov02.o: readPairs.cc
	$(CXX) $(CPPFLAGS) -DNOV02 -DFORMATBEFORE_AUG06 -DSIMULATION $(CXXFLAGS) -c $< -o $@
pairsSnov02.o: pairs.cc
	$(CXX) $(CPPFLAGS) -DNOV02 -DFORMATBEFORE_AUG06 -DSIMULATION $(CXXFLAGS) -c $< -o $@
readPairsShyd800.o: readPairs.cc
	$(CXX) $(CPPFLAGS) -DFORMAT_HYDRA800 -DSIMULATION $(CXXFLAGS) -c $< -o $@
pairsShyd800.o: pairs.cc
	$(CXX) $(CPPFLAGS) -DFORMAT_HYDRA800 -DSIMULATION $(CXXFLAGS) -c $< -o $@

