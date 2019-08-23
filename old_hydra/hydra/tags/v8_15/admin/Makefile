###############################################################################
#
#  The global Makefile to build all Hydra modules from scratch.
#
#  Examples and explanations can be found in the file README
#
###############################################################################

# no external dependencies for a total build
export HADDIR   := $(shell pwd)
MYHADDIR        :=
INSTALL_DIR     ?= /misc/hadessoftware/install/hydra-dev
USES_RFIO       ?= yes

MODULES ?=	alignment	\
		base		\
		dst		\
		evtserver	\
		hadesgo4	\
		hodo		\
		hyp		\
		kickplane	\
		mdc		\
		mdcgarfield	\
		mdcpid		\
		mdctrackD	\
		mdctrackG	\
		mdctrackS	\
		mdcutil		\
		online		\
		ora		\
		orasim		\
		orautil		\
		pairs		\
		phyana		\
		pid		\
		pidutil		\
		qa		\
		rich		\
		richutil	\
		rpc		\
		shower		\
		showertofino	\
		showerutil	\
		simulation	\
		start		\
		tof		\
		tofino		\
		tofutil		\
		tools		\
		trigger		\
		triggerutil	\
		wall

ifeq ($(strip $(USES_RFIO)),yes)
   MODULES += rfio
endif

include $(HADDIR)/hades.def.mk
include $(HADDIR)/hades.rules.mk
