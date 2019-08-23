include $(HADDIR_BASEDIR)/Makefile.base

ALL_LIBS_0=$(HYDRA_MODULES)
ALL_LIBS_1=$(ALL_LIBS_0:revtlib=)
ALL_LIBS_2=$(ALL_LIBS_1:basedirlib=)
ALL_LIBS_3=$(ALL_LIBS_2:evtserverlib=)
ALL_LIBS_4=$(ALL_LIBS_3:oralib=)
ALL_LIBS=$(ALL_LIBS_4:orautillib=)

ifdef ONLINEHYDRA
ALL_LIBS += evtserverlib
endif
ifdef ORACLE_HOME
ALL_LIBS += oralib orautillib
endif

ALL_LIBS_CLEAN = $(ALL_LIBS:lib=clean)
ALL_LIBS_DIR = $(ALL_LIBS:lib=)

all: $(ALL_LIBS)
	@echo ' '
	@echo 'Hydra libs successfully created'
	@echo ' '
	@echo 'Root used:  $(ROOTSYS)' > VERSIONS
	@echo 'Hydra used: $(HADDIR)' >> VERSIONS
	@echo 'Compiler used: ' >> VERSIONS
	@gcc -v 2>&1 | cat - >> VERSIONS 


%lib:
	@cd $* && $(MAKE) shlib

clean: $(ALL_LIBS_CLEAN)
	@echo removing shared object libraries...
	@if [ -w .make.tmp ] ; \
	then \
		rm .make.tmp ; \
	fi ; \
		for item in ${ALL_LIBS_DIR} ; \
			do  \
				myitem=`echo $${item} | tr '[a-z]' '[A-Z]' ` ; \
				echo "if [ -w \$$HADDIR_$${myitem}_LIB ]" >> .make.tmp ; \
				echo "then" >> .make.tmp ; \
				echo "	rm -rf \$$HADDIR_$${myitem}_LIB/*.so" >> .make.tmp ; \
				echo "	rm -rf \$$HADDIR_$${myitem}_LIB/active/*.so" >> .make.tmp ; \
				echo "	echo removing .so of $${myitem}" >> .make.tmp ; \
				echo "else" >> .make.tmp ; \
				echo "	echo \$$HADDIR_$${myitem}_LIB/ cannot be cleaned by user ${USER}" >> .make.tmp ; \
				echo "fi" >> .make.tmp ; \
				echo "if [ -w \$$HADDIR_$${myitem} ]" >> .make.tmp ; \
				echo "then" >> .make.tmp ; \
				echo " cd \$$HADDIR_$${myitem} && $(MAKE) clean" >> .make.tmp ;\
				echo "else" >> .make.tmp ; \
				echo "	echo \$$HADDIR_$${myitem}/ cannot be cleaned by user ${USER}" >> .make.tmp ; \
				echo "fi" >> .make.tmp ; \
			done ; \
	chmod u+x .make.tmp && .make.tmp && rm .make.tmp

%clean:
	@cd $* && $(MAKE) clean

docs : macros/makeDocs.C $(ALL_LIBS) 
	@echo Generating the documentation...
	@root -q -l makeDocs.C
alldocs : makedoc.C $(SRCS) $(HDRS) $(SOLIB)
	@echo Generating the documentation...
	@root -q -l makealldoc.C


test:
	@echo $(ALL_LIBS)









