CXX = g++
CPPFLAGS = -I$(ROOTSYS)/include
CXXFLAGS = -g -Wall
LDFLAGS = -shared
LINKDEF = LinkDef.h
NAME = mdcgui
SOURCE := $(filter-out $(NAME)Cint.cc, $(wildcard *.cc))
OBJECTS := $(patsubst %.cc,%.o,$(SOURCE))
CLASSINCLUDES = mdcgui.h 

include $(SOURCE:.cc=.d)

all: $(NAME)

$(NAME):  $(OBJECTS) $(NAME)Cint.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) $(NAME)Cint.o \
	-o lib$(NAME).so > /dev/null 2>&1

$(NAME)Cint.o: $(CLASSINCLUDES) $(LINKDEF)
	@rm -f $(NAME)Cint.cc
	rootcint $(NAME)Cint.cc -c $(CLASSINCLUDES) $(LINKDEF)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(NAME)Cint.cc -c

(%.o): %.cc %.d
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $%

%.d: %.cc
	@$(SHELL) -ec '$(CXX) -M $(CPPFLAGS) \
	$< |sed '\''s/$*.o/& $@/g'\'' > $@'
	@echo Makefile $*.d created

clean:
	rm -f $(OBJECTS) $(NAME)Cint.* lib$(NAME).so 

