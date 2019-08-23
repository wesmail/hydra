/********************************************************
* DataSourceCint.cc
********************************************************/
#include "DataSourceCint.h"

#ifdef G__MEMTEST
#undef malloc
#endif

extern "C" void G__cpp_reset_tagtableDataSourceCint();

extern "C" void G__set_cpp_environmentDataSourceCint() {
  G__add_macro("__MAKECINT__");
  G__add_compiledheader("TROOT.h");
  G__add_compiledheader("TMemberInspector.h");
  G__add_compiledheader("hdatasource.h");
  G__add_compiledheader("hgeantkine.h");
  G__add_compiledheader("hgeantreader.h");
  G__add_compiledheader("hgeantsource.h");
  G__add_compiledheader("hldbase.h");
  G__add_compiledheader("hldevt.h");
  G__add_compiledheader("hldfilesource.h");
  G__add_compiledheader("hldfilevt.h");
  G__add_compiledheader("hldremotesource.h");
  G__add_compiledheader("hldremotevt.h");
  G__add_compiledheader("hldsource.h");
  G__add_compiledheader("hldsubevt.h");
  G__add_compiledheader("hldunpack.h");
  G__add_compiledheader("hrootsource.h");
  G__cpp_reset_tagtableDataSourceCint();
}
extern "C" int G__cpp_dllrevDataSourceCint() { return(51111); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* HDataSource */
static int G__HDataSource_setEventAddress_0_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HDataSource*)(G__getstructoffset()))->setEventAddress((HEvent**)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_getNextEvent_1_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HDataSource*)(G__getstructoffset()))->getNextEvent());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_init_2_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HDataSource*)(G__getstructoffset()))->init());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_DeclFileName_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HDataSource*)(G__getstructoffset()))->DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_DeclFileLine_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HDataSource*)(G__getstructoffset()))->DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_ImplFileName_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HDataSource*)(G__getstructoffset()))->ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_ImplFileLine_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HDataSource*)(G__getstructoffset()))->ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_Class_Version_7_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,115,(long)((HDataSource*)(G__getstructoffset()))->Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_Class_8_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HDataSource*)(G__getstructoffset()))->Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_Dictionary_9_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HDataSource*)(G__getstructoffset()))->Dictionary();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_IsA_0_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HDataSource*)(G__getstructoffset()))->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_ShowMembers_1_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HDataSource*)(G__getstructoffset()))->ShowMembers(*(TMemberInspector*)libp->para[0].ref,(char*)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HDataSource_Streamer_2_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HDataSource*)(G__getstructoffset()))->Streamer(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HDataSource_wAHDataSource_3_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HDataSource *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HDataSource *)((G__getstructoffset())+sizeof(HDataSource)*i);
   else  delete (HDataSource *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HGeantKine */
static int G__HGeantKine_HGeantKine_0_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HGeantKine *p=NULL;
   if(G__getaryconstruct()) p=new HGeantKine[G__getaryconstruct()];
   else                    p=new HGeantKine;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HGeantKine);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_HGeantKine_1_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HGeantKine *p=NULL;
      p = new HGeantKine(*(HGeantKine*)libp->para[0].ref);
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HGeantKine);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_setParticle_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->setParticle((Int_t)G__int(libp->para[0]),(Int_t)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_setCreator_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->setCreator((Int_t)G__int(libp->para[0]),(Int_t)G__int(libp->para[1])
,(Int_t)G__int(libp->para[2]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_setVertex_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->setVertex((Float_t)G__double(libp->para[0]),(Float_t)G__double(libp->para[1])
,(Float_t)G__double(libp->para[2]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_setMomentum_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->setMomentum((Float_t)G__double(libp->para[0]),(Float_t)G__double(libp->para[1])
,(Float_t)G__double(libp->para[2]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_setGenerator_7_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->setGenerator((Float_t)G__double(libp->para[0]),(Float_t)G__double(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_getParticle_8_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->getParticle(libp->para[0].ref?*(Int_t*)libp->para[0].ref:G__Mint(libp->para[0]),libp->para[1].ref?*(Int_t*)libp->para[1].ref:G__Mint(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_getCreator_9_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->getCreator(libp->para[0].ref?*(Int_t*)libp->para[0].ref:G__Mint(libp->para[0]),libp->para[1].ref?*(Int_t*)libp->para[1].ref:G__Mint(libp->para[1])
,libp->para[2].ref?*(Int_t*)libp->para[2].ref:G__Mint(libp->para[2]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_getVertex_0_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->getVertex(libp->para[0].ref?*(Float_t*)libp->para[0].ref:G__Mfloat(libp->para[0]),libp->para[1].ref?*(Float_t*)libp->para[1].ref:G__Mfloat(libp->para[1])
,libp->para[2].ref?*(Float_t*)libp->para[2].ref:G__Mfloat(libp->para[2]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_getMomentum_1_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->getMomentum(libp->para[0].ref?*(Float_t*)libp->para[0].ref:G__Mfloat(libp->para[0]),libp->para[1].ref?*(Float_t*)libp->para[1].ref:G__Mfloat(libp->para[1])
,libp->para[2].ref?*(Float_t*)libp->para[2].ref:G__Mfloat(libp->para[2]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_getGenerator_2_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->getGenerator(libp->para[0].ref?*(Float_t*)libp->para[0].ref:G__Mfloat(libp->para[0]),libp->para[1].ref?*(Float_t*)libp->para[1].ref:G__Mfloat(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_getNLocationIndex_3_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HGeantKine*)(G__getstructoffset()))->getNLocationIndex());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_getLocationIndex_4_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HGeantKine*)(G__getstructoffset()))->getLocationIndex((Int_t)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_DeclFileName_5_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HGeantKine*)(G__getstructoffset()))->DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_DeclFileLine_6_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HGeantKine*)(G__getstructoffset()))->DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_ImplFileName_7_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HGeantKine*)(G__getstructoffset()))->ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_ImplFileLine_8_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HGeantKine*)(G__getstructoffset()))->ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_Class_Version_9_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,115,(long)((HGeantKine*)(G__getstructoffset()))->Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_Class_0_2(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HGeantKine*)(G__getstructoffset()))->Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_Dictionary_1_2(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->Dictionary();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_IsA_2_2(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HGeantKine*)(G__getstructoffset()))->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_ShowMembers_3_2(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->ShowMembers(*(TMemberInspector*)libp->para[0].ref,(char*)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantKine_Streamer_4_2(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantKine*)(G__getstructoffset()))->Streamer(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HGeantKine_wAHGeantKine_5_2(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HGeantKine *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HGeantKine *)((G__getstructoffset())+sizeof(HGeantKine)*i);
   else  delete (HGeantKine *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HGeantReader */
static int G__HGeantReader_HGeantReader_0_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HGeantReader *p=NULL;
   if(G__getaryconstruct()) p=new HGeantReader[G__getaryconstruct()];
   else                    p=new HGeantReader;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HGeantReader);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_execute_2_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HGeantReader*)(G__getstructoffset()))->execute());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_init_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HGeantReader*)(G__getstructoffset()))->init());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_setInput_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantReader*)(G__getstructoffset()))->setInput((TFile*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_DeclFileName_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HGeantReader*)(G__getstructoffset()))->DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_DeclFileLine_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HGeantReader*)(G__getstructoffset()))->DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_ImplFileName_7_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HGeantReader*)(G__getstructoffset()))->ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_ImplFileLine_8_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HGeantReader*)(G__getstructoffset()))->ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_Class_Version_9_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,115,(long)((HGeantReader*)(G__getstructoffset()))->Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_Class_0_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HGeantReader*)(G__getstructoffset()))->Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_Dictionary_1_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantReader*)(G__getstructoffset()))->Dictionary();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_IsA_2_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HGeantReader*)(G__getstructoffset()))->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_ShowMembers_3_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantReader*)(G__getstructoffset()))->ShowMembers(*(TMemberInspector*)libp->para[0].ref,(char*)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantReader_Streamer_4_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantReader*)(G__getstructoffset()))->Streamer(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__HGeantReader_HGeantReader_5_1(G__value *result7,char *funcname,struct G__param *libp,int hash)
{
   HGeantReader *p;
   if(1!=libp->paran) ;
   p=new HGeantReader(*(HGeantReader*)G__int(libp->para[0]));
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HGeantReader);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HGeantReader_wAHGeantReader_6_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HGeantReader *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HGeantReader *)((G__getstructoffset())+sizeof(HGeantReader)*i);
   else  delete (HGeantReader *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HGeantSource */
static int G__HGeantSource_HGeantSource_0_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HGeantSource *p=NULL;
   if(G__getaryconstruct()) p=new HGeantSource[G__getaryconstruct()];
   else                    p=new HGeantSource;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HGeantSource);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_HGeantSource_1_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HGeantSource *p=NULL;
      p = new HGeantSource(*(HGeantSource*)libp->para[0].ref);
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HGeantSource);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_addGeantReader_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HGeantSource*)(G__getstructoffset()))->addGeantReader((HGeantReader*)G__int(libp->para[0]),(Text_t*)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_getNextEvent_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HGeantSource*)(G__getstructoffset()))->getNextEvent());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_init_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HGeantSource*)(G__getstructoffset()))->init());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_DeclFileName_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HGeantSource*)(G__getstructoffset()))->DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_DeclFileLine_7_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HGeantSource*)(G__getstructoffset()))->DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_ImplFileName_8_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HGeantSource*)(G__getstructoffset()))->ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_ImplFileLine_9_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HGeantSource*)(G__getstructoffset()))->ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_Class_Version_0_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,115,(long)((HGeantSource*)(G__getstructoffset()))->Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_Class_1_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HGeantSource*)(G__getstructoffset()))->Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_Dictionary_2_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantSource*)(G__getstructoffset()))->Dictionary();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_IsA_3_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HGeantSource*)(G__getstructoffset()))->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_ShowMembers_4_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantSource*)(G__getstructoffset()))->ShowMembers(*(TMemberInspector*)libp->para[0].ref,(char*)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HGeantSource_Streamer_5_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HGeantSource*)(G__getstructoffset()))->Streamer(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HGeantSource_wAHGeantSource_6_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HGeantSource *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HGeantSource *)((G__getstructoffset())+sizeof(HGeantSource)*i);
   else  delete (HGeantSource *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HldBase */
static int G__HldBase_getSize_2_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,104,(long)((HldBase*)(G__getstructoffset()))->getSize());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldBase_getId_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,104,(long)((HldBase*)(G__getstructoffset()))->getId());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldBase_getDecoding_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,104,(long)((HldBase*)(G__getstructoffset()))->getDecoding());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldBase_getData_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,72,(long)((HldBase*)(G__getstructoffset()))->getData());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldBase_getEnd_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,72,(long)((HldBase*)(G__getstructoffset()))->getEnd());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldBase_getPaddedEnd_7_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,72,(long)((HldBase*)(G__getstructoffset()))->getPaddedEnd());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HldBase_wAHldBase_9_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HldBase *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HldBase *)((G__getstructoffset())+sizeof(HldBase)*i);
   else  delete (HldBase *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HldUnpack */
static int G__HldUnpack_getSubEvtId_2_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldUnpack*)(G__getstructoffset()))->getSubEvtId());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_getpSubEvt_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HldUnpack*)(G__getstructoffset()))->getpSubEvt());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_execute_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldUnpack*)(G__getstructoffset()))->execute());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_init_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldUnpack*)(G__getstructoffset()))->init());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_setCategory_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldUnpack*)(G__getstructoffset()))->setCategory((HCategory*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_DeclFileName_7_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HldUnpack*)(G__getstructoffset()))->DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_DeclFileLine_8_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldUnpack*)(G__getstructoffset()))->DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_ImplFileName_9_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HldUnpack*)(G__getstructoffset()))->ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_ImplFileLine_0_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldUnpack*)(G__getstructoffset()))->ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_Class_Version_1_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,115,(long)((HldUnpack*)(G__getstructoffset()))->Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_Class_2_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HldUnpack*)(G__getstructoffset()))->Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_Dictionary_3_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldUnpack*)(G__getstructoffset()))->Dictionary();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_IsA_4_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HldUnpack*)(G__getstructoffset()))->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_ShowMembers_5_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldUnpack*)(G__getstructoffset()))->ShowMembers(*(TMemberInspector*)libp->para[0].ref,(char*)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldUnpack_Streamer_6_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldUnpack*)(G__getstructoffset()))->Streamer(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HldUnpack_wAHldUnpack_7_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HldUnpack *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HldUnpack *)((G__getstructoffset())+sizeof(HldUnpack)*i);
   else  delete (HldUnpack *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HldSource */
static int G__HldSource_getNextEvent_2_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldSource*)(G__getstructoffset()))->getNextEvent());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_initUnpacker_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldSource*)(G__getstructoffset()))->initUnpacker());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_addUnpacker_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldSource*)(G__getstructoffset()))->addUnpacker((HldUnpack*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_showIt_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldSource*)(G__getstructoffset()))->showIt((HldEvt*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_dumpEvt_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldSource*)(G__getstructoffset()))->dumpEvt());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_setDump_8_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldSource*)(G__getstructoffset()))->setDump();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_DeclFileName_9_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HldSource*)(G__getstructoffset()))->DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_DeclFileLine_0_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldSource*)(G__getstructoffset()))->DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_ImplFileName_1_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HldSource*)(G__getstructoffset()))->ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_ImplFileLine_2_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldSource*)(G__getstructoffset()))->ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_Class_Version_3_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,115,(long)((HldSource*)(G__getstructoffset()))->Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_Class_4_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HldSource*)(G__getstructoffset()))->Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_Dictionary_5_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldSource*)(G__getstructoffset()))->Dictionary();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_IsA_6_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HldSource*)(G__getstructoffset()))->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_ShowMembers_7_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldSource*)(G__getstructoffset()))->ShowMembers(*(TMemberInspector*)libp->para[0].ref,(char*)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldSource_Streamer_8_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldSource*)(G__getstructoffset()))->Streamer(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HldSource_wAHldSource_9_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HldSource *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HldSource *)((G__getstructoffset())+sizeof(HldSource)*i);
   else  delete (HldSource *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HldFileSource */
static int G__HldFileSource_HldFileSource_0_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HldFileSource *p=NULL;
   if(G__getaryconstruct()) p=new HldFileSource[G__getaryconstruct()];
   else                    p=new HldFileSource;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HldFileSource);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_init_2_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldFileSource*)(G__getstructoffset()))->init());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_getNextEvent_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldFileSource*)(G__getstructoffset()))->getNextEvent());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_getNextFile_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldFileSource*)(G__getstructoffset()))->getNextFile());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_DeclFileName_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HldFileSource*)(G__getstructoffset()))->DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_DeclFileLine_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldFileSource*)(G__getstructoffset()))->DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_ImplFileName_7_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HldFileSource*)(G__getstructoffset()))->ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_ImplFileLine_8_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldFileSource*)(G__getstructoffset()))->ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_Class_Version_9_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,115,(long)((HldFileSource*)(G__getstructoffset()))->Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_Class_0_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HldFileSource*)(G__getstructoffset()))->Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_Dictionary_1_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldFileSource*)(G__getstructoffset()))->Dictionary();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_IsA_2_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HldFileSource*)(G__getstructoffset()))->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_ShowMembers_3_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldFileSource*)(G__getstructoffset()))->ShowMembers(*(TMemberInspector*)libp->para[0].ref,(char*)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFileSource_Streamer_4_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldFileSource*)(G__getstructoffset()))->Streamer(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__HldFileSource_HldFileSource_5_1(G__value *result7,char *funcname,struct G__param *libp,int hash)
{
   HldFileSource *p;
   if(1!=libp->paran) ;
   p=new HldFileSource(*(HldFileSource*)G__int(libp->para[0]));
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HldFileSource);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HldFileSource_wAHldFileSource_6_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HldFileSource *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HldFileSource *)((G__getstructoffset())+sizeof(HldFileSource)*i);
   else  delete (HldFileSource *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HldFilEvt */
static int G__HldFilEvt_HldFilEvt_0_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HldFilEvt *p=NULL;
   if(G__getaryconstruct()) p=new HldFilEvt[G__getaryconstruct()];
   else                    p=new HldFilEvt;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFilEvt_HldFilEvt_1_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HldFilEvt *p=NULL;
      p = new HldFilEvt((const char*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFilEvt_init_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldFilEvt*)(G__getstructoffset()))->init();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFilEvt_readSubEvt_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldFilEvt*)(G__getstructoffset()))->readSubEvt((size_t)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFilEvt_read_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldFilEvt*)(G__getstructoffset()))->read());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFilEvt_execute_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldFilEvt*)(G__getstructoffset()))->execute());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldFilEvt_swap_7_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldFilEvt*)(G__getstructoffset()))->swap());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__HldFilEvt_HldFilEvt_8_0(G__value *result7,char *funcname,struct G__param *libp,int hash)
{
   HldFilEvt *p;
   if(1!=libp->paran) ;
   p=new HldFilEvt(*(HldFilEvt*)G__int(libp->para[0]));
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HldFilEvt_wAHldFilEvt_9_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HldFilEvt *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HldFilEvt *)((G__getstructoffset())+sizeof(HldFilEvt)*i);
   else  delete (HldFilEvt *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HldRemoteSource */
static int G__HldRemoteSource_HldRemoteSource_0_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HldRemoteSource *p=NULL;
   if(G__getaryconstruct()) p=new HldRemoteSource[G__getaryconstruct()];
   else                    p=new HldRemoteSource;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_HldRemoteSource_1_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HldRemoteSource *p=NULL;
      p = new HldRemoteSource((Text_t*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_init_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldRemoteSource*)(G__getstructoffset()))->init());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_getNextEvent_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldRemoteSource*)(G__getstructoffset()))->getNextEvent());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_getNodeName_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HldRemoteSource*)(G__getstructoffset()))->getNodeName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_DeclFileName_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HldRemoteSource*)(G__getstructoffset()))->DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_DeclFileLine_7_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldRemoteSource*)(G__getstructoffset()))->DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_ImplFileName_8_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HldRemoteSource*)(G__getstructoffset()))->ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_ImplFileLine_9_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldRemoteSource*)(G__getstructoffset()))->ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_Class_Version_0_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,115,(long)((HldRemoteSource*)(G__getstructoffset()))->Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_Class_1_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HldRemoteSource*)(G__getstructoffset()))->Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_Dictionary_2_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldRemoteSource*)(G__getstructoffset()))->Dictionary();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_IsA_3_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HldRemoteSource*)(G__getstructoffset()))->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_ShowMembers_4_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldRemoteSource*)(G__getstructoffset()))->ShowMembers(*(TMemberInspector*)libp->para[0].ref,(char*)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemoteSource_Streamer_5_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HldRemoteSource*)(G__getstructoffset()))->Streamer(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__HldRemoteSource_HldRemoteSource_6_1(G__value *result7,char *funcname,struct G__param *libp,int hash)
{
   HldRemoteSource *p;
   if(1!=libp->paran) ;
   p=new HldRemoteSource(*(HldRemoteSource*)G__int(libp->para[0]));
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HldRemoteSource_wAHldRemoteSource_7_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HldRemoteSource *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HldRemoteSource *)((G__getstructoffset())+sizeof(HldRemoteSource)*i);
   else  delete (HldRemoteSource *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HldRemotEvt */
static int G__HldRemotEvt_HldRemotEvt_0_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HldRemotEvt *p=NULL;
      p = new HldRemotEvt((const char*)G__int(libp->para[0]));
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HldRemotEvt);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemotEvt_open_2_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldRemotEvt*)(G__getstructoffset()))->open((const char*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemotEvt_getBuffer_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldRemotEvt*)(G__getstructoffset()))->getBuffer((void*)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemotEvt_getLen_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,104,(long)((HldRemotEvt*)(G__getstructoffset()))->getLen());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemotEvt_read_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldRemotEvt*)(G__getstructoffset()))->read());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemotEvt_readSubEvt_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldRemotEvt*)(G__getstructoffset()))->readSubEvt((size_t)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemotEvt_reGetValue_7_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HldRemotEvt*)(G__getstructoffset()))->reGetValue((Int_t)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemotEvt_execute_8_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldRemotEvt*)(G__getstructoffset()))->execute());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HldRemotEvt_swap_9_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HldRemotEvt*)(G__getstructoffset()))->swap());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__HldRemotEvt_HldRemotEvt_0_1(G__value *result7,char *funcname,struct G__param *libp,int hash)
{
   HldRemotEvt *p;
   if(1!=libp->paran) ;
   p=new HldRemotEvt(*(HldRemotEvt*)G__int(libp->para[0]));
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HldRemotEvt);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HldRemotEvt_wAHldRemotEvt_1_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HldRemotEvt *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HldRemotEvt *)((G__getstructoffset())+sizeof(HldRemotEvt)*i);
   else  delete (HldRemotEvt *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* HRootSource */
static int G__HRootSource_HRootSource_0_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   HRootSource *p=NULL;
   if(G__getaryconstruct()) p=new HRootSource[G__getaryconstruct()];
   else                    p=new HRootSource;
      result7->obj.i = (long)p;
      result7->ref = (long)p;
      result7->type = 'u';
      result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HRootSource);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_getNextEvent_2_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HRootSource*)(G__getstructoffset()))->getNextEvent());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_init_3_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HRootSource*)(G__getstructoffset()))->init());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_getEvent_4_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HRootSource*)(G__getstructoffset()))->getEvent((Int_t)G__int(libp->para[0])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_setInput_5_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,98,(long)((HRootSource*)(G__getstructoffset()))->setInput((Text_t*)G__int(libp->para[0]),(Text_t*)G__int(libp->para[1])));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_deactivateBranch_6_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HRootSource*)(G__getstructoffset()))->deactivateBranch((Text_t*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_getTree_7_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HRootSource*)(G__getstructoffset()))->getTree());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_getSplitLevel_8_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HRootSource*)(G__getstructoffset()))->getSplitLevel());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_Clear_9_0(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HRootSource*)(G__getstructoffset()))->Clear();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_DeclFileName_0_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HRootSource*)(G__getstructoffset()))->DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_DeclFileLine_1_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HRootSource*)(G__getstructoffset()))->DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_ImplFileName_2_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((HRootSource*)(G__getstructoffset()))->ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_ImplFileLine_3_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((HRootSource*)(G__getstructoffset()))->ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_Class_Version_4_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,115,(long)((HRootSource*)(G__getstructoffset()))->Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_Class_5_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HRootSource*)(G__getstructoffset()))->Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_Dictionary_6_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HRootSource*)(G__getstructoffset()))->Dictionary();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_IsA_7_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((HRootSource*)(G__getstructoffset()))->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_ShowMembers_8_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HRootSource*)(G__getstructoffset()))->ShowMembers(*(TMemberInspector*)libp->para[0].ref,(char*)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__HRootSource_Streamer_9_1(G__value *result7,char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((HRootSource*)(G__getstructoffset()))->Streamer(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__HRootSource_HRootSource_0_2(G__value *result7,char *funcname,struct G__param *libp,int hash)
{
   HRootSource *p;
   if(1!=libp->paran) ;
   p=new HRootSource(*(HRootSource*)G__int(libp->para[0]));
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__DataSourceCintLN_HRootSource);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__HRootSource_wAHRootSource_1_2(G__value *result7,char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (HRootSource *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (HRootSource *)((G__getstructoffset())+sizeof(HRootSource)*i);
   else  delete (HRootSource *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/* HDataSource */

/* HGeantKine */

/* HGeantReader */

/* HGeantSource */

/* HldBase */

/* HldUnpack */

/* HldSource */

/* HldFileSource */

/* HldFilEvt */

/* HldRemoteSource */

/* HldRemotEvt */

/* HRootSource */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncDataSourceCint {
 public:
  G__Sizep2memfuncDataSourceCint() {p=&G__Sizep2memfuncDataSourceCint::sizep2memfunc;}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncDataSourceCint::*p)();
};

size_t G__get_sizep2memfuncDataSourceCint()
{
  G__Sizep2memfuncDataSourceCint a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceDataSourceCint() {

   /* Setting up class inheritance */
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DataSourceCintLN_HDataSource))) {
     HDataSource *G__Lderived;
     G__Lderived=(HDataSource*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HDataSource),G__get_linked_tagnum(&G__DataSourceCintLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantKine))) {
     HGeantKine *G__Lderived;
     G__Lderived=(HGeantKine*)0x1000;
     {
       HLocatedDataObject *G__Lpbase=(HLocatedDataObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantKine),G__get_linked_tagnum(&G__DataSourceCintLN_HLocatedDataObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       HDataObject *G__Lpbase=(HDataObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantKine),G__get_linked_tagnum(&G__DataSourceCintLN_HDataObject),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantKine),G__get_linked_tagnum(&G__DataSourceCintLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantReader))) {
     HGeantReader *G__Lderived;
     G__Lderived=(HGeantReader*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantReader),G__get_linked_tagnum(&G__DataSourceCintLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantSource))) {
     HGeantSource *G__Lderived;
     G__Lderived=(HGeantSource*)0x1000;
     {
       HDataSource *G__Lpbase=(HDataSource*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantSource),G__get_linked_tagnum(&G__DataSourceCintLN_HDataSource),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantSource),G__get_linked_tagnum(&G__DataSourceCintLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DataSourceCintLN_HldUnpack))) {
     HldUnpack *G__Lderived;
     G__Lderived=(HldUnpack*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldUnpack),G__get_linked_tagnum(&G__DataSourceCintLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DataSourceCintLN_HldSource))) {
     HldSource *G__Lderived;
     G__Lderived=(HldSource*)0x1000;
     {
       HDataSource *G__Lpbase=(HDataSource*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldSource),G__get_linked_tagnum(&G__DataSourceCintLN_HDataSource),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldSource),G__get_linked_tagnum(&G__DataSourceCintLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DataSourceCintLN_HldFileSource))) {
     HldFileSource *G__Lderived;
     G__Lderived=(HldFileSource*)0x1000;
     {
       HldSource *G__Lpbase=(HldSource*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldFileSource),G__get_linked_tagnum(&G__DataSourceCintLN_HldSource),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       HDataSource *G__Lpbase=(HDataSource*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldFileSource),G__get_linked_tagnum(&G__DataSourceCintLN_HDataSource),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldFileSource),G__get_linked_tagnum(&G__DataSourceCintLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt))) {
     HldFilEvt *G__Lderived;
     G__Lderived=(HldFilEvt*)0x1000;
     {
       HldEvt *G__Lpbase=(HldEvt*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt),G__get_linked_tagnum(&G__DataSourceCintLN_HldEvt),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       HldBase *G__Lpbase=(HldBase*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt),G__get_linked_tagnum(&G__DataSourceCintLN_HldBase),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource))) {
     HldRemoteSource *G__Lderived;
     G__Lderived=(HldRemoteSource*)0x1000;
     {
       HldSource *G__Lpbase=(HldSource*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource),G__get_linked_tagnum(&G__DataSourceCintLN_HldSource),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       HDataSource *G__Lpbase=(HDataSource*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource),G__get_linked_tagnum(&G__DataSourceCintLN_HDataSource),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource),G__get_linked_tagnum(&G__DataSourceCintLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemotEvt))) {
     HldRemotEvt *G__Lderived;
     G__Lderived=(HldRemotEvt*)0x1000;
     {
       HldEvt *G__Lpbase=(HldEvt*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemotEvt),G__get_linked_tagnum(&G__DataSourceCintLN_HldEvt),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       HldBase *G__Lpbase=(HldBase*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemotEvt),G__get_linked_tagnum(&G__DataSourceCintLN_HldBase),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
   }
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__DataSourceCintLN_HRootSource))) {
     HRootSource *G__Lderived;
     G__Lderived=(HRootSource*)0x1000;
     {
       HDataSource *G__Lpbase=(HDataSource*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HRootSource),G__get_linked_tagnum(&G__DataSourceCintLN_HDataSource),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HRootSource),G__get_linked_tagnum(&G__DataSourceCintLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,0);
     }
   }
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableDataSourceCint() {

   /* Setting up typedef entry */
   G__search_typename2("size_t",104,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("Char_t",99,-1,0,
-1);
   G__setnewtype(-1,"Signed Character 1 byte",0);
   G__search_typename2("UChar_t",98,-1,0,
-1);
   G__setnewtype(-1,"Unsigned Character 1 byte",0);
   G__search_typename2("Short_t",115,-1,0,
-1);
   G__setnewtype(-1,"Signed Short integer 2 bytes",0);
   G__search_typename2("UShort_t",114,-1,0,
-1);
   G__setnewtype(-1,"Unsigned Short integer 2 bytes",0);
   G__search_typename2("Int_t",105,-1,0,
-1);
   G__setnewtype(-1,"Signed integer 4 bytes",0);
   G__search_typename2("UInt_t",104,-1,0,
-1);
   G__setnewtype(-1,"Unsigned integer 4 bytes",0);
   G__search_typename2("Seek_t",105,-1,0,
-1);
   G__setnewtype(-1,"File pointer",0);
   G__search_typename2("Long_t",108,-1,0,
-1);
   G__setnewtype(-1,"Signed long integer 8 bytes",0);
   G__search_typename2("ULong_t",107,-1,0,
-1);
   G__setnewtype(-1,"Unsigned long integer 8 bytes",0);
   G__search_typename2("Float_t",102,-1,0,
-1);
   G__setnewtype(-1,"Float 4 bytes",0);
   G__search_typename2("Double_t",100,-1,0,
-1);
   G__setnewtype(-1,"Float 8 bytes",0);
   G__search_typename2("Text_t",99,-1,0,
-1);
   G__setnewtype(-1,"General string",0);
   G__search_typename2("Bool_t",98,-1,0,
-1);
   G__setnewtype(-1,"Boolean (0=false, 1=true)",0);
   G__search_typename2("Byte_t",98,-1,0,
-1);
   G__setnewtype(-1,"Byte (8 bits)",0);
   G__search_typename2("Version_t",115,-1,0,
-1);
   G__setnewtype(-1,"Class version identifier",0);
   G__search_typename2("Option_t",99,-1,0,
-1);
   G__setnewtype(-1,"Option string",0);
   G__search_typename2("Ssiz_t",105,-1,0,
-1);
   G__setnewtype(-1,"String size",0);
   G__search_typename2("Real_t",102,-1,0,
-1);
   G__setnewtype(-1,"TVector and TMatrix element type",0);
   G__search_typename2("VoidFuncPtr_t",89,-1,0,
-1);
   G__setnewtype(-1,"pointer to void function",0);
   G__search_typename2("FreeHookFun_t",89,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("ReAllocFun_t",81,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("ReAllocCFun_t",81,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("Axis_t",102,-1,0,
-1);
   G__setnewtype(-1,"Axis values type",0);
   G__search_typename2("Stat_t",100,-1,0,
-1);
   G__setnewtype(-1,"Statistics type",0);
   G__search_typename2("Cat_t",115,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("IdOfs_t",115,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("Loc_t",104,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("UInt4",104,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("UInt1",98,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("rpcbuf",117,G__get_linked_tagnum(&G__DataSourceCintLN_rpcbuf),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("Font_t",115,-1,0,
-1);
   G__setnewtype(-1,"Font number",0);
   G__search_typename2("Style_t",115,-1,0,
-1);
   G__setnewtype(-1,"Style number",0);
   G__search_typename2("Marker_t",115,-1,0,
-1);
   G__setnewtype(-1,"Marker number",0);
   G__search_typename2("Width_t",115,-1,0,
-1);
   G__setnewtype(-1,"Line width",0);
   G__search_typename2("Color_t",115,-1,0,
-1);
   G__setnewtype(-1,"Color number",0);
   G__search_typename2("SCoord_t",115,-1,0,
-1);
   G__setnewtype(-1,"Screen coordinates",0);
   G__search_typename2("Coord_t",102,-1,0,
-1);
   G__setnewtype(-1,"Pad world coordinates",0);
   G__search_typename2("Angle_t",102,-1,0,
-1);
   G__setnewtype(-1,"Graphics angle",0);
   G__search_typename2("Size_t",102,-1,0,
-1);
   G__setnewtype(-1,"Attribute size",0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* HDataSource */
static void G__setup_memvarHDataSource(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HDataSource));
   { HDataSource *p; p=(HDataSource*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,85,2,0,G__get_linked_tagnum(&G__DataSourceCintLN_HEvent),-1,-1,2,"fEventAddr=",0,"! Address of the event to fill");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* HGeantKine */
static void G__setup_memvarHGeantKine(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantKine));
   { HGeantKine *p; p=(HGeantKine*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,105,0,0,-1,G__defined_typename("Int_t"),-1,4,"trackNumber=",0,"GEANT track number");
   G__memvar_setup((void*)NULL,105,0,0,-1,G__defined_typename("Int_t"),-1,4,"parentTrack=",0,"track number of parent particle");
   G__memvar_setup((void*)NULL,105,0,0,-1,G__defined_typename("Int_t"),-1,4,"particleID=",0,"GEANT particle ID number");
   G__memvar_setup((void*)NULL,105,0,0,-1,G__defined_typename("Int_t"),-1,4,"mediumNumber=",0,"GEANT medium of creation number");
   G__memvar_setup((void*)NULL,105,0,0,-1,G__defined_typename("Int_t"),-1,4,"creationMechanism=",0,"GEANT creation mechanism number");
   G__memvar_setup((void*)NULL,102,0,0,-1,G__defined_typename("Float_t"),-1,4,"xVertex=",0,"x of track vertex  (in cm)");
   G__memvar_setup((void*)NULL,102,0,0,-1,G__defined_typename("Float_t"),-1,4,"yVertex=",0,"y");
   G__memvar_setup((void*)NULL,102,0,0,-1,G__defined_typename("Float_t"),-1,4,"zVertex=",0,"z");
   G__memvar_setup((void*)NULL,102,0,0,-1,G__defined_typename("Float_t"),-1,4,"xMom=",0,"x component of particle momentum (in MeV/c)");
   G__memvar_setup((void*)NULL,102,0,0,-1,G__defined_typename("Float_t"),-1,4,"yMom=",0,"y");
   G__memvar_setup((void*)NULL,102,0,0,-1,G__defined_typename("Float_t"),-1,4,"zMom=",0,"z");
   G__memvar_setup((void*)NULL,102,0,0,-1,G__defined_typename("Float_t"),-1,4,"generatorInfo=",0,"event generator info");
   G__memvar_setup((void*)NULL,102,0,0,-1,G__defined_typename("Float_t"),-1,4,"generatorWeight=",0,"associated weight ");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* HGeantReader */
static void G__setup_memvarHGeantReader(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantReader));
   { HGeantReader *p; p=(HGeantReader*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TFile),-1,-1,2,"fInputFile=",0,"Pointer to the input file.");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* HGeantSource */
static void G__setup_memvarHGeantSource(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantSource));
   { HGeantSource *p; p=(HGeantSource*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,117,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TList),-1,-1,4,"fReaderList=",0,"List of active geant readers.");
   G__memvar_setup((void*)NULL,117,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_THashTable),-1,-1,4,"fFileTable=",0,"Hash table with input files");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* HldBase */
static void G__setup_memvarHldBase(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldBase));
   { HldBase *p; p=(HldBase*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,108,0,0,-1,-1,-1,4,"G__virtualinfo=",0,(char*)NULL);
   G__memvar_setup((void*)NULL,72,0,0,-1,G__defined_typename("UInt4"),-1,2,"pHdr=",0,(char*)NULL);
   G__memvar_setup((void*)NULL,72,0,0,-1,G__defined_typename("UInt4"),-1,2,"pData=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* HldUnpack */
static void G__setup_memvarHldUnpack(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldUnpack));
   { HldUnpack *p; p=(HldUnpack*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_HldSubEvt),-1,-1,2,"pSubEvt=",0,"! Buffer where data are read from");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_HCategory),-1,-1,2,"pRawCat=",0,"! The category where data will be stored;");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* HldSource */
static void G__setup_memvarHldSource(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldSource));
   { HldSource *p; p=(HldSource*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,98,0,0,-1,G__defined_typename("Bool_t"),-1,2,"isDumped=",0,(char*)NULL);
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TList),-1,-1,2,"fUnpackerList=",0,"! List of the unpackers used to extract data");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_HldEvt),-1,-1,2,"fReadEvent=",0,"! Buffer where the data is first read.");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* HldFileSource */
static void G__setup_memvarHldFileSource(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldFileSource));
   { HldFileSource *p; p=(HldFileSource*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TIterator),-1,-1,2,"iter=",0,(char*)NULL);
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* HldFilEvt */
static void G__setup_memvarHldFilEvt(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt));
   { HldFilEvt *p; p=(HldFilEvt*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_istream),-1,-1,4,"file=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* HldRemoteSource */
static void G__setup_memvarHldRemoteSource(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource));
   { HldRemoteSource *p; p=(HldRemoteSource*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,67,0,0,-1,G__defined_typename("Text_t"),-1,2,"currNodeName=",0,(char*)NULL);
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TIterator),-1,-1,2,"iter=",0,(char*)NULL);
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}


   /* HldRemotEvt */
