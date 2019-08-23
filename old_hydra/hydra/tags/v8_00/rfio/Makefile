include $(HADDIR_BASEDIR)/Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = RFIOtsm
LIBH    = hrfiofiletsm.h hrfiotsmsignalhandler.h htrfiofile.h

LIBL    = $(LIBNAME)LinkDef.h
LIBS    = $(LIBH:.h=.cc)
LIBO    = $(LIBS:.cc=.o)
LIBDS   = $(LIBNAME)Cint.cc
LIBDH   = $(LIBDS:.cc=.h)
LIBDO   = $(LIBDS:.cc=.o)
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)
LIBDEP  = $(LIBS:.cc=.d)

ADSM_INC = $(ADSM_BASE_NEW)/inc

CXXFLAGS += -I$(ADSM_INC)
CINTINC  += -I$(ADSM_INC)
CINTFLAGS+=-DRFIO_SUPPORT

LTSMRFIOLIB = -L${ADSM_BASE_NEW}/Linux -lrawapiclin -lrawservn
###########################################

shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) $(LIBDO) ${LTSMRFIOLIB} -o $(LIBLIB)
	@cp $(LIBLIB) $(HADDIR_RFIO_LIB)/
	@rm $(LIBLIB)
	@if [ '$(ACTIVELIB)' = TRUE ]; \
	then \
		ar -r $(LIBA) *.o ; \
		if [ ! -d  $(HADDIR_RFIO_LIB)/active/ ]; \
		then \
			mkdir -p  $(HADDIR_RFIO_LIB)/active/; \
		fi; \
		mv $(LIBA) $(HADDIR_RFIO_LIB)/active/ ;\
		echo static library created; \
	fi
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	@rootcint -f $(LIBDS) -c $(CINTINC) hmdcdef.h $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:	%.cc
	@echo $(CXX) -c $<
	@$(CXX) $(CXXFLAGS) -c $<

%.html : %.cc %.h $(SOLIB)
	@echo "{" > doctemp.C
	@echo "gROOT->LoadMacro(\"MakeClassDoc.C\");" >> doctemp.C
	@echo "MakeClassDoc(\"$*\");" >> doctemp.C
	@echo "}" >> doctemp.C
	@root -q -l doctemp.C
	@rm doctemp.C

%.d:	%.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'


test:
	@echo CXXFLAGS: $(CXXFLAGS)