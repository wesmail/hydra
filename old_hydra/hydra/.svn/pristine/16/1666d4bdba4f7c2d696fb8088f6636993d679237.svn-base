include Makefile.base


all: baselib toflib mdclib oralib startlib showerlib richlib mdctrackDlib tofinolib triggerlib

%lib:
	@cd $* && $(MAKE) shlib

clean: baseclean tofclean mdcclean oraclean startclean showerclean richclean mdctrackDclean tofinoclean triggerclean
	@rm -rf $(HADDIR)/lib/*.so
	@rm -rf $(HADDIR)/lib/active/*.a

%clean:
	@cd $* && $(MAKE) clean


docs : macros/makeDocs.C baselib toflib mdclib oralib startlib showerlib mdctrackD tofinolib triggerlib
	@echo Generating the documentation...
	@root -q -l makeDocs.C
alldocs : makedoc.C $(SRCS) $(HDRS) $(SOLIB)
	@echo Generating the documentation...
	@root -q -l makealldoc.C




