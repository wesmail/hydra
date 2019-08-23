include $(HADDIR_BASEDIR)/Makefile.base

LIBNAME = Hydra
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)

ALLDIRS = datasourcedir datastructdir eventhandlingdir geometrydir \
runtimedbdir utildir

shlib: createshlib

createtemp:
	@echo $(TEMPBASE)
	@rm -rf $(TEMPBASE)
	@mkdir -p $(TEMPBASE)

allobject: createtemp ${ALLDIRS} 


createshlib: allobject
	@cd $(TEMPBASE) \
	&& $(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) *.o -o $(LIBLIB) \
	&& if [ ! -d $(HADDIR_BASE_LIB) ]; \
		then \
			mkdir $(HADDIR_BASE_LIB); \
		fi; \
	cp $(LIBLIB) $(HADDIR_BASE_LIB) \
	&& if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
			ar -r $(LIBA) *.o; \
			if [ ! -d  $(HADDIR_BASE_LIB)/active/ ]; \
			then \
				mkdir -p  $(HADDIR_BASE_LIB)/active/; \
      fi; \
			cp $(LIBA) $(HADDIR_BASE_LIB)/active; \
	fi; \
	rm -rf $(TEMPBASE);

%dir:
	@cd $* && $(MAKE) obj


clean: datasourceclean datastructclean eventhandlingclean geometryclean \
runtimedbclean utilclean
	@rm -f lib/$(LIBLIB)

%clean:
	@cd $* && $(MAKE) clean








