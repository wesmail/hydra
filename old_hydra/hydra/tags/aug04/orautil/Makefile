# old style
#CXX             = g++
#LD              = g++
#LDFLAGS         = -g

#CXXFLAGS        = -O -g -Wall -fPIC  -DR__GLIBC \

#                  -DDEBUG_LEVEL=$(HDL)  \
#                  -I$(ROOTSYS)/include
#                  -fno-rtti -fno-exceptions \

#ROOTLIBS        =  $(shell root-config --libs)

#ROOTGLIBS       = -lGpad -lGui -lGX11 -lX3d
#RINT            =  $(ROOTSYS)/src/RINT_G__Rint.o $(ROOTSYS)/src/RINT_Rint.o
#LIBS            =  $(ROOTLIBS)
#GLIBS           =  $(LIBS) $(ROOTGLIBS) -L/usr/X11R6/lib \
#                  -lm -lXpm -ldl -lX11 -rdynamic
#SOEXT           =  so
#SOFLAGS         =  -shared -Wl,-soname,

include $(HADDIR_BASEDIR)/Makefile.base
##########################################
#Configuration area
#########################################
LIBNAME = OraUtil
LIBH    = hdbfileio.h hdbconn.h hdbfileutil.h \
	  hdbtable.h hdbcolumn.h hdbcolumntype.h \
	  horasimul.h horasimutil.h horasimeventfileconfig.h \
	  horasimeventfile.h horasimeventfilestorage.h \
	  horasimrunconfig.h horasimrun.h horasimrunstorage.h

LIBL    = $(LIBNAME)LinkDef.h
LIBS    = $(LIBH:.h=.cc)
LIBO    = $(LIBS:.cc=.o)
LIBDS   = $(LIBNAME)Cint.cc
LIBDH   = $(LIBDS:.cc=.h)
LIBDO   = $(LIBDS:.cc=.o)
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBDEP  = $(LIBS:.cc=.d)
PROC    = $(ORACLE_HOME)/bin/proc

PROCFLAGS = oraca=yes code=cpp parse=partial sqlcheck=semantics \
            userid=$(ORA_USER) ireclen=130 oreclen=130 \
            include=$(ORACLE_HOME)/precomp/public \
            include=$(ORACLE_HOME)/rdbms/public \
            include=$(ORACLE_HOME)/rdbms/demo \
            include=$(ORACLE_HOME)/plsql/public \
            include=$(ORACLE_HOME)/network/public \
            include=$(ROOTSYS)/include

CORAFLAGS = $(CXXFLAGS) \
            -I$(ORACLE_HOME)/precomp/public \
            -I$(ORACLE_HOME)/rdbms/public \
            -I$(ORACLE_HOME)/rdbms/demo \
            -I$(ORACLE_HOME)/plsql/public \
            -I$(ORACLE_HOME)/network/public

# for Oracle precompiler version 8.1.6 and Oracle version 8.0.5
ifeq (8.0.5,$(findstring 8.0.5,$(ORACLE_HOME)))
LORALIB = -L$(ORACLE_HOME)/lib/ -lclntsh \
          $(ORACLE_HOME)/lib/scorept.o \
          $(ORACLE_HOME)/lib/sscoreed.o \
          $(ORACLE_HOME)/rdbms/lib/kpudfo.o -lclient -lsql \
          $(ORACLE_HOME)/lib/nautab.o \
          $(ORACLE_HOME)/lib/naeet.o \
          $(ORACLE_HOME)/lib/naect.o \
          $(ORACLE_HOME)/lib/naedhs.o \
          -L$(ORACLE_HOME)/lib/ \
           `cat $(ORACLE_HOME)/lib/naldflgs` -lnetv2 -lnttcp \
           -lnetwork -lncr -lclient -lvsn -lcommon -lgeneric \
           -lmm  -lcore4  -lplsf -lplsb -lextp -lepc \
           `cat $(ORACLE_HOME)/lib/sysliblist` -lc -lm -lnlsrtl3
endif

# for Oracle precompiler version 8.1.7
ifeq (8.1.7,$(findstring 8.1.7,$(ORACLE_HOME)))
LORALIB	= -L$(ORACLE_HOME)/lib/ -lclntsh \
	  -L$(ORACLE_HOME)/lib/ \
	  `cat $(ORACLE_HOME)/lib/sysliblist` -ldl -lm -lpthread
endif

# for Oracle precompiler version 9.0.1 and Oracle version 8.1.7
ifeq (9.0.1,$(findstring 9.0.1,$(ORACLE_HOME)))
LORALIB	= -L$(ORACLE_HOME)/lib/ -lclntsh \
	  -L$(ORACLE_HOME)/lib/ \
	  `cat $(ORACLE_HOME)/lib/sysliblist` -ldl -lm
#	    `cat $(ORACLE_HOME)/lib/sysliblist` -ldl -lm -lpthread
endif
 
##########################################
shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~ *.lis
	@for i in *.pc;do rm `echo $$i | sed -e "s/\.pc/\.cc/"`;done

$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) \
	$(LIBDO) $(LORALIB) -o $(LIBLIB)
	mv $(LIBLIB) $(HADDIR_ORAUTIL_LIB)/
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(LIBH) $(LIBL)

include $(LIBDEP)
#########################################
%.o:	%.cc
	$(CXX) $(CORAFLAGS) -c $<

%.html : %.cc %.h $(SOLIB)
	@echo "{" > doctemp.C
	@echo "gROOT->LoadMacro(\"MakeClassDoc.C\");" >> doctemp.C
	@echo "MakeClassDoc(\"$*\");" >> doctemp.C
	@echo "}" >> doctemp.C
	@root -q -l doctemp.C
	@rm doctemp.C

%.d:	%.cc
	@$(SHELL) -ec '$(CXX) -M $(CORAFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

%.cc:	%.pc
	$(PROC) $(PROCFLAGS) iname=$< oname=$*.cc
