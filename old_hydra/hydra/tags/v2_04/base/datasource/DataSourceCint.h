/********************************************************************
* DataSourceCint.h
********************************************************************/
#ifdef __CINT__
#error DataSourceCint.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
extern "C" {
#define G__ANSIHEADER
#include "G__ci.h"
extern void G__cpp_setup_tagtableDataSourceCint();
extern void G__cpp_setup_inheritanceDataSourceCint();
extern void G__cpp_setup_typetableDataSourceCint();
extern void G__cpp_setup_memvarDataSourceCint();
extern void G__cpp_setup_globalDataSourceCint();
extern void G__cpp_setup_memfuncDataSourceCint();
extern void G__cpp_setup_funcDataSourceCint();
extern void G__set_cpp_environmentDataSourceCint();
}


#include "TROOT.h"
#include "TMemberInspector.h"
#include "hdatasource.h"
#include "hgeantkine.h"
#include "hgeantreader.h"
#include "hgeantsource.h"
#include "hldbase.h"
#include "hldevt.h"
#include "hldfilesource.h"
#include "hldfilevt.h"
#include "hldremotesource.h"
#include "hldremotevt.h"
#include "hldsource.h"
#include "hldsubevt.h"
#include "hldunpack.h"
#include "hrootsource.h"

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__DataSourceCintLN_TClass;
extern G__linked_taginfo G__DataSourceCintLN_TList;
extern G__linked_taginfo G__DataSourceCintLN_TObject;
extern G__linked_taginfo G__DataSourceCintLN_TIterator;
extern G__linked_taginfo G__DataSourceCintLN_istream;
extern G__linked_taginfo G__DataSourceCintLN_TFile;
extern G__linked_taginfo G__DataSourceCintLN_HEvent;
extern G__linked_taginfo G__DataSourceCintLN_EDsState;
extern G__linked_taginfo G__DataSourceCintLN_HDataSource;
extern G__linked_taginfo G__DataSourceCintLN_HCategory;
extern G__linked_taginfo G__DataSourceCintLN_HDataObject;
extern G__linked_taginfo G__DataSourceCintLN_HLocatedDataObject;
extern G__linked_taginfo G__DataSourceCintLN_HGeantKine;
extern G__linked_taginfo G__DataSourceCintLN_HGeantReader;
extern G__linked_taginfo G__DataSourceCintLN_THashTable;
extern G__linked_taginfo G__DataSourceCintLN_HGeantSource;
extern G__linked_taginfo G__DataSourceCintLN_HldBase;
extern G__linked_taginfo G__DataSourceCintLN_HldSubEvt;
extern G__linked_taginfo G__DataSourceCintLN_HldUnpack;
extern G__linked_taginfo G__DataSourceCintLN_HldEvt;
extern G__linked_taginfo G__DataSourceCintLN_HldSource;
extern G__linked_taginfo G__DataSourceCintLN_HldFileSource;
extern G__linked_taginfo G__DataSourceCintLN_HldFilEvt;
extern G__linked_taginfo G__DataSourceCintLN_HldRemoteSource;
extern G__linked_taginfo G__DataSourceCintLN_rpcbuf;
extern G__linked_taginfo G__DataSourceCintLN_HldRemotEvt;
extern G__linked_taginfo G__DataSourceCintLN_TTree;
extern G__linked_taginfo G__DataSourceCintLN_HRootSource;
