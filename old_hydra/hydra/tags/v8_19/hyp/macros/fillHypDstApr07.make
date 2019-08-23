
APP_NAME := fillHypDstApr07
SOURCE_FILES := fillHypDstApr07.cc

USES_RFIO    := no
USES_CERNLIB := no
USES_ORACLE  := yes

CXXFLAGS += -DUSEORA

include $(HADDIR)/hades.def.mk

CXXFLAGS += -DUSEORA

# override default list of linked Hydra libraries - before they can act on the rules
HYDRA_LIBS += -lHyp

.PHONY:  default
default: build install

include $(HADDIR)/hades.app.mk
