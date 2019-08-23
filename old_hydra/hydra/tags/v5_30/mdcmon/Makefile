include ../Makefile.base

LINKDEF = LinkDef.h
NAME = mdcmaingui
SOURCE := $(filter-out $(NAME)Cint.cc, $(wildcard *.cc))
OBJECTS := $(patsubst %.cc,%.o,$(SOURCE))
CLASSINCLUDES = mdcmaingui.h mdcgui.h histdraw.h

CXXFLAGS += -I$(HADDIR)/mdc
CINTINC += -I$(HADDIR)/mdc
CXXFLAGS += -I$(HADDIR)/ora
CINTINC += -I$(HADDIR)/ora

include $(SOURCE:.cc=.d)

all: $(NAME)


#ascii: $(NAME)

$(NAME):  $(OBJECTS) $(NAME)Cint.o
	$(CXX) $(CXXFLAGS) $(SOFLAGS)$(NAME).$(SOEXT) $(LDFLAGS) $(OBJECTS) $(NAME)Cint.o \
	-o lib$(NAME).so > /dev/null 2>&1

$(NAME)Cint.o: $(CLASSINCLUDES) $(LINKDEF)
	@rm -f $(NAME)Cint.cc
	rootcint $(NAME)Cint.cc -c $(CINTINC) $(CLASSINCLUDES) $(LINKDEF)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(NAME)Cint.cc -c

(%.o): %.cc %.d
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $%


%.d: %.cc
	@$(SHELL) -ec '$(CXX) -M $(CXXFLAGS) $(CPPFLAGS) \
	$< |sed '\''s/$*.o/& $@/g'\'' > $@'
	@echo Makefile $*.d created


clean:
	rm -f $(OBJECTS) $(NAME)Cint.* *.so *.o *.d *~


ascii: 
	cp mdcgui.cc.ascii mdcgui.cc
	cp mdcgui.h.ascii mdcgui.h
	cp rootlogon.C.ascii rootlogon.C
	cp param/monit.C.ascii param/monit.C
	make all

ora:  	
	cp mdcgui.cc.ora mdcgui.cc
	cp mdcgui.h.ora mdcgui.h
	cp rootlogon.C.ora rootlogon.C
	cp param/monit.C.ora param/monit.C
	make all	