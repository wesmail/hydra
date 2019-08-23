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

O		= readPairs.$(ObjSuf)
S		= readPairs.$(SrcSuf)
E		= readPairs(ExeSuf)

OBJS          = $(O)

PROGRAMS      = $(E)

#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

clean:		
		@rm -f $(OBJS) core

$(E):      	$(O)  pairsS.o
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		@echo "$@ done"


.SUFFIXES: .$(SrcSuf)

#------------------------------------------------------------------------------

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
