include Makefile.base


ifdef ONLINEHYDRA
all: baselib mdclib toflib  startlib showerlib richlib mdctrackDlib tofinolib triggerlib showertofinolib evtserverlib kickplanelib mdctrackSlib onlinelib evtserverlib
else
all: baselib mdclib toflib  startlib showerlib richlib mdctrackDlib tofinolib triggerlib showertofinolib evtserverlib kickplanelib mdctrackSlib onlinelib 
endif


%lib:
	@cd $* && $(MAKE) shlib

ifdef ONLINEHYDRA
clean:  baseclean mdcclean tofclean startclean showerclean richclean mdctrackDclean tofinoclean triggerclean showertofinoclean evtserverclean kickplaneclean mdctrackSclean onlineclean evtserverclean
else
clean:  baseclean mdcclean tofclean startclean showerclean richclean mdctrackDclean tofinoclean triggerclean showertofinoclean evtserverclean kickplaneclean mdctrackSclean onlineclean evtserverclean
endif


	@rm -rf $(HADDIR)/lib/*.so
	@rm -rf $(HADDIR)/lib/active/*.a

%clean:
	@cd $* && $(MAKE) clean


docs : macros/makeDocs.C baselib mdclib toflib oralib orautillib startlib showerlib richlib mdctrackD.lib tofinolib triggerlib showertofinolib evtserverlib kickplanelib mdctrackSlib onlinelib evtserverlib
	@echo Generating the documentation...
	@root -q -l makeDocs.C
alldocs : makedoc.C $(SRCS) $(HDRS) $(SOLIB)
	@echo Generating the documentation...
	@root -q -l makealldoc.C











