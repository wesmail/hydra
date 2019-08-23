include $(HADDIR_BASEDIR)/Makefile.base

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
	&& cp $(LIBLIB) $(HADDIR_BASE_LIB) \
	&& cp $(LIBA) $(HADDIR_BASE_LIB)/active \
	&& rm -rf $(TEMPBASE) 

%dir:
	@cd $* && $(MAKE) obj


clean: datasourceclean datastructclean eventhandlingclean geometryclean \
runtimedbclean utilclean 
	@rm -f lib/$(LIBLIB)

%clean:
	@cd $* && $(MAKE) clean








