##############################################################################
#
#  Makefile for Hydra library libDst.so
#
#  This makefile contains all definitions local to this module. All
#  general definitions are included from makefiles named "hydra.*.mk".
#
##############################################################################


LIB_NAME := Dst


SOURCE_FILES :=	hdstproduction.cc	\
		hdstrealdata.cc		\
		hdstsimulation.cc	\
		hdstembedding.cc	\
		hdstrealdataapr06.cc	\
		hdstrealdatasep05.cc	\
		hdstsimulationsep05.cc	\
		hdstembeddingsep05.cc   \
		hdstutilities.cc


include $(HADDIR)/hades.def.mk

# set this, while debugging
SO_CXX_FLAGS += -O0

include $(HADDIR)/hades.module.mk