static void G__setup_memvarHldRemotEvt(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemotEvt));
   { HldRemotEvt *p; p=(HldRemotEvt*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,67,0,1,-1,G__defined_typename("Char_t"),-1,4,"pcNode=",0,"server node name");
   G__memvar_setup((void*)NULL,89,0,0,-1,-1,-1,4,"rpcHandle=",0,"RPC handle for remote access");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_rpcbuf),G__defined_typename("rpcbuf"),-1,4,"rpcBuf=",0,"ptr to buffer provided by RPC");
   G__memvar_setup((void*)NULL,105,0,0,-1,G__defined_typename("Int_t"),-1,4,"iEvtNum=",0,"last event no. handled");
   }
   G__tag_memvar_reset();
}


   /* HRootSource */
static void G__setup_memvarHRootSource(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HRootSource));
   { HRootSource *p; p=(HRootSource*)0x1000; if (p) { }
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TTree),-1,-1,4,"fInputTree=",0,"TTree to be read.");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TFile),-1,-1,4,"fInputFile=",0,"Input file.");
   G__memvar_setup((void*)NULL,105,0,0,-1,G__defined_typename("Int_t"),-1,4,"fCursor=",0,"Number of next event.");
   G__memvar_setup((void*)NULL,105,0,0,-1,G__defined_typename("Int_t"),-1,4,"fSplitLevel=",0,"Split level of input tree");
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarDataSourceCint() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncHDataSource(void) {
   /* HDataSource */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HDataSource));
   G__memfunc_setup("setEventAddress",1556,G__HDataSource_setEventAddress_0_0,121,-1,-1,0,1,1,1,0,"U 'HEvent' - 2 - ev",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getNextEvent",1249,G__HDataSource_getNextEvent_1_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_EDsState),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,3);
   G__memfunc_setup("init",436,G__HDataSource_init_2_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,3);
   G__memfunc_setup("DeclFileName",1145,G__HDataSource_DeclFileName_3_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileLine",1152,G__HDataSource_DeclFileLine_4_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileName",1171,G__HDataSource_ImplFileName_5_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileLine",1178,G__HDataSource_ImplFileLine_6_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class_Version",1339,G__HDataSource_Class_Version_7_0,115,-1,G__defined_typename("Version_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class",502,G__HDataSource_Class_8_0,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Dictionary",1046,G__HDataSource_Dictionary_9_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsA",253,G__HDataSource_IsA_0_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("ShowMembers",1132,G__HDataSource_ShowMembers_1_1,121,-1,-1,0,2,1,1,0,
"u 'TMemberInspector' - 1 - insp C - - 0 - parent",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Streamer",835,G__HDataSource_Streamer_2_1,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,1);
   // automatic destructor
   G__memfunc_setup("~HDataSource",1201,G__HDataSource_wAHDataSource_3_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHGeantKine(void) {
   /* HGeantKine */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantKine));
   G__memfunc_setup("HGeantKine",958,G__HGeantKine_HGeantKine_0_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HGeantKine),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("HGeantKine",958,G__HGeantKine_HGeantKine_1_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HGeantKine),-1,0,1,1,1,0,"u 'HGeantKine' - 1 - aKine",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setParticle",1152,G__HGeantKine_setParticle_3_0,121,-1,-1,0,2,1,1,0,
"i - 'Int_t' 0 - aTrack i - 'Int_t' 0 - aID",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setCreator",1052,G__HGeantKine_setCreator_4_0,121,-1,-1,0,3,1,1,0,
"i - 'Int_t' 0 - aPar i - 'Int_t' 0 - aMed "
"i - 'Int_t' 0 - aMech",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setVertex",970,G__HGeantKine_setVertex_5_0,121,-1,-1,0,3,1,1,0,
"f - 'Float_t' 0 - ax f - 'Float_t' 0 - ay "
"f - 'Float_t' 0 - az",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setMomentum",1182,G__HGeantKine_setMomentum_6_0,121,-1,-1,0,3,1,1,0,
"f - 'Float_t' 0 - apx f - 'Float_t' 0 - apy "
"f - 'Float_t' 0 - apz",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setGenerator",1267,G__HGeantKine_setGenerator_7_0,121,-1,-1,0,2,1,1,0,
"f - 'Float_t' 0 - aInfo f - 'Float_t' 0 - aWeight",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getParticle",1140,G__HGeantKine_getParticle_8_0,121,-1,-1,0,2,1,1,0,
"i - 'Int_t' 1 - aTrack i - 'Int_t' 1 - aID",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getCreator",1040,G__HGeantKine_getCreator_9_0,121,-1,-1,0,3,1,1,0,
"i - 'Int_t' 1 - aPar i - 'Int_t' 1 - aMed "
"i - 'Int_t' 1 - aMech",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getVertex",958,G__HGeantKine_getVertex_0_1,121,-1,-1,0,3,1,1,0,
"f - 'Float_t' 1 - ax f - 'Float_t' 1 - ay "
"f - 'Float_t' 1 - az",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getMomentum",1170,G__HGeantKine_getMomentum_1_1,121,-1,-1,0,3,1,1,0,
"f - 'Float_t' 1 - apx f - 'Float_t' 1 - apy "
"f - 'Float_t' 1 - apz",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getGenerator",1255,G__HGeantKine_getGenerator_2_1,121,-1,-1,0,2,1,1,0,
"f - 'Float_t' 1 - aInfo f - 'Float_t' 1 - aWeight",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getNLocationIndex",1727,G__HGeantKine_getNLocationIndex_3_1,105,-1,G__defined_typename("Int_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("getLocationIndex",1649,G__HGeantKine_getLocationIndex_4_1,105,-1,G__defined_typename("Int_t"),0,1,1,1,0,"i - 'Int_t' 0 - -",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("DeclFileName",1145,G__HGeantKine_DeclFileName_5_1,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileLine",1152,G__HGeantKine_DeclFileLine_6_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileName",1171,G__HGeantKine_ImplFileName_7_1,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileLine",1178,G__HGeantKine_ImplFileLine_8_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class_Version",1339,G__HGeantKine_Class_Version_9_1,115,-1,G__defined_typename("Version_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class",502,G__HGeantKine_Class_0_2,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Dictionary",1046,G__HGeantKine_Dictionary_1_2,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsA",253,G__HGeantKine_IsA_2_2,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("ShowMembers",1132,G__HGeantKine_ShowMembers_3_2,121,-1,-1,0,2,1,1,0,
"u 'TMemberInspector' - 1 - insp C - - 0 - parent",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Streamer",835,G__HGeantKine_Streamer_4_2,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,1);
   // automatic destructor
   G__memfunc_setup("~HGeantKine",1084,G__HGeantKine_wAHGeantKine_5_2,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHGeantReader(void) {
   /* HGeantReader */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantReader));
   G__memfunc_setup("HGeantReader",1162,G__HGeantReader_HGeantReader_0_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HGeantReader),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("execute",755,G__HGeantReader_execute_2_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("init",436,G__HGeantReader_init_3_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("setInput",860,G__HGeantReader_setInput_4_0,121,-1,-1,0,1,1,1,0,"U 'TFile' - 0 - file",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("DeclFileName",1145,G__HGeantReader_DeclFileName_5_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileLine",1152,G__HGeantReader_DeclFileLine_6_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileName",1171,G__HGeantReader_ImplFileName_7_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileLine",1178,G__HGeantReader_ImplFileLine_8_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class_Version",1339,G__HGeantReader_Class_Version_9_0,115,-1,G__defined_typename("Version_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class",502,G__HGeantReader_Class_0_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Dictionary",1046,G__HGeantReader_Dictionary_1_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsA",253,G__HGeantReader_IsA_2_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("ShowMembers",1132,G__HGeantReader_ShowMembers_3_1,121,-1,-1,0,2,1,1,0,
"u 'TMemberInspector' - 1 - insp C - - 0 - parent",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Streamer",835,G__HGeantReader_Streamer_4_1,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,1);
   // automatic copy constructor
   G__memfunc_setup("HGeantReader",1162,G__HGeantReader_HGeantReader_5_1,(int)('i'),G__get_linked_tagnum(&G__DataSourceCintLN_HGeantReader),-1,0,1,1,1,0,"u 'HGeantReader' - 1 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~HGeantReader",1288,G__HGeantReader_wAHGeantReader_6_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHGeantSource(void) {
   /* HGeantSource */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantSource));
   G__memfunc_setup("HGeantSource",1192,G__HGeantSource_HGeantSource_0_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HGeantSource),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("HGeantSource",1192,G__HGeantSource_HGeantSource_1_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HGeantSource),-1,0,1,1,1,0,"u 'HGeantSource' - 1 - s",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("addGeantReader",1387,G__HGeantSource_addGeantReader_3_0,98,-1,G__defined_typename("Bool_t"),0,2,1,1,0,
"U 'HGeantReader' - 0 - r C - 'Text_t' 0 - inputFile",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getNextEvent",1249,G__HGeantSource_getNextEvent_4_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_EDsState),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("init",436,G__HGeantSource_init_5_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("DeclFileName",1145,G__HGeantSource_DeclFileName_6_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileLine",1152,G__HGeantSource_DeclFileLine_7_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileName",1171,G__HGeantSource_ImplFileName_8_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileLine",1178,G__HGeantSource_ImplFileLine_9_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class_Version",1339,G__HGeantSource_Class_Version_0_1,115,-1,G__defined_typename("Version_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class",502,G__HGeantSource_Class_1_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Dictionary",1046,G__HGeantSource_Dictionary_2_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsA",253,G__HGeantSource_IsA_3_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("ShowMembers",1132,G__HGeantSource_ShowMembers_4_1,121,-1,-1,0,2,1,1,0,
"u 'TMemberInspector' - 1 - insp C - - 0 - parent",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Streamer",835,G__HGeantSource_Streamer_5_1,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,1);
   // automatic destructor
   G__memfunc_setup("~HGeantSource",1318,G__HGeantSource_wAHGeantSource_6_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHldBase(void) {
   /* HldBase */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldBase));
   G__memfunc_setup("getSize",731,G__HldBase_getSize_2_0,104,-1,G__defined_typename("size_t"),0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getId",493,G__HldBase_getId_3_0,104,-1,G__defined_typename("UInt4"),0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getDecoding",1117,G__HldBase_getDecoding_4_0,104,-1,G__defined_typename("UInt4"),0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getData",698,G__HldBase_getData_5_0,72,-1,G__defined_typename("UInt4"),0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getEnd",599,G__HldBase_getEnd_6_0,72,-1,G__defined_typename("UInt4"),0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getPaddedEnd",1177,G__HldBase_getPaddedEnd_7_0,72,-1,G__defined_typename("UInt4"),0,0,1,1,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getHdrSize",1017,(G__InterfaceMethod)NULL,104,-1,G__defined_typename("size_t"),0,0,1,2,8,"",(char*)NULL,(void*)NULL,3);
   G__memfunc_setup("getPaddedSize",1309,(G__InterfaceMethod)NULL,104,-1,G__defined_typename("size_t"),0,0,1,2,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getHdrLen",893,(G__InterfaceMethod)NULL,104,-1,G__defined_typename("size_t"),0,0,1,2,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getDataSize",1109,(G__InterfaceMethod)NULL,104,-1,G__defined_typename("size_t"),0,0,1,2,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getDataPaddedSize",1687,(G__InterfaceMethod)NULL,104,-1,G__defined_typename("size_t"),0,0,1,2,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("isSwapped",944,(G__InterfaceMethod)NULL,98,-1,G__defined_typename("Bool_t"),0,0,1,2,8,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("swapHdr",729,(G__InterfaceMethod)NULL,121,-1,-1,0,0,1,2,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("byte",436,(G__InterfaceMethod)NULL,105,-1,-1,0,2,1,2,8,
"i - - 0 - b h - 'UInt4' 0 - i",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("align8",579,(G__InterfaceMethod)NULL,104,-1,G__defined_typename("size_t"),0,1,1,2,8,"h - 'size_t' 0 - i",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("swap4",495,(G__InterfaceMethod)NULL,121,-1,-1,0,2,1,2,8,
"H - 'UInt_t' 0 - p h - 'size_t' 0 - len",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("swap2",493,(G__InterfaceMethod)NULL,121,-1,-1,0,2,1,2,8,
"R - 'UShort_t' 0 - p h - 'size_t' 0 - len",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~HldBase",785,G__HldBase_wAHldBase_9_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHldUnpack(void) {
   /* HldUnpack */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldUnpack));
   G__memfunc_setup("getSubEvtId",1094,G__HldUnpack_getSubEvtId_2_0,105,-1,-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,3);
   G__memfunc_setup("getpSubEvt",1033,G__HldUnpack_getpSubEvt_3_0,85,G__get_linked_tagnum(&G__DataSourceCintLN_HldSubEvt),G__defined_typename("HPP"),2,0,1,1,4,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("execute",755,G__HldUnpack_execute_4_0,105,-1,G__defined_typename("Int_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("init",436,G__HldUnpack_init_5_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,3);
   G__memfunc_setup("setCategory",1162,G__HldUnpack_setCategory_6_0,121,-1,-1,0,1,1,1,0,"U 'HCategory' - 0 - aCat",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileName",1145,G__HldUnpack_DeclFileName_7_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileLine",1152,G__HldUnpack_DeclFileLine_8_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileName",1171,G__HldUnpack_ImplFileName_9_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileLine",1178,G__HldUnpack_ImplFileLine_0_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class_Version",1339,G__HldUnpack_Class_Version_1_1,115,-1,G__defined_typename("Version_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class",502,G__HldUnpack_Class_2_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Dictionary",1046,G__HldUnpack_Dictionary_3_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsA",253,G__HldUnpack_IsA_4_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("ShowMembers",1132,G__HldUnpack_ShowMembers_5_1,121,-1,-1,0,2,1,1,0,
"u 'TMemberInspector' - 1 - insp C - - 0 - parent",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Streamer",835,G__HldUnpack_Streamer_6_1,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,1);
   // automatic destructor
   G__memfunc_setup("~HldUnpack",1016,G__HldUnpack_wAHldUnpack_7_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHldSource(void) {
   /* HldSource */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldSource));
   G__memfunc_setup("getNextEvent",1249,G__HldSource_getNextEvent_2_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_EDsState),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("initUnpacker",1261,G__HldSource_initUnpacker_3_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("addUnpacker",1122,G__HldSource_addUnpacker_4_0,121,-1,-1,0,1,1,1,0,"U 'HldUnpack' - 0 - unpacker",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("showIt",638,G__HldSource_showIt_5_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_EDsState),-1,0,1,1,1,0,"U 'HldEvt' - 0 - evt",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("dumpEvt",741,G__HldSource_dumpEvt_6_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_EDsState),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("decodeHeader",1197,(G__InterfaceMethod)NULL,121,-1,-1,0,0,1,2,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setDump",738,G__HldSource_setDump_8_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileName",1145,G__HldSource_DeclFileName_9_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileLine",1152,G__HldSource_DeclFileLine_0_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileName",1171,G__HldSource_ImplFileName_1_1,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileLine",1178,G__HldSource_ImplFileLine_2_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class_Version",1339,G__HldSource_Class_Version_3_1,115,-1,G__defined_typename("Version_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class",502,G__HldSource_Class_4_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Dictionary",1046,G__HldSource_Dictionary_5_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsA",253,G__HldSource_IsA_6_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("ShowMembers",1132,G__HldSource_ShowMembers_7_1,121,-1,-1,0,2,1,1,0,
"u 'TMemberInspector' - 1 - insp C - - 0 - parent",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Streamer",835,G__HldSource_Streamer_8_1,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,1);
   // automatic destructor
   G__memfunc_setup("~HldSource",1031,G__HldSource_wAHldSource_9_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHldFileSource(void) {
   /* HldFileSource */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldFileSource));
   G__memfunc_setup("HldFileSource",1289,G__HldFileSource_HldFileSource_0_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HldFileSource),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("init",436,G__HldFileSource_init_2_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("getNextEvent",1249,G__HldFileSource_getNextEvent_3_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_EDsState),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("getNextFile",1119,G__HldFileSource_getNextFile_4_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileName",1145,G__HldFileSource_DeclFileName_5_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileLine",1152,G__HldFileSource_DeclFileLine_6_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileName",1171,G__HldFileSource_ImplFileName_7_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileLine",1178,G__HldFileSource_ImplFileLine_8_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class_Version",1339,G__HldFileSource_Class_Version_9_0,115,-1,G__defined_typename("Version_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class",502,G__HldFileSource_Class_0_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Dictionary",1046,G__HldFileSource_Dictionary_1_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsA",253,G__HldFileSource_IsA_2_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("ShowMembers",1132,G__HldFileSource_ShowMembers_3_1,121,-1,-1,0,2,1,1,0,
"u 'TMemberInspector' - 1 - insp C - - 0 - parent",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Streamer",835,G__HldFileSource_Streamer_4_1,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,1);
   // automatic copy constructor
   G__memfunc_setup("HldFileSource",1289,G__HldFileSource_HldFileSource_5_1,(int)('i'),G__get_linked_tagnum(&G__DataSourceCintLN_HldFileSource),-1,0,1,1,1,0,"u 'HldFileSource' - 1 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~HldFileSource",1415,G__HldFileSource_wAHldFileSource_6_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHldFilEvt(void) {
   /* HldFilEvt */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt));
   G__memfunc_setup("HldFilEvt",866,G__HldFilEvt_HldFilEvt_0_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("HldFilEvt",866,G__HldFilEvt_HldFilEvt_1_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt),-1,0,1,1,1,0,"C - - 0 - name",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("init",436,G__HldFilEvt_init_3_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("readSubEvt",1013,G__HldFilEvt_readSubEvt_4_0,98,-1,G__defined_typename("Bool_t"),0,1,1,1,0,"h - 'size_t' 0 - i",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("read",412,G__HldFilEvt_read_5_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("execute",755,G__HldFilEvt_execute_6_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("swap",443,G__HldFilEvt_swap_7_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   // automatic copy constructor
   G__memfunc_setup("HldFilEvt",866,G__HldFilEvt_HldFilEvt_8_0,(int)('i'),G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt),-1,0,1,1,1,0,"u 'HldFilEvt' - 1 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~HldFilEvt",992,G__HldFilEvt_wAHldFilEvt_9_0,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHldRemoteSource(void) {
   /* HldRemoteSource */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource));
   G__memfunc_setup("HldRemoteSource",1525,G__HldRemoteSource_HldRemoteSource_0_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("HldRemoteSource",1525,G__HldRemoteSource_HldRemoteSource_1_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource),-1,0,1,1,1,0,"C - 'Text_t' 0 - nodeName",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("init",436,G__HldRemoteSource_init_3_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("getNextEvent",1249,G__HldRemoteSource_getNextEvent_4_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_EDsState),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("getNodeName",1095,G__HldRemoteSource_getNodeName_5_0,67,-1,G__defined_typename("Text_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileName",1145,G__HldRemoteSource_DeclFileName_6_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileLine",1152,G__HldRemoteSource_DeclFileLine_7_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileName",1171,G__HldRemoteSource_ImplFileName_8_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileLine",1178,G__HldRemoteSource_ImplFileLine_9_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class_Version",1339,G__HldRemoteSource_Class_Version_0_1,115,-1,G__defined_typename("Version_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class",502,G__HldRemoteSource_Class_1_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Dictionary",1046,G__HldRemoteSource_Dictionary_2_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsA",253,G__HldRemoteSource_IsA_3_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("ShowMembers",1132,G__HldRemoteSource_ShowMembers_4_1,121,-1,-1,0,2,1,1,0,
"u 'TMemberInspector' - 1 - insp C - - 0 - parent",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Streamer",835,G__HldRemoteSource_Streamer_5_1,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,1);
   // automatic copy constructor
   G__memfunc_setup("HldRemoteSource",1525,G__HldRemoteSource_HldRemoteSource_6_1,(int)('i'),G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource),-1,0,1,1,1,0,"u 'HldRemoteSource' - 1 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~HldRemoteSource",1651,G__HldRemoteSource_wAHldRemoteSource_7_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHldRemotEvt(void) {
   /* HldRemotEvt */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemotEvt));
   G__memfunc_setup("HldRemotEvt",1102,G__HldRemotEvt_HldRemotEvt_0_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HldRemotEvt),-1,0,1,1,1,0,"C - - 0 - node","init and open rpc connection",(void*)NULL,0);
   G__memfunc_setup("open",434,G__HldRemotEvt_open_2_0,98,-1,G__defined_typename("Bool_t"),0,1,1,1,0,"C - - 0 - pcnode","open connection to server node",(void*)NULL,0);
   G__memfunc_setup("getBuffer",922,G__HldRemotEvt_getBuffer_3_0,98,-1,G__defined_typename("Bool_t"),0,1,1,1,0,"Y - - 0 - handle","buffer",(void*)NULL,0);
   G__memfunc_setup("getLen",607,G__HldRemotEvt_getLen_4_0,104,-1,G__defined_typename("UInt_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("read",412,G__HldRemotEvt_read_5_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("readSubEvt",1013,G__HldRemotEvt_readSubEvt_6_0,98,-1,G__defined_typename("Bool_t"),0,1,1,1,0,"h - 'size_t' 0 - i",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("reGetValue",1012,G__HldRemotEvt_reGetValue_7_0,105,-1,G__defined_typename("Int_t"),0,1,1,1,0,"i - 'Int_t' 0 - iNumber",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("execute",755,G__HldRemotEvt_execute_8_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("swap",443,G__HldRemotEvt_swap_9_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   // automatic copy constructor
   G__memfunc_setup("HldRemotEvt",1102,G__HldRemotEvt_HldRemotEvt_0_1,(int)('i'),G__get_linked_tagnum(&G__DataSourceCintLN_HldRemotEvt),-1,0,1,1,1,0,"u 'HldRemotEvt' - 1 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~HldRemotEvt",1228,G__HldRemotEvt_wAHldRemotEvt_1_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}

static void G__setup_memfuncHRootSource(void) {
   /* HRootSource */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HRootSource));
   G__memfunc_setup("HRootSource",1117,G__HRootSource_HRootSource_0_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_HRootSource),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getNextEvent",1249,G__HRootSource_getNextEvent_2_0,105,G__get_linked_tagnum(&G__DataSourceCintLN_EDsState),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("init",436,G__HRootSource_init_3_0,98,-1,G__defined_typename("Bool_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("getEvent",834,G__HRootSource_getEvent_4_0,98,-1,G__defined_typename("Bool_t"),0,1,1,1,0,"i - 'Int_t' 0 - eventN",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setInput",860,G__HRootSource_setInput_5_0,98,-1,G__defined_typename("Bool_t"),0,2,1,1,0,
"C - 'Text_t' 0 - fileName C - 'Text_t' 0 - treeName",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("deactivateBranch",1640,G__HRootSource_deactivateBranch_6_0,121,-1,-1,0,1,1,1,0,"C - 'Text_t' 0 - branchName",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getTree",720,G__HRootSource_getTree_7_0,85,G__get_linked_tagnum(&G__DataSourceCintLN_TTree),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("getSplitLevel",1348,G__HRootSource_getSplitLevel_8_0,105,-1,G__defined_typename("Int_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Clear",487,G__HRootSource_Clear_9_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileName",1145,G__HRootSource_DeclFileName_0_1,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileLine",1152,G__HRootSource_DeclFileLine_1_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileName",1171,G__HRootSource_ImplFileName_2_1,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileLine",1178,G__HRootSource_ImplFileLine_3_1,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class_Version",1339,G__HRootSource_Class_Version_4_1,115,-1,G__defined_typename("Version_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class",502,G__HRootSource_Class_5_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Dictionary",1046,G__HRootSource_Dictionary_6_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsA",253,G__HRootSource_IsA_7_1,85,G__get_linked_tagnum(&G__DataSourceCintLN_TClass),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("ShowMembers",1132,G__HRootSource_ShowMembers_8_1,121,-1,-1,0,2,1,1,0,
"u 'TMemberInspector' - 1 - insp C - - 0 - parent",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Streamer",835,G__HRootSource_Streamer_9_1,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,1);
   // automatic copy constructor
   G__memfunc_setup("HRootSource",1117,G__HRootSource_HRootSource_0_2,(int)('i'),G__get_linked_tagnum(&G__DataSourceCintLN_HRootSource),-1,0,1,1,1,0,"u 'HRootSource' - 1 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~HRootSource",1243,G__HRootSource_wAHRootSource_1_2,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,1);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncDataSourceCint() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
extern "C" void G__cpp_setup_globalDataSourceCint() {

   /* Setting up global variables */
   G__resetplocal();

   G__memvar_setup((void*)(&catTrack),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catTrack=",0,(char*)NULL);
   G__memvar_setup((void*)(&catInvalid),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catInvalid=",0,(char*)NULL);
   G__memvar_setup((void*)(&catMdcRaw),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catMdcRaw=",0,(char*)NULL);
   G__memvar_setup((void*)(&catMdcHit),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catMdcHit=",0,(char*)NULL);
   G__memvar_setup((void*)(&catMdcGeantRaw),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catMdcGeantRaw=",0,(char*)NULL);
   G__memvar_setup((void*)(&catRichRaw),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catRichRaw=",0,(char*)NULL);
   G__memvar_setup((void*)(&catRichHit),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catRichHit=",0,(char*)NULL);
   G__memvar_setup((void*)(&catShowerRaw),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catShowerRaw=",0,(char*)NULL);
   G__memvar_setup((void*)(&catShowerHit),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catShowerHit=",0,(char*)NULL);
   G__memvar_setup((void*)(&catTofRaw),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catTofRaw=",0,(char*)NULL);
   G__memvar_setup((void*)(&catTofHit),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catTofHit=",0,(char*)NULL);
   G__memvar_setup((void*)(&catTofGeantRaw),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catTofGeantRaw=",0,(char*)NULL);
   G__memvar_setup((void*)(&catMdc),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catMdc=",0,(char*)NULL);
   G__memvar_setup((void*)(&catRich),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catRich=",0,(char*)NULL);
   G__memvar_setup((void*)(&catShower),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catShower=",0,(char*)NULL);
   G__memvar_setup((void*)(&catTof),115,0,1,-1,G__defined_typename("Cat_t"),-1,1,"catTof=",0,(char*)NULL);
   G__memvar_setup((void*)(&rlRaw),105,0,1,-1,G__defined_typename("Int_t"),-1,1,"rlRaw=",0,(char*)NULL);
   G__memvar_setup((void*)(&rlHit),105,0,1,-1,G__defined_typename("Int_t"),-1,1,"rlHit=",0,(char*)NULL);
   G__memvar_setup((void*)(&rlUndefined),105,0,1,-1,G__defined_typename("Int_t"),-1,1,"rlUndefined=",0,(char*)NULL);
   G__memvar_setup((void*)(&ctMatrix),105,0,1,-1,G__defined_typename("Int_t"),-1,1,"ctMatrix=",0,(char*)NULL);
   G__memvar_setup((void*)(&ctSplit),105,0,1,-1,G__defined_typename("Int_t"),-1,1,"ctSplit=",0,(char*)NULL);

   G__resetglobalenv();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
extern "C" void G__cpp_setup_funcDataSourceCint() {
   G__lastifuncposition();


   G__resetifuncposition();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__DataSourceCintLN_TClass = { "TClass" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_TList = { "TList" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_TObject = { "TObject" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_TIterator = { "TIterator" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_istream = { "istream" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_TFile = { "TFile" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HEvent = { "HEvent" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_EDsState = { "EDsState" , 101 , -1 };
G__linked_taginfo G__DataSourceCintLN_HDataSource = { "HDataSource" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HCategory = { "HCategory" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HDataObject = { "HDataObject" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HLocatedDataObject = { "HLocatedDataObject" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HGeantKine = { "HGeantKine" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HGeantReader = { "HGeantReader" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_THashTable = { "THashTable" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HGeantSource = { "HGeantSource" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HldBase = { "HldBase" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HldSubEvt = { "HldSubEvt" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HldUnpack = { "HldUnpack" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HldEvt = { "HldEvt" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HldSource = { "HldSource" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HldFileSource = { "HldFileSource" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HldFilEvt = { "HldFilEvt" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HldRemoteSource = { "HldRemoteSource" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_rpcbuf = { "rpcbuf" , 115 , -1 };
G__linked_taginfo G__DataSourceCintLN_HldRemotEvt = { "HldRemotEvt" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_TTree = { "TTree" , 99 , -1 };
G__linked_taginfo G__DataSourceCintLN_HRootSource = { "HRootSource" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableDataSourceCint() {
  G__DataSourceCintLN_TClass.tagnum = -1 ;
  G__DataSourceCintLN_TList.tagnum = -1 ;
  G__DataSourceCintLN_TObject.tagnum = -1 ;
  G__DataSourceCintLN_TIterator.tagnum = -1 ;
  G__DataSourceCintLN_istream.tagnum = -1 ;
  G__DataSourceCintLN_TFile.tagnum = -1 ;
  G__DataSourceCintLN_HEvent.tagnum = -1 ;
  G__DataSourceCintLN_EDsState.tagnum = -1 ;
  G__DataSourceCintLN_HDataSource.tagnum = -1 ;
  G__DataSourceCintLN_HCategory.tagnum = -1 ;
  G__DataSourceCintLN_HDataObject.tagnum = -1 ;
  G__DataSourceCintLN_HLocatedDataObject.tagnum = -1 ;
  G__DataSourceCintLN_HGeantKine.tagnum = -1 ;
  G__DataSourceCintLN_HGeantReader.tagnum = -1 ;
  G__DataSourceCintLN_THashTable.tagnum = -1 ;
  G__DataSourceCintLN_HGeantSource.tagnum = -1 ;
  G__DataSourceCintLN_HldBase.tagnum = -1 ;
  G__DataSourceCintLN_HldSubEvt.tagnum = -1 ;
  G__DataSourceCintLN_HldUnpack.tagnum = -1 ;
  G__DataSourceCintLN_HldEvt.tagnum = -1 ;
  G__DataSourceCintLN_HldSource.tagnum = -1 ;
  G__DataSourceCintLN_HldFileSource.tagnum = -1 ;
  G__DataSourceCintLN_HldFilEvt.tagnum = -1 ;
  G__DataSourceCintLN_HldRemoteSource.tagnum = -1 ;
  G__DataSourceCintLN_rpcbuf.tagnum = -1 ;
  G__DataSourceCintLN_HldRemotEvt.tagnum = -1 ;
  G__DataSourceCintLN_TTree.tagnum = -1 ;
  G__DataSourceCintLN_HRootSource.tagnum = -1 ;
}

extern "C" void G__cpp_setup_tagtableDataSourceCint() {

   /* Setting up class,struct,union tag entry */
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HDataSource),sizeof(HDataSource),-1,2,"Event's input data manager.",G__setup_memvarHDataSource,G__setup_memfuncHDataSource);
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantKine),sizeof(HGeantKine),-1,0,"Geant KINE event data class",G__setup_memvarHGeantKine,G__setup_memfuncHGeantKine);
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantReader),sizeof(HGeantReader),-1,0,"Base class for the different GEANT readers",G__setup_memvarHGeantReader,G__setup_memfuncHGeantReader);
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HGeantSource),sizeof(HGeantSource),-1,0,"Data source to read GEANT ouput",G__setup_memvarHGeantSource,G__setup_memfuncHGeantSource);
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldBase),sizeof(HldBase),-1,1,(char*)NULL,G__setup_memvarHldBase,G__setup_memfuncHldBase);
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldUnpack),sizeof(HldUnpack),-1,2,"base class for unpack",G__setup_memvarHldUnpack,G__setup_memfuncHldUnpack);
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldSource),sizeof(HldSource),-1,1,"Data source to read LMD data",G__setup_memvarHldSource,G__setup_memfuncHldSource);
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldFileSource),sizeof(HldFileSource),-1,0,"! Data source to read LMD files;",G__setup_memvarHldFileSource,G__setup_memfuncHldFileSource);
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldFilEvt),sizeof(HldFilEvt),-1,0,(char*)NULL,G__setup_memvarHldFilEvt,G__setup_memfuncHldFilEvt);
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemoteSource),sizeof(HldRemoteSource),-1,0,"Data source to read rpc buffers;",G__setup_memvarHldRemoteSource,G__setup_memfuncHldRemoteSource);
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HldRemotEvt),sizeof(HldRemotEvt),-1,0,(char*)NULL,G__setup_memvarHldRemotEvt,G__setup_memfuncHldRemotEvt);
   G__tagtable_setup(G__get_linked_tagnum(&G__DataSourceCintLN_HRootSource),sizeof(HRootSource),-1,0,"Data source to read Root files.",G__setup_memvarHRootSource,G__setup_memfuncHRootSource);
}
extern "C" void G__cpp_setupDataSourceCint() {
  G__check_setup_version(51111,"G__cpp_setupDataSourceCint()");
  G__set_cpp_environmentDataSourceCint();
  G__cpp_setup_tagtableDataSourceCint();

  G__cpp_setup_inheritanceDataSourceCint();

  G__cpp_setup_typetableDataSourceCint();

  G__cpp_setup_memvarDataSourceCint();

  G__cpp_setup_memfuncDataSourceCint();
  G__cpp_setup_globalDataSourceCint();
  G__cpp_setup_funcDataSourceCint();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncDataSourceCint();
  return;
}
class G__cpp_setup_initDataSourceCint {
  public:
    G__cpp_setup_initDataSourceCint() { G__add_setup_func("DataSourceCint",&G__cpp_setupDataSourceCint); }
   ~G__cpp_setup_initDataSourceCint() { G__remove_setup_func("DataSourceCint"); }
};
G__cpp_setup_initDataSourceCint G__cpp_setup_initializerDataSourceCint;

//
// File generated by rootcint at Fri May 21 17:06:50 1999.
// Do NOT change. Changes will be lost next time file is generated
//

#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TError.h"

//______________________________________________________________________________
TBuffer &operator>>(TBuffer &buf, HDataSource *&obj)
{
   // Read a pointer to an object of class HDataSource.

   obj = (HDataSource *) buf.ReadObject(HDataSource::Class());
   return buf;
}

//______________________________________________________________________________
void HDataSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class HDataSource.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
   } else {
      R__b.WriteVersion(HDataSource::IsA());
      TObject::Streamer(R__b);
   }
}

//______________________________________________________________________________
void HDataSource::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
   // Inspect the data members of an object of class HDataSource.

   TClass *R__cl  = HDataSource::IsA();
   Int_t   R__ncp = strlen(R__parent);
   if (R__ncp || R__cl || R__insp.IsA()) { }
   R__insp.Inspect(R__cl, R__parent, "*fEventAddr", &fEventAddr);
   TObject::ShowMembers(R__insp, R__parent);
}

//______________________________________________________________________________
TBuffer &operator>>(TBuffer &buf, HGeantKine *&obj)
{
   // Read a pointer to an object of class HGeantKine.

   obj = (HGeantKine *) buf.ReadObject(HGeantKine::Class());
   return buf;
}

//______________________________________________________________________________
void HGeantKine::Streamer(TBuffer &R__b)
{
   // Stream an object of class HGeantKine.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HLocatedDataObject::Streamer(R__b);
      R__b >> trackNumber;
      R__b >> parentTrack;
      R__b >> particleID;
      R__b >> mediumNumber;
      R__b >> creationMechanism;
      R__b >> xVertex;
      R__b >> yVertex;
      R__b >> zVertex;
      R__b >> xMom;
      R__b >> yMom;
      R__b >> zMom;
      R__b >> generatorInfo;
      R__b >> generatorWeight;
   } else {
      R__b.WriteVersion(HGeantKine::IsA());
      HLocatedDataObject::Streamer(R__b);
      R__b << trackNumber;
      R__b << parentTrack;
      R__b << particleID;
      R__b << mediumNumber;
      R__b << creationMechanism;
      R__b << xVertex;
      R__b << yVertex;
      R__b << zVertex;
      R__b << xMom;
      R__b << yMom;
      R__b << zMom;
      R__b << generatorInfo;
      R__b << generatorWeight;
   }
}

//______________________________________________________________________________
void HGeantKine::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
   // Inspect the data members of an object of class HGeantKine.

   TClass *R__cl  = HGeantKine::IsA();
   Int_t   R__ncp = strlen(R__parent);
   if (R__ncp || R__cl || R__insp.IsA()) { }
   R__insp.Inspect(R__cl, R__parent, "trackNumber", &trackNumber);
   R__insp.Inspect(R__cl, R__parent, "parentTrack", &parentTrack);
   R__insp.Inspect(R__cl, R__parent, "particleID", &particleID);
   R__insp.Inspect(R__cl, R__parent, "mediumNumber", &mediumNumber);
   R__insp.Inspect(R__cl, R__parent, "creationMechanism", &creationMechanism);
   R__insp.Inspect(R__cl, R__parent, "xVertex", &xVertex);
   R__insp.Inspect(R__cl, R__parent, "yVertex", &yVertex);
   R__insp.Inspect(R__cl, R__parent, "zVertex", &zVertex);
   R__insp.Inspect(R__cl, R__parent, "xMom", &xMom);
   R__insp.Inspect(R__cl, R__parent, "yMom", &yMom);
   R__insp.Inspect(R__cl, R__parent, "zMom", &zMom);
   R__insp.Inspect(R__cl, R__parent, "generatorInfo", &generatorInfo);
   R__insp.Inspect(R__cl, R__parent, "generatorWeight", &generatorWeight);
   HLocatedDataObject::ShowMembers(R__insp, R__parent);
}

//______________________________________________________________________________
TBuffer &operator>>(TBuffer &buf, HGeantReader *&obj)
{
   // Read a pointer to an object of class HGeantReader.

   obj = (HGeantReader *) buf.ReadObject(HGeantReader::Class());
   return buf;
}

//______________________________________________________________________________
void HGeantReader::Streamer(TBuffer &R__b)
{
   // Stream an object of class HGeantReader.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> fInputFile;
   } else {
      R__b.WriteVersion(HGeantReader::IsA());
      TObject::Streamer(R__b);
      R__b << fInputFile;
   }
}

//______________________________________________________________________________
void HGeantReader::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
   // Inspect the data members of an object of class HGeantReader.

   TClass *R__cl  = HGeantReader::IsA();
   Int_t   R__ncp = strlen(R__parent);
   if (R__ncp || R__cl || R__insp.IsA()) { }
   R__insp.Inspect(R__cl, R__parent, "*fInputFile", &fInputFile);
   TObject::ShowMembers(R__insp, R__parent);
}

//______________________________________________________________________________
TBuffer &operator>>(TBuffer &buf, HGeantSource *&obj)
{
   // Read a pointer to an object of class HGeantSource.

   obj = (HGeantSource *) buf.ReadObject(HGeantSource::Class());
   return buf;
}

//______________________________________________________________________________
void HGeantSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class HGeantSource.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HDataSource::Streamer(R__b);
      fReaderList.Streamer(R__b);
      fFileTable.Streamer(R__b);
   } else {
      R__b.WriteVersion(HGeantSource::IsA());
      HDataSource::Streamer(R__b);
      fReaderList.Streamer(R__b);
      fFileTable.Streamer(R__b);
   }
}

//______________________________________________________________________________
void HGeantSource::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
   // Inspect the data members of an object of class HGeantSource.

   TClass *R__cl  = HGeantSource::IsA();
   Int_t   R__ncp = strlen(R__parent);
   if (R__ncp || R__cl || R__insp.IsA()) { }
   fReaderList.ShowMembers(R__insp, strcat(R__parent,"fReaderList.")); R__parent[R__ncp] = 0;
   fFileTable.ShowMembers(R__insp, strcat(R__parent,"fFileTable.")); R__parent[R__ncp] = 0;
   HDataSource::ShowMembers(R__insp, R__parent);
}

//______________________________________________________________________________
TBuffer &operator>>(TBuffer &buf, HldUnpack *&obj)
{
   // Read a pointer to an object of class HldUnpack.

   obj = (HldUnpack *) buf.ReadObject(HldUnpack::Class());
   return buf;
}

//______________________________________________________________________________
void HldUnpack::Streamer(TBuffer &R__b)
{
   // Stream an object of class HldUnpack.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
   } else {
      R__b.WriteVersion(HldUnpack::IsA());
      TObject::Streamer(R__b);
   }
}

//______________________________________________________________________________
void HldUnpack::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
   // Inspect the data members of an object of class HldUnpack.

   TClass *R__cl  = HldUnpack::IsA();
   Int_t   R__ncp = strlen(R__parent);
   if (R__ncp || R__cl || R__insp.IsA()) { }
   R__insp.Inspect(R__cl, R__parent, "*pSubEvt", &pSubEvt);
   R__insp.Inspect(R__cl, R__parent, "*pRawCat", &pRawCat);
   TObject::ShowMembers(R__insp, R__parent);
}

//______________________________________________________________________________
TBuffer &operator>>(TBuffer &buf, HldSource *&obj)
{
   // Read a pointer to an object of class HldSource.

   obj = (HldSource *) buf.ReadObject(HldSource::Class());
   return buf;
}

//______________________________________________________________________________
void HldSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class HldSource.

   HDataSource::Streamer(R__b);
}

//______________________________________________________________________________
void HldSource::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
   // Inspect the data members of an object of class HldSource.

   TClass *R__cl  = HldSource::IsA();
   Int_t   R__ncp = strlen(R__parent);
   if (R__ncp || R__cl || R__insp.IsA()) { }
   R__insp.Inspect(R__cl, R__parent, "isDumped", &isDumped);
   R__insp.Inspect(R__cl, R__parent, "*fUnpackerList", &fUnpackerList);
   R__insp.Inspect(R__cl, R__parent, "*fReadEvent", &fReadEvent);
   HDataSource::ShowMembers(R__insp, R__parent);
}

//______________________________________________________________________________
TBuffer &operator>>(TBuffer &buf, HldFileSource *&obj)
{
   // Read a pointer to an object of class HldFileSource.

   obj = (HldFileSource *) buf.ReadObject(HldFileSource::Class());
   return buf;
}

//______________________________________________________________________________
void HldFileSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class HldFileSource.

   HldSource::Streamer(R__b);
}

//______________________________________________________________________________
void HldFileSource::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
   // Inspect the data members of an object of class HldFileSource.

   TClass *R__cl  = HldFileSource::IsA();
   Int_t   R__ncp = strlen(R__parent);
   if (R__ncp || R__cl || R__insp.IsA()) { }
   R__insp.Inspect(R__cl, R__parent, "*iter", &iter);
   HldSource::ShowMembers(R__insp, R__parent);
}

//______________________________________________________________________________
TBuffer &operator>>(TBuffer &buf, HldRemoteSource *&obj)
{
   // Read a pointer to an object of class HldRemoteSource.

   obj = (HldRemoteSource *) buf.ReadObject(HldRemoteSource::Class());
   return buf;
}

//______________________________________________________________________________
void HldRemoteSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class HldRemoteSource.

   HldSource::Streamer(R__b);
}

//______________________________________________________________________________
void HldRemoteSource::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
   // Inspect the data members of an object of class HldRemoteSource.

   TClass *R__cl  = HldRemoteSource::IsA();
   Int_t   R__ncp = strlen(R__parent);
   if (R__ncp || R__cl || R__insp.IsA()) { }
   R__insp.Inspect(R__cl, R__parent, "*currNodeName", &currNodeName);
   R__insp.Inspect(R__cl, R__parent, "*iter", &iter);
   HldSource::ShowMembers(R__insp, R__parent);
}

//______________________________________________________________________________
TBuffer &operator>>(TBuffer &buf, HRootSource *&obj)
{
   // Read a pointer to an object of class HRootSource.

   obj = (HRootSource *) buf.ReadObject(HRootSource::Class());
   return buf;
}

//______________________________________________________________________________
void HRootSource::Streamer(TBuffer &R__b)
{
   // Stream an object of class HRootSource.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      HDataSource::Streamer(R__b);
      R__b >> fInputTree;
      R__b >> fInputFile;
      R__b >> fCursor;
      R__b >> fSplitLevel;
   } else {
      R__b.WriteVersion(HRootSource::IsA());
      HDataSource::Streamer(R__b);
      R__b << fInputTree;
      R__b << fInputFile;
      R__b << fCursor;
      R__b << fSplitLevel;
   }
}

//______________________________________________________________________________
void HRootSource::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
   // Inspect the data members of an object of class HRootSource.

   TClass *R__cl  = HRootSource::IsA();
   Int_t   R__ncp = strlen(R__parent);
   if (R__ncp || R__cl || R__insp.IsA()) { }
   R__insp.Inspect(R__cl, R__parent, "*fInputTree", &fInputTree);
   R__insp.Inspect(R__cl, R__parent, "*fInputFile", &fInputFile);
   R__insp.Inspect(R__cl, R__parent, "fCursor", &fCursor);
   R__insp.Inspect(R__cl, R__parent, "fSplitLevel", &fSplitLevel);
   HDataSource::ShowMembers(R__insp, R__parent);
}

