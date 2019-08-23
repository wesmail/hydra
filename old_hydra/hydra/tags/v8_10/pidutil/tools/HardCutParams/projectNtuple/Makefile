ARCH         := $(shell root-config --arch)
PLATFORM      = $(ARCH)

ObjSuf        = o
SrcSuf        = cc
ExeSuf        =
DllSuf        = so
OutPutOpt     = -o # keep whitespace after "-o"

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)


HYDRALIBS 	  = -L$(HADDIR)/lib -lPid -lPidUtil -lPairs \
				-lShower \
				-lRFIOtsm \
				-lRich \
				-lRichUtil \
				-lMdcTrackD \
				-lMdc \
				-lMdcUtil \
				-lMdcTrackS \
				-lMdcTrackG \
				-lKick \
				-lShowerUtil \
				-lShowerTofino \
				-lStart \
				-lTof \
				-lTofino \
				-lOra \
				-lQA \
				-lWall \
				-lTrigger \
				-lTriggerUtil \
				-lPhyAna \
				-lHyp \
				-lHydra 

HYDRAINC = -I$(HADDIR)/include 

CXX           = g++
CXXFLAGS      = -O -Wall -g -fPIC $(HYDRAINC)
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

#------------------------------------------------------------------------------

O1		= projectPidNtuple.$(ObjSuf)
S1		= projectPidNtuple.$(SrcSuf)

O2		= anaProjectPidNtuple.$(ObjSuf)
S2		= anaProjectPidNtuple.$(SrcSuf)

E		= anaProjectPidNtuple

OBJS          = $(O1) $(O2)

PROGRAMS      = $(E)

#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

clean:		
		@rm -f $(OBJS) core

$(E):      	$(O1) $(O2) 
		$(LD) $(LDFLAGS) $^ $(HYDRALIBS) $(LIBS) $(GLIBS) $(OutPutOpt)$@
		@echo "$@ done"


.SUFFIXES: .$(SrcSuf)

#------------------------------------------------------------------------------

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
