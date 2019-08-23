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

CXX           = g++
CXXFLAGS      = -O -Wall -g -fPIC
LD            = g++
LDFLAGS       = -O
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
LIBS          = $(ROOTLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

#------------------------------------------------------------------------------

O1		= projectPidHisto.$(ObjSuf)
S1		= projectPidHisto.$(SrcSuf)
O2		= anaProjectPidHisto.$(ObjSuf)
S2		= anaProjectPidHisto.$(SrcSuf)

E		= anaProjectPidHisto(ExeSuf)

OBJS          = $(O1) $(O2)

PROGRAMS      = $(E)

#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

clean:		
		@rm -f $(OBJS) core

$(E):      	$(O1)  $(O2)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		@echo "$@ done"


.SUFFIXES: .$(SrcSuf)

#------------------------------------------------------------------------------

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
