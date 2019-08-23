include Makefile.base


all: baselib toflib mdclib oralib startlib showerlib richlib

%lib:
	@cd $* && $(MAKE) shlib

clean: baseclean tofclean mdcclean oraclean startclean showerclean richclean
	@rm -f lib/*.so

%clean:
	@cd $* && $(MAKE) clean


docs : macros/makeDocs.C baselib toflib mdclib oralib startlib showerlib richlib
	@echo Generating the documentation...
	@root -q -l makeDocs.C
alldocs : makedoc.C $(SRCS) $(HDRS) $(SOLIB)
	@echo Generating the documentation...
	@root -q -l makealldoc.C




