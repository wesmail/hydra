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

HYDRALIBS 	  = -L$(HADDIR_PID_LIB) -lPid \
    			-L$(HADDIR_PIDUTIL_LIB) -lPidUtil \
				-L$(HADDIR_PAIRS_LIB) -lPairs \
				-L$(HADDIR_RICH_LIB) -lRich \
				-L$(HADDIR_RICHUTIL_LIB) -lRichUtil \
				-L$(HADDIR_MDCTRACKD_LIB) -lMdcTrackD \
				-L$(HADDIR_MDC_LIB) -lMdc \
				-L$(HADDIR_MDCUTIL_LIB) -lMdcUtil \
				-L$(HADDIR_MDCTRACKS_LIB) -lMdcTrackS \
				-L$(HADDIR_MDCTRACKG_LIB) -lMdcTrackG \
				-L$(ROOTSYS)/lib -lProof -lTreePlayer -lMinuit \
				-L$(HADDIR_KICKPLANE_LIB) -lKick \
				-L$(HADDIR_SHOWER_LIB) -lShower \
				-L$(HADDIR_SHOWERUTIL_LIB) -lShowerUtil \
				-L$(HADDIR_SHOWERTOFINO_LIB) -lShowerTofino \
				-L$(HADDIR_START_LIB) -lStart \
				-L$(HADDIR_TOF_LIB) -lTof \
				-L$(HADDIR_TOFINO_LIB) -lTofino \
				-L$(HADDIR_ORA_LIB) -lOra \
				-L$(HADDIR_QA_LIB) -lQA \
				-L$(HADDIR_WALL_LIB) -lWall \
				-L$(HADDIR_TRIGGER_LIB) -lTrigger \
				-L$(HADDIR_TRIGGERUTIL_LIB) -lTriggerUtil \
				-L$(HADDIR_PHYANA_LIB) -lPhyAna \
				-L$(HADDIR_HYP_LIB) -lHyp \
				-L$(HADDIR_BASE_LIB) -lHydra 

HYDRAINC =-I$(HADDIR_BASE)/datasource \
			-I$(HADDIR_BASE)/datastruct \
			-I$(HADDIR_BASE)/eventhandling \
			-I$(HADDIR_BASE)/geometry  \
			-I$(HADDIR_BASE)/runtimedb \
			-I$(HADDIR_BASE)/util \
			-I$(HADDIR_BASE)/correlation \
			-I$(HADDIR_BASE)/geantutil \
			-I$(HADDIR_SIMULATION) \
			-I$(HADDIR_KICKPLANE) \
			-I$(HADDIR_MDCTRACKD) \
			-I$(HADDIR_MDCTRACKG) \
			-I$(HADDIR_MDCTRACKS) \
			-I$(HADDIR_MDCGARFIELD) \
			-I$(HADDIR_MDCPID) \
			-I$(HADDIR_MDCUTIL) \
			-I$(HADDIR_MDC) \
			-I$(HADDIR_PHYANA) \
			-I$(HADDIR_RICHUTIL) \
			-I$(HADDIR_RICH) \
			-I$(HADDIR_SHOWERTOFINO) \
			-I$(HADDIR_SHOWERUTIL) \
			-I$(HADDIR_SHOWER) \
			-I$(HADDIR_START) \
			-I$(HADDIR_TOFINO) \
			-I$(HADDIR_TOF) \
			-I$(HADDIR_WALL) \
			-I$(HADDIR_TRIGGER) \
			-I$(HADDIR_QA) \
			-I$(HADDIR_TRIGGERUTIL) \
			-I$(HADDIR_PID) \
			-I$(HADDIR_PIDUTIL) \
			-I$(HADDIR_ALIGNMENT) \
			-I$(HADDIR_TOOLS) \
			-I$(HADDIR_PAIRS) \
			-I$(HADDIR_HYP) \
			-I$(HADDIR_ORA) \
			-I$(HADDIR_ORAUTIL) \
			-I$(HADDIR_ORASIM) 

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

E		= anaProjectPidNtuple(ExeSuf)

OBJS          = $(O1) $(O2)

PROGRAMS      = $(E)

#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

clean:		
		@rm -f $(OBJS) core

$(E):      	$(O1) $(O2) 
		$(LD) $(LDFLAGS) $^ $(LIBS) $(HYDRALIBS) $(OutPutOpt)$@
		@echo "$@ done"


.SUFFIXES: .$(SrcSuf)

#------------------------------------------------------------------------------

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<
