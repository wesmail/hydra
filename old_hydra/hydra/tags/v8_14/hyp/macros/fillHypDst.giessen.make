
APP_NAME := fillHypDst
SOURCE_FILES := fillHypDst.cc

USES_RFIO    := no
USES_CERNLIB := no
USES_ORACLE  := no


include $(HADDIR)/hades.def.mk

# override default list of linked Hydra libraries - before they can act on the rules
HYDRA_LIBS := \
        -lHyp \
        -lRich \
        -lRichUtil \
        -lMdcTrackD \
        -lMdc \
        -lMdcUtil \
        -lMdcTrackS \
        -lMdcTrackG \
        -lKick \
        -lShower \
        -lShowerUtil \
        -lShowerTofino \
        -lStart \
        -lTof \
        -lTofino \
        -lTrigger \
        -lTriggerUtil \
        -lPid \
        -lPidUtil \
        -lPairs \
        -lHydra


include $(HADDIR)/hades.app.mk
