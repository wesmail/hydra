##############################################################################
#
#  Makefile for Hydra library libHydra.so and libHydra.a
#
#  This makefile contains all definitions local to this module. All
#  general definitions are included from makefiles named "hades.*.mk".
#
##############################################################################

LIB_NAME := Hydra

USES_RFIO ?= yes

SOURCE_FILES :=	datasource/hdatasource.cc		\
		datasource/hgeantreader.cc		\
		datasource/hgeantsource.cc		\
		datasource/hldbase.cc			\
		datasource/hldevt.cc			\
		datasource/hldfilevt.cc			\
		datasource/hldsource.cc			\
		datasource/hldfilesource.cc		\
		datasource/hldsubevt.cc			\
		datasource/hldfileoutput.cc		\
		datasource/hldunpack.cc			\
		datasource/hrootsource.cc		\
		datasource/hkinegeantreader.cc		\
		datasource/hsimulgeantreader.cc		\
		datasource/hdirectsource.cc		\
		datasource/hldfilesourcebase.cc		\
		datasource/hldremotesource.cc		\
		datasource/hldremotevt.cc		\
		datasource/hldgrepfilesource.cc		\
		datasource/htrbbaseunpacker.cc		\
		datasource/htrblookup.cc

SOURCE_FILES +=	datastruct/hcategory.cc			\
		datastruct/hclonestable.cc		\
		datastruct/hdataobject.cc		\
		datastruct/hdataobjid.cc		\
		datastruct/hfilter.cc			\
		datastruct/hindextable.cc		\
		datastruct/hiterator.cc			\
		datastruct/hlinearcategory.cc		\
		datastruct/hlinearcatiter.cc		\
		datastruct/hlineparser.cc		\
		datastruct/hlocateddataobject.cc	\
		datastruct/hlocation.cc			\
		datastruct/hmatrixcategory.cc		\
		datastruct/hmatrixcatiter.cc		\
		datastruct/hnode.cc			\
		datastruct/hratree.cc			\
		datastruct/hratreeext.cc		\
		datastruct/hrecobject.cc		\
		datastruct/htrack.cc			\
		datastruct/hlinkeddataobject.cc		\
		datastruct/hcompositecategory.cc	\
		datastruct/hordclonesarray.cc		\
		datastruct/hordcategory.cc		\
		datastruct/hordcatiter.cc		\
		datastruct/hclonesarray.cc		\
		datastruct/hbarray.cc			\
		datastruct/hbitfieldcategory.cc
# removed from Makefile, but not from CVS
#		datastruct/hcategorysplit.cc		\
#		datastruct/hcategorysplititer.cc	\
#		datastruct/hmatrixcategorysplit.cc

SOURCE_FILES +=	eventhandling/hades.cc			\
		eventhandling/hdetector.cc		\
		eventhandling/hevent.cc			\
		eventhandling/heventheader.cc		\
		eventhandling/hpartialevent.cc		\
		eventhandling/hrecevent.cc		\
		eventhandling/hreconstructor.cc		\
		eventhandling/hspectrometer.cc		\
		eventhandling/htask.cc			\
		eventhandling/htaskset.cc		\
		eventhandling/htree.cc
# removed from Makefile, but not from CVS
#		eventhandling/htaskseqlist.cc

SOURCE_FILES +=	geantutil/hgeantheader.cc		\
		geantutil/hgeantkine.cc			\
		geantutil/hgeantmdc.cc			\
		geantutil/hgeantrich.cc			\
		geantutil/hgeanttof.cc			\
		geantutil/hgeantshower.cc		\
		geantutil/hgeantwall.cc			\
		geantutil/hkinesim.cc			\
		geantutil/hgeantrpc.cc

SOURCE_FILES +=	geometry/hgeomtransform.cc		\
		geometry/hgeomvector.cc			\
		geometry/hgeomrotation.cc		\
		geometry/hgeomvolume.cc			\
		geometry/hgeomcompositevolume.cc	\
		geometry/hspecgeompar.cc		\
		geometry/hdetgeompar.cc			\
		geometry/hgeomshapes.cc			\
		geometry/hgeombasicshape.cc		\
		geometry/hgeombrik.cc			\
		geometry/hgeomtrap.cc			\
		geometry/hgeomtrd1.cc			\
		geometry/hgeompgon.cc			\
		geometry/hgeompcon.cc			\
		geometry/hgeomtube.cc			\
		geometry/hgeomtubs.cc			\
		geometry/hgeomcone.cc			\
		geometry/hgeomcons.cc			\
		geometry/hgeomsphe.cc			\
		geometry/hgeomeltu.cc			\
		geometry/hgeomvertexfit.cc		\
		geometry/hgeommatrix.cc			\
		geometry/hgeomvertexfit2.cc		\
		geometry/hgeomvertexfit3.cc

SOURCE_FILES +=	runtimedb/hcontfact.cc			\
		runtimedb/hbasecontfact.cc		\
		runtimedb/hruntimedb.cc			\
		runtimedb/hparset.cc			\
		runtimedb/hparcond.cc			\
		runtimedb/hparamlist.cc			\
		runtimedb/hscspar.cc			\
		runtimedb/hscschannel.cc		\
		runtimedb/hrun.cc			\
		runtimedb/hpario.cc			\
		runtimedb/hdetpario.cc			\
		runtimedb/hparrootfileio.cc		\
		runtimedb/hdetparrootfileio.cc		\
		runtimedb/hcondparrootfileio.cc		\
		runtimedb/hspecparrootfileio.cc		\
		runtimedb/hparasciifileio.cc		\
		runtimedb/hdetparasciifileio.cc		\
		runtimedb/hcondparasciifileio.cc	\
		runtimedb/hspecparasciifileio.cc	\
		runtimedb/hadioascii.cc			\
		runtimedb/hadascii.cc			\
		runtimedb/hdetparhadasciifileio.cc	\
		runtimedb/hparhadasciifileio.cc		\
		runtimedb/hparamfilegenerator.cc	\
		runtimedb/hruninfo.cc			\
		runtimedb/hmagnetpar.cc

SOURCE_FILES +=	util/hdebug.cc				\
		util/hobjtable.cc			\
		util/hrunanalizer.cc			\
		util/hsymmat.cc				\
		util/hmessagemgr.cc			\
		util/hntuple.cc	   		        \
		util/hntuplemap.cc    		        \
		util/hpidphysicsconstants.cc		\
		util/htool.cc				\
		util/hgeantfilter.cc

# addtional source files related to RFIO
ifeq ($(strip $(USES_RFIO)),yes)
	SOURCE_FILES +=	datasource/hrfiorootsource.cc	\
			datasource/hldrfiofilesource.cc	\
			datasource/hldrfiofilevt.cc	\
			datasource/hrfiofile.cc
endif

# additional source files needed on a Linux system, only
SOURCE_FILES +=	datasource/linux_server_clnt.cc	\
		datasource/linux_server_xdr.cc

# additional source files needed on non Linux systems, only
#SOURCE_FILES += datasource/osf_server_clnt.c	\
#		datasource/osf_server_xdr.c

# directories removed from Makefile, but not from CVS
#		correlation
#		evtserver

include $(HADDIR)/hades.def.mk
include $(HADDIR)/hades.module.mk
