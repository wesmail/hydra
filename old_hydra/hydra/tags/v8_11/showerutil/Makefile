##############################################################################
#
#  Makefile for Hydra library libShowerUtil.so
#
#  This makefile contains all definitions local to this module. All
#  general definitions are included from makefiles in $(HADDIR).
#
##############################################################################

LIB_NAME := ShowerUtil

SOURCE_FILES :=	hsuprogress.cc		\
		hsuprogressrec.cc	\
		hsuhldinfo.cc		\
		hsuexemacro.cc		\
		hsudummyrec.cc		\
		hsuerrorhandler.cc	\
		hsupaddivider.cc	\
		hsuchain.cc
# removed from Makefile, but not from CVS
#		hsuhldfileinfo.cc

# Uncomment this block to compile with GUI support
# (do the same in ShowerUtilLinkDef.h)
#
#SOURCE_FILES += hsuframe.cc		\
#		hsuactivewidget.cc	\
#		hsueditboxes.cc		\
#		hsulistboxes.cc		\
#		hsudialog.cc		\
#		hsudialogframe.cc	\
#		hsubuttons.cc

include $(HADDIR)/hades.def.mk
include $(HADDIR)/hades.module.mk
