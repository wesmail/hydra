include ../Makefile.base
LIBNAME = Hydra
LIBLIB  = lib$(LIBNAME).$(SOEXT)

shlib: createtemp allobject createshlib

createtemp:
	@rm -rf $(TEMP)/base
	@mkdir $(TEMP)/base

allobject: datasourcedir datastructdir eventhandlingdir geometrydir \
runtimedbdir utildir

createshlib:
	@cd $(TEMP)/base && $(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) *.o -o $(LIBLIB) && cp $(LIBLIB) $(HADDIR)/lib && rm -rf $(TEMP)/base

%dir:
	@cd $* && $(MAKE) obj

clean: datasourceclean datastructclean eventhandlingclean geometryclean \
runtimedbclean  utilclean
	@rm -f lib/$(LIBLIB)

%clean:
	@cd $* && $(MAKE) clean








