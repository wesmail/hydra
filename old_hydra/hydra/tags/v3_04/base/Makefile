include ../Makefile.base
LIBNAME = Hydra
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)

shlib: createtemp allobject createshlib

createtemp:
	@rm -rf $(TEMPBASE)
	@mkdir $(TEMPBASE)

allobject: datasourcedir datastructdir eventhandlingdir geometrydir \
runtimedbdir utildir

createshlib:
	@cd $(TEMPBASE) \
	&& $(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) *.o -o $(LIBLIB) \
	&& ar -r $(LIBA) *.o \
	&& cp $(LIBLIB) $(HADDIR)/lib \
	&& cp $(LIBA) $(HADDIR)/lib/active \
	&& rm -rf $(TEMPBASE) 


%dir:
	@cd $* && $(MAKE) obj

clean: datasourceclean datastructclean eventhandlingclean geometryclean \
runtimedbclean  utilclean
	@rm -f lib/$(LIBLIB)

%clean:
	@cd $* && $(MAKE) clean








