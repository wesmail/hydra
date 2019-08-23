include ../Makefile.base

##########################################
#Configuration area
#########################################
LIBNAME = Ora
LIBH    = hparoraio.h horaconn.h horainfo.h hdetparoraio.h \
	  hmdcparoraio.h htofparoraio.h hrichparoraio.h \
	  hshowerparoraio.h hstartparoraio.h

LIBL    = $(LIBNAME)LinkDef.h
LIBS    = $(LIBH:.h=.cc) 
LIBO    = $(LIBS:.cc=.o)
LIBDS   = $(LIBNAME)Cint.cc
LIBDH   = $(LIBDS:.cc=.h)
LIBDO   = $(LIBDS:.cc=.o)
LIBLIB  = lib$(LIBNAME).$(SOEXT)
LIBA    = lib$(LIBNAME).$(AEXT)
LIBDEP  = $(LIBS:.cc=.d)
PROC    = $(ORACLE_HOME)/bin/proc

PROCFLAGS = oraca=yes code=cpp parse=partial sqlcheck=semantics \
	    userid=$(ORA_USER) ireclen=130 oreclen=130 \
            include=$(ORACLE_HOME)/precomp/public \
            include=$(ORACLE_HOME)/rdbms/public \
            include=$(ORACLE_HOME)/rdbms/demo \
            include=$(ORACLE_HOME)/plsql/public \
            include=$(ORACLE_HOME)/network/public \
	    include=$(ROOTSYS)/include \
	    include=$(HADDIR)/mdc \
	    include=$(HADDIR)/tof \
	    include=$(HADDIR)/rich \
	    include=$(HADDIR)/shower \
	    include=$(HADDIR)/start \
	    include=$(HADDIR)/base/datasource \
            include=$(HADDIR)/base/datastruct \
            include=$(HADDIR)/base/eventhandling \
            include=$(HADDIR)/base/geometry  \
            include=$(HADDIR)/base/runtimedb \
            include=$(HADDIR)/base/util 
 
CORAFLAGS = $(CXXFLAGS) \
	    -I$(HADDIR)/mdc \
	    -I$(HADDIR)/tof \
	    -I$(HADDIR)/rich \
	    -I$(HADDIR)/shower \
	    -I$(HADDIR)/start \
	    -I$(ORACLE_HOME)/precomp/public \
	    -I$(ORACLE_HOME)/rdbms/public \
	    -I$(ORACLE_HOME)/rdbms/demo \
	    -I$(ORACLE_HOME)/plsql/public \
	    -I$(ORACLE_HOME)/network/public 

  
LORALIB	= -L$(ORACLE_HOME)/lib/ -lclntsh \
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
 
##########################################

shlib: $(LIBLIB)

clean:
	@rm -f $(LIBO) $(LIBDS) $(LIBDH) $(LIBDO) $(LIBDEP) core *~ *.lis
	@for i in *.pc;do rm `echo $$i | sed -e "s/\.pc/\.cc/"`;done
 
$(LIBLIB): $(LIBO) $(LIBDO)
	$(LD) $(SOFLAGS)$(LIBLIB) $(LDFLAGS) $(LIBO) \
        $(LIBDO) $(LORALIB) -o $(LIBLIB)
	@ar -r $(LIBA) *.o
	@mv $(LIBLIB) ../lib
	@mv $(LIBA) $(HADDIR)/lib/active
	@echo Library $(LIBLIB) done.

$(LIBDS): $(LIBH) $(LIBL)
	@echo Generating dictionary $(LIBDS)
	rootcint -f $(LIBDS) -c $(CINTINC) horadef.h $(LIBH) $(LIBL)

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
