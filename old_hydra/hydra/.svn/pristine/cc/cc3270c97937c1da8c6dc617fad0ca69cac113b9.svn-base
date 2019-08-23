include ../Makefile.base
LIBNAME = Hydra
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)

shlib: createtemp allobject createshlib

createtemp:
	@rm -rf $(TEMPBASE)
	@mkdir $(TEMPBASE)

ifdef ONLINEHYDRA
allobject: datasourcedir datastructdir eventhandlingdir geometrydir \
runtimedbdir utildir correlationdir evtserverdir
endif

ifndef ONLINEHYDRA
allobject: datasourcedir datastructdir eventhandlingdir geometrydir \
runtimedbdir utildir correlationdir 
endif


createshlib:
	@cd $(TEMPBASE) \
	&& $(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) *.o -o $(LIBLIB) \
	&& ar -r $(LIBA) *.o \
	&& cp $(LIBLIB) $(HADDIR)/lib \
	&& cp $(LIBA) $(HADDIR)/lib/active \
	&& rm -rf $(TEMPBASE) 


%dir:
	@cd $* && $(MAKE) obj

ifdef ONLINEHYDRA
clean: datasourceclean datastructclean eventhandlingclean geometryclean \
runtimedbclean utilclean correlationclean evtserverclean
	@rm -f lib/$(LIBLIB)

endif
ifndef ONLINEHYDRA
clean: datasourceclean datastructclean eventhandlingclean geometryclean \
runtimedbclean utilclean correlationclean 
	@rm -f lib/$(LIBLIB)

endif

%clean:
	@cd $* && $(MAKE) clean








