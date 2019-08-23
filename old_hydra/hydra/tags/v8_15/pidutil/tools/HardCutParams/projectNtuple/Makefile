APP_NAME     := projectProgram
SOURCE_FILES := projectProgram.cc
USES_RFIO    := no
USES_CERNLIB := no
USES_ORACLE  := yes

include $(HADDIR)/hades.def.mk

# override default list of linked Hydra libraries - before they can act on the rules
HYDRA_LIBS := 	-lPairs \
				-lPid \
				-lPidUtil \
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
				-lOra \
				-lQA \
				-lWall \
				-lTrigger \
				-lTriggerUtil \
				-lPhyAna \
				-lHyp \
				-lHydra 

include $(HADDIR)/hades.app.mk
