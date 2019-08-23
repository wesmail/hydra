APP_NAME     := fillHypDst
SOURCE_FILES := fillHypDst.cc
USES_RFIO    := no
USES_CERNLIB := yes
USES_ORACLE  := yes


include $(HADDIR)/hades.def.mk

APP_CXX_FLAGS += -DUSEORA

# override default list of linked Hydra libraries - before they can act on the rules
# HYDRA_LIBS += -lHyp 

HYDRA_LIBS  = -lPid -lPidUtil -lPairs
HYDRA_LIBS += -lMdcTrackG -lMdcTrackD -lMdcTrackS -lKick
HYDRA_LIBS += -lWall -lShowerUtil -lShowerTofino -lShower -lTofino -lTof
HYDRA_LIBS += -lMdc -lRichUtil -lRich
HYDRA_LIBS += -lTriggerUtil -lTrigger -lStart
HYDRA_LIBS += -lOra -lQA -lTools -lRFIOtsm -lHydra -lHyp


include $(HADDIR)/hades.app.mk